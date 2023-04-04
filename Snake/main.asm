; SPI pins

.equ    MOSI = PB3
.equ    SCK  = PB5
.equ    SS   = PB2
; LED colors
.equ    B = 0
.equ    G = 1
.equ    R = 2
.equ    A = 3
; Game constants
.equ    SPEED = 200 ; lower is faster
; Game mode 0 = boring game, 1 = fun game
.equ	GAME_MODE = 0

.equ    P1_COLOR = R
.equ    P2_COLOR = B
.equ    FRUIT_COLOR = G
.equ    MAX_LEN = 64
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

    FRUIT:  .byte 1     ; $xy
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
.include "hits.inc"
.include "ljud.inc"

PROG_START:
    ldi     r16,HIGH(RAMEND)
    out     SPH,r16
    ldi     r16,LOW(RAMEND)
    out     SPL,r16
    clr     ZERO


MAIN:
	; sets hardware, flags, clears memory
    call    INIT
    ; sets start conditions, game mode
    call    SETUP

	call    ERASE_VMEM
    call    UPDATE_VMEM
PLAY:
    

	
    call    JOYSTICK
	call    MOVE
	call    ERASE_VMEM
    call    UPDATE_VMEM

    call    DELAY    
	call	DELAY


	call    CHECK_HITS

    lds     r16,STATUS
    cp      r16,ZERO
    brne    GAME_OVER
    rjmp    PLAY

GAME_OVER:
    ldi     r16,30
    call    WAIT
    lds     r16,STATUS
    cpi     r16,TIED
    breq    tied_screen
    cpi     r16,P1_WINS
    breq    p1_win_screen
    cpi     r16,P2_WINS
    breq    p2_win_screen
tied_screen:
    call    ERASE_VMEM
    ldi     ZH,HIGH(TIE_TAB*2)
    ldi     ZL,LOW(TIE_TAB*2)
    rjmp    win_prep_done
p1_win_screen:
    call    ERASE_VMEM
    ldi     ZH,HIGH(RED_WIN_TAB*2)
    ldi     ZL,LOW(RED_WIN_TAB*2)
    rjmp    win_prep_done
p2_win_screen:
    call    ERASE_VMEM
    ldi     ZH,HIGH(BLUE_WIN_TAB*2)
    ldi     ZL,LOW(BLUE_WIN_TAB*2)
win_prep_done:
    call    UPDATE_WIN_VMEM
    ldi     r16,100
    call    WAIT
	call	LJUD
    jmp     MAIN
