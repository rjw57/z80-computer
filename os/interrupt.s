; Interrupt service routine
    .module isr
    
    .globl interrupt

; Output port
port_a = 0x81

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
    ; 4 t-states per character. We don't use the full horizonal display area so
    ; for our signal the timing is as follows:
    ;
    ; tBP   =  14 chars =  56 t-states
    ; tV    =  64 chars = 256 t-states
    ; tFP   =  10 chars =  40 t-states
    ; tSP   =  12 chars =  48 t-states
    ; tL    = 100 chars = 400 t-states

    ; After the Z80 acknowledges the interrups, the HW introduces wait states
    ; until the low-going edge of HSYNC at which point this timing starts. We
    ; take a full line of set up before we start displaying pixels.

    ; We annotate instructions below with the number of t-states they use.

    ; Interrupt handler prologue

    ex af,af'           ; 4
    exx                 ; 4
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
    nop                 ; 4

    ; Initialise line loop count
    ld de, #352         ; 10 t-states

                        ; total 73 t-states, want to be at start of front porch
                        ; which is 360 t-states from start of routine so a
                        ; remainder of 287

    xor #0              ; 7-tstates

    nop                 ; 70 nops = 280 t-states
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop                 ; 10

    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop                 ; 20

    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop                 ; 30

    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop                 ; 40

    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop                 ; 50

    nop
    nop
    nop
    nop
    ;nop                ; FIXME: why an extra 24 t-states?
    ;nop
    ;nop
    ;nop
    ;nop
    ;nop                 ; 60

    nop
    nop
    nop
    nop
    ;nop
    ;nop
    ;nop
    ;nop
    ;nop
    ;nop                ; 70

    xor #0              ; 7 t-states
    xor #0              ; 7 t-states
    ld hl, #0x4000-0x40      ; 10 t-states

    ; Now at start of front porch. The decrement and loop code at the end of the
    ; interrupt_line_loop block takes 24 t-states which we use here
    nop
    nop
    nop
    nop
    nop
    nop

interrupt_line_loop:
    ; We have tFP + tSP + tBP - 24 = 120 t-states for setup
    nop                 ; 4 t-states
    nop                 ; 4 t-states
    nop                 ; 4 t-states
    nop                 ; 4 t-states
    nop                 ; 4 t-states
    nop                 ; 4 t-states

    nop                 ; 4 t-states
    nop                 ; 4 t-states
    nop                 ; 4 t-states
    nop                 ; 4 t-states
    nop                 ; 4 t-states
    nop                 ; 4 t-states

    xor #0              ; 7 t-states
    inc bc              ; 6 t-states
    inc bc              ; 6 t-states
    inc bc              ; 6 t-states

    ld bc, #0x0040      ; 10 t-states
    add hl, bc          ; 11 t-states
    ld a, h             ; 4 t-states
    ld i, a             ; 9 t-states
    ld a, l             ; 4 t-states
    ld r, a             ; 9 t-states

    ; Displaying line, 64 chars or 256 t-states
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop

    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop

    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop

    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop

    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop

    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop

    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop

    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop

    ; Decrement de and loop = 24 t-states
    dec de                      ; 6 t-states
    ld a, d                     ; 4 t-states
    or e                        ; 4 t-states
    jp nz, interrupt_line_loop  ; 10 t-states

    ; Epilogue
    exx                 ; 4
    ex af,af'           ; 4
    ei
    reti

; vim:sw=4:sts=4:et
