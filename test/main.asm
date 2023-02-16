.equ    MOSI = PB3
.equ    SCK  = PB5
.equ    SS   = PB2    
.def    ZERO = r2
.equ    SCL = PC5
.equ    SDA = PC4

.equ	B = 0
.equ	G = 1
.equ	R = 2
.equ	A = 3
.equ    disp_size = 8

.equ	xtal = $A3
.equ	control_status_2 = $01
.equ	timer_control = $0E
.equ	timer_register = $0F

//-----INTERUPT_TEST--------/
/*
.org 0x0000
	jmp	HW_INIT
.org 0x0002
	jmp	INTERRUPT

-------------------------*/

.dseg
VMEM:   .byte 32
P1:     .byte 2 ; x, y
P2:     .byte 2
LINE:   .byte 1
.cseg

.macro  send
    push    @0
    push    @1
    call    TWI_SEND
    pop     @1
    pop     @0
.endmacro


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
/*
INT_INIT:
	ldi		r16,(1<<INT0)
	out		EIMSK,r16
	ldi		r16,(1<<ISC01) | (1<<ISC00)
	sts		EICRA,r16
    sei

RTC_INIT:
    ldi		r17,xtal

	; enables timer, 4 kHz
	ldi		r18,timer_control
	send	r17,r18
	ldi		r18,0b1000_0011 
	send	r17,r18

	; enables int, timer flag, timer int
	ldi		r18,control_status_2
	send	r17,r18
	ldi		r18,0b00010101
	send	r17,r18

	; loads timer
	ldi		r18,timer_register
	send	r17,r18
	ldi		r18,$ff
	send	r17,r18
    */

DATA_INIT:
    ldi		ZL,LOW(P1)
	ldi		ZH,HIGH(P1)
    st      Z+,ZERO
    st      Z,ZERO
    sts     LINE,ZERO
    

MAIN:
	push	ZL
	push	ZH

	ldi		ZL,LOW(P1)
	ldi		ZH,HIGH(P1)

	ldi		r16,3    // X
	st		Z,r16		
	ldi		r17,1    // Y
	std		Z+1,r17

	call	ERASE_VMEM
	call	UPDATE_VMEM

	ldi		r16,8
	push	r16
TEST_LOOP:
    call    INTERRUPT
	pop		r16
	dec		r16
	brne	TEST_LOOP

	pop		ZH
	pop		ZL

again:
    rjmp    again
    .include "twisend.inc"




//-----------------------//
INTERRUPT:
	push	ZH
	push	ZL
	push	r16
	push	r17

	ldi		ZL,LOW(VMEM)
	ldi		ZH,HIGH(VMEM)
	

	// av någon efterbliven andledning så ville inte skit adressen LINE funka för att lagra vilken line vi e på
	// så denna kollar rad 1 och den funkar och kan ändra på 1:an till 0-7 för att få correct data på den linjen
	// ps LINE SUGER BALLE
	ldi		r16,1
	ldi		r17,1

    lsl     r17 ; line * 4
    lsl     r17
    add     ZL,r17 ; titta på line
    adc     ZH,ZERO
    
    call    COORD2BYTE ; line i r16 som arg, ut i r17
FORLOOP7:
	ldd		r16,Z+G
    push	r16
	call	SPI_TX
	pop		r16
    
	ldd		r16,Z+B
    push	r16
	call	SPI_TX
	pop		r16

	ldd		r16,Z+R
	push	r16
	call	SPI_TX
	pop		r16

    com     r17
	push	r17
	call	SPI_TX
	pop		r17

    call	CLOCK

	pop		r17
	pop		r16
	pop		ZL
	pop		ZH
	ret

COORD_TEST:
    ldi     r16,2
    ldi     r17,5
    call    FIRE
	ret

ERASE_VMEM:
    push    ZH
    push    ZL
    push    r16
    ldi     ZH,HIGH(VMEM)
    ldi     ZL,LOW(VMEM)
    ldi     r16,32
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
    ldi		ZH,HIGH(P1)
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
    adc     ZH,ZERO ; klar med r17
    call    COORD2BYTE  ; omvandla r16
 
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
    push    ZH
    push    ZL
    push    r16

    in      ZH,SPH
    in      ZL,SPL
    ldd     r16,Z+6
    out     SPDR,r16
SPI_TX_WAIT:
    in	    r16,SPSR
    sbrs    r16,SPIF
    rjmp    SPI_TX_WAIT

    pop     r16
    pop     ZL
    pop     ZH
    ret
