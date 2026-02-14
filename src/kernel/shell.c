#include "../include/shell.h"
#include "../include/terminal.h"
#include "../include/string.h"

#define BUFFER_SIZE 256
static char buffer[BUFFER_SIZE];
static int buffer_pos = 0;


void shell_init(void) {
    terminal_writestring("kernel@auri-os~$ ");
}

static void shell_execute(char *cmd)
{
    if (strcmp(cmd, "help") == 0) {
        terminal_writestring("\nhelp - show this command\n");
        terminal_writestring("about - show informations about AuriOS\n");
        terminal_writestring("clear - clear the terminal\n");
    }
    else if (strcmp(cmd, "clear") == 0) {
        terminal_writestring("Command executed : clear\n");
    }
    else if (strcmp(cmd, "about") == 0) {
        terminal_writestring("        X                 \n");
        terminal_writestring("       XXX                kernel@auri-os\n");
        terminal_writestring("      XXXXX               \n");
        terminal_writestring("     X XXXXX              Kernel: AuriKernel\n");
        terminal_writestring("    XXX XXXXX             Version: 0.2\n");
        terminal_writestring("  XXXXXX  XXXXX           \n");
        terminal_writestring(" XXXXXX    XXXXX          \n");
        terminal_writestring("XXXXXX      XXXXX         \n\n");
        terminal_writestring("Type 'help' for available commands\n\n");
    }
    else if (strcmp(cmd, "") == 0){
        return;
    }
    else {
        return;
    }

}

void shell_handle_key(char c)
{
    if (c == '\n') {
        terminal_putchar('\n');
        buffer[buffer_pos] = '\0';
        shell_execute(buffer);
        buffer_pos = 0;
        terminal_writestring("kernel@auri-os~$ ");
    }
    else if (c == '\b') {
        if (buffer_pos > 0) {
            buffer_pos--;
            terminal_backspace();
        }
    }
    else {
        if (buffer_pos < BUFFER_SIZE - 1) {
            buffer[buffer_pos++] = c;
            terminal_putchar(c);
        }
    }
}