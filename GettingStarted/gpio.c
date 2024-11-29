
#include "includes.h"

char strcmd[15];
char BEAT;
int  DIGIT_SELECT[]={//ascii lookup 
                             0,
                             0,                                                       
                             0x00200000, 
                             0x00400000, 
                             0x00800000, 
                             0x01000000, 
                             0x02000000,
                             0x04000000,             
                             0x00100000, 
                             0x00080000, 
                             0,
                             0,
                            } ;
/*************************************************************************
 * Function Name: DelayResolution100us
 * Parameters: Int32U Dly
 * Return: void
 *
 * Description: Delay Dly * [100us]
 *		
 *************************************************************************/
void DelayResolution100us(Int32U Dly)
{
  
  
  for(; Dly; Dly--)
  {
    for(volatile Int32U j = DLY_100US; j; j--)
    {
    }
  }
}


void init_relays(void)
{
//
  FIO3SET = 0x02000000; // buzer off
  FIO0SET = 0x00000030; // status leds off
  FIO1CLR = 0x00030000; // relays off  
//
}

//----------------------------------------------------------
// Controls the reset pin to the scorboard digit panel
//----------------------------------------------------------
void relays_function(long set1, long set2)
{
//long gradweight = SCALE.DIS_GROSSWEIGHT;// / SCALE.DIS_GRAD; 
long gradweight = SCALE.GRADWEIGHT;  
long setpoint1 = set1/CAL.GRADd;
long setpoint2 = set2/CAL.GRADd;

float bar;
long i;
float fweight;
float under;
float over;
float accept;
float range;

SCALE.SETPOINT = 0;   

if( CAL.RELAY_EN == 0)
     {     
     return;
     }

/*
fweight = SCALE.DIS_WEIGHT;

if (SCALE.NEGATIVE)
 {
   fweight=fweight*-1;
 }

 
under= (float)set1;  // get the under weight
over= (float)set2;    // get the over weight
//determine below how the bargraph functions

   // check is we have an accept contition 
   
          

// check if over under bars are solid bars            

     
         if ( (fweight >= under ) )  // are we in under range
              {   
               FIO1SET = 0x00010000; // disable output 
                          
               }
                else
                {  
                  FIO1CLR = 0x00010000; // disable output  
                }
             


          if ( (fweight >= over ) )  // are we in over range
              {
                FIO1SET = 0x00020000; // disable output               
              
              }
             else
             {                
                FIO1CLR = 0x00020000; // disable output
              }
         

       if ( (fweight > under ) && (fweight < over )  )  // are we in accept range
              {
                 SCALE.SETPOINT = 1; 
                 
                    if( CAL.BUZZ_EN == 2)
                   {
                   FIO3CLR = 0x02000000; // disable output 
                 //  FIO3SET = 0x02000000; // disable output
                   }
             
              }
           else
            if ( (fweight == under ) || (fweight == over )  )  // are we in accept range
              {
                SCALE.SETPOINT = 1; 
                  if( CAL.BUZZ_EN == 2)
                   {
                   FIO3CLR = 0x02000000; // disable output 
                 //  FIO3SET = 0x02000000; // disable output
                   }
            
              }
          else
           {
                SCALE.SETPOINT = 0; 
                 
                    if( CAL.BUZZ_EN == 2)
                   {
                   
                   FIO3SET = 0x02000000; // disable output
                   }
              
           }
   



return;
*/

// 
// 
// 

if(SCALE.NEGATIVE ) // timer value for delaying peakdetect 
{// positive
 FIO1CLR = 0x00010000; // disable output  
 FIO1CLR = 0x00020000; // disable output 
 if( CAL.BUZZ_EN == 2)
                   {
                 //  FIO3CLR = 0x02000000; // disable output 
                   FIO3SET = 0x02000000; // disable output
                   } 
 return;  
}


if (  gradweight >= setpoint1)  // check if scale is over capacity
  {
if (  gradweight <= setpoint2 )  // check if scale is over capacity
  {
  SCALE.SETPOINT = 1;  
  }
  }


 if (gradweight > setpoint1)  // check if scale is over capacity
  {
    FIO1SET = 0x00010000; // disable output
  }
  else
  {  
    FIO1CLR = 0x00010000; // disable output   
  }

if (gradweight > setpoint2)  // check if scale is over capacity
{
   FIO1SET = 0x00020000; // disable output      
  }
  else
  {
    FIO1CLR = 0x00020000; // disable output
    
    
  }

if ( SCALE.SETPOINT == 1 )  // check if scale is over capacity
  {
    if( CAL.BUZZ_EN == 2)
                   {
                   FIO3CLR = 0x02000000; // disable output 
                 //  FIO3SET = 0x02000000; // disable output
                   }
  }
  else
  {
     if( CAL.BUZZ_EN == 2)
                   {
                 //  FIO3CLR = 0x02000000; // disable output 
                   FIO3SET = 0x02000000; // disable output
                   }
  }



} // end of funtion latchbankA
  // 
//----------------------------------------------------------
// Controls the reset pin to the scorboard digit panel
//----------------------------------------------------------
void recevie_comport(char data, char port)
{
  
} // end of funtion latchbankA
// 
//----------------------------------------------------------
// Controls the reset pin to the scorboard digit panel
//----------------------------------------------------------
void reset_driver(char ctrl)
{
   if (ctrl) FIO0SET = 0x60000000; // set latch A 
        else FIO0CLR = 0x60000000; // p0.29 and O0.30  
} // end of funtion latchbankA

//----------------------------------------------------------
// Controls the reset pin to the scorboard digit panel
//----------------------------------------------------------
void reset_digit(char ctrl)
{
  if (ctrl) FIO3SET = 0x04000000; // set latch A 
       else FIO3CLR = 0x04000000; // p3.26  
} // end of funtion latchbankA
// 
//----------------------------------------------------------
// Allegro 6277 latch control for bank A digits
// Controls the latch pin to the scorboard digit panel
//----------------------------------------------------------
void latchbankA(char ctrl)
{
  if (ctrl) FIO0SET = 0x04000000; // set latch A 
       else FIO0CLR = 0x04000000; // p0.26  
} // end of funtion latchbankA
// 
//----------------------------------------------------------
// Allegro 6277 latch control for bank B digits
// Controls the latch pin to the scorboard digit panel
//----------------------------------------------------------
void latchbankB(char ctrl)
{
 if (ctrl) FIO0SET = 0x02000000; //set latch B  
      else FIO0CLR = 0x02000000;  //p0.25
} // end of funtion latchbankB 

//----------------------------------------------------------
// Allegro 6277 latch control for bank C digits
// Controls the latch pin to the scorboard digit panel
//----------------------------------------------------------
void latchbankC(char ctrl)
{
 if (ctrl) FIO0SET= 0x01000000; //set latch C  
      else FIO0CLR = 0x01000000;  //p0.24
} // end of funtion latchbankB 


char read_legal(void)
{
  // p3.26       PWM     OUTPUT  AUXILIARY   OEnable  
  if ((FIO2PIN & 0x00000800)==0) // disable output
  {
    return(1);
  }
   
        return(0); 
       
     
} // end of funtion latchbankA



void Output_Enable (char ctrl)
{
  // p3.26       PWM     OUTPUT  AUXILIARY   OEnable  
  if (ctrl) FIO3SET = 0x04000000; // disable output
       else FIO3CLR = 0x04000000; // Enable out put p3.26  
     
} // end of funtion latchbankA
//--------------------------------------------------------
// selects the digit
// for six digit MSB 3 and LSB 2 dig 0,1,2,3,4,5
//--------------------------------------------------------
void Digit_Select(char digit)
{      
 //int d=1;
  /*
p1.19         GPIO  INPUT  SW12  dig6 OUTPUT
p1.20         GPIO  INPUT  SW12  dig7 OUTPUT
                        
                         p1.21         GPIO  INPUT  SW12  dig0 OUTPUT
                         p1.22         GPIO  INPUT  SW13  dig1 OUTPUT
                         p1.23         GPIO  INPUT  SW14  dig2 OUTPUT
                         p1.24         GPIO  INPUT  SW15  dig3 OUTPUT
                         p1.25         GPIO  INPUT  SW16  dig4 OUTPUT
                         p1.26         GPIO  INPUT  SW17  dig5 OUTPUT
                         0x00000010, // digit0    p0.4 1
                         0x00000020, // digit1    p0.5 2
                         0x00000040, // digit2    p0.6 4
                         0x00000080, // digit3    p0.7 8
                         0x00000100, // digit4    p0.8 16
                         0x00000200, // digit5    p0.9 32                        
*/
  
 
  if(digit !=0)FIO1CLR  = DIGIT_SELECT[digit]; 
      else FIO1SET  = 0x07F80000;       
     
// 
      
} // lathes the row number


//--------------------------------------------------------
// selects the digit
// for six digit MSB 3 and LSB 2 dig 0,1,2,3,4,5
//--------------------------------------------------------
void Select_Digit(long digit)
{      
 if(digit !=0)FIO1PIN &= digit;   
         else FIO1PIN |= 0x07F80000;            
  /*
p1.19         GPIO  INPUT  SW12  dig6 OUTPUT
p1.20         GPIO  INPUT  SW12  dig7 OUTPUT
                        
                         p1.21         GPIO  INPUT  SW12  dig0 OUTPUT
                         p1.22         GPIO  INPUT  SW13  dig1 OUTPUT
                         p1.23         GPIO  INPUT  SW14  dig2 OUTPUT
                         p1.24         GPIO  INPUT  SW15  dig3 OUTPUT
                         p1.25         GPIO  INPUT  SW16  dig4 OUTPUT
                         p1.26         GPIO  INPUT  SW17  dig5 OUTPUT
                         0x00000010, // digit0    p0.4 1
                         0x00000020, // digit1    p0.5 2
                         0x00000040, // digit2    p0.6 4
                         0x00000080, // digit3    p0.7 8
                         0x00000100, // digit4    p0.8 16
                         0x00000200, // digit5    p0.9 32                        

  


      
 if(digit)
   {
   FIO1PIN &= digit;   
   }
    else
    {
     FIO1PIN |= 0x07F80000;       
    }
*/
     
//       
//       
      
} // lathes the row number

//----------------------------------------------------------------------------------------
// toggle status light
//----------------------------------------------------------------------------------------
void status_light(char mode)
{

if (timer(mode,TIMEOUT)) // check the blink timer
    {    
    if(BEAT==1)
    {
        FIO0CLR = 0x00000010;        
        BEAT = 0;
       
       }
     else
       {
       // FIO0SET = 0x00000020; 
        FIO0SET = 0x00000010;
        BEAT = 1; 
  
       }    
    timer(mode,START);
    } // 

}

// 