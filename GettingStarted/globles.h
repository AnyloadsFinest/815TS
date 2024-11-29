#ifndef __GLOBLES_INCLUDED
#define __GLOBLES_INCLUDED

#define _TRECORDS  100    // number of tranactions checkweigher can store
#define _XBUFFMAX  40     // buffer length for com port xmit
#define TICKET_BUFFER_SIZE 900    // size of overal ticket buffer memory
#define MMAX 10    // maximum number of menu items

// circular table for storing zero value
//#define _ZERO_TABLE 64
#define _ACC_REGS 6      // how many accumalators we support in the ticket system (ACC0..ACCx)

static long lTIMERTICS ;
static long dTIMERTICS ;

extern struct cal CAL;               // calibration values for scale
extern struct scale SCALE;           // structure for main scale operations
extern struct display DISPLAY;       // structure that holds display information
extern struct system SYSTEM;         // structure for system operations/maintnence
extern struct checkweigh CHECKWEIGH; // strcture for checkweigher values
extern struct com COM[4];               // com 0 structure
extern struct transaction TRANSACTION; // transaction stored for recording checkweigher activity
extern struct stat STAT;             // holds statistical information for checkweighing
extern struct ticket TICKET;         // ticket buffer
extern struct flash_mem FLASH_MEM;   // volatile memory that is backed up in flash
extern struct menu MENU;   // volatile memory that is backed up in flash
extern struct nvmem NVMEM;     // non volatile memory stored seperatly it's own flash sector flash
// structure that holds everything to do with calibrations, recalled from flash and copied into this structure
struct cal
{  
//Audit Trail Calibration Parameters   START  -------------------------------------------------------------------------
  char CAL_START;           //address marker used only for checksum calculation for CAL data
  float SPAN;               //calibration span for scale
  float SPAN3;               //calibration span for scale
  float SPAN2;               //calibration span for scale
  float SPAN1;               //calibration span for scale
  signed long AD3;               //calibration span for scale
  signed long AD2;               //calibration span for scale
  signed long AD1;               //calibration span for scale
  signed long DEADLOAD;     //zero scale value at calibration 
  int CAL_CHECKSUM;         //Checksum for Audit Trail Calibration Parameters
//Audit Trail Calibration Parameters END ------------------------------------------------------------------------------

 //Audit Trail Configuration Parameters START --------------------------------------------------------------------------
  char CFG_START;           //address marker used only for checksum calculation for CONFIG data
  char GRAD;                //grad size of scale (1,2,5) that was used during calibration
  char GRADd;               //this is used to store the grad size selected under calibration menu (1,2,5,10,20,50,100)
  long SCALECAP;            //scale capacity of scale entered from menu
  char DEC;                 //number of decimals displayed
  char DEADZERO;            //number of deadzeros the scale has (1 or 2 zeros i.e: 10,20,50,100)

  char UNITS;               //scale units the scale is calibrated under
  char D_UNITS[3];          //holds the different units enabled to be displayed (kg,lb,g,oz,lb-oz)

  char ALT_UNITS;           //alternate scale units
  char PWR_UNITS;           //what units scale powers up under

  char SCLOVER;             //scale over is capacity

  char REGULA;              // regulatory zone, 0=NTEP,1=CANADA,2=NONE
  char TARELOC;             // lockout tare
  char AUTOTARE;            // auto tare
  char AUTOCLEAR;           // auto clear tare

  // parameters related to filter
  int  FILTER;        //number of samples used in the moving average register
  int  FILTERTHRESH;  //weight change required to trip fastep
  int  FILTERSENS;    //how many samples in a row there is a weight change to trigger fastep
  char FASTSTEP;      //if 0, faststep feature in the filter system is turned off
  char FIR;           //holds which FIR filter we are selecting
  char FLTPRESET;     //filter preset

  // parameters related to motion
  char MOTION;        //how may grads change to trigger motion
  int  MOTION_TIME;   //how long the motion flag will be active in 0.25 sec increments
  char MOTION_BLANK;  //flag that enables blanking display on motion

  // parameters related to zero
  int  ZERORANGE;     //holds the setting for zerorange value 1% to 99%
  int  ZEROTRACK;     //zero track range in % for zero tracking
  //char TRACKENABLE;   //flag that enables zero tracking
  int  PWRZERO;         // flag to indicate that we are auto zeroing the scale on power up
 
  float GFORCES;     //holds the setting for zerorange value 1% to 99%
  float GFORCED;
  float GFSPAN;     //holds the setting for zerorange value 1% to 99%
  
  char LINEAR;             //scale over is capacity  
  char TON;
  
  int  CFG_CHECKSUM;
 //Audit Trail Configuration END -----------------------------------------------
// All calibrations parameters below are not part of the audit trail.

  int  BAUD1;              // baudrate COM1
  char DATABITS1;          // databits COM1
  char COM1_MODE;          // COM1 mode
  char STREAM1;            // string emulation COM1
  char STREAMDELAY;        // serial comunication delay time
  char AUX_COM1;           // auxilary function com1
  char COM2_MODE;          // COM1 mode
  int BAUD2;               // baudrate COM2
  char DATABITS2;          // databits COM2
  char STREAM2;            // string emulation com2
  int BAUD3;               // baudrate COM2
  char DATABITS3;          // databits COM2  
  char COM3_MODE;          // COM1 mode
  char STREAM3;            // string emulation COM1
  
  long CALVALUE;            //last value used to calibrate span
  long PASSWORD;      //calibration password

  char CHECK_ID ;   // enables checkweigher ID key
  char CHECK_MODE;  // checkweigher mode type
  char BAR_MODE;    // display mode for bargraph
  char BAR_BLANK;   // blank light bar on motion
  int  CHECK_THRESH; // checkweigher transaction threshold

  int DISPLAY_RATE; // update rate for display in 0.25sec intervals

  int AUTO_PRINT;  // holds weight threshold to enable autoprint
  char STATS;      // 1=statistical printing 

  int TICKETBUFFER[TICKET_BUFFER_SIZE]; // ticket codes stored in flash
    
  // parameters related to peak hold
  char PEAK_EN;  // flag to enable peak hold is enabled
  long PEAKTHRESH; //set weight threshold to enabe/reset peak detect
  char PEAKRESET; //flag to enable auto clear of peak detect
  char  PEAKDELAY; // timer value for delaying peakdetect
  long PRINTLOCKOUT; // threshold set in calibration for the PRINT LOCKOUT FUNCTION
 
  char PIECE_EN;  // flag to enable peak hold is enabled
  float PIECE_AVG; //set weight threshold to enabe/reset peak detect
  char PIECE_CNT; //flag to enable auto clear of peak detect
    
  char RELAY_EN; // timer value for delaying peakdetect
  long RELAY_SET1; // threshold set in calibration for the PRINT LOCKOUT FUNCTION
  long RELAY_SET2; // ticket codes stored in flash
    
  char BUZZ_EN;
  char ADDRESS;
   
  char LEGAL;
  char GFORCE;
  char STARTUP;
  
  char CLOCK_US_INT; //  0=international  1=US format
  char CLOCK_12_24;   // 0=24hour 1=12hour
  char CLOCKMODE;     // set clock operation mode
  
  int AUDIT_CAL; // holds audit trail count
  int AUDIT_CFG; // holds audit trail count
  int TEST;         //used to test if this is valid calibration from flash
  //-----------------------------------------------------------------------------------
  // to maintain compatibility in calibration versions, all new CAL variables after ver 1.41 are declared below
  
  char CAL_END;
  int MASTER_CHECKSUM;  // CRC16 checksum for calibration structure
 
};// end of structure cal


// This structures holds all values related to weight calculations and general operations of the scale
struct scale
{
long AD;            // ad converter raw counts

long GRADWEIGHT;    //(rawcounts-deadloaad) * span rounded up from a float to a long (no decimals or deadzeros, 1d gradsize )

long SCALEOVER;     // scaleover value in 1d graduation, used to detect scale over condition

float GROSSWEIGHT;  // [(rawcounts-deadloaad) * span]   no formatting (grad size or deadzeros)
float TAREWEIGHT;   // when scale is tared, current grossweight is copied to tareweight, no formatting (grad size or deadzeros)
float NETWEIGHT;    // netweight is grossweight - tareweight

char NETMODE;     // net mode flag, scale is in net mode
char TAREMODE;

long ZERO;        // zero value of scale in AD counts
char NEGATIVE;    // flag that indicates that the scale is negative, used to insert sign character

char OVER;        // scale is over capacity flag, when set the display must balnk

long  DIS_GROSSWEIGHT; //  formatted grossweight without decimals or deadzeros formatting, signed value
long  DIS_TAREWEIGHT;  //  formatted tareweight without decimals or deadzeros formatting, signed value
long  DIS_NETWEIGHT;   //  formatted netweight without decimals or deadzeros formatting, signed value
long  DIS_WEIGHT;      //  weight (gross or net) displayed on display with deadzeros, sign stripped (absolute value)   
char  DIS_DEADZERO;    //  displayed deadzeros, the current deadzero count on the display 0,00,000
char  DIS_DEC;         //  displayed decimals, the decimal position that has to be activated on the display
char  DIS_GRAD;        //  displayed gradsize, the grad size that is being dislayed on the display (1,2,5)
char  DIS_UNITS;       //  displayed scale units, the units that are being displayed
char  DIS_CHECK;       //  holds current status of checkweigher light bar OVER,UNDER,ACCEPT
 
long  DIS_COUNT;      //  weight (gross or net) displayed on display with deadzeros, sign stripped (absolute value)  

char AVLEN;       // legnth of averaging register

long TRACKSENS;   // zero tracking sensitivity in rawcounts
long GRADCOUNT;   // how many AD counts 1 graduation is

char MOTION;       //flag that indicates that there is motion

long ZEROTRK;      // zero track range in rawcounts
long CENTEROFZERO; // center of zero range in rawcounts

long ABSZERO;
char ATZERO;
// 
long ZERORANGE;

long CHECKTARGET;

int PWRZERO;   // power up zero flag

char CHECK_EN;  // checkweigher is enabled when flag is set
char OVERUNDER; // used for selecting over/under when editing check weights

char X10;  // used for testing only for x10 multi

char  IZSM_ERROR; // used to indicate that we could not zero on power up
unsigned long  AD_COUNT;   // used for counting AD samples for the IZSM function
char ZEROSCALE;  // flag used to zero scale
long CHECK_RANGE; //checkweigher range

char BLANK_BAR;
char COM_STREAM0; // flag to indicate to COM0 stream that PRINT has been pressed or poll command
char COM_STREAM1; // flag to indicate to COM1 stream that PRINT has been pressed or poll command
char COM_STREAM2; // flag to indicate to COM1 stream that PRINT has been pressed or poll command

char TICKET;    // curent ticket being accessed

char PEAK;             // when this flag is set, peak weight is locked on the display, this flag is cleared by the CLEAR key on the indicator keypad, automatically depending on mode
char PEAKCLEAR;        // set by CLEAR key and reset by peakhold (used to clear peak hold)
char SETPOINT;
bool PRINTLOCKOUT;     // print lockput flag
char EX_PRINT_EDIT;    // flag that enabled external print edit
char PRINT_ABORT;      // flag that when set by C36 will abort the current print transaction
char TIME_NOSECONDS;   // flag to indicate that when printing time, omit secodes
};


// this struct contain system operations
struct system
{
char DISPLAY_MODE;  // identify what mode the display is in
char CAL; // flag that identifies that we are in calibration mode
int CAL_PARAMETER; //current calibration command selected from menu
char UNITS; // current units being displayed
bool AD_ZERO_CALIBRATE;  // when this flag is true, the AD converter will perform a zero 
};


struct com
{
char BUFFER[_XBUFFMAX]; // buffer that holds 40 characters
char BUFF_XMIT; // flag that indicates the trasmision is complete, buffer is ready for new data
char W;  // buffer character position for writing to buffer
char X;  // transmit character position for transmitting from buffer
bool IDLE; // comport in idle, fishing tranmission
};


struct checkweigh
{
  //   
  long CHECK_OVER[100];        // over value
  long CHECK_ACCEPT[100];      //target value
  long CHECK_UNDER[100];       // under value
  char CHECK_UNITS[100];       // units checkweigh value is stored under
  char CHECK_DEC;              //
  
  long OVER;   // current OVER value used for lightbar
  long UNDER;  // current UNDER value used for light bar
  char ID;     // current ID point to memory location
  
  long ACCEPT_VAL; // current ACCEPT value operator entry
  long OVER_VAL;   // current OVER value operator entry
  long UNDER_VAL;  // current UNDER value operator entry
   
  int CHECK_CNT;    // number of transaction count set for statistics
  
};

// checkweigh transactions
struct transaction
{
   int CNT;             // number of transactions
   long WEIGHT[_TRECORDS];   // 1000 records for weight
   char CHECK[_TRECORDS];    // 1000 records for check status (over,under,accept)
};

struct stat
{
int  OVER;     // over count
int  UNDER;    // under count
int  ACCEPT;   // accept count
float AVG;     // average weight
long  HIGH;    // high weight
long  LOW;     // low weight
float SD;      // standard deviation
float CV;      // Coefficient of varience
int   SS;      // sample count
};

struct ticket
{
int ADDRESS;        // address in ticket array that we are pointing to 
int ACC_INDEX;      // holds accumalator as we are current performing an operation on
bool NO_UNITS;      // flag for unit printing
bool NO_HEADER;     // flag for gross/tare/net printing
char COM_DIRECT;    // holds which COM port we are pointing to
char REL_ADDRESS;    // relative address displayed in ticket editor
}; 

// all contents in this structure cannot exceed 4096 bytes!
struct flash_mem   // sram structure holds all memory writen to a 4k flash block.
{
 struct  checkweigh;  // checkweigh structure
 
};


struct menu
{
char MENU[MMAX][7];  // holds the strings for the menu display
char ITEMS;          //  number of menu itmes to be displayed
char MENUPOS;        // current menu item being selected  
void (*FUNCTION[MMAX])(void); // array of pointer to functions
 
};


struct display
{
  char DIGITS[7]; // holds the digits to be display
  char STATUS1;    // status bits for status display
  char STATUS2;    // status bits for status display
  long LIGHTBAR;  // holds the lightbar information
  char BLINK;     // blink the display (keypress feedback)
 // long TIMERTIC;
  //char SET;
};

struct nvmem     // non volatile memory stored seperatly in flash
{
char CRC_START;  // dummy variabe used for marker
long ZERO; // last ZERO stored
long ACC[_ACC_REGS]; // backup of the working array used for managing ACCumalators 
char CRC_END;   // dummy variable
int CHECKSUM;
};


#endif



