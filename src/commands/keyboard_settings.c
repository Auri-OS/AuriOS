#include "../include/keyboard.h"
#include "../include/commands.h"
#include "../include/terminal.h"
#include "../include/integer.h"
#include "../include/keymap.h"

int max_index = 1;

void keyboard(void *args, int argc) {
    char **argv = (char **)args;
    if (argc != 2) {
        terminal_writestring("usage: keyboard <index>\n");
        terminal_writestring("index 0 : qwerty\n");
        terminal_writestring("index 1 : azerty\n");
        return;
    }

    int index = atoi(argv[1]);
    if (index > max_index) {
        terminal_writestring("index not valid");
        return;
    }

    keymap_set_index(index);
}