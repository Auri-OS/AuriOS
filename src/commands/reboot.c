#include "../include/terminal.h"
#include "../include/io.h"
#include "../include/commands.h"

void reboot(void) {
    terminal_writestring("Rebooting...\n");
    while (inb(0x64) & 0x02)
        ;
    outb(0x64, 0xFE);
    asm volatile("cli; hlt");
}
