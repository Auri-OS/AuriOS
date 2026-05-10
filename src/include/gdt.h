#ifndef GDT_H
#define GDT_H

#include <stdint.h>

/* Flags */
#define SEG_DESCTYPE(x)  ((x) << 0x04)
#define SEG_PRES(x)      ((x) << 0x07)
#define SEG_SAVL(x)      ((x) << 0x0C)
#define SEG_LONG(x)      ((x) << 0x0D)
#define SEG_SIZE(x)      ((x) << 0x0E)
#define SEG_GRAN(x)      ((x) << 0x0F)
#define SEG_PRIV(x)     (((x) &  0x03) << 0x05)

/*  Segment Types  */
#define SEG_DATA_RD        0x00
#define SEG_DATA_RDWR      0x02
#define SEG_CODE_EX        0x08
#define SEG_CODE_EXRD      0x0A

/* Predefined Segment Flags */
#define GDT_CODE_PL0 SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | \
                     SEG_LONG(0) | SEG_SIZE(1) | SEG_GRAN(1) | SEG_PRIV(0) | SEG_CODE_EXRD

#define GDT_DATA_PL0 SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | \
                     SEG_LONG(0) | SEG_SIZE(1) | SEG_GRAN(1) | SEG_PRIV(0) | SEG_DATA_RDWR

#define GDT_CODE_PL3 SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | \
                     SEG_LONG(0) | SEG_SIZE(1) | SEG_GRAN(1) | SEG_PRIV(3) | SEG_CODE_EXRD

#define GDT_DATA_PL3 SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | \
                     SEG_LONG(0) | SEG_SIZE(1) | SEG_GRAN(1) | SEG_PRIV(3) | SEG_DATA_RDWR

/* Structures */
struct __attribute__((packed)) gdt_ptr {
    uint16_t limit;
    uint32_t base;
};


struct tss_entry {
    uint32_t prev_tss;
    uint32_t esp0;
    uint32_t ss0;
    uint32_t esp1;
    uint32_t ss1;
    uint32_t esp2;
    uint32_t ss2;
    uint32_t cr3;
    uint32_t eip, eflags, eax, ecx, edx, ebx, esp, ebp, esi, edi;
    uint32_t es, cs, ss, ds, fs, gs;
    uint32_t ldt;
    uint16_t trap;
    uint16_t iomap_base;
} __attribute__((packed));


uint64_t create_descriptor(uint32_t base, uint32_t limit, uint16_t flag);
void gdt_init(void);

#endif
