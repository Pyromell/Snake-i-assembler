MULTIPLEX:
; Multiplexes DAMatrix.
; Draws LINE on each display 3..0.
    push    ZH
    push    ZL
    push    r16
    push    r17
    push    r18
    push    r19

    in      r16,SREG
    push    r16 

    ldi     ZL,LOW(VMEM)
    ldi     ZH,HIGH(VMEM)

    ; sets Z to start of display 3
    ldi     r17,96
    add     ZL,r17
    adc     ZH,ZERO

    ; resets LINE to 0 if 8
    lds     r16,LINE
    andi    r16,7
    mov     r17,r16

    ; LINE * 4 to get row in VMEM,
    ; sets Z to that row
    lsl     r17
    lsl     r17
    add     ZL,r17
    adc     ZH,ZERO

    call    COORD2BYTE ; in r16, out r19

    cbi     PORTB,SS ; pulls latch

    ; loops through displays
    ldi     r18,4
disp_loop:
    ; transmits blue
    ldd     r16,Z+B 
    push    r16
    call    SPI_TX
    pop     r16

    ; transmits green
    ldd     r16,Z+G
    push    r16
    call    SPI_TX
    pop     r16

    ; transmits red
    ldd     r16,Z+R
    push    r16
    call    SPI_TX
    pop     r16
    
    ; transmits anode
    com     r19 ; invert bytes for DAMatrix
    push    r19
    call    SPI_TX
    pop     r19
    com     r19

    ; moves Z to next display
    ldi     r17,32
    sub     ZL,r17
    sbc     ZH,ZERO

    dec     r18
    brne    disp_loop

    sbi     PORTB,SS ; pulls latch

    ; incs LINE before next INT
    lds     r16,LINE
    inc     r16
    sts     LINE,r16

    pop     r16
    out     SREG,r16

    pop     r19
    pop     r18
    pop     r17
    pop     r16
    pop     ZL
    pop     ZH
    reti