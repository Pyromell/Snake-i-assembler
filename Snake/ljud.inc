.dseg
  BEEP_PITCH_1:     .byte 1
  BEEP_PITCH_2:     .byte 1
  BEEP_LENGTH:      .byte 1
.cseg

ldi     r16,50
sts     BEEP_LENGTH,r16
call G
call G
call D
call D
call E
call E
call D ;  halv not
call C
call C
call B
call B
call A
call A
call G ; Halv not
call D
call D
call C
call C
call B
call B
call A ; Halv not
call D
call D
call C
call C
call B
call B
call A ; Halv not
call G
call G
call D
call D
call E
call E
call D ; Halv not
call E
call E
call D
call C
call C
call B
call B
call A
call A
call G ; Halv not


A:      ; 3520 Hz
    ldi     r16,60
    sts     BEEP_PITCH_1,r16
    ldi     r16,1
    sts     BEEP_PITCH_2,r16
    call    BEEP
    ret
B:      ; 3951 Hz
    ldi     r16,50
    sts     BEEP_PITCH_1,r16
    ldi     r16,1
    sts     BEEP_PITCH_2,r16
    call    BEEP
    ret
C:      ; 2093 HZ
    ldi     r16,100
    sts     BEEP_PITCH_1,r16
    ldi     r16,1
    sts     BEEP_PITCH_2,r16
    call    BEEP
    ret
D:      ; 2489 HZ
    ldi     r16,90
    sts     BEEP_PITCH_1,r16
    ldi     r16,1
    sts     BEEP_PITCH_2,r16
    call    BEEP
    ret

E:      ; 2637 HZ
    ldi     r16,80
    sts     BEEP_PITCH_1,r16
    ldi     r16,1
    sts     BEEP_PITCH_2,r16
    call    BEEP
    ret

G:      ; 3136 Hz
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
    sbi     PORTB,1         ; Rising edge
    call    DELAY           ; Hold
    cbi     PORTB,1         ; Falling edge
    call    DELAY           ; Hold

    dec     r22
    brne    LOOP3
	dec     r20
    brne    LOOP2

    ret

DELAY:
    ldi     r17,100

DELAY_INNER:
    dec     r17
    brne    DELAY_INNER
    dec     r16
    brne    DELAY
    ret
