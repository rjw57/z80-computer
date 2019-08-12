; Interrupt service routine
    .module isr
    
    .globl interrupt

; Output port
port_a = 0x81

    .area _CODE

interrupt::
    ; The HW introduces wait states until the low-going edge of HSYNC. The sync
    ; pulse width + back porch is 96 + 48 = 144 dots. This is 72 CPU clocks.
    ; Our prologue and setup takes the remainder of the line or 640 dots = 320
    ; CPU clocks. As such we have 72 + 320 = 392 CPU clocks or t-states to play
    ; with.

    ; Prologue 12*4 + 15 = 63 t-states
    di
    push    af
    push    bc
    push    de
    push    hl
    push    iy

    ld a, #0x40         ; 7 t-states
    ld i, a             ; 9 t-states
    ld a, #0x0          ; 7 t-states
    ld r, a             ; 9 t-states

    ; Initialise line loop count
    ld de, #360         ; 10 t-states

    ; Remainder: 291 t-states = 71 * 4 + 7
    xor #0

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

interrupt_line_loop:
    ld a, #0x01
    ld b, #0x00
    ld c, #port_a

    nop
    nop
    nop
    nop
    nop
    nop
    nop

    ; Enable display output. NOTE: changing display enable doesn't actually take
    ; effect until the end of the current character so there is some wiggle room
    ; in this timing.
    out (c), a
    nop

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
    ;nop  ; out(c), b takes time of three nops
    ;nop
    nop

    ; Disable display output
    out (c), b

    nop
    nop
    nop
    nop
    nop
    nop
    nop

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

    ld c, #port_a
    ld a, #0
    out(c), a

    ; Epilogue
    pop iy
    pop hl
    pop de
    pop bc
    pop af
    ei
    reti

; vim:sw=4:sts=4:et
