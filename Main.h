/*------------------------------------------------------------------*-

   Main.H (v1.00)

  ------------------------------------------------------------------
   
   'Project Header' (see Chap 5).


   COPYRIGHT
   ---------

   This code is associated with the book:

   EMBEDDED C by Michael J. Pont 
   [Pearson Education, 2002: ISBN: 0-201-79523-X].

   This code is copyright (c) 2001 by Michael J. Pont.
 
   See book for copyright details and other information.

-*------------------------------------------------------------------*/
#ifndef _MAIN_H
#define _MAIN_H

//------------------------------------------------------------------
// WILL NEED TO EDIT THIS SECTION FOR EVERY PROJECT
//------------------------------------------------------------------

// Must include the appropriate microcontroller header file here
#include "AT89C51RC2.h"

// Oscillator / resonator frequency (in Hz) e.g. (11059200UL)
#define OSC_FREQ (18432000UL)

// Number of oscillations per instruction (12, etc)
// 12 - Original 8051 / 8052 and numerous modern versions
//  6 - Various Infineon and Philips devices, etc.
//  4 - Dallas 320, 520 etc.
//  1 - Dallas 420, etc.
#define OSC_PER_INST (6)

//------------------------------------------------------------------
// SHOULD NOT NEED TO EDIT THE SECTIONS BELOW
//------------------------------------------------------------------

// Typedefs (see Chap 5)  
typedef unsigned char uint8_t;
typedef unsigned int  uint16_t;
typedef unsigned long uint32_t;

// Interrupts (see Chap 7)  
#define Timer_0_Overflow 1
#define Timer_1_Overflow 3
#define Timer_2_Overflow 5

#endif

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/
