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


CHECK_HITS:
    call CHECK_PLAYER_HITS
    call CHECK_FRUIT_HITS
    ret


CHECK_PLAYER_HITS:
    ; head to head collision
    lds     r16,P1
    lds     r17,P2
    cp      r16,r17
    breq    tie

   ; p1 head and p2 body 
    ldi     ZH,HIGH(P2)
    ldi     ZL,LOW(P2)
    lds     r16,P1
    lds     r18,P2_LEN
p2_win_loop:
; Requires Z at body to loop 
; through and its length in
; r18 as well as the head to
; check in r16.
    ld      r17,Z+ ; move through body
    cp      r16,r17 ; check if hit
    breq    p2_win
    dec     r18 ; count down
    brne    p2_win_loop

   ; p2 head and p1 body 
    ldi     ZH,HIGH(P1)
    ldi     ZL,LOW(P1)
    lds     r16,P2 ; head
    lds     r18,P1_LEN
p2_on_p1:
    ld      r17,Z+
    cp      r16,r17    
    breq    p1_win
    dec     r18
    brne    p2_on_p1
tie:
    ldi     r16,TIED
    sts     STATUS,r16
    rjmp    check_players_done
p1_win:    
    ldi     r16,P1_WINS
    sts     STATUS,r16
    rjmp    check_players_done
p2_win:
    ldi     r16,P2_WINS
    sts     STATUS,r16
check_players_done:
    ret


CHECK_FRUIT_HITS:
    lds     r18,FRUIT
    ; P1 and fruit
    lds     r16,P1
    cp      r18,r16
    breq    p1_eats
    ; P2 and fruit
    lds     r16,P2
    cp      r18,r16
    breq    p2_eats
    rjmp    check_fruit_done
p1_eats:
    ; increase length if not max
    lds     r16,P1_LEN
    ldi     r17,MAX_LEN
    cpse    r16,r17
    inc     r16
    sts     P1_LEN,r16
    rjmp    new_fruit
p2_eats:
    ; increase length if not max
    lds     r16,P2_LEN
    ldi     r17,MAX_LEN
    cpse    r16,r17
    inc     r16
    sts     P2_LEN,r16
new_fruit:
    call    PLACE_FRUIT
check_fruit_done:
    ret
