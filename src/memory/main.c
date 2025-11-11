#include <gdt.h>

int main(void){
    gdt_init();
    while (1);
    return 0;
}