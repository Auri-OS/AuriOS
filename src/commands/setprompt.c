#include "../include/terminal.h"
#include "../include/string.h"
#include "../include/colors.h"
#include "../include/commands.h"

#define NAME_SIZE 64

#define PROMPT_TEMPLATE                                                                   \
  COLOR_WHITE_BRIGHT "[" COLOR_RED_BRIGHT "%user%" COLOR_CYAN_BRIGHT "@" COLOR_WHITE_BRIGHT "%os%" COLOR_RESET \
                   " ~" COLOR_WHITE_BRIGHT "]" COLOR_GREEN_BRIGHT "$ " COLOR_RESET

static char prompt_user[NAME_SIZE] = "root";
static char prompt_os[NAME_SIZE] = "auri-os";

static int prompt_match(const char *s, const char *tok) {
  int i = 0;
  while (tok[i] != '\0') {
    if (s[i] != tok[i])
      return 0;
    i++;
  }
  return 1;
}

void shell_render_prompt(void) {
  const char *fmt = PROMPT_TEMPLATE;
  for (int i = 0; fmt[i] != '\0'; i++) {
    if (prompt_match(&fmt[i], "%user%")) {
      terminal_writestring(prompt_user);
      i += 5;
    } else if (prompt_match(&fmt[i], "%os%")) {
      terminal_writestring(prompt_os);
      i += 3;
    } else {
      terminal_putchar(fmt[i]);
    }
  }
}

void setprompt(void *args, int argc) {
  char **argv = (char **)args;

  if (argc < 2) {
    terminal_writestring("Usage: setprompt <user> <os>\n");
    return;
  }

  strlcpy(prompt_user, argv[1], NAME_SIZE);
  if (argc >= 3)
    strlcpy(prompt_os, argv[2], NAME_SIZE);
}