
#include "includes.h"
/******************************************************************************
** Function name:		TargetInit
**
** Descriptions:		Initialize the target board; it is called in a necessary 
**						place, change it as needed
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void TargetInit(void)
{

PINSEL0=0x00000000; // Configure 0.0 to 0.15
PINSEL1=0x00000000; // Configure 0.16 to 0.31
//PINSEL0=0x8050005A; // Configure 0.0 to 0.15 
//PINSEL1=0x00000020; // Configure 0.16 to 0.31 
PINSEL2=0x00000000; // Configure 1.0 to 1.15 
PINSEL3=0x00000000; // Configure 1.16 to 1.31
PINSEL4=0x00000000; // Configure 2.0 to 2.15
//PINSEL4=0x00000008; // Configure 2.0 to 2.15 
//PINSEL5=0x00000000; // Configure 2.16 to 2.31
//PINSEL6=0x00000000; // Configure 3.0 to 3.15 
PINSEL7=0x00000000; // Configure 3.16 to 3.31
PINSEL8=0x00000000; // Configure 4.0 to 4.15 
PINSEL9=0x00000000; // Configure 4.16 to 4.31
//
/*----------------------------------------------------------------------------
PINMODE
00= PULL UP
01= RESERVED
10= OPEN
11= PULL DOWN
------------------------------------------------------------------------------*/

PINMODE0=0x00000000; // Configure Pin PULL UP to reflect the table above
//PINMODE0=0x00000A00; // Configure Pin PULL UP to reflect the table above 
PINMODE1=0x00000000; // 
PINMODE2=0x00000000; // 
PINMODE3=0x00000000; // 
//PINMODE3=0x0AAAAAAA; // 
PINMODE4=0x00000000; // 
PINMODE5=0x00000000; // 
PINMODE6=0x00000000; // 
PINMODE7=0x00000000; // 
PINMODE8=0x00000000; // 
PINMODE9=0x00000000; // 
// 
/*----------------------------------------------------------------------------
IODIR
0= INPUT
1= OUPUT
------------------------------------------------------------------------------*/
//FIO0DIR=0XFFFF00F0;
FIO0DIR=0X0F0000F0;
//FIO1DIR=0XFFFF0000;
FIO1DIR=0X0FFF0000;
FIO2DIR=0X00000000;
FIO3DIR=0X0F000000;
FIO4DIR=0X00000000;
return;
}
//   
/******************************************************************************
** Function name:		GPIOResetInit
**
** Descriptions:		Initialize the target board before running the main() 
**				function; User may change it as needed, but may not 
**				deleted it.
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void GPIOResetInit( void )
{   //
   /* Reset all GPIO pins to default: primary function */
    PINSEL0 = 0x00000000;
    PINSEL1 = 0x00000000;
    PINSEL2 = 0x00000000;
    PINSEL3 = 0x00000000;
    PINSEL4 = 0x00000000;
    //PINSEL5 = 0x00000000;
    //PINSEL6 = 0x00000000;
    PINSEL7 = 0x00000000;
    PINSEL8 = 0x00000000;
    PINSEL9 = 0x00000000;
    PINSEL10= 0x00000000;
      
    FIO0DIR = 0x00000000;
    FIO1DIR = 0x00000000;
    FIO2DIR = 0x00000000;
    FIO3DIR = 0x00000000;
    FIO4DIR = 0x00000000;
    
    PINMODE0=0x00000000; // Configure Pin PULL UP to reflect the table above 
    PINMODE1=0x00000000; // 
    PINMODE2=0x00000000; // 
    PINMODE3=0x00000000; // 
    PINMODE4=0x00000000; // 
    PINMODE5=0x00000000; // 
    PINMODE6=0x00000000; // 
    PINMODE7=0x00000000; // 
    PINMODE8=0x00000000; // 
    PINMODE9=0x00000000; // 
    
    FIO0SET = 0x00000000;
    FIO1SET = 0x00000000;
    FIO2SET = 0x00000000;
    FIO3SET = 0x00000000;
    FIO4SET = 0x00000000;
   
}

/*************************************************************************
 * Function Name: InitClock
 * Parameters: void
 * Return: void
 *
 * Description: Initialize PLL and clocks' dividers. Hclk - 288MHz,
 * Cclk- 57.6MHz, Usbclk - 48MHz
 *
 *************************************************************************/
void InitClock(void)
{
  
  // 1. Init OSC
  SCS_bit.OSCRANGE = 0;
  SCS_bit.OSCEN = 1;
  // 2.  Wait for OSC ready
  while(!SCS_bit.OSCSTAT);
  // 3. Disconnect PLL
  PLL0CON_bit.PLLC = 0;
  PLL0FEED = 0xAA;
  PLL0FEED = 0x55;
  // 4. Disable PLL
  PLL0CON_bit.PLLE = 0;
  PLL0FEED = 0xAA;
  PLL0FEED = 0x55;
  // 5. Select source clock for PLL
  CLKSRCSEL_bit.CLKSRC = 1;   // Selects the main oscillator as a PLL clock source.
  PCLKSEL0 = PCLKSEL1  = 0;   // other peripherals 100/4 = 25MHz
  // 6. Set PLL settings 300 MHz
  PLL0CFG_bit.MSEL = 25-1;
  PLL0CFG_bit.NSEL = 2-1;
  PLL0FEED = 0xAA;
  PLL0FEED = 0x55;
  // 7. Enable PLL
  PLL0CON_bit.PLLE = 1;
  PLL0FEED = 0xAA;
  PLL0FEED = 0x55;
  // 8. Wait for the PLL to achieve lock
  while(!PLL0STAT_bit.PLOCK);
  // 9. Set clk divider settings
  CCLKCFG   = 3-1;            // 1/3 Fpll
  // 10. Connect the PLL
  PLL0CON_bit.PLLC = 1;
  PLL0FEED = 0xAA;
  PLL0FEED = 0x55;
}

/******************************************************************************
** Function name:   ConfigurePLL
**
** Descriptions:    Configure PLL switching to main OSC instead of IRC
**            at power up and wake up from power down.
**            This routine is used in TargetResetInit() and those
**            examples using power down and wake up such as
**            USB suspend to resume, ethernet WOL, and power management
**            example
** parameters:      None
** Returned value:    None
**
******************************************************************************/
void ConfigurePLL ( void )
{
  // Flash accelerator init
  //FLASHCFG = (0x5UL<<12) | 0x3AUL;
  // Init clock
  InitClock();
  FLASHCFG = (0x5UL<<12) | 0x3AUL;
 /*
#if FCCLK_FREQ < 20000000
  FLASHCFG = (0x0UL<<12) | 0x3AUL;
#elif FCCLK_FREQ < 40000000
  FLASHCFG = (0x1UL<<12) | 0x3AUL;
#elif FCCLK_FREQ < 60000000
  FLASHCFG = (0x2UL<<12) | 0x3AUL;
#elif FCCLK_FREQ < 80000000
  FLASHCFG = (0x3UL<<12) | 0x3AUL;
#elif FCCLK_FREQ < 100000000
  FLASHCFG = (0x4UL<<12) | 0x3AUL;
#endif
*/
  return;
 
}

/******************************************************************************
**                            End Of File
******************************************************************************/
/******************************************************************************
** Function name:   TargetResetInit
**
** Descriptions:    Initialize the target board before running the main()
**            function; User may change it as needed, but may not
**            deleted it.
**
** parameters:      None
** Returned value:    None
**
******************************************************************************/
void TargetResetInit(void)
{
 
  ConfigurePLL ( );
  // Init GPIO
  GPIOResetInit( );
  //GpioInit();  
  TargetInit();
  //   
  // Configure PLL, switch from IRC to Main OSC 
  PCONP |= 0x03000000;    // Turn On UART 2 and 3
  PCONP |= 0x00001000;    // Turn On ADC 
  PCONP |= 0x00000200;    // Turn On RTC  
 
  FIO3SET = 0x02000000; // buzer off
  FIO0SET = 0x00000030; // status leds off
  FIO1CLR = 0x00030000; // relays off  
  
  init_VIC();  
  return;
}

