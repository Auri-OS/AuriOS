#include <stdint.h>

struct gdt_entry {
    uint16_t limit_low;            // limit bits 0-15
    uint16_t base_low;             // base limit 0-15
    uint8_t base_middle;          // base bits 16-23
    uint8_t access;                // acces flag 
    uint8_t granularity;          // Limit bits 16-19 and flags
    uint8_t base_high;            //  base bits 24-31
} __attribute__((packed));

struct gdt_ptr {
    uint16_t limit;
    uint32_t base;             // address of the first GDT entry 
} __attribute((packed));

struct gdt_entry gdt[5];
struct gdt_ptr gp;

void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
    gdt[num].base_low      = (base & 0xFFFF);
    gdt[num].base_middle   = (base >> 16) & 0xFF;
    gdt[num].base_high     = (base >> 24) & 0xFF;

    gdt[num].limit_low     = (limit & 0xFFFF);
    gdt[num].granularity   = (limit >> 16) & 0x0F;

    gdt[num].granularity   |= (gran & 0xF0);
    gdt[num].access     = access;
}
extern void gdt_flush(uint32_t); // assembly code gdt_flush.asm

void gdt_install()
{
    gp.limit = (sizeof(struct gdt_entry) * 5) - 1;
    gp.base  = (uint32_t)&gdt;

    // Entrée 0: Null
    gdt_set_gate(0, 0, 0, 0, 0);

    // Entrée 1: Code segment (PL0)
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

    // Entrée 2: Data segment (PL0)
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    // Entrée 3: Code segment (PL3)
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);

    // Entrée 4: Data segment (PL3)
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);

    gdt_flush((uint32_t)&gp);
}

