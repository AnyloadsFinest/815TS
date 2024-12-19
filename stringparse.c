#include "includes.h"

char STRING[BUFLEN + 1];  // string used for parsing scoreboard information
char PARSESTATE;            // state machine for string parsing
char SPOS;                  //position of string count
//----- ---------------------------------------------------------------------
// scan the character and see if we are parsing it
//--------------------------------------------------------------------------
void  scan_character(char ch)
{   
  switch  (PARSESTATE)
    {
     case WAIT_FOR_FRAME_START:  // wait for the character that marks the beginning of the string
        {
         if (ch == PARSE.FRAME)                
          {           
           SPOS = 1;
           STRING[SPOS++] = ch;
	   BOARD.COUNTER++ ;
           PARSESTATE = COUNT_IN_STRING; // go to next state and count characters
           } // 
        break;
    	} // 

    case COUNT_IN_STRING:  // count the characters into string buffer
        {
          STRING[SPOS++] = ch;
          if (SPOS > BUFLEN) SPOS = BUFLEN; // safety check for count run away	 
	  if (SPOS == PARSE.CNT) PARSESTATE = PARSE_STRING; // string has been read in, now parse it to scoreboard
	  break;
    	} // 
                
    case PARSE_STRING: // parse the string buffer to the scoreboard
        {
	if (PARSE.AUTOSCAN == 1)  // check how we are going to parse the string
		  {                 
                  autoscan();  // string is to be auto parsed, no tables used 
		  }
		  else
                  { // 
                   parsestring(); // parse the string from the table based string definitions 
                  }//
        
    	PARSESTATE = WAIT_FOR_FRAME_START; // rearm statemachine and wait for next frame start marker
        break;
		}
        
        default:
		{ //  
                 SPOS=0; 
                 PARSESTATE= WAIT_FOR_FRAME_START;  // invalid state condition                
                 break;
		} // 
    } // end of switch

} // end of function

void autoscan(void)
{
char i;
char digit[10];
char d;
char ch;
char dec;
char flag;
char j;
char spacecount;
char start;
char end;

start = 1;
end = PARSE.CNT;

if(PARSE.LENGTH == 0)
{
PARSE.LENGTH = PARSE.CNT;  
}
if(PARSE.WEIGHT == 0)
{
PARSE.WEIGHT = 1;  
} //  

if(PARSE.WEIGHT > 1)
{
start = PARSE.WEIGHT;
}

if((start + PARSE.LENGTH) < PARSE.CNT )
{
end = start + PARSE.LENGTH;
}
//char *ptr;
// scan from left to right in search of numeric values 
dec=0;  // clear decimal postion
d=0; // clear character count
flag=0; // clear flag that indicates that we are parsing a string of numbers
spacecount=0;
for (i=0;i!=10;i++) digit[i]=' '; // fill with spaces
//--------------------------------------------------------------------
// Some strings have individual numbers as status characters (weightronix).
// Loop through and detect any single digit characters that could cause confusion for a weight number.
// If we find a single digit that does not preceed with 2 spaces, or follow with another digit, we blank it out.
// This avoids confusion when extracting the weight from the string
// start analyzing string to see how many numeric characters there are in the string, count them
j=0;
for (i=1;i!=PARSE.CNT;i++) // loop through length of string
{//12
  if ( isdigit(STRING[i]) )  // check if value is a numeric digit
    {
     j++;  // incrememnt digit count
     }     // 
}// 12
//  11
 
// start analyzing string for single digit characters
for (i=1;i!=PARSE.CNT;i++) // loop through length of string
{//11
   if (j < 2) // if there are less than 2 digits in the string, no need to filter single digit characters
    {
     break; // break out of this loop
     }
   
    if ( isdigit(STRING[i]) )  // check if value is a numeric digit
	   {//1
             if (isdigit(STRING[i+1])) // check if the next character is a digit    
			  {//4
                break;  // exit the loop, we have two digits in a row, most likely a real weight
			  }//4           
           	  //////////////////////////////////////////////////////////////
           // added on Aug 20 2010 to fix the 00.00 bug in command mode 
              if  (STRING[i + 1]=='.')// check if value is a numeric digit
			    {
			     break;  // incrememnt digit count
			     } // 
             ////////////////////////////////////////////////////////////////////////////
              if (spacecount > 1) // check for spaces preceding digit
			      {//5
                              break;  // there are 2 spaces or more before the digit, break out of loop
			      }//5                    
               
			   STRING[i]=' '; // remove single digit string, fill in with space
                           break; // break out of loop
       }//1

    if (STRING[i]==' ') // check for a space
        {//2
	   spacecount ++; // we deteced a space, increment counter
	 }//2
	 else
	 {//3
	   spacecount = 0; // reset counter
	 }//3		
} // 11 

for (i= start ;i!= end; i++) // loop through length of string
{ 
   ch = STRING[i]; // get a character from the string buffer

    if ( isdigit(ch) )  // check if value is a digit
	   {//1       
         digit[d++] = ch; //copy numeric into string
         if (d > 6) break; // can have 7 digits max
	  	   flag=1;
	    }//1
        else 
	     { //2
           if (ch=='.') dec=d; // make note of a decimal position  
                   else
                    if(BOARD.COMMA)
                    { 
                    if (ch==',') dec=d; // make note of a decimal position  
                    }
                       else
			  {
			    if (flag) break; // non numeric character detected, finished reading numeric string
                           }    // 
                     
		}//2
               
} // end of for loop scanning string

//   
// format decimals *********************************************
if (dec)
BOARD.DEC = d-dec;
else
BOARD.DEC = 0;

// check is we have mode that 6 numeric characters, than left shift to bring down to 6
if (d > 5)  // more than 6 characters
{
 for (j=0;j!= d-6;j++)
 for (i=0;i!=8;i++) digit[i]=digit[i+1];  // left shift out leading zero
 d=6;
}



switch ( CAL.Dec ) // forced decimal point
        {
        case 0 : {break;}
        case 1 : { BOARD.DEC=1; 
                    break;} //
        case 2 : { BOARD.DEC=2; 
                                 
                  break;} //
        case 3 : { BOARD.DEC=0; break;} 
        
        default: {break;} 
               
        } 


 if (BOARD.ZSUPPRESS==1) // fix for deadzero and zerosuppression together
{//6
// remove leading zeroes (if any)
// some equipment pads zeros in front of the weight
// if there is a decimal we do not remove the leading zero in front of the decimal
for (i=0;i!=(5 - BOARD.DEC);i++) // loop through and remove leading zeros
{ //4 
  if (digit[i]=='0') // check if the character is a zero
   {//5
          digit[i]=' '; // replace zero with a space to blank it out
   }//5
   else // character is not a zero, check if we are finished 
   {//7
    if (digit[i]!=' ') // if there is any other character other than a space  
	 {//8
        break; // no more leading zeroes, break out of loop
	 } //8 
   } // 7

 } //4 //

}//6 
        
   /*     // 
if (BOARD.ZSUPPRESS==1) // check if zero suppression is enabled
{//6
// s
// remove leading zeros if any
if (BOARD.DEC!=d-1) // check for leading zero before a decimal "0.0"
{
    for (i=0;i!=d-1;i++) // loop through and remove leading zeros
     { //4 
      if (digit[0]=='0')
       {//5
         for (j=0;j!=7;j++) digit[j]=digit[j+1];  // left shift out leading zero
       }//5
       else
       {       // 
        break; // no more leading zeroes
       }// 45   
     }  //
}//
}   
*/
    // 
    // format weight string ****************************************
for (j=0;j!=6;j++) // right shift string until there are no more spaces
 {
  if (digit[5]==' ')
   { // 
    for (i=5;i!=0;i--) digit[i]=digit[i-1]; // right shift character by 1
    digit[0]=' '; // fill with space
   }
   else                  // 
   break;
  } // 

for (i=0;i!=6;i++)  
{
BOARD.DIGIT[i] = digit[i]; // copy string over scoreboard output
BOARD.DIGIT[6]=0; // null terminate
}
//////////////////////////////////
//check if we are adding a dead zero
//check if we are adding a dead zero
if (BOARD.DEC ==1) // two dead zero
{//1  
         if (BOARD.DIGIT[4]==' ')
		   {
              BOARD.DIGIT[4]='0';
		   } 
  
 }//1
else if (BOARD.DEC==2)// three dead zeros
{//1                 
         
         if (BOARD.DIGIT[4]==' ')
		   {              
              BOARD.DIGIT[4]='0';              
		   }
          
              if (BOARD.DIGIT[3]==' ')
		   {     
               BOARD.DIGIT[3]='0';
		   }        
		
         
	
  
 }//1


if (BOARD.DEADZERO==1) // two dead zero
{//1
    if (BOARD.DIGIT[5]=='0') 
	  {
         if (BOARD.DIGIT[4]==' ')
		   {
              BOARD.DIGIT[4]='0';
		   }
	  } // 
  
 }//1
else if (BOARD.DEADZERO==2)// three dead zeros
{//1
    if (BOARD.DIGIT[5]=='0') 
	  {
         if (BOARD.DIGIT[4]==' ')
		   {
                    BOARD.DIGIT[4]='0'; 
                   }
         
        if (BOARD.DIGIT[3]==' ')
		   {      
               BOARD.DIGIT[3]='0';
		   }        
		
	  } // 
  
 }//1


extract_weight();   // extract a long weight from the weight string
// search for LB or KG flag *************************************
BOARD.KG=0;
BOARD.LB=0;
         
for (i=1;i!=PARSE.CNT;i++) // loop through length of string
{//1
    ch=toupper(STRING[i]); // get a character from the string buffer

    if (ch=='K') 
	{
         BOARD.KG=1;
	 BOARD.LB=0;
	 break;
	}
    else
    if (ch=='L') 
	{
         BOARD.KG=0;
	 BOARD.LB=1;
	 break;
	} // 
    else
    if (ch=='T') 
	{
         BOARD.KG=0;
	 BOARD.LB=1;
	  // break;
	} // 
          //  
}//1

  BOARD.GROSS = 1;    // The previous character was not a 'K'
  BOARD.NET = 0;
  //search for NET or GROSS flag ************************************
for (i=1; i!= PARSE.CNT; i++) // loop through length of string
{
    ch=toupper(STRING[i]); // get a character from the string buffer
   
    if (ch=='N') 
	{//3
         BOARD.GROSS=0;
	 BOARD.NET=1;
	 break; // h
	}//3
}

BOARD.POL = 0;
//search for sign character ************************************
for (i=1; i!= PARSE.CNT; i++) // loop through length of string
  {
    ch = STRING[i]; // get a character from the string buffer

    if (ch=='-') 
	{
	 BOARD.POL=1;
         break;
	}
	else
        BOARD.POL=0;
} //  

BOARD.OVER = 0;

if(BOARD.OCAP == 1)
{//1
//search for overload character ************************************
for (i = 1; i!= PARSE.CNT; i++) // loop through length of string
{
  ch=toupper(STRING[i]);       // get a character from the string buffer
  // 
  if (ch=='O') 
	{
	 BOARD.OVER = 1;
         break;
	 } //
   else if (ch=='U') 
	{
	 BOARD.OVER = 1;
         break;
	 } //   
	 else
         BOARD.OVER = 0;    
} //  
}//1
else if(BOARD.OCAP == 2)
{//1
//search for overload character ************************************
for (i = 1; i!= PARSE.CNT; i++) // loop through length of string
{
  ch=toupper(STRING[i]);       // get a character from the string buffer
  // 
  if (ch=='C') 
	{
	 BOARD.OVER = 1;
         break;
	 } // 
	 else
         BOARD.OVER = 0;    
} //  
}//1
else if(BOARD.OCAP == 3)
{//1
//search for overload character ************************************
for (i = 1; i!= PARSE.CNT; i++) // loop through length of string
{
  ch=toupper(STRING[i]);       // get a character from the string buffer
  // 
  if (ch=='E') 
	{
	 BOARD.OVER = 1;
         break;
	 } // 
	 else
         BOARD.OVER = 0;    
} //  
}//1



BOARD.ZERO = 0;

for (i = 1; i!= PARSE.CNT; i++) // loop through length of string
{
  ch=toupper(STRING[i]);       // get a character from the string buffer
  // 
  if (ch=='Z') 
	{
	 BOARD.ZERO = 1;
         break;
	 } //
   else if (BOARD.VALUE == 0) 
	{
     if (BOARD.GROSS == 1) 
	{
	 BOARD.ZERO = 1;
         break;
	 } //
         } //
	 else
         BOARD.ZERO = 0;
  
}//1

 
} // end of function

//---------------------------------------------------------------------------------
// Take the string captured and parse the components to build up the scoreboard output
//---------------------------------------------------------------------------------
void parsestring(void)
{
char i;
char j;
char ch;
int chu;
char end;
char start;
char status;  // specific for toledo strings
char chone;
char chuone;
//
for (i=0; i!=6; i++) BOARD.DIGIT[i]=' '; // fill with spaces
//************* extract weight string **********************************************
start = PARSE.WEIGHT - 1;   // calculate the beginning character pos of the weight digits
end = start + PARSE.LENGTH; // calculate the end characters of the weight digits
// *************************************************   
// some indicator strings put spaces or decimals in the right character pos (cardinal for example)
if ( isdigit(STRING[end])==0  ) end--; // if the rightmost character is not a digit, skip over
//************* extract weight ********************************************
BOARD.DEC=0; // clear decimal pos count
// ** 
i=0;
j=5;
// ** 
for (i=end;i!=start;i--)  // count backwards throught string
 {//1
  //if (j<0) break; //safety check, max 6 digits can be read
  if (STRING[i]=='.') // check for decimal place
   {//2
    BOARD.DEC = end - i; // 
    i--; // skip over decimal in weight string
   }//2
 // 
  BOARD.DIGIT[j--]=STRING[i]; // copy over weight digits
 }//1
             

if(CAL.StringType == 3)
{ //start if 
  for (i=0;i!=end;i++)  // count backwards throught string
   {//1  
    if (STRING[i]== 0x02) // check for decimal place
     {//2
      status = STRING[i + 1];  // status byte A location in string
      status = status & 0x07;  
      break;
     }//2 
   }//1  
      
  switch  (status)
    {
     case 3:  // wait for the character that marks the beginning of the string
        {         
        BOARD.DEC = 1;  
        break;
    	}
  case 4:  // wait for the character that marks the beginning of the string
        {         
        BOARD.DEC = 2;  
        break;
    	}
  case 5:  // wait for the character that marks the beginning of the string
          {           
          BOARD.DEC = 3;  
          break;
          }
      default: {break;}  
}//end of switch

}// end of if

switch ( CAL.Dec ) // forced decimal point
        {
        case 0 : {break;}
        case 1 : { BOARD.DEC=1; break;} //
        case 2 : { BOARD.DEC=2; break;} //
        case 3 : { BOARD.DEC=0; break;} 
        
        default: {break;}   
        
        } //  

if (BOARD.ZSUPPRESS==1) // fix for deadzero and zerosuppression together
{//6
// remove leading zeroes (if any)
// some equipment pads zeros in front of the weight
// if there is a decimal we do not remove the leading zero in front of the decimal
for (i=0;i!=(5 - BOARD.DEC);i++) // loop through and remove leading zeros
{ //4 
  if (BOARD.DIGIT[i]=='0') // check if the character is a zero
   {//5
          BOARD.DIGIT[i]=' '; // replace zero with a space to blank it out
   }//5
   else // character is not a zero, check if we are finished 
   {//7
    if (BOARD.DIGIT[i]!=' ') // if there is any other character other than a space  
	 {//8
        break; // no more leading zeroes, break out of loop
	 } //8 
   } // 7

 } //4 //

}//6 


//check if we are adding a dead zero
if (BOARD.DEC ==1) // two dead zero
{//1  
         if (BOARD.DIGIT[4]==' ')
		   {
              BOARD.DIGIT[4]='0';
		   } 
  
 }//1
else if (BOARD.DEC==2)// three dead zeros
{//1                 
         
         if (BOARD.DIGIT[4]==' ')
		   {              
              BOARD.DIGIT[4]='0';              
		   }
          
              if (BOARD.DIGIT[3]==' ')
		   {     
               BOARD.DIGIT[3]='0';
		   }        
		
    
 }//1

///////////////////////////////////
//check if we are adding a dead zero
if (BOARD.DEADZERO==1) // two dead zero
{//1
    if (BOARD.DIGIT[5]=='0') 
	  {
         if (BOARD.DIGIT[4]==' ')
		   {
              BOARD.DIGIT[4]='0';
		   }
	  } //  
  
 }//1
else if (BOARD.DEADZERO==2)// three dead zeros
{//1
    if (BOARD.DIGIT[5]=='0') 
	  {
         if (BOARD.DIGIT[4]==' ')
		   {
                    BOARD.DIGIT[4]='0'; 
                   }
         
        if (BOARD.DIGIT[3]==' ')
		   {      
               BOARD.DIGIT[3]='0';
		   }        
	  } // 
  
 }//1
 
//************* extract weight ********************************************
extract_weight();   // extract a long weight from the weight string
//************* extract sign status ********************************************
BOARD.POL=0;   // clear status flags

ch=STRING[PARSE.PPOS]; // get the status character for polarity

if (ch==PARSE.Pmask) BOARD.POL=1;

if (PARSE.BINARY)  // check if we are using binary status bits
{//1
 if (PARSE.Pmask!=0)
 {//2
   chu=ch & PARSE.Pmask;
   //binary mask status character to check if the POL status is present
   if ( chu==PARSE.Pmask )  BOARD.POL=1; // set pol flag
  }//2
  else
   BOARD.POL=0;
}//1


// ************* extract units *************************************************
BOARD.KG=0; // clear status flags
BOARD.LB=0;
//
ch=STRING[PARSE.UPOS];
chu=toupper(ch); // get the status character for lb/kg

if(CAL.StringType == 5)
{ // SB400 different size string 

if(BOARD.DEC == 0)  
{  
ch = STRING[PARSE.UPOS - 1]; // get the status character for polarity
chu = toupper(ch); 
} 
  
}// 

// 

if (chu == PARSE.KGmask)
{
 BOARD.KG=1; 
 BOARD.LB=0;
}
else
if (chu == PARSE.LBmask)
{
 BOARD.LB=1;
 BOARD.KG=0; 
} // 

if(CAL.StringType == 9) // specific to Rinstrum string to keep units on while in motion
{
chone = STRING[PARSE.UPOS + 1];
chuone=toupper(chone); // get the status character for lb/kg

if (chuone== 'T')   // added to pick the metric tonne
{
 BOARD.LB=1;
 BOARD.KG=0; 
}    
}// end of rinstrum string 


if (PARSE.BINARY)  // check if we are using binary status bits (e.g. TOLEDO indicators)
{//1
 if (PARSE.KGmask!=0) // check if we are bit masking the KG status bit
 {//2
  chu = ch & PARSE.KGmask;  // bitmask

   if (chu == PARSE.KGmask) BOARD.KG = 1;   // set flags accordanly
    else
     BOARD.LB = 1;  
 }//2
 //   

 if (PARSE.LBmask!=0) // check if we are bitmasking the LB status bit
 {//2
  chu=ch & PARSE.LBmask;  // bitmask

   if (chu==PARSE.LBmask) BOARD.LB = 1;   // set flags accordanly
    else
     BOARD.KG=1;  
 }//2
}//1


//************* extract gross/net status ***************************************
BOARD.GROSS=0; // 
BOARD.NET=0;   // clear status flags

ch=STRING[PARSE.SPOS];
chu=toupper(ch); // get the status character for net/gross

if(CAL.StringType == 5)
{ // SB400 different size string 

if(BOARD.DEC == 0)  
{  
ch = STRING[PARSE.SPOS - 1]; // get the status character for polarity
chu = toupper(ch); 
} 
  
}// 


if (chu==PARSE.Nmask) BOARD.NET=1;
else
if (chu==PARSE.Gmask) BOARD.GROSS=1;

if (PARSE.BINARY)  // check if we are using binary status bits (e.g. TOLEDO indicators)
{//1
 if (PARSE.Nmask!=0)
 {//2
  chu=ch & PARSE.Nmask;

   if (chu==PARSE.Nmask) BOARD.NET=1; 
    else
     BOARD.GROSS=1;  
 }//2
// 
 if (PARSE.Gmask!=0)
 {//2
  chu=ch & PARSE.Gmask;

   if (chu==PARSE.Gmask) BOARD.GROSS=1; 
    else
     BOARD.NET=1;  
 }//2
 //  

}//1

//************* extract OVER status ********************************************
BOARD.OVER=0;   // clear status flags

ch = STRING[PARSE.OPOS]; // get the status character for polarity
chu = toupper(ch);

if(CAL.StringType == 5)
{ // SB400 different size string 

if(BOARD.DEC == 0)  
{  
ch = STRING[PARSE.OPOS - 1]; // get the status character for polarity
chu = toupper(ch); 
} 
  
}// 


if (chu == PARSE.Omask) 
 {
  BOARD.OVER=1;
 }// 

if (PARSE.BINARY)  // check if we are using binary status bits
{//1
 if (PARSE.Omask!=0)
 {//2
   chu=ch & PARSE.Omask;
   //binary mask status character to check if the POL status is present
   if ( chu == PARSE.Omask )  BOARD.OVER = 1; // set pol flag
  }//2
  else
   BOARD.OVER = 0;
}//1
 

if(BOARD.OCAP == 1)
{//1
//search for overload character ************************************
for (i = 1; i!= PARSE.CNT; i++) // loop through length of string
{
  ch=toupper(STRING[i]);       // get a character from the string buffer
  // 
  if (ch=='O') 
	{
	 BOARD.OVER = 1;
         break;
	 } //
   else if (ch=='U') 
	{
	 BOARD.OVER = 1;
         break;
	 } //   
	 else
         BOARD.OVER = 0;    
} //  
}//1
else if(BOARD.OCAP == 2)
{//1
//search for overload character ************************************
for (i = 1; i!= PARSE.CNT; i++) // loop through length of string
{
  ch=toupper(STRING[i]);       // get a character from the string buffer
  // 
  if (ch=='C') 
	{
	 BOARD.OVER = 1;
         break;
	 } // 
	 else
         BOARD.OVER = 0;    
} //  
}//1
else if(BOARD.OCAP == 3)
{//1
//search for overload character ************************************
for (i = 1; i!= PARSE.CNT; i++) // loop through length of string
{
  ch=toupper(STRING[i]);       // get a character from the string buffer
  // 
  if (ch=='E') 
	{
	 BOARD.OVER = 1;
         break;
	 } // 
	 else
         BOARD.OVER = 0;    
} //  
}//1
else if(BOARD.OCAP == 4)
{//1

         BOARD.OVER = 0;    
 
}//1
//************* extract Motion status ********************************************
BOARD.LMOTION=0;   // clear status flags

ch = STRING[PARSE.MPOS]; // get the status character for polarity
chu = toupper(ch);

if (chu==PARSE.Mmask) 
 {
  BOARD.LMOTION = 1;
 }// 

if (PARSE.BINARY)  // check if we are using binary status bits
{//1
 if (PARSE.Mmask!=0)
 {//2
   chu=ch & PARSE.Mmask;
   //binary mask status character to check if the POL status is present
   if ( chu == PARSE.Mmask )  BOARD.LMOTION = 1; // set pol flag
  }//2
  else
   BOARD.LMOTION = 0;
}//1


BOARD.ZERO = 0;

for (i = 1; i!= PARSE.CNT; i++) // loop through length of string
{
  ch=toupper(STRING[i]);       // get a character from the string buffer
  // 
  if (ch=='Z') 
	{
	 BOARD.ZERO = 1;
         break;
	 } //
   else if (BOARD.VALUE == 0) 
	{
     if (BOARD.GROSS == 1) 
	{
	 BOARD.ZERO = 1;
         break;
	 } //
         } //
	 else
         BOARD.ZERO = 0;
  
}//1


////////////////// added in version 4-02////////////////////////////////////////
// added for toledo strings to detect the decimal point
// type 3 moved up

// stop light controls
if (parse_stoploght())
{
BOARD.STOPCONTROL = 1; // over wrting   
}
else
{
 // reset stop control every cycle
BOARD.STOPCONTROL = 0; // over wrting  
}//  

}// end of function

char parse_stoploght(void)
{// parsing stop light
char i;
char status;  // specific for toledo strings
char stopstatus;
char rssiflag;
char id;
  
if(CAL.StringType == 10)
{ //string type Western Advance
  
  stopstatus = STRING[19];
  rssiflag  = STRING[18]; 
 
  if ( (BOARD.PGM==1) || (BOARD.PGM==2) || (BOARD.PGM==3) || (BOARD.PGM==4) ||
        (BOARD.PGM==5) || (BOARD.PGM==6) || (BOARD.PGM==7) || (BOARD.PGM==8) ||
        (BOARD.PGM==9) || (BOARD.PGM==10)
        ) // special mode with a forced 9600N81 baud
          {//1               // set to 9600, and break out of autolearn          
           return(0);  // exit out of autobaud with 9600 set, we don't need to learn the string for special mode
          }//1
   
   switch  (stopstatus)
    {//switch
     case '&':  // red light character
        {
        //  stoplight(RED);
          
         if(BOARD.TRAFFIC==2)
         {
         stoplight(RED);
         }
         else
            if(BOARD.TRAFFIC==1)
               {
               stoplight(GREEN);
               }     
         break;
    	}
    case '*':  // green light character
          {         
        //  stoplight(GREEN);  
            
              if(BOARD.TRAFFIC==1)
         {
         stoplight(RED);
         }
         else
            if(BOARD.TRAFFIC==2)
               {
               stoplight(GREEN);
               }
   
          break; // 
          }
          
          
   case '%':  // green light character
          {
          if(rssiflag =='$')
          {  
          BOARD.RSSIFLAG =1;
          }
           
          break; // 
          }  
    default:  // wait for the character that marks the beginning of the string
        {         
         stoplight(OFF); 
        break;
    	}      
 
    }// end of switch     

 
 return(1);
}// end of wester nadvance

if(CAL.StringType == 11)
{ //string type Western Advance
 
  stopstatus = STRING[28];
  rssiflag   = STRING[27];
     
  if ( (BOARD.PGM==1) || (BOARD.PGM==2) || (BOARD.PGM==3) || (BOARD.PGM==4) ||
        (BOARD.PGM==5) || (BOARD.PGM==6) || (BOARD.PGM==7) || (BOARD.PGM==8) ||
        (BOARD.PGM==9) || (BOARD.PGM==10)
        ) // special mode with a forced 9600N81 baud
          {//1               // set to 9600, and break out of autolearn          
           return(0);  // exit out of autobaud with 9600 set, we don't need to learn the string for special mode
          }//1

  
   switch  (stopstatus)
    {//switch
    case '&':  // red light character
        {
        //  stoplight(RED);
          
         if(BOARD.TRAFFIC==2)
         {
         stoplight(RED);
         }
         else
            if(BOARD.TRAFFIC==1)
               {
               stoplight(GREEN);
               }
                 
         
         break;
    	}
    case '*':  // green light character
          {         
        //  stoplight(GREEN);  
            
              if(BOARD.TRAFFIC==1)
         {
         stoplight(RED);
         }
         else
            if(BOARD.TRAFFIC==2)
               {
               stoplight(GREEN);
               }
                 
            
            
          break; // 
          }
     case '%':  // green light character
          {
          if(rssiflag =='$')
          {  
          BOARD.RSSIFLAG = 1;
          } // 
          //  
          break; // 
          } //
   
      default:  // wait for the character that marks the beginning of the string
        {         
         stoplight(OFF); 
        break;
    	}      
          
    }// end of switch  
   
 
 return(1);
}// end of wester nadvance 
 // 

if(CAL.StringType == 12)
{ //string type Western Advance
 
  id         = STRING[29];
  stopstatus = STRING[28];
  rssiflag   = STRING[27];
   
  if ( (id > 47) && (id < 58) )  // check fpr valid number between 0 and 7                                            
                                                // fixed on version 4-05 for 0-9
					      { // 1
                                              if(BOARD.ID != (id - 48))  // calculate ID number
					      { // 2
                                               clear_annuciators();
                                               for (i=0; i!=6; i++) BOARD.DIGIT[i]=' '; // fill with spaces
                                               return(0);
                                              } // 2
                                              
                                              } // 1
 //  
    
  if ( (BOARD.PGM==1) || (BOARD.PGM==2) || (BOARD.PGM==3) || (BOARD.PGM==4) ||
        (BOARD.PGM==5) || (BOARD.PGM==6) || (BOARD.PGM==7) || (BOARD.PGM==8) ||
        (BOARD.PGM==9)  || (BOARD.PGM==10)
        ) // special mode with a forced 9600N81 baud
          {//1               // set to 9600, and break out of autolearn          
           return(0);  // exit out of autobaud with 9600 set, we don't need to learn the string for special mode
          }//1
  
   switch  (stopstatus)
    {//switch
     case '&':  // red light character
        {
        //  stoplight(RED);
          
         if(BOARD.TRAFFIC==2)
         {
         stoplight(RED);
         }
         else
            if(BOARD.TRAFFIC==1)
               {
               stoplight(GREEN);
               }
                 
         
         break;
    	}
    case '*':  // green light character
          {         
        //  stoplight(GREEN);  
            
              if(BOARD.TRAFFIC==1)
         {
         stoplight(RED);
         }
         else
            if(BOARD.TRAFFIC==2)
               {
               stoplight(GREEN);
               }
                
                        
          break; // 
          }
          
     case '%':  // green light character
          {
          if(rssiflag =='$')
          {  
          BOARD.RSSIFLAG = 1;
          } // 
          
          break; // 
          } //
   
      default:  // wait for the character that marks the beginning of the string
        {         
         stoplight(OFF); 
        break;
    	}      
          
    }// end of switch  
   
 return(1);
}// end of wester nadvance 

if(CAL.StringType == 7)
{ //Fair banks new string with binary code
stopstatus = STRING[2]; // stop light position
status = STRING[3];    // status byte position
// BOARD.BLINK=100;
if( stopstatus == '4'); // get a character from the string bufferstopstatus == 'A')
{//1

   switch  (status)
    {//switch
 case '0':  // Lb, Gross
        {         
        BOARD.NET=0;   // clear status flags
        BOARD.GROSS=1; // 
        BOARD.KG=0; // clear status flags
        BOARD.LB=1;
        break;
    	}
  case '1':  // Lb, Net
        {         
        BOARD.NET=1;   // clear status flags
        BOARD.GROSS=0; // 
        BOARD.KG=0; // clear status flags
        BOARD.LB=1;  
        break;
    	}
  case '2':  // Lb, Tare
          {           
          BOARD.NET=0;   // clear status flags
          BOARD.GROSS=0; // 
          BOARD.KG=0; // clear status flags
          BOARD.LB=1;  
          break;
          }
  case'3':  // Kg, Gross
          {           
        BOARD.NET=0;   // clear status flags
        BOARD.GROSS=1; // 
        BOARD.KG=1; // clear status flags
        BOARD.LB=0;
          break;
          }
  case '4':  // Kg, Net
          {           
          BOARD.NET=1;   // clear status flags
          BOARD.GROSS=0; // 
          BOARD.KG=1; // clear status flags
          BOARD.LB=0;
          break;
          }
  case '5':  // Kg, Tare
          {           
          BOARD.NET=0;   // clear status flags
          BOARD.GROSS=0; // 
          BOARD.KG=1; // clear status flags
          BOARD.LB=0;  
          break;
          }
    }// end of switch
}//1
  
 return(0);
}// end of fair banks



if(CAL.StringType == 21)
{ //Fair banks new string with binary code
/*  
Added Fairbanks new stringversion 4-21
CODE DATA DISPLAYED
No traffic light Scale # 4,5,6,7,8
40 Lb. Gross
41 Lb. Net
42 Lb. Tare
43 Kg. Gross
44 Kg. Net
45 Kg. Tare

Green  Scale # A,B,C,D,E
A0 Lb. Gross
A1 Lb. Net
A2 Lb. Tare
A3 Kg. Gross
A4 Kg. Net
A5 Kg. Tare

Red    Scale # a,b,c,d,e 
a0 Lb. Gross
a1 Lb. Net
a2 Lb. Tare
a3 Kg. Gross
a4 Kg. Net
a5 Kg. Tare
00 Display all data received
*/  
stopstatus = STRING[2]; // stop light position
status = STRING[3];    // status byte position
// BOARD.BLINK=100;
if( toupper(stopstatus) == 'A'); // get a character from the string bufferstopstatus == 'A')
{//1

   switch  (status)
    {//switch
 case '0':  // Lb, Gross
        {         
        BOARD.NET=0;   // clear status flags
        BOARD.GROSS=1; // 
        BOARD.KG=0; // clear status flags
        BOARD.LB=1;
        break;
    	}
  case '1':  // Lb, Net
        {         
        BOARD.NET=1;   // clear status flags
        BOARD.GROSS=0; // 
        BOARD.KG=0; // clear status flags
        BOARD.LB=1;  
        break;
    	}
  case '2':  // Lb, Tare
          {           
          BOARD.NET=0;   // clear status flags
          BOARD.GROSS=0; // 
          BOARD.KG=0; // clear status flags
          BOARD.LB=1;  
          break;
          }
  case'3':  // Kg, Gross
          {           
        BOARD.NET=0;   // clear status flags
        BOARD.GROSS=1; // 
        BOARD.KG=1; // clear status flags
        BOARD.LB=0;
          break;
          }
  case '4':  // Kg, Net
          {           
          BOARD.NET=1;   // clear status flags
          BOARD.GROSS=0; // 
          BOARD.KG=1; // clear status flags
          BOARD.LB=0;
          break;
          }
  case '5':  // Kg, Tare
          {           
          BOARD.NET=0;   // clear status flags
          BOARD.GROSS=0; // 
          BOARD.KG=1; // clear status flags
          BOARD.LB=0;  
          break;
          }
    }// end of switch
}//1

   // if we are in utility program traffic light is controlled by software
  
   if ( (BOARD.PGM==1) || (BOARD.PGM==2) || (BOARD.PGM==3) || (BOARD.PGM==4) ||
        (BOARD.PGM==5) || (BOARD.PGM==6) || (BOARD.PGM==7) || (BOARD.PGM==8) ||
        (BOARD.PGM==9)  || (BOARD.PGM==10)
        ) // special mode with a forced 9600N81 baud
          {//1               // set to 9600, and break out of autolearn          
           return(0);  // exit out of autobaud with 9600 set, we don't need to learn the string ffor special mode
          }//1
  
   // if no utility program then take over the traffic light
   switch  (stopstatus)
    {//switch
      
     case 'A':  // scale number 1 green light
       case 'B':  // scale number 2 green light
         case 'C':  // scale number 3 green light
           case 'D':  // scale number 4 green light
             case 'E':  // scale number 5 green light
        {         
         stoplight(GREEN);
         break;
        
    	}// 
 
     case 'a':  // scale number 1 red light
       case 'b':  // scale number 2 red light
         case 'c':  // scale number 3 red light
           case 'd':  // scale number 4 red light
             case 'e':  // scale number 5 red light
               
        {  //    
         stoplight(RED);
         break;
       
    	}     
        
        
    default:
      {
      break;  
      }
    }// end of switch
  
 return(1);
}// end of fair banks


if(CAL.StringType == 26)
if(CAL.StringType == 27)
if(CAL.StringType == 28)
if(CAL.StringType == 29)
{ //string type Ranger A, B, C , and D
  stopstatus = STRING[2]; // stop light position
  status     = STRING[3]; // status byte position
   
   if ( (BOARD.PGM==1) || (BOARD.PGM==2) || (BOARD.PGM==3) || (BOARD.PGM==4) ||
        (BOARD.PGM==5) || (BOARD.PGM==6) || (BOARD.PGM==7) || (BOARD.PGM==8) ||
        (BOARD.PGM==9)  || (BOARD.PGM==10)
        ) // special mode with a forced 9600N81 baud
          {//1               // set to 9600, and break out of autolearn          
           return(0);  // exit out of autobaud with 9600 set, we don't need to learn the string for special mode
          }//1

   //BOARD.BLINK=100;
   switch  (stopstatus)
    {//switch
     case 0X31:  // red light character
        {         
       stoplight(RED);
       break;
    	}
    case 0X32:  // green light character
          {         
          stoplight(GREEN);
          break;
          }
     case 0X33:  // green light character
          {
           stoplight(OFF); // red and green togetther ?????
           break;
          }   // k           
          
      }// end of switch     
  
   
   return(1);
}// end of wester nadvance 
 

return(0);

} // end of function

//--------------------------------------------------------------------------------------------------------
// Autoscan string
// This function is used when the learn mode was not able to identify the string from the table.
// This function will then parse the string and try and figure out the string on its owm 
//--------------------------------------------------------------------------------------------------------
void autoscan2(void)
{
char i;
char digit[10];
char d;
char ch;
char dec;
char flag;
char j;
char *ptr;
char spacecount;

// scan from left to right in search of numeric values 
dec=0;  // clear decimal postion
d=0; // clear character count
flag=0; // clear flag that indicates that we are parsing a string of numbers
spacecount=0;

for (i=0;i!=10;i++) digit[i]=' '; // fill with spaces
//--------------------------------------------------------------------
// Some strings have individual numbers as status characters (weightronix).
// Loop through and detect any single digit characters that could cause confusion for a weight number.
// If we find a single digit that does not preceed with 2 spaces, or follow with another digit, we blank it out.
// This avoids confusion when extracting the weight from the string
// start analyzing string to see how many numeric characters there are in the string, count them
 j=0;
for (i=1;i!=PARSE.CNT;i++) // loop through length of string
{//12
  if ( isdigit(STRING[i]) )  // check if value is a numeric digit
    {
     j++;  // incrememnt digit count
     }
}//12
//  
// 
// start analyzing string for single digit characters
for (i=1;i!=PARSE.CNT;i++) // loop through length of string
{//11
   if (j<2) // if there are less than 2 digits in the string, no need to filter single digit characters
    {
     break; // break out of this loop
     }
    if ( isdigit(STRING[i]) )  // check if value is a numeric digit
	   {//1
             if (isdigit(STRING[i+1])) // check if the next character is a digit    
			  {//4
                break;  // exit the loop, we have two digits in a row, most likely a real weight
			  }//4           
           	  //////////////////////////////////////////////////////////////
           // added on Aug 20 2010 to fix the 00.00 bug in command mode 
              if  (STRING[i + 1]=='.')// check if value is a numeric digit
			    {
			     break;  // incrememnt digit count
				}
             ////////////////////////////////////////////////////////////////////////////
              if (spacecount>1) // check for spaces preceding digit
			    {//5
                 break;  // there are 2 spaces or more before the digit, break out of loop
				}//5                    
               
			   STRING[i]=' '; // remove single digit string, fill in with space
               break; // break out of loop
       }//1

    if (STRING[i]==' ') // check for a space
     {//2
	   spacecount ++; // we deteced a space, increment counter
	 }//2
	 else
	 {//3
	   spacecount = 0; // reset counter
	 }//3		
} // 11 
  // 
  // now loop through string and search for block of weight numbers

for (i=1;i!=PARSE.CNT;i++) // loop through length of string
{ 
   ch=STRING[i]; // get a character from the string buffer

    if ( isdigit(ch) )  // check if value is a digit
	   {//1
         digit[d++] = ch; //copy numeric into string
          if (d>6) break; // can have 7 digits max
	  	   flag=1;
	    }//1
        else 
	     { //2
          if (ch=='.') dec=d; // make note of a decimal position  
                   else
                    if(BOARD.COMMA)
                    { 
                    if (ch==',') dec=d; // make note of a decimal position  
                    }
                       else
			  {
			    if (flag) break; // non numeric character detected, finished reading numeric string
                           }    //
		}//2		
 //   
 // 
}// end of for loop scanning string

 // format decimals *********************************************
if (dec)
BOARD.DEC = d-dec;
else
BOARD.DEC=0;

// check is we have mode that 6 numeric characters, than left shift to bring down to 6

if (d>5)  // more than 6 characters
{
 for (j=0;j!= d-6;j++)
 for (i=0;i!=8;i++) digit[i]=digit[i+1];  // left shift out leading zero
 d=6;
}

// format weight string ****************************************
for (j=0;j!=6;j++) // right shift string until there are no more spaces
 {
  if (digit[5]==' ')
   {
    for (i=5;i!=0;i--) digit[i]=digit[i-1]; // right shift character by 1
    digit[0]=' '; // fill with space
   }
   else                  // 
   break;
  }


//for (i=0;i!=6;i++)  BOARD.DIGIT[i]=digit[i]; // copy string over scoreboard output
//BOARD.DIGIT[6]=0; // null terminate



//extract_weight();   // extract a long weight from the weight string
digit[6]=0;
BOARD.VALUE=strtol(digit,&ptr,10);  // convert board digits to long
BOARD.ISWEIGHT=1; // set flag to indicate the

//search for LB or KG flag *************************************
for (i=1;i!=PARSE.CNT;i++) // loop through length of string
{//1
    ch=toupper(STRING[i]); // get a character from the string buffer

    if (ch=='K') 
	{
         BOARD.KG=1;
	 BOARD.LB=0;
	 break;
	}
    else
    if (ch=='L') 
	{
         BOARD.KG=0;
	 BOARD.LB=1;
	 break;
	} //
     else
    if (ch=='T') 
	{
         BOARD.KG=0;
	 BOARD.LB=1;
	// break;
	} // 
    // 
    // 
    // 
    
}//1



  BOARD.GROSS=1;         // The previous character was not a 'K'
  BOARD.NET=0;
//search for NET or GROSS flag ************************************
for (i=1;i!=PARSE.CNT;i++) // loop through length of string
{
    ch=toupper(STRING[i]); // get a character from the string buffer
   
    if (ch=='N') 
	{//3
         BOARD.GROSS=0;
	 BOARD.NET=1;
	 break; // 
	}//3
}
//search for sign character ************************************
for (i=1;i!=PARSE.CNT;i++) // loop through length of string
{
    ch=STRING[i]; // get a character from the string buffer

    if (ch=='-') 
	{
	 BOARD.POL=1;
     break;
	}
	else
        BOARD.POL=0;
}

} // end of function

void simplescan(void)
{
char i;
char digit[10];
char d;
char ch;
char dec;
//char flag;
char j;
//char *ptr;
//char spacecount;

// scan from left to right in search of numeric values 
dec=0;  // clear decimal postion
d=0; // clear character count
//flag=0; // clear flag that indicates that we are parsing a string of numbers
//spacecount=0;
for (i=0;i!=10;i++) digit[i]=' '; // fill with spaces
//--------------------------------------------------------------------
// Some strings have individual numbers as status characters (weightronix).
// Loop through and detect any single digit characters that could cause confusion for a weight number.
// If we find a single digit that does not preceed with 2 spaces, or follow with another digit, we blank it out.
// This avoids confusion when extracting the weight from the string
// start analyzing string to see how many numeric characters there are in the string, count them
/*
for (i=1;i!=PARSE.CNT;i++) // loop through length of string
{ 
   ch=STRING[i]; // get a character from the string buffer

    if ( isdigit(ch) )  // check if value is a digit
	   {//1 
           spacecount=i-1;  
            break;
	    }//1
    
}// 
*/
//for (i=2;i!=PARSE.CNT;i++) // loop through length of string
//for (i=0;i!=PARSE.CNT;i++) // loop through length of string
for (i = PARSE.WEIGHT; i!= PARSE.WEIGHT+ PARSE.LENGTH;i++) // loop through length of string
{ 
   ch = STRING[i]; // get a character from the string buffer

   if (ch=='.') 
   {//2
     dec=d; // make note of a decimal position         
   }//2
   else
     if(BOARD.COMMA)
                    { 
                    if (ch==',') dec=d; // make note of a decimal position  
                    }
                      
   else{//1  
        digit[d++] = ch; //copy numeric into string      
       } // l 
   
 if (d > 5) break; // can have 7 digits max		
}// 
  // 
  // format decimals *********************************************
if (dec)
BOARD.DEC = d-dec;
else
BOARD.DEC=0;
// check is we have mode that 6 numeric characters, than left shift to bring down to 6

if (d > 5)  // more than 6 characters
{
 for (j=0;j!= d-6;j++)
 for (i=0;i!=8;i++) digit[i]=digit[i+1];  // left shift out leading zero
 d=6;
}
 
    // format weight string ****************************************
for (j=0;j!=6;j++) // right shift string until there are no more spaces
 {
  if (digit[5]==' ')
   { // 
    for (i=5;i!=0;i--) digit[i]=digit[i-1]; // right shift character by 1
    digit[0]=' '; // fill with space
   }
   else  // 
   break;
  } // 
for (i=0;i!=6;i++)  BOARD.DIGIT[i]=digit[i]; // copy string over scoreboard output
BOARD.DIGIT[6]=0; // null terminate

extract_weight();   // extract a long weight from the weight string

// search for LB or KG flag *************************************
for (i=1;i!=PARSE.CNT;i++) // loop through length of string
{//1
    ch=toupper(STRING[i]); // get a character from the string buffer

    if (ch=='K') 
	{
         BOARD.KG=1;
	 BOARD.LB=0;
	 break;
	}
    else
    if (ch=='L') 
	{
         BOARD.KG=0;
	 BOARD.LB=1;
	 break;
	} // 
    else
    if (ch=='T') 
	{
         BOARD.KG=0;
	 BOARD.LB=1;
	  // break;
	} // 
          // 
}//1

  BOARD.GROSS = 1;    // The previous character was not a 'K'
  BOARD.NET = 0;
  //search for NET or GROSS flag ************************************
for (i=1; i!=PARSE.CNT; i++) // loop through length of string
{
    ch=toupper(STRING[i]); // get a character from the string buffer
   
    if (ch=='N') 
	{//3
         BOARD.GROSS=0;
	 BOARD.NET=1;
	 break; // h
	}//3
}
// 
//search for sign character ************************************
for (i=1; i!= PARSE.CNT; i++) // loop through length of string
// for (i=PARSE.WEIGHT;i!=PARSE.CNT;i++) // loop through length of string
{
    ch=STRING[i]; // get a character from the string buffer

    if (ch=='-') 
	{
	 BOARD.POL=1;
     break;
	}
	else
        BOARD.POL=0;
} // 


} // end of function

//--------------------------------------------------------------------------
// scan the character and see if we are parsing it
//--------------------------------------------------------------------------
void  scan_character_manual(char ch)
{ 
  // 
  switch  (PARSESTATE)
    {
     case WAIT_FOR_FRAME_START:  // wait for the character that marks the beginning of the string
        {
         if (ch==PARSE.FRAME)                
          {
           SPOS=1;
           STRING[SPOS++]=ch;
           PARSESTATE=COUNT_IN_STRING; // go to next state and count characters
	  }
        break;
    	} //  
        
    case COUNT_IN_STRING:  // count the characters into string buffer
        {
          STRING[SPOS++]=ch;
          if (SPOS>BUFLEN) SPOS=BUFLEN; // safety check for count run away
		  if (SPOS==PARSE.CNT) PARSESTATE=PARSE_STRING; // string has been read in, now parse it to scoreboard
        break;
    	} // 
                
    case PARSE_STRING: // parse the string buffer to the scoreboard
        {		            
           autoscan();
           shift_manual(0);        
    	   PARSESTATE=WAIT_FOR_FRAME_START; // rearm statemachine and wait for next frame start marker
        break;
		}
        default:
		{
         PARSESTATE=0;  // invalid state condition
         SPOS=0;  
         break;
		}
    } // end of switch

} // end of function

// 
void parse_manual(void)
{
//char i;
//char ch;
char key=0;
//static char Lstate;

//************* extract weight string ******************************************
   key = scan_keypad(_NORMAL);  // scan the keypad for keypresses
         
         switch (key)
          {//3
       
        case _UP: {  // up key is pressed, set time  
                  PARSE.LENGTH++;                      
                  if(PARSE.WEIGHT == 1)
                  {//2       
                  if((PARSE.LENGTH - PARSE.WEIGHT) > 7)
                  {//1
                  PARSE.WEIGHT++;                  
                  }//1            
                  }//2
                  else
                  {//3
                  PARSE.WEIGHT++;  
                  }//3                      
                  if(PARSE.LENGTH > PARSE.CNT) {PARSE.LENGTH = PARSE.CNT;}
                  if(PARSE.WEIGHT > (PARSE.CNT - 2)) {PARSE.WEIGHT = PARSE.CNT - 2;}
                  break; // 
                  } //         
       case _DOWN: {  // up key is pressed, set time
                  PARSE.WEIGHT--;
                  if(PARSE.LENGTH == PARSE.CNT)
                  {//2                 
                  if((PARSE.LENGTH - PARSE.WEIGHT) > 7)
                  {//1
                  PARSE.LENGTH--;                
                  }//1
                  }//2
                  else
                  {//3  
                  PARSE.LENGTH--;            
                  }//3     
                      
                  if(PARSE.WEIGHT < 1) {PARSE.WEIGHT = 1;}   
                  if(PARSE.LENGTH < 3) {PARSE.LENGTH = 3;}                    
                  break; // 
                  } //            
      
          case _ENTER: {  // up key is pressed, set time
                  CAL.ParseWeight = PARSE.WEIGHT;
                  CAL.ParseLength = PARSE.LENGTH;
                  return;        
                  } //            
         
          case _ENTER_SET: {  // up key is pressed, set time
                  CAL.ParseWeight = PARSE.WEIGHT;
                  CAL.ParseLength = PARSE.LENGTH;
                  return;        
                  } //            
                  
        }//3 end of switch        
           

} // end of function


void Manual_Learn()
{ 
// char key=0;
// char stat=0;   
// char ch=0;
      
      BOARD.MODE = _LEARN; 
      text_message("EDIT  ",1);
  
 
} // 


char checksum(char length, char string[])
{ 
// this function calcualtes the recieved checksun in the string 
//  and compares it with the calcualted checksum  
// 
 char i=0; // 
 char csumrec=0; // received checksum
 char csumcal=0; // calculated checksum
 char csstring[2]; // sting to hold and separate nibbles of chekcsum
 char temp; // temp for calculation
 char len = length - 2; // length of string minus two checksum position
  
              for (i=1;i!=len;i++)  // calculate checksum
                   {
                    csumcal = csumcal + string[i]; // calcuate the checksum
                   }    // 
              
             csumcal = ~csumcal;  // invert bits ones compliment
             csumcal = csumcal + 1; // 2s compliment
             // csumcal = csumcal & 0x7f; // strip the 7's bit
             // csumrec = string[len];// load original checksum from string 
                           
             csstring[0] = string[len];// load original checksum nibble from string
             csstring[1] = string[len + 1];// load original checksum nibble from string
             
             if(csstring[0] == 0x02)
             {// chekc if there is no checksum
              return(1); // western advance without checksum                             
             } // 
             
             // reverse calculation to calculate checksum received from SCale Link
             temp = csstring[0] - 97; // offset
             temp = temp << 4; // shift right to recover second nibble
             temp = temp & 0XF0; 
             csumrec = temp;  
             temp = csstring[1] - 97; // offset
             temp = temp & 0X0F; // recover first nibble
             csumrec = csumrec | temp; // combine two nibbles to recover recieved chacksum
           
             //  csumrec = atoi(csstring);  // convert board digits to long
                          
             if(csumrec == csumcal)// compare sent and calculated for error
              {             
              return(1); // check sums are matching           
              }//  
             
 
 return(0); // checksums dont match
} // 


char checksumbyte(char length, char string[])
{ 
// this function calcualtes the recieved checksun in the string 
//  and compares it with the calcualted checksum  
 char i=0; // 
 char csumrec=0; // received checksum
 char csumcal=0;
 unsigned int chk=0; // calculated checksum
 char len = length - 1; // length of string minus two checksum position
  
            chk=0;
              for (i=1;i!=len;i++)  // calculate checksum
                   {
                    chk = chk + string[i];
                   }
              
             chk = ~chk;  // invert bits ones compliment
             chk = chk + 1; // 2s compliment
                             
             csumcal = chk; // add checksum
                                    
             csumrec = string[len];// load original checksum nibble from string
            
             if(csumrec == 0x02)
             {// chekc if there is no checksum
              return(1); // western advance without checksum                             
             } // 
             
             
                                 
             if(csumrec == csumcal)// compare sent and calculated for error
              {             
              return(1); // check sums are matching           
              }//  
             else
             {
              return(0); // checksums dont match  
             }
 

} // 

//--------------------------------------------------------------------------------------------------------
// Autoscan string specific for DF-3500 strings
// This function is used when the learn mode was not able to identify the string from the table.
// This function will then parse the string and try and figure out the string on its owm 
//--------------------------------------------------------------------------------------------------------
void autoscan3(void)
{

char i;
char digit[10];
char d;
char ch;
char dec;
char flag;
char j;
//char *ptr;
char spacecount;
char df_total;
char df_total_start;
// scan from left to right in search of numeric values 

dec=0;  // clear decimal postion
d=0; // clear character count
flag=0; // clear flag that indicates that we are parsing a string of numbers
spacecount=0;

//////////////////////////////////////////////////////////////////////////////////////////////////////
// specific start for the DF 3500 command mode special in Ver 4-05
df_total = 'R'; // indicates the start of the Totla part:       0x02 www.www TONNE/HR www.www TONNE
//////////////////////////////////////////////////////////////////////////////////////////////////////
/*
STRING[1]=' ';
STRING[2]=' ';
STRING[3]=' ';
STRING[4]='-';
STRING[5]=' ';
STRING[6]=' ';
STRING[7]=' ';
STRING[8]=' ';
STRING[9]='1';
STRING[10]=' ';
STRING[11]=' ';
STRING[12]=' ';
STRING[13]=' ';
STRING[14]=13;
STRING[15]=0;
*/

for (i=0;i!=10;i++) digit[i]=' '; // fill with spaces
//--------------------------------------------------------------------
// Some strings have individual numbers as status characters (weightronix).
// Loop through and detect any single digit characters that could cause confusion for a weight number.
// If we find a single digit that does not preceed with 2 spaces, or follow with another digit, we blank it out.
// This avoids confusion when extracting the weight from the string
// start analyzing string to see how many numeric characters there are in the string, count them
for (i=1; i!= PARSE.CNT; i++) // loop through length of string
{//12
  if ( STRING[i] == df_total )  // check if value is a numeric digit
       {
     df_total_start = i;
	}
}//12
//                               
//       
//    
//
//
j=0;
for (i=df_total_start;i!=PARSE.CNT;i++) // loop through length of string
{//12
  if ( isdigit(STRING[i]) )  // check if value is a numeric digit
    {
     j++;  // incrememnt digit count
	}
}//12

// 
// start analyzing string for single digit characters
for (i=df_total_start;i!=PARSE.CNT;i++) // loop through length of string
{//11

   if (j < 2) // if there are less than 2 digits in the string, no need to filter single digit characters
    {
     break; // break out of this loop
	}

    if ( isdigit(STRING[i]) )  // check if value is a numeric digit
	   {//1

             if (isdigit(STRING[i+1])) // check if the next character is a digit    
			  {//4
                break;  // exit the loop, we have two digits in a row, most likely a real weight
			  }//4

              	  //////////////////////////////////////////////////////////////
           // added on Aug 20 2010 to fix the 00.00 bug in command mode 
              if  (STRING[i + 1]=='.')// check if value is a numeric digit
			    {
			     break;  // incrememnt digit count
				}

              if (spacecount>1) // check for spaces preceding digit
			{//5
                         break;  // there are 2 spaces or more before the digit, break out of loop
			}//5
                    
               
	       STRING[i]=' '; // remove single digit string, fill in with space
               break; // break out of loop

       }//1

     
   if (STRING[i]==' ') // check for a space
     {//2
	   spacecount++; // we deteced a space, increment counter
	 }//2
	 else
	 {//3
	   spacecount=0; // reset counter
	 }//3
		
} //11 



// now loop through string and search for block of weight numbers

for (i=df_total_start;i!=PARSE.CNT;i++) // loop through length of string
{
 
   ch=STRING[i]; // get a character from the string buffer

    if ( isdigit(ch) )  // check if value is a digit
	   {//1
         digit[d++] = ch; //copy numeric into string
          if (d > 6) break; // can have 7 digits max
	  	   flag=1;
	    }//1
        else 
	     { //2
         if (ch=='.') dec=d; // make note of a decimal position  
                   else
                    if(BOARD.COMMA)
                    { 
                    if (ch==',') dec=d; // make note of a decimal position  
                    }
                       else
			  {
			    if (flag) break; // non numeric character detected, finished reading numeric string
                           }    //

		}//2		
        
// 		
} // end of for loop scanning string


// format decimals *********************************************
if (dec)
BOARD.DEC = d-dec;
else
BOARD.DEC=0;



// check is we have mode that 6 numeric characters, than left shift to bring down to 6

if (d > 5)  // more than 6 characters
{
 for (j=0;j!= d-6;j++)
 for (i=0;i!=8;i++) digit[i]=digit[i+1];  // left shift out leading zero
 d=6;
}




// format weight string ****************************************
for (j=0;j!=6;j++) // right shift string until there are no more spaces
 {
  if (digit[5]==' ')
   {
    for (i=5;i!=0;i--) digit[i]=digit[i-1]; // right shift character by 1
    digit[0]=' '; // fill with space
   }
   else
   break;
  }

for (i=0;i!=6;i++)  BOARD.DIGIT[i]=digit[i]; // copy string over scoreboard output
BOARD.DIGIT[6]=0; // null terminate
extract_weight();   // extract a long weight from the weight string
//digit[6]=0;
//BOARD.VALUE=strtol(digit,&ptr,10);  // convert board digits to long
//search for LB or KG flag *************************************
for (i=df_total_start;i!=PARSE.CNT;i++) // loop through length of string
{//1
    ch=toupper(STRING[i]); // get a character from the string buffer

    if (ch=='K') 
	{
     BOARD.KG=1;
	 BOARD.LB=0;
	 break;
	}
    else
    if (ch=='L') 
	{
     BOARD.KG=0;
	 BOARD.LB=1;
	 break;
	}   

}//1
  // 
  // 
  BOARD.GROSS=1;         // The previous character was not a 'K'
  BOARD.NET=0;

//search for NET or GROSS flag ************************************
for (i=df_total_start;i!=PARSE.CNT;i++) // loop through length of string
{
    ch=toupper(STRING[i]); // get a character from the string buffer
   
    if (ch=='N') 
	{//3
        BOARD.GROSS=0;
	BOARD.NET=1;
	break;
	}//3

} //

//search for sign character ************************************
for (i=df_total_start;i!=PARSE.CNT;i++) // loop through length of string
{
    ch=STRING[i]; // get a character from the string buffer

    if (ch=='-') 
	{
	 BOARD.POL=1;
     break;
	}
	else
    BOARD.POL=0;

} 



 

} // end of function


void parse_traffic(char cmd)
{//
char ch = cmd;
  
  if (BOARD.PGM !=3)
  {
  return;  
  }//

           
                                 if (  ch == '&' )  // stop light red on   & character 38
							   {                                                                           
                                                           stoplight(RED);                                                          
                                                             
							   } // 
							
                                 else if (  ch == '*' )  // stop light green on * character 42
							      {                                                                                      
                                                               stoplight(GREEN);                                                      
							      }							
                           
                                 else if (  ch == '%' )  // blank stoplight 37 
							   {                                                                                 
                                                           stoplight(OFF); //							
                                                           } //
                           
                                 else if (  ch == '$' )  // red blink
							   {                                                  
                                                          stoplight(R_BLINK);
                                                           }        
                                                           
                                 else if (  ch == '^' )  // green blink
							   {                                                   
                                                           stoplight(G_BLINK);                                                        
                                                           }                    
                           
         
}//
 

char parse_command(char cmd)
{//
                            if (  cmd== '&' )  // stop light red on & character
							   {                                                      
                                                         
                                                           stoplight(RED);                                                        
                                                          return(1);
							   }
							   else
                            if (  cmd == '*' )  // stop light green on * character
							   {                                                     
                                                           stoplight(GREEN);         
                                                                         
                                                          return(1);
							   }// 
							   else                            
                         if (  cmd == '%' )  // blank stoplight 
							   {                               
                                                           stoplight(OFF);     
                                                           return(1);
                                 
							   } //
                          
                                                           else
                            if (  cmd == ')' )  // turned blink off for ')' character
							   {
                                                            BOARD.BLINK = 0;
                                                            return(1);
							   }
							   else
                            if (  cmd == '!' )  // ! enable blink for 3 seconds timeout function 
							   {
                                                           BOARD.BLINK = 3;
                                                           return(1);
							   }
                                                          else
                             if (  cmd == '(' )  // turned blink on for '(' character
							   {
                                                          BOARD.BLINK = 255;
                                                           return(1);
							   }       
                          
                                                          else
                            if ( cmd == '{' )  // show time 
							   {
                                                          BOARD.MODE = _COMMAND;   
                                                          BOARD.CYCOMMAND = 3; 
                                                            return(1);
                                                           }
                                                          else
                            if ( cmd == '}' )  // show date 
							   {
                                                          BOARD.MODE = _COMMAND;   
                                                          BOARD.CYCOMMAND = 4;
                                                            return(1);
                                                           }
                                                          else
                            if (  cmd == '[' )  // show temperature
							   {
                                                          BOARD.MODE = _COMMAND;   
                                                          BOARD.CYCOMMAND = 2;
                                                            return(1);
                                                           }
                                                          else
                            if (  cmd == ']' )  // show cycle
							   {
                                                          BOARD.MODE = _COMMAND;   
                                                          BOARD.CYCOMMAND = 1;
                                                            return(1);
                                                           }
                                                           else
                            if (  cmd == '$' )  // red blink
							   {                                                           
                                                          stoplight(R_BLINK);
                                                            return(1);
                                                           }          
                                                           else
                            if (  cmd == '^' )  // green blink
							   {                                                       
                                                       
                                                        stoplight(G_BLINK);
                                                          return(1);
                                                           } 
                                                           else
                             if (  cmd == '+' )  // green blink
							   {                                                           
                                                        BOARD.MIRROR = 0;
                                                          return(1);
                                                           } 
                                                           else
                             if (  cmd == '_' )  // green blink
							   {                                                           
                                                        BOARD.MIRROR = 1;
                                                          return(1);
                                                           } 
                              else
                            if (  cmd == '~' )  // show temperature
							   {
                                                          BOARD.MODE = _COMMAND;   
                                                          BOARD.CYCOMMAND = 5; 
                                                            return(1);
                                                           }
                               else
                            if (  cmd == '<' )  // show temperature
							   {
                                                          BOARD.MODE = _WEIGHT;   
                                                          BOARD.CYCOMMAND = 0;
                                                            return(1);
                                                           }
                                                          else
                            if (  cmd == '>' )  // show cycle
							   {
                                                          BOARD.MODE = _WEIGHT;   
                                                          BOARD.CYCOMMAND = 0;
                                                          return(1);
                                                           }
                            
                  
   
 return(0);  
}// 
