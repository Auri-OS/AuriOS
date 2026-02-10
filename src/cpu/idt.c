#include "../include/idt.h"

#define IDT_ENTRY_COUNT 256

struct idt_entry idt[IDT_ENTRY_COUNT];
struct idt_ptr idtp;

extern void idt_flush(uint32_t);

void idt_set_gate(uint8_t num, uint32_t base, uint16_t selector, uint8_t flags)
{
    idt[num].base_low = base & 0xFFFF;
    idt[num].base_high = (base >> 16) & 0xFFFF;
    idt[num].selector = selector;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}

void idt_init(void)
{
    idtp.limit = sizeof(idt) - 1;
    idtp.base = (uint32_t)&idt;

    for (int i = 0; i < IDT_ENTRY_COUNT; i++) {
        idt_set_gate(i, 0, 0, 0);
    }

    idt_flush((uint32_t)&idtp);
}
