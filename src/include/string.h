#ifndef STRING_H
#define STRING_H

#include <stddef.h>

size_t strlen(const char* str);
int strcmp(const char *s1, const char *s2);
int	strcat(char *dest, char *src);
char *strncat(char *dest, char *src, unsigned int nb);
int str_is_uppercase(char *str);
int str_is_lowercase(char *str);
char *strupcase(char *str);
char *strlowcase(char *str);
char *strcpy(char *dest, const char *src);
char *str_trim(char *str);
#endif
