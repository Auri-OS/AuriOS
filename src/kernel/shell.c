#include "../include/shell.h"
#include "../include/terminal.h"
#include "../include/string.h"
#include "../include/timer.h"
#include "../include/integer.h"

#define BUFFER_SIZE 256
static char buffer[BUFFER_SIZE];
static int buffer_pos = 0;


void shell_init(void) {
    terminal_writestring("kernel@auri-os~$ ");
}

static void shell_execute(char* cmd)
{
    int i = 0;
    while (cmd[i] == ' ') i++;
    if (cmd[i] == '\0') return;

    if (strcmp(cmd, "help") == 0) {
        terminal_writestring("\nhelp - show this command\n");
        terminal_writestring("about - show informations about AuriOS\n");
        terminal_writestring("clear - clear the terminal\n");
        terminal_writestring("uptime - show uptime since machine started\n");
        terminal_writestring("crash - make the machine freeze (fun cmd)\n\n");
    }
    else if (strcmp(cmd, "clear") == 0) {
        terminal_clear();
    }
    else if (strcmp(cmd, "about") == 0) {
        terminal_writestring("\n        X                 \n");
        terminal_writestring("       XXX                kernel@auri-os\n");
        terminal_writestring("      XXXXX               \n");
        terminal_writestring("     X XXXXX              Kernel: AuriKernel\n");
        terminal_writestring("    XXX XXXXX             Version: 0.2\n");
        terminal_writestring("   XXXXX XXXXX            Release: 2-14-26\n");
        terminal_writestring("  XXXXXX  XXXXX           \n");
        terminal_writestring(" XXXXXX    XXXXX          \n");
        terminal_writestring("XXXXXX      XXXXX         \n\n");
        terminal_writestring("Type 'help' for available commands\n\n");
    }
    else if (strcmp(cmd, "crash") == 0) {
        asm volatile("cli");
        for (;;) asm volatile("hlt");
    }
    else if (strcmp(cmd, "uptime") == 0) {
        uint32_t ticks = get_tick();
        uint32_t seconds = ticks / 1000;
        uint32_t minutes = seconds / 60;
        uint32_t hours = minutes / 60;
        
        char buf[32];
        terminal_writestring("Current Uptime: ");
        itoa(hours, buf);
        terminal_writestring(buf);
        terminal_writestring("h ");
        itoa(minutes, buf);
        terminal_writestring(buf);
        terminal_writestring("m ");
        itoa(seconds, buf);
        terminal_writestring(buf);
        terminal_writestring("s\n");
    }
    else {
        terminal_writestring("command not found: ");
        terminal_writestring(cmd);
        terminal_putchar('\n');
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