#include "includes.h"






//----------------------------------------------------------------------------
//Display Auditrail CAl and Config
//
//
//----------------------------------------------------------------------------
void display_audit(void)
{


char toggle;
 display_message("AUDIT ",2);
 display_message("TRAIL ",2);
  while(1)
  {

    if (timer(PARAMEDIT_D,TIMEOUT)  )
      {
         if (toggle==1)
         {
           toggle=0;
         }
         else
         {
          toggle=1;
         }

         timer(PARAMEDIT_D,START) ;
      }

  if (toggle)
  {
  displayvalue(CAL.AUDIT_CAL, 2, 0, "");
  DISPLAY.DIGITS[0]='C';
  DISPLAY.DIGITS[1]='A';
  DISPLAY.DIGITS[2]='L';
  }
  else
  {
  displayvalue(CAL.AUDIT_CFG, 2, 0, "");
  DISPLAY.DIGITS[0]='C';
  DISPLAY.DIGITS[1]='F';
  DISPLAY.DIGITS[2]='G';
  }



   if  (scan_keypad(0)==_CLEAR)
     {
       break;
     }
      update_status(1);
      output_display(3);
     


  } // end of while




} // end of function








//----------------------------------------------------------------------------
//  Calculate checksum in memory from a start to end address specified
//
//----------------------------------------------------------------------------
unsigned int update_checksum(char *  start, char *  end )
{

  
unsigned int crc;
int length;
int i;

length=end-start; // how many bytes (* char) in the block of data that we going calculate the CRC
//length++;
crc=0xffff;  // initialize starting CRC value

for (i=0;i!=length;i++) // loop through the block of data we want calculate the checksum for
{
crc=crc16(*start++,crc);  // calculate a running CRC
}

return(crc);

} // end of function




//---------------------------------------------------------------------------
// Calculate a running CRC16 value
// CITT CRC16 polynomial ^16 + ^12 + ^5 + 1 */
// Tested CRC : http://www.zorc.breitbandkatze.de/crc.html
//---------------------------------------------------------------------------
 unsigned int  crc16(unsigned char ser_data, unsigned int crc)
 {






    crc  = (unsigned char)(crc >> 8) | (crc << 8);
    crc ^= ser_data;
    crc ^= (unsigned char)(crc & 0xff) >> 4;
    crc ^= (crc << 8) << 4;
    crc ^= ((crc & 0xff) << 4) << 1;

   return (crc);
 }  // end of function







//----------------------------------------------------------------------------
// check checksum for the entire CAL structure
char check_checksum(void)
{
int checksum;  

//checksum = update_checksum((char *) &CAL.CAL_START, (char *) &CAL.MASTER_CHECKSUM);  // generate CRC checksum for entire calibration
checksum = update_checksum((char *) &CAL.CAL_START, (char *) &CAL.CAL_END); 
//  checksum=update_checksum((char *) &CAL.CAL_START, (char *) &CAL.CAL_CHECKSUM);

if (checksum != CAL.MASTER_CHECKSUM)
{
  
  while(1) {
          
              display_message( "--E5--",0);  
             
              if (scan_keypad(_NORMAL))  // scan the keypad for keypresses
              {
               break; 
              }
             
           }
  
   return(1);   
}


return(0);
}// end of function









