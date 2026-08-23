#ifndef SHELL_H
#define SHELL_H

#define cli_nav                                                                          \
  COLOR_RED_BRIGHT "root" COLOR_CYAN_BRIGHT "@" COLOR_WHITE_BRIGHT "auri-os" COLOR_RESET \
                   "~" COLOR_GREEN_BRIGHT "$ " COLOR_RESET
void shell_init(void);
void shell_handle_key(char c);
void shell_history(int a); // a for arrow
void shell_buffer_pos_increment(void);
void shell_buffer_pos_decrement(void);

#endif