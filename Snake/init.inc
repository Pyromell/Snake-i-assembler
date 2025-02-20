INIT:
; Entry point for call from main
spi_init:
    ; Set MOSI and SCK output, all others input
    ldi     r16,( 1 << MOSI | 1 << SCK | 1 << SS )
    out     DDRB,r16

    ; Enable SPI, Master, set clock rate fck/16
    ldi     r16,( 1 << SPE  | 1 << MSTR |\
                  0 << SPR0 | 0 << SPR1 )
    out     SPCR,r16

    ldi     r17,( 0 << SPI2X )
    out     SPSR,r17

adc_init:
    ; ADC Control and Status Register A:
    ; ADEN, ADSC, ADATE, ADIF, ADIE, ADPS2..0
    ; Prescaler 2..0: Set to 1/128 => 125 kHz
    ldi     r16,0b1000_0111
    sts     ADCSRA,r16

    ; ADC Multiplexer Selection Register:
    ; REFS1..0, ADLAR, --, MUX3..0
    ; Ref set to AVCC (01)
    ; Channel Selection 3..0: 0000 = ADC0
    ldi     r16,0b0110_0000
    sts     ADMUX,r16

interrupt_init:
    ldi     r16,( 0 << COM0A1 | 0 << COM0A0 |\
                  0 << WGM01  | 0 << WGM00  )
    out     TCCR0A,r16

    ldi     r16,( 0 << WGM02 | 0 << CS02 |\
                  1 << CS01  | 1 << CS00 )
    out     TCCR0B,r16
    
    ldi     r16,( 1 << TOIE0 )
    sts     TIMSK0,r16


	ret ; from INIT
