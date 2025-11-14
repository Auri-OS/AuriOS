#include <stdio.h>
#include <stdint.h>
#include <../includes/gdt.h>

create_descriptor(uint32_t base, uint32_t limit, uint16_t flag)
{
    uint32_t descriptor;

    descriptor  = (limit & 0x000F0000ULL);             // limit 19:16
    descriptor |= ((uint32_t)flag << 8) & 0x00F0FF00ULL; // type + flags
    descriptor |= ((uint32_t)(base >> 16) & 0xFF) << 16; // base 23:16
    descriptor |= ((uint32_t)(base & 0xFF000000));       // base 31:24
    descriptor <<= 32;
    descriptor |= ((uint32_t)base << 16);                // base 15:0
    descriptor |= (limit & 0x0000FFFF);                  // limit 15:0

    return descriptor;
}

int main(void)
{
    create_descriptor(0, 0, 0);
    create_descriptor(0, 0x000FFFFF, (GDT_CODE_PL0));
    create_descriptor(0, 0x000FFFFF, (GDT_DATA_PL0));
    create_descriptor(0, 0x000FFFFF, (GDT_CODE_PL3));
    create_descriptor(0, 0x000FFFFF, (GDT_DATA_PL3));
   
    return 0;
}
