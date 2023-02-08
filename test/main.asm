
.equ	DD_MOSI = PB3
.equ	DD_SCK	 = PB5
.equ	SS	 = PB2	

	
SPI_MasterInit:
	; Set MOSI and SCK output, all others input
	ldi		r17, (1<<DD_MOSI)|(1<<DD_SCK)|(1<<SS)
	out		DDRB,r17
	
	; Enable SPI, Master, set clock rate fck/16
	ldi		r17,(1<<SPE)|(1<<MSTR)|(1<<SPR0)
	out		SPCR,r17

MAIN:
	ldi	r16,0b11111000
	;call	SPI_MasterTransmit
	ldi	r17,0b11111011
	;call	SPI_MasterTransmit
	;ldi	r18,0b11111111
	;call	SPI_MasterTransmit
	;ldi	r19,0b11111111
	
	
SPI_MasterTransmit:
	; Start transmission of data (r16)
	out		SPDR,r16
	out		SPDR,r17
	;out		SPDR,r18
	;out		SPDR,r19
Wait_Transmit:
	; Wait for transmission complete
	in		r16,SPSR
	sbrs	r16,SPIF
	rjmp	Wait_transmit
Again:
rjmp	Again