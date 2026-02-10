#include "../include/terminal.h"
#include "../include/gdt.h"

void kernel_main(void) {
    gdt_init();
    terminal_initialize();
    terminal_writestring("AuriOS Kernel v0.1\n");
    terminal_writestring("GDT initialized successfully\n");
    terminal_writestring("System ready.\n");
    for (;;);
}