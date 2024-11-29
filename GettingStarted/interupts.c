#include "includes.h"

extern long lTIMERTICS;


// interupt vectors for ARM7
//-------------------------------------------------------------------------
// Function Name: IRQSub
// Parameters: void
// Return: void
//
// Description: IRQ subroutine
// This is the handler for a vectored interupt		
//--------------------------------------------------------------------------


#pragma vector=0x18
__irq __arm void IRQ_ISR_Handler (void)
{
void (*interrupt_function)();
unsigned int vector;

  vector = VICVectAddr;     // Get interrupt vector.
  interrupt_function = (void(*)())vector;
  (*interrupt_function)();  // Call vectored interrupt function.
} // end of vector





//------------------------------------------------------------------------
// Function Name: IRQSub
// Parameters: void
// Return: void
//
// Description: FIQ subroutine
// Handle for a fast vectored interupt		
//------------------------------------------------------------------------

#pragma vector=0x1c
__fiq __arm void FIQ_ISR_Handler (void)
{
void (*interrupt_function)();
unsigned int vector;

  vector = VICVectAddr;     // Get interrupt vector.
  interrupt_function = (void(*)())vector;
  (*interrupt_function)();  // Call vectored interrupt function.
} // end of vecor




//-------------------------------------------------------------------------
// Function Name: NonVectISR
// Parameters: void
// Return: void
//
// Description: non vectored callback subroutine
//		
//--------------------------------------------------------------------------
void NonVectISR(void)
{

}






//--------------------------------------------------------------------------
// setup system interupts
// Setup system timer tic interupt
// Setup system display update interupt
// Interupts are enabled after this function call
//--------------------------------------------------------------------------
void setup_interupts(void)
{


  VICIntSelect  =  0;             // Set all VIC interrupts to IRQ for now
  VICIntEnClear = 0xFFFFFFFF;     // Diasable all interrupts
  VICProtection = 0;              // VIC registers can be accessed in User or
                                  // privileged mode
  VICVectAddr = 0;                // Clear interrupt
  VICProtection = 0;              // Accesss VIC in USR | PROTECT




  //VICIntSelect &= ~(1<<VIC_TIMER0);            // Timer 0 intrpt is an IRQ (VIC_TIMER0 = 4)

  VICIntSelect = 0x00000000;  // timer 0 is irq and timer 1 is fiq


  VICVectAddr0 = (unsigned int)&TIMER0_ISR;    // Install ISR in VIC addr slot 0
  VICVectCntl0 = 0x20 | VIC_TIMER0;            // assign timer0 to interupt vector 0 and enable it

  VICVectAddr1 = (unsigned int)&TIMER1_TIC;    // Install ISR in VIC addr slot 1
  VICVectCntl1 = 0x20 | VIC_TIMER1;            // assign timer1 to interupt vector 1 and enable it

//  VICVectAddr2 = (unsigned int)&UART0RX;        // Install ISR in VIC addr slot 2
//  VICVectCntl2 = 0x20 | VIC_UART0;            // assign uart0 interupt vector 2 and enable it




  VICIntEnable |= (1<<VIC_TIMER0);             // Turn on Timer0 Interrupt
  VICIntEnable |= (1<<VIC_TIMER1);             // Turn on Timer1 Interrupt
  //VICIntEnable |= (1<<VIC_UART0);              // Turn on Timer1 Interrupt



  __enable_interrupt();                         // Global interrupt enable





} // end of function







//----------------------------------------------------------------------------
// Timer 0 ISR used for refreshing the LED display
//
// This function updates the digits, status leds and graph bar.
// Every time this function is called the next digit is refreshed.
//
//----------------------------------------------------------------------------
void TIMER0_ISR ()
{

static char pos=0;  // the index for which digit is being updated
//static char dim=0;  // 


//
// pfet transistors driving segments

// display segment 1 = q1   // left most seven segment display
// display segment 2 = q2
// display segment 3 = q3
// display segment 4 = q4
// display segment 5 = q5
// display segment 6 = q6   // right most seven segment display

// red light bar     = q7
// yellow light bar   = q9
// green light bar  = q8

// led status lights = q10


// lookup table used for selecting digit select transistor when scanning the display
//                    transistor  q1         q2       q3         q4        q5         q6         q10          q7          q8          q9
const long DigitSelect[10] = {0x00040000,0x00020000,0x00000080,0x00000400,0x00080000,0x00001000,0x00400000,0x00000000,0x00000000,0x00000000};



           T0IR =0x02; // clear int flags for timer 0 MR0
           T0TCR=0x1;  // reset timer0, this timer causes an interupt to switch to the next digit

           // turn off all transistors that are driving the display
           IO0SET=0x007e3480; // clear outputs for digit select (pfets active low, so set to 1s clear)
           IO0CLR=0x7e810000; // clear 8 segment outputs by clearing bits (nfets)
 
           // turn on the segment drive transistors (n type fet) for the selected digit (pos)
           // segment drive is sinking current to display segments.
           IO0SET=RAW_DISPLAY[pos]; // the RAWDISPLAY array holds the current display information


           //turn on the digit select (0=on, 1=off) transistors (p type fet) for the selected digit (pos)
           // digit select is sourcing current to 1 of 9  8 segment displays.

           IO0CLR=DigitSelect[pos]; // select segment n
           


           // We fresh one segment every interupt cycle.

           //The 7 segment display is brighter than the bargraph LEDs.
           //To balance the brightness levels we pulse the 7 segment display and status LEDs once every 5 times we
           //pulse the bargraph. This dims the seven segment displays so they are not as bright.

          
           
          
        pos++; // incremnt the pos counter so it is pointing to the next 8 segment display for the next ISR call
                      
           
       
           //pos is the index to which segment we are refreshing for this interupt
           if (pos>6)  // have we refreshed all 6 (0..6) digits of the display
             {
             //  dim++;       // incrememnt the dim counter
             //  if (dim>4)   // for every 5 scans, we refresh the 7 segments display once
              // {
              //   pos=0; // set the index back the first segment
              //   dim=0; // reset the dim counter
              // }
                pos=0; // set the index back the first segment
               
             }
           
//NOTE     
// To solve problems with ghosting on the display for the M1, the dim code above was commented out, and pos=0 was added

           // a segment is pulsed for 1.21ms,  peak current is about 115mA
         T0MR1=0x00001d4c; //set the timer mask register duration for pulse periode



         T0TCR=0x1;  // start timer0
         VICVectAddr = 0;    // Clear interrupt in VIC.

} // end of ISR






//----------------------------------------------------------------------------------------------
//  Timer Tic ISR
//----------------------------------------------------------------------------------------------

void TIMER1_TIC()
{
       T1IR =0x01; // clear int flags for timer 1  MR0
       lTIMERTICS++;
       VICVectAddr = 0;    // Clear interrupt in VIC.
} // end of ISR





//----------------------------------------------------------------------------------------------
//  UART0 ISR
//----------------------------------------------------------------------------------------------

void UART0RX()
{
       T1IR =0x01; // clear int flags for timer 1  MR0
       lTIMERTICS++;
       VICVectAddr = 0;    // Clear interrupt in VIC.
} // end of ISR


