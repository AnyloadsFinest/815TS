#include "includes.h"

long   RAW_DISPLAY[12]; // holds raw binary pattern for scanning LED display

// fix for y must be 0x6E instead of 0X66
char  cSETBIT7; // special flag used in stop light versions only
char  bLINK;
long  aCTIVITY_cOUNTER;
long  lAST_dISPLAY;
extern char BEAT;
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
char  DIGIT_TABLE[]={                                       //ascii lookup 
 0    , 0    , 0    ,0    ,0    ,0x40 ,0    ,0    ,0x3f ,0x06 ,       //40 - 49
 0x5b , 0x4f , 0x66 ,0x6d ,0x7d ,0x07 ,0x7f ,0x6F ,0    ,0    ,       //50 - 59 0x67
 0    , 0    , 0    ,0    ,0    ,0x77 ,0x7c ,0x39 ,0x5e ,0x79 ,       //60 - 69
 0x71 , 0x3D , 0x76 ,0x30 ,0x1e ,0x76 ,0x38 ,0    ,0x54 ,0x5c ,       //70 - 79 0X6F for G
 0x73 , 0x67 , 0x50 ,0x6d ,0x78 ,0x3E ,0x3E ,0    ,0x76 ,0x6e ,       //80 - 89
 0x5b , 0    , 0    ,0    ,0    ,0    ,0    ,0x77 ,0x7c ,0x58 ,       //90 - 99 
 0x5e , 0x79 , 0x71 , 0x3D ,0x74 ,0x10 ,0x1e ,0x76,0x38 ,0    ,       //100 - 109 0X6F for G
 0x54 , 0x5c , 0x73 , 0x67 ,0x50 ,0x6d ,0x78 ,0x1c,0x1c ,0    ,       //110 - 119
 0x76 , 0x6e , 0x5b ,0    ,0    ,0    ,0    ,0    ,0    ,0    ,       //120 - 129
                           } ;

//---------------------------------------------------------------------------------------------
// This fuction translates SYSTEM.DIGITS[] to  7 segment display driven by the processor
//  output on the ARM 7 is 32 bit IO, ports 23 to 29 drive the 7 segments

//                            a (p23)
//                   ####################
//                   #                  #
//         f (p29)   #                  #  b (p25)
//                   #                  #
//                   #       g (p30)    #
//                   ####################
//                   #                  #
//         e (p28)   #                  #  c (p26)
//                   #                  #
//                   #                  #
//                   ####################
//                            d (p27)
//
//
//    p31 p30 p29 p28 p27 p26 p25 p24 p23
// a   0   0   0   0   0   0   0   0   1        0x008
// b   0   0   0   0   0   0   1   0   0        0x020
// c   0   0   0   0   0   1   0   0   0        0x040
// d   0   0   0   0   1   0   0   0   0        0x080
// e   0   0   0   1   0   0   0   0   0        0x100
// f   0   0   1   0   0   0   0   0   0        0x200
// g   0   1   0   0   0   0   0   0   0        0x400
//---------------------------------------------------------------------------------------------
void output_display(char decimal)
{
char i;
//char digit;
//long temp;
//char specialdec=0;
// lookup table for seven segment display
//                                0           1         2          3          4          5           6         7           8          9
//const long DIGIT_TABLE[10] = {0x3e800000,0x06000000,0x5a800000,0x4e800000,0x66000000,0x6c800000,0x7c800000,0x06800000,0x7e800000,0x66800000};
//                               A          B           C          D          E         F           G          H         I          J
//const long ALPHA_TABLE[27] = {0x76800000,0x7c000000,0x38800000,0x5e000000,0x78800000,0x70800000,0x3c800000,0x76000000,0x04000000,0x00000000,
//                               K          L          M          N           O         P         Q          R           S          T
//                              0x00000000,0x38000000,0x00000000,0x54000000,0x3e800000,0x72800000,0x00000000,0x50000000,0x6c800000,0x70000000,
//                               U         V            W         X         Y            Z          -
//                             0x3E000000,0x3E000000,0x00000000,0x00000000,0x6e000000,0x40000000,0x40000000};
// 
// 
// decimal is driven by p16 and is set seperatly

if (DISPLAY.BLINK >0)  // check if we have to blink the display
 {//1
   DISPLAY.BLINK--;  // decrement display blcnk count

   for (i=0;i!=7;i++)  // blank the display
   {
    DISPLAY.DIGITS[i]=' ';
   }

 }//1
//decimal = 6 - decimal;
//scale_status();
transmit_digits(decimal);

/*
for (i=0;i!=6;i++) // loop through the string and build numeric display
{//1
 
  // insert minus sign
  if (DISPLAY.DIGITS[i] =='-') // insert minus sign is left most segment
  {
   DISPLAY.DIGITS[i] ='Z';  // insert minus (we use 'Z' to represent -)
  }

  digit=DISPLAY.DIGITS[i];

   if  ( (digit > 47) && (digit < 58) ) // check that it is a digit between 0 and 9
   {//2

      digit=digit-48; // subtract ascii value
      temp=DIGIT_TABLE[digit]; // we need to do this to prevent flicker with decimals

      
      
  if (i==decimal) // check if we are activating the decimal
      {
        if (i!=5) // no decimals ever on the right most segment
        {
           temp=temp+0x10000;  // p16 drives the decimal anunciator
        }

      }



      RAW_DISPLAY[i]=temp;
   }//2
   else  // check for alphanumeric
   {//3
          if  ( (digit > 64) && (digit < 91) ) // else check that it is a digit between A and Z
            {//4
               digit=digit-65; // subtract ascii value
               temp=ALPHA_TABLE[digit];
             //   temp=DIGIT_TABLE[digit]; // we need to do this to prevent flicker with decimals
               

       if (i==decimal) // check if we are activating the decimal
        {
            if (i!=5) // no decimals ever on the right most segment
            {
              temp=temp+0x10000;  // p16 drives the decimal anunciator
            }
        }

               RAW_DISPLAY[i]=temp;
            }//4
            else
            {//5
            RAW_DISPLAY[i]=0;
            }//5


   }//3


}//1  end of loop





  switch(specialdec)  // special condition for displaying clock
  {            // insert multiple decimals
        case 100: {
                  RAW_DISPLAY[1]=RAW_DISPLAY[1]+0x10000;
                  RAW_DISPLAY[3]=RAW_DISPLAY[3]+0x10000;
                  break;
                  }
        
        case 101: {
                  RAW_DISPLAY[2]=RAW_DISPLAY[2]+0x10000;
                  
                  }        
        
 
  } // end of switch

*/

} // end of function


//-------------------------------------------------------------------------
//  update LED bar graph on display
//  There are three lightbars of 8 bits each
//  parse a single 24bit word into three 8bit lightbars
//-------------------------------------------------------------------------
void update_bar_graph(char flash,char blank)
{
char i;
char j;
char k;

long bar;
long barbits;

static char blink;

//lookup table for I/O that are mapped to lightbar segments
//                          segment 0  segment 1  segment 2   segment 3  segment 4  segment 5  segment 6 segment 7
//const long BAR_TABLE[8] = {0x08000000,0x04000000,0x02000000,0x00800000,0x00010000,0x40000000,0x20000000,0x10000000};
const long BAR_TABLE[8] = {0x10000000,0x20000000,0x40000000,0x00010000,0x00800000,0x02000000,0x04000000,0x08000000};


bar = DISPLAY.LIGHTBAR; // get a copy of the current display of the lightbar



if (flash)
{//1
  // blink the bar
  if (timer(BAR,TIMEOUT))
   {//1
    timer(BAR,START);

     if (blink==1) blink=0;
      else
        blink=1;
    }//1

   if (blink)
   {
     bar=0;
   }






} //1


 if (blank)
   bar=0;


  if (CAL.CHECK_MODE==0)  // if the checkweigher is not enabled, then blank the display
  {
    bar=0;
  }

  // check if we are blanking bargraph on motion
if (CAL.BAR_BLANK==1)
{
  
  // if we are balnking bar, and not in TARGET display mode (need bar to display targetes)
  if ((SCALE.BLANK_BAR) && ( SYSTEM.DISPLAY_MODE != _TARGET_D) ) 
  {
     bar=0;
  
  }
      
}
  
  
  
  
  
  if (SCALE.CHECK_EN==0)
  {
    bar=0;
  }

  
  
  
  

//bar=0xffffff;

j=0;
//k=6; // starting position for bar graph in RAW_DISPLAY[] array
k=7; // starting position for bar graph in RAW_DISPLAY[] array
barbits=0;

                 



for (i=0;i!=24;i++) // loop through all 24 bits (bars) of light bar
{//1
     // shift right 24bit word and check first bit, do this 24 times

      if (bar&0x1)  // is the first bit set?
      {//3
        barbits=barbits | BAR_TABLE[j]; // logical OR which segments will be turned for a 8 bit lightbar
      }//3

       j++; // switch to next segment bit. There are 8 bits per light bar
       bar=bar>>1; // right shift by 1

       if (j==8) // are we finish processing 8 bits of a lightbar?
         {//2
           RAW_DISPLAY[k]=barbits; // copy lightbar bits to display data
           k++;  // increment to the next light bar (there are three of them)
           j=0;  // reset j to process the next 8 bits
           barbits=0; // reset light bar bits for display
         }//2






}//1

}// end of function



//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
void update_status(char blank)
{

//char i;
//char j;
//char stat;
//long statbits;

//lookup table for I/O that are mapped to lightbar segments
//                          segment 0  segment 1  segment 2   segment 3  segment 4  segment 5  segment 6 segment 7
//const long STATUS_TABLE[8] = {0x08000000,0x04000000,0x02000000,0x00800000,0x00010000,0x40000000,0x20000000,0x10000000};
//const long STATUS_TABLE[8] ={0x10000000,0x20000000,0x40000000,0x00010000,0x00800000,0x02000000,0x04000000,0x08000000};
//const long STATUS_TABLE[8] ={0x10,0x20,0x40,0x80,0x01,0x02,0x04,0x08};

if (blank==1) 
{
DISPLAY.STATUS1=0;
DISPLAY.STATUS2=0;
}
else if (blank==2) 
{
  if(BEAT)
  {
  DISPLAY.STATUS1=0;
  DISPLAY.STATUS2=0;  
  }
     
}
/*
stat = DISPLAY.STATUS1; // get a copy of the current status


if (blank) stat=0; // blank status



statbits=0;
j=0;
for (i=0;i!=8;i++) // loop through all 8 bits of status LEDs
{//1
     // shift right 24bit word and check first bit, do this 24 times

      if (stat&0x1)  // is the first bit set?
      {//3
        statbits=statbits | STATUS_TABLE[j]; // logical OR which segments will be turned for a 8 bit lightbar
      }//3

       j++; // switch to next led bit. There are 8 leds used for status display
       stat=stat>>1; // right shift by 1

}//1

RAW_DISPLAY[6]=statbits; // update raw bits display
*/
} // end of function





//------------------------------------------------------------------------------------
//  update the status lights on the scale
//
//------------------------------------------------------------------------------------
void scale_status(void)
{

//1 = nt
//2 = GR
//4 = motion
//8  = centre of zero
//10 = g
//20 = oz
//40 = kg
//80=  lb

 DISPLAY.STATUS1=0;
 DISPLAY.STATUS2=0;

 
 if (CAL.PEAK_EN !=0)   
   
 {
   
   if(BEAT)
     {
       DISPLAY.STATUS1 = DISPLAY.STATUS1 | 0X08;  //  scale motion 
     }
   
   if( SCALE.PEAK ==1 )
   {
   DISPLAY.STATUS1 = DISPLAY.STATUS1 | 0X08;  //  scale motion
   }
  
   }
 
  if (CAL.PIECE_EN !=0)  
    
  {
    
            if(BEAT)
     {
       DISPLAY.STATUS1 = DISPLAY.STATUS1 | 0X20;  //  scale motion 
     }
    
      if (SYSTEM.DISPLAY_MODE== _PIECE)
                 {
               DISPLAY.STATUS1 = DISPLAY.STATUS1 | 0X20;  //  center of zero
                 }
         
  }
  
 if (CAL.RELAY_EN !=0) 
 {
   
      if(BEAT)
     {
       DISPLAY.STATUS1 = DISPLAY.STATUS1 | 0X10;  //  scale motion 
     }
       
   
   if(SCALE.SETPOINT == 1)
   {
   DISPLAY.STATUS1 = DISPLAY.STATUS1 | 0X10;  //  center of zero
   }
  
 // 
   
   }

  if (SYSTEM.DISPLAY_MODE != _PIECE)
                 {
 
    switch(SCALE.DIS_UNITS)

    { // 

    case _KG:  {
               DISPLAY.STATUS1 = DISPLAY.STATUS1 | 0x04;  //  kg
              break;
              }

    case _LB:  {
               DISPLAY.STATUS1 = DISPLAY.STATUS1 | 0x01;  //  lb
              break;
              }
    
    case _OZ:  {
              DISPLAY.STATUS1 = DISPLAY.STATUS1 | 0x02;  //  oz
              break;
              }

     case _G:  {
              DISPLAY.STATUS2 = DISPLAY.STATUS2 | 0x20;  //  g
              break;
              }

    case _TON:  {
               DISPLAY.STATUS2 = DISPLAY.STATUS2 | 0x20;  //  g
          //      DISPLAY.STATUS1 = DISPLAY.STATUS1 | 0x04;  //  kg
              break;
              }           
              
      case _STON:  {
        
       //   DISPLAY.STATUS1 = DISPLAY.STATUS1 | 0x02;  //  oz
               DISPLAY.STATUS2 = DISPLAY.STATUS2 | 0x20;  //  g
          //      DISPLAY.STATUS1 = DISPLAY.STATUS1 | 0x04;  //  kg
              break;
              } 
          
            case _LTON:  {
        
        //  DISPLAY.STATUS1 = DISPLAY.STATUS1 | 0x02;  //  oz
               DISPLAY.STATUS2 = DISPLAY.STATUS2 | 0x20;  //  g
          //      DISPLAY.STATUS1 = DISPLAY.STATUS1 | 0x04;  //  kg
              break;
              }                     
    } // end of switch
                 }
//
 
  if (SCALE.ATZERO)  
  {    
    if (SCALE.NETMODE)
     {
    if (!SCALE.NEGATIVE)
     {
    DISPLAY.STATUS2 = DISPLAY.STATUS2 | 0X08;  //  center of zero
     } 
     }
    else{
   DISPLAY.STATUS2 = DISPLAY.STATUS2 | 0X08;  //  center of zero
    }
    }

  if (!SCALE.MOTION)  
  {    
    DISPLAY.STATUS2 = DISPLAY.STATUS2 | 0X10;  //  scale motion
  }
    
    
 if (SCALE.NETMODE)
 {
   DISPLAY.STATUS2 = DISPLAY.STATUS2 | 0x02; //  gr
 }
 else
 {
   DISPLAY.STATUS2 = DISPLAY.STATUS2 | 0x01; //  nt
 }

if (SCALE.TAREMODE)
 {
   DISPLAY.STATUS2 = DISPLAY.STATUS2 | 0x04; //  gr
 }

update_status(0);

}  // end of function


// 
//-------------------------------------------------------------------------------------
void display_test(void)
{
  
  char i;
  char j;

for (j=9;j!= 0;j--)
  {
   // RAW_DISPLAY[i]=0xffff0000; // holds raw binary pattern for scanning LED display
 for (i=0;i!=6;i++)
  {
    DISPLAY.DIGITS[i]= j + '0'; // holds raw binary pattern for scanning LED display
  }
  
  timer(STATUS_FAST,START);

  while (1)
  {
   output_display(9-j); // update seven segment display

    if (timer(STATUS_FAST,TIMEOUT))  // break out loop on timeout
    {
      break;
    }

  }  
  }
} // end of function


//--------------------------------------------------------------------------
// Transmits all characters to the scoreboard
//--------------------------------------------------------------------------
void transmit_digits(char decimal)
{
char dec1;
char dec2;
char digit[8];
char i;

for (i=0;i!=6;i++) // loop through the string and build numeric display
{//1
 
 digit[i] =DISPLAY.DIGITS[i];
}

digit[6] = DISPLAY.STATUS2;
digit[7] = DISPLAY.STATUS1;

dec1=0;
dec2=0;  

if(decimal < 6)
{
dec1=decimal;
dec2=0;
}
else if(decimal == 100)
{
dec1=3;
dec2=0;  
}
else if(decimal == 101)
{
dec1=2;
dec2=4;  
}
/*
if(BEAT)
{
//digit[0] = '1';
//digit[1] ='2';
//digit[2] ='3';
//digit[3] ='4';
//digit[4]='5';
//digit[5]='6';
//digit[6] = '0';
//digit[7] =  0X0F;
}

else
{
//digit[0] = '1';
//digit[1] ='2';
//digit[2] ='3';
//digit[3] ='4';
//digit[4]='5';
//digit[5]='6'; 
//digit[6] = 0X0F;
//digit[7] = '0'; 
}

*/
if(dec2==5)
{
cSETBIT7=1; // insert decimal
}
if(dec1==5)
{
cSETBIT7=1; // insert decimal
}
Select_Digit(0xFFEFFFFF); // selsect the digit
send_digit(digit[0]);  // digit 0 MSB
latchbankA(1); // latch the new values to bank A
latchbankA(0);   //
Output_Enable (0);
DelayResolution100us(3);
Output_Enable (1);
Select_Digit(0); // selsect the digit 

if(dec2==4)
{
cSETBIT7=1; // insert decimal
}
if(dec1==4)
{
cSETBIT7=1; // insert decimal
}
Select_Digit(0xFFDFFFFF); // selsect the digit
//DelayResolution100us(1);
send_digit(digit[1]);  // digit 0 MSB
latchbankA(1); // latch the new values to bank A
latchbankA(0);   //
Output_Enable (0);
DelayResolution100us(3);
Output_Enable (1);
Select_Digit(0); // selsect the digit 

if(dec2==3)
{
cSETBIT7=1; // insert decimal
}
if(dec1==3)
{
cSETBIT7=1; // insert decimal
}
Select_Digit(0xFFBFFFFF); // selsect the digit
//DelayResolution100us(1);
send_digit(digit[2]);  // digit 0 MSB
latchbankA(1); // latch the new values to bank A
latchbankA(0);   //
Output_Enable (0);
DelayResolution100us(3);
Output_Enable (1);
Select_Digit(0); // selsect the digit 

if(dec2==2)
{
cSETBIT7=1; // insert decimal
}
if(dec1==2)
{
cSETBIT7=1; // insert decimal
}
Select_Digit(0xFF7FFFFF); // selsect the digit
//DelayResolution100us(1);
send_digit(digit[3]);  // digit 0 MSB
latchbankA(1); // latch the new values to bank A
latchbankA(0);   //
Output_Enable (0);
DelayResolution100us(3);
Output_Enable (1);
Select_Digit(0); // selsect the digit 

if(dec2==1)
{
cSETBIT7=1; // insert decimal
}

if(dec1==1)
{
cSETBIT7=1; // insert decimal
}
Select_Digit(0xFEFFFFFF); // selsect the digit
//DelayResolution100us(1);
send_digit(digit[4]);  // digit 0 MSB
latchbankA(1); // latch the new values to bank A
latchbankA(0);   //
Output_Enable (0);
DelayResolution100us(3);
Output_Enable (1);
Select_Digit(0); // selsect the digit 


if(dec1==99)
{
//cSETBIT7=1; // insert decimal
}
Select_Digit(0xFDFFFFFF); // selsect the digit
//DelayResolution100us(1);
send_digit(digit[5]);  // digit 0 MSB
latchbankA(1); // latch the new values to bank A
latchbankA(0);   //
Output_Enable (0);
DelayResolution100us(3);
Output_Enable (1);
Select_Digit(0); // selsect the digit 

Select_Digit(0xFBFFFFFF); // selsect the digit
//DelayResolution100us(1);
spi_write8bits(digit[6]);
latchbankA(1); // latch the new values to bank A
latchbankA(0);   //
Output_Enable (0);
DelayResolution100us(3);
Output_Enable (1);
Select_Digit(0); // selsect the digit 

Select_Digit(0xFFF7FFFF); // selsect the digit
//DelayResolution100us(1);
spi_write8bits(digit[7]);
latchbankA(1); // latch the new values to bank A
latchbankA(0);   //
Output_Enable (0);
DelayResolution100us(3);
Output_Enable (1);
Select_Digit(0); // selsect the digit 


/*
if(dec1==5)
{
cSETBIT7=1; // insert decimal
}
send_digit(digit[0]);  // digit 0 MSB
latchbankA(0);   //
latchbankA(1); // latch the new values to bank A
Digit_Select(1); // selsect the digit

Output_Enable (0);
DelayResolution100us(2);
Output_Enable (1);
Digit_Select(0); // selsect the digit     

if(dec1==4)
{
cSETBIT7=1; // insert decimal
}
send_digit(digit[1]);  // digit 0 MSB
latchbankA(0);   //
latchbankA(1); // latch the new values to bank A
Digit_Select(2); // selsect the digit

Output_Enable (0);
DelayResolution100us(2);
Output_Enable (1);
Digit_Select(0); // selsect the digit     

if(dec1==3)
{
cSETBIT7=1; // insert decimal
}
send_digit(digit[2]);  // digit 0 MSB
latchbankA(0);   //
latchbankA(1); // latch the new values to bank A
Digit_Select(3); // selsect the digit

Output_Enable (0);
DelayResolution100us(2);
Output_Enable (1);
Digit_Select(0); // selsect the digit     

if(dec1==2)
{
cSETBIT7=1; // insert decimal
}
send_digit(digit[3]);  // digit 0 MSB
latchbankA(0);   //
latchbankA(1); // latch the new values to bank A
Digit_Select(4); // selsect the digit

Output_Enable (0);
DelayResolution100us(2);
Output_Enable (1);
Digit_Select(0); // selsect the digit


if(dec1==1)
{
cSETBIT7=1; // insert decimal
}
send_digit(digit[4]);  // digit 0 MSB
latchbankA(0);   //
latchbankA(1); // latch the new values to bank A
Digit_Select(5); // selsect the digit

Output_Enable (0);
DelayResolution100us(2);
Output_Enable (1);
Digit_Select(0); // selsect the digit

send_digit(digit[5]);  // digit 0 MSB
latchbankA(0);   //
latchbankA(1); // latch the new values to bank A
Digit_Select(6); // selsect the digit

Output_Enable (0);
DelayResolution100us(2);
Output_Enable (1);
Digit_Select(0); // selsect the digit


send_digit(digit[6]);  // digit 0 MSB
latchbankA(0);   //
latchbankA(1); // latch the new values to bank A
Digit_Select(7); // selsect the digit

Output_Enable (0);
DelayResolution100us(2);
Output_Enable (1);
Digit_Select(0); // selsect the digit
*/


/*
//cSETBIT7=0;
if(dec1==5)
{
cSETBIT7=1; // insert decimal
}
latchbankA(1); // latch the new values to bank A
send_digit(digit[0]);  // digit 1
latchbankA(0);
Digit_Select(1);  // select the digit
Output_Enable (0);
DelayResolution100us(2);  
Output_Enable (1);
Digit_Select(0); // selsect the digit       

// Digit 1
//cSETBIT7=0;
if(dec1==4)
{
cSETBIT7=1; // insert decimal
}

latchbankA(1); // latch the new values to bank A
send_digit(digit[1]);  // digit 1
latchbankA(0);
Digit_Select(2);  // select the digit
Output_Enable (0);
DelayResolution100us(2);  
Output_Enable (1);
Digit_Select(0); // selsect the digit 
// digit 2
//cSETBIT7=0;
if(dec1==3)
{
cSETBIT7=1; // insert decimal
}

latchbankA(1); // latch the new values to bank A
send_digit(digit[2]);  // digit 2
latchbankA(0);
Digit_Select(3);   // select the digit
Output_Enable (0);  // Enable the driver output
DelayResolution100us(2);          
Output_Enable (1);
Digit_Select(0); // selsect the digit  
// digit 3 
//cSETBIT7=0;
if(dec1==2)
{
cSETBIT7=1; // insert decimal
}
 
latchbankA(1); // latch the new values to bank A
send_digit(digit[3]);  // digit 3
latchbankA(0);    
Digit_Select(4);    // select the digit
Output_Enable (0);  // Enable the driver output
DelayResolution100us(2);          
Output_Enable (1);
Digit_Select(0); // selsect the digit
// digit 4
//cSETBIT7=0;

if(dec1==1)
{
cSETBIT7=1; // insert decimal
}
    
latchbankA(1); // latch the new values to bank A
send_digit(digit[4]);  // digit 4
latchbankA(0);
Digit_Select(5);
Output_Enable (0);  // Enable the driver output/
DelayResolution100us(2);         
Output_Enable (1);
Digit_Select(0); // selsect the digit    

if(dec1==0)
{
//cSETBIT7=1; // insert decimal
}
// LSB digit 5 
latchbankA(1); // latch the new values to bank A
send_digit(digit[5]);  // digit 5 LSB
latchbankA(0);
Digit_Select(6);
Output_Enable (0);  // Enable the driver output
DelayResolution100us(2);         
Output_Enable (1);
Digit_Select(0); // selsect the digit 

// LSB digit 5 
latchbankA(1); // latch the new values to bank A
spi_write8bits(digit[6]);
latchbankA(0);
Digit_Select(7);
Output_Enable (0);  // Enable the driver output
DelayResolution100us(2);   
Output_Enable (1);
Digit_Select(0); // selsect the digit


// LSB digit 5 
//cSETBIT7=0;
latchbankA(1); // latch the new values to bank A
spi_write8bits(digit[7]);
latchbankA(0);
Digit_Select(8);
Output_Enable (0);  // Enable the driver output
DelayResolution100us(2);         
Output_Enable (1);
Digit_Select(0); // selsect the digit
// end of digit scanning from left to right
// LSB digit 5 
// LSB digit 5 

latchbankA(1); // latch the new values to bank A
spi_write8bits(0);
latchbankA(0);
*/
//
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

 


