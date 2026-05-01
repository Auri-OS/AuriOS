#include "../include/terminal.h"
#include "../include/keyboard.h"
#include "../include/memory.h"
#include "../include/gdt.h"
#include "../include/idt.h"
#include "../include/pic.h"
#include "../include/shell.h"
#include "../include/timer.h"
#include "../include/integer.h"
#include "../include/string.h"
#include "../include/timer.h" 

static void set_cursor_ansi(int x, int y) {
    char buf[16];
    terminal_writestring("\x1b[");
    itoa(y, buf);
    terminal_writestring(buf);
    terminal_writestring(";");
    itoa(x, buf);
    terminal_writestring(buf);
    terminal_writestring("H");
}

void animate_logo(void) {
    const char *logo[] = {
        "                                      X",
        "                                     XXX",
        "                                    XXXXX",
        "                                   X XXXXX",
        "                                  XXX XXXXX",
        "                                 XXXXX XXXXX",
        "                                XXXXXX  XXXXX",
        "                               XXXXXX    XXXXX",
        "                              XXXXXX      XXXXX",
        0 
    };

    terminal_clear();
    terminal_writestring("\x1b[96m");

    int start_y = 6;
    int max_width = 48; 
    int num_rows = 9;

    for (int col = 0; col < max_width; col++) {
        for (int row = num_rows - 1; row >= 0; row--) {
            
            if (col < (int)strlen(logo[row]) && logo[row][col] != ' ') {
                int target_x = col + 1; // ANSI base 1
                int target_y = start_y + row;
                
                for (int y = 1; y <= target_y; y++) {
                    if (y > 1) {
                        set_cursor_ansi(target_x, y - 1);
                        terminal_putchar(' '); 
                    }
                    
                    set_cursor_ansi(target_x, y);
                    terminal_putchar(logo[row][col]);
                    
                    sleep(2); 
                }
            }
        }
    }

    const char *target_text = " Auri-Os - Kernel v0.2 ";
    int text_len = strlen(target_text);
    int text_x = 28;
    int text_y = 17;

    terminal_writestring("\x1b[97;44m");

    for (int frame = 0; frame < 30; frame++) {
        set_cursor_ansi(text_x, text_y);
        
        for (int i = 0; i < text_len; i++) {
            if (frame > i) {
                terminal_putchar(target_text[i]);
            } else {
                uint32_t chaos = get_tick() + (i * 17) + (frame * 31);
                char random_char = 33 + (chaos % 94);
                terminal_putchar(random_char);
            }
        }
        sleep(40);
    }

    set_cursor_ansi(text_x, text_y);
    terminal_writestring(target_text);

    terminal_writestring("\x1b[0m");
    set_cursor_ansi(1, 20);
    sleep(1000);
}

void kernel_main(void) {
    serial_init();
    KINFO("[KRN] Starting AuriOS boot sequence...");
    gdt_init();
    pic_remap();
    idt_init();
    terminal_initialize();
    memory_init();
    timer_init(1000);

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
    animate_logo();
    sleep(2000);
    keyboard_init();
    terminal_clear();
    shell_init();
    KINFO("[KRN] Boot sequence complete. System ready.");

    for (;;) {
        asm volatile("hlt");
    }
}
