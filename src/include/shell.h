#ifndef SHELL_H
#define SHELL_H

typedef struct {
  char *cmd;
  char **argv;
  int argc;
  char *raw_args;  
} t_cmd_context;

void shell_init(void);
void shell_handle_key(char c);

#endif
