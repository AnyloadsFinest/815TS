
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
 
  init_VIC();  
  return;
}

