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
	push	ZH
	push	ZL
	ldi     ZH,HIGH(P1)
    ldi     ZL,LOW(P1)
    lds     r16,P1_LEN
	call	MOVE_BODY
	ldi     ZH,HIGH(P2)
    ldi     ZL,LOW(P2)
    lds     r16,P2_LEN
	call	MOVE_BODY
	pop		ZL
	pop		ZH
	ret
MOVE_BODY:
    // Head location starts at 0 instead of 1
    dec     r16 
    // Start looking at the back of the snake 
    add     ZL,r16
    adc     ZH,ZERO
move_loop:
    // Example: if size = 3 then takes
    // from 2nd and put into 3rd
    ld      r17,-Z
    std     Z+1,r17
    dec     r16
    brne    move_loop
    // Now Head is the same value as
    // first part of tail
    // After this call we want to update
    // head with new cord from Joystick
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
    rjmp    xlim_done
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
    rjmp    ylim_done
ylim_done:
    ret


CHECK_HITS:
    ; head to head collision
    lds     r16,P1
    lds     r17,P2
    cp      r16,r17
    breq    tie
    
    ; P1 head to P2 tail
    ldi     ZH,HIGH(P2)
    ldi     ZL,LOW(P2)
    lds     r16,P1
    lds     r18,P2_LEN
collide_p2_loop:
    ld      r17,Z+
    cp      r16,r17  
    breq    p2_win
    dec     r18
    brne    collide_p2_loop

    ; P2 head to P1 tail
    ldi     ZH,HIGH(P1)
    ldi     ZL,LOW(P1)
    lds     r16,P2 ; head
    lds     r18,P1_LEN
collide_p1_loop:
    ld      r17,Z+
    cp      r16,r17    
    breq    p1_win
    dec     r18
    brne    collide_p1_loop

    lds     r18,FRUIT
    ; P1 and fruit
    lds     r16,P1
    cp      r18,r16
    breq    p1_eats
    ; P2 and fruit
	lds     r16,P2
    cp      r18,r16
    breq    p2_eats

    rjmp    fruit_done ; no new fruit

p1_eats:
    ; increase length if not max
    lds     r16,P1_LEN
	ldi		r17,MAX_LEN
	cpse	r16,r17
    inc     r16
    sts     P1_LEN,r16
    rjmp    new_fruit

p2_eats:
    ; increase length if not max
    lds     r16,P2_LEN
	ldi		r17,MAX_LEN
	cpse	r16,r17
    inc     r16
    sts     P2_LEN,r16

new_fruit:
    call    PLACE_FRUIT

fruit_done:
    rjmp    check_hits_done

tie:
    ldi     r16,TIED
    sts     STATUS,r16
    rjmp    check_hits_done

p1_win:    
    ldi     r16,P1_WINS
    sts     STATUS,r16
    rjmp    check_hits_done

p2_win:
    ldi     r16,P2_WINS
    sts     STATUS,r16

check_hits_done:
    ret