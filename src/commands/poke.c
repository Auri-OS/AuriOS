#include "../include/types.h"
#include "../include/terminal.h"
#include "../include/integer.h"
#include "../include/mm.h"
#include "../include/commands.h"

void poke(void *args, int argc) {
    char **argv = (char **)args;
    if (argc != 3) {
        terminal_writestring("usage: poke <hex address> <hex value>\n");
        return;
    }
    uint32_t addr = htoi(argv[1]);
    uint8_t value = (uint8_t) htoi(argv[2]);
    mmu_debug_poke(addr, value);
}
