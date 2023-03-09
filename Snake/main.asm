; SPI pins
.equ    MOSI = PB3
.equ    SCK  = PB5
.equ    SS   = PB2
; LED colors
.equ    B = 0
.equ    G = 1
.equ    R = 2
.equ	A = 3
; Game constants
.equ	P1_COLOR = R
.equ	P2_COLOR = B
.equ	FRUIT_COLOR = G
.equ	MAX_LEN  = 64
; Controls
.equ    J1X = 2
.equ    J1Y = 3
.equ    J2X = 0
.equ    J2Y = 1
; Registers
.def    ZERO = r2

.org 0x0000
    jmp     PROG_START

.org OVF0ADDR
    jmp     MULTIPLEX

.include "help.inc"
.include "init.inc"
.include "vmem.inc"
.include "mux.inc"
.include "joystick.inc"

.dseg
    VMEM:   .byte 128
    LINE:   .byte 1

    P1:     .byte MAX_LEN ; $xy
    P1_LEN: .byte 1

    P2:     .byte MAX_LEN ; $xy
    P2_LEN: .byte 1

    FRUIT:  .byte 1	  ; $xy

    SEED:   .byte 1 ; upconunt
.cseg


PROG_START:
    ldi     r16,HIGH(RAMEND)
    out     SPH,r16
    ldi     r16,LOW(RAMEND)
    out     SPL,r16
    clr     ZERO
    call    INIT


SETUP:
; Initiates player positions
    ldi		ZL,LOW(P2)
	ldi		ZH,HIGH(P2)

	ldi		r16,$d7  // head with x = 13, y = 7
	st		Z,r16		
	ldi		r16,$e7  // tail with x = 14, y = 7
	std		Z+1,r16
	ldi		r16,$f7  // tail with x = 15, y = 7
	std		Z+2,r16
	ldi		r16,3
	sts		P1_LEN,r16		 // store lenght of the snake

	ldi		ZL,LOW(P1)
	ldi		ZH,HIGH(P1)

	ldi		r16,$28  // head with x = 2, y = 8
	st		Z,r16		
	ldi		r16,$18  // tail with x = 1, y = 8
	std		Z+1,r16
	ldi		r16,$08  // tail with x = 0, y = 8
	std		Z+2,r16
	ldi		r16,3
	sts		P2_LEN,r16		 // store lenght of the snake
    

    call    ERASE_VMEM
    call    UPDATE_VMEM

    sei ; interrupt enabled
    call    RANDOM

PLAY:
	call    MOVE_BODYS
    call    JOYSTICK_1
    call    JOYSTICK_2
    call    ERASE_VMEM
    call    UPDATE_VMEM
    call    DELAY
    call    HIT

    rjmp    PLAY

