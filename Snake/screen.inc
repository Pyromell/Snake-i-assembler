UPDATE_WIN_VMEM:
update_win_loop:
    lpm     r16,Z+
    cpi	    r16,0
    breq    end_of_list
    ; sets Y to correct line in VMEM
    ; returns DAMatrix byte in r19
    call    POINT_AT_LINE
    ; adds player color to byte
    lds     r17,STATUS
    cpi     r17,P2_WINS
    breq    first_jmp
    ldd     r16,Y+R
    or      r19,r16
    std     Y+R,r19
    cpi     r17,TIED
    breq    first_jmp
    rjmp    second_jmp
first_jmp:
    ldd     r16,Y+B
    or      r19,r16
    std     Y+B,r19
SECOND_JMP:
    rjmp    update_win_loop
end_of_list:
	ret


RED_WIN_TAB: 
    .db $36, $37, $38, $39,\
        $3A, $4A, $59, $58,\
        $48, $47, $56, $56 ; R
    .db $76, $77, $78, $79,\
        $7A, $8A, $88, $86,\
        $9A, $98, $96, $96 ; E
    .db $B6, $B7, $B8, $B9,\
        $BA, $C6, $CA, $D7,\
        $D8, $D9, $00, $00 ; D
BLUE_WIN_TAB:
    .db $16, $17, $18, $19,\
        $1A, $26, $28, $2A,\
        $37, $39           ; B
    .db $56, $57, $58, $59,\
        $5A, $66, $76, $76 ; L
    .db $96, $97, $98, $99,\
        $9A, $A6, $B6, $B7,\
        $B8, $B9, $BA, $BA ; U
    .db $D6, $D7, $D8, $D9,\
        $DA, $E6, $E8, $EA,\
        $F6, $F8, $FA, $00 ; E
TIE_TAB:
    .db $3A, $46, $47, $48,\
        $49, $4A, $5A, $5A ; T
    .db $76, $77, $78, $79,\
        $7A, $7A           ; I
    .db $96, $97, $98, $99,\
        $9A, $A6, $A8, $AA,\
        $B6, $B8, $BA, $00 ; E
    
