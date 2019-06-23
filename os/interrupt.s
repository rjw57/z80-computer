        .module interrupt

        .globl _isr

; Interrupt service outine has 32 NOPs to guarantee alignment
        .area _HEADER (ABS)
        .org 0x40
interrupt::
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        jp _isr
