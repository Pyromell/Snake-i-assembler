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

.equ	xtal = $A3
.equ	control_status_2 = $01
.equ	timer_control = $0E
.equ	timer_register = $0F

//-----INTERUPT--------/

.org 0x0000
	jmp	HW_INIT
.org OVF0ADDR
	jmp	INTERRUPT


.dseg
VMEM:   .byte 128
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
    ldi     r17,(1<<SPE)|(1<<MSTR)|(0<<SPR0)|(0<<SPR1)
    out     SPCR,r17

	ldi		r17,(0<<SPI2X)
	out		SPSR,r17



	
	
INT_INIT:
	
	
	ldi		r16,(0<<COM0A1)|(0<<COM0A0)|(0<<WGM01)|(0<<WGM00)
	out		TCCR0A,r16

	ldi		r16,(0<<WGM02)|(0<<CS02)|(1<<CS01)|(1<<CS00)
	out		TCCR0B,r16
	
	ldi		r16,(1<<TOIE0)
	sts		TIMSK0,r16

DATA_INIT:
    ldi		ZL,LOW(P1)
	ldi		ZH,HIGH(P1)
    st      Z+,ZERO
    st      Z,ZERO

	ldi		ZL,LOW(P2)
	ldi		ZH,HIGH(P2)
    st      Z+,ZERO
    st      Z,ZERO

    sts     LINE,ZERO
    

START:
	push	ZL
	push	ZH

	ldi		ZL,LOW(P1)
	ldi		ZH,HIGH(P1)

	ldi		r16,4  // X
	st		Z,r16		
	ldi		r17,15    // Y
	std		Z+1,r17
	
	ldi		ZL,LOW(P2)
	ldi		ZH,HIGH(P2)

	ldi		r16,15    // X
	st		Z,r16		
	ldi		r17,0   // Y
	std		Z+1,r17
	
	call	ERASE_VMEM
	call	UPDATE_VMEM

	pop		ZH
	pop		ZL

	//INTERUPT_ENABLE//
	sei
	


MAIN:
	

again:
    rjmp    again
    .include "twisend.inc"




//-----------------------//
INTERRUPT:
	push	r16
	in		r16,SREG
	push	r16

	push	ZH
	push	ZL
	push	r17
	push	r18
	push	r19

	ldi		ZL,LOW(VMEM)
	ldi		ZH,HIGH(VMEM)
	ldi		r17,96

	add		ZL,r17
	adc		ZH,ZERO
	
	lds		r16,LINE
	
	andi	r16,7
	mov		r17,r16

	lsl     r17 ; line * 4
    lsl     r17
    add     ZL,r17 ; titta på line
    adc     ZH,ZERO
    
	call    COORD2BYTE ; line i r16 som arg, ut i r17
	
	cbi     PORTB,SS   // denna gör att alla cordinater fungerar

	ldi		r18,4
DISPLOOP:
	
	ldd		r16,Z+B	
    push	r16
	call	SPI_TX
	pop		r16
	
	ldd		r16,Z+G


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
	com		r17

	ldi		r19,32			// Titta på disp innan
	sub		ZL,r19
	sbc		ZH,ZERO
	
	dec		r18				// Loop för en rad för vaje skärm
	brne	DISPLOOP
	
	sbi     PORTB,SS		// LATCH

	lds		r16,LINE
	inc		r16
	sts		LINE,r16

	pop		r19
	pop		r18
	pop		r17
	pop		ZL
	pop		ZH

	pop		r16
	out		SREG,r16
	pop		r16

	reti

DELAY:
	push	r16
	push	r17
	push	r18

	ldi		r16,150
DELAY1:
	ldi		r17,150
DELAY2:
	ldi		r18,150
DELAY3:
	dec		r18
	brne	DELAY3
	dec		r17
	brne	DELAY2
	dec		r16
	brne	DELAY1

	pop		r18
	pop		r17
	pop		r16

	ret
ERASE_VMEM:
    push    ZH
    push    ZL
    push    r16
    ldi     ZH,HIGH(VMEM)
    ldi     ZL,LOW(VMEM)
    ldi     r16,128
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
	push	r17
	push	r18

UPDATE_P1:
    ldi		ZH,HIGH(P1)
    ldi     ZL,LOW(P1)

    ld      r16,Z+ ; x
    ld      r17,Z ; y
   
    push    ZH
    push    ZL
	call	ADD_TO_VMEM
    ldd     r16,Z+R
    or      r17,r16
    std     Z+R,r17
	pop     ZL
    pop     ZH

	UPDATE_P2:
    ldi		ZH,HIGH(P2)
    ldi     ZL,LOW(P2)

    ld      r16,Z+ ; x
    ld      r17,Z ; y
   
    push    ZH
    push    ZL
	call	ADD_TO_VMEM
    ldd     r16,Z+B
    or      r17,r16
    std     Z+B,r17
	pop     ZL
    pop     ZH
u_done:
	pop		r18
	pop		r17
    pop     r16
    pop     ZL
    pop     ZH
    ret

ADD_TO_VMEM:
	ldi     ZH,HIGH(VMEM)
    ldi     ZL,LOW(VMEM)

	; determine y display
	cpi		r17,8     
	brlo	P1Y_CHOSEN
	; move to upper display
	subi	r17,8
	ldi		r18,64
	add		ZL,r18
	adc		ZH,ZERO			
P1Y_CHOSEN:
    lsl     r17 ; * 2
    lsl     r17 ; * 2
    add     ZL,r17
    adc     ZH,ZERO ; klar med r17
    
	
	; determine x display
	cpi		r16,8
	brlo	P1X_CHOSEN
	subi	r16,8
	ldi		r18,32
	add		ZL,r18
	adc		ZH,ZERO
P1X_CHOSEN:

	call    COORD2BYTE  ; IN r16, OUT r17

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
