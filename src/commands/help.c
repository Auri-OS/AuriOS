#include "../include/terminal.h"
#include "../include/commands.h"

void help(void) {
    terminal_writestring("\nhelp  - show this command\n");
    terminal_writestring("fetch   - show informations about AuriOS\n");
    terminal_writestring("clear   - clear the terminal (can be done with CTRL + L)\n");
    terminal_writestring(
        "uptime  - show uptime since machine started\n           -h for options help\n");
    terminal_writestring("memdump - print the PMM Bitmap in the log\n");
    terminal_writestring("memtest - allocate/free on the kernel heap (heap self-test)\n");
    terminal_writestring("mia     - force a Page Fault for MMU testing\n");
    terminal_writestring("mmap    - print current virtual memory mappings\n");
    terminal_writestring("peek    - read and print memory at a given hex address\n");
    terminal_writestring("poke    - write a hex byte at a given hex address\n");
    terminal_writestring("echo    - repeats your input to the console\n");
    terminal_writestring("reboot  - restart the machine\n");
    terminal_writestring("exit    - shut the machine down (QEMU/Bochs)\n");
    terminal_writestring("crash   - make the machine freeze (fun cmd)\n\n");
}
