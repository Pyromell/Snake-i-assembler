LJUD:
ldi     r16,1
out     DDRB,r16

.dseg
  BEEP_PITCH_1:     .byte 1
  BEEP_PITCH_2:     .byte 1
  BEEP_LENGTH:      .byte 1
.cseg

ldi     r16,50
sts     BEEP_LENGTH,r16
call NOTE_G
call NOTE_G
call NOTE_D
call NOTE_D
call NOTE_E
call NOTE_E
call NOTE_D ;  halv not
call NOTE_C
call NOTE_C
call NOTE_B
call NOTE_B
call NOTE_A
call NOTE_A
call NOTE_G ; Halv not
call NOTE_D
call NOTE_D
call NOTE_C
call NOTE_C
call NOTE_B
call NOTE_B
call NOTE_A ; Halv not
call NOTE_D
call NOTE_D
call NOTE_C
call NOTE_C
call NOTE_B
call NOTE_B
call NOTE_A ; Halv not
call NOTE_G
call NOTE_G
call NOTE_D
call NOTE_D
call NOTE_E
call NOTE_E
call NOTE_D ; Halv not
call NOTE_E
call NOTE_E
call NOTE_D
call NOTE_C
call NOTE_C
call NOTE_B
call NOTE_B
call NOTE_A
call NOTE_A
call NOTE_G ; Halv not


NOTE_A:      ; 3520 Hz
    ldi     r16,60
    sts     BEEP_PITCH_1,r16
    ldi     r16,1
    sts     BEEP_PITCH_2,r16
    call    BEEP
    ret
NOTE_B:      ; 3951 Hz
    ldi     r16,50
    sts     BEEP_PITCH_1,r16
    ldi     r16,1
    sts     BEEP_PITCH_2,r16
    call    BEEP
    ret
NOTE_C:      ; 2093 HZ
    ldi     r16,100
    sts     BEEP_PITCH_1,r16
    ldi     r16,1
    sts     BEEP_PITCH_2,r16
    call    BEEP
    ret
NOTE_D:      ; 2489 HZ
    ldi     r16,90
    sts     BEEP_PITCH_1,r16
    ldi     r16,1
    sts     BEEP_PITCH_2,r16
    call    BEEP
    ret

NOTE_E:      ; 2637 HZ
    ldi     r16,80
    sts     BEEP_PITCH_1,r16
    ldi     r16,1
    sts     BEEP_PITCH_2,r16
    call    BEEP
    ret

NOTE_G:      ; 3136 Hz
    ldi     r16,200
    sts     BEEP_PITCH_1,r16
    ldi     r16,1
    sts     BEEP_PITCH_2,r16
    call    BEEP
    ret
  

 
    ; --- BEEP(r16) r16 halfcycles of BEEP-PITCH
BEEP:
    lds     r20,BEEP_LENGTH

LOOP2:
    ldi     r22,2

LOOP3:
	lds     r16,BEEP_PITCH_1  ; Load period of square wave
    sbi     PORTB,1           ; Rising edge
    call    DELAY_SOUND       ; Hold
    cbi     PORTB,1           ; Falling edge
    call    DELAY_SOUND       ; Hold

    dec     r22
    brne    LOOP3
	dec     r20
    brne    LOOP2

    ret

DELAY_SOUND:
    ldi     r17,100

DELAY_INNER:
    dec     r17
    brne    DELAY_INNER
    dec     r16
    brne    DELAY_SOUND
    ret
