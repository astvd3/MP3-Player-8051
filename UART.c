#include <stdio.h>
#include "main.h"
#include "UART.h"


/***********************************************************************
DESC:    Initializes UART for mode 1
         Baudrate: 9600
INPUT: Nothing
RETURNS: Nothing
CAUTION: Uses Baudrate Generator instead of a standard timer
************************************************************************/
void uart_init(void)
{
  
  // configure UART
  // set or clear SMOD0
  PCON |= (SMOD1 << 7);
  PCON &= ~((~(SMOD1) <<7) | 0x40);
  SCON = UART_MODE1|RECEIVE_ENABLE;

  // Initialize Baud Rate Generator
  BDRCON=0;   //Stops and disables Baud Rate Gen.
  BRL= BAUD_RATE_RELOAD;
  BDRCON= (0x1C | (SPD << 1));  // Enables Baud Rate Gen. for RxD and TxD

  // initially not busy

  TI=1;

  // serial interrupt is disabled
  ES=0;


} // uart_init



