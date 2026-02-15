[BITS 32]
global loader 
extern kernel_main              ; call  the external function on the kernel.c to print */ 

MAGIC_NUMBER equ 0x1BADB002
FLAGS        equ 0x00000003        ; align + meminfo seulement
CHECKSUM     equ -(MAGIC_NUMBER + FLAGS)


section .bootloader
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
