CHECK_HITS:
    call CHECK_PLAYER_HITS
    call CHECK_FRUIT_HITS
    ret


CHECK_PLAYER_HITS:
    ; p1 head and p2 head
    lds     r16,P1
    lds     r17,P2
    cp      r16,r17
    breq    tie
   ; p1 head and p1 body 
    ldi     ZH,HIGH(P1)
    ldi     ZL,LOW(P1)
    adiw    Z,1 ; looks past head
    lds     r16,P1
    lds     r18,P1_LEN
    dec     r18 ; compensates for head
    call    hit_loop
    cpse    r19,ZERO
    rjmp    p2_win
   ; p2 head and p2 body 
    ldi     ZH,HIGH(P2)
    ldi     ZL,LOW(P2)
    adiw    Z,1 ; looks past head
    lds     r16,P2
    lds     r18,P2_LEN
    dec     r18 ; compensates for head
    call    hit_loop
    cpse    r19,ZERO
    rjmp    p1_win
   ; p1 head and p2 body 
    ldi     ZH,HIGH(P2)
    ldi     ZL,LOW(P2)
    lds     r16,P1
    lds     r18,P2_LEN
    call    hit_loop
    cpse    r19,ZERO
    rjmp    p2_win
   ; p2 head and p1 body 
    ldi     ZH,HIGH(P1)
    ldi     ZL,LOW(P1)
    lds     r16,P2
    lds     r18,P1_LEN
    call    hit_loop
    cpse    r19,ZERO
    rjmp    p1_win

    ; no player hit
    rjmp    check_players_done
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


hit_loop:
; Requires Z at body to loop through 
; and its length in r18 as well as 
; the head to check in r16.
; Sets in r19 if hit, else 0;
    ld      r17,Z+ ; move through body
    cp      r16,r17 ; check if hit
    breq    hit_found
    dec     r18 ; count down
    brne    hit_loop
    rjmp    no_hit_found
hit_found:
    ser     r19
    rjmp    hit_loop_done
no_hit_found:
    clr     r19
hit_loop_done:
    ret
