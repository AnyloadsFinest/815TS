#include "includes.h"



//-------------------------------------------------------------------------------
// init_scale()
// start up initializeation of scale
//-------------------------------------------------------------------------------
void init_scale(void)
{
  
  
read_calibration();  // read calibration
  
if (CAL.TEST!=0xAA) // check if this is a brand new indicator with no calibration
{ 
  factory_init();   // init factory values
    
  
  RTCSEC = 1;
  RTCMIN = 1;
  RTCHOUR = 12;
  RTCDOM =  1;
  RTCMONTH = 12;
  RTCYEAR = 23; 
  // RTCDOW = Time.RTC_Wday;
//  RTCDOY = Time.RTC_Yday;  
  CAL.AUDIT_CAL = 0; // holds audit trail count
  CAL.AUDIT_CFG = 0; // holds audit trail count  
  CAL.TEST=0xAA;   // set flag  
    
  cycle_display(); // cycle display for factory testing
 // CAL.CAL_START = 0XAA;
 // CAL.CAL_END = 0XAA;  
 // CAL.MASTER_CHECKSUM = update_checksum((char *) &CAL.CAL_START, (char *) &CAL.CAL_END);  // generate CRC checksum for entire calibration
    
}

SCALE.CENTEROFZERO=calc_centerofzero();  // calculate the center of zero range
SCALE.ZERORANGE=calc_zerorange(); // calculate the zero range of the indcator
SCALE.PWRZERO=CAL.PWRZERO; // set the power up zero flag
SCALE.DIS_GRAD=CAL.GRAD;        // redudant
SCALE.DIS_UNITS=CAL.D_UNITS[0];  // set the power up units option

load_nvm();       // load non volatile memory  (zero and acc variables)

calc_scaleover(); // calculate scale over value
set_trkval(); // calculate the zero tracking range
initialize_FIR(CAL.FIR);  // select FIR filter 1hz,2hz,3hz
calculate_targets();  // calculate checkweigh values

SYSTEM.CAL=0;              // system  calibration mode
SYSTEM.CAL_PARAMETER=1;    // default starting menu calibration parameter is 1

SCALE.CHECK_EN=CAL.CHECK_MODE; // is light bar enabled

SCALE.IZSM_ERROR=0;        // initialize to zero
SCALE.AD_COUNT=0;          // init free runing sample count

TICKET.COM_DIRECT=0;       // used for redirecting comport in ticket printing
SCALE.PRINT_ABORT=false;    // clear this flag
}

//------------------------------------------------------------------
// factory init
// first time initialize the flash memory, setting to factory defualts
//-------------------------------------------------------------------

void factory_init(void)
{

SCALE.ZERO = -500;
CAL.DEADLOAD = -500;
CAL.SPAN = 0.05;

CAL.SPAN3= 0.05;
CAL.SPAN2= 0.05;
CAL.SPAN1= 0.05;

CAL.AD3= -500;
CAL.AD2= -500;
CAL.AD1= -500;

CAL.LINEAR=0;             //scale over is capacity  
CAL.TON = 1;

CAL.GRAD=1;              //grad size set to 1
CAL.GRADd=1;
CAL.DEADZERO=0;          //no deadzeros
CAL.MOTION_BLANK=0;      //flag that enables blanking display on motion

CAL.FILTER=50;            //number of samples used in the moving average register
CAL.FILTERTHRESH=4;      //weight change required to trip fastep
CAL.FILTERSENS=5;        //how many samples in a row there is a weight change to trigger fastep
CAL.FASTSTEP=1;          //fastep is enabled
CAL.FIR=3;               //which FIR filter selected
CAL.FLTPRESET=3;         //filter preset

CAL.DISPLAY_RATE=1;      //display update rate in 0.25sec increments

CAL.MOTION=2;            //how may grads change to trigger motion
CAL.MOTION_TIME=4;      //how long the motion flag will be active in 0.25 sec increments

CAL.UNITS=_KG;           //default cal units is kgs
CAL.D_UNITS[0]=_KG;      //set the power up units
CAL.D_UNITS[1]=_LB;      // set the power up units option
CAL.D_UNITS[2]=0;        // set the power up units option

CAL.ALT_UNITS=1;         //default alternative units is lb
CAL.PWR_UNITS=0;

CAL.SCALECAP=5000;      //scale capacity of scale

CAL.PASSWORD=1;       //default factory password
CAL.SCLOVER=2;
CAL.ZERORANGE=2;        //holds the setting for zerorange value 1% to 99%
CAL.ZEROTRACK=50;       //zero track range in % for zero tracking

CAL.PWRZERO=0;          // power up zero off

CAL.CALVALUE=5000;       //defualt value when SPANNING
CAL.DEC=0 ;               //decimals=0;

CAL.REGULA=2;             // regulatory zone, 2=NTEP,1=CANADA,0=NONE
CAL.TARELOC=0;            // lockout tare
CAL.AUTOTARE=0;           // auto tare
CAL.AUTOCLEAR=0;          // auto clear tare

CAL.BAUD1=9600;            // baudrate COM1
CAL.DATABITS1=0;          // databits COM1
CAL.COM1_MODE=0;          // COM1 mode
CAL.STREAM1=0;            // string emulation COM1
CAL.STREAMDELAY=1;        // serial comunication delay time
CAL.AUX_COM1=0;           // auxilary function com1
CAL.COM2_MODE=1;          // COM1 mode
CAL.BAUD2=9600;              // baudrate COM2
CAL.DATABITS2=0;          // databits COM2
CAL.STREAM2=0;            // string emulation com2
CAL.BAUD3=9600;              // baudrate COM2
CAL.DATABITS3=0;          // databits COM2
CAL.STREAM3=0;            // string emulation com2
CAL.COM3_MODE=3;          // COM1 mode
 
CAL.BAR_MODE=2;           //display mode for bargraph
CAL.CHECK_ID=0;           //disable checkweigher ID key
CAL.CHECK_MODE=1;         //enable checkweigher mode type
CAL.BAR_BLANK=0;          // disable motion blanking of light bar

CAL.AUTO_PRINT=0;         //auto print is turned off
CAL.CHECK_THRESH=50;      //threshold is 50% for transactions
CAL.STATS=0;              // disable statistics

 // parameters related to peak hold
CAL.PEAK_EN=0;  //  peak hold is disabled
CAL.PEAKTHRESH=0; //peak weight threshold is 0
CAL.PEAKRESET=0; //flag to enable auto clear of peak detect = 0
CAL.PEAKDELAY=0; // timer value for delaying peakdetect = 0

CAL.PIECE_EN=0;  // flag to enable peak hold is enabled
CAL.PIECE_AVG=0; //set weight threshold to enabe/reset peak detect
CAL.PIECE_CNT=5; //flag to enable auto clear of peak detect
    
CAL.RELAY_EN=0; // timer value for delaying peakdetect
CAL.RELAY_SET1=0; // threshold set in calibration for the PRINT LOCKOUT FUNCTION
CAL.RELAY_SET2=0; // ticket codes stored in flash

CAL.BUZZ_EN=1;
CAL.ADDRESS=1;

CAL.LEGAL =0;
CAL.GFORCE =0;
CAL.STARTUP =0;
CAL.GFORCES = 9.8000;     //holds the setting for zerorange value 1% to 99%
CAL.GFORCED = 9.8000 ;
CAL.GFSPAN = 1.000;

CAL.PRINTLOCKOUT=10;  // default print lockout value
// clock mode of operation
CAL.CLOCKMODE = 0;   //set default configuration
CAL.CLOCK_US_INT=0;  // 0=international  1=US date format  
CAL.CLOCK_12_24=1;   // 0=24hour 1=12hour clcok       


} // end of function

//---------------------------------------------------------------------------------------------
void cycle_display(void)
{
 char i;
 char j;
 char k;
 int key;
 char l;

 char keytest=0;

 SCALE.CHECK_EN=1; // enables light bar
 j='0';

 timer(PARAMEDIT_B,START);
 DISPLAY.LIGHTBAR=0x00010101;
 DISPLAY.STATUS1=0xaa;
 DISPLAY.STATUS2=0xaa;
 CAL.CHECK_MODE=1;
 CAL.CHECK_ID=0;

 while(1)
 {//2

     for (i=0;i!=6;i++)
      {//3
       DISPLAY.DIGITS[i]=j+'0';
      }//3



     if (keytest==0) // if we are not testing keys
     {
      j++; // cycle display count
     }
     else
     {
      j=keytest;
     }


      if (l>10)
      {

        l=0;
        keytest=0;
      }

     l++;
// 
 update_bar_graph(0,0);  // output bargraph

      if (j>9)
      {//4
        j=0;
         DISPLAY.LIGHTBAR=0x00010101;
      }//4

    if (j<7)
    {//5
      DISPLAY.LIGHTBAR=DISPLAY.LIGHTBAR<<1;
    }//5
    else
    {//6
       DISPLAY.LIGHTBAR=0x00ffffff;
    }//6

    if (k>1)
    {//7
     k=0;
     DISPLAY.STATUS1=0xaa;
      DISPLAY.STATUS2=0xaa;
      FIO3SET = 0x02000000; // disable output
      FIO1SET = 0x00030000; // disable output
      
    }//7
    else
    {//8
     DISPLAY.STATUS1=0x55;
     DISPLAY.STATUS2=0x55;
     FIO3CLR = 0x02000000; // disable output
     FIO1CLR = 0x00030000; // disable output
    }//8

    k++;

    update_status(0);  // output status
    output_display(j); // output digits and decimal position

    while(1) // delay, stay in this loop
    {//9

      key=scan_keypad(_NORMAL);

      
       if ( key ) // if key pressed
         {
           
           return;
         }
      
      if(SYSTEM.CAL)
      {//10

        if ( key ) // if key pressed
         {
           DISPLAY.LIGHTBAR=0;
           update_bar_graph(0,0);
           return;
         }
      }//10
      else  //  in burn in mode
      {//11


 //      if (key== _TAREUNITS_H)
   //     {
  //         SYSTEM.CAL=1; // enter into calibration mode from burn in mode
  //         return;
  //      }



        if (key == _CLEAR_H) // enter into weight mode for testing
         {
           return;
         }




       if (key)
       {
         keytest=key;
         l=0;
         break; // break out of while loop
       }

       }//11

      output_display(j); // output digits and decimal position

        if (timer(SECOND,TIMEOUT))
       {
        timer(SECOND,START);
        break;
        }

      }//9  end of while delay loop

 }//2



}  // end of function
