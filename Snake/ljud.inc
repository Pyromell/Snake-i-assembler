ldi r16, (1<<WGM11)|(1<<WGM10)|(1<<COM1A1)|(1<<CS11)
out TCCR1A, r16
ldi r16, (1<<WGM13)|(1<<WGM12)|(1<<CS11)
out TCCR1B, r16
ldi r16, 195
out OCR1A, r16





ldi r16, (1 << WGM21) | (1 << COM2A1) | (1 << CS21) ; PWM-inställningar för TCCR2A
out TCCR2A, r16
ldi r16, (1 << WGM21) | (1 << WGM20) | (1 << COM2A1) | (1 << CS21) ; PWM-inställningar för TCCR2B
out TCCR2B, r16

ldi r16, 61 ; Pulser per period (8000000 Hz klocka / 523 Hz ton)
ldi r17, 50 ; 50% pulsbredd
mul r16, r17 ; Räkna ut pulsbredden (pulser)
mov r18, r0 ; Spara pulsbredden i r18:r19

ldi r16, 523 ; Tonhöjd (frekvens i Hz)
ldi r17, 100 ; Ljudstyrka (0-255)
ldi r20, 200 ; Antal millisekunder att spela upp varje ton

blinka_lilla_stjarna:
ldi r21, 6 ; Antal toner i refrängen
ldi r22, 7 ; Antal toner i versen
ldi r23, 3 ; Antal gånger att spela upp refrängen

refrang:
ldi r24, 7 ; Antal toner i första raden i refrängen
ldi r25, 200 ; Längd i millisekunder för första raden i refrängen
call play_tone ; Spela upp första raden i refrängen
dec r24 ; Minska antal toner kvar
ldi r25, 100 ; Längd i millisekunder för övriga rader i refrängen
refrang_loop:
call play_tone ; Spela upp nästa ton i refrängen
dec r24 ; Minska antal toner kvar
brne refrang_loop ; Upprepa tills alla toner är spelade

dec r23 ; Minska antal refränger kvar
brne refrang ; Upprepa tills alla refränger är spelade

vers:
ldi r24, 6 ; Antal toner i första raden i versen
ldi r25, 400 ; Längd i millisekunder för första raden i versen
call play_tone ; Spela upp första raden i versen
dec r24 ; Minska antal toner kvar
ldi r25, 200 ; Längd i millisekunder för övriga rader i versen
vers_loop:
call play_tone ; Spela upp nästa ton i versen
dec r24 ; Minska antal toner kvar
brne vers_loop ; Upprepa tills alla toner är spelade

ldi r23, 2 ; Antal gånger att spela upp versen
vers_repeat:
call vers ; Spela upp versen igen
dec r23 ; Minska antal repetitioner kvar
brne vers_repeat ; Upprepa tills alla repetitioner är gjorda

play_tone:
out OCR2A










loop:
    lpm r18, Z+
    lpm r19, Z+
    ldi r20, 10
    call delay_ms
    cpi r18, 0
    breq loop
    ldi r16, high(william_tell)
    ldi r17, low(william_tell)
    ldi ZH, high(william_tell)
    ldi ZL, low(william_tell)
    rjmp loop

delay_ms:
    push r18
    push r19
    push r20
    ldi r18, 100
    ldi r19, 10
delay_loop:
    dec r19
    brne delay_loop
    dec r18
    brne delay_loop
    pop r20
    pop r19
    pop r18
    ret
