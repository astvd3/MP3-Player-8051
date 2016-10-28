#include "sdcard.h"
#include "spi.h"
#include "PORT.H"
#include <stdio.h>

uint8_t SD_Card_Type = Type2;
/***********************************************************************
DESC: Sends Command to SD Card
INPUT: Command, Arggument
RETURNS: erroe flag in case of error
CAUTION: None
************************************************************************/
uint8_t send_command(uint8_t command, uint32_t argument)
{
	uint8_t error_flag=no_errors, send_val, return_val, index;
	if(	command < 64)
	{
		send_val = (0x40|command);
		error_flag = SPI_Transfer( send_val, &return_val);
		index=24;
		while((error_flag == no_errors) && (index<25))
		{
			send_val=(uint8_t)(argument >> index);
			error_flag = SPI_Transfer( send_val, &return_val);
			index= index-8;
		}
		if(	command == 0)
		{
			send_val = 0x95;
		}
		else if (command ==8)
		{
			send_val = 0x87;
		}
		else
		{
			send_val = 0x01;
		}

		if(	error_flag == no_errors)
		{
			error_flag = SPI_Transfer( send_val, &return_val);
		}
		else
		{
			error_flag = SPI_error;
			printf("\n SPI error in Send Command \n" );
		}
	}
	else
	{
		error_flag=illegal_command;
		printf("\n illegal_command in Send Command \n" );
	}
	return error_flag;
}
/***********************************************************************
DESC: Receives response from SD card
INPUT: No. of bytes, array_out
RETURNS: Error flag
CAUTION: None
************************************************************************/
uint8_t receive_response(uint8_t num_of_bytes, uint8_t *array_out)
{
	uint8_t time_out=0,receive_val,error_flag=0,index;
	do
	{
		error_flag = SPI_Transfer( 0xFF, &receive_val);
		time_out++;
	}
	while((time_out!=0) && (error_flag==no_errors) && (receive_val==0xFF));
	*array_out = receive_val;
	if(time_out ==0)
	{
		error_flag=TIMEOUT_ERROR;
	
	}
	else if(error_flag!=no_errors)
	{
		error_flag=SPI_error;
	}
	else if((receive_val!=0x00)&& (receive_val!=0x01) )
	{
		error_flag=SD_CARD_ERROR;
	}
	else if(num_of_bytes >1)
	{
		for(index=1;index<num_of_bytes;index++)
		{
			SPI_Transfer( 0xFF, &receive_val);
			array_out[index]=receive_val;	
		}
	}
	SPI_Transfer( 0xFF, &receive_val);
	return error_flag;
	
}
/***********************************************************************
DESC: Initializes SD Card
INPUT: None
RETURNS: Nothing
CAUTION: Generate Clock pulses manually
************************************************************************/
uint8_t SDCard_Init(void)
{

	uint8_t index,receive_value[5],sendcmd41=0,timeout,error_flag =no_errors;
	uint32_t ACMD41_argument=0x40000000;
	// 80 clock pulses
	nCS=1;
	for(index=0;index<=10;index++)
	{
		SPI_Transfer( 0xFF, receive_value);
	}
	

	//Send CMD 0
	nCS=0;
	
	error_flag = send_command(0,0);
	printf("\n Command 0 Sent \n");

	if(error_flag == no_errors)
	{
		error_flag=receive_response(1,receive_value) ;
	}

	if(error_flag != no_errors)
	{
		printf(" CMD 0 error"); 
		return error_flag;
	}
	nCS=1;
	printf("\n Response for command 0 is %bx \n", *receive_value);

	if( *receive_value == 0x01)
	{
		error_flag= no_errors;
	}
	else
	{
		error_flag= SD_CARD_ERROR;
		printf(" Incorrect R1 response for command 0"); 
		return error_flag;
	}


	// Send CMD 8
	nCS=0;
	error_flag=send_command(8,0x000001AA);
	printf("\n Command 8 Sent \n");
	
	if(error_flag == no_errors)
	{
		error_flag=receive_response(5,receive_value) ;
	}

	if(error_flag != no_errors)
	{
		printf(" CMD 8 error"); 
		return error_flag;
	}
	
	nCS=1;

	if(receive_value[0] == 0x05)
	{
		error_flag = no_errors;
		ACMD41_argument=0;
		SD_Card_Type = Type2;

	}

	if(receive_value[3] != 0x01)
	{
		error_flag = voltage_error;
		printf(" Voltage Error");
		return error_flag;
	}

	if(receive_value[4] != 0xAA)
	{
		error_flag = invalid_echopattern;
		printf(" Invalid Echo Pattern");
		return error_flag;
	}

	printf("\n Response for command 8 is \t");
	for(index=0;index<5;index++)
	{
		printf(" %bx \t", *(receive_value+index));
	}



	//Send CMD 55 and 41
	nCS=0 ;
	timeout=1;
	while(sendcmd41==0 && timeout!=0)
	{
		error_flag =send_command(55,0);
		printf("\n \n Command 55 Sent \n");

		if(error_flag == no_errors)
		{
		error_flag=receive_response(1,receive_value) ;
		}

		if(error_flag != no_errors)
		{
		printf(" CMD 55 error"); 
		return error_flag;
		}

		printf("\n Response for command 55 is %bx \n", *receive_value);
	
	
   		error_flag = send_command(41,0x40000000);

		if(error_flag == no_errors)
		{
		error_flag=receive_response(1,receive_value) ;
		}

		if(error_flag != no_errors)
		{
		printf(" CMD 41 error"); 
		return error_flag;
		}

		if(*receive_value == 0x00)
		{
			sendcmd41 =1;	
		}
		timeout++;
	}
	nCS=1;

	printf("\n \n Command 41 Sent \n");
	printf("\n Response for command 41 is %bx \n", *receive_value);


	//Send CMD 58
	nCS=0;
	error_flag =send_command(58,0);
	printf("\n Command 58 Sent \n");

	    if(error_flag == no_errors)
		{
		error_flag=receive_response(5,receive_value) ;
		}

		if(error_flag != no_errors)
		{
		printf(" CMD 58 error"); 
		return error_flag;
		}

	nCS=1;
	printf("\n Response for command 58 is \t");
	for(index=0;index<5;index++)
	{
		printf(" %bx \t", *(receive_value+index));
	}

	
	return 0;

}
/***********************************************************************
DESC: Reads a Block of DATA from SD card
INPUT: No. of bytes, array
RETURNS: Error flag
CAUTION: None
************************************************************************/
uint8_t read_block(uint16_t num_of_bytes, uint8_t *array)
{

	uint8_t time_out=0,receive_val,error_flag=0;
	uint16_t index;
	do
	{
		error_flag = SPI_Transfer( 0xFF, &receive_val);
		time_out++;
	}
	while((time_out!=0) && (error_flag==no_errors) && (receive_val==0xFF));
	if(time_out ==0)
	{
		error_flag=TIMEOUT_ERROR;
		printf(" Timeout error");
		
	}
	else if(error_flag!=no_errors)
	{
		error_flag=SPI_error;
		printf(" SPI error");
	}
	else if(receive_val!=0x00 )
	{
		error_flag=SD_CARD_ERROR;
		printf(" SD Card error");
	}


	do
	{	time_out=0;
		error_flag = SPI_Transfer( 0xFF, &receive_val);
		time_out++;
	}
	while((time_out!=0) && (error_flag==no_errors) && (receive_val==0xFF));

	 if(time_out ==0)
	{
		error_flag=TIMEOUT_ERROR;
		printf(" Timeout error ");

	}
	else if(error_flag!=no_errors)
	{
		error_flag=SPI_error;
		printf(" SPI error ");
	}
	else if(receive_val==0xFE )
	{
		error_flag= no_errors;
	}
	else
	{
		error_flag= DATA_ERROR_TOKEN;
		printf(" DATA_ERROR_TOKEN ");
	}


	for(index=0;index<num_of_bytes;index++)
		{
			SPI_Transfer( 0xFF, &receive_val);
			array[index]=receive_val;	
		}
	
	SPI_Transfer( 0xFF, &receive_val);
	SPI_Transfer( 0xFF, &receive_val);
	SPI_Transfer( 0xFF, &receive_val);
	
	return error_flag;
} 