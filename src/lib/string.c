#include "../include/string.h"

// Count char in string
size_t strlen(const char* str) {
    size_t len = 0;
    while (str[len] != '\0')
        len++;
    return len;
}

// Compare two string between them
int strcmp(const char *s1, const char *s2)
{
    while (*s1 && *s1 == *s2) {
        s1++;
        s2++;
    }
    return *s1 - *s2;
}