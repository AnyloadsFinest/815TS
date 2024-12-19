#include "includes.h" 



//------------------------------------------------------------------------------
// build a string for output on COM port
//------------------------------------------------------------------------------

void stringformat(int port,int format)
{
  
long weight;           // displayed weight
char negative;         // polarity
char wstring[20];      // weight string
char tstring[20];      // weight string for tare
char outstring[30];    // final string for transmission 
char addstring[5];
char units[3];
char unit=' ';   // used for condec string
char mode[3];
char mode2=' ';        
char status=' ';
char status2[3];
char polarity;
char i;

char toledoA=0x20;      // toledo status A                                   
char toledoB=0x20;      // toledo status B
char toledoC=0x20;      // toledo status C
long deadzero=0;          // used for deadzero on TOLEDO string

unsigned int chk;      // checksum for TOLEDO

long dweight;   // displayed weight scratchpad 
long tweight;   // tare weight scratchpad

char motion=' ';   // used for motion flag for SMA format
char sl;    // used for stoplight 
char cz;    // used for center of zero flag
char address;

static unsigned char TIMERCOUNT[4];  // keep tracks for delay counts for inserting delays inbetween streams
int stimer;

// check if the COM port is IDLE ready for a new string to transmit
if (COM[port].IDLE==FALSE)  // if the com port is still busy tranmitting a string, wait for it to be idle
{ 
 return;   
}

if (CAL.STREAMDELAY!=0)  // check if we are not using stream delays 
{//1

      // depending on which port we are using,we have to select between stream timers for delays
      if (port==_PORT0)
      {
        stimer=STREAM_DELAY0;      
        
      }
      else if (port==_PORT1)
      {
        
        stimer=STREAM_DELAY1;
        
      }
    else if (port==_PORT2)
      {
        //
        stimer=STREAM_DELAY2;
        // 
      }
  

      if (timer(stimer,TIMEOUT))      // call the timer to keep track of delay counts used for inserting delays in between string tranmissions
      {
        
        timer(stimer,START);  // restart timer (0.25sec)
        TIMERCOUNT[port]++;   //increment timercount  (counts the number of 0.25sec tics)
      }
      
      
      if (TIMERCOUNT[port]>4)  // safety check, cannot got past 4
      {
        TIMERCOUNT[port]=0;  // reset to zero
      }
      
      
      if (CAL.STREAMDELAY==TIMERCOUNT[port])  // check if we have reached delay duration
      {
       TIMERCOUNT[port]=0;     
      }      
      else
      {
      return;  // have not reached adequate delay in between streams, return
      }

}//1

    
   //
   weight=SCALE.DIS_WEIGHT;   // get the displayed weight 
   format_string(weight, wstring,SCALE.DIS_DEC);   // format the weight string and insert decimals if any
   negative=SCALE.NEGATIVE;  
   address = CAL.ADDRESS + 48;
   sprintf(addstring,"%c%c%c",0x01,address,0x03);   
//special deadzero for NTEP conformance of displaying deadzeroes when the scale is at zero on auxialry equipment
// deadzeroes must be transmitted in the string when scale is at zero
if (weight==0)  // if the scale is zero check if we have to insert deadzeros
{  //1
 
      if (SCALE.DIS_DEADZERO==1)  // check for single deadzero
      {
        wstring[1]='0';
        wstring[2]=0;  // null terminate
      }
      else
      if (SCALE.DIS_DEADZERO==2)  // check for double deadzero
      {
        wstring[1]='0';
        wstring[2]='0';
        wstring[3]=0; // null terminate
      } 
   
}  //1
   

   // format units string for output ----------------------------------
   
   switch (SCALE.DIS_UNITS)
   {
     
   case _KG: {
             strcpy(units,"KG");
             unit='K';  // used in Condec string
             toledoB=toledoB | 0x10;  //fortmat Toledo bit mask for toledo string
             break;     
            }
           
            
   case _LB: {
             
             strcpy(units,"LB");
             unit='L';  // used in Condec string
             break;
            }         
     
   
   case  _G: {
             strcpy(units," G"); 
             toledoC=toledoC | 0x01;  //fortmat Toledo bit mask for toledo string
             unit='G';  // used in Condec string
             break;
             }
            
             
   case  _OZ: {
             strcpy(units,"OZ");
             unit='O';  // used in Condec string
             break;
             }     
   
 case  _TON: {
             strcpy(units," T");
             unit='T';  // used in Condec string
             break;
             }  
             
 case  _STON: {
             strcpy(units,"TN");
             unit='T';  // used in Condec string
             break;
             }  
             
             case  _LTON: {
             strcpy(units,"LT");
             unit='T';  // used in Condec string
             break;
             }                               
   }  // end of switch
 
   
   // format GROSS/NET mode for output ------------------------------
   
   if (SCALE.NETMODE)
   {
    strcpy(mode,"NT");
    mode2='N';
    toledoB=toledoB | 0x01;  //fortmat Toledo bit mask for toledo string
   }
   else
   {
     strcpy(mode,"GR");
     mode2='G';
   }
   

   
  // format  status character ----------------------------------------------------------------
  status=' ';  // initialize status character 
  polarity=' '; // initialize polarity 

   if (negative)
  {
   toledoB=toledoB | 0x02;  //fortmat Toledo bit mask for toledo string
   status='-'; 
   polarity='-';
  }
  
    if (SCALE.MOTION)
  {
    status='M'; 
    toledoB=toledoB | 0x08;  //fortmat Toledo bit mask for toledo string
    motion='M';  // set motion flag for SMA string
  }
  
  
   if (SCALE.OVER)
  {
    status='O'; 
    strcpy(wstring,"EEEEEE");
    toledoB=toledoB | 0x04;  //fortmat Toledo bit mask for toledo string
  }
  
  
 // format TOLEDO special status A byte
  if (SCALE.DIS_DEADZERO==1)
  {
     toledoA=toledoA | 0x01;  //fortmat Toledo bit mask for toledo string
  }    
   else
  {
   toledoA=0x02+SCALE.DIS_DEC;
  }
  
 if(SYSTEM.DISPLAY_MODE == _PIECE)
             {
             weight= SCALE.DIS_COUNT;   // get the displayed weight 
             format_string(weight, wstring,0);   // format the weight string and insert decimals if any
             strcpy(units,"PC");
           //  strcpy(mode,"CNT");
           //  sprintf(outstring,"%c%7.7s %s%s %c%c%c",0x02,wstring,units,mode,status,0x0d,0x0a);
           //  sprintf(outstring,"%c%7.7s %c%c%c",0x02,SCALE.DIS_COUNT);
             }

  switch(format)
  {  
   case 0:{               
            
                // DF1500 string ------------------------------------------------------------------------------- 
             if(port ==_PORT2)
             {
            sprintf(outstring,"%c%c%c %c%c%7.7s %s %s %c%c%c",0x01,address,0x03,0x02,polarity,wstring,units,mode,status,0x0d,0x0a);
             }
             else
             {
              sprintf(outstring,"%c%c%7.7s %s %s %c%c%c",0x02,polarity,wstring,units,mode,status,0x0d,0x0a);   
             }
           break;
          }
  
 
 
  case 1:    // Toledo  ----------------------------------------------------------------------------
  case 2:    // Toledo with checksum ---------------------------------------------------------------
  case 3:    // Toledo with checksum and leading zeroes --------------------------------------------
  case 11:   // Fairbanks R2500 without checksum ---------------------------------------------------  
    { 
    //        
    deadzero=SCALE.DIS_DEADZERO*10;
    
   if (deadzero==0)
              {
                deadzero=1;
              }
              else
                if (deadzero==1)
                {
                   deadzero=10;
                }
               else
                if (deadzero==2)
                {
                   deadzero=100;
                }
    
    dweight = SCALE.DIS_WEIGHT;  // deadzero multiplier already in place
    tweight = SCALE.DIS_TAREWEIGHT * deadzero;   // need to add deadzero multiplier
    
   toledoA=toledoA | 0x38; // final formating of Toledo Status A 
   
   if (format==3)  // leading zeroes
   { // 
     sprintf(outstring,"%c%c%c%c%06ld%06ld%c",0x02,toledoA,toledoB,toledoC, dweight , tweight,13);
   }
   else   // no leading zeros
   {
     sprintf(outstring,"%c%c%c%c%6ld%6ld%c",0x02,toledoA,toledoB,toledoC, dweight , tweight,13);
   }


   if (format==2) // check if we have to add a checksum
      {
                  chk=0;
                   for (i=0;i!=17;i++)  // calculate checksum
                   {
                    chk=chk+outstring[i];
                   }
              
                 chk=~chk;  // invert bits ones compliment
                 chk=chk+1; // 2s compliment
                                
                 outstring[17]=chk; // add checksum
                 outstring[18]=0; // null terminate
      }              
   
     break;
    }  
      
case 4:     // trancell -----------------------------------------------------------------------------------
case 5:  {  // ricelake/condec ----------------------------------------------------------------------------
           
           //
            sprintf(outstring,"%c%c%7.7s%c%c%c%c%c",0x02,polarity,wstring,unit,mode2,status,0x0d,0x0a);

         break;
         }
  

case 6:  { // Cardinal SB400 ------------------------------------------------------------------------------
  
           strcpy(status2,"  ");  // initialize to spaces
                      
           if (status=='O')
             {
               strcpy(status2,"OC");
             }  
           
           if (status=='M')
             {
               strcpy(status2,"MO");
             }  
           
            if (polarity=='-')
             {
               strcpy(status2,"BZ");
             }   
  
  
           if (SCALE.DIS_DEC==0)  // format with no decimals
           {
             sprintf(outstring,"%c%6s %s %c %s %c", polarity, wstring,units,mode2,status2,13);
           }
           else
           { // format with decimals
             sprintf(outstring,"%c%7s %s %c %s %c", polarity, wstring,units,mode2,status2,13); 
           }
  
         break;
         }
         
         
 
case 7:  { // Cardinal SB200 ------------------------------------------------------------------------------
  
  
           units[0]=tolower(units[0]);
           units[1]=tolower(units[1]);
           
           status=tolower(status);  // lower case for cardinal 
           mode2=tolower(mode2);
           
  
           if (SCALE.DIS_DEC==0)  // format with no decimals
           {
            sprintf(outstring,"%c%c%06s.%c %s %c  %c",13, polarity, wstring,status,units,mode2,0x03);
           }
           else
           { // format with decimals
            sprintf(outstring,"%c%c%07s%c %s %c  %c",13, polarity,wstring ,status,units,mode2,0x03);
            
           }
  
          // insert leading zeros
         
           for (i=2;i!=8;i++) // loop until non space
           {
             if (outstring[i]==' ') // replace spaces with leading zeros
             {
               outstring[i]='0';  // pad with leading zero
             }
             else
             {
               break;  // nothing more to pad, break out of loop
             }
             
           }
         
           //         
         break;
         }
          
 
case 8:  { // SMA    ------------------------------------------------------------------------------
  
  
           units[0]=tolower(units[0]);  // convert to lower case to support cardinal
           units[1]=tolower(units[1]);
  
           
           if (status=='M')   // motion translation for SMA motion flag
           {
            status=' ';       // clear motion flag from status
           }
         
            if (status=='-')  // check and remove sign
           {
            status=' ';       // clear motion flag from status
           }         
                      
           if (SCALE.ATZERO==1)  // are we at center of zero 
           {
            status='Z'; 
           }
             
          // build SMA string 
          sprintf(outstring,"%c%c%c%c%c%c   %7.7s%s%c%c",10, status, '1',mode2,motion,' ',wstring ,units,' ' ,13);
          //          
          // 
         break;
         } //
         
         
         
case 9:  { // A&D 4323    ------------------------------------------------------------------------------
  
           units[1]=tolower(units[1]);
      
            if (SCALE.NETMODE)         
             {
              strcpy(mode,"NT");
             }
             else
             {
               strcpy(mode,"GS");
             }
   
           
           if (status=='M')
             {
               strcpy(status2,"US");
             }  
             else
                {
                  strcpy(status2,"ST");
                } 
           
            
            if (status=='O')
             {
               strcpy(status2,"OL");
             }
             
            
            if (polarity==' ')
            {
             polarity='+'; 
            }
            
          // build A&D string 
          sprintf(outstring,"%s,%s,%c%7.7s%s%c%c", status2,mode,polarity,wstring,units,13,10);
         
          
          if (SCALE.DIS_DEC==0)   // insert leading zeroes
          {
            i=8;   // leading zeroes start at pos 8 
          }
          else
          {
           i=7;  // leading zeros start at pos 7
          }
          
          for (;i!=13;i++) // loop until non space
           {
             if (outstring[i]==' ') // replace spaces with leading zeros
             {
               outstring[i]='0';  // pad with leading zero
             }
             else
             {
               break;  // nothing more to pad, break out of loop
             }
             
           }
             
         
          
  
         break;
         }          
         

case 10:  { // Ohaus CW-11 ------------------------------------------------------------------------------
  
           units[0]=tolower(units[0]);  // convert to lower case 
           units[1]=tolower(units[1]);
                  
           strcpy(status2,"  ");  // initialize to spaces
                     
           
           if (status=='M')
             {
               status='?';
             }  
             else
               status=' ';
           
    
            // format with decimals
             sprintf(outstring,"%9.9s %s %c%c%c%c",  wstring,units,status,mode2,13,10); 
           
     if (polarity=='-')
     {//1
             for (i=1;i!=7;i++)  // insert polarity
             {
                if (outstring[i]!=' ')
                {
                  i--;
                  outstring[i]='-';
                  break;
                }
               
             }
     }//1           
             
  
         break;
         }
           
         

  
  
 case 12:  { // Weightronix W125  ------------------------------------------------------------------------------
  
           units[0]=tolower(units[0]);  // convert to lower case 
           units[1]=tolower(units[1]);
           
            if (SCALE.NETMODE)         
             {
              status='N';
             }
             else    
                                  
             {
               status='G';
             }
           
            
            sprintf(outstring," %c%8.8s %s %c%c",status, wstring,units,13,10); 
            
           
     if (polarity=='-')
     {//1
             for (i=2;i!=9;i++)  // insert polarity
             {
                if (outstring[i]!=' ')
                {
                  i--;
                  outstring[i]='-';
                  break;
                }
               
             }
     }//1          
                
           
         break;
         }
                  
  
  
  
 case 13:  { // Transmit ACC5  ------------------------------------------------------------------------------            
                       
            weight=ACC[5];   // get the displayed weight 
            format_string(weight, wstring,SCALE.DIS_DEC);   // format the weight string and insert decimals if any
  
            // format with decimals
             sprintf(outstring,"%7.7s%c%c",  wstring,13,10); 
      
         break;
         }
           
        
         
case 14:  { // Western advanced string  ------------------------------------------------------------------------------
  
  
             deadzero=SCALE.DIS_DEADZERO*10;
                              
              if (deadzero==0)
              {
                deadzero=1;
              }
              else
                if (deadzero==1)
                {
                   deadzero=10;
                }
               else
                if (deadzero==2)
                {
                   deadzero=100;
                }
              
             
             tweight = SCALE.DIS_TAREWEIGHT * deadzero;             
             format_string(tweight, tstring,SCALE.DIS_DEC);   // format the weight string and insert decimals if any                
             
             sl=' ';  // stoplight status initialized   'R'=RED 'G'=GREEN 'r'=blinking red 'g'=blinking green  
             
             if (SCALE.ATZERO)  // center of zero flag
             {  
               cz='Z';
             }
             else
             {
               cz=' '; 
             }
    
           //              
             if (SCALE.OVER)  // blank tare weight on scale over
            {
              strcpy(tstring,"EEEEEE");
            }  

            // DFadvanced string -------------------------------------------------------------------------------        
              sprintf(outstring,"%c%c%7.7s %7.7s %s %s %c %c%c%c%c",0x02,polarity,wstring,tstring,units,mode,status,cz,sl,0x0d,0x0a);
   
         break;
         }         
  
  } // end of switch


  
  //-------------------------------------------------------------------------------------------
  // transmit string to selected com port
  if (port==_PORT0)   // send to port 0
  {
    xmitport(outstring,_PORT0);  
  }  
  
  if (port==_PORT1)   // send to port 0  // or port 1
  {
    xmitport(outstring,_PORT1); 
  }
 
  
 if (port==_PORT2)   // send to port 0  // or port 1
 {
  
   xmitport(outstring,_PORT2); 
    
 }
  
} // end of function 

//-----------------------------------------------------------------------------------------------
// process any commands recieved on COM port
//-----------------------------------------------------------------------------------------------
void process_serial_poll_commands( int port)
{
char command=0; 
char stream=0;  
 
if (port==_PORT0)
{  
command = poll_com_port1();  


switch (command)
 {
 
 case 'G':  { // zero scale
               SCALE.NETMODE=0; 
               stream=1;
             break;
            }
            
 case 'N':  { // zero scale
                if (SCALE.TAREWEIGHT !=0)   // there is no tare recorded, GROSS/NET has no function
                  {
                    SCALE.NETMODE=1;                   
                  } 
               stream=1;
             break;
            }           
   
   
 case 'Z':  { // zero scale
                SCALE.ZEROSCALE=1; 
               stream=1;
             break;
            }
   
   
 case 'C':  {  // clear tare
               SCALE.PEAKCLEAR=1; // set flag to CLEAR peakdetect (if enabled)
               clear_tare();
               stream=1;
             break;
            }
  
            
 case 'T':  { //tare scale
             platter_tare();  // tare the scale
             stream=1;
             break;
            }
            
  case '?':  { // poll for weight
              stream=1;
              break;
            }
              
  case 'K':  { // switch to kgs
              SCALE.DIS_UNITS=_KG;
              stream=1;
              break;
            }               
            
  case 'L':  { // switch to lbs
              SCALE.DIS_UNITS=_LB;
              stream=1;
              break;
            }   
            
  case 'O':  { // switch to onzse
              SCALE.DIS_UNITS=_OZ;
              stream=1;
              break;
            }           
   
            
  case 'g':  { // switch to grams
              SCALE.DIS_UNITS=_G;
              stream=1;
              break;
            }  
            
  case 'U':  { // toggle Units
                toggle_units(); 
            stream=1;
              break;
            }    
            
  case 'P':  { // print
                 if ((CAL.COM1_MODE==2) || (CAL.COM2_MODE==2))  // is printing enabled
                    {
                     print_ticket(1); 
                    } 
            
              break;
            }           
                        
            
            
 case '@': { // enter ticket download mode
             ticket_download();
             break;
           } 
                      
 
 default: { // enter ticket download mode
            stream= 0;
            command =0;
             break;
           } 
                  
       
 } // end of switch
 
 
 if (stream==1)  // check if we are streaming a string to the comport
   {//1
      
       SCALE.COM_STREAM0=1;  // set flag to transmit stream to comport0 (if enabled)
       
   }//1

}

if (port==_PORT1)
{  
command = poll_com_port2(); 

switch (command)
 {
 
 case 'G':  { // zero scale
               SCALE.NETMODE=0; 
               stream=1;
             break;
            }
            
 case 'N':  { // zero scale
                if (SCALE.TAREWEIGHT !=0)   // there is no tare recorded, GROSS/NET has no function
                  {
                    SCALE.NETMODE=1;                   
                  } 
               stream=1;
             break;
            }           
   
   
 case 'Z':  { // zero scale
                SCALE.ZEROSCALE=1; 
               stream=1;
             break;
            }
   
   
 case 'C':  {  // clear tare
               SCALE.PEAKCLEAR=1; // set flag to CLEAR peakdetect (if enabled)
               clear_tare();
               stream=1;
             break;
            }
  
            
 case 'T':  { //tare scale
             platter_tare();  // tare the scale
             stream=1;
             break;
            }
            
  case '?':  { // poll for weight
              stream=1;
              break;
            }
              
  case 'K':  { // switch to kgs
              SCALE.DIS_UNITS=_KG;
              stream=1;
              break;
            }               
            
  case 'L':  { // switch to lbs
              SCALE.DIS_UNITS=_LB;
              stream=1;
              break;
            }   
            
  case 'O':  { // switch to onzse
              SCALE.DIS_UNITS=_OZ;
              stream=1;
              break;
            }           
   
            
  case 'g':  { // switch to grams
              SCALE.DIS_UNITS=_G;
              stream=1;
              break;
            }  
            
  case 'U':  { // toggle Units
                toggle_units(); 
             stream=1;
              break;
            }    
            
  case 'P':  { // print
                 if ((CAL.COM1_MODE==2) || (CAL.COM2_MODE==2))  // is printing enabled
                    {
                     print_ticket(1); 
                    } 
            
              break;
            }           
                        
             
 case '@': { // enter ticket download mode
             ticket_download();
             break;
           } 
                      
 
  default: { // enter ticket download mode
            command =0;
             stream=0;
             break;
           }               
                  
       
 } // end of switch
  
 if (stream==1)  // check if we are streaming a string to the comport
   {//1
      
      // SCALE.COM_STREAM0=1;  // set flag to transmit stream to comport0 (if enabled)
       SCALE.COM_STREAM1=1;  // set flag to transmit stream to comport1 (if enabled)   
     //  SCALE.COM_STREAM2=1;  // set flag to transmit stream to comport1 (if enabled) 
   }//1
 
}

if (port==_PORT2)
{  
command = poll_com_port3();

switch (command)
 {
 
 case 'G':  { // zero scale
              SCALE.NETMODE=0; 
              stream=1;
             break;
            }
            
 case 'N':  { // zero scale
                if (SCALE.TAREWEIGHT !=0)   // there is no tare recorded, GROSS/NET has no function
                  {
                    SCALE.NETMODE=1;                   
                  } 
               stream=1;
             break;
            }           
   
   
 case 'Z':  { // zero scale
                SCALE.ZEROSCALE=1; 
               stream=1;
             break;
            }
   
   
 case 'C':  {  // clear tare
               SCALE.PEAKCLEAR=1; // set flag to CLEAR peakdetect (if enabled)
               clear_tare();
               stream=1;
             break;
            }
  
            
 case 'T':  { //tare scale
             platter_tare();  // tare the scale
             stream=1;
             break;
            }
            
  case '?':  { // poll for weight
              stream=1;
              SCALE.COM_STREAM2=1;  // set flag to transmit stream to comport1 (if enabled)               
              break;
            }
              
  case 'K':  { // switch to kgs
              SCALE.DIS_UNITS=_KG;
              stream=1;
              break;
            }               
            
  case 'L':  { // switch to lbs
              SCALE.DIS_UNITS=_LB;
              stream=1;
              break;
            }   
            
  case 'O':  { // switch to onzse
              SCALE.DIS_UNITS=_OZ;
              stream=1;
              break;
            }           
   
            
  case 'g':  { // switch to grams
              SCALE.DIS_UNITS=_G;
              stream=1;
              break;
            }  
            
  case 'U':  { // toggle Units
                toggle_units(); 
             stream=1;
              break;
            }    
            
  case 'P':  { // print
                 if ((CAL.COM1_MODE==2) || (CAL.COM2_MODE==2))  // is printing enabled
                    {
                     print_ticket(1); 
                    } 
            
              break;
            }         
              
                      
 
 default: { // enter ticket download mode
            command =0;
             stream=0;
             break;
           } 
                  
   //  
 } // end of switch
 
   
 if (stream==1)  // check if we are streaming a string to the comport
   {//1
      
   //    SCALE.COM_STREAM0=1;  // set flag to transmit stream to comport0 (if enabled)
   //    SCALE.COM_STREAM1=1;  // set flag to transmit stream to comport1 (if enabled)  
         SCALE.COM_STREAM2=1;  // set flag to transmit stream to comport1 (if enabled) 
   }//1
 
} 
   
} // end of function
//--------------------------------------------------------------------
// see if there  are characters to transmit
// Transmits 1 character out of the comport every time it is called until there is nothing to xmit.
//--------------------------------------------------------------------
void process_COM_xmit(int port)
{
char c;

  if (isCOMready(port)==false)  // check if com port is ready
  {
    return;  // comport register not empty, abort try again next time    
  }
     
  if (COM[port].X==COM[port].W)  // check if com xmit pointer = write pointer
  {
    COM[port].IDLE=TRUE;  // there is nothing more to transmit, indicate idle state
    return;   // nothing more to do, return
  }
  
   COM[port].IDLE=FALSE;  // indicate that we are not idle and we are still transmitting data from the buffer   
// we are processing data to transmit 
  
 if (COM[port].X==_XBUFFMAX)  // check if we have a wrap around condition
        {
          COM[port].X=0;  // set xmit pointer to zero for wraparounf of circular buffer     
        } 
        
        c = COM[port].BUFFER[COM[port].X];  // get the next character, 
        COM[port].X++;   //incrememnt xmit pointer
           
        write_COM_TX(c,port); // transmit character
       
 //           
}  // end of function

//----------------------------------------------------------------------------------------------------------------
// COM output stream depending on how it is setup in calibration
//----------------------------------------------------------------------------------------------------------------
void process_outputstream2(void)
{
    
  switch(CAL.COM3_MODE)
  {
 
  case 0:   // continuous stream output -----------------------------
          {
                 stringformat(_PORT2,CAL.STREAM3);  //  format output string and transmit 
                
                 break;
          }
  
  case 1:   //output stream on PRINT  -------------------------------
          {
                if (SCALE.COM_STREAM2==1) // check if lag is set for 1 time tranmit of stream
                 {
                //   SCALE.COM_STREAM0=0; // clear flag
                //   COM[_PORT0].IDLE=TRUE;  // there is nothing more to transmit, indicate idle state
                   stringformat(_PORT2,CAL.STREAM3);     //  format output string and transmit
                 }
                break;
          } 
          
          
 case 3:   //output stream on poll  -------------------------------
          {
                if (SCALE.COM_STREAM2==1) // check if lag is set for 1 time transmit of stream
                 {
               //    SCALE.COM_STREAM0=0; // clear flag
               //    COM[_PORT0].IDLE=TRUE;  // there is nothing more to transmit, indicate idle state
                   stringformat(_PORT2,CAL.STREAM3);     //  format output string and transmit
                 }
                break;
          }          
          
                
          
          
  case 4: //output stream on NO MOTION ----------------------------
           {
            if (SCALE.MOTION==0)
             {
               stringformat(_PORT2,CAL.STREAM3);     //  format output string and transmit
             }
           break;
          } 
          
          
     default: {
                SCALE.COM_STREAM2=0;  // set flag to transmit stream to comport0 (if enabled)
       //SCALE.COM_STREAM1=1;  // set flag to transmit stream to comport1 (if enabled)    
             break;
            }
     
 
    
  } // end of switch
  
  
  SCALE.COM_STREAM2=0;  // set flag to transmit stream to comport0 (if enabled) 
  
 }  


//----------------------------------------------------------------------------------------------------------------
// COM output stream depending on how it is setup in calibration
//----------------------------------------------------------------------------------------------------------------
void process_outputstream0(void)
{  
  switch(CAL.COM1_MODE)
  {
  
  case 0:   // continuous stream output -----------------------------
          {
                 stringformat(_PORT0,CAL.STREAM1);  //  format output string and transmit 
                
                 break;
          }
  
  case 1:   //output stream on PRINT  -------------------------------
          {
                if (SCALE.COM_STREAM0==1) // check if lag is set for 1 time tranmit of stream
                 {
                // SCALE.COM_STREAM0=0; // clear flag
                //   COM[_PORT0].IDLE=TRUE;  // there is nothing more to transmit, indicate idle state
                   stringformat(_PORT0,CAL.STREAM1);     //  format output string and transmit
                 }
                break;
          } 
          
          
 case 3:   //output stream on poll  -------------------------------
          {
                if (SCALE.COM_STREAM0==1) // check if lag is set for 1 time transmit of stream
                 {
               //    SCALE.COM_STREAM0=0; // clear flag
               //    COM[_PORT0].IDLE=TRUE;  // there is nothing more to transmit, indicate idle state
                   stringformat(_PORT0,CAL.STREAM1);     //  format output string and transmit
                 }
                break;
          }          
      
               
  case 4: //output stream on NO MOTION ----------------------------
           {
            if (SCALE.MOTION==0)
             {
               stringformat(_PORT0,CAL.STREAM1);     //  format output string and transmit
             }
           break;
          } 
          
          
     default: {
                SCALE.COM_STREAM0=0;  // set flag to transmit stream to comport0 (if enabled)
       //SCALE.COM_STREAM1=1;  // set flag to transmit stream to comport1 (if enabled)    
             break;
            }
    
  } // end of switch
  
  SCALE.COM_STREAM0=0; // clear flag 
} // end of function


//----------------------------------------------------------------------------------------------------------------
// COM output stream depending on how it is setup in calibration
//----------------------------------------------------------------------------------------------------------------
void process_outputstream1(void)
{  
  
  switch(CAL.COM2_MODE)
  {
  
  case 0:   // continuous stream output -----------------------------
          {
                 stringformat(_PORT1,CAL.STREAM2);  //  format output string and transmit 
                 break;
          }
  
  case 1:   //output stream on PRINT  -------------------------------
          {
                if (SCALE.COM_STREAM1==1)  // check if flag is set for 1 time tranmit of stream
                 {
                //  SCALE.COM_STREAM1=0; // clear flag
                //  COM[_PORT1].IDLE=TRUE;  // there is nothing more to transmit, indicate idle state
                  stringformat(_PORT1,CAL.STREAM2);     //  format output string and transmit
                 }
                break;
          } 
          
          
 case 3:   //output stream on poll  -------------------------------
          {
                if (SCALE.COM_STREAM1==1)  // check if flag is set for 1 time tranmit of stream
                 {
                //    SCALE.COM_STREAM1=0; // clear flag
                //   COM[_PORT1].IDLE=TRUE;  // there is nothing more to transmit, indicate idle state
                    stringformat(_PORT1,CAL.STREAM2);     //  format output string and transmit
                 }
                break;
          }
             
          
          
  case 4: //output stream on NO MOTION ----------------------------
           {
            if (SCALE.MOTION==0)
             {
               stringformat(_PORT1,CAL.STREAM2);     //  format output string and transmit
             }
           break;
          } 
          
          
     default: {
                SCALE.COM_STREAM1=0;  // set flag to transmit stream to comport0 (if enabled)
       //SCALE.COM_STREAM1=1;  // set flag to transmit stream to comport1 (if enabled)    
             break;
            }
     
 
    
  } // end of switch
  
 // 
 SCALE.COM_STREAM1=0; // clear flag
 
} // end of function

//
// 
//--------------------------------------------------------------------------------
void xmitport(char string[],int port)
{
  int i;
  
  
  if (COM[port].IDLE==FALSE)  // if the com port is still busy tranmitting a string, wait for it to be idle
{
 return;   
}
   
    COM[port].X=0;  // reset buffer pointers before spooling
    COM[port].W=0;
    
   if (port==_PORT0)
      {
    SCALE.COM_STREAM0=0; // clear flag 
      }
   
   if (port==_PORT1)
      {
      SCALE.COM_STREAM1=0; // clear flag   
      } 
   
   if (port==_PORT2)
      {
      SCALE.COM_STREAM2=0; // clear flag   
      }   
  
   i=0;
  while(string[i]!=0)
  {
  COM[port].BUFFER[COM[port].W++]=string[i++];  // add string to buffer to spooled to comport
      
  }   
  
  
} // end of function

//------------------------------------------------------------------------------------
//  read poll commands com 1
//  poll commands can either come from the comport as characters, or as external switch contacts
// wired to COM port as I/O, this is determined in the mode menu
//--- -----------------------------------------------------------------------------------
char poll_com_port1(void)
{
char commands[]="ZPCT"; // holds the com port poll command character
char string[]="      ";
char ch;
char mode; 
static char state; // static statemachine for external switch debouncing
static long debounce=0;  // static used for debouncing
char extswitch;

 mode = CAL.COM1_MODE;
 
if ( mode ==  3)   // check if we are polling for a character or not
  {
      return (read_COM_RX(_PORT0));  // poll com port for poll command
  }
else
if ( mode < 6)   // check if we are polling for a character or not
  {
     return(0);
  }
mode = mode - 6;  // the mode parameter determines which command to respomd to key presses
//  scan for external push buttons attached to comport 
// else poll comport for key press

//extswitch =read_legal();
extswitch = read_COM_level(_PORT0); // read the external push button switch status

           if (timer(REMOTE_SWITCH1 ,TIMEOUT))  // check  debounce timer
           {//1
             
              timer(REMOTE_SWITCH1 ,START); 
             
                   if (extswitch)   // debouncing and noise filtering
                   {//2
                     debounce++;
                     if (debounce>3) 
                     {
                       debounce=3;   // this is max count = press
                     } 
                   }//2
                   else
                   {//3
                     debounce--;
                     if (debounce<0)
                     {
                       debounce=0;      // count 0 is release
                     }
                   }//3
           }//1
   // statemachine for debouncing external pushbutton
   switch(state)
   {
   case 0: {  // wait for pushbutton
            if ( debounce == 3 )  // check if a switch is pressed 
            {
             state=2;  // goto state and execute pushbutton       
         //    DISPLAY.BLINK=1; // blink the display (100 scan duration) to indicate keypress
              ch=commands[mode];
             
              string[3]=ch;
              display_message( string,2);  // debugging
              
              return(ch); // return push button command
            }
            break;
           }          
           
           
   case 2: { // wait for pushbutton release
     
           if ( debounce == 0 )  // check if a switch is released
            {
             state=0;  // goto state and execute pushbutton       
            
            }
         //
          break;
           }
       // 
     
   default: {
             state=0;  // safety
            break;
               }
     
     
   } // end of switch
    
return(0);

  
} // end of function 

//----------------------------------------------------------------------------------------

char poll_com_port3(void)
{
// 
char commands[]="ZPCT"; // holds the com port poll command character
char string[]="      ";
char ch;
char mode; 
char extswitch; 
char i;
static char state; // static statemachine for external switch debouncing
static char pos;
static long debounce=0;  // static used for debouncing
static char address;
static char idflag;

mode = CAL.COM3_MODE;

if ( mode ==  3)   // check if we are polling for a character or not
  { 
  ch=read_COM_RX(_PORT2);  // read from COM0 port 
  
   if (ch=='#') // check for ID number 
			{ //22
					  idflag=1; // set flag                       
					  return(0); // finished break out
  			}//22         
    
   if (idflag==1 ) // check for ID number 
			{ //22	                                               
                          
   if ( (ch > 47) && (ch < 58) )  // check fpr valid number between 0 and 7                                           
                                                // fixed on version 4-05 for 0-9
					      {                                               
                                                 address = ch - 48;  // calculate ID number
                                                 idflag=0;
                                                 return(0); // finished break out    
                                              } // 
   if ( ch == ':' )  // check fpr valid number between 0 and 7                                           
                                                // fixed on version 4-05 for 0-9
					      {                                                  
                                                 address = ch; // calculate ID number
                                                 idflag=0;
                                                 return(0); // finished break out    
					       } //                          
                        
                        }
  
  if(CAL.ADDRESS == address)
  {   
   return (ch);  // read from COM0 port      
  }
 
  if( address ==  ':' )
  {   
   return (ch);  // read from COM0 port      
  }
  
   // idflag=0; 
    return(0); // finished break out     
  
  switch(state)
 {
   
 case 0:  // setup for recieving characters
         {                                              
            if (ch=='#') // check for ID number 
			{ //22
					   idflag=1; // set flag                       
					   pos = 1;   // init globle index count to 1 
                                           state = 1;
                                           break;
  			}//22  
                    
           if(CAL.ADDRESS == address)
              {   
               return (ch);  // read from COM0 port      
              }
                    
         break;
	 }  // 
         
      case 1: // check for new character in buffer, fill STRING[BUFLEN]
	     {           
                   if (idflag==1) // was the previous character a '#' id marker
		   {//33      then this character should be a multidrop id number
                                           //   idflag=0;  // clear flag  
                                              
                                              if ( (ch > 47) && (ch < 58) )  // check fpr valid number between 0 and 7
                                            
                                                // fixed on version 4-05 for 0-9
					      {
                                                 address = ch - 48;  // calculate ID number
					         break; // finished break out
					       } // 
		    }//33                    
                   
                    
                     
                   if (pos == 5)  // safety check for count run away
                         {
                          //pos = 0;  // set SPOS to maximum buffer length of STRING[BUFLEN];
                          state = 2;
                          break;
                         }                  
                   
                   if (ch == 13)  //check for CR character, signifying end of string
		    {                      
                       //   pos = 0;  // set SPOS to maximum buffer length of STRING[BUFLEN];
                          state = 2;
                          break;
		    }   //                  
                            
                    
		  string[pos++]=ch;       // add a character to the processing string STRING
               
                break;
		 } // 
                
                // we are here because the string terminated with a CR
   case 2:   {  // determine if string is alpha numeric, or numeric
                     if (pos==1) // check if there are no characters to process (CR character only)
                       {
                         state=0;
                         break;
		       }    // n
                       if (CAL.ADDRESS !=address) 
                       // if the multi drop ID does not match, abort
		      {
                         state=0;
                         break;
                      } 
                      
                       for (i=1;i!=pos;i++) // loop through and check for control characters
					    {//2  
                                             extswitch = string[i];
                                             
                             if ( ( extswitch > 60) && ( extswitch < 90) )  // check fpr valid number between 0 and 7                                            
                                                // fixed on version 4-05 for 0-9
					      {
                                                 state=0;
                                                 return(extswitch);            
                                               } 
                                            }
                         state=0;
                         break;
		          } // if

 } 
 //  
 return(0);       
//   
 }
else
if ( mode < 6)   // check if we are polling for a character or not
  {
     return(0);
  }
mode = mode - 6;  // the mode parameter determines which command to respomd to key presses
//  scan for external push buttons attached to comport 
// else poll comport for key press
//extswitch =read_legal();
extswitch = read_COM_level(_PORT2); // read the external push button switch status

           if (timer(REMOTE_SWITCH1 ,TIMEOUT))  // check  debounce timer
           {//1             
              timer(REMOTE_SWITCH1 ,START); 
             
                   if (extswitch)   // debouncing and noise filtering
                   {//2
                     debounce++;
                     if (debounce>3) 
                     {
                       debounce=3;   // this is max count = press
                     } 
                   }//2
                   else
                   {//3
                     debounce--;
                     if (debounce<0)
                     {
                       debounce=0;      // count 0 is release
                     }
                   }//3
           }//1
   // statemachine for debouncing external pushbutton
   switch(state)
   {
   case 0: {  // wait for pushbutton
            if ( debounce == 3 )  // check if a switch is pressed 
            {
             state=2;  // goto state and execute pushbutton       
         //    DISPLAY.BLINK=1; // blink the display (100 scan duration) to indicate keypress
              ch=commands[mode];             
              string[3]=ch;
              display_message( string,2);  // debugging
              
              return(ch); // return push button command
            }
            break;
           }
           
       //        
           
   case 2: { // wait for pushbutton release
     
           if ( debounce == 0 )  // check if a switch is released
            {
             state=0;  // goto state and execute pushbutton      
            
            }     
          break;
           }       
     
   default: {
             state=0;  // safety
            break;
               }
     
     
   } // end of switch
    
return(0);
// S
}

//------------------------------------------------------------------------------------
//  read poll commands com2
//  poll commands can either come from the comport as characters, or as external switch contacts
// wired to COM port as I/O, this is determined in the mode menu
//--- -----------------------------------------------------------------------------------
char poll_com_port2(void)
{
char commands[]="ZPCT"; // holds the com port poll command character
char string[]="      ";
char ch;
char mode; 
static char state; // static statemachine for external switch debouncing
static long debounce=0;  // static used for debouncing
char extswitch;

mode = CAL.COM2_MODE;

if ( mode == 3)   // check if we are polling for a character or not
  {
     return (read_COM_RX(_PORT1));  // poll com port for poll command
  }
else
if ( mode < 6)   // check if we are polling for a character or not
  {
     return(0);
  }

//  scan for external push buttons attached to comport 
//  else poll comport for key press

 mode=mode - 6;  // the mode parameter determines which command to respomd to key presses
 
 extswitch= read_COM_level( _PORT1); // read the external push button switch status
 
           if (timer(REMOTE_SWITCH2 ,TIMEOUT))  // check  debounce timer
           {//1
             
              timer(REMOTE_SWITCH2 ,START); 
             
                   if (extswitch)   // debouncing and noise filtering
                   {//2
                     debounce++;
                     if (debounce>3) 
                     {
                       debounce=3;   // this is max count = press
                     } 
                   }//2
                   else
                   {//3
                     debounce--;
                     if (debounce<0)
                     {
                       debounce=0;      // count 0 is release
                     }
                   }//3
           }//1
 
  // statemachine for debouncing external pushbutton
   switch(state)
   {
   case 0: {  // wait for pushbutton
            if ( debounce == 3 )  // check if a switch is pressed 
            {
             state=2;  // goto state and execute pushbutton       
  //            DISPLAY.BLINK=1; // blink the display (100 scan duration) to indicate keypress
              ch=commands[mode];
              
              string[3]=ch;
              display_message( string,2);  // version number changed here
              
              return(ch); // return push button command
            }
            break;
           }
    
           
   case 2: { // wait for pushbutton release
     
           if ( debounce == 0 )  // check if a switch is released
            {
             state=0;  // goto state and execute pushbutton       
            
            }
     
          break;
           }
       
     
   default: {
             state=0;  // safety
            }
    
      //  
   } // end of switch
    // 
   return(0);
   
} // end of function 

//----------------------------------------------------------------------------------------
// Command mode
// Load in string until CR is received.  
// Parse Numeric, or parse AlpaNumeric
//-----------------------------------------------------------------------------------------------------------

void command_string(void)
{
  
/*
char ch;
char i;
char digit;
char alpha;
char ctrl;
//char j;
//char id;
static char idflag;


  switch (cSTATE)
    {//1      
      
     case 0:  // setup for recieving characters
         {
           SPOS = 1;   // init globle index count to 1 
           cSTATE = 1;
           break;
	 }  // 
         
      case 1: // check for new character in buffer, fill STRING[BUFLEN]
	     { 
                ch = readcom0buffer();   // read the serial RX circular buffer //     
                
                if (ch) // is there a character to read?
                 {//2   
                  command_timeout_timer(START);  
                                     
                  if (ch=='#') // check for ID number 
			{ //22
					  idflag=1; // set flag                       
					  break; // finished break out
  			}//22         
                   
                   if (idflag) // was the previous character a '#' id marker
		   {//33      then this character should be a multidrop id number
                                              idflag=0;  // clear flag  
                                              
                                              if ( (ch > 47) && (ch < 58) )  // check fpr valid number between 0 and 7
                                            
                                                // fixed on version 4-05 for 0-9
					      {
                                                 cID = ch - 48;  // calculate ID number
					         break; // finished break out
					       } // 
		    }//33                    
                  
                   
                  // if
                   if (ch !=10)  //check for CR character, signifying end of string
		    {//3                    
		    STRING[SPOS++]=ch;       // add a character to the processing string STRING
                    }//3
                    
                   if (SPOS > BUFLEN)  // safety check for count run away
                         {
                          SPOS = BUFLEN;  // set SPOS to maximum buffer length of STRING[BUFLEN];
                         }                   
                        
                  
                 //  if (ch == 13)  //check for CR character, signifying end of string
                     if (ch ==  BOARD.FRAME)  //check for CR character, signifying end of string  
                     
		    {
                                     PARSE.CNT = SPOS;  // set the length of string recieved
                                     cSTATE = 2; // go and process string
				  //   break;
		    }   //                                                         
                
                   //  
                   // Error
                    
                 }//2
            
                break;
		 } // 
                
                // we are here because the string terminated with a CR
   case 2:   {  // determine if string is alpha numeric, or numeric
                    
                     
                      if (SPOS==1) // check if there are no characters to process (CR character only)
                       {
                         cSTATE=0;
                         break;
		       }    //   
                       
                       
                      if (BOARD.ID!=cID) 
                       // if the multi drop ID does not match, abort
		      {
                         cSTATE=0;
			 break;
                      } 
                      
                      switch (STRING[1])  
                      // check the first charactacter for the string to determine action
                      {//5                        
                      case '@': {                        
                                  cSTATE = 4;  // parse string as characters
                                  break; 
                                 }                          
                                 
                      default: {
                                 BOARD.CYCOMMAND=0; // to reset the cycle command
                                 cSTATE = 3;  // parse string as weight
                                 break;
                               }               
                        
                      } // 5  end of switch      
                       
                   
                  break;
		          } //   

                     // string has been determined to hold a weight string
        case 3:   {  // process captured weight string and DISPLAY it
                     autoscan();  // parse STRING[BUFLEN], extract weight and units
                     cSTATE=0;     // reinit cSTATE machine and start again
                      break;
		          }
                     // string is an alphanumeric display
        case 4:   {  // process captured string as alphanumeric DISPLAY  (DISPLAY letters)
 
                      ctrl =0; 
                         
                      for (i=1;i!=SPOS;i++) // loop through and check for control characters
					    {//2
                                                          
                             ctrl = parse_command(STRING[i]);        
                              
                                       if(ctrl)
                                       {// 
                                       break;  
                                       }//  
                             
                             
                                            }// 2
                      
                                         
                  cSTATE = 5;     // reinit cSTATE machine and start again
                  break;
		          }//  
                        
                      // string is an alphanumeric display
        case 5:   {  // process captured string as alphanumeric DISPLAY  (DISPLAY letters)
 
                        alpha=0; // clear alphanumeric flag
			digit=0;
                                            
                        for (i=1;i!=SPOS;i++) // loop through string and copy DISPLAYable characters to the DISPLAY
					    {//1
                                                     alpha=0;
                           
                           if ( ( STRING[i] > 47 ) && ( STRING[i] < 58 ) ) // is the character between 0 and 9
					             {
	        				     alpha=1; // DISPLAYable character
						     }
                                                     else
                            if ( ( STRING[i] > 64 ) && ( STRING[i] < 91 ) ) // is the character between A and Z
					           {
	        				     alpha=1; // DISPLAYable character
							   }
                                                      else
                            if ( ( STRING[i] > 96 ) && ( STRING[i] < 123 ) ) // is the character between a and z
					           {
	        				     alpha=1; // DISPLAYable character
							   }
							   else
                            if (STRING[i] ==' ') // is the character a space
					           {
	        				     alpha=1; // DISPLAYable character
							   }
							   else
                            if (STRING[i] =='-') // is the character a minus
					           {
	        				     alpha=1; // DISPLAYable character
							   } 
                           
                             if (alpha) // add character to scoreboard DISPLAY
						      {
                                                        BOARD.DEC=0;  // clear decimals
			                                BOARD.POL=0;  // clear sign character
                                                        
                                                        BOARD.LB=0; // clear anunciators in alpha mode
                                                        BOARD.KG=0;
                                                        BOARD.GROSS=0;
                                                        BOARD.NET=0;  
                                                        BOARD.CYCOMMAND=0; // to reset the cycle command                        
                                                                             
                                                        BOARD.DIGIT[digit++] = STRING[i];

                                                       if (digit==6) // check if we are finished loading characters to the DISPLAY 
                                                                 {
								  break;  // break out of for i loop
								 } //                                                      
				                           } // 
				}//1  for i loop
                              
                           
                      
                              //check if we have to pad any spaces in the DISPLAY
                             if (digit) // has an alpha numeric character been DISPLAYed
                              { //                       
                             
                                for (i=digit; i!=6;i++)
                                                     {
                                   BOARD.DIGIT[i]=' '; // fill remaining unused digit spaces with blanks
                                   BOARD.DIGIT[6] = 0; // fill remaining unused digit spaces with blanks
                                                     }
                               } // 
                      
                  
                  cSTATE = 0;     // reinit cSTATE machine and start again
                  break;
		          }//  

       default: { // 
                   cSTATE = 0;
                   break;
	             } // 

	}  // end of switch

*/

                         
               
} // end of function

//##################################################################################################################
//##################################################################################################################
char read_COM_level(int port) // read the external push button switch status
{  
 // uart_txc('T');
 // return (read_COM_RX(_PORT0));  // poll com port for poll command  
  
//return (read_legal());
//////////////////////////////////////////////////////////
//  
  //////////////////////////////////////////////////
  // p3.26       PWM     OUTPUT  AUXILIARY   OEnable  
  // 
  
  if (port==_PORT0)
 { 
   
// PINSEL0 &=0xFFFFFF0F; // Configure 0.0 to 0.15   
  
 if ((FIO0PIN & 0x00000008)==0) // disable output
  {
    return(1);
  }
     
        return(0); 
       
}

if (port==_PORT1)
{  

 if ((FIO0PIN & 0x00000800)==0) // disable output
  {
    return(1);
  }
        
       
        return(0); 
       
     
}

if (port==_PORT2)
{  

 if ((FIO0PIN & 0x00000002)==0) // disable output
  {
    return(1);
  }
        
       
        return(0); 
       
}



return(0);
}