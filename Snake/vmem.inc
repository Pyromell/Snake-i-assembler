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

update_p1:
    ldi     ZH,HIGH(P1)
    ldi     ZL,LOW(P1)
    ld      r16,Z+ ; x
    ld      r17,Z  ; y

    ; sets Y to correct line in VMEM
    ; returns byte in r17
    call    POINT_AT_LINE
    ; adds red at coord to that byte
    ldd     r16,Y+R
    or      r17,r16
    std     Y+R,r17

update_p2:
    ldi     ZH,HIGH(P2)
    ldi     ZL,LOW(P2)
    ld      r16,Z+ ; x
    ld      r17,Z  ; y

    ; sets Y to coord in VMEM
    ; returns byte in r17
    call    POINT_AT_LINE
    ; adds blue at coord to that byte
    ldd     r16,Y+B
    or      r17,r16
    std     Y+B,r17

updates_done:
    pop     ZL
    pop     ZH
    ret


POINT_AT_LINE:
; Sets Y to line with (x, y) = (r16, r17) in VMEM.
; Returns byte to write in r17

    ldi     YH,HIGH(VMEM)
    ldi     YL,LOW(VMEM)

    ; determine if upper or lower display
    cpi     r17,8     
    brlo    upper_lower_chosen
    ; move to upper display
    subi    r17,8
    ldi     r18,64
    add     YL,r18
    adc     YH,ZERO

upper_lower_chosen:
    lsl     r17
    lsl     r17
    add     YL,r17
    adc     YH,ZERO
    
    ; determine if left or right display
    cpi     r16,8
    brlo    left_right_chosen
    ; move to right display
    subi    r16,8
    ldi     r18,32
    add     YL,r18
    adc     YH,ZERO

left_right_chosen:
    call    COORD2BYTE ; in r16, out r17
    ret