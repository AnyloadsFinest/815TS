
#include "includes.h"
// base address of r0 register used for parameter passing when calling FLASH IAP routine
//#define IAP_LOCATION 0x7ffffff1
//extern long   ZERO_SAVE[_ZERO_TABLE];
//extern struct nvmem NVMEM;     // non volatile memory stored seperatly it's own flash sector flash

extern struct nvmem NVMEM;     // non volatile memory stored seperatly it's own flash sector flash

//------------------------------------------------------------------------------------------
// write calibration data to flash
// To program the flash memory with the Phillips flash API command calls (called IAP).
// On the Phillips ARM parts you cannot access flash on a byte by byte access.
// All flash programming is throught the IAP.
// sector  1  (0x1000 to 0x1fff)  4k block   calibration data and ticket data
//------------------------------------------------------------------------------------------
void write_calibration(void)
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
//CAL.MASTER_CHECKSUM = update_checksum((char *) &CAL.CAL_START, (char *) &CAL.CAL_END); 
//update checksum in calibration structure
typedef void (*IAP)(unsigned long[],unsigned long[]);
IAP iap_entry;
iap_entry=(IAP) IAP_LOCATION; // seting function pointer to point to  0x7ffffff1
// all flash programming will be done using the function iap_entry(command,result);
 CAL.TEST = 0xAA;
 //update checksum in calibration structure
 CAL.MASTER_CHECKSUM = update_checksum((char *) &CAL.CAL_START, (char *) &CAL.CAL_END);  // generate CRC checksum for entire calibration
         
         __disable_interrupt();            // Global interrupt disable   
         
          unlock_flash_sector(17);
          erase_flash_sector(17);                 
          unlock_flash_sector(17);  // unlock flash for write/erase operation
          //-------------------------------------------------------------------------------------
          // write calibration ram to flash
          command[0]=51;   // command 51
          command[1]= (int) _FLASH_CAL_STORE;  // flash address location
          command[2]=(int)&CAL;  // pointer to cal structure
          command[3]=4096;      // block size
          command[4]=14748;//14748;
          iap_entry(command, result);
                    
         // T0TC=0; // reset the display update back to zero
          __enable_interrupt();            // Global interrupt enable
        
        //   check_checksum();       // make sure that the calibration is still intact by checking the checksum
           
}// end of function


// 
//  
//------------------------------------------------------------------------------------------
// read calibration data from flash
// To program the flash memory with the Phillips flash API command calls (called IAP).
// On the Phillips ARM parts you cannot access flash on a byte by byte access.
// All flash programming is throught the IAP.
// sector  1  (0x1000 to 0x1fff)  4k block   calibration data and ticket data
//------------------------------------------------------------------------------------------
void read_calibration(void)
{

        int i;
        int length;
        
        char *p;      // pointer to calibration array in SRAM
        char *flash; // pointer to calibration structure in flash
        
        length = sizeof(CAL); // get the length of the calibration register
                
        p = (char *) &CAL;         // read in calibration
        flash= (char *) _FLASH_CAL_STORE;   // starting position of calibration 0x01000
        
        for (i=0;i!=length;i++)    // loop length of cal structure
        {
          *p++=*flash++;  // copy flash over to calibration structure in ram
        }

}// end of function




//----------------------------------------------------------------------------------------------------
//  Store the current zero value to flash memory
// FLASH memory has a life span of 100K read/erase cycles
// We use a circular tables of 256 entries to reduce wear and tear on the flash
// When the circular table is filled, we the  erase the flash sector and initialize the circular buffer
//----------------------------------------------------------------------------------------------------
void storezero(void)
{
  //char status; 
  NVMEM.ZERO = SCALE.ZERO;
 if (write_circular_flash()==0)
 {
    
 }

} // end of function







//-------------------------------------------------------------------------------------------
// Search through the circular memory and find the last stored zero value
// This is called on indicator startup, reading the last zero from non volatile memory stored in flash
//  
//-------------------------------------------------------------------------------------------
void read_zero_data(void)
{      
  
    float absweight;
           
    SCALE.ZERO=NVMEM.ZERO;
                         
          
           // this is a safety check for a corrupt zero value
           absweight= fabs( (SCALE.ZERO - CAL.DEADLOAD)*CAL.SPAN ); // calculate zero offset weight with ref to deadload
          
                           if (absweight > SCALE.ZERORANGE) // abs weight must be below zerorange to zero
                                  {//2
                                   SCALE.ZERO=CAL.DEADLOAD; // then zero scale
                                  }//2

}// end of function






//------------------------------------------------------------------------------------------
// write checkweigh data to flash
// There are several arrays of data that shares the 4K flash block in sector 2.
// Most if the these arrays are circular buffers, when full, must be reset.
// This requires the flash block to be erased.  This function erases the flash, and writes all
// the memory arrays back to the flash block.  All arrays that share this flash block must be 
// be written back here.
//------------------------------------------------------------------------------------------
void write_checkweigh(void)
{


}// end of function






 
//-------------------------------------------------------------------------------
// NVMEM is a structure of varaibles that are non volatile.  There are stored in a 256byte block.
// NVRAM is stored in sector 2 4k block in flash as a circular buffer of 16 entries.
// This fuction attempts to find an empty 256byte block to write to, and if the sector is full,
// the flash is erased.
//-------------------------------------------------------------------------------
char write_circular_flash(void)
{
unsigned long command[5]; // up tp 4 parameters can be passed to the IAP
unsigned long result[5];  // return results from an IAP operation is stored here
 
   //   int i;
      char *flash; // pointer to calibration structure in flash
   //   long mem;    
      int checksum; 
      int length;
      
//unsigned long save_VicInt;                // for saving of interrupt enable register
//unsigned long save_VicSoftInt;            // for saving of interrupt enable register
// Define a pointer to function type which takes two parameters and returns void.
//CAL.MASTER_CHECKSUM = update_checksum((char *) &CAL.CAL_START, (char *) &CAL.CAL_END); 
//update checksum in calibration structure
typedef void (*IAP)(unsigned long[],unsigned long[]);
IAP iap_entry;
iap_entry=(IAP) IAP_LOCATION; // seting function pointer to point to  0x7ffffff1
// all flash programming will be done using the function iap_entry(command,result);
   
      __disable_interrupt(); 
      // Global interrupt disable
  
            
      flash =  (char *) _FLASH_ZERO_STORE;  // set starting location in flash
                      
      // we found an empty block
      NVMEM.CRC_START=0xAA;  // mark block as used
      NVMEM.CHECKSUM=update_checksum((char *) &NVMEM.CRC_START, (char *) &NVMEM.CRC_END); // calculate checksum of block
        
      unlock_flash_sector(16);  // erase sector 2
      erase_flash_sector(16);
      unlock_flash_sector(16);  // unlock flash for write/erase operation
      //-------------------------------------------------------------------------------------
          // write calibration ram to flash
         
      // write 256 byte block  to flash
      command[0]=51;   // command 51
      command[1]=(int) _FLASH_ZERO_STORE;
      command[2]=(int)&NVMEM;   // get the check start address of the block of memory
      command[3]= 4096; // this is the length
      command[4]= 14748;//14748;14748;; // clock frequency
      iap_entry(command, result);
                
     // T0TC=0; // reset the display update back to zero
      __enable_interrupt();            // Global interrupt enable

      
     length = sizeof(NVMEM) - 8;  // get length of structure minus checksum 
      
     checksum =update_checksum((char *) flash, (char *) (flash + length)); // calculate checksum of block 
          
     
     
      if ( NVMEM.CHECKSUM != checksum)
      {
        
        return(0); 
      }
      else
      {
    
        return(1);
        
      }
      
      
      
  
      
   
   
      
} // end of function



//-------------------------------------------------------------------------------
//  read in non volatile variables from flash memory
//  NVRAM is a structure that stores all non volatile memory including ZERo
//  Any Variable that must be maintained when power is turned off 
//-------------------------------------------------------------------------------
char read_circular_flash(void)
{
    
      int i;
      char *flash; // pointer to calibration structure in flash
     
          
       int length;
       int checksum=0; 
        char *p;      // pointer to calibration array in SRAM
     //  long mem; 
     
      
       length = sizeof(NVMEM); // get the length of the NVMEM structure
                      
       p = (char *) &NVMEM;         // read in calibration
      
       flash= (char *) _FLASH_ZERO_STORE;   // starting position of calibration 0x01000
        
        for (i=0;i!=length;i++)    // loop length of cal structure
        {
          *p++=*flash++;  // copy flash over to calibration structure in ram
        }
          
  
       
    
        
        return(1);
        
    
        
} // end of function





//-------------------------------------------------------------------------------
// call to the IAP to
// unlock flash sector before erasing or writing to flash
//-------------------------------------------------------------------------------
void unlock_flash_sector(char sector)
{
     //Define a pointer to function type which takes two parameters and returns void.
    typedef void (*IAP)(unsigned int [],unsigned int[]);
    IAP iap_entry; 
      
    unsigned int command[5]; // up tp 4 parameters can be passed to the IAP
    unsigned int result[5];  // return results from an IAP operation is stored here
    
    iap_entry=(IAP) IAP_LOCATION; // seting function pointer to point to  0x7ffffff1
    // all flash programming will be done using the function iap_entry(command,result);
    
    
    result[0]=0;
    result[1]=0;
    
   
    // unlock flash for write/erase operation
    command[0]=50; // command 50
    command[1]=sector;  // begin at sector x
    command[2]=sector;  // end at sector x
    command[3]=14748;//14748;14748;  // 12Mhz xtal for flash timing
    iap_entry(command, result); // execute operation

    
} // end of function



//-------------------------------------------------------------------------------
// call to IAP to
// erase flash sector before erasing or writing to flash
//-------------------------------------------------------------------------------
void erase_flash_sector(char sector)
{
     //Define a pointer to function type which takes two parameters and returns void.
    typedef void (*IAP)(unsigned int [],unsigned int[]);
    IAP iap_entry; 
      
    unsigned int command[5]; // up tp 4 parameters can be passed to the IAP
    unsigned int result[5];  // return results from an IAP operation is stored here
    
    iap_entry=(IAP) IAP_LOCATION; // seting function pointer to point to  0x7ffffff1
    // all flash programming will be done using the function iap_entry(command,result);
        
    result[0]=0;
    result[1]=0;
      
    
    // erase flash sector 1
    command[0]=52;     // coamnd 52
    command[1]=sector;      // begin sector 1
    command[2]=sector;      // end at sector 1
    command[3]=147456;//14748;14748;  // 12Mhz xtal for flash timing
    iap_entry(command, result); // erase the block - else you get old + new data mixed !!!

  
  
} // end of function



//------------------------------------------------------------------------------
// load nvm memory
// intitalize last zero value
// initialize accumalator values for ticket system
//------------------------------------------------------------------------------
void load_nvm(void) 
{
  
  int status;
 
  int j;
  status =  read_circular_flash(); // read from circular memory the nvm value
  
  if (status==0)
  {
   SCALE.ZERO=CAL.DEADLOAD;
   
   
    
  }
  else
  {
    
     
      
    read_zero_data();
     
  }
  
  
  
  
} // end of function



