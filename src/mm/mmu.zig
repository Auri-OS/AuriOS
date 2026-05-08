const std = @import("std");

extern fn pmm_alloc_frame() usize;
extern fn serial_write_string(str: [*c]const u8) void;
extern fn print_hex(val: usize) void;

const PAGE_SIZE = 4096;

const PageTableEntry = packed struct {
    present: u1 = 0, // page is present in RAM ? present == 0 and we try to read it -> page fault
    rw: u1 = 0, // 0 = read only, 1 = read & write
    user: u1 = 0, // 0 = kernel only, 1 = user (Ring 3)
    write_through: u1 = 0,
    cache_disable: u1 = 0,
    accessed: u1 = 0,
    dirty: u1 = 0,
    pat: u1 = 0,
    global: u1 = 0,
    available: u3 = 0, // Free bits for OS
    physical_frame: u20 = 0, // 20 high bits of physical address
};

const PageDirectoryEntry = packed struct {
    present: u1 = 0,
    rw: u1 = 0,
    user: u1 = 0,
    write_through: u1 = 0,
    cache_disable: u1 = 0,
    accessed: u1 = 0,
    ignored: u1 = 0,
    page_size: u1 = 0, // 0 = 4 Ko pages, 1 = 4 Mo pages
    global: u1 = 0,
    available: u3 = 0,
    pt_frame: u20 = 0, // Physical address of the page table
};

const PageTable = [1024]PageTableEntry;
const PageDirectory = [1024]PageDirectoryEntry;

var kernel_directory: *PageDirectory = undefined;

export fn mmu_init() void {
    const pd_addr = pmm_alloc_frame();
    kernel_directory = @as(*PageDirectory, @ptrFromInt(pd_addr));

    @memset(@as([*]u8, @ptrCast(kernel_directory))[0..PAGE_SIZE], 0);
    const pt_addr = pmm_alloc_frame();
    const first_table = @as(*PageTable, @ptrFromInt(pt_addr));
    @memset(@as([*]u8, @ptrCast(first_table))[0..PAGE_SIZE], 0);

    var i: u32 = 0;
    while (i < 1024) : (i += 1) {
        first_table[i] = PageTableEntry{
            .present = 1,
            .rw = 1, // kernel can write
            .user = 0, // libOS and User cannot access to this area
            .physical_frame = @as(u20, @truncate(i)),
        };
    }

    kernel_directory[0] = PageDirectoryEntry{
        .present = 1,
        .rw = 1,
        .user = 0,
        .pt_frame = @as(u20, @truncate(pt_addr / PAGE_SIZE)),
    };
}

export fn mmu_enable() void {
    const ptr_phys_addr = @intFromPtr(kernel_directory);

    asm volatile (
        \\ mov %[addr], %%cr3
        \\ mov %%cr0, %%eax
        \\ or $0x80000000, %%eax
        \\ mov %%eax, %%cr0
        :
        : [addr] "r" (ptr_phys_addr),
        : .{ .eax = true });
}

export fn mmu_is_paging_enabled() u8 {
    var cr0: u32 = undefined;

    asm volatile ("mov %%cr0, %[val]"
        : [val] "=r" (cr0),
    );

    if ((cr0 & 0x80000000) != 0) {
        return 1;
    } else {
        return 0;
    }
}

export fn mmu_map_page(phys_addr: usize, virt_addr: usize, is_writeable: bool, is_user: bool) void {
    // virtual address is splitted like
    // [10 bits Directory][10 bits Table][12 bits Offset]
    const pd_index = virt_addr >> 22;
    const pt_index = (virt_addr >> 12) & 0x3FF;

    const pde = &kernel_directory[pd_index];
    var table_ptr: *PageTable = undefined;

    if (pde.present == 0) {
        // page does not already exist for this 4Mo Area
        // so we ask PMM for a new one
        const new_pt_phys = pmm_alloc_frame();
        table_ptr = @as(*PageTable, @ptrFromInt(new_pt_phys));

        // clean the new table
        @memset(@as([*]u8, @ptrCast(table_ptr))[0..PAGE_SIZE], 0);

        // register it in Directory
        pde.* = PageDirectoryEntry{
            .present = 1,
            .rw = if (is_writeable) 1 else 0,
            .user = if (is_user) 1 else 0,
            .pt_frame = @as(u20, @truncate(new_pt_phys / PAGE_SIZE)),
        };
    } else {
        // already exist so we just grab it address
        const pt_phys = @as(usize, pde.pt_frame) * PAGE_SIZE;
        table_ptr = @as(*PageTable, @ptrFromInt(pt_phys));
    }

    table_ptr[pt_index] = PageTableEntry{
        .present = 1,
        .rw = if (is_writeable) 1 else 0,
        .user = if (is_user) 1 else 0,
        .physical_frame = @as(u20, @truncate(phys_addr / PAGE_SIZE)),
    };

    asm volatile ("invlpg (%[addr])"
        :
        : [addr] "r" (virt_addr),
        : .{ .memory = true });
}

// ================
// DEBUG
// ===============

export fn mmu_get_fault_address() usize {
    var addr: usize = undefined;
    asm volatile ("mov %%cr2, %[val]"
        : [val] "=r" (addr),
    );
    return addr;
}

export fn mmu_view_mappings() void {
    serial_write_string("\r\n=== ACTIVE MEMORY MAPPINGS ===\r\n");

    var i: usize = 0;
    while (i < 1024) : (i += 1) {
        const pde = kernel_directory[i];
        if (pde.present == 1) {
            const start_addr = i * 4 * 1024 * 1024;
            const end_addr = start_addr + (4 * 1024 * 1024) - 1;

            serial_write_string("Range: ");
            print_hex(start_addr);
            serial_write_string(" - ");
            print_hex(end_addr);

            if (pde.user == 1) {
                serial_write_string(" [USER] ");
            } else {
                serial_write_string(" [KERNEL] ");
            }

            if (pde.rw == 1) {
                serial_write_string("R/W\r\n");
            } else {
                serial_write_string("R-O\r\n");
            }
        }
    }
    serial_write_string("===============================\r\n");
}

export fn mmu_debug_peek(addr: usize) void {
    serial_write_string("\r\n[DEBUG] Peeking at ");
    print_hex(addr);
    serial_write_string("...\r\n");

    const ptr = @as(*volatile u8, @ptrFromInt(addr));
    const value = ptr.*;

    serial_write_string("Value found: ");
    print_hex(value);
    serial_write_string("\r\n");
}

export fn mmu_handle_page_fault(error_code: u32) noreturn {
    var cr2: usize = undefined;

    asm volatile ("mov %%cr2, %[val]"
        : [val] "=r" (cr2),
    );

    serial_write_string("\r\n\x1b[91m[PANIC] PAGE FAULT DETECTED!\x1b[0m\r\n");
    serial_write_string("Faulting Address : ");
    print_hex(cr2);
    serial_write_string("\r\n");

    serial_write_string("Reason           : ");
    if ((error_code & 1) == 0) {
        serial_write_string("Page not present (Unmapped memory)");
    } else {
        serial_write_string("Protection violation (Access denied)");
    }

    if ((error_code & 2) != 0) {
        serial_write_string(" on WRITE attempt\r\n");
    } else {
        serial_write_string(" on READ attempt\r\n");
    }

    while (true) {
        asm volatile ("cli; hlt");
    }
}
