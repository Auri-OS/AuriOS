#include "../include/types.h"
#include "../include/terminal.h"
#include "../include/memory.h"
#include "../include/string.h"
#include "../include/commands.h"

void memtest(void) {
    char *a = malloc(32);
    char *b = malloc(100);
    strlcpy(a, "heap ok", 8);
    terminal_writestring("\nalloc a: ");
    terminal_writestring(a);
    free(a);
    char *c = malloc(16);
    terminal_writestring("\nreuse : ");
    print_unit((uint32_t) (uintptr_t) c, "(addr)", 1);
    free(b);
    free(c);
}
