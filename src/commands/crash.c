#include "../include/commands.h"

void crash(void) {
    asm volatile("cli");
    for (;;)
        asm volatile("hlt");
}
