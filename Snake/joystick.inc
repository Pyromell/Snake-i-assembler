READ_JOY:
; Reads from pin specified by MUX3..0 in r18
; Returns ADCH in r19    
    ldi     r16,0b0110_0000 ; load default ADMUX
    or      r16,r18 ; set MUXn = PCn from r18 = n
    sts     ADMUX,r16
    lds     r16,ADCSRA
    ori     r16,0b0100_0000 ; set ADSC
    sts     ADCSRA,r16

adc_not_done: ; wait till conversion done
    lds     r16,ADCSRA
    sbrc    r16,ADSC
    rjmp    adc_not_done
adc_done:
    lds     r19,ADCH ; read 8 MSB
    ;ldi     r16,0b0110_0000
    ;sts     ADMUX,r16 ; reset ADMUX
    ret


JOYSTICK_1:
; Performs ADC conversion and
; updates head coords in SRAM
    ; x-direction
    ldi     r18,J1X
    call    READ_JOY ; returns ADCH in r19
    lds     r16,P1
    call    SEP_XY
    cpi     r19,0b1100_0000 ; checks right
    brlo    not_right
    inc     r16
    rjmp    not_left
not_right:
    cpi     r19,0b0100_0000 ; checks left
    brsh    not_left
    dec     r16
not_left:
    call    XLIM
    call    JOIN_XY
    sts     P1,r16
    ; y-direction
    ldi     r18,J1Y
    call    READ_JOY  ; returns ADCH in r19
    lds     r16,P1
    call    SEP_XY
    cpi     r19,0b1100_0000 ; checks up
    brlo    not_up
    inc     r17
    rjmp    all_done
not_up:
    cpi     r19,0b0100_0000 ; checks down
    brsh    all_done
    dec     r17

all_done:
    call    YLIM
    call    JOIN_XY
    sts     P1,r16

    ldi     ZH,HIGH(P1)
    ldi     ZL,LOW(P1)
    call    AUTO_RUN
    ret


JOYSTICK_2:
; Performs ADC conversion and
; updates head coords in SRAM
    ; x-direction
    ldi     r18,J2X
    call    READ_JOY ; returns ADCH in r19
    lds     r16,P2
    call    SEP_XY
    cpi     r19,0b1100_0000 ; checks right
    brlo    not_right2
    inc     r16
    rjmp    not_left2
not_right2:
    cpi     r19,0b0100_0000 ; checks left
    brsh    not_left2
    dec     r16
not_left2:
    call    XLIM
    call    JOIN_XY
    sts     P2,r16
    ; y-direction
    ldi     r18,J2Y
    call    READ_JOY  ; returns ADCH in r19
    lds     r16,P2
    call    SEP_XY
    cpi     r19,0b1100_0000 ; checks up
    brlo    not_up2
    inc     r17
    rjmp    all_done2
not_up2:
    cpi     r19,0b0100_0000 ; checks down
    brsh    all_done2
    dec     r17
all_done2:
    call    YLIM
    call    JOIN_XY
    sts     P2,r16

    ldi     ZH,HIGH(P2)
    ldi     ZL,LOW(P2)
    call    AUTO_RUN
    ret


XLIM:
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
    ret

YLIM:
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



AUTO_RUN:

	ld		r16,Z
    ldd     r17,Z+1
	cp		r16,r17			// CHECK IF JOYSTICK HAS BEEN USED
	brne	AUTO_DONE

	ldd		r17,Z+2			// Compares with +2 due to body function
	swap	r16
	swap	r17
	andi	r16,0b0000_1111  
	andi	r17,0b0000_1111

	cp		r16,r17
	breq	AUTO_X_DONE
	
	cpi		r16,0			// Due to limits special cases are needed
	brne	NO_LIM_X1
	cpi		r17,15
	brne	NO_LIM_X1
	ldi		r16,1
	rjmp	AUTO_X_DONE
NO_LIM_X1:

	cpi		r16,15
	brne	NO_LIM_X2
	cpi		r17,0
	brne	NO_LIM_X2
	ldi		r16,14
	rjmp	AUTO_X_DONE

	
NO_LIM_X2:					// The normal cases
	cp		r16,r17
	brsh	INC_AUTO_X
	dec		r16
	call	XLIM
	rjmp	AUTO_X_DONE

INC_AUTO_X:
	inc		r16
	call	XLIM
AUTO_X_DONE:

	ld		r17,Z
	ldd		r18,Z+2
	andi	r17,0b0000_1111
	andi	r18,0b0000_1111

	cp		r17,r18
	breq	AUTO_Y_DONE

	cpi		r17,0				// Due to limits special cases are needed
	brne	NO_LIM_Y1
	cpi		r18,15
	brne	NO_LIM_Y1
	ldi		r17,1
	rjmp	AUTO_Y_DONE	
NO_LIM_Y1:
	cpi		r17,15
	brne	NO_LIM_Y2
	cpi		r18,0
	brne	NO_LIM_Y2
	ldi		r17,14
	rjmp	AUTO_Y_DONE	
	
NO_LIM_Y2:
	cp		r17,r18				// the normal cases
	brsh	INC_AUTO_Y

	dec		r17
	call	YLIM
	rjmp	AUTO_Y_DONE
INC_AUTO_Y:
	inc		r17
	call	YLIM
AUTO_Y_DONE:
		
	call	JOIN_XY
	st		Z,r16
AUTO_DONE:

    ret




/*
MOVE_P1_BODY:
    ldi     ZH,HIGH(P1)
    ldi     ZL,LOW(P1)
    ld      r16,Z+ ; stores head
    lds     r20,P1_LEN
move_p1_loop:
    ld      r17,Z ; store curr to tmp
    st      Z+,r16 ; write prev to curr
    mov     r16,r17 ; store tmp as new prev
    dec     r20
    brne    move_p1_loop
    ret
*/