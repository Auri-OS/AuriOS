#include "../include/keyboard.h"
#include "../include/io.h"
#include "../include/terminal.h"
#include "../include/shell.h"
#include "../include/log.h"
#include "../include/history.h"
#include "../include/integer.h"

static Keys keys_table[256] = {
    // pressed
    KEY_NOTHING, 
    KEY_ESCAPE,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,
    KEY_0,
    KEY_SUB,
    KEY_EQ,
    KEY_BACKSPACE,
    KEY_TAB,
    KEY_Q,
    KEY_W,
    KEY_E,
    KEY_R,
    KEY_T,
    KEY_Y,
    KEY_U,
    KEY_I,
    KEY_O,
    KEY_P,
    KEY_LBRACKET,
    KEY_RBRACKET,
    KEY_ENTER,
    KEY_CONTROL,
    KEY_A,
    KEY_S,
    KEY_D,
    KEY_F,
    KEY_G,
    KEY_H,
    KEY_J,
    KEY_K,
    KEY_L,
    KEY_SEMICOLON,
    KEY_SINGLE_QUOTE,
    KEY_BACK_TICK,
    KEY_SHIFT,
    KEY_ANTISLASH,
    KEY_Z,
    KEY_X,
    KEY_C,
    KEY_V,
    KEY_B,
    KEY_N,
    KEY_M,
    KEY_COMMA,
    KEY_DOT,
    KEY_SLASH,
    KEY_SHIFT,
    KEY_NOTHING, // * on keypad
    KEY_ALT,
    KEY_SPACE,
    KEY_CAPS_LOCK,
    KEY_F1,
    KEY_F2,
    KEY_F3,
    KEY_F4,
    KEY_F5,
    KEY_F6,
    KEY_F7,
    KEY_F8,
    KEY_F9,
    KEY_F10,
    KEY_NOTHING, //NumberLock
    KEY_NOTHING, // ScrollLock
    KEY_NOTHING, // keypad 7
    KEY_NOTHING, // keypad 8,
    KEY_NOTHING, // keypad 9,
    KEY_NOTHING, // Keypad -
    KEY_NOTHING, // keypad 4,
    KEY_NOTHING, // keypad 5,
    KEY_NOTHING, // Keypad 6,
    KEY_NOTHING, // keypad +
    KEY_NOTHING, // Keypad 1
    KEY_NOTHING, // keypad 2
    KEY_NOTHING, // keypad 3
    KEY_NOTHING, // keypad 0,
    KEY_NOTHING, // keypad .
    KEY_NOTHING, // Literaly nothing is written in osdev 
    KEY_NOTHING, // Literaly nothing is written in osdev 
    KEY_NOTHING, // Literaly nothing is written in osdev 
    KEY_F11,
    KEY_F12,
};
uint8_t extended = 0;

Key keyboard_get_current_key() {
    Key k;
    uint8_t scancode = inb(0x60);

    if (scancode == 0xe0) {
        extended = 1;
        k.key = KEY_NOTHING;
        k.state = 2;
        return k;
    }
    if (extended) {
        extended = 0;
        if (scancode == 0x48 || scancode == 0xc8) {
            k.key = KEY_ARROW_UP;
            k.state = scancode == 0x48 ? PRESSED : RELEASED;
        } else if (scancode == 0x50 || scancode == 0xd0) {
            k.key = KEY_ARROW_DOWN;
            k.state = scancode == 0x50 ? PRESSED : RELEASED;
        } else if (scancode == 0x4b || scancode == 0xcb) {
            k.key = KEY_ARROW_LEFT;
            k.state = scancode == 0x4b ? PRESSED : RELEASED;
        } else if (scancode == 0x4d || scancode == 0xcd) {
            k.key = KEY_ARROW_RIGHT;
            k.state = scancode == 0x4d ? PRESSED : RELEASED;
        } else if (scancode == 0x53 || scancode == 0xd3) {
            k.key = KEY_DELETE;
            k.state = scancode == 0x53 ? PRESSED : RELEASED;
        } else {
            k.key = KEY_NOTHING;
            k.state = PRESSED; 
        }
        return k;
    }

    k.key = keys_table[scancode & 0x7f];
    k.state = (scancode & 0x80) ? 1 : 0;

    return k;
}

void keyboard_init(void) {
    KINFO("[KBD] PS/2 Keyboard driver active");
}
