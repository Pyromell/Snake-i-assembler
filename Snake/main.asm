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
.include "ljud.inc"

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

    cpi     r16,TIED
    breq    tied_screen
    cpi     r16,P1_WINS
    breq    p1_win_screen
    cpi     r16,P2_WINS
    breq    p2_win_screen

tied_screen:
p1_win_screen:
p2_win_screen:
    jmp     PROG_START