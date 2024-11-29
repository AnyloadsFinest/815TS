/*****************************************************************************
 *   pwm.c:  PWM module file for NXP LPC23xx/24xx Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.07.20  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#include "includes.h"

/******************************************************************************
** Function name:		pwm0Handler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**				executes each 10ms @ 60 MHz CPU Clock
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void PWMHandler (void)
{
char ch;

  PWM1IR = 0X10;			/* clear interrupt flag */
  
  timer(RADIODETECT,START); // start the radio detect timeout timer
  
  if(PWM1CCR == 0X05)  // risign edge will reset the timer and set for the fallling edge
   {   
    PWM1CCR = 0X07;	        // Capture Control register on PCAP1:0 event
    PWM1TCR = TCR_RESET ;	// Counter Reset     
    PWM1TCR = TCR_CNT_EN;	// enable 
   }
   else
   { // fallling edge reads the counter and set for rising edge interrup
    PWM1CCR = 0X05;
    ch = PWM1CR0;   // read the timer value
    com2buffer(ch); // store byte from rssi into circular buffer
    } 
   
    __enable_interrupt();       /* handles nested interrupt */
   
 //   VICVectAddr = 0;		/* Acknowledge Interrupt */  
 
  return;  

} // 



/******************************************************************************
** Function name:		PWM_Init
**
** Descriptions:		PWM initialization, setup all GPIOs to PWM0~6,
**				reset counter, all latches are enabled, interrupt
**				on PWMMR0, install PWM interrupt to the VIC table.
**
** parameters:			ChannelNum, Duty cycle
** Returned value:		true or fase, if VIC table is full, return false
** 
******************************************************************************/

DWORD PWM_Init( DWORD ChannelNum, DWORD cycle )
{
  if ( ChannelNum == 0 )
  {       
  }
  else if ( ChannelNum == 1 )
  {
	PCONP   |= 0x00000040;    // Turn On PWM 
        FIO2DIR &= 0XFFFFFFBF;    // set P2.6 as input
	PINSEL4 |= 0x00001000;	  // set GPIOs for CAP1:0 
        
       	PWM1CTCR = 0X00;	  // set for PCAP1.0 timer mode	
        PWM1CCR = 0X05;	         // Capture Control register on PCAP1:0 event
	PWM1PR = 2500;		// Prescaler set for 120hz (8.2ms) 1850  (calibration for 50)
        
	PWM1TCR = TCR_RESET;	  // Counter Reset        
        PWM1TCR = TCR_CNT_EN;	  // enable 
        
 // 
  }
  else
  {  // 
	return ( FALSE );		/* Unknown channel number */
  }
	
  if ( install_irq( PWM0_1_INT, (void *)PWMHandler, HIGHEST_PRIORITY ) == FALSE )
  {
   return ( FALSE );
  }
  return (TRUE);
} // 

/******************************************************************************
** Function name:		PWM_Set
**
** Descriptions:		PWM cycle setup
**
** parameters:			Channel number, PWM cycle, and offset
** Returned value:		None
** 
******************************************************************************/
void PWM_Set( DWORD ChannelNum, DWORD cycle, DWORD offset )
{			
  if ( ChannelNum == 0 )
  {
  }
  else if ( ChannelNum == 1 )
  {
	
  } // 
  return;
}

/******************************************************************************
** Function name:		PWM_Start
**
** Descriptions:		Enable PWM by setting the PCR, PTCR registers
**
** parameters:			channel number
** Returned value:		None
** 
******************************************************************************/
void PWM_Start( DWORD channelNum )
{
  if ( channelNum == 0 )
  {
	
  }
  else if ( channelNum == 1 )
  {
	/* All single edge, all enable */
	       
        PWM1CCR = 0X05;	         // Capture Control register on PCAP1:0 event
	        
	PWM1TCR = TCR_RESET;	  // Counter Reset        
        PWM1TCR = TCR_CNT_EN;	  // enable 
  }
  return;
} // 

/******************************************************************************
** Function name:		PWM_Stop
**
** Descriptions:		Stop all PWM channels
**
** parameters:			channel number
** Returned value:		None
** 
******************************************************************************/
void PWM_Stop( DWORD channelNum )
{
  if ( channelNum == 0 )
  {
    // 
  }
  else if ( channelNum == 1 )
  {
     // 
        BOARD.RSSICAPTURE = 0X00;
        
        PWM1CCR = 0X00;	       // Capture Control register on PCAP1:0 event
	       
	PWM1TCR = 0X00;	  // Counter Reset        
         
	
  }
  return;
}


int read_rssi(void)            // scanning the keypad and system
{
char ch;
char i;
char avg;
static char counter;
static char ch1[5];
static char flag;

if(BOARD.RADIO != 1)
{
//BOARD.RSSICAPTURE=0; // defautl set to zero if there is no rssi comming
//PWM_Stop( 1 );    
//flag = 0;
return(0);  
}

if(!flag)
{
PWM_Start( 1 );// start pwm
flag =1;
}// one time start up  


 if (timer(RADIODETECT,TIMEOUT)) // check if the radio detect has timed out
 //if (timer(REMAIN_ZERO,TIMEOUT)) // check if the radio detect has timed out
 { // 
  //BOARD.RSSICAPTURE=0; // defautl set to zero if there is no rssi comming
  PWM_Stop( 1 );  
  flag = 0;
 } // check if there is no activity in rssi
 else
 {//3 
  ch = readcom2buffer();   // read the serial RX2 circular buffer from rssi pin 
   
  if(ch)ch1[counter++] = ch;   // read the serial RX2 circular buffer from rssi pin 
   
  if(counter > 4) // 3
  {  
  counter = 0;
  avg =0;
  
  for (i=0;i!=5;i++)  // 4 loop through and copy score board over to 
          {
          avg = avg + ch1[i];
          ch1[i]=0;            
          }  
   BOARD.RSSICAPTURE = avg / 5; //  
  }// 
   
 }//3  
return(ch);


}

/******************************************************************************
**                            End Of File
******************************************************************************/
