[BITS 32]
global gdt_flush
gdt_flush:
    mov eax, [esp +4]           ; gdt_ptr adress
    lgdt [eax]                  ; load the GDT Pointer 

    mov ax, 0x10                ; data segment selector (second entry so index=1 -> 0x05)
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

        jmp 0x08:.flush         ; far jump to reload CS
    .flush:
        ret