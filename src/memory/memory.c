#include "../includes/memory.h"

void *memset(void *ptr, int c, size_t size) {
    char *c_ptr = (char *) ptr;
    for (size_t i = 0; i < size; i++){
        c_ptr[i] = (char)c;
    }
    return ptr;
}

int memcmp(void *s1, void *s2, int count){
    char *c1 = s1;
    char *c2 = s2;
    while(count-- > 0)
    {
        if (*c1++ != *c2++)
            return c1[-1] < c2[-1] ? -1 : 1;
    }
    return 0;
}

void *memcpy(void *dest, void *src, int len){
    char *d = dest;
    char *s = src;
    while(len--)
        *d++ = *s++;
    return dest;
}

void *memccpy(void *dest, str *src, int len, size_t n) {
  unsigned char *ptr;
  unsigned char *pr;
  unsigned char count;

  ptr = (unsigned char *) dest;
  pr = (unsigned char *) src;
  count = (int) len;

  while (n-- > 0) {
    *ptr++ = *pr++;
  }
  return (dest);
}
