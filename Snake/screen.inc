



UPDATE_WIN_VMEM:
    
UPDATE_WIN_LOOP:
    lpm     r16,Z+
	cpi		r16,0
	breq	END_OF_LIST
    ; sets Y to correct line in VMEM
    ; returns DAMatrix byte in r19
    call    POINT_AT_LINE
    ; adds player color at coord to that byte
    lds     r17,STATUS
    cpi     r17,P2_WINS
    breq    FIRST_JMP

    ldd     r16,Y+R
    or      r19,r16
    std     Y+R,r19
    
    cpi     r17,TIED
    breq    FIRST_JMP
    rjmp    SECOND_JMP

FIRST_JMP:
    ldd     r16,Y+B
    or      r19,r16
    std     Y+B,r19
SECOND_JMP:
    rjmp    UPDATE_WIN_LOOP
END_OF_LIST:
	ret