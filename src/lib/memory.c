#include "../include/memory.h"
#include <stdint.h>
#include "../include/log.h"
#include "../include/memory.h"
#include "../include/mm.h"
#include "../include/string.h"

typedef struct block_header {
  size_t size;
  int free;
  struct block_header *next;
  struct block_header *prev;
} block_header_t;

#define HEADER_SIZE sizeof(block_header_t)
#define PAGE_SIZE 4096
#define HEAP_VIRT_START 0xD0000000u
#define HEAP_INITIAL_PAGES 16
#define ALIGN_UP(x, a) (((x) + ((a) - 1)) & ~((a) - 1))

static uintptr_t heap_end = 0;
static block_header_t *head = NULL;

static int heap_map_pages(size_t pages) {
  for (size_t i = 0; i < pages; i++) {
    uint32_t phys = pmm_alloc_frame();
    if (phys == 0) {
      return 0;
    }
    mmu_map_page(phys, heap_end, true, false);
    heap_end += PAGE_SIZE;
  }
  return 1;
}

void memory_init(void) {
  heap_end = HEAP_VIRT_START;

  if (!heap_map_pages(HEAP_INITIAL_PAGES)) {
    KPANIC("[MEM] Failed to allocate initial heap pages");
    return;
  }


  head = (block_header_t *) HEAP_VIRT_START;
  head->size = (heap_end - HEAP_VIRT_START) - HEADER_SIZE;
  head->free = 1;
  head->next = NULL;
  head->prev = NULL;
  KINFO("[MEM] Initial heap mapped");
}

// Set memory zone to a specific value
void *memset(void *ptr, int c, size_t size) {
  unsigned char *c_ptr = ptr;
  for (size_t i = 0; i < size; i++) {
    c_ptr[i] = (unsigned char) c;
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
  unsigned char *d = (unsigned char *) dest;
  const unsigned char *s = (const unsigned char *) src;

  if (d == s)
    return dest;

  if (d < s)
    return memcpy(dest, src, n);
  else {
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
  unsigned char uc = (unsigned char) c;

  while (n--) {
    *d++ = *s;
    if (*s == uc)
      return d;
    s++;
  }
  return NULL;
}

// Duplicate a string pointer
char *strdup(const char *src) {
  char *cpy = malloc(sizeof(char) * (strlen(src) + 1));
  if (cpy == NULL)
    return NULL;
  strlcpy(cpy, src, strlen(src) + 1);
  return (cpy);
}

static block_header_t *heap_grow(size_t needed) {
  size_t bytes = ALIGN_UP(needed, PAGE_SIZE);
  size_t pages = bytes / PAGE_SIZE;

  uintptr_t old_end = heap_end;
  if (!heap_map_pages(pages)) {
    return NULL;
  }

  block_header_t *block = (block_header_t *) old_end;
  block->size = (heap_end - old_end) - HEADER_SIZE;
  block->free = 1;
  block->next = NULL;
  block->prev = NULL;

  block_header_t *tail = head;
  while (tail->next) {
    tail = tail->next;
  }
  tail->next = block;
  block->prev = tail;

  return block;
}

static void split_block(block_header_t *block, size_t size) {
  size_t remaining = block->size - size;
  if (remaining < HEADER_SIZE + 16) {
    return;
  }

  block_header_t *new_block = (block_header_t *) ((uint8_t *) block + HEADER_SIZE + size);
  new_block->size = remaining - HEADER_SIZE;
  new_block->free = 1;
  new_block->next = block->next;
  new_block->prev = block;

  if (block->next) {
    block->next->prev = new_block;
  }
  block->next = new_block;
  block->size = size;
}

// Memory Allocation
void *malloc(size_t size) {
  if (size == 0) {
    return NULL;
  }
  size = ALIGN_UP(size, 8);

  block_header_t *block = head;

  while (block) {
    if (block->free && block->size >= size) {
      split_block(block, size);
      block->free = 0;
      return (void *) ((uint8_t *) block + HEADER_SIZE);
    }
    block = block->next;
  }

  block = heap_grow(size + HEADER_SIZE);
  if (!block) {
    return NULL;
  }
  split_block(block, size);
  block->free = 0;
  return (void *) ((uint8_t *) block + HEADER_SIZE);
}

// Free allocated mem bloc
void free(void *ptr) {
  if (!ptr)
    return;

  block_header_t *block = (block_header_t *) ((uint8_t *) ptr - HEADER_SIZE);
  block->free = 1;

  if (block->next && block->next->free &&
      (uint8_t *) block + HEADER_SIZE + block->size == (uint8_t *) block->next) {
    block->size += HEADER_SIZE + block->next->size;
    block->next = block->next->next;
    if (block->next) {
      block->next->prev = block;
    }
  }

  if (block->prev && block->prev->free &&
      (uint8_t *) block->prev + HEADER_SIZE + block->prev->size == (uint8_t *) block) {
    block->prev->size += HEADER_SIZE + block->size;
    block->prev->next = block->next;
    if (block->next) {
      block->next->prev = block->prev;
    }
  }
}
