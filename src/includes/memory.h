#ifndef MEMORY_H
#define MEMORY_H

typedef unsigned int size_t;

void *memset(void *ptr, int c, size_t size);
int memcmp(void *s1, void *s2, int count);
void *memcpy(void *dest, void *src, int len);
void *memccpy(void *dest, void *src, int len, size_t n);

#endif
