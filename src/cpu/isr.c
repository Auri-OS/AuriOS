#include "../include/isr.h"
#include "../include/terminal.h"
#include "../include/log.h"
#include <stdbool.h>


static const char *exception_messages[32] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "Bound Range Exceeded",
    "Invalid Opcode",
    "Device Not Available",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack-Segment Fault",
    "General Protection Fault",
    "Page Fault",
    "Reserved",
    "x87 Floating-Point Exception",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating-Point Exception",
    "Virtualization Exception",
    "Control Protection Exception",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Hypervisor Injection Exception",
    "VMM Communication Exception",
    "Security Exception",
    "Reserved"
};

// Helper function to read CR2 register
static inline uintptr_t read_cr2(void)
{
    uintptr_t cr2;
    __asm__ __volatile__("mov %%cr2, %0" : "=r" (cr2));
    return cr2;
}

void isr_handler(registers_t *regs)
{
    if (regs->int_no == 14){
        uintptr_t faulting_address = read_cr2();
        
        bool present_violation = (regs->err_code & 0x1);
        bool write_violation   = (regs->err_code & 0x2);
        bool user_mode         = (regs->err_code & 0x4);

        terminal_writestring("[PANIC] Page fault at ");
        terminal_writehex(faulting_address);
        terminal_writestring("\nReason:");
        terminal_writestring(present_violation ? "Protection-Violation " : "Page-Not-Present ");
        terminal_writestring(write_violation ? "Write-Access " : "Read-Access ");
        terminal_writestring(user_mode ? "User-Mode" : "Supervisor-Mode");
        
        terminal_writestring("\n");
    }
    if (regs->int_no < 32) {
        terminal_writestring("EXCEPTION: ");
        terminal_writestring(exception_messages[regs->int_no]);
        terminal_writestring("\n");
        
        KPANIC(exception_messages[regs->int_no]);
        for (;;) {
          asm volatile("cli; hlt");
        };
    }
}
