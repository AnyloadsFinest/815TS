#include "includes.h"


// this module converts between different scale using using units and grad size the scale was calibrated in.
//--------------------------------------------------------------------------
// convert pounds to kilos
//
// This fuction takes a
//
//
//--------------------------------------------------------------------------

long  display_kilos(float weight)
{

float unitweight;
long dweight;

double fraction;
double integer;

char negative=0;

// when converting from pounds to kilos, the decimal position may shift to the left, or to the right
SCALE.DIS_DEC=CAL.DEC;  // copy the number of decimal places used in calibration
// copy calibration deadzero over to displayed deadzero. When converting to kilos the deadzeros can be shifted
SCALE.DIS_DEADZERO=CAL.DEADZERO;

//check if we are already in Kilos or Grams
// if we are displaying Kgs or Grams and calibrated in Kgs then no convertion is required
if  (CAL.UNITS==_KG) // is the calibrated units in kgs
{//1
  
   // weight value is stripped of its sign bit
    if (weight <0) // check if value is negative
     {//2
	weight=fabs(weight);  //take absolute value of weight removing the negative sign bit
	negative=1;           //set negative flag
     }//2

   dweight=(long) weight;   //typecast as integer strips integer value from float
   fraction=weight-dweight; //get the remaining fractional value from float

   //fraction now holds the fractional part of the weight

   if (fraction> 0.5) dweight++;  //if the fractional value is > 0.5 then round up the integer value
   // dweight now holds a rounded integer value of the weight

   if (negative) dweight=dweight*-1;
   dweight=dweight*CAL.GRAD;
   SCALE.DIS_GRAD = CAL.GRAD; // current displayed gradize is the same as in calibration

   SCALE.DIS_WEIGHT=dweight;

   return(dweight); // units are the same, no further conversion required

}//1

weight=weight*CAL.GRAD;   // multiply float with grad size

//we are in pounds and want to convert the display in Kilos

unitweight=weight*0.45359237; // convert  pounds to Kilos

// unitweight holds kg value coverted from lbs, but be rounded to apropriate grad size.
// roundings to depends on grad size of what the scale is calibrated to





// make final calculation depending on what the current displayed graduation in pounds
//
//    1 Lbs grads to  0.5 grads Kilos
//    2 Lbs grads to  1   grads Kilos
//    5 Lbs grads to  2   grads Kilos



switch(CAL.GRAD)  // convert to pounds using the gradsize that coreesponds to the grad size used in kilos
{


            // special condition, convert 1lb to 0.5kg incrememnts
case 1:  {  // weight must be displayed in 0.5 grad steps, must shift decimal place 1 to the left

	 unitweight=unitweight*10; // shift decimal (float) to the left by 1

	 unitweight=unitweight/5;  // divide by 5 grads


         // seperate the integer result from the fractional remainder

	 fraction=modf(unitweight,&integer); //seperate fraction from integer

	 // we now determine rounding to the nearest 0.5 grad from the fraction


         // multiply by 5 grads to bring the integer back to 5 graduation incrememnts
	 integer=integer*5; // integer now holds weight in 5Kg graduation

         // now check the fractional remainder to see if we need to round up by 1 grad

	 if (fraction>=0.5) integer=integer+5;   // round up to next graduation if fraction > 0.5


	 dweight=(long) integer; // dweight now holds current weight without decimals or sign


	 SCALE.DIS_DEC++;   // shift left decimal place
	 SCALE.DIS_GRAD=5;  // displayed gradsize is 5

         // special condition below!    -----------------------------------------------

	 // if we are in 10 lb graduations converting to 5kg grads
	 // then remove decimal place
	 // HB44 does not allow dead zeroes after the decimal place

		if (CAL.DEADZERO>0) // check if we have deadzeros (10,20,50,100)
		{//1
		 SCALE.DIS_DEC--; //  shift decimal place to the right


		// for 100 grad size to 50kg grad size, must remove a deadzero
		if  (CAL.DEADZERO==2) // check if we have 2 deadzeroes
		 {//2
		  SCALE.DIS_DEADZERO=1; //  remove a dead zero
		 }//2
		else // we are converting 10 grad to 5kg size
		  SCALE.DIS_DEADZERO=0;

		}//1

	 break;

	 }


case 2:  { //weight must be displayed in 1 grad steps

	 fraction=modf(unitweight,&integer); //seperate fraction from integer
	 if ( fraction >= 0.5 ) integer ++; // round integer based on fraction

	 dweight=(long)integer; // final weight without decimal or sign


	 SCALE.DIS_GRAD=1; // displayed gradsize is 1
	 break;
	 }


case 5:  { // weight must be diplayed in 2 grad steps


	 unitweight=unitweight/2; // divide by 2 grads

	 fraction=modf(unitweight,&integer); //seperate fraction from integer

	 integer=integer*2; // multiply by 2 graduations

	 if (fraction>=0.5) integer=integer+2; // round up 2 grad steps

	 dweight= (long)integer; // final weight without decimal or sign

	 SCALE.DIS_GRAD=2; // displayed gradsize is 2

	 break;
	 }

    } // end of switch


SCALE.DIS_WEIGHT=dweight;

return(dweight);


} // end of function display_kilos()




//--------------------------------------------------------------------------
// Display the weight as pounds
// Current weight in Kilos is passed to the function.

// Weight passed is integer weight.
// Weight does not contain any decimals or sign information.
// This function may change the decimal position of the display through scale.decimal.
// The new weight value is returned, converted to pounds.
// The returned converted weight is formatted to its correct corresponding
// graduation range ie: 1,2 or 5 grad steps.

// If the internal weight is aready in pounds then this function does nothing
// returning the weight that was passed to it.
//--------------------------------------------------------------------------
long display_pounds(float weight)
{

float unitweight;
float fraction;
long dweight;
double integer;
float i;
char negative=0;

SCALE.DIS_DEC=CAL.DEC; // reset decimal position from cal value
SCALE.DIS_DEADZERO=CAL.DEADZERO;  // reset number of dead zeroes from calibration

//check if the displayed units are the same as the calibrated units
//if they are the same we do not have to do a unit convertion
if (CAL.UNITS== _LB)
{

// weight value is stripped of its sign bit
if (weight <0) // check if value is negative
 {
	weight=fabs(weight);  //take absolute value of weight removing the negative sign bit
	negative=1;           //set negative flag
	 }


dweight=(long) weight;   //typecast as integer strips integer value from float
fraction=weight-dweight; //get the remaining fractional value from float

//fraction now holds the fractional part of the weight

if (fraction> 0.5) dweight++;  //if the fractional value is > 0.5 then round up the integer value
// dweight now holds a rounded integer value of the weight

if (negative) dweight=dweight*-1;


dweight=dweight*CAL.GRAD;
							 // displayed units are the same as calibrated
				 // no converstion is required, but we must round to a long
SCALE.DIS_GRAD = CAL.GRAD; // current displayed gradize as in calibration

return(dweight); //return the final weight without decimals
 }



weight=weight*CAL.GRAD;

//otherwise...
//we are in kilos and want to convert the display to pounds
//perform a convertion and round to the nearest grad size


unitweight=weight/0.45359237;  // convert Kilos to pounds


//unitweight now holds the converted weight as as fractional float value.
//we must convert to a long integer and round it up to the nearest grad.


//dweight now holds the weight in pounds rounded to the nearest integer


// cal.grad holds the graduation step calibrated in Kilos
// we now determine which graduation is used and rounds the converted weight
// to the nearest corresponding value



// make final calculation depending on what the current displayed graduation in pounds
//
//    0.5 Kg grads to  1 grads pounds
//    1   Kg grads to  2 grads pounds
//    2   Kg grads to  5 grads pounds
//


switch(CAL.GRAD)
		{
case 1:  {  // we are displaying in 1 grad intervals in KILOs, convert to 2 grad in pounds

	 i=unitweight/2; // divide by 2 grads

	 fraction=modf(i,&integer); //seperate fraction from integer
	 integer=integer*2; // multiply integer by 2 grads

	 // if the fraction is 0.5 then round weight up to next graduation (2 pound step)
	 if (fraction>0.5) integer=integer+2;

	 dweight=(long)integer;  // dweight now holds current weight in pounds
	 SCALE.DIS_GRAD= 2; // displayed grads is 2
	 break;
	 }


case 2:  {// we are displaying in 2 grad intervals in KILOs, convert to 5 grad in pounds

	 i=unitweight/5; // divide by 5 grads
	 fraction=modf(i,&integer); //seperate fraction from integer
	 integer=integer*5; // multiply integer by 5 grads

	 // if the fraction is =>0.5 then round weight up to next graduation (2 pound step)
	 if (fraction>=0.5) integer=integer+5; //round up to next graduation

	 dweight= (long) integer;  // dweight now holds current weight in pounds
	 SCALE.DIS_GRAD= 5; // displayed grads is 5

	 break;
	 }


case 5:  {// we are displaying in 5 grad intervals in KILOs, convert to 10 grad pounds

	 i=unitweight/10; // divide by 10 grads
	 fraction=modf(i,&integer); //seperate fraction from integer
	 integer=integer*10; // multiply integer by 10 grads

	 // if the fraction is =>0.5 then round weight up to next graduation (2 pound step)
	 if (fraction>=0.5) integer=integer+10;

	 dweight=(long)integer;  // dweight now holds current weight in pounds

	 // Because we are now displaying as 10 grad intervals, there
	 // will be a dead zero after the decimal place.

	 // If decimals are used, then shift the decimal to the right,
	 // and remove the dead zero. This is only when switching from KGs to LBs.
	 // This is a HB44 requirment

	 if (CAL.DEC>0) // check if we have decimals
		{
		 SCALE.DIS_DEC = CAL.DEC-1; //  shift decimal place to the right
			dweight=dweight/10; // divide by 10 to remove dead zero.
			 }

	 SCALE.DIS_GRAD= 1; // displayed grads is 1



	 break;
	 }

		} // end of switch



return(dweight);

}  // end of funtion display_pounds()





//----------------------------------------------------------------------------------------------------
// Display the weight as ounces
// ounces are converted from pounds by multiplying by 16
//----------------------------------------------------------------------------------------------------

long display_ounce(float weight)
{


float unitweight;
float fraction;
long dweight;
double integer;
float i;
//char negative=0;
char j;



 if (CAL.UNITS==_LB) // check if we are calibrated in pounds
 {//1
   weight=weight*CAL.GRAD;  // calculate the weight in pounds
   unitweight=weight*16;  // convert to ounce

   SCALE.DIS_DEC=CAL.DEC;  // copy over the number of decimals set in calibration
   switch (CAL.GRAD)
   {

   case 1:   {  // 1 lb grad size to 2 oz grad size

             i=unitweight/2; // divide by 2 grads
             i=i/10; // left shift decimal position

 
	     fraction=modf(i,&integer); //seperate fraction from integer
	     integer=integer*2; // multiply integer by 2 grads

	     // if the fraction is 0.5 then round weight up to next graduation (2 pound step)
	     if (fraction>0.5) integer=integer+2;

	     dweight=(long)integer;  // dweight now holds current weight in oz

               if (SCALE.DIS_DEC==0)
               {
               dweight=dweight*10;
               }
                else
                 {
                  SCALE.DIS_DEC--; // move decimal position
                 }

	     SCALE.DIS_GRAD= 2; // displayed grads is 2;
             break;
            }

    case 2:  {  //2 lb grad size to 5oz grad size

             i=unitweight/5; // divide by 5 grads
             i=i/10; // left shift decimal position

	     fraction=modf(i,&integer); //seperate fraction from integer
             integer=integer*5; // multiply integer by 5 grads
	
	     // if the fraction is 0.5 then round weight up to next graduation (2 pound step)
	     if (fraction>0.5) integer=integer+5;

	     dweight=(long)integer;  // dweight now holds current weight in oz


             if (SCALE.DIS_DEC==0)
             {
               dweight=dweight*10;
             }
             else
             {
               SCALE.DIS_DEC--; // move decimal position
             }



	     SCALE.DIS_GRAD= 5; // displayed grads is 5;

             break;
            }

  case 5:  {  // 5 lb grad size to 1 oz grad size
             i=unitweight;
             i=i/100;  //left shift decimal 2 positions

	     fraction=modf(i,&integer); //seperate fraction from integer

	
	     // if the fraction is 0.5 then round weight up to next graduation (2 pound step)
	     if (fraction>0.5) integer=integer+1;

	     dweight=(long)integer;  // dweight now holds current weight in oz



             for (j=0;j!=2;j++)  // left shift decimal twice
             {

               if (SCALE.DIS_DEC==0)
               {
                 dweight=dweight*10;
               }
               else
               {
               SCALE.DIS_DEC--; // move decimal position
               }
             }


	      SCALE.DIS_GRAD= 1; // displayed grads is 1;

           break;
           }





  } // end of switch


 }//1

 else  // we are calibrated in kilograms

 {//2

   weight=weight*CAL.GRAD;  // calculate the weight in pounds
   unitweight=weight*35.2739619;  // convert to ounce

   SCALE.DIS_DEC=CAL.DEC;  // copy over the number of decimals set in calibration



   switch (CAL.GRAD)
   {

     case 1:   {  // 1 kg grad size to 5 oz grad size

             i=unitweight/5; // divide by 5 grads
             i=i/10; // left shift decimal position


	     fraction=modf(i,&integer); //seperate fraction from integer
	     integer=integer*5; // multiply integer by 5 grads

	     // if the fraction is 0.5 then round weight up to next graduation
	     if (fraction>0.5) integer=integer+5;

	     dweight=(long)integer;  // dweight now holds current weight in oz


               if (SCALE.DIS_DEC==0)
               {
               dweight=dweight*10;
               }
                else
                 {
                  SCALE.DIS_DEC--; // move decimal position
                 }



	     SCALE.DIS_GRAD= 5; // displayed grads is 5;


             break;
            }



    case 2:  {  //2 kg grad size to 1oz grad size

             i=unitweight;
             i=i/100; // left shift decimal 2 positions

	     fraction=modf(i,&integer); //seperate fraction from integer


	
	     // if the fraction is 0.5 then round weight up to next graduation (2 pound step)
	     if (fraction>0.5) integer=integer+1;

	     dweight=(long)integer;  // dweight now holds current weight in oz


            for (j=0;j!=2;j++)
            {
             if (SCALE.DIS_DEC==0)
             {
               dweight=dweight*10;
             }
             else
             {
               SCALE.DIS_DEC--; // move decimal position
             }
            }


	     SCALE.DIS_GRAD= 1; // displayed grads is 5;


             break;
            }






  case 5:  {  // 5 kg grad size to 2 oz grad size

             i=unitweight/2; // divide by 2 grads
             i=i/100;  //left shift decimal

	     fraction=modf(i,&integer); //seperate fraction from integer
             integer=integer*2; // multiply integer by 2 grads
	
	     // if the fraction is 0.5 then round weight up to next graduation (2 pound step)
	     if (fraction>0.5) integer=integer+2;

	     dweight= (long)integer;  // dweight now holds current weight in oz




             for (j=0;j!=2;j++)
              {
               if (SCALE.DIS_DEC==0)
               {
                 dweight=dweight*10;
               }
               else
               {
               SCALE.DIS_DEC--; // move decimal position
               }
              }

	      SCALE.DIS_GRAD= 1; // displayed grads is 1;

           break;
           }

  } // end of switch

 }//2

return(dweight);

} // end of function

//----------------------------------------------------------------------------------------------------
// Display the weight as grams
// gram are calculated from kilos then multiplied by 
//----------------------------------------------------------------------------------------------------

long display_grams(float weight)
{

long kg;
int i;
int dec;
long grams;
long mult;

              // first get the weight as kilos
        kg =  display_kilos(weight);  // format output for kilograms
             
        
        // calculate multiplier to covert kgs to grams
        // the multiplier depends on how many decimals are set              
            dec=SCALE.DIS_DEC; // make a copy of decimals
              
              mult=1000;  // initialize multiplier to 1000
            
              
              for (i=0;i!=SCALE.DIS_DEC;i++)  // loop through depending how many deciomals are set in the kg display
              {
                mult=mult/10;  // reduce multiplyer by one decimal
              }

              
              if (dec==4) // check for special condition 
              {
               dec=1;     // we will be displaying 000.5 grams
               mult=1;    // no decimal shifting
              }
              else
              {
               dec=0;   // else we are stripping any decimals for grams display
              }              
              grams=kg * mult;  //  multiplier to convert to grams                     
              SCALE.DIS_GRAD=SCALE.DIS_GRAD*mult;              
              SCALE.DIS_DEC=dec;  // set any decimals to zero              
              return(grams);

} // end of function

//----------------------------------------------------------------------------------------------------
// Display the weight as grams
// gram are calculated from kilos then multiplied by 
//----------------------------------------------------------------------------------------------------

long display_ton(float weight)
{
long kg;  
//int i;
char dec;
//long ston;
//float ton;
long mult;

            
            // first get the weight as kilos
             if (CAL.DEC !=0)
              {            
              kg =  display_kilos(weight);  // format output for kilograms 
              SCALE.DIS_UNITS = _KG;
              return(kg);  
              }
                                  
             //weight = weight * 0.984207;// long ton             
           //  weight = weight * 1.1023113; // short ton
             kg =  display_kilos(weight);  // format output for kilograms          
            
        
           if ( SCALE.DIS_DEADZERO ==0)
              {              
              mult = 1;                       
              dec = 3;  // set any decimals to zero
           
              }else               
              if (  SCALE.DIS_DEADZERO ==  1) // check for special condition 
              {
              mult = 10;                       
              dec = 2;  // set any decimals to zer             
           
              }
             else
             if (   SCALE.DIS_DEADZERO  ==  2) // check for special condition 
              {
              mult = 100;                       
              dec = 1;  // set any decimals to zero
              }         
         
             if (SCALE.DIS_DEC !=0)
              {                                
              dec++;  // set any decimals to zero 
              } 
                          
              //SCALE.DIS_GRAD = SCALE.DIS_GRAD / mult;      
              SCALE.DIS_DEC = dec;  // set any decimals to zero
              SCALE.DIS_DEADZERO = 0; //  remove a dead zero
              return(kg);
              
              
} // end of function



//----------------------------------------------------------------------------------------------------
// Display the weight as grams
// gram are calculated from kilos then multiplied by 
//----------------------------------------------------------------------------------------------------

long display_short_ton(float weight)
{

long kg;  
//int i;
char dec;
//long ston;
//float ton;
long mult;

            
            // first get the weight as kilos
             if (CAL.DEC !=0)
              {            
              kg =  display_kilos(weight);  // format output for kilograms 
              SCALE.DIS_UNITS = _KG;
              return(kg);  
              }
                                  
             //weight = weight * 0.984207;// long ton             
             weight = weight * 1.1023113; // short ton
             kg =  display_kilos(weight);  // format output for kilograms          
            
        
           if ( SCALE.DIS_DEADZERO ==0)
              {              
              mult = 1;                       
              dec = 3;  // set any decimals to zero
           
              }else               
              if (  SCALE.DIS_DEADZERO ==  1) // check for special condition 
              {
              mult = 10;                       
              dec = 2;  // set any decimals to zer             
           
              }
             else
             if (   SCALE.DIS_DEADZERO  ==  2) // check for special condition 
              {
              mult = 100;                       
              dec = 1;  // set any decimals to zero
              }         
         
             if (SCALE.DIS_DEC !=0)
              {                                
              dec++;  // set any decimals to zero 
              } 
                    
              // 
                      
              //SCALE.DIS_GRAD = SCALE.DIS_GRAD / mult;      
              SCALE.DIS_DEC = dec;  // set any decimals to zero
              SCALE.DIS_DEADZERO = 0; //  remove a dead zero
              return(kg);
              


/*  
  
  float unitweight;
long dweight;

double fraction;
double integer;

char negative=0;

   weight = weight * 1.10231; // short ton
// when converting from pounds to kilos, the decimal position may shift to the left, or to the right
//SCALE.DIS_DEC=CAL.DEC;  // copy the number of decimal places used in calibration
// copy calibration deadzero over to displayed deadzero. When converting to kilos the deadzeros can be shifted
SCALE.DIS_DEADZERO=CAL.DEADZERO;
// SCALE.DIS_DEC = 3;   
             
            if (  CAL.DEADZERO==  2) // check for special condition 
              {
              
              SCALE.DIS_DEC =1;            
              }
             else
             if (  CAL.DEADZERO==  1) // check for special condition 
              {
                                 
              SCALE.DIS_DEC = 2;
              }
             else
             {
              SCALE.DIS_DEC = 3;  
             }
             
 //SCALE.DIS_DEADZERO=0;       
//check if we are already in Kilos or Grams
// if we are displaying Kgs or Grams and calibrated in Kgs then no convertion is required
if  (CAL.UNITS==_KG) // is the calibrated units in kgs
{//1
   // weight value is stripped of its sign bit
    if (weight <0) // check if value is negative
     {//2
	weight=fabs(weight);  //take absolute value of weight removing the negative sign bit
	negative=1;           //set negative flag
     }//2

   dweight=(long) weight;   //typecast as integer strips integer value from float
   fraction=weight-dweight; //get the remaining fractional value from float

   //fraction now holds the fractional part of the weight

   if (fraction> 0.5) dweight++;  //if the fractional value is > 0.5 then round up the integer value
   // dweight now holds a rounded integer value of the weight

   if (negative) dweight=dweight*-1;
   dweight=dweight*CAL.GRAD;
   SCALE.DIS_GRAD = CAL.GRAD; // current displayed gradize is the same as in calibration

   SCALE.DIS_WEIGHT=dweight;

   return(dweight); // units are the same, no further conversion required

}//1

weight=weight*CAL.GRAD;   // multiply float with grad size

//we are in pounds and want to convert the display in Kilos

unitweight=weight*0.45359237; // convert  pounds to Kilos

// unitweight holds kg value coverted from lbs, but be rounded to apropriate grad size.
// roundings to depends on grad size of what the scale is calibrated to





// make final calculation depending on what the current displayed graduation in pounds
//
//    1 Lbs grads to  0.5 grads Kilos
//    2 Lbs grads to  1   grads Kilos
//    5 Lbs grads to  2   grads Kilos
//  SCALE.DIS_DEC ++;


switch(CAL.GRAD)  // convert to pounds using the gradsize that coreesponds to the grad size used in kilos
{


            // special condition, convert 1lb to 0.5kg incrememnts
case 1:  {  // weight must be displayed in 0.5 grad steps, must shift decimal place 1 to the left

	 unitweight=unitweight*10; // shift decimal (float) to the left by 1

	 unitweight=unitweight/5;  // divide by 5 grads


         // seperate the integer result from the fractional remainder

	 fraction=modf(unitweight,&integer); //seperate fraction from integer

	 // we now determine rounding to the nearest 0.5 grad from the fraction


         // multiply by 5 grads to bring the integer back to 5 graduation incrememnts
	 integer=integer*5; // integer now holds weight in 5Kg graduation

         // now check the fractional remainder to see if we need to round up by 1 grad

	 if (fraction>=0.5) integer=integer+5;   // round up to next graduation if fraction > 0.5


	 dweight=(long) integer; // dweight now holds current weight without decimals or sign


	 SCALE.DIS_DEC++;   // shift left decimal place
	 SCALE.DIS_GRAD=5;  // displayed gradsize is 5
         // special condition below!    -----------------------------------------------

	 // if we are in 10 lb graduations converting to 5kg grads
	 // then remove decimal place
	 // HB44 does not allow dead zeroes after the decimal place

		if (CAL.DEADZERO>0) // check if we have deadzeros (10,20,50,100)
		{//1
		 SCALE.DIS_DEC--; //  shift decimal place to the right


		// for 100 grad size to 50kg grad size, must remove a deadzero
		if  (CAL.DEADZERO==2) // check if we have 2 deadzeroes
		 {//2
		  SCALE.DIS_DEADZERO=1; //  remove a dead zero
		 }//2
		else // we are converting 10 grad to 5kg size
		  SCALE.DIS_DEADZERO=0;

		}//1

	 break;

	 }


case 2:  { //weight must be displayed in 1 grad steps

	 fraction=modf(unitweight,&integer); //seperate fraction from integer
	 if ( fraction >= 0.5 ) integer ++; // round integer based on fraction

	 dweight=(long)integer; // final weight without decimal or sign


	 SCALE.DIS_GRAD=1; // displayed gradsize is 1
	 break;
	 }


case 5:  { // weight must be diplayed in 2 grad steps


	 unitweight=unitweight/2; // divide by 2 grads

	 fraction=modf(unitweight,&integer); //seperate fraction from integer

	 integer=integer*2; // multiply by 2 graduations

	 if (fraction>=0.5) integer=integer+2; // round up 2 grad steps

	 dweight= (long)integer; // final weight without decimal or sign

	 SCALE.DIS_GRAD=2; // displayed gradsize is 2

	 break;
	 }

    } // end of switch


SCALE.DIS_WEIGHT=dweight;

return(dweight);

*/
  


} // end of function










//----------------------------------------------------------------------------------------------------
// Display the weight as grams
// gram are calculated from kilos then multiplied by 
//----------------------------------------------------------------------------------------------------

long display_long_ton(float weight)
{

long kg;  
//int i;
char dec;
//long ston;
//float ton;
long mult;

            
            // first get the weight as kilos
             if (CAL.DEC !=0)
              {            
              kg =  display_kilos(weight);  // format output for kilograms 
              SCALE.DIS_UNITS = _KG;
              return(kg);  
              }
                                  
             weight = weight * 0.984207;// long ton             
             //weight = weight * 1.1023113; // short ton
             kg =  display_kilos(weight);  // format output for kilograms          
            
        
           if ( SCALE.DIS_DEADZERO ==0)
              {              
              mult = 1;                       
              dec = 3;  // set any decimals to zero
           
              }else               
              if (  SCALE.DIS_DEADZERO ==  1) // check for special condition 
              {
              mult = 10;                       
              dec = 2;  // set any decimals to zer             
           
              }
             else
             if (   SCALE.DIS_DEADZERO  ==  2) // check for special condition 
              {
              mult = 100;                       
              dec = 1;  // set any decimals to zero
              }         
         
             if (SCALE.DIS_DEC !=0)
              {                                
              dec++;  // set any decimals to zero 
              } 
                          
              //SCALE.DIS_GRAD = SCALE.DIS_GRAD / mult;      
              SCALE.DIS_DEC = dec;  // set any decimals to zero
              SCALE.DIS_DEADZERO = 0; //  remove a dead zero
              return(kg);
              
}












































































































































































































































































