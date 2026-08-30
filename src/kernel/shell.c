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
#include "../include/commands.h"
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

static void shell_execute(char *cmd) {
  history_push(cmd);
  char *args[MAX_CMD_ARGS + 1]; // +1 for the NULL terminator written by shell_parse
  int argc = shell_parse(cmd, args);
  if (argc == 0)
    return;

  char *cmd_name = args[0]; 

  if (strcmp(cmd_name, "help") == 0) {
    help();
  }
  else if (strcmp(cmd_name, "clear") == 0) {
    terminal_clear();
  }
  else if (strcmp(cmd_name, "fetch") == 0) {
    fetch();
  }
  else if (strcmp(cmd_name, "crash") == 0) {
    crash();
  }
  else if (strcmp(cmd_name, "reboot") == 0) {
    reboot();
  }
  else if (strcmp(cmd_name, "exit") == 0) {
    poweroff();
  }
  else if (strcmp(cmd_name, "uptime") == 0) {
    uptime(args, argc);
  }
  else if (strcmp(cmd_name, "echo") == 0) {
    echo(args, argc);
  }
  else if (strcmp(cmd_name, "memdump") == 0) {
    memdump(args, argc);
  }
  else if (strcmp(cmd_name, "mia") == 0) {
    mia();
  }
  else if (strcmp(cmd_name, "mmap") == 0) {
    mmu_view_mappings();
  }
  else if (strcmp(cmd_name, "peek") == 0) {
    peek(args, argc);
  }
  else if (strcmp(cmd_name, "poke") == 0) {
    poke(args, argc);
  }
  else if (strcmp(cmd_name, "memtest") == 0) {
    memtest();
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