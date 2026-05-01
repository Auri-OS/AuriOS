#include "../include/serial.h"
#include "../include/io.h"

#define SERIAL_PORT 0x3F8

void serial_init(void) {
  outb(SERIAL_PORT + 1, 0x00);
  outb(SERIAL_PORT + 3, 0x80);
  outb(SERIAL_PORT + 0, 0x03);
  outb(SERIAL_PORT + 1, 0x00);
  outb(SERIAL_PORT + 3, 0x03);
  outb(SERIAL_PORT + 2, 0xC7);
  outb(SERIAL_PORT + 4, 0x0B);
}

static int serial_is_transmit_empty(void) {
  return inb(SERIAL_PORT + 5) & 0x20;
}

void serial_write_char(char c) {
  while (serial_is_transmit_empty() == 0)
    ;
  outb(SERIAL_PORT, c);
}

void serial_write_string(const char *str) {
  for (int i = 0; str[i] != '\0'; i++) {
    serial_write_char(str[i]);
  }
}
