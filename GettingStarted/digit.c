#include "includes.h"
 
// fix for y must be 0x6E instead of 0X66
char  cSETBIT7; // special flag used in stop light versions only
char  bLINK;
long  aCTIVITY_cOUNTER;
long  lAST_dISPLAY;

//extern struct DISPLAY; // structure that holds display information

/*  digit segment values
        a (1)
#####################
#                   #
#                   #
#                   #
# f (20)            # b (2)
#                   #
#                   #
#                   #
#####################
#      g (40)       #
#                   #
#                   #
# e (10)            #c  (4)
#                   #
#                   #
#                   #
#                   #
#####################
         d (8)

example  '9' = 1 + 20 +2 +4 +40 = 67 hex
the digit table below holds the values for different digits

lookup table for numbers and characters 
lookup is by ascii mumber
non valid character is by space, or 0
*/
static char  DIGIT_TABLE[]={                                       //ascii lookup 
 0    , 0    , 0    ,0    ,0    ,0x40 ,0    ,0    ,0x3f ,0x06 ,       //40 - 49
 0x5b , 0x4f , 0x66 ,0x6d ,0x7d ,0x07 ,0x7f ,0x67 ,0    ,0    ,       //50 - 59
 0    , 0    , 0    ,0    ,0    ,0x77 ,0x7c ,0x39 ,0x5e ,0x79 ,       //60 - 69
 0x71 , 0x6f , 0x76 ,0x30 ,0x1e ,0x76 ,0x38 ,0    ,0x54 ,0x5c ,       //70 - 79
 0x73 , 0x67 , 0x50 ,0x6d ,0x78 ,0x3E ,0x3E ,0    ,0x76 ,0x6e ,       //80 - 89
 0x5b , 0    , 0    ,0    ,0    ,0    ,0    ,0x77 ,0x7c ,0x58 ,       //90 - 99 
 0x5e , 0x79 , 0x71 , 0x6f ,0x74 ,0x10 ,0x1e ,0x76,0x38 ,0    ,       //100 - 109 
 0x54 , 0x5c , 0x73 , 0x67 ,0x50 ,0x6d ,0x78 ,0x1c,0x1c ,0    ,       //110 - 119
 0x76 , 0x6e , 0x5b ,0    ,0    ,0    ,0    ,0    ,0    ,0    ,       //120 - 129
                           } ;

//--------------------------------------------------------------------------
// Transmits all characters to the scoreboard
//--------------------------------------------------------------------------
void transmit_digits()
{
char dec1=0;
//char s[8];
char digit[8];
char i;

for (i=0;i!=6;i++) // loop through the string and build numeric display
{//1
 
 digit[i] =DISPLAY.DIGITS[i];
}

digit[0] = '8';
digit[1] ='0';
digit[2] ='8';
digit[3] ='0';
digit[4]='8';
digit[5]='0';
  if(!refresh_timer(TIMEOUT)) 
    {
    return;  
    }                      
//  refresh_timer(START);
 
send_digit(digit[0]);  // digit 0 MSB
send_digit(digit[1]);  // digit 0 MSB
send_digit(digit[2]);  // digit 0 MSB
latchbankA(1); // latch the new values to bank A
latchbankA(0);   //
send_digit(digit[3]);  // digit 0 MSB
send_digit(digit[4]);  // digit 0 MSB
send_digit(digit[5]);  // digit 0 MSB
latchbankB(1); // latch the new values to bank A
latchbankB(0);   //
//Output_Enable (1);  // Enable the driver output

/*
if(dec1==5)
{
cSETBIT7=1; // insert decimal
}
send_digit(digit[0]);  // digit 0 MSB
latchbankA(1); // latch the new values to bank A
latchbankA(0);   //
Digit_Select(4); // selsect the digit
Output_Enable (0);
update_display();
Digit_Select(0); // selsect the digit     
Output_Enable (1);  // Enable the driver output

// Digit 1
//cSETBIT7=0;
if(dec1==4)
{
cSETBIT7=1; // insert decimal
}
send_digit(digit[1]);  // digit 1
latchbankA(1); // latch the new values to bank A
latchbankA(0);
Digit_Select(5);  // select the digit
Output_Enable (0);
update_display();
Digit_Select(0); // selsect the digit             
Output_Enable (1);  // Enable the driver output

// digit 2
//cSETBIT7=0;
if(dec1==3)
{
cSETBIT7=1; // insert decimal
}
send_digit(digit[2]);  // digit 2
latchbankA(1); // latch the new values to bank A
latchbankA(0);
Digit_Select(6);   // select the digit
Output_Enable (0);
update_display();
Digit_Select(0); // selsect the digit             
Output_Enable (1);  // Enable the driver output

// digit 3 
//cSETBIT7=0;
if(dec1==2)
{
cSETBIT7=1; // insert decimal
}
send_digit(digit[3]);  // digit 3
latchbankA(1); // latch the new values to bank A
latchbankA(0);      
Digit_Select(1);    // select the digit
Output_Enable (0);  // Enable the driver output
update_display();
Digit_Select(0); // selsect the digit             
Output_Enable (1);  // Enable the driver output

// digit 4
//cSETBIT7=0;
if(dec1==1)
{
cSETBIT7=1; // insert decimal
}
send_digit(digit[4]);  // digit 4
latchbankA(1); // latch the new values to bank A
latchbankA(0);
Digit_Select(2);
Output_Enable (0);  // Enable the driver output
update_display();
Digit_Select(0); // selsect the digit             
Output_Enable (1);  // Enable the driver output/

// LSB digit 5 
//cSETBIT7=0;
send_digit(digit[5]);  // digit 5 LSB
latchbankA(1); // latch the new values to bank A
latchbankA(0);
Digit_Select(3);
Output_Enable (0);  // Enable the driver output
update_display();
Digit_Select(0); // selsect the digit             
Output_Enable (1);  // Enable the driver output
// end of digit scanning from left to right

*/

} // end of funtion



///////////////////////////////////////////////////////////////////////////////
void refresh_display(void)

{// update digit

 int s;
 int usec = 20 + 1 * 40;
 //int usec = 20 + BOARD.DBRIGHT * 40;
 
 for(s=0; s < usec; s++);// delay  1useconds = 1/2usec 
  
 
}// up date digit

//-------------------------------------------------------------------------
// Character lookup
//-------------------------------------------------------------------------
void send_digit(char digit)
{
//char mirror;

if ( (digit < 40) || (digit > 122) )// if below ascii 40 blank digit and return
 {   
 digit = 0;   
 } // 
else
{ // normal digit
digit = digit - 40; // subtract 40 for lookup table index
digit = DIGIT_TABLE[digit];

/*
if (BOARD.MIRROR==1)  // check if we are mirroring digits
 {//1
   // to mirror we swap f,b segments and e,c segments
   mirror = digit;         // make a copy  
   digit = digit & 0x49;   // clear segments  f,b,e,c

   if (mirror & 0x02) digit = digit | 0x20;  // swap segment  b to f
   if (mirror & 0x20) digit = digit | 0x02;  // swap segment  f to b       
   if (mirror & 0x04) digit = digit | 0x10;  // swap segment  c to e
   if (mirror & 0x10) digit = digit | 0x04;  // swap segment  e to c
   // 
 }//1
*/
}// normal digit

if (cSETBIT7 == 1) // special flag used to activate 7th driver bit. Only used on scoreboards with built in traffic light
{                // as the status lights for GR and NT are driven from the 7th bit of the digit driver on panel A only.
digit = digit | 0x80; // 128
cSETBIT7 = 0;
}
// 
spi_write8bits(digit);
} // end of funtion

 
