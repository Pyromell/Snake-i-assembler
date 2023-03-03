INIT:
.equ    MOSI = PB3
.equ    SCK  = PB5
.equ    SS   = PB2
.def    ZERO = r2
 clr    ZERO


STACK_INIT:
    ldi     r16,HIGH(RAMEND)
    out     SPH,r16
    ldi     r16,LOW(RAMEND)
    out     SPL,r16


SPI_INIT:
    ; Set MOSI and SCK output, all others input
    ldi     r16,( 1 << MOSI | 1 << SCK | 1 << SS )
    out     DDRB,r16

    ; Enable SPI, Master, set clock rate fck/16
    ldi     r16,( 1 << SPE  | 1 << MSTR |\
                  0 << SPR0 | 0 << SPR1 )
    out     SPCR,r16

    ldi     r17,( 0 << SPI2X )
    out     SPSR,r17


ADC_INIT:
    ; ADC Control and Status Register A
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
    ldi     r16,( 1 << REFS1 |\ ; Reference Selection 1..0
                  1 << REFS0 |\ ; Set to Int. 1.1 V ref
                  0 << ADLAR |\ ; Left Adjust Result
                  0 << MUX3  |\ ; Channel Selection 3..0
                  0 << MUX2  |\ ; 0000 = ADC0
                  0 << MUX1  |\ ;
                  0 << MUX0  )
    sts     ADMUX,r16


INTERRUPT_INIT:
    ldi     r16,( 0 << COM0A1 | 0 << COM0A0 |\
                  0 << WGM01  | 0 << WGM00  )
    out     TCCR0A,r16

    ldi     r16,( 0 << WGM02 | 0 << CS02 |\
                  1 << CS01  | 1 << CS00 )
    out     TCCR0B,r16
    
    ldi     r16,( 1 << TOIE0 )
    sts     TIMSK0,r16


DATA_INIT:
    ldi     ZL,LOW(P1)
    ldi     ZH,HIGH(P1)
    st      Z+,ZERO
    st      Z,ZERO

    ldi     ZL,LOW(P2)
    ldi     ZH,HIGH(P2)
    st      Z+,ZERO
    st      Z,ZERO

    sts     LINE,ZERO


ret ; from INIT
