.org 0x0000
	jmp	PROG_START

.org OVF0ADDR
	jmp	MULTIPLEX

.include "init.inc"
.include "vmem.inc"
.include "mux.inc"
.include "help.inc"

.dseg
    VMEM:   .byte 128
    P1:     .byte 2 ; x, y
    P2:     .byte 2 ; x, y
    FRUIT:  .byte 2 ; x, y
    LINE:   .byte 1
.cseg


PROG_START:
    call    INIT

SETUP:
; Initiates player positions
	ldi		ZL,LOW(P1)
	ldi		ZH,HIGH(P1)
	ldi		r16,4
	st		Z,r16		
	ldi		r17,15
	std		Z+1,r17

	ldi		ZL,LOW(P2)
	ldi		ZH,HIGH(P2)
	ldi		r16,15
	st		Z,r16		
	ldi		r17,0
	std		Z+1,r17

	call	ERASE_VMEM
	call	UPDATE_VMEM

	sei ; interrupt enabled


PLAY:
   ;call    READ_JOYSTICK
   ;call    UPDATE_POS
   ;call    CHECK_HIT
   ;call    ERASE_VMEM
   ;call    UPDATE_VMEM
   ;call    DELAY
    rjmp    PLAY
