





SPI_MasterInit:
	; Set MOSI and SCK output, all others input
	ldi		r17, (1<<DD_MOSI)|(1<<DD_SCK)
	out		DDR_SPI,r17
	
	; Enable SPI, Master, set clock rate fck/16
	ldi		r17,(1<<SPE)|(1<<MSTR)|(1<<SPRO)
	out		SPCR,r17
	ret



SPI_MasterTransmit:
	; Start transmission of data (r16)
	out		SPDR,r16

Wait_Transmit:
	; Wait for transmission complete
	in		r16,SPSR
	sbrs	r16,SPIF
	rjmp	Wait_transmit
	ret