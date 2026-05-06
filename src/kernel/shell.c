#include "../include/shell.h"
#include "../include/terminal.h"
#include "../include/string.h"
#include "../include/timer.h"
#include "../include/integer.h"
#include "../include/colors.h"
#include <stdint.h>

#define BUFFER_SIZE 256
static char buffer[BUFFER_SIZE];
static int buffer_pos = 0;
static char cli_nav[58] = COLOR_RED_BRIGHT "kernel" COLOR_CYAN_BRIGHT "@" COLOR_WHITE_BRIGHT "auri-os" COLOR_RESET "~" COLOR_GREEN_BRIGHT "$ " COLOR_RESET;

void shell_init(void) {
    terminal_writestring(cli_nav);
}

static void shell_execute(char* input)
{
    t_cmd_context ctx;
    char *args[1]; // Maximum args
    char input_copy[256]; // We don't have malloc yet

    input = str_trim(input);
    if (input == NULL || input[0] == '\0') return;

    strcpy(input_copy, input);
    
    ctx.cmd = strtok(input_copy, " ");
    if (!ctx.cmd) return;

    ctx.raw_args = input + strlen(ctx.cmd) + 1;
    ctx.raw_args = str_trim(ctx.raw_args);

    ctx.argc = 0;
    while ((args[ctx.argc] = strtok(NULL, " ")) != NULL && ctx.argc < 0) {
        ctx.argc++;
    }
    args[ctx.argc] = NULL;
    ctx.argv = args;
    
    if (strcmp(ctx.cmd, "help") == 0) {
        terminal_writestring("\nhelp - show this command\n");
        terminal_writestring("about - show informations about AuriOS\n");
        terminal_writestring("clear - clear the terminal\n");
        terminal_writestring("uptime - show uptime since machine started\n");
        terminal_writestring("echo - repeats your input to the console\n");
        terminal_writestring("crash - make the machine freeze (fun cmd)\n\n");
    }
    else if (strcmp(ctx.cmd, "clear") == 0) {
        terminal_clear();
    }
    else if (strcmp(ctx.cmd, "about") == 0) {
        terminal_writestring("\n        X                 \n");
        terminal_writestring("       XXX                " COLOR_RED_BRIGHT "kernel" COLOR_CYAN_BRIGHT "@" COLOR_WHITE_BRIGHT "auri-os" COLOR_RESET"\n");
        terminal_writestring("      XXXXX               \n");
        terminal_writestring("     X XXXXX              Kernel: AuriKernel\n");
        terminal_writestring("    XXX XXXXX             Version: 0.2\n");
        terminal_writestring("   XXXXX XXXXX            Release: 2-14-26\n");
        terminal_writestring("  XXXXXX  XXXXX           \n");
        terminal_writestring(" XXXXXX    XXXXX          \n");
        terminal_writestring("XXXXXX      XXXXX         \n\n");
        terminal_writestring("Type 'help' for available commands\n\n");
    }
    else if (strcmp(ctx.cmd, "crash") == 0) {
        asm volatile("cli");
        for (;;) asm volatile("hlt");
    }
    else if (strcmp(ctx.cmd, "uptime") == 0) {
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
    else if (strcmp(ctx.cmd, "echo") == 0) {
        if (ctx.raw_args && ctx.raw_args[0] != '\0') {
            terminal_writestring(ctx.raw_args);
        }
        terminal_writestring("\n");
    }
    else {
        terminal_writestring("command not found: ");
        terminal_writestring(ctx.cmd);
        terminal_putchar('\n');
    }

}

void shell_handle_key(char c)
{
    if (c == 0x0C) {
        terminal_clear();
		buffer_pos = 0;
        shell_init();
        return;
    }
    if (c == '\n') {
        terminal_putchar('\n');
        buffer[buffer_pos] = '\0';
        shell_execute(buffer);
        buffer_pos = 0;
        terminal_writestring(cli_nav);
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
