global loader 

    MAGIC_NUMBER equ 0x1BADB002         ; take the MAGIC_NUMBER of the bootloader */
    FLAGS        equ 0x0
    CHECKSUM     equ -MAGIC_NUMBER 


section .text       ; go to the .text section, recup MAGIC, FLAGS, and CHECKSUM  */
    align 4
    dd MAGIC_NUMBER
    dd FLAGS
    dd CHECKSUM

loader:                 ; loader link to link.ld ( connected with the bootloader magic) */
mov eax, 0xCAFEBABE

.loop:              ; jump in loop on the section assigned  */
jmp .loop