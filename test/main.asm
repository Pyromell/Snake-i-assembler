.equ    MOSI = PB3
.equ    SCK     = PB5
.equ    SS     = PB2    

.dseg
vmem:    .byte    36
.cseg

    ldi     r16,HIGH(RAMEND)
    out     SPH,r16
    ldi     r16,LOW(RAMEND)
    out     SPL,r16
    
SPI_INIT:
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
AGAIN:
    rjmp    AGAIN


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
    brne    FORLOOP
RDONE:
    push    r18
    call    SPI_TX
    pop     r18

ANODE:
    ldi     r18,0b1000_0000
    cpi     r17,0
    breq    DONE2
ALOOP:    
    lsr     r18
    dec     r17    
    brne    FORLOOP2
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
    in     ZH,SPH
    in     ZL,SPL
    ldd     r16,z+4
    out     SPDR,r16
SPI_TX_WAIT:
    in	    r16,SPSR
    sbrs    r16,SPIF
    rjmp    SPI_TX_WAIT
    pop     r16
    ret
