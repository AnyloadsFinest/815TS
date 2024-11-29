#include "includes.h"

////////////////////////////////////////////////////////////////////
// load_calibration()
//
// load calibration from flash
//
///////////////////////////////////////////////////////////////////
void load_calibration(void)
{
     flash_recall_cal(); // copy flash memory to CAL structure
    // flash memory that has not been written to will always be 0xff
    // a marker is set to 0xaa to indicate that previous flash calibration has occurred
    // firstime initialization
   
      
} // end of function
 

void flash_save_cal(void)
 {  
 //------------------------------------------------------------------------------------------
// write calibration data to flash
// To program the flash memory with the Phillips flash API command calls (called IAP).
// On the Phillips ARM parts you cannot access flash on a byte by byte access.
// All flash programming is throught the IAP.
//------------------------------------------------------------------------------------------
unsigned long command[5]; // up tp 4 parameters can be passed to the IAP
unsigned long result[5];  // return results from an IAP operation is stored here
//unsigned long save_VicInt;                // for saving of interrupt enable register
//unsigned long save_VicSoftInt;            // for saving of interrupt enable register
// Define a pointer to function type which takes two parameters and returns void.

CAL.MASTER_CHECKSUM = update_checksum((char *) &CAL.CAL_START, (char *) &CAL.CAL_END); 

typedef void (*IAP)(unsigned long[],unsigned long[]);
IAP iap_entry;
iap_entry=(IAP) IAP_LOCATION; // seting function pointer to point to  0x7ffffff1
// all flash programming will be done using the function iap_entry(command,result);
      
__disable_interrupt();       // Global interrupt disable

   //CAL.FirstTime=0x55;  // flag to indicate that the FLASH needs to be initialized  
  //   save_VicInt = VICIntEnable;              // save interrupt enable status
 //   save_VicSoftInt = VICSoftInt;            // save interrupt enable status
  //   update_checksum(&CAL.CAL_START,&CAL.CAL_END); 
 //   VICIntEnClr =   0xFFFFFFFF;              // disable all interrupts
 //   VICSoftIntClr = 0xFFFFFFFF;              // disable all interrupts
    command[0]=0; // command 50
    command[1]=0;  // begin at sector 10
    command[2]=0;  // end at sector 10
    command[3]=0;  // begin at sector 10
    command[4]=0;  // end at sector 10
   
    result[0]=0; // command 50
    result[1]=0;  // begin at sector 10
    result[2]=0;  // end at sector 10
    result[3]=0;  // begin at sector 10
    result[4]=0;  // end at sector 10
   
    // unlock flash for write/erase operation
    command[0]=50; // command 50
    command[1]=17;  // begin at sector 10
    command[2]=17;  // end at sector 10
    iap_entry(command, result);
    // erase flash sector 8
    command[0]=52;     // coamnd 52
    command[1]=17;      // begin sector 10
    command[2]=17;      // end at sector 10
    command[3]=14748;  // 12Mhz xtal for flash timing
    iap_entry(command, result); 
    // erase the block - else you get old + new data mixed !!!
    // unlock flash for write/erase operation
    command[0]=50; // command 50
    command[1]=17;  // begin at sector 7
    command[2]=17;  // end at sector 7
    iap_entry(command, result); // 
    // write ram to flash
    command[0]=51;        // command 51
    command[1]= CAL_LOCATION;  // flash address location
    command[2]=(unsigned long)&CAL;
    command[3]= 256;
    command[4]=14748; //
    iap_entry(command, result);
 
    __enable_interrupt();            // Global interrupt enable
 } // end of function

// 

void flash_recall_cal(void)
 { 
//------------------------------------------------------------------------------
// read calibration data to flash
// To program the flash memory with the Phillips flash API command calls (called IAP).
// On the Phillips ARM parts you cannot access flash on a byte by byte access.
// All flash programming is throught the IAP.
//------------------------------------------------------------------------------
int i;
int length;    //
char *p;      // pointer to calibration array in SRAM
char *flash; // pointer to calibration structure in flash

length = sizeof(CAL); // get the length of the calibration register
p = (char *) &CAL;         // read in calibration
flash= (char *) CAL_LOCATION;   // starting position of calibration

for (i=0;i!=length;i++)
{
  *p++=*flash++;
} // 


 } // end of function



void system_init(char mode)
{
           
                
}  


void board_system_init(void)
{
                                                 
    
}  // 

void string_init(char mode)
{                   
  

                
}
