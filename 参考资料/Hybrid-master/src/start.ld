
MEMORY
{
    flash (rx):  ORIGIN = 0x08000000, LENGTH = 0x10000
    ram   (rxw): ORIGIN = 0x20000000, LENGTH = 0x5000
}

ENTRY (_start)

SECTIONS
{
    .text :{
        KEEP(*(.vectors))
        *(.text)          
        *(.rodata)         
    } > flash
    
   
    .data :{}> ram
    .bss  :{}> ram
}




