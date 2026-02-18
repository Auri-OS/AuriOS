#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <stdint.h>
#include "multiboot.h"

void fb_init(multiboot_info_t *mboot);
void fb_put_pixel(uint32_t x, uint32_t y, uint32_t color);
void fb_draw_char(char c, uint32_t x, uint32_t y, uint32_t fg, uint32_t bg);
void fb_clear(uint32_t color);

uint32_t fb_get_width(void);
uint32_t fb_get_height(void);
uintptr_t fb_get_addr(void);
uint32_t fb_get_pitch(void);

#endif