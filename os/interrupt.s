; Interrupt service routine
    .module isr
    
    .globl interrupt

; Output port
port_a = 0x0001

    .area _CODE

interrupt::
    ; Prologue
    push    af
    push    bc
    push    de
    push    hl
    push    iy

    ld de, #0x202 ; This needs calibrating
interrupt_wait:
    dec de
    ld a, d
    or e
    jp nz, interrupt_wait

    nop

    ; Initialise line loop count 480
    ld de, #480

interrupt_line_loop:
    ld a, #0x01
    ld b, #0x00
    ld c, #port_a

    ; Enable display output
    out (c), a

    ; Line is 640/2 = 320 clocks = 80 nops
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
    ;nop  ; out(c), b takes time of three nops
    ;nop
    ;nop

    ; Disable display output
    out (c), b

    ld a, #0

    ; 4*4 = 16 t-states
    ;nop
    ;nop
    ;nop
    ;nop

    ; 9+7 = 16 t-states
    ld r, a
    xor #0

    ; Loop
    dec de
    ld a, d
    or e
    jp nz, interrupt_line_loop

    ld a, #0
    ld i, a

    ; Epilogue
    pop iy
    pop hl
    pop de
    pop bc
    pop af
    ei
    reti

; vim:sw=4:sts=4:et
