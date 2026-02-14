#include "../include/terminal.h"
#include "../include/keyboard.h"
#include "../include/gdt.h"
#include "../include/idt.h"
#include "../include/pic.h"

void kernel_main(void) {
    gdt_init();
    pic_remap();
    idt_init();
    terminal_initialize();
    keyboard_init();
    terminal_writestring("AuriOS Kernel v0.1\n");
    terminal_writestring("GDT initialized successfully\n");
    terminal_writestring("PIC remapped\n");
    terminal_writestring("IDT initialized with ISR handlers\n");
    terminal_writestring("System ready.\n\n");
    asm volatile("sti");
    for (;;);
}