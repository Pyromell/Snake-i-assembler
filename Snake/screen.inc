



UPDATE_WIN_VMEM:
    
UPDATE_WIN_LOOP:
    lpm     r16,Z+
	cpi		r16,0
	breq	END_OF_LIST
    ; sets Y to correct line in VMEM
    ; returns DAMatrix byte in r19
    call    POINT_AT_LINE
    ; adds player color at coord to that byte
    ldd     r16,Y+R
    or      r19,r16
    std     Y+R,r19
	;std		Y+B,r19
	;std		Y+G,r19
	
    rjmp    UPDATE_WIN_LOOP
END_OF_LIST:
	ret