#include "../includes/memory.h"
#include <stddef.h>

// Set memory zone to a specific value
void *memset(void *ptr, int c, size_t size) {
    unsigned char *c_ptr = ptr;
    for (size_t i = 0; i < size; i++) {
        c_ptr[i] = (unsigned char)c;
    }
    return ptr;
}

// Compare two memory zones
int memcmp(const void *s1, const void *s2, size_t count) {
    const unsigned char *c1 = s1;
    const unsigned char *c2 = s2;
    
    while (count-- > 0) {
        if (*c1 != *c2)
            return (*c1 < *c2) ? -1 : 1;
        c1++;
        c2++;
    }
    return 0;
}

// Copy memory zone
void *memcpy(void *dest, const void *src, size_t len) {
    unsigned char *d = dest;
    const unsigned char *s = src;
    
    while (len--) {
        *d++ = *s++;
    }
    return dest;
}

// Copy up to n bytes or until character c is found
void *memccpy(void *dest, const void *src, int c, size_t n) {
    unsigned char *d = dest;
    const unsigned char *s = src;
    unsigned char uc = (unsigned char)c;
    
    while (n--) {
        *d++ = *s;
        if (*s == uc)
            return d; // Returns address after the found character
        s++;
    }
    return NULL;
};