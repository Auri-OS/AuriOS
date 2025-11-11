[BITS 32]
global loader 
extern kernel_main              ; call  the external function on the kernel.c to print */ 

MAGIC_NUMBER equ 0x1BADB002         ; take the MAGIC_NUMBER of the bootloader */
FLAGS        equ 0x0
CHECKSUM     equ -MAGIC_NUMBER 


section .bootloader       ; go to the .text section, recup MAGIC, FLAGS, and CHECKSUM  */
    align 4
    dd MAGIC_NUMBER
    dd FLAGS
    dd CHECKSUM

section .text

loader:
    ; Init heap
    mov esp, 0x90000


    ; deleting flags
    push 0
    popf

    ; call kernel.c     */
    call kernel_main

.loop:
    hlt
    jmp .loop
