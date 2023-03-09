JOYSTICK:
    ldi     r18,JOY_1
    lds     r16,P1_DIR
    call    UPDATE_DIR
    sts     P1_DIR,r16
    
    ldi     r18,JOY_2
    lds     r16,P2_DIR
    call    UPDATE_DIR
    sts     P2_DIR,r16
    ret


UPDATE_DIR:
; Performs ADC conversion and
; updates direction in SRAM
    push    r16 ; store dir
    clr     r17 ; temporary direction
    ; x-direction
    call    READ_JOY ; returns ADCH in r19
    cpi     r19,0b1100_0000 ; checks right
    brlo    not_right
    sbr     r17,(1 << RIGHT)
    rjmp    not_left
not_right:
    cpi     r19,0b0100_0000 ; checks left
    brsh    not_left
    sbr     r17,(1 << LEFT)
not_left:
    ; y-direction
    inc     r18 ; change to y-channel
    call    READ_JOY ; returns ADCH in r19
    cpi     r19,0b1100_0000 ; checks up
    brlo    not_up
    sbr     r17,(1 << UP)
    rjmp    all_done
not_up:
    cpi     r19,0b0100_0000 ; checks down
    brsh    all_done
    sbr     r17,(1 << DOWN)
all_done:
    pop     r16 ; restore dir
    ; check dir ok here
    cpse    r17,ZERO
    mov     r16,r17
    ret


READ_JOY:
; Reads from pin specified by MUX3..0 in r18
; Returns ADCH in r19    
    ldi     r16,0b0110_0000 ; load default ADMUX
    or      r16,r18 ; set MUXn = PCn from r18 = n
    sts     ADMUX,r16
    lds     r16,ADCSRA
    ori     r16,0b0100_0000 ; set ADSC
    sts     ADCSRA,r16
adc_not_done: ; wait till conversion done
    lds     r16,ADCSRA
    sbrc    r16,ADSC
    rjmp    adc_not_done
adc_done:
    lds     r19,ADCH ; read 8 MSB
    ret