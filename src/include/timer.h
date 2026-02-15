#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

void timer_init(uint32_t frequency);
void sleep(uint32_t ms);
uint32_t get_tick(void);

#endif