ERASE_VMEM:
    push    ZH
    push    ZL
    ldi     ZH,HIGH(VMEM)
    ldi     ZL,LOW(VMEM)
    ldi     r16,128
clear_byte:
    st      Z+,ZERO
    dec     r16
    brne    clear_byte
    pop     ZL
    pop     ZH
    ret


UPDATE_VMEM:
    push    ZH
    push    ZL
    push    YH
    push    YL
update_p1:
    ldi     ZH,HIGH(P1)
    ldi     ZL,LOW(P1)
    lds     r20,P1_LEN
update_p1_loop:
    ld      r16,Z+
    ; sets Y to correct line in VMEM
    ; returns DAMatrix byte in r19
    call    POINT_AT_LINE
    ; adds player color at coord to that byte
    ldd     r16,Y+P1_COLOR
    or      r19,r16
    std     Y+P1_COLOR,r19
    dec     r20
    brne    update_p1_loop


update_p2:  ; same as p1
    ldi     ZH,HIGH(P2)
    ldi     ZL,LOW(P2)
    lds     r20,P2_LEN
update_p2_loop:
    ld      r16,Z+
    call    POINT_AT_LINE
    ldd     r16,Y+P2_COLOR
    or      r19,r16
    std     Y+P2_COLOR,r19
    dec     r20
    brne    update_p2_loop

update_fruit:
    ldi     ZH,HIGH(FRUIT)
    ldi     ZL,LOW(FRUIT)
    ld      r16,Z
    call    POINT_AT_LINE
    ldd     r16,Y+FRUIT_COLOR
    or      r19,r16
    std     Y+FRUIT_COLOR,r19

update_done:
    pop     YL
    pop     YH
    pop     ZL
    pop     ZH
    ret


POINT_AT_LINE:
; Sets Y to line with r16=x,y in VMEM.
; Returns byte to write in r19
    ldi     YH,HIGH(VMEM)
    ldi     YL,LOW(VMEM)
    call    SEP_XY ; x,y = r16,r17
    ; determine if upper display
    cpi     r17,8
    brlo    upper_lower_chosen
    ; move Y to upper display
    subi    r17,8
    ldi     r18,64
    add     YL,r18
    adc     YH,ZERO
upper_lower_chosen:
    ; Y += y * 4 to point at line
    lsl     r17
    lsl     r17
    add     YL,r17
    adc     YH,ZERO
    ; determine if right display
    cpi     r16,8
    brlo    left_right_chosen
    ; move Y to right display
    subi    r16,8
    ldi     r18,32
    add     YL,r18
    adc     YH,ZERO
left_right_chosen:
    ; convert x to byte for DAMatrix
    call    COORD2BYTE ; in r16, out r19
    ret