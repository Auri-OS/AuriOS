#ifndef HISTORY_H
#define HISTORY_H

void history_init(void);
void history_push(const char *cmd);
const char *history_prev(const char *current);
const char *history_next(void);
const char *history_getcurrentcommand(void);
int history_getcursor(void);
int history_getcount(void);
int history_gethead(void);

#endif