#include "includes.h"





//---------------------------------------------------------------------------
// setup phase lock loop
// Processor is running at 60MHz internally from a 12Mhz clock
//
//---------------------------------------------------------------------------
void PLL_startup(void)
{
// setup PLL to run at 60MHz from a 12MHz xtal
// processor will run at 60MHz after the PLL kicks in


  PLLCFG=0x00000024; // setup the PLL multiplier/divisers for a 12MHz clock
  PLLCON=0x00000001; // enable the PLL
  PLLFEED=0x000000AA;// safety sequence to activate the PLL
  PLLFEED=0x00000055;// safety sequence to activate the PLL


  while (!(PLLSTAT & 0x00000400)); // wait for PLL lock, test lock bit


  PLLCON=0x00000003; // connect system to the PLL
  PLLFEED=0x000000AA;// safety sequence to activate the PLL
  PLLFEED=0x00000055;// safety sequence to activate the PLL


  // At this point the system clock internally in running at 60Mhz

  //Setup the Pheriperal clock to be the same as the system clock
  VPBDIV=0x1; // Set pclk to same as cclk

  // system clock settings are now cclk=60mhz pclk=60mhz

} // end of function PLL_startup()




//---------------------------------------------------------------------------
// setup system timer
// There are two timers used, timer0 and timer1
// Timer 0 runs the display update, and timer 1 is the timer tick
//---------------------------------------------------------------------------
void setup_system_timers(void)
{



} // end of function





//---------------------------------------------------------------------------
// Setup the general purpose input output and assign conectivity block
/*


port 0

p0.0         OUTPUT  TX0 SERIAL
p0.1         INPUT   RX0 SERIAL
p0.2         NOT USED
p0.3         NOT USED
p0.4         OUTPUT SPI SCLK
p0.5         INPUT  SPI MISO
p0.6         OUTPUT GPIO AD CONVERTER SPEED CONTROL
p0.7         OUTPUT GPIO DIGIT SELECT 2

p0.8         OUTPUT  TX1 SERIAL
p0.9         INPUT   RX1 SERIAL
p0.10        OUTPUT GPIO DIGIT SELECT 3
p0.11        AD RESET
p0.12        OUTPUT GPIO DIGIT SELECT 5
p0.13        OUTPUT GPIO DIGIT SELECT 5
p0.14        INPUT  TARE KEY (in circuit programming function)
p0.15        NOT USED

p0.16        OUTPUT GPIO DISPLAY SEGMENT 7
p0.17        OUTPUT GPIO DIGIT SELECT 1
p0.18        OUTPUT GPIO DIGIT SELECT 0
p0.19        OUTPUT GPIO DIGIT SELECT 4
p0.20        OUTPUT GPIO DIGIT SELECT 7
p0.21        OUTPUT GPIO DIGIT SELECT 8
p0.22        OUTPUT GPIO DIGIT SELECT 9
p0.23        OUTPUT GPIO DISPLAY SEGMENT 0


p0.25        OUTPUT GPIO DISPLAY SEGMENT 1
p0.26        OUTPUT GPIO DISPLAY SEGMENT 2
p0.27        OUTPUT GPIO DISPLAY SEGMENT 3
p0.28        OUTPUT GPIO DISPLAY SEGMENT 4
p0.29        OUTPUT GPIO DISPLAY SEGMENT 5
p0.30        OUTPUT GPIO DISPLAY SEGMENT 6
p0.31        NOT USED


port 1

p1.16        INPUT GPIO TARGET KEY (keypad)
p1.17        INPUT GPIO ID     KEY (keypad)
p1.18        NOT USED
p1.19        INPUT GPIO CLEAR  KEY (keypad)
p1.20        INPUT GPIO TARE   KEY (keypad)
p1.21        INPUT GPIO UNITS  KEY (keypad)
p1.22        INPUT GPIO ZERO   KEY (keypad)
p1.23        INPUT GPIO TARGET KEY (keypad)

p1.24        NOT USED
p1.25        NOT USED
p1.26        JTAG DEBUG
p1.27        JTAG DEBUG
p1.28        JTAG DEBUG
p1.29        JTAG DEBUG
p1.30        JTAG DEBUG
p1.31        JTAG DEBUG





*/
//---------------------------------------------------------------------------
void setup_gpio(void)
{

//IO0DIR=0x7eff34d5;  // set the IO direction to refect the above
  
IO0DIR=0x7eff3dd5;  // set the IO direction to refect the above
PINSEL0=0x50585; // Configure Pin Connect Block to reflect the table above  

//PINSEL0=0x10585; // Configure Pin Connect Block to reflect the table above  




} // end of function




//---------------------------------------------------------------------------
// This function configures the processor to switch the UART outputs to regular I/O for polling
// This is used for alternate I/O for remote ZERO, TARE, ETC
//---------------------------------------------------------------------------
char read_COM_level(int port)
{
  
  
  long i;
  

  
  // reinitialize I/O to disconnect from UART and use as regular GPIO through the RS232 transciever 
  if (port == _PORT0)
  {
        // set TX port to output I/O
        PINSEL0 = 0xfffffff0 &  PINSEL0 ;   // bit mask to clear P0.0 P0.1 to set to gpio
          
        IO0CLR = 1;   // set TX to 1   P0.0
        
        
        i=IO0PIN&0x0002;  // read RX port to see if it has changed
  }
  else  // _PORT1
  {
        // set TX port to output I/O 
        PINSEL0 = 0xfff0ffff &  PINSEL0 ;   // bit mask to clear P0.8 P0.9 to set to gpio
          
        IO0CLR = 1;   // set TX to 1  P0.8
        i=IO0PIN&0x200;  // read RX port to see if it has changed
   }
  
   
  // a low is deected when a switch closes across RX ans TX
  if (i!=0) // if it is a non zero then the switch should is open
  {
    return(0);  // switch is open
  }  
   else
   {
     
 
    return(1);  // switch is closed
    
    
   }
  
   
  
} // end of function











//--------------------------------------------------------------------------
// setup SPI communications for AD converter
// SPI is setup as a master reading 8 bit blocks of data
//--------------------------------------------------------------------------
void setup_spi(void)
{


//Set to highest speed for SPI at 5mhz  (60mhz/12)
S0SPCCR=0x0c;
// Device selected as master
S0SPCR=0x20;
// Configure Pin Connect Block for MOSI and SCLK connection
PINSEL0=PINSEL0|0x0500;

} // end of function






//--------------------------------------------------------------------------
// setup timers
//
// timer0 is a free running timer used in conjuction with a match comparator
// timer0 triggers the display update interupt that refreshes the display.
// on a match, timer0 resets and generates an interupt vector to refresh the display
//---------------------------------------------------------------------------
void setup_timers(void)
{

 // setup the timer0, used to drive display

  T0PR= 0x0;  // prescale counter is 30Mhz


  T0MCR=0x18; // when the match register matches, reset T0 counter and trip interupt

  T0MR1=0x0000ea60;  //divide by 10000
  T0MR0=0x0000ea60;  //divide by 10000

  T0TC=0;
  T0TCR=0x1;  // enable timer0

  T0EMR=0x20;



  // setup the timer1 used for the timer tic for 25 times a second

  T1PR= 0x3c;// prescale counter is 1Mhz

  T1MCR=0x03;  // when the match register MR0 matches, reset T1 counter and trip interupt

  T1MR0=0x00009c40;  //divide by 40000

  T1TCR=0x1;  // enable timer1




} // end of function





