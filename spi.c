#include "spi.h"
#include <stdio.h>
/***********************************************************************
DESC:  Initialization of SPI
INPUT: 	None
RETURNS: None
CAUTION: 
************************************************************************/
uint8_t SPI_Master_Init(uint32_t clock_rate)
{
	if(	clock_rate == 400000)
	{
		SPCON=0xF1;  // Clock Rate 400kHz
	}
	else if(clock_rate == OSC_FREQ/2)
	{
		SPCON=0x70;	// Clock rate Oscillation freq/2
	}  
	return 0;

}
/***********************************************************************
DESC: SPI Transfer
INPUT: send value, received value
RETURNS: timeout
CAUTION: None
************************************************************************/
uint8_t SPI_Transfer(uint8_t send_value, uint8_t *received_value)
{
	uint8_t test,timeout;
	timeout=0;
	SPDAT=send_value;
	do
	{
		test=SPSTA;
		timeout++;
	}				   
	while(!(test&0xF0) && (timeout!=0));

	if( timeout!=0 )
	{
		*received_value = SPDAT;
		timeout=test & 0x70;
	}
	else
	{
	 	timeout=TIMEOUT_ERROR;
		printf("\n SPI ERROR in SPI transfer \n");
	}
	return timeout;
}
