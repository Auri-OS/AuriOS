const std = @import("std");

const PAGE_SIZE = 4096;

var bitmap: []u8 = undefined;

extern fn serial_write_char(c: u8) void;
extern fn serial_write_string(str: [*c]const u8) void;

pub fn panic(msg: []const u8, error_return_trace: ?*std.builtin.StackTrace, ret_addr: ?usize) noreturn {
    _ = msg;
    _ = error_return_trace;
    _ = ret_addr;

    while (true) {
        asm volatile ("cli; hlt");
    }
}

fn print_hex(val: usize) void {
    serial_write_string("0x");
    var temp = val;
    var buf: [8]u8 = undefined;
    var i: usize = 8;

    while (i > 0) {
        i -= 1;
        const nibble = @as(u8, @truncate(temp & 0xF));
        buf[i] = if (nibble < 10) '0' + nibble else 'A' + (nibble - 10);
        temp >>= 4;
    }

    for (buf) |c| {
        serial_write_char(c);
    }
}

export fn pmm_dump_bitmap(bytes_to_dump: usize) void {
    serial_write_string("\r\n=== PMM BITMAP DUMP ===\r\n");
    serial_write_string("Legend: [#] Used/Reserved  [.] Free\r\n\n");

    const limit = @min(bytes_to_dump, bitmap.len);
    var i: usize = 0;

    while (i < limit) : (i += 1) {
        if (i % 8 == 0) {
            serial_write_string("\r\n");
            const phys_base = i * 8 * PAGE_SIZE;
            print_hex(phys_base);
            serial_write_string(" | ");
        }

        const byte = bitmap[i];
        var bit: u8 = 0;

        while (bit < 8) : (bit += 1) {
            const shift_amount = @as(u3, @intCast(bit));
            const mask = @as(u8, 1) << shift_amount;

            if ((byte & mask) != 0) {
                serial_write_char('#');
            } else {
                serial_write_char('.');
            }
        }
        serial_write_char(' ');
    }
    serial_write_string("\r\n\n=======================\r\n");
}

export fn pmm_init(max_ram_addr: usize, kernel_end_addr: usize) void {
    const total_pages = max_ram_addr / PAGE_SIZE;
    const bitmap_size = total_pages / 8;

    const bitmap_ptr = @as([*]u8, @ptrFromInt(kernel_end_addr));
    bitmap = bitmap_ptr[0..bitmap_size];

    @memset(bitmap, 0xFF);
}

export fn pmm_mark_region_free(base: usize, size: usize) void {
    const start_page = base / PAGE_SIZE;
    const end_page = (base + size) / PAGE_SIZE;

    var i: usize = start_page;
    while (i < end_page) : (i += 1) {
        const byte_idx = i / 8;
        const bit_idx = @as(u3, @truncate(i % 8));

        bitmap[byte_idx] &= ~(@as(u8, 1) << bit_idx);
    }
}

export fn pmm_mark_region_used(base: usize, size: usize) void {
    const start_page = base / PAGE_SIZE;
    const end_page = (base + size) / PAGE_SIZE;

    var i: usize = start_page;
    while (i < end_page) : (i += 1) {
        const byte_idx = i / 8;
        const bit_idx = @as(u3, @truncate(i % 8));

        bitmap[byte_idx] |= (@as(u8, 1) << bit_idx);
    }
}

/// Finds the first available physical page (frame) in the memory.
///
/// How it works:
/// The entire RAM is represented as an array of bytes (the bitmap).
/// 1 bit = 1 page (4096 bytes).
///   - Bit is 1 : Page is USED or RESERVED.
///   - Bit is 0 : Page is FREE.
///
/// Example of a byte in the bitmap: 11101111 (0xEF)
///   [1] [1] [1] [0] [1] [1] [1] [1]
///    |   |   |   |   |   |   |   |
///   p7  p6  p5  p4  p3  p2  p1  p0
///                ^
///             Free page
///
/// To optimize the search, we don't check bits one by one. We check byte by byte.
/// If a byte is 0xFF (11111111), it means ALL 8 pages are FULL. We skip it instantly.
/// If a byte is != 0xFF, it means there is AT LEAST ONE '0' inside. We then scan its bits.
export fn pmm_alloc_frame() usize {
    for (bitmap, 0..) |byte, byte_idx| {
        if (byte != 0xFF) {
            var bit_idx: u3 = 0;
            while (bit_idx < 8) : (bit_idx += 1) {
                const mask = @as(u8, 1) << bit_idx;
                if ((byte & mask) == 0) {
                    const page_idx = (byte_idx * 8) + bit_idx;
                    const phys_addr = page_idx * PAGE_SIZE;
                    pmm_mark_region_used(phys_addr, PAGE_SIZE);
                    return phys_addr;
                }
            }
        }
    }
    return 0;
}

export fn pmm_free_frame(phys_addr: usize) void {
    pmm_mark_region_used(phys_addr, PAGE_SIZE);
}
