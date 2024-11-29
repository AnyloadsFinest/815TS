
#include "includes.h"


//-----------------------------------------------------------------------
// read port 1/0, and scan for keypresses
// keys have dual functions, this function returns the correct function
// based on the mode of operation of the indicator
// keys are not debounced here
//-----------------------------------------------------------------------

//

//PRINT    p1.9
//UNITS    p1.0
//TARE     p1.1   (note tare is on port 0 for ISP activation for in circuit programming)
//CLEAR    p1.4
//ZERO     p1.8



int get_key(void)
{

long keyscan; // scan port 1
//long keyscanb;// scan port 0
// scan port 0 and port 1 for keypresses
keyscan = FIO1PIN;  // read IO from port1
//keyscan = keyscan & 0x00fb0000;  // bitmask out all but the keypad bits
keyscan = keyscan & 0x00000313;  // bitmask out all but the keypad bits
//keyscanb = FIO0PIN;  // read IO from port0
//keyscanb = keyscanb & 0x00004000;  // bitmask out all but the bit 14 (tare key) bits

//keyscan=keyscan | keyscanb;  // logical OR port 0 and 1 together

if ( keyscan == 0x00000313 ) // check for no key
  {
  // FIO3SET = 0x02000000; // disable output
   return(0);
  }

   

switch (keyscan)
       {

                case 0x00000113:
                  {   //  PRINT
                      return(_PRINT);
                   }

                 case 0x00000312:
                   {   // UNITS

                    return(_UNITS);
                   
                   }


                 case 0x00000211:
                   {   // TARE & ZERO are pressed

                    return(_TAREZERO);
                   }


                

                 case 0x00000303:
                   {   // CLEAR
                     return (_CLEAR);
                 
                   }


                 case 0x00000213:
                   {   //ZERO

                     return(_ZERO);
                   
                   }

                 case 0x00000311:
                   {   // TARE
                      return (_TARE);

                   }

                 case 0x00000111:
                   {   // TAREPRINT
                      return (_TAREPRINT);

                   }

          

                   default:
                     {
                      return(0);

                     }

  } // end of switch










} // end of function











