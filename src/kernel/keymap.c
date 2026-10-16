#include "../include/integer.h"
#include "../include/keymap.h"
#include "../include/keyboard.h"
#include "../include/string.h"
#include "../include/log.h"

#define MAX_PARSED_CHAR_LENGTH 8

static int control = 0;
static int shift = 0;
static int capslock = 0;
static int alt = 0;
static int index = 0;

static const char* scancode_to_ascii[8][128] = {
    {
        0, 0, "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=", "\b",
        "\t", "q", "w", "e", "r", "t", "y", "u", "i", "o", "p", "[", "]", "\n",
        0, "a", "s", "d", "f", "g", "h", "j", "k", "l", ";", "\'", "`",
        0, "\\", "z", "x", "c", "v", "b", "n", "m", ",", ".", "/", 0,
        "*", 0, " "
    },
    {
        0, 0, "&", "é", "\"", "'", "(", "-", "è", "_", "ç", "à", ")", "=", "\b",
        "\t", "a", "z", "e", "r", "t", "y", "u", "i", "o", "p", "^", "$", "\n",
        0, "q", "s", "d", "f", "g", "h", "j", "k", "l", "m", "ù", 0,
        0, "*", "w", "x", "c", "v", "b", "n", ",", ";", ":", "!", 0,
        0, 0, " "
    }
};
static const char* scancode_to_ascii_shift[8][128] = {
    {
        0, 0, "!", "@", "#", "$", "%", "^", "&", "*", "(", ")", "_", "+", "\b",
        "\t", "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "{", "}", "\n",
        0, "A", "S", "D", "F", "G", "H", "J", "K", "L", ":", "\"", "~",
        0, "|", "Z", "X", "C", "V", "B", "N", "M", "<", ">", "?", 0,
        "*", 0, " "
    },
    {
        0, 0, "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", 0, "+", "\b",   // ° not allowed
        "\t", "A", "Z", "E", "R", "T", "Y", "U", "I", "O", "P", 0, "£", "\n",   // ¨ not allowed
        0, "Q", "S", "D", "F", "G", "H", "J", "K", "L", "M", "%", 0,
        0, 0, "W", "X", "C", "V", "B", "N", "?", ".", "/", 0, 0, // µ and § not allowed
        0, 0, " "
    }
};

const char* keymap_parse(Key scancode) {
    static char pchar[MAX_PARSED_CHAR_LENGTH];
    Keys k = scancode.key;
    State s = scancode.state;

    pchar[0] = '\0';

    if (k == KEY_SHIFT) {
        shift = !s;
        return "";
    } else if (k == KEY_CONTROL) {
        control = !s;
        return "";
    } else if (k == KEY_ALT) {
        alt = !s;
        return "";
    } else if (scancode.state == PRESSED) {
        if (k == KEY_ARROW_UP) {
            strlcpy(pchar, "^[[A", sizeof(pchar));
        } else if (k == KEY_ARROW_DOWN) {
            strlcpy(pchar, "^[[B", sizeof(pchar));
        } else if (k == KEY_ARROW_RIGHT) {
            strlcpy(pchar, alt ? "^[[F" : "^[[C", sizeof(pchar)); // END : RIGHT
        } else if (k == KEY_ARROW_LEFT) {
            strlcpy(pchar, alt ? "^[[E" : "^[[D", sizeof(pchar)); // HOME : LEFT
        } else if (k == KEY_CAPS_LOCK) {
            capslock = !capslock;
        } else if (k == KEY_DELETE) {
            strlcpy(pchar, "^[[2~", sizeof(pchar));
        } else if (shift || capslock) {
            const char* mapped = scancode_to_ascii_shift[index][k];
            if (mapped) strlcpy(pchar, mapped, sizeof(pchar));
        } else if (control && k == KEY_L) {
            strlcpy(pchar, "^L", sizeof(pchar));
        } else {
            const char* mapped = scancode_to_ascii[index][k];
            if (mapped) strlcpy(pchar, mapped, sizeof(pchar));
        }
    }
    
    return pchar;
}

void keymap_set_index(int i) {
    index = i;
}