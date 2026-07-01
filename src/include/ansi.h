#ifndef ANSI_H
#define ANSI_H

#include <stdint.h>

#define ANSI_CSI "\x1b["
#define CURSOR_UP ANSI_CSI "A"
#define CURSOR_DOWN ANSI_CSI "B"
#define CURSOR_RIGHT ANSI_CSI "C"
#define CURSOR_LEFT ANSI_CSI "D"

void ansi_process_char(uint8_t c);

#endif
