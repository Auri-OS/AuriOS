#include "../include/timer.h"
#include "../include/isr.h"
#include "../include/io.h"
#include <stdint.h>

static uint32_t tick = 0;
static uint32_t frequency = 0;

static void timer_callback(registers_t* regs)
{
    (void)regs;
    tick++;
}

void timer_init(uint32_t freq)
{
    frequency = freq;
    irq_register_handler(0, timer_callback);
    
    uint32_t divisor = 1193180 / freq;
    
    outb(0x43, 0x36);
    outb(0x40, (uint8_t)(divisor & 0xFF));
    outb(0x40, (uint8_t)((divisor >> 8) & 0xFF));

    uint8_t mask = inb(0x21);
    mask &= ~0x01;
    outb(0x21, mask);
}

void sleep(uint32_t ms)
{
    uint32_t ticks_to_wait = (frequency * ms) / 1000;
    uint32_t start = tick;

    while (tick - start < ticks_to_wait) {
        asm volatile("hlt");
    }
}

uint32_t get_tick(void) {
    return tick;
}