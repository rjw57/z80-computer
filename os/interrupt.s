; Interrupt service routine
    .module isr

    .globl interrupt
    .globl _gfx_screen

    .area _CODE

interrupt::
    ; Our HSYNC pulse and display on signals look like the following:
    ;
    ;                  :       :                                :     :
    ;        ____.     .________________________________________.     .____
    ; HSYNC      |_____|       :                                |_____|
    ;        _.        :       .__________________________.     :     :
    ; DPY ON  |________________|                          |________________
    ;                  :       :                          :     :     :
    ;                  :--tBP--'------------tV------------'-tFP-'-tSP-:
    ;                  :                                              :
    ;                  '--------------------tL------------------------'
    ;
    ; BP = Back Porch, V = Visible, FP = Front Porch, SP = Sync Pulse
    ;
    ; The CPU is clocked so that each t-states is 2 pixels. Therefore there are
    ; 4 t-states per character. We don't use the full horizontal display area so
    ; for our signal the timing is as follows:
    ;
    ; tBP   =  14 chars =  56 t-states
    ; tV    =  64 chars = 256 t-states
    ; tFP   =  10 chars =  40 t-states
    ; tSP   =  12 chars =  48 t-states
    ; tL    = 100 chars = 400 t-states

    ; We annotate instructions below with the number of t-states they use.

    ; After the Z80 acknowledges the interrups, the HW introduces wait states
    ; until the low-going edge of HSYNC. There is then one more t-state (T3) of
    ; the special interrupt M1 cycle followed by a jump to $0038. This takes 12
    ; t-states to complete after the low-going edge of HSYNC. The interrupt
    ; vector is a JP to this routine (10 t-states) meaning that this routine
    ; starts 22 t-states after the low going edge of HSYNC.

    ; We take the entirety of the remainder of the line up until the start of
    ; the next visible section for setup. That is tL + tBP - 22 t-states or
    ; 434 t-states.

    ; Interrupt prologue. We have plenty of setup time so do not make use of the
    ; shadow registers. Takes 74 t-states in total.
    push af                 ; 11
    push bc                 ; 11
    push de                 ; 11
    push hl                 ; 11
    push ix                 ; 15
    push iy                 ; 15

    ; 360 t-states until start of line

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
    nop
    nop
    nop
    nop

    nop
    nop

    xor #0
    ;nop
    ;nop

    xor #0
    ;nop
    ;nop

    xor #0
    ;nop
    ;nop

    ; Prepare for start of refresh loop

    ; Initialise remaining lines to draw which is twice the current graphics
    ; screen height.
    ld bc, #352             ; 10

    ; Initialise refresh address for screen line
    ld hl, (_gfx_screen)    ; 20

refresh_line_loop:
    ; Start of loop - 26 t-states
    ld a, h                 ; 4
    ld i, a                 ; 9
    ld a, l                 ; 4
    ld r, a                 ; 9

    ; tV - 256 t states
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

    ; 144 t-states until next line. 26 t-states of loop prologue mean 118
    ; t-states until next loop iteration

    nop
    nop
    nop
    nop
    nop
    nop
    ;nop
    ;nop
    ;nop
    ;nop

    ;nop
    ;nop
    ;nop

    xor #0                      ; 7
    xor #0                      ; 7
    xor #0                      ; 7

    ; set A to 0x40 if bottom bit of C set and 0 otherwise
    ld a, #0x01                 ; 7
    and c                       ; 4
    rrc a                       ; 8
    rrc a                       ; 8

    ; load de with offset to next line
    ld e, a                     ; 4
    ld d, #0                    ; 7

    add hl, de                  ; 11

    ; Decrement and loop = 24 t-states
    dec bc                      ; 6 t-states
    ld a, b                     ; 4 t-states
    or c                        ; 4 t-states
    jp nz, refresh_line_loop    ; 10 t-states

    ; Interrupt epilogue
    pop iy
    pop ix
    pop hl
    pop de
    pop bc
    pop af
    ei
    reti

; vim:sw=4:sts=4:et
