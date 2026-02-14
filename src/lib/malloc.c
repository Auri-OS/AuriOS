#include "../include/memory.h"
#include <stdint.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds="

typedef struct block_header {
    size_t size;
    int free;
    struct block_header *next;
} block_header_t;

#define HEADER_SIZE sizeof(block_header_t)

extern char kernel_end;
static block_header_t *head = NULL;

void memory_init(void)
{
    uintptr_t heap_start = (uintptr_t)&kernel_end;
    head = (block_header_t *)heap_start;
    head->size = 1024 * 1024;
    head->free = 1;
    head->next = NULL;
}

#pragma GCC diagnostic pop

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

void free(void *ptr)
{
    if (!ptr)
        return;

    block_header_t *block = (block_header_t *)((uint8_t *)ptr - HEADER_SIZE);
    block->free = 1;
}