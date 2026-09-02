#ifndef SHELL_H
#define SHELL_H

void shell_init(void);
void shell_handle_key(char c);
void shell_history(int a); // a for arrow
void shell_buffer_pos_increment(void);
void shell_buffer_pos_decrement(void);
void shell_render_prompt(void);

#endif