#include "includes.h"



//------------------------------------------------------------------------------------------
// perform platter tare
//------------------------------------------------------------------------------------------

// void platter_tare(void)
// {
  
//   // check for scale motion or scale over
//   if ((SCALE.MOTION) || (SCALE.OVER) )  // cannot tare on scale over or motion
//    {

//     display_message( "--E2--",2);  // display error 2
        
//     return;  // abort
//    }

//    //check for negative weight (not allowed)
//   if (SCALE.GROSSWEIGHT < -0.5)   // if floatingpoint weight less than zero, cannot tare
//   {
 
//        display_message( "--E2--",2);  // display error 2
//     return; //abort
//   }

//  //check for zero (not allowed)
//   if (SCALE.DIS_GROSSWEIGHT ==0)  // if grossweight is zero, cannot  tare
//   {
//      display_message( "--E2--",2);  // display error 2
//      return; //abort
//   }
 
  

//   switch (CAL.REGULA)  // tare the scale depending on which regulatory setting is set
//   {

        
//     case 1: {  //CANADA  allow tare to only if the previsouse tare weight has been cleared
//                if (SCALE.NETMODE==0) // check if cleared
//                {
//                 SCALE.TAREWEIGHT = SCALE.GROSSWEIGHT;  // tare the scale
//                 SCALE.NETMODE=1;  // set the netmode flag
//                 SCALE.TAREMODE=1;  // set the netmode flag
//                }

//              break;
//              }

    
//      case 3:    //NTEP  allow tare to be aquired even if there is a tare weight already stored         
//     case 2:    //NTEP  allow tare to be aquired even if there is a tare weight already stored
//     case 0: {  //NONE

//               SCALE.TAREWEIGHT = SCALE.GROSSWEIGHT;  // tare the scale
//               SCALE.NETMODE=1;  // set the netmode flag
//               SCALE.TAREMODE=1;  // set the netmode flag
//              break;
//              }



//   }  // end of switch


void platter_tare(void)
{
  
  // check for scale motion or scale over
  if ((SCALE.MOTION) || (SCALE.OVER) )  // cannot tare on scale over or motion
   {

    display_message( " ERROR",2);  // display error 2
        
    return;  // abort
   }

   //check for negative weight (not allowed)
  if (SCALE.GROSSWEIGHT < -0.5)   // if floatingpoint weight less than zero, cannot tare
  {
 
    display_message( " ERROR",2);  // display error 2
    return; //abort
  }

 //check for zero (not allowed)
  if (SCALE.DIS_GROSSWEIGHT ==0)  // if grossweight is zero, cannot  tare
  {
    display_message( " ERROR",2);  // display error 2
     return; //abort
  }
 
  

  switch (CAL.REGULA)  // tare the scale depending on which regulatory setting is set
  {

    case 0: {  //NONE




        if (SCALE.NETMODE==0){
              SCALE.TAREWEIGHT = SCALE.GROSSWEIGHT;  // tare the scale
              SCALE.NETMODE=1;  // set the netmode flag
              SCALE.TAREMODE=1;  // set the netmode flag
             break;
             }

        else {
          display_message( " TARED",2);  // display error 2
        return; //abort
        }
    }



  }  // end of switch

} // end of function

 //-----------------------------------------------------------------------------
 //   
 //-----------------------------------------------------------------------------
 void clear_tare(void)
 {

  switch (CAL.REGULA)  // clear the scale depending on which regulatory setting is set
  {


    case 0: {  //NONE
    
                SCALE.TAREWEIGHT = 0;
                SCALE.NETMODE=0;  // clear the netmode flag
                SCALE.TAREMODE=0;  // clear the netmode flag
                SCALE.DIS_TAREWEIGHT=0;
                SCALE.DIS_NETWEIGHT=0;

             break;
             }



  }  // end of switch


 } // end of function


//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
void auto_tare(void)
{

 static char tare_state;  // state machine variable


  if (SYSTEM.CAL)  // if in calibration mode, return
  {
   return;
  }



 if (CAL.AUTOTARE) // if auto tare mode enabled
 {//0


    switch(tare_state)  // statemachine to detect if we can auto tare or not
    {
      // 

      case 0: {  // waiting for conditions to tare the scale

                 if (SCALE.NETMODE==0) // check if we are not in net mode
                 {//1
                      if (SCALE.GRADWEIGHT > 4)  // if the gradweight is greater than tripweight
                        {//2
                          if (SCALE.MOTION==0)  // and no motion
                         {//3
                           platter_tare();  // tare the scale
                           tare_state=1;    // update state condition
                         }//3
                        }//2
                 }//1

               break;
              }


      case 1: {  // scale has been auto tared, weight for weight to go below threshold and re-arm

                // this state condition prevents the scale auto taring a new tare weight ig the tare weight is cleared manually
                if (SCALE.GRADWEIGHT < 4)  // if the gradweight is less than tripweight
                {//22

                  tare_state=0;    // update state condition


                }//22

               break;
              }



  } // end of switch


 }//0






 if (CAL.AUTOCLEAR) // if auto clear enabled
 {//01

  if (SCALE.NETMODE) // check if we are in net mode
   {//1
     if (SCALE.GRADWEIGHT < 2)  // if the gradweight is less than than 2
     {//2

       if (SCALE.MOTION==0)  // and no motion
       {//3
        clear_tare();  // tare the scale
       }//3

     }//2
    }//1

 }//01




 }// end of function













