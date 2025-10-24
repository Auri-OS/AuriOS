#ifndef MEMORY_H
#define MEMORY_H

// Définir size_t correctement pour votre architecture
#if defined(__x86_64__) || defined(__aarch64__)
    typedef unsigned long size_t;
#else
    typedef unsigned int size_t;
#endif

// Set memory zone to a specific value
void *memset(void *ptr, int c, size_t size);

// Compare two memory zones
int memcmp(const void *s1, const void *s2, size_t count);

// Copy memory zone
void *memcpy(void *dest, const void *src, size_t len);

// Copy up to n bytes or until character c is found
void *memccpy(void *dest, const void *src, int c, size_t n);

#endif // MEMORY_H