COORD2BYTE:
; Converts coordinate in r16
; to byte for DAMatrix in r19

    ldi     r19,0b1000_0000
    cpi     r16,0
    breq    c2b_done
c2b_loop:    
    lsr     r19
    dec     r16    
    brne    c2b_loop
c2b_done:
    ret


SPI_TX:
; Sends argument register on stack,
; waiting until send is complete
; by checking flag in SPSR

    push    ZH
    push    ZL
    push    r16

    in      ZH,SPH
    in      ZL,SPL
    ldd     r16,Z+6
    out     SPDR,r16

spi_tx_wait:
    in      r16,SPSR
    sbrs    r16,SPIF
    rjmp    spi_tx_wait

    pop     r16
    pop     ZL
    pop     ZH
    ret


DELAY:
    ldi     r16,8
delay0:
    ldi     r17,$ff
delay1:
    ldi     r18,$ff
delay2:
    dec     r18
    brne    delay2
    dec     r17
    brne    delay1
    dec     r16
    brne    delay0
    ret


SEP_XY:
; In:  r16 = 0bxxxx_yyyy.
; Out: r16 = 0b0000_xxxx,
;      r17 = 0b0000_yyyy.
    mov     r17,r16
    swap    r16
    andi    r16,0b0000_1111 ; x
    andi    r17,0b0000_1111 ; y
    ret


JOIN_XY:
; In:  r16 = 0b0000_xxxx.
;      r17 = 0b0000_yyyy,
; Out: r16 = 0bxxxx_yyyy.
    swap    r16
    or      r16,r17
    ret

/*
JOYSTICK:
    lds     r16,P1
    call    JOY_X
    sts     P1,r16

    lds     r16,P1
    call    JOY_Y
    sts     P1,r16

    lds     r16,P2
    call    JOY_X
    sts     P2,r16

    lds     r16,P2
    call    JOY_Y
    sts     P2,r16
    ret


JOY_X:
; Performs ADC conversion and
; updates head coords in SRAM
    ldi     r18,3 ; reads from PC3
    push    r16
    call    READ_JOY ; returns ADCH in r19
    pop     r16
    call    SEP_XY
check_right:
    cpi     r19,0b1100_0000 ; checks right
    brlo    check_left
    inc     r16
    rjmp    x_done
check_left:
    cpi     r19,0b0100_0000 ; checks left
    brsh    x_done
    dec     r16
x_done:
    call    JOIN_XY
    ret

JOY_Y:
; Performs ADC conversion and
; updates head coords in SRAM
    ldi     r18,2 ; reads from PC2
    push    r16
    call    READ_JOY ; returns ADCH in r19
    pop     r16
    call    SEP_XY
check_up:
    cpi     r19,0b1100_0000 ; checks right
    brlo    check_down
    inc     r17
    rjmp    y_done
check_down:
    cpi     r19,0b0100_0000 ; checks left
    brsh    y_done
    dec     r17
y_done:
    call    JOIN_XY
    ret
    */