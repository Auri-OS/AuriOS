#include "../include/keyboard.h"
#include "../include/isr.h"
#include "../include/io.h"
#include "../include/terminal.h"

static char scancode_to_ascii[128] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0,
    0, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0, ' '
};

static char scancode_to_ascii_shift[128] = {
    0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', 0,
    0, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',
    0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0,
    '*', 0, ' '
};

static int shift_pressed = 0;

void keyboard_callback(registers_t *regs)
{
    (void)regs;
    uint8_t scancode = inb(0x60);

        // Shift captured
    if (scancode == 0x2A || scancode == 0x36) {
        shift_pressed = 1;
        return;
    }

    // Shift free
    if (scancode == 0xAA || scancode == 0xB6) {
        shift_pressed = 0;
        return;
    }

    if (scancode & 0x80)
        return;

    // Backspace
    if (scancode == 0x0E) {
        terminal_backspace();
        return;
    }
    char c = scancode_to_ascii[scancode];

    if (shift_pressed)
        c = scancode_to_ascii_shift[scancode];
    else
        c = scancode_to_ascii[scancode];
    if (c)
        terminal_putchar(c);
}

void keyboard_init(void)
{
    irq_register_handler(1, keyboard_callback);
    outb(0x21, 0xFD);
}