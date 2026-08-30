#include "../include/terminal.h"
#include "../include/io.h"
#include "../include/commands.h"

void poweroff(void) {
    terminal_writestring("Powering off...\n");
    outw(0x604, 0x2000);
    outw(0xB004, 0x2000);
    asm volatile("cli; hlt");
}
