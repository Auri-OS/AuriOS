#include "../include/types.h"
#include "../include/terminal.h"
#include "../include/integer.h"
#include "../include/mm.h"
#include "../include/commands.h"

void peek(void *args, int argc) {
    char **argv = (char **)args;
    if (argc != 2) {
        terminal_writestring("usage: peek <hex address>\n");
        return;
    }
    uint32_t addr = htoi(argv[1]);
    mmu_debug_peek(addr);
}
