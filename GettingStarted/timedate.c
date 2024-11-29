#include "includes.h"

//----------------------------------------------------------------------
// init real time clock
void init_clock(void)
{
// CCR = 0x11; 
RTCCCR = 0X11; 
return;  
}  // end of function

//--------------------------------------------------------------------
//  prepare the time string for printing
//--------------------------------------------------------------------
void format_time(char string[])
{
  
long h,m,s;
char pm=0;

h=RTCHOUR;  // RTC registers
m=RTCMIN;
s=RTCSEC;

//h=HOUR;  // RTC registers
//m=MIN;
//s=SEC;

if (CAL.CLOCK_12_24==1) // check if are formatting for 12 hour mode
{// 11
  
    // time format am/pm
              //---------------------------------------------------------------------------
              
              
                  if ( (h>12) || (h==12) )  // PM
                    {//1
                         h=h-12; // convert to 12 hour from 24hr mode
                         pm=1; // set PM flag
                           if (h<0)
                            {//2
                              h=h*-1; 
                            }//2
                         
                         
                    }//1
 
                
                           if (h==0)
                           {
                            h=12; 
                           }    
                  
        
         // format string for AM or PM         
         if (pm)
         {//1                  
                   if (SCALE.TIME_NOSECONDS==1)  
                   {//2
                     sprintf(string,"%02ld:%02ldPM ",h,m);  // time without seconds
                   }//2
                   else
                   {//3     
                     sprintf(string,"%02ld:%02ld:%02ldPM ",h,m,s); // time with seconds           
                   }//3  
         }//1
         else
         {//2       
            if (SCALE.TIME_NOSECONDS==1)  
                   {//22
                       sprintf(string,"%02ld:%02ldAM ",h,m);  // time without seconds
                   }//22
                   else
                   {//33
                       sprintf(string,"%02ld:%02ld:%02ldAM ",h,m,s); //time with seconds                                        
                   }//33
            
                    
            
         }//2
                  
                  
}//22

else
 { // 333 format weight for 24hour mode
   
      if (SCALE.TIME_NOSECONDS==1)  
                   {//44
                      sprintf(string,"%02ld:%02ld ",h,m); // time without seconds
                   }//44 
                   else
                   {//55
                     sprintf(string,"%02ld:%02ld:%02ld ",h,m,s); // time with seconds
                   }//55
     
 } // 333
 
  
return;  
}  // end of function
//---------------------------------------------------------------------



//------------------------------------------------------------------
// format date for printing
//------------------------------------------------------------------
void format_date(char string[])
{

int d,m,y;

d=RTCDOM;  //  read RTC registers
m=RTCMONTH;
y=RTCYEAR;
 


if (CAL.CLOCK_US_INT == 1)
{
  sprintf(string,"%02d/%02d/%02d ",m,d,y);  // format date US   mm/dd/yy;
}
else
{
  
   sprintf(string,"%02d/%02d/%02d ",d,m,y);  // format date Int   dd/mm/yy;
  
 
}
  

 
} // end of function

//------------------------------------------------------------------


//---------------------------------------------------------------------
void change_time(void)
{

int mode;
long h,m;
char timestring[7];



// determine format we are editing time
if (CAL.CLOCK_12_24==0)    // 0=24hour 1=12hour
{
 mode=_24HOUR; 
}
else
{
 mode=_12HOUR;
}



// read time from processor
// low level processor specific registers
h=RTCHOUR;    // get hour from procesor RTC register
m= RTCMIN;     // minutes from RTC register



      if (mode== _24HOUR)  // edit in 24 hour clock mode
      {//1
          //24 hour clock
          //-----------------------------------------------------------------     
          sprintf(timestring," %02ld%02ld ",h,m);  // build a timestring for 24hour time mode
          time_date_edit(timestring, mode);  // edit the clock
          
          h=extract_digits(&timestring[1]);
          m=extract_digits(&timestring[3]);       
      
      }//1
      
      else
        
      { //2
     
              // time format am/pm
              //---------------------------------------------------------------------------
              
              
                  if ( (h>12) || (h==12) )  // PM
                    {//1
                     h=h-12; // convert to 12 hour from 24hr mode
                
                       if (h<0)
                        {//2
                          h=h*-1; 
                        }//2
                       
                       
                       if (h==0)
                       {
                        h=12; 
                       }
                       
                       
                      sprintf(timestring," %02ld%02ldP",h,m);  // build a timestring for 12hour PM
                      }//1
                      else
                      {//3
                        
                       if (h==0)
                       {
                         h=12;
                       }
                       
                       sprintf(timestring," %02ld%02ldA",h,m);  // build a timestring for 12hour AM 
                      }//3
                  
              
              mode= _12HOUR;    
              time_date_edit(timestring, mode);  // edit the clock
               
              h=extract_digits(&timestring[1]);
              m=extract_digits(&timestring[3]);
              
              
              if (timestring[5]=='P') // check if we are in PM mode
              {
                h=h+12; 
              }

      }//22
      
     
      
// write time back to processor      
// low level processor specific registers
RTCHOUR=h;    // update procesor RTC register
RTCMIN=m;     // update RTC register

 
  
} // end of function





//---------------------------------------------------------------------
void change_date(void)
{
long d,m,y;
int mode;
char timestring[7]; 
 

// determine format we are editing date
if (CAL.CLOCK_US_INT==0) //  0=international  1=US format
{
 mode=_INTDATE; 
}
else
{
 mode=_USDATE;
}



// read registers from processor
d=RTCDOM;  //  read RTC registers
m=RTCMONTH;
y=RTCYEAR;


if (mode==_INTDATE)
{//1
      //----------------------------------------------------------------------------
      //format date international dd/mm/yy
      
      // depending if we are formating for US or International date format
      // setup editor for correct order
      
     
      sprintf(timestring,"%02ld%02ld%02ld",d,m,y);  // build a timestring for dd/mm/yy
      time_date_edit(timestring, mode);  // edit the clock
      
      d=extract_digits(&timestring[0]);
      m=extract_digits(&timestring[2]);
      y=extract_digits(&timestring[4]);


}//1
else // US format mm/dd/yy
{//2

               
        // depending if we are formating for US or International date format
        // setup editor for correct order
        
    
        sprintf(timestring,"%02ld%02ld%02ld",m,d,y);  // build a timestring for dd/mm/yy
        time_date_edit(timestring, mode);  // edit the clock
        
        m=extract_digits(&timestring[0]);
        d=extract_digits(&timestring[2]);
        y=extract_digits(&timestring[4]);
        
}//2      




// write registers to processor
 RTCDOM=d;   
 RTCMONTH =m; 
 RTCYEAR=y;  // update year

return;  
} // end of function
















  
  








//----------------------------------------------------------------------
// change clock time
// when called the time will be first displayed for editing
// folowed by the date 
//----------------------------------------------------------------------
void change_clock(void)
{
  
   if ( CAL.CLOCKMODE ==0)
        {
          return;
        }
   
display_message( " CLOC ",2);  // flash clock
display_message( " EDIT ",2);  // flash clock
change_time();  // edit time
change_date();  // edit date
return;  
}




// --------------------------------------------------------------------------------------------------------
// time/date entry
//
//
//---------------------------------------------------------------------------------------------------------


int time_date_edit(char digit[], char mode)
  {

char key;       // keypress value
char digitpos;  // which digit position are we pointing to 0-5
char i;
char blink;      // flag used to for blinking a character
char left_boundary;
char right_boundary;
//char digit[]="091467";
char digitmax[6];
char digitmin[6];

                                    





switch (mode)   // set up min max of clock/date display entries for editiing
  {

              case _24HOUR:  {     //2359   0000 to 2399
                              strcpy(digitmax," 2959 ");  // max and min clock entry for 24hour mode
                              strcpy(digitmin," 0000 ");   
                              left_boundary=1;          //setup bounderies for editor
                              right_boundary=4;
                              digitpos=1;
                              break;
                              }
            
                              
              case _12HOUR:   {   //1259A or 1259P  Am or Pm
                              strcpy(digitmax," 1959P");  // max and min entry for 12 hour mode
                              strcpy(digitmin," 0000A");  
                              left_boundary=1;          //setup bounderies for editor
                              right_boundary=5;
                              digitpos=1;
                              break;
                             }
                      
                             
              case  _USDATE:   {  // MMDDYY
                              strcpy(digitmax,"193999");  // max and min entry for international date format  mmddyy
                              strcpy(digitmin,"000000");
                              left_boundary=0;            //setup boudarines for editor
                              right_boundary=5;
                              digitpos=1;
                              break;
                             }                 
                           
                             
              case _INTDATE:   {  // DDMMYY
                              strcpy(digitmax,"391999");  // max and min entry for international date format  ddmmyy
                              strcpy(digitmin,"000000");  
                              left_boundary=0;
                              right_boundary=5;
                              digitpos=1;
                             }
                        
 
  } // end of switch;               
                 




while(1) // loop until enter key is pressed
{

 key = scan_keypad(_EXTENDED);  // scan the keypad for keypresses

  
      switch (key)
      {//3

       case _UP:
                { // incrememnt digit
                  
         
                
                          if (digit[digitpos]!=digitmax[digitpos]) // check we are not going past max
                          {
                            digit[digitpos]++;
                            
                            if (digit[digitpos] > 'A') // special condition for 12 hour time A= AM P=PM
                            {
                              digit[digitpos]='P';
                            }  
                            
                          }
      
                        timer(PARAMEDIT_B,START); // restart the blink timer
                        blink=0; // clear blinking while scrolling up/down
      
                      break;
                }


       case _DOWN:
               
                {   // decrament digit
                     if (digit[digitpos]!=digitmin[digitpos]) // check we are not going past min
                     {
                       digit[digitpos]--;
                       if (digit[digitpos] == 'O')
                            {
                              digit[digitpos]='A';
                            }
                       
                     }
    
                      timer(PARAMEDIT_B,START); // restart the blink timer, blink the digit
                      blink=0; // clear blinking while scrolling up/down
    
                     break;
                 }



        case _LEFT:
                 { // shift the digit position to the left
                  if (digitpos!=left_boundary)
                  {
                    digitpos--;
                  }

                  timer(PARAMEDIT_B,START); // restart the blink timer, blink the digit
                  blink=1; // blink character right away to show that you have moved left or right

                 break;
                 }



        case _RIGHT:
                 {  // shift the digit position to the right

                  if(digitpos!=5)
                  {
                    digitpos++;
                  }

                  timer(PARAMEDIT_B,START); // restart the blink timer, blink the digit
                  blink=1;  // blink character right away to show that you have moved left or right

                 break;
                 }


          case _CLEAR:
                 {  // abort editing and return -1 to indicate we have aborted

                   
                   return(-1);

                 }

 

      case _ENTER: {  // ENTER key is pressed, process calibration command that we are currrently pointing to in the menu

                    return(1);
                   }

      } // end of switch





// build the display to display the value being edited
// copy the digit array over to the display array and format


 for (i=0;i!=6;i++)
 {
   DISPLAY.DIGITS[i]=digit[i];
   
 }





  // blink the digit position we are pointing to in the editor
  if (timer(PARAMEDIT_B,TIMEOUT))
   {//1
    timer(PARAMEDIT_B,START);

     if (blink==1)
     {
       
       blink=0;
     } 
      else
      {
       blink=1;

      }
    }//1

  
  if (blink==1)
     {
       DISPLAY.DIGITS[digitpos]=' ';
       
     } 
   
     
  if (left_boundary==0)
  {   
       output_display(101); // update seven segment display
  }
  else
  {
       output_display(100); // update seven segment display
  }



} //3 end of while





}// end of function parameter entry




//----------------------------------------------------------------------
// extract a 2 digit value from string
// &p is pointer to string

long extract_digits(char *p)
{
char temp[6];
char i;


for (i=0;i!=2;i++)
{
 temp[i]=*p++;  // copy string over
}
  
temp[i]=0; // null terminate

return(atol(temp)); // return long extracted from string

} // end of function






