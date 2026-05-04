#include "../include/isr.h"
#include "../include/idt.h"
#include "../include/io.h"

static void (*irq_handlers[16])(registers_t *) = { 0 };

void irq_register_handler(int irq, void (*handler)(registers_t *))
{
    irq_handlers[irq] = handler;
}

void irq_handler(registers_t *regs)
{
    if (regs->int_no >= 40)
        outb(0xA0, 0x20);

    outb(0x20, 0x20);

    if (irq_handlers[regs->int_no - 32] != 0)
        irq_handlers[regs->int_no - 32](regs);
}