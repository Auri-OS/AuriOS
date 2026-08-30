#include "../include/types.h"
#include "../include/terminal.h"
#include "../include/log.h"
#include "../include/commands.h"

void mia(void) {
    terminal_writestring("\n");
    KINFO("[TEST] Attempting to read unmapped memory at 10 MB...");

    uint32_t *illegal_ptr = (uint32_t *) 0x00A00000;

    volatile uint32_t crash_value = *illegal_ptr;

    (void) crash_value;

    KPANIC("MMU failed to block unmapped memory access!");
}
