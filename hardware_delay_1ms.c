#include "main.h"
#include "PORT.h"
#include "hardware_delay_1ms.h"


// ***** Caution: Uses Timer1 *******//
void DELAY_1ms_T1(uint16_t duration)
{
   // Configure Timer 1 as a 16-bit timer 
   TMOD &= 0x0F; // Clear all T1 bits (T0 left unchanged)
   TMOD |= 0x10; // Set required T1 bits (T0 left unchanged) 

	  
   ET1 = 0;  // No interupts
      
   do
   {
     // Values for 1 ms delay
      TH1 = Timer_Reload_1ms_THx;  // Timer 0 initial value (High Byte)
      TL1 = Timer_Reload_1ms_TLx;  // Timer 0 initial value (Low Byte)

      TF1 = 0;          // Clear overflow flag
      TR1 = 1;          // Start timer 0

      while (TF1 == 0); // Loop until Timer 0 overflows (TF0 == 1)

      TR1 = 0;          // Stop Timer 0
	  duration--;
	}while(duration>0);
}




