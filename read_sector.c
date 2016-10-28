#include "read_sector.h"
#include "sdcard.h"
#include "PORT.H"
#include "stdio.h"

uint8_t Read_Sector(uint32_t sector_number, uint16_t sector_size, uint8_t * array_for_data)
{
		uint8_t error_flag=no_errors;
		nCS=0;
		error_flag=send_command(17,sector_number);	//Send CMD17
		printf("\n Command 17 Sent to read sector number: %lu  \n", sector_number);

		if(error_flag == no_errors)
		{
			error_flag=read_block(sector_size, array_for_data);	//reads 512 bytes of a block
		}

		if(error_flag != no_errors)
		{
			printf("\n CMD 17 error while reading sector: %lu \n", sector_number ); 
		}
		nCS=1;
		return error_flag;

}
