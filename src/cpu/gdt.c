#include "../include/gdt.h"

#define GDT_ENTRY_COUNT 5

uint64_t gdt[GDT_ENTRY_COUNT];
struct gdt_ptr gp;

extern void gdt_flush(uint32_t);

uint64_t create_descriptor(uint32_t base, uint32_t limit, uint16_t flag)
{
    uint64_t descriptor;
    descriptor  = (limit & 0x000F0000ULL);
    descriptor |= ((uint32_t)flag << 8) & 0x00F0FF00ULL;
    descriptor |= ((uint32_t)(base >> 16) & 0xFF) << 16;
    descriptor |= ((uint32_t)(base & 0xFF000000));
    descriptor <<= 32;
    descriptor |= ((uint32_t)base << 16);
    descriptor |= (limit & 0x0000FFFF);
    return descriptor;
}

void gdt_init()
{
    gdt[0] = create_descriptor(0, 0, 0);                    // null
    gdt[1] = create_descriptor(0, 0x000FFFFF, GDT_CODE_PL0); // code pl0
    gdt[2] = create_descriptor(0, 0x000FFFFF, GDT_DATA_PL0); // data pl0
    gdt[3] = create_descriptor(0, 0x000FFFFF, GDT_CODE_PL3); // code pl3
    gdt[4] = create_descriptor(0, 0x000FFFFF, GDT_DATA_PL3); // data pl3

    gp.limit = sizeof(gdt) - 1;
    gp.base = (uint32_t)&gdt;

    gdt_flush((uint32_t)&gp);
}
