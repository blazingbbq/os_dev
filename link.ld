ENTRY(loader)                /* the name of the entry label */

SECTIONS {
    . = 0xC0100000;
    /* addd symbol that indicated the start address of the kernel */
    _kernel_start = .;
    .text ALIGN (0x1000) : AT(ADDR(.text) - 0xC0000000)   /* align at 4 KB */
    {
        *(.grub_sig)  
        *(.text)             /* all text sections from all files */
    }

    .rodata ALIGN (0x1000) : AT(ADDR(.rodata) - 0xC0000000)  /* align at 4 KB */
    {
        *(.rodata*)          /* all read-only data sections from all files */
    }

    .data ALIGN (0x1000) : AT(ADDR(.data) - 0xC0000000)  /* align at 4 KB */
    {
        *(.data)             /* all data sections from all files */
    }

    .bss ALIGN (0x1000) : AT(ADDR(.bss) - 0xC0000000)   /* align at 4 KB */
    {
        *(COMMON)            /* all COMMON sections from all files */
        *(.bss)              /* all bss sections from all files */
    }
    /* add symbol that indicates the end address of the kernel */
    _kernel_end = .;
}
