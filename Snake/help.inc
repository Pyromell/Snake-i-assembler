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
    ldi     r16,16
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


RANDOM:
RE_TRY:    
    lds     r16,SEED
    ldi     ZH,HIGH(P1)
    ldi     ZL,LOW(P1)
    lds     r17,P1_LEN
    
    RANDOM_LOOP1:
    ld      r18,Z+
    cp      r18,r16
    breq    RE_TRY   
    dec     r17
    brne    RANDOM_LOOP1

    ldi     ZH,HIGH(P2)
    ldi     ZL,LOW(P2)
    lds     r17,P2_LEN

    RANDOM_LOOP2:
    ld      r18,Z+
    cp      r18,r16
    breq    RE_TRY   
    dec     r17
    brne    RANDOM_LOOP2

    sts     FRUIT,r16
    ret



HIT:
    
    ldi     ZH,HIGH(P1)
    ldi     ZL,LOW(P1)

    ldi     YH,HIGH(P2)
    ldi     YL,LOW(P2)
   
    ld      r16,Z
    ld      r17,Y

    cp      r16,r17
    breq    TIE
    lds     r18,P2_LEN

COLIDE_P2_LOOP:
    ld      r17,Y+
    cp     r16,r17  
    breq    P2_WIN

    dec     r18
    brne    COLIDE_P2_LOOP

    ldi     YH,HIGH(P2)
    ldi     YL,LOW(P2)
    // RESETING Y TO POINT AT HEAD

COLIDE_P1_LOOP:
    ld      r16,Z+
    cp      r16,r17    
    breq    P1_WIN
    dec     r18
    brne    COLIDE_P1_LOOP
   
    rjmp    SNAKE_HIT_DONE

// Exercise for the reader :D
TIE:
P1_WIN:
P2_WIN:        

SNAKE_HIT_DONE:

    ldi     ZH,HIGH(P1)
    ldi     ZL,LOW(P1)
	ldi		YH,HIGH(P2)
	ldi		YL,LOW(P2)

    ld      r16,Z
    ld      r17,Y
    lds     r18,FRUIT
    
    cp      r18,r16
    brne    P1_NO_FRUIT
        
    lds     r16,P1_LEN
    inc     r16
    sts     P1_LEN,r16  
    call    RANDOM
P1_NO_FRUIT:

    cp      r18,r17
    brne    P2_NO_FRUIT

    lds     r17,P1_LEN
    inc     r17
    sts     P2_LEN,r17
    call    RANDOM
P2_NO_FRUIT:

    ret


