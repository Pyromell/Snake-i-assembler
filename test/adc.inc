ADC_INIT:
    ; ADC Control and Status Register A
    clr     r16
    ldi     r16,( 1 << ADEN  |\ ; Enable
                  1 << ADSC  |\ ; Start Conversion
                  0 << ADATE |\ ; Auto Trigger Enable
                  0 << ADIF  |\ ; Interrupt Flag
                  0 << ADIE  |\ ; Interrupt Enable
                  1 << ADPS2 |\ ; Prescaler 2..0
                  1 << ADPS1 |\ ; Set to 1/128 => 125 kHz
                  1 << ADPS0 )
    sts     ADCSRA,r16

    ; ADC Multiplexer Selection Register
    clr     r16
    ldi     r16,( 1 << REFS1 |\ ; Reference Selection 1..0
                  1 << REFS0 |\ ; Set to Int. 1.1 V ref
                  0 << ADLAR |\ ; Left Adjust Result
                  0 << MUX3  |\ ; Channel Selection 3..0
                  0 << MUX2  |\ ; 0000 = ADC0
                  0 << MUX1  |\ ;
                  0 << MUX0  )
    sts     ADMUX,r16

    ret
