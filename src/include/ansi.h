#ifndef ANSI_H
#define ANSI_H

#include <stdint.h>

#define ANSI_CSI "\x1b["

void ansi_process_char(uint8_t c);

#endif
