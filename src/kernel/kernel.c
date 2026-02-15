#include "../include/terminal.h"
#include "../include/keyboard.h"
#include "../include/memory.h"
#include "../include/gdt.h"
#include "../include/idt.h"
#include "../include/pic.h"
#include "../include/shell.h"
#include "../include/timer.h"

void kernel_main(void) {
    gdt_init();
    pic_remap();
    idt_init();
    terminal_initialize();
    memory_init();
    timer_init(1000);
    keyboard_init();

    asm volatile("sti");

    terminal_writestring("AuriOS Kernel v0.2\n");
    sleep(100);
    terminal_writestring("GDT initialized successfully\n");
    sleep(50);
    terminal_writestring("PIC remapped\n");
    sleep(30);
    terminal_writestring("IDT initialized with ISR handlers\n");
    sleep(60);
    terminal_writestring("Memory initialized\n");
    sleep(120);
    terminal_writestring("System ready.\n\n");
    sleep(1000);
    terminal_clear();
    terminal_writestring("\n\n\n\n\n\n\n                                      X                 \n");
    terminal_writestring("                                     XXX                \n");
    terminal_writestring("                                    XXXXX               \n");
    terminal_writestring("                                   X XXXXX              \n");
    terminal_writestring("                                  XXX XXXXX             \n");
    terminal_writestring("                                 XXXXX XXXXX            \n");
    terminal_writestring("                                XXXXXX  XXXXX           \n");
    terminal_writestring("                               XXXXXX    XXXXX          \n");
    terminal_writestring("                              XXXXXX      XXXXX         \n\n");
    terminal_writestring("                            Auri-Os - Kernel v0.2\n\n");
    sleep(1500);
    terminal_clear();
    shell_init();

    for (;;) {
        asm volatile("hlt");
    }
}