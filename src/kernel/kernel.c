#include <stddef.h>
#include <stdint.h>

uint16_t terminal_make_char(char c, char color){
    return(color << 8) | c;
}


void kernel_main();