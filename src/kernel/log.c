#include "../include/log.h"
#include "../include/serial.h"

void klog(const char *level, const char *msg) {
  serial_write_char('[');
  serial_write_string(level);
  serial_write_string("] ");
  serial_write_string(msg);
  serial_write_char('\n');
  serial_write_char('\r');
}
