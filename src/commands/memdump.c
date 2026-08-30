#include "../include/terminal.h"
#include "../include/integer.h"
#include "../include/mm.h"
#include "../include/commands.h"

void memdump(void *args, int argc) {
    char **argv = (char **)args;
    if (argc != 2) {
        terminal_writestring("usage: memdump <size>\n");
        return;
    }
    int size = atoi(argv[1]);
    if (size < 1 || size > 16384) {
        terminal_writestring("memdump: size must be between 1-16384\n");
        return;
    }
    pmm_dump_bitmap(size);
}
