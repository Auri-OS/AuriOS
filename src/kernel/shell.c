#include "../include/shell.h"
#include "../include/colors.h"
#include "../include/fetch.h"
#include "../include/integer.h"
#include "../include/io.h"
#include "../include/log.h"
#include "../include/memory.h"
#include "../include/mm.h"
#include "../include/string.h"
#include "../include/terminal.h"
#include "../include/timer.h"
#include "../include/history.h"
#define BUFFER_SIZE 256
#define MAX_CMD_ARGS 16
#define MAX_HISTORY_SIZE 16


static char buffer[BUFFER_SIZE];
static int buffer_pos = 0;

static void command_completion(void);
static void shell_insert_completion(const char *text, int len, int add_space);

static const char *command_list[] = {
    "help", "fetch", "clear", "uptime", "memdump", "memtest", "mia",
    "mmap", "peek",  "poke",  "echo",  "reboot",  "exit",   "crash", NULL};

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
  history_push(cmd);
  char *args[MAX_CMD_ARGS + 1]; // +1 for the NULL terminator written by shell_parse
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
    terminal_writestring("memtest - allocate/free on the kernel heap (heap self-test)\n");
    terminal_writestring("mia     - force a Page Fault for MMU testing\n");
    terminal_writestring("mmap    - print current virtual memory mappings\n");
    terminal_writestring("peek    - read and print memory at a given hex address\n");
    terminal_writestring("poke    - write a hex byte at a given hex address\n");
    terminal_writestring("echo    - repeats your input to the console\n");
    terminal_writestring("reboot  - restart the machine\n");
    terminal_writestring("exit    - shut the machine down (QEMU/Bochs)\n");
    terminal_writestring("crash   - make the machine freeze (fun cmd)\n\n");
  }
  else if (strcmp(cmd_name, "clear") == 0) {
    terminal_clear();
  }
  /*
  To modify the info strings displayed to the right of the fetch ASCII art,
  edit src/include/fetch.h
  */
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
                         "            " fetch_kernel_name "\n");
    ///
    terminal_writestring(
        "      " COLOR_WHITE_BRIGHT "." COLOR_CYAN_BRIGHT "=====" COLOR_BLUE_BRIGHT
        "###" COLOR_WHITE_BRIGHT "." COLOR_RESET "            " fetch_version "\n");
    ///
    terminal_writestring("     " COLOR_WHITE_BRIGHT "." COLOR_CYAN_BRIGHT
                         "======" COLOR_WHITE_BRIGHT "." COLOR_BLUE_BRIGHT "###" COLOR_WHITE_BRIGHT
                         "." COLOR_RESET "           " fetch_release_date "\n");
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
  else if (strcmp(cmd_name, "reboot") == 0) {
    terminal_writestring("Rebooting...\n");
    while (inb(0x64) & 0x02)
      ;
    outb(0x64, 0xFE);
    asm volatile("cli; hlt");
  }
  else if (strcmp(cmd_name, "exit") == 0) {
    terminal_writestring("Powering off...\n");
    outw(0x604, 0x2000);
    outw(0xB004, 0x2000);
    asm volatile("cli; hlt");
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
    if (argc != 2) {
      terminal_writestring("usage: peek <hex address>\n");
      return;
    }
    uint32_t addr = htoi(args[1]);
    mmu_debug_peek(addr);

  }
  else if (strcmp(cmd_name, "poke") == 0) {
    if (argc != 3) {
      terminal_writestring("usage: poke <hex address> <hex value>\n");
      return;
    }
    uint32_t addr = htoi(args[1]);
    uint8_t value = (uint8_t) htoi(args[2]);
    mmu_debug_poke(addr, value);
  }
  else if (strcmp(cmd_name, "memtest") == 0) {
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
  } else {
    terminal_writestring("command not found: ");
    terminal_writestring(cmd_name);
    terminal_putchar('\n');
  } 
}

static int command_recognition(const char *a, const char *b) {
  int i = 0;
  while (a[i] && b[i] && a[i] == b[i])
    i++;
  return i;
}

static void command_completion(void) {
  int i = 0;
  int prefix_len = buffer_pos;
  const char *matches[32];
  int n = 0;
  int lcp = -1;

  while (i < buffer_pos) 
    if (buffer[i++] == ' ')
      return;

  i = 0;

  while (command_list[i] != NULL) {
    if (strncmp(command_list[i], buffer, prefix_len) == 0) {
      if (n < 32) 
        matches[n++] = command_list[i];
      if (lcp < 0)
        lcp = (int) strlen(command_list[i]);
      else
        lcp = command_recognition(matches[0], command_list[i]) < lcp ? command_recognition(matches[0], command_list[i]) : lcp;
    }
    i++;
  }
if (n == 0) 
  return;

if (n == 1) {
  shell_insert_completion(matches[0], (int) strlen(matches[0]), 1);
  return;
}

if (lcp > prefix_len) {
  shell_insert_completion(matches[0], lcp, 0);
  return;
}

terminal_putchar('\n');
for (i = 0; i < n; i++) {
  terminal_writestring(matches[i]);
  terminal_writestring("  ");
}
terminal_putchar('\n');
terminal_writestring(cli_nav);
terminal_writestring(buffer);

}

static void shell_insert_completion(const char *text, int len, int add_space) {
  int i = buffer_pos;
  while (i < len) {
    if (i >= BUFFER_SIZE - 2) break;
    char ch = text[i];
    buffer[i] = ch;
    terminal_putchar(ch);
    i++;
  }

  buffer_pos = len;
  buffer[buffer_pos] = '\0';

  if (add_space && buffer_pos < BUFFER_SIZE - 1) {
    buffer[buffer_pos] = ' ';
    buffer_pos++;
    buffer[buffer_pos] = '\0';
    terminal_putchar(' ');
  }
}

void shell_handle_key(char c) {
  if (c == 0x0C) {
    terminal_clear();
    buffer_pos = 0;
    shell_init();
    return;
  } if (c == '\n') {
    terminal_putchar('\n');
    shell_execute(buffer);
    buffer[0] = '\0';
    buffer_pos = 0;
    terminal_writestring(cli_nav);
  } else if (c == '\b') {
    if (buffer_pos > 0) {
      int len = (int) strlen(buffer);
      int tail = len - buffer_pos; 
      for (int x = buffer_pos; x < len; x++) buffer[x - 1] = buffer[x];
      buffer[len - 1] = '\0';
      buffer_pos--;
      terminal_backspace();
      terminal_writestring(&buffer[buffer_pos]);
      terminal_putchar(' ');
      terminal_move_cursor(-(tail + 1));
    }
  } else if (c == '\t') {
    command_completion();
  } else {
    int len = (int) strlen(buffer);

    if(len < BUFFER_SIZE - 1) {
      int tail = len - buffer_pos;

      for (int x = len; x > buffer_pos; x--)
        buffer[x] = buffer[x - 1];

      buffer[buffer_pos] = c;
      buffer[len + 1] = '\0';
      terminal_writestring(&buffer[buffer_pos]);
      buffer_pos++;
      terminal_move_cursor(-tail);
    }
  }
}

void shell_history(int a) {
	int old_len = (int) strlen(buffer);
	terminal_move_cursor(old_len - buffer_pos);

	if (a == 1) history_prev(buffer);
	else        history_next();

	for (int x = 0; x < old_len; x++) terminal_backspace();

	strlcpy(buffer, history_getcurrentcommand(), BUFFER_SIZE);
	buffer_pos = strlen(buffer);

	terminal_writestring(buffer);
}

void shell_buffer_pos_decrement(void) {
  if (buffer_pos <= 0) return;
  buffer_pos--;
  terminal_move_cursor(-1);
}

void shell_buffer_pos_increment(void) {
  if (buffer_pos >= (int) strlen(buffer)) return;
  buffer_pos++;
  terminal_move_cursor(1);
}