#include "../include/framebuffer.h"
#include "../include/font.h"

static uint8_t  *fb_addr   = 0;
static uint32_t  fb_pitch  = 0;
static uint32_t  fb_width  = 0;
static uint32_t  fb_height = 0;
static uint32_t  fb_bpp    = 0;

void fb_init(multiboot_info_t *mboot)
{
    fb_addr   = (uint8_t *)(uint32_t)mboot->framebuffer_addr;
    fb_pitch  = mboot->framebuffer_pitch;
    fb_width  = mboot->framebuffer_width;
    fb_height = mboot->framebuffer_height;
    fb_bpp    = mboot->framebuffer_bpp;
}

uintptr_t fb_get_addr(void) { 
    return (uintptr_t)fb_addr; 
}

uint32_t fb_get_pitch(void) { 
    return fb_pitch; 
}

void fb_put_pixel(uint32_t x, uint32_t y, uint32_t color)
{
    if (x >= fb_width || y >= fb_height)
        return;

    uint32_t offset = y * fb_pitch + x * (fb_bpp / 8);
    *((uint32_t *)(fb_addr + offset)) = color;
}

void fb_draw_char(char c, uint32_t x, uint32_t y, uint32_t fg, uint32_t bg)
{
    const uint8_t *glyph = font_8x16[(uint8_t)c];

    for (uint32_t row = 0; row < 16; row++) {
        for (uint32_t col = 0; col < 8; col++) {
            if (glyph[row] & (0x80 >> col))
                fb_put_pixel(x + col, y + row, fg);
            else
                fb_put_pixel(x + col, y + row, bg);
        }
    }
}

void fb_clear(uint32_t color)
{
    for (uint32_t y = 0; y < fb_height; y++) {
        for (uint32_t x = 0; x < fb_width; x++) {
            fb_put_pixel(x, y, color);
        }
    }
}

uint32_t fb_get_width(void) { return fb_width; }
uint32_t fb_get_height(void) { return fb_height; }