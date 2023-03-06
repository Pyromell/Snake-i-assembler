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
    ldi     r16,64
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
