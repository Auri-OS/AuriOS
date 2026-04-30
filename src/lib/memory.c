#include "../include/memory.h"
#include "../include/string.h"
#include "../include/memory.h"
#include "../include/log.h"
#include <stdint.h>

typedef struct block_header {
    size_t size;
    int free;
    struct block_header *next;
} block_header_t;

#define HEADER_SIZE sizeof(block_header_t)

extern char kernel_end;
static block_header_t *head = NULL;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"

void memory_init(void) {
    uintptr_t heap_start = (uintptr_t)&kernel_end;
    head = (block_header_t *)heap_start;
    head->size = 1024 * 1024;
    head->free = 1;
    head->next = NULL;
    KINFO("[MEM] Initial heap mapped");
}

#pragma GCC diagnostic pop


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


// Cpoy memory zone taking into account the overlap
void *memmove(void *dest, const void *src, size_t n) {
    unsigned char *d = (unsigned char *)dest;
    const unsigned char *s = (const unsigned char *)src;

    if (d == s) return dest;

    if (d < s) 
      return memcpy(dest, src, n);
    else
    {
      d += n;
      s += n;
      while (n--) {
        *(--d) = *(--s);
      }
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
            return d;
        s++;
    }
    return NULL;
}

// Duplicate a string pointer
char *strdup(const char *src)
{
	char *cpy = malloc(sizeof(char) * (strlen(src) + 1));
	if (cpy == NULL)
		return NULL;
	strcpy(cpy, src);
	return (cpy);
} 

// Memory Allocation
void *malloc(size_t size)
{
    block_header_t *block = head;

    while (block) {
        if (block->free && block->size >= size) {
            block->free = 0;
            return (void *)((uint8_t *)block + HEADER_SIZE);
        }
        block = block->next;
    }

    return NULL;
}

// Free allocated mem bloc
void free(void *ptr)
{
    if (!ptr)
        return;

    block_header_t *block = (block_header_t *)((uint8_t *)ptr - HEADER_SIZE);
    if (block != 0)
        block->free = 1;
}
