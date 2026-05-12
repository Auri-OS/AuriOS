#include "../include/shell.h"
#include "../include/colors.h"
#include "../include/integer.h"
#include "../include/log.h"
#include "../include/memory.h"
#include "../include/mm.h"
#include "../include/string.h"
#include "../include/terminal.h"
#include "../include/timer.h"

#define BUFFER_SIZE 256
#define MAX_CMD_ARGS 16
#define fetch_user \
  COLOR_RED_BRIGHT "kernel" COLOR_CYAN_BRIGHT "@" COLOR_WHITE_BRIGHT "auri-os" COLOR_RESET


static char buffer[BUFFER_SIZE];
static int buffer_pos = 0;
static char cli_nav[58] =
    COLOR_RED_BRIGHT "kernel" COLOR_CYAN_BRIGHT "@" COLOR_WHITE_BRIGHT "auri-os" COLOR_RESET
                     "~" COLOR_GREEN_BRIGHT "$ " COLOR_RESET;

void shell_init(void) {
  // Flush any keystrokes captured by the keyboard interrupt during boot.
  memset(buffer, 0, BUFFER_SIZE);
  buffer_pos = 0;
  terminal_writestring(cli_nav);
}

static int shell_parse(char *cmd, char **args) {
  int argc = 0;
  int i = 0;

  cmd = str_trim(cmd);
  if (cmd == NULL || cmd[0] == '\0') {
    return 0;
  };

  while (cmd[i] != '\0' && argc < MAX_CMD_ARGS) {
    args[argc++] = &cmd[i];

    while (cmd[i] != '\0' && cmd[i] != ' ') {
      i++;
    }

    if (cmd[i] == ' ') {
      cmd[i++] = '\0';
    }

    while (cmd[i] == ' ') {
      i++;
    }
  }

  args[argc] = NULL;
  return argc;
}

// Show value with it's unit. if newline = 1, then make a new line
void print_unit(uint32_t val, const char *unit, int new_line) {
  char out_buf[32];
  itoa(val, out_buf);
  terminal_writestring(out_buf);
  terminal_writestring(unit);
  if (new_line == 1)
    terminal_writestring("\n");
}

void debug_trigger_page_fault(void) {
  terminal_writestring("\n");
  KINFO("[TEST] Attempting to read unmapped memory at 10 MB...");

  uint32_t *illegal_ptr = (uint32_t *) 0x00A00000;

  volatile uint32_t crash_value = *illegal_ptr;

  (void) crash_value;

  KPANIC("MMU failed to block unmapped memory access!");
}

static void shell_execute(char *cmd) {
  char *args[MAX_CMD_ARGS];
  int argc = shell_parse(cmd, args);
  if (argc == 0)
    return;

  char *cmd_name = args[0];

  if (strcmp(cmd_name, "help") == 0) {
    terminal_writestring("\nhelp  - show this command\n");
    terminal_writestring("fetch   - show informations about AuriOS\n");
    terminal_writestring("clear   - clear the terminal (can be done with CTRL + L)\n");
    terminal_writestring(
        "uptime  - show uptime since machine started\n           -h for options help\n");
    terminal_writestring("memdump - print the PMM Bitmap in the log\n");
    terminal_writestring("mia     - force a Page Fault for MMU testing\n");
    terminal_writestring("mmap    - print current virtual memory mappings\n");
    terminal_writestring("peek    - read and print memory at a given hex address\n");
    terminal_writestring("echo    - repeats your input to the console\n");
    terminal_writestring("crash   - make the machine freeze (fun cmd)\n\n");
  }
  else if (strcmp(cmd_name, "clear") == 0) {
    terminal_clear();
  }
  else if (strcmp(cmd_name, "fetch") == 0) {
    terminal_writestring(
        "\n          " COLOR_WHITE_BRIGHT ".**." COLOR_RESET "                 \n");
    ///
    terminal_writestring("         " COLOR_WHITE_BRIGHT "." COLOR_CYAN_BRIGHT "=" COLOR_BLUE_BRIGHT
                         "###" COLOR_WHITE_BRIGHT "." COLOR_RESET "             " fetch_user "\n");
    ///
    terminal_writestring("        " COLOR_WHITE_BRIGHT "." COLOR_CYAN_BRIGHT "==" COLOR_WHITE_BRIGHT
                         "." COLOR_BLUE_BRIGHT "##%" COLOR_WHITE_BRIGHT "." COLOR_RESET
                         "            --------------\n");
    ///
    terminal_writestring("       " COLOR_WHITE_BRIGHT "." COLOR_CYAN_BRIGHT "===" COLOR_WHITE_BRIGHT
                         "." COLOR_BLUE_BRIGHT "###" COLOR_WHITE_BRIGHT "." COLOR_RESET
                         "            Kernel: AuriKernel\n");
    ///
    terminal_writestring("      " COLOR_WHITE_BRIGHT "." COLOR_CYAN_BRIGHT "=====" COLOR_BLUE_BRIGHT
                         "###" COLOR_WHITE_BRIGHT "." COLOR_RESET "            Version: 0.2.2\n");
    ///
    terminal_writestring("     " COLOR_WHITE_BRIGHT "." COLOR_CYAN_BRIGHT
                         "======" COLOR_WHITE_BRIGHT "." COLOR_BLUE_BRIGHT "###" COLOR_WHITE_BRIGHT
                         "." COLOR_RESET "           Release: M-DY-26\n");
    ///
    terminal_writestring(
        "    " COLOR_WHITE_BRIGHT "." COLOR_CYAN_BRIGHT "======" COLOR_WHITE_BRIGHT
        ".." COLOR_BLUE_BRIGHT "###" COLOR_WHITE_BRIGHT "." COLOR_RESET "         \n");
    ///
    terminal_writestring(
        "   " COLOR_WHITE_BRIGHT ".===.    ." COLOR_BLUE_BRIGHT "###" COLOR_WHITE_BRIGHT
        "." COLOR_RESET "           " bg_color_bright "\n");
    ///
    terminal_writestring(
        "             " COLOR_WHITE_BRIGHT ".***." COLOR_RESET "          " bg_color "\n\n");
    ///
    terminal_writestring("Type 'help' for available commands\n\n");
  }
  else if (strcmp(cmd_name, "crash") == 0) {
    asm volatile("cli");
    for (;;)
      asm volatile("hlt");
  }
  else if (strcmp(cmd_name, "uptime") == 0) {
    uint32_t ticks = get_tick();
    uint32_t total_seconds = ticks / 1000;
    uint32_t seconds = total_seconds % 60;

    uint32_t total_minutes = total_seconds / 60;
    uint32_t minutes = total_minutes % 60;

    uint32_t hours = total_minutes / 60;

    int j = 1;
    int raw = 0;
    int sec = 0;
    int pretty = 0;
    while (j < argc && args[j][0] == '-') {
      if (strcmp(args[j], "-h") == 0) {
        terminal_writestring("uptime - show uptime since machine started\n");
        terminal_writestring("-h     - show this message\n");
        terminal_writestring("-r     - show uptime in miliseconds\n");
        terminal_writestring("-s     - show uptime in seconds\n");
        terminal_writestring("-p     - show uptime in a pretty format\n");
        return;
      }
      else if (strcmp(args[j], "-r") == 0)
        raw = 1;
      else if (strcmp(args[j], "-s") == 0)
        sec = 1;
      else if (strcmp(args[j], "-p") == 0)
        pretty = 1;
      else {
        break;
      }
      j++;
    }

    if (raw == 1) {
      print_unit(ticks, "ms", 1);
    }
    else if (sec == 1) {
      print_unit(total_seconds, "s", 1);
    }
    else if (pretty == 1) {
      terminal_writestring("Current Uptime: \n");
      if (hours != 0)
        print_unit(hours, hours > 1 ? " hours" : " hour", 1);
      print_unit(minutes, minutes > 1 ? " minutes" : " minute", 1);
      print_unit(seconds, seconds > 1 ? " seconds" : " second", 1);
      terminal_writestring("\n");
    }
    else {
      terminal_writestring("Current Uptime: ");
      print_unit(hours, "h ", 0);
      print_unit(minutes, "m ", 0);
      print_unit(seconds, "s", 1);
    }
  }
  else if (strcmp(cmd_name, "echo") == 0) {
    int j = 1;
    int skip_newline = 0;

    while (j < argc && args[j][0] == '-') {
      if (strcmp(args[j], "-n") == 0) {
        skip_newline = 1;
      }
      else if (strcmp(args[j], "-h") == 0) {
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
      terminal_writestring(args[j]);
      terminal_writestring(" ");
      j++;
    }

    if (!skip_newline)
      terminal_writestring("\n");
  }
  else if (strcmp(cmd_name, "memdump") == 0) {
    if (argc != 2) {
      terminal_writestring("usage: memdump <size>\n");
      return;
    }
    pmm_dump_bitmap(atoi(args[1]));
  }
  else if (strcmp(cmd_name, "mia") == 0) {
    debug_trigger_page_fault();
  }
  else if (strcmp(cmd_name, "mmap") == 0) {
    mmu_view_mappings();
  }
  else if (strcmp(cmd_name, "peek") == 0) {
    uint32_t addr = htoi(args[1]);
    mmu_debug_peek(addr);
  }
  else {
    terminal_writestring("command not found: ");
    terminal_writestring(cmd_name);
    terminal_putchar('\n');
  }
}

void shell_handle_key(char c) {
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
