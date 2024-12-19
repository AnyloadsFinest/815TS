/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006 - 2009
 *
 *    File name   : main.c
 *    Description : Define main module
 *
 *    History :
 *    1. Date        : 30, November 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *    2. Date        : 9, February 2009
 *       Author      : Stoyan Choynev
 *       Description : Port for IAR-LPC-1768-SK
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for the IAR LPC-1768-SK board. It shows basic use of the I/O,
 * the timer and the interrupt controllers.
 *  It starts by blinking LED1.
 *
 * Note:
 * After power-up the controller get clock from internal RC oscillator that
 * is unstable and may fail with J-Link auto detect. In this case adaptive clocking
 * should be used. The adaptive clock can be select from menu:
 * Project->Options..., section Debugger->J-Link/J-Trace  JTAG Speed - Adaptive.
 *
 * $Revision: 28 $
 **************************************************************************/
#include "includes.h"
/*************************************************************************
 * Function Name: main
 * Parameters: none
 *
 * Return: none
 *
 * Description: main
 *
 *************************************************************************/
 void main(void)
{
  //////////////////////////////////////////////////////////////////////////////
  // HARDWARE INITIALIAZATION 
  // initialize the low level processor specific tasks
  TargetResetInit();
  init_timer0( TIMER0_TICK_PER_SEC );    // 40ms interval 
  enable_timer( TIMER0 );        // enable timer 0
  init_timer1( TIMER1_TICK_PER_SEC );  // 4ms interval 
  enable_timer( TIMER1 );        // enable timer 1   
  spi_init();                     // setup the spi  
  init_ads1232();       // init startup condition for AD converter, perform zero calibration  
  ADoffset_calibration(); // initiate low level offset calibration of AD converter    
  init_VIC(); //
  // initialize scale system from calibration 
    init_scale();  
    init_relays();
    init_clock();          // initialize RTC
    init_com0();           // set up and initialize com 0
    init_com1();           // set up and initialize com 1
    init_com2();           // set up and initialize com 1
 
if(CAL.STARTUP !=1)
   { 
display_version();     // display software version     
display_test();        // displaytest
   }

warmup_test();

while(1)
{//1 
  status_light(STATUS_RUN);//flash the status light, processor is alive   
  // main system tasks  
  scan_system();    // scan the keypad and process input
  update_display(); // update weight on the display
   
  scan_AD();        // read AD converter value and filter  
 
   get_weight();     // get AD values and calculate weights
  motion();                   // scale motion task
  zero_tracking();            // zero tracking task
  power_up_izsm();            // power up zero task;
  pushbutton_zero();          // process the zero scale task
   
  process_outputstream0();    // process weight strings streamed to com0
  process_outputstream1();    // process weight strings streamed to com1
  process_outputstream2();
    
  process_COM_xmit(_PORT0);  // check if there are any characters to transmit COM 0
  process_COM_xmit(_PORT1);  // check if there are any characters to transmit COM 1 
  process_COM_xmit(_PORT2);  // check if there are any characters to transmit COM 1 
  
  process_serial_poll_commands(_PORT0); // process comport0 poll commands
  process_serial_poll_commands(_PORT1); // process comport1 poll commands
  process_serial_poll_commands(_PORT2); // process comport1 poll commands
  
  relays_function(CAL.RELAY_SET1,CAL.RELAY_SET2); 
  }  // end of main system loop

 }// end of main








