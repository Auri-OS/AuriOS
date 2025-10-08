[BITS 32]
global loader 
extern kernel_main              ; call  the external function on the kernel.c to print */ 

    MAGIC_NUMBER equ 0x1BADB002         ; take the MAGIC_NUMBER of the bootloader */
    FLAGS        equ 0x0
    CHECKSUM     equ -MAGIC_NUMBER 


section .text       ; go to the .text section, recup MAGIC, FLAGS, and CHECKSUM  */
    align 4
    dd MAGIC_NUMBER
    dd FLAGS
    dd CHECKSUM

loader:                 ; loader link to link.ld ( connected with the bootloader magic) */
    ; Initialisation segment
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Initialisation heap
    mov esp, 0x90000

    ; call kernel.c 
    call kernel_main

.loop:
    jmp .loop
