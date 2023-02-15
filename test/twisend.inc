TWI_SEND:
    push    ZL
    push    ZH
    call    TWI_INIT
    
    in      ZH,SPH
    in      ZL,SPL

    ldd     r18,z+5 ; data

    push    r18
    call    SEND_BYTE ; data
    pop     r18
    call    STOP

    pop     ZH
    pop     ZL
    ret

TWI_INIT:
    in      ZH,SPH
    in      ZL,SPL
    push    r17

    ldd     r17,z+8 ; adress

    call    START

    push    r17
    call    SEND_BYTE ; adress
    pop     r17
    pop     r17

    ret

SEND_BYTE:
    in      ZH,SPH
    in      ZL,SPL
	push    r17
    push    r18

    ldd     r17,z+3
    ldi     r18,8
SEND_LOOP:
    lsl     r17
    brcc    lo
hi: call    SDH
	rjmp	DONE
lo: call    SDL
DONE:
    dec     r18
    brne    SEND_LOOP
    call    SDH     ; ack
    pop     r18
    pop     r17
    ret

START:
    sbi     DDRC,SDA
    call    WAIT
    sbi     DDRC,SCL
    call    WAIT
    ret

SDH:
    cbi     DDRC,SDA
    call    WAIT
    cbi     DDRC,SCL
    call    WAIT
    sbi     DDRC,SCL
    call    WAIT
    ret

SDL:
    sbi     DDRC,SDA
    call    WAIT
    cbi     DDRC,SCL
    call    WAIT
    sbi     DDRC,SCL
    call    WAIT
    ret

STOP:
    sbi     DDRC,SDA
    call    WAIT
    cbi     DDRC,SCL
    call    WAIT
    cbi     DDRC,SDA
    call    WAIT
    ret

WAIT:
    push    r16
    ldi     r16,160
WAITING:
    dec     r16
    brne    WAITING
    pop     r16
    ret