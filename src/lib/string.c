#include "../include/string.h"

// Count char in string
size_t strlen(const char* str) {
    size_t len = 0;
    while (str[len])
        len++;
    return len;
}
