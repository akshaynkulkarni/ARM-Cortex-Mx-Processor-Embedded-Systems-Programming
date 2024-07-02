ENTRY(Handler_Reset)

MEMORY {
    RAM(rwx) : ORIGIN = 0x20000000, LENGTH = 96K
    ROM(rx) : ORIGIN = 0x08000000, LENGTH = 512K
}

__sstack = ORIGIN(RAM) + LENGTH(RAM);
_estack = __sstack;

SECTIONS {
    .text : {
        __stext = .;
        *(.vector_table)
        *(.text.*)
        *(.text)
        *(.fini)
        *(.rodata)
        *(*.init)
        *(*.finit)
        *(.eh_frame)
        *(.ARM.exidx)
        *(.rodata.str1.4)
        . = ALIGN(4);
        __etext = .;
    } > ROM

    .data : {
        __sdata = .;
        *(.data)
        *(.init_array*)
        *(.fini_array)
        . = ALIGN(4);
        __edata = .;
    } > RAM AT > ROM

    .bss : {
        __sbss = .;
        __bss_start__ = __sbss;
        *(.bss)
        *(COMMON)
        . = ALIGN(4);
        __ebss = .;
        __bss_end__ = __ebss;
        __end__ = __bss_end__;
        end =  __end__;
    } > RAM

}