
#include "includes.h"

volatile long SPI0Status = 0;
volatile long TxCounter = 0;
volatile char TxBuffer[112];
/*****************************************************************************
 *   spi.c:  SPI C file for NXP LPC23xx/24xx Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.07.19  ver 1.00    Prelimnary version, first Release
 *
*****************************************************************************/
	void spi_init()
	{ 
         // Configure Pin Connect Block for MOSI and SCLK connection     
        // Port 0.15 SPI SCK, port0.16 uses GPIO SPI_SEL, port0.18 MOSI /        
        PINSEL0 |= 0x80000000;
	PINSEL1 |= 0x00000020;  //ssp0
        //ADC setup          
        //Set to highest speed for SPI at 5mhz  (60mhz/12)
        SSP0CR0 = 0x2B07;
        SSP0CR1 = 0x02;       
        SSP0CPSR = 0x02;
        SSP0DR = 0x0000;
        
	} // end of function spi_init();
        
  
////////////////////////////////////////////////////////////////
// Transmit 8 bit word via SPI
//////////////////////////////////////////////////////////////
	void spi_write8bits(char word)
	{ //   
	spi_output(&word); // 8 bit word
 	} // end of function spi_write8bits()
        // 
////////////////////////////////////////////////////////////////
//  This function setups and outputs the 8bit byte to the spi register
///////////////////////////////////////////////////////////////
	void spi_output(char *byte)
	{	 
	
      //     S0SPDR = *byte;       // transmit to SPI port       
        //  while(!(S0SPSR & 0x80)){}        
          SSP0DR = *byte;       // transmit to SPI port
          while(!(SSP0SR & 0x01)){}         
                              // SPI_Transmit_Data_Empty();
         } // end of function spi_output()



/*
// Alpha
////////////////////////////////////////////////////////////////
//  This function setups and outputs the 8bit byte to the spi register
///////////////////////////////////////////////////////////////
	void spi_output(char *byte)
	{	
          char i = 0;  
          
         S0SPDR = *byte;       // transmit to SPI port
         //S0SPINT = 0x01;		
         
        // timer(SPI_TIMEOUT,START);    // restart the timer and update display  
         while(!(S0SPSR & 0x80))
         {  
          i++;
          if(i > 20) return;           
          //if (timer(SPI_TIMEOUT,TIMEOUT)) return; // return, no need to refresh display 
         };
             
         // SPI_Transmit_Data_Empty();
         } // end of function spi_output()
*/
      
// 
/*****************************************************************************
** Function name:		SPI0Handler
**
** Descriptions:		SPI0 interrupt handler
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/

void SPI0Handler (void)

{
 long regValue;
  
  //SPI0Status &= ~0x10;
  
  S0SPINT = 0x01;		/* clear interrupt flag */
       
  regValue = S0SPSR;
 
  if ( regValue & 0x40 )
  {
	SPI0Status |= 0x08;
        
  } // 
  if ( regValue & 0x80 )
  {       
  TxCounter++;
  if(TxCounter > 29) TxCounter = 0;
        
  }    // 
  
  __enable_interrupt();       /* handles nested interrupt */ 
 // VICVectAddr = 0;		/* Acknowledge Interrupt */    
  // 
 

}
// 

