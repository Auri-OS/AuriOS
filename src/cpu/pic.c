#include "../include/pic.h"
#include "../include/io.h"
#include "../include/log.h"

#define PIC1_COMMAND 0x20
#define PIC1_DATA    0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA    0xA1

void pic_remap(void)
{
    outb(PIC1_COMMAND, 0x11);
    io_wait();
    outb(PIC2_COMMAND, 0x11);
    io_wait();

    outb(PIC1_DATA, 0x20);
    io_wait();
    outb(PIC2_DATA, 0x28);
    io_wait();

    outb(PIC1_DATA, 0x04);
    io_wait();
    outb(PIC2_DATA, 0x02);
    io_wait();

    outb(PIC1_DATA, 0x01);
    io_wait();
    outb(PIC2_DATA, 0x01);
    io_wait();

    outb(PIC1_DATA, 0xFF);
    outb(PIC2_DATA, 0xFF);
    KINFO("[PIC] IRQs remapped to 32-47");
}

void pic_unmask_irq(uint8_t irqline)
{
    uint16_t port;
    uint8_t value;

    // Si l'IRQ est entre 0-7, PIC Master (0x21)
    // else, 8-15, PIC Slave (0xA1)
    if (irqline < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        irqline -= 8;
    }

    value = inb(port) & ~(1 << irqline);
    outb(port, value);
}
