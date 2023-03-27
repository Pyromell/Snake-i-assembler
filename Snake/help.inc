SETUP:
; Initiates player positions
    ; player 1
	call	data_init
    ldi		ZL,LOW(P1)
	ldi		ZH,HIGH(P1)
    ; head
	ldi		r16,$48
	st		Z,r16
    ; body
	ldi		r16,$38
	std		Z+1,r16
	ldi		r16,$28
	std		Z+2,r16
    ldi		r16,$18
	std		Z+3,r16
    ldi		r16,$08
	std		Z+4,r16
    ; length
	ldi		r16,5
	sts		P1_LEN,r16
    ; direction
    clr     r16
    sbr     r16,(1 << RIGHT)
    sts     P1_DIR,r16

    ; player 2
    ldi		ZL,LOW(P2)
	ldi		ZH,HIGH(P2)
    ; head
	ldi		r16,$b7
	st		Z,r16
    ; body
	ldi		r16,$c7
	std		Z+1,r16
	ldi		r16,$d7
	std		Z+2,r16
	ldi		r16,$e7
	std		Z+3,r16
	ldi		r16,$f7
	std		Z+4,r16
    ; length
	ldi		r16,5
	sts		P2_LEN,r16
    ; direction
    clr     r16
    sbr     r16,(1 << LEFT)
    sts     P2_DIR,r16

    ; clear and place
    sei     ; interrupt enabled
    call    PLACE_FRUIT
    ret


data_init:
    ldi     ZL,LOW(P1)
    ldi     ZH,HIGH(P1)
    ldi     r16,MAX_LEN
clear_p1_loop:
    st      Z+,ZERO 
    dec     r16
    brne    clear_p1_loop
    sts     P1_LEN,ZERO
    
    ldi     ZL,LOW(P2)
    ldi     ZH,HIGH(P2)
    ldi     r16,MAX_LEN
clear_p2_loop:
    st      Z+,ZERO
    dec     r16
    brne    clear_p2_loop
    sts     P2_LEN,ZERO

    ldi     ZL,LOW(FRUIT)
    ldi     ZH,HIGH(FRUIT)
    st      Z,ZERO

    sts     LINE,ZERO
    sts     STATUS,ZERO
    ret
 
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
    ldi     r16,SPEED
delay0:
    ldi     r17,32
delay1:
    ldi     r18,32
delay2:
    dec     r18
    brne    delay2
    dec     r17
    brne    delay1
    dec     r16
    brne    delay0
    ret

WAIT:
    push    r16
    call    DELAY
    pop     r16
    dec     r16
    brne    WAIT
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


PLACE_FRUIT:
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