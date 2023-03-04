READ_JOY:
; Reads from pin specified by MUX3..0 in r17
; JOY_RH = PC0, JOY_RV = PC1, JOY_LH = PC2, JOY_LV = PC3
; Returns ADCH in r16
    lds     r16,ADCSRA
    ori     r16,0b0100_0000 ; set ADSC
    sts     ADCSRA,r16
    lds     r16,ADMUX
    or      r16,r17 ; set MUXn = PCn from r17 = n
    sts     ADMUX,r16
adc_not_done: ; wait till conversion done
    lds     r16,ADCSRA
    sbrc    r16,ADSC
    rjmp    adc_not_done
adc_done:
    lds     r16,ADCH ; read 8 MSB
    ret


JOYSTICK_X:
    ldi     ZH,HIGH(P1)
    ldi     ZL,LOW(P1)

    ldi     r17,0 ; reads from PC0
    call    READ_JOY ; returns ADCH in r16
    ; checks right
    cpi     r16,0b1100_0000
    brlo    not_right
    ; moves right
    ld      r16,Z
    inc     r16
    st      Z,r16
    jmp     not_left
not_right:
    cpi     r16,0b0100_0000
    brsh    not_left
    ; moves left
    ld      r16,Z
    dec     r16
    st      Z,r16
not_left:
    ret


JOYSTICK_Y:
    ldi     ZH,HIGH(P1)
    ldi     ZL,LOW(P1)

    ldi     r17,3 ; reads from PC1
    call    READ_JOY  ; returns ADCH in r16
    ; checks up
    cpi     r16,0b1100_0000
    brlo    not_up
    ; moves up
    ldd      r16,Z+1
    inc      r16
    std      Z+1,r16
    jmp      not_down
not_up:
    cpi     r16,0b0100_0000
    brsh    not_down
    ; moves down
    ldd     r16,Z+1
    dec     r16
    std     Z+1,r16
not_down:
    ret