#include "../include/terminal.h"
#include "../include/string.h"
#include "../include/commands.h"

void echo(void *args, int argc) {
    char **argv = (char **)args;
    int j = 1;
    int skip_newline = 0;

    while (j < argc && argv[j][0] == '-') {
        if (strcmp(argv[j], "-n") == 0) {
            skip_newline = 1;
        }
        else if (strcmp(argv[j], "-h") == 0) {
            terminal_writestring("echo - repeats your input to the console\n\n");
            terminal_writestring("-h   - show this command\n");
            terminal_writestring("-n   - do not output the trailing new line");
            terminal_writestring("\n");
            return;
        }
        else {
            // No more recognized args
            break;
        }
        j++;
    }

    while (j < argc) {
        terminal_writestring(argv[j]);
        terminal_writestring(" ");
        j++;
    }

    if (!skip_newline)
        terminal_writestring("\n");
}
