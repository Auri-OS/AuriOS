ENTRY(loader)

SECTIONS {
    . = 0x00100000;

    .bootloader ALIGN(0x1000) : {
        *(.bootloader)
    }
    
    .text ALIGN(0x1000) : {
        __text_start = .;
        *(.text)
        __text_end = .;
    }

    .rodata ALIGN(0x1000) : {
        __rodata_start = .;
        *(.rodata*)
        __rodata_end = .;
    }

    .data ALIGN(0x1000) : {
        __data_start = .;
        *(.data)
        __data_end = .;
    }

    .bss ALIGN(0x1000) : {
        __bss_start = .;
        *(COMMON)
        *(.bss)
        __bss_end = .;
    }

    /DISCARD/ : {
        *(.comment)
        *(.note*)
    }

    kernel_end = .;
}