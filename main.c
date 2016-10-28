//////////////////////////////////////////////////////////////////
//	CpE5160 Experiment #2
//	SPI Peripheral and SD Card Initialization
//	
//	Vinod Kumar Nagandla and Sudhir Sornapudi
//////////////////////////////////////////////////////////////////

#include "stdio.h"
#include "main.h"
#include "PORT.H"
#include "UART.h"
#include "print_bytes.h"
#include "memory_test.h"
#include "hardware_delay_1ms.h"
#include "LCD_routines.h"
#include "spi.h"
#include "sdcard.h"
#include "Long_Serial_In.h"
#include "file_system.h"
#include "i2c.h"

uint8_t code Line1_string[]="Vinod K N";
uint8_t code Line2_string[]="Sudhir S";


void Update_Line1(void);
void Update_Line2(void);

main()
{
   uint8_t *memory1, *memory2,error_flag=no_errors,i,array_name[10],timeout_val=255,target=255;
   uint32_t currentDirctory,entry_cluster;
   uint16_t NoOfEntries,entry_number;
   uint8_t xdata sector_contents[512];
   FS_values_t * Drive_Val;
   REDLED=0;
   AUXR=0x0c;   // make all of XRAM available, ALE always on
   if(OSC_PER_INST==6)
   {
       CKCON0=0x01;  // set X2 clock mode
   }
   else if(OSC_PER_INST==12)
   {
       CKCON0=0x00;  // set standard clock mode
   }
  
   uart_init();
   DELAY_1ms_T1(300);
   REDLED=1;
   memory1=code_memory_init();
   memory2=xdata_memory_init();

   print_memory(memory1, 41);
   putchar(CR);
   putchar(LF);
   print_memory(memory2, 42);
   putchar(CR);
   putchar(LF);
   LCD_Init();  
   Update_Line1();
   Update_Line2();

   SPI_Master_Init(400000);	//Set frequency 400kHz before SD card Init
   SDCard_Init();

   SPI_Master_Init(OSC_FREQ/2);	//frequency after SD card init
	
	mount_drive();
	Drive_Val = Export_Drive_values();
	currentDirctory=Drive_Val->FirstRootDirSec;
	while(1)
	{
		while(target>0)
		{
		i=timeout_val; 
		do 
		{ 
			error_flag=i2c_write(0x43,1,array_name); 
			i--; 
		}while((error_flag!=0) && (i!=0)); 
		i=timeout_val; 
		do 
		{ 
			error_flag=i2c_read(0x43,1,array_name); 
			i--; 
		}while((error_flag!=0)&&(i!=0)); 
			printf("Received Value = %2.2bX\n\r", array_name[0]);
			target--;
		}
		
		NoOfEntries=Print_Directory(currentDirctory, sector_contents);
		printf(" No Of Entries: %d",NoOfEntries);
		printf("\n Enter the entry number: \n");
		entry_number=(uint16_t)long_serial_input();
		if(entry_number<=NoOfEntries)
		{
			entry_cluster=Read_Dir_Entry(currentDirctory,entry_number,sector_contents);
			
			if((entry_cluster & 0x10000000) == 0x10000000)
			{
				printf("\nDirctory\n\n");
				entry_cluster&=0x0FFFFFFF;
				printf("\nCluster:%lu\n\n",entry_cluster);
				currentDirctory=First_Sector(entry_cluster);
			}
			else //if (entry_cluster & 0x10000000 == 0)
			{
				entry_cluster&=0x0FFFFFFF;
				open_file(entry_cluster,sector_contents);
			}
		}
		else
		{
			printf("Invalid Entry");
		}
	

	}
}

void Update_Line1(void)
{
   LCD_Write(COMMAND,set_ddram_addr|line1);
   DELAY_1ms_T1(1);
   LCD_Print(0,Line1_string);
}


void Update_Line2(void)
{
   LCD_Write(COMMAND,set_ddram_addr|line2);
   DELAY_1ms_T1(1);
   LCD_Print(12,Line2_string);
}
