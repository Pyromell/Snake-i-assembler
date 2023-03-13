MOVE:
    call   MOVE_BODYS
    lds    r16,P1
    lds    r18,P1_DIR
    call   MOVE_HEAD
    sts    P1,r16
    lds    r16,P2
    lds    r18,P2_DIR
    call   MOVE_HEAD
    sts    P2,r16
    ret


MOVE_HEAD:
    call    SEP_XY ; r16 = x, r17 = y
    sbrc    r18,RIGHT
    inc     r16
    sbrc    r18,LEFT
    dec     r16
    sbrc    r18,UP
    inc     r17
    sbrc    r18,DOWN
    dec     r17
    call    LIMITS
    call    JOIN_XY
    ret


MOVE_BODYS:
    push    ZH
    push    ZL
    ldi     ZH,HIGH(P1)
    ldi     ZL,LOW(P1)
    lds     r16,P1_LEN
    call    MOVE_BODY
    ldi     ZH,HIGH(P2)
    ldi     ZL,LOW(P2)
    lds     r16,P2_LEN
    call    MOVE_BODY
    pop     ZL
    pop     ZH
    ret


MOVE_BODY:
; Makes body follow head.
; Requires snake length in r16
; and Z pointing at head.
    dec     r16 ; exclude head
    add     ZL,r16 ; point at tail
    adc     ZH,ZERO
move_body_loop: ; starts at tail
    ld      r17,-Z ; save next
    std     Z+1,r17 ; store at curr
    dec     r16 ; count down
    brne    move_body_loop
    ret


LIMITS:
; Takes x,y = r16,r17 and
; moves them to other side of
; screen if outside.
    cpi     r16,255
    breq    x_under
    cpi     r16,16
    breq    x_over
    rjmp    xlim_done
x_under:
    ldi     r16,15
    rjmp    xlim_done
x_over:
    ldi     r16,0
xlim_done:
    cpi     r17,255
    breq    y_under
    cpi     r17,16
    breq    y_over
    rjmp    ylim_done
y_under:
    ldi     r17,15
    rjmp    ylim_done
y_over:
    ldi     r17,0
ylim_done:
    ret
