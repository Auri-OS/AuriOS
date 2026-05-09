#ifndef INTEGER_H
#define INTEGER_H

#include <stdint.h>

int	atoi(char *str);
char *itoa(int nb, char *buffer);
uint32_t htoi(const char *hex_str);
void putnbr(int nb);

#endif
