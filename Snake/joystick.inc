READ_JOY:
; Reads from pin specified by MUX3..0 in r18
; JOY_RH = PC0, JOY_RV = PC1,
; JOY_LH = PC2, JOY_LV = PC3
; Returns ADCH in r19
    lds     r16,ADCSRA
    ori     r16,0b0100_0000 ; set ADSC
    sts     ADCSRA,r16
    ldi     r16,0b0110_0000
    or      r16,r18 ; set MUXn = PCn from r18 = n
    sts     ADMUX,r16
adc_not_done: ; wait till conversion done
    lds     r16,ADCSRA
    sbrc    r16,ADSC
    rjmp    adc_not_done
adc_done:
    lds     r19,ADCH ; read 8 MSB
    ret


JOYSTICK_1:
; TODO: Try replacing Z pointer
;       usage with lds and sts.
    
    ; x-direction
    ldi     r18,2 ; reads from PC2
    call    READ_JOY ; returns ADCH in r19
    lds     r16,P1
    call    SEPARATE_XY
    cpi     r19,0b1100_0000 ; checks right
    brlo    not_right
    inc     r16
    jmp     all_done
not_right:
    cpi     r19,0b0100_0000 ; checks left
    brsh    not_left
    dec     r16
    rjmp    all_done
not_left:
    ; y-direction
    ldi     r18,3 ; reads from PC3
    call    READ_JOY  ; returns ADCH in r19
    lds     r16,P1
    call    SEPARATE_XY
    cpi     r19,0b1100_0000 ; checks up
    brlo    not_up
    inc     r17
    jmp     all_done
not_up:
    cpi     r19,0b0100_0000 ; checks down
    brsh    all_done
    dec     r17
all_done:
    call    LIMITS
    ; writes back to SRAM
    call    JOIN_XY
    st      Z,r16
    ret


LIMITS:
; Takes x,y = r16,r17 and
; moves them to other side of
; screen if outside.
    sbrc    r16,4 ; x != 16
    ldi     r16,0
    sbrc    r16,7 ; x != 255
    ldi     r16,15
    sbrc    r17,3 ; y != 16
    ldi     r17,0
    sbrc    r17,7 ; y != 255
    ldi     r17,15
    ret

/*
MOVE_P1_BODY:
    ldi     ZH,HIGH(P1)
    ldi     ZL,LOW(P1)
    ld      r16,Z+ ; get past head
    lds     r20,P1_LEN
move_p1_loop:
    ld      r16,Z
    st      Z+,r16
    dec     r20
    brne    move_p1_loop
    ret
*/