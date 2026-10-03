#include "../include/isr.h"
#include "../include/terminal.h"
#include "../include/log.h"

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

extern uint8_t mmu_handle_page_fault(uint32_t error_code);

void isr_handler(registers_t *regs)
{
    if (regs->int_no < 32) {
        if (regs->int_no == 14 && mmu_handle_page_fault(regs->err_code)) {
            return;
        }

        terminal_writestring("EXCEPTION: ");
        terminal_writestring(exception_messages[regs->int_no]);
        terminal_writestring("\n");
        KPANIC(exception_messages[regs->int_no]);
        for (;;) {
          asm volatile("cli; hlt");
        };
    }
}
