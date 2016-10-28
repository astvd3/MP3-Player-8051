/*--------------------------------------------------------------------------
REG932.H

Header file for Philips 89LPC932
Copyright (c) 1988-2003 Keil Elektronik GmbH and Keil Software, Inc.
All rights reserved.
V1.3
--------------------------------------------------------------------------*/

#ifndef __REGAT89C51RC2_H__
#define __REGAT89C51RC2_H__

/*  BYTE Registers  */
/*  CORE SFRs  */
sfr ACC  = 0xE0;
sfr B    = 0xF0;
sfr PSW  = 0xD0;
sfr SP   = 0x81;
sfr DPL  = 0x82;
sfr DPH  = 0x83;

/* System Management SFRs */
sfr PCON = 0x87;
sfr AUXR = 0x8E;
sfr AUXR1  = 0xA2;
sfr CKRL = 0x97;
sfr CKCON0 = 0x8F;
sfr CKCON1 = 0xAF;

/* Interrupt SFRs */
sfr IEN0 = 0xA8;
sfr IEN1 = 0xB1;
sfr IPH0 = 0xB7;
sfr IPL0 = 0xB8;
sfr IPH1 = 0xB3;
sfr IPL1 = 0xB2;

/* Port SFRs  */
sfr P0   = 0x80;
sfr P1   = 0x90;
sfr P2   = 0xA0;
sfr P3   = 0xB0;


/* Timer SFRs  */
sfr TCON = 0x88;
sfr TMOD = 0x89;
sfr TL0  = 0x8A;
sfr TH0  = 0x8C;
sfr TL1  = 0x8B;
sfr TH1  = 0x8D;
sfr WDTPRG  = 0xA7;
sfr WDTRST  = 0xA6;
sfr T2CON  = 0xC8;
sfr T2MOD  = 0xC9;
sfr RCAP2H = 0xCB;
sfr RCAP2L = 0xCA;
sfr TL2    = 0xCC;
sfr TH2    = 0xCD;

/* PCA SFRs  */
sfr CCON = 0xD8;
sfr CMOD = 0xD9;
sfr CL   = 0xE9;
sfr CH   = 0xF9;
sfr CCAPM0 = 0xDA;
sfr CCAPM1 = 0xDB;
sfr CCAPM2 = 0xDC;
sfr CCAPM3 = 0xDD;
sfr CCAPM4 = 0xDE;
sfr CCAP0H = 0xFA;
sfr CCAP1H = 0xFB;
sfr CCAP2H = 0xFC;
sfr CCAP3H = 0xFD;
sfr CCAP4H = 0xFE;
sfr CCAP0L = 0xEA;
sfr CCAP1L = 0xEB;
sfr CCAP2L = 0xEC;
sfr CCAP3L = 0xED;
sfr CCAP4L = 0xEE;

/* Serial I/O SFRs */
sfr SCON = 0x98;
sfr SBUF = 0x99;
sfr SADEN  = 0xB9;
sfr SADDR  = 0xA9;
sfr BDRCON = 0x9B;
sfr BRL    = 0x9A;


/* SPI Controller SFRs  */
sfr SPCON  = 0xC3;
sfr SPSTA  = 0xC4;
sfr SPDAT  = 0xC5;

/* Keyboard Interface SFRs */
sfr KBLS = 0x9C;
sfr KBE  = 0x9D;
sfr KBF  = 0x9E;




/*  BIT Registers  */
/*  PSW   */
sbit CY   = PSW^7;
sbit AC   = PSW^6;
sbit F0   = PSW^5;
sbit RS1  = PSW^4;
sbit RS0  = PSW^3;
sbit OV   = PSW^2;
sbit F1   = PSW^1;
sbit P    = PSW^0;

/*  TCON  */
sbit TF1  = TCON^7;
sbit TR1  = TCON^6;
sbit TF0  = TCON^5;
sbit TR0  = TCON^4;
sbit IE1  = TCON^3;
sbit IT1  = TCON^2;
sbit IE0  = TCON^1;
sbit IT0  = TCON^0;

/*  IEN0   */
sbit EA   = IEN0^7;
sbit EC   = IEN0^6;
sbit ET2  = IEN0^5;
sbit ES   = IEN0^4;
sbit ET1  = IEN0^3;
sbit EX1  = IEN0^2;
sbit ET0  = IEN0^1;
sbit EX0  = IEN0^0;



/*  IPL0   */ 
sbit PPCL = IPL0^6;
sbit PT2L = IPL0^5;
sbit PLS  = IPL0^4; 
sbit PT1L = IPL0^3;
sbit PX1L = IPL0^2;
sbit PT0L = IPL0^1;
sbit PX0L = IPL0^0;


/*  SCON  */
sbit SM0  = SCON^7; // alternatively "FE"
sbit FE   = SCON^7;
sbit SM1  = SCON^6;
sbit SM2  = SCON^5;
sbit REN  = SCON^4;
sbit TB8  = SCON^3;
sbit RB8  = SCON^2;
sbit TI   = SCON^1;
sbit RI   = SCON^0;



/*  P0  */
sbit AD7 = P0^7;
sbit AD6 = P0^6;
sbit AD5 = P0^5;
sbit AD4 = P0^4;
sbit AD3 = P0^3;
sbit AD2 = P0^2;
sbit AD1 = P0^1;
sbit AD0 = P0^0; 


/*  P1  */
sbit MOSI    = P1^7;
sbit SCK     = P1^6;
sbit MISO    = P1^5;
sbit CEX1    = P1^4;
sbit CEX0    = P1^3; 
sbit ECI     = P1^2; 
sbit SS      = P1^1;
sbit T2EX    = P1^1;
sbit T2      = P1^0;

/*  P2  */
sbit AD15  = P2^7;
sbit AD14  = P2^6;
sbit AD13  = P2^5;
sbit AD12  = P2^4;
sbit AD11  = P2^3;
sbit AD10  = P2^2;
sbit AD9   = P2^1;
sbit AD8   = P2^0;

/*  P3  */
sbit nRD   = P3^7;
sbit nWR   = P3^6;
sbit T1    = P3^5;
sbit T0    = P3^4;
sbit nINT1 = P3^3;
sbit nINT0 = P3^2;
sbit TxD   = P3^1;
sbit RxD   = P3^0;

/* T2CON */
sbit TF2   = T2CON^7;
sbit EXF2  = T2CON^6;
sbit RCLK  = T2CON^5;
sbit TCLK  = T2CON^4;
sbit EXEN2 = T2CON^3;
sbit TR2   = T2CON^2;
sbit CnT2  = T2CON^1;
sbit CP    = T2CON^0;

/* CCON */
sbit CF    = CCON^7;
sbit CR    = CCON^6;

sbit CCF4  = CCON^4;
sbit CCF3  = CCON^3;
sbit CCF2  = CCON^2;
sbit CCF1  = CCON^1;
sbit CCF0  = CCON^0;

#endif
