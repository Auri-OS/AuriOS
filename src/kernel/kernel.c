#include "../include/terminal.h"
#include "../include/keyboard.h"
#include "../include/memory.h"
#include "../include/gdt.h"
#include "../include/idt.h"
#include "../include/pic.h"
#include "../include/shell.h"

void kernel_main(void) {
    gdt_init();
    pic_remap();
    idt_init();
    terminal_initialize();
    memory_init();
    keyboard_init();
    terminal_writestring("AuriOS Kernel v0.2\n");
    terminal_writestring("GDT initialized successfully\n");
    terminal_writestring("PIC remapped\n");
    terminal_writestring("IDT initialized with ISR handlers\n");
    terminal_writestring("Memory initialized\n");
    terminal_writestring("System ready.\n\n");
    terminal_initialize();
    shell_init();
    asm volatile("sti");
    for (;;);
}