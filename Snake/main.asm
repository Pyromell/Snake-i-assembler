.equ    MOSI = PB3
.equ    SCK  = PB5
.equ    SS   = PB2
.equ    B    = 0
.equ    G    = 1
.equ    R    = 2
.equ	A    = 3
.def    ZERO = r2
 clr    ZERO

.org 0x0000
    jmp     PROG_START

.org OVF0ADDR
    jmp     MULTIPLEX

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
    ldi     r16,HIGH(RAMEND)
    out     SPH,r16
    ldi     r16,LOW(RAMEND)
    out     SPL,r16
    call    INIT

SETUP:
; Initiates player positions
    ldi     ZL,LOW(P1)
    ldi     ZH,HIGH(P1)
    ldi     r16,0
    st      Z,r16
    ldi     r17,0
    std     Z+1,r17

    ldi     ZL,LOW(P2)
    ldi     ZH,HIGH(P2)
    ldi     r16,15
    st      Z,r16
    ldi     r17,15
    std     Z+1,r17

    call    ERASE_VMEM
    call    UPDATE_VMEM

    sei ; interrupt enabled


PLAY:
    call    JOYSTICK
    call    ERASE_VMEM
    call    UPDATE_VMEM
    call    DELAY
    rjmp    PLAY


JOYSTICK:
; MUX3..0 binary value determines PC0..PC5 = A0..A5
; JOY_RH = PC0 = MUX[0000]
; JOY_RV = PC1 = MUX[0001]
; JOY_LH = PC2 = MUX[0010]
; JOY_LV = PC3 = MUX[0100]
; 1. Set ADSC
; 2. Wait till ADSC cleared
; 3. Read from ADCH, 8 MSB
    
    ; horizontal
    ldi     r17,0b000 ; set MUX3..0
    call    READ_JOY ; returns ADCH in r16
    ldi     ZH,HIGH(P1)
    ldi     ZL,LOW(P1)

    ; check right
    cpi     r16,0b1100_0000
    brlo    not_right
    ; move right
    ld      r16,Z
    inc     r16
    st      Z,r16
    rjmp    horizontal_done
not_right:
    cpi     r16,0b0100_0000
    brsh    horizontal_done
    ; move left
    ld      r16,Z
    dec     r16
    st      Z,r16
horizontal_done:
    ; vertical
    ldi     r17,0b001 ; set MUX3..0
    call    READ_JOY ; returns ADCH in r16
    ldi     ZH,HIGH(P1)
    ldi     ZL,LOW(P1)
    ; check up
    cpi     r16,0b1100_0000
    brlo    not_up
    ; move up
    ldd     r16,Z+1
    inc     r16
    std     Z+1,r16
    rjmp    vertical_done
not_up:
    cpi     r16,0b0100_0000
    brsh    vertical_done
    ; move left
    ldd     r16,Z+1
    dec     r16
    std     Z+1,r16
vertical_done:
    ret


READ_JOY:
; Reads from pin specified in by MUX3..0 in r17,
; Returns ADCH in r16
    lds     r16,ADCSRA
    ori     r16,0b0100_0000 ; set ADSC
    sts     ADCSRA,r16
    lds     r16,ADMUX
    or      r16,r17 ; set MUX3..0 from r17
    sts     ADMUX,r16
adc_not_done: ; wait till conversion done
    lds     r16,ADCSRA
    sbrc    r16,ADSC
    rjmp    adc_not_done
adc_done:
    clr     r16
    lds     r16,ADCH ; read 8 MSB
    ret