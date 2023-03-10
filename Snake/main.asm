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
.equ    SPEED = 10 ; lower is faster
.equ	P1_COLOR = R
.equ	P2_COLOR = B
.equ	FRUIT_COLOR = G
.equ	MAX_LEN = 64
.equ    P1_WINS = 1
.equ    P2_WINS = 2
.equ    TIED    = 3
; Controls
.equ    JOY_1 = 2
.equ    JOY_2 = 0
.equ    RIGHT = 0
.equ    UP    = 1
.equ    LEFT  = 2
.equ    DOWN  = 3
; Registers
.def    ZERO = r2


.dseg ; SRAM
    VMEM:   .byte 128
    LINE:   .byte 1

    P1:     .byte MAX_LEN ; $xy
    P1_LEN: .byte 1
    P1_DIR: .byte 1 ; $0b0000_dlur

    P2:     .byte MAX_LEN ; $xy
    P2_LEN: .byte 1
    P2_DIR: .byte 1 ; $0b0000_dlur

    FRUIT:  .byte 1	; $xy
    SEED:   .byte 1 ; for random

    STATUS: .byte 1
.cseg


.org 0x0000
    jmp     PROG_START
.org OVF0ADDR
    jmp     MULTIPLEX


; Includes
.include "help.inc"
.include "init.inc"
.include "vmem.inc"
.include "mux.inc"
.include "joystick.inc"
.include "move.inc"
.include "screen.inc"
;.include "ljud.inc"

PROG_START:
    ldi     r16,HIGH(RAMEND)
    out     SPH,r16
    ldi     r16,LOW(RAMEND)
    out     SPL,r16
    clr     ZERO

    ; sets hardware, flags, clears memory
    call    INIT
    ; sets start conditions, game mode
    call    SETUP
PLAY:
    call    ERASE_VMEM
    call    UPDATE_VMEM
    call    CHECK_HITS
    call    JOYSTICK
	call    MOVE
    call    DELAY
    ; game over?
    lds     r16,STATUS
    cp      r16,ZERO
    brne    GAME_OVER
    ; not over
    rjmp    PLAY

GAME_OVER:
    call    DELAY
    call    DELAY
    call    DELAY
    call    DELAY

    lds     r16,STATUS
    cpi     r16,TIED
    breq    tied_screen
    cpi     r16,P1_WINS
    breq    p1_win_screen
    cpi     r16,P2_WINS
    breq    p2_win_screen

tied_screen:
    call    ERASE_VMEM
    ldi		ZH,HIGH(TIE_TAB*2)
	ldi		ZL,LOW(TIE_TAB*2)
    rjmp    WIN_PREP_DONE

p1_win_screen:
	call	ERASE_VMEM
	ldi		ZH,HIGH(RED_WIN_TAB*2)
	ldi		ZL,LOW(RED_WIN_TAB*2)
    rjmp    WIN_PREP_DONE

p2_win_screen:
	call	ERASE_VMEM
	ldi		ZH,HIGH(BLUE_WIN_TAB*2)
	ldi		ZL,LOW(BLUE_WIN_TAB*2)

WIN_PREP_DONE:
	call	UPDATE_WIN_VMEM
	call	DELAY
	call	DELAY
	call	DELAY
	call	DELAY
	call	DELAY
	call	DELAY
	call	DELAY
    jmp     PROG_START

	
RED_WIN_TAB: 
	.db		$36, $37, $38, $39, $3A, $4A, $59, $58, $48, $47, $56, $56	//R 
	.db		$76, $77, $78, $79, $7A, $8A, $88, $86, $9A, $98, $96, $96  //E
	.db		$B6, $B7, $B8, $B9, $BA, $C6, $CA, $D7, $D8, $D9, $00, $00	//D
	
BLUE_WIN_TAB:
	.db		$16, $17, $18, $19, $1A, $26, $28, $2A, $37, $39			//B
	.db		$56, $57, $58, $59, $5A, $66, $76, $76						//L
	.db		$96, $97, $98, $99, $9A, $A6, $B6, $B7, $B8, $B9, $BA, $BA	//U
	.db     $D6, $D7, $D8, $D9, $DA, $E6, $E8, $EA, $F6, $F8, $FA, $00  //E
TIE_TAB:
	.db     $3A, $46, $47, $48, $49, $4A, $5A, $5A                      //T
    .db     $76, $77, $78, $79, $7A, $7A                                //I
    .db     $96, $97, $98, $99, $9A, $A6, $A8, $AA, $B6, $B8, $BA, $00  //E
	