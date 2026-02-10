[BITS 32]
global idt_flush

idt_flush:
    mov eax, [esp + 4]  ; Get the pointer to the IDT passed as a parameter
    lidt [eax]          ; Load the IDT pointer
    ret
