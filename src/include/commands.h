#ifndef COMMANDS_H
#define COMMANDS_H

#include "types.h"

void print_unit(uint32_t val, const char *unit, int new_line);

void help(void);
void fetch(void);
void crash(void);
void reboot(void);
void poweroff(void);
void mia(void);
void memtest(void);
void uptime(void *args, int argc);
void echo(void *args, int argc);
void memdump(void *args, int argc);
void peek(void *args, int argc);
void poke(void *args, int argc);
void setprompt(void *args, int argc);

#endif