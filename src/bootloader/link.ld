ENTRY(loader)

SECTIONS {
    . = 0x00100000;     /* code should be load at 1  MB */

    .text ALIGN  (0x1000) :
    {
        *(.text)        /* all text section from all files */
    }
    .rodata ALIGN (0x1000) :  /* align to 4 KB */
    {
        *(.rodata*)         /* all read only sections from all files */
    }

    .data ALIGN (0x1000) :   /* align to 4 KB  */
    {
        *(.data)            /* all data sections from all files */
    }

    .bss ALIGN (0x1000) :
    {
        *(COMMON)           /* all COMMON sections from all files */
        *(.bss)             /*  all bss sections from all files */
    }
}