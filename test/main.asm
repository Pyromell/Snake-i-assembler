.equ    MOSI = PB3
.equ    SCK  = PB5
.equ    SS   = PB2    
.def    ZERO = r2

.equ	B = 0
.equ	G = 1
.equ	R = 2
.equ	A = 3
.equ    disp_size = 8

.dseg
VMEM:   .byte disp_size*4
P1:     .byte 2 ; x, y
P2:     .byte 2
.cseg

HW_INIT:
    ldi     r16,HIGH(RAMEND)
    out     SPH,r16
    ldi     r16,LOW(RAMEND)
    out     SPL,r16
    clr     ZERO

    ; Set MOSI and SCK output, all others input
    ldi     r17, (1<<MOSI)|(1<<SCK)|(1<<SS)
    out     DDRB,r17
    ; Enable SPI, Master, set clock rate fck/16
    ldi     r17,(1<<SPE)|(1<<MSTR)|(1<<SPR0)|(1<<SPR1)
    out     SPCR,r17


MAIN:
    ldi     r16,2
    ldi     r17,5
    call    FIRE
again:
    rjmp    again


ERASE_VMEM:
    push    ZH
    push    ZL
    push    r16
    ldi     ZH,HIGH(VMEM)
    ldi     ZL,LOW(VMEM)
    ldi     r16,disp_size*4
clear:
    st      Z+,ZERO
    dec     r16
    brne    clear
e_done:
    pop     r16
    pop     ZL
    pop     ZH
    ret

UPDATE_VMEM:
    push    ZH
    push    ZL
    push    r16

; player 1
    clr     ZH
    ldi     ZL,LOW(P1)
    ld      r16,Z+ ; x
    ld      r17,Z+ ; y

    push    ZH
    push    ZL
    ldi     ZH,HIGH(VMEM)
    ldi     ZL,LOW(VMEM)

    lsl     r17 ; * 2
    lsl     r17 ; * 2
    add     ZL,r17
    adc     ZH,ZERO
    call    COORD2BYTE
    ldd     r16,Z+R
    or      r17,r16
    std     Z+R,r17

    pop     ZL
    pop     ZH

u_done:
    pop     r16
    pop     ZL
    pop     ZH
    ret


COORD2BYTE: ; uses r16 and r17
    ldi     r17,0b1000_0000
    cpi     r16,0
    breq    c2b_done
c2b_loop:    
    lsr     r17
    dec     r16    
    brne    c2b_loop
c2b_done:
    ret




















FIRE:
    clr     r18

    push    r18
    call    SPI_TX
    pop     r18

    push    r18
    call    SPI_TX
    pop     r18

RED:
    ldi     r18,0b1000_0000
    cpi     r16,0
    breq    RDONE
RLOOP:    
    lsr     r18
    dec     r16    
    brne    RLOOP
RDONE:
    push    r18
    call    SPI_TX
    pop     r18

ANODE:
    ldi     r18,0b1000_0000
    cpi     r17,0
    breq    ADONE
ALOOP:    
    lsr     r18
    dec     r17    
    brne    ALOOP
    com     r18
ADONE:
    push    r18
    call    SPI_TX
    pop     r18

CLOCK:
    cbi     DDRB,SS
    sbi     DDRB,SS
    cbi     DDRB,SS

    ret
    

SPI_TX:
    push    r16
    in      ZH,SPH
    in      ZL,SPL
    ldd     r16,Z+4
    out     SPDR,r16
SPI_TX_WAIT:
    in	    r16,SPSR
    sbrs    r16,SPIF
    rjmp    SPI_TX_WAIT
    pop     r16
    ret
