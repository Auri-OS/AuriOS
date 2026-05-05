#ifndef PIC_H
#define PIC_H
#include <stdint.h>
void pic_remap(void);
void pic_unmask_irq(uint8_t irqline);

#endif