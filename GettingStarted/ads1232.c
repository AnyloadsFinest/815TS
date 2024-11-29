#include "includes.h"

signed long readAD(void)
{
long ad;     // ad converter value
char *byte;  // pointer used to read in data as 8bit blocks, type casted as char

if (FIO0PIN & 0x00000100) // if high then return, there is no data to read
{
//FIO0SET=0x00000040; // setup P0.11 as high
return(0); // MISO pin is high, there is no data to read
}
 //PINSEL0 |=0x000A8000; // ssp1
 PINSEL0 |=0x00028000; // ssp1
//ADC setup          
        //Set to highest speed for SPI at 5mhz  (60mhz/12)
     //   SSP1CR0 = 0x2B87;
     //   SSP1CR1 = 0x02;
     //   SSP1CPSR = 0x02;
ad=0; // clear to zero
byte = (char *)&ad; // casting a pointer to get the address of "ad"
byte=byte + 2; // setup pointer to point to MSB
//while((SSP1SR & 0x01)==0){}
SSP1DR=0xAA;
// Wait for transfer to be completed
while((SSP1SR & 0x04)==0){}
//while((SSP1SR & 0x10)){}
*byte--=SSP1DR;  // read in MSB first 8 bits from AD converter, decrement pointer by 1
//while((SSP1SR & 0x01)==0){}
SSP1DR=0xAA;
// Wait for transfer to be completed
while((SSP1SR & 0x04)==0){}
//byte2=S0SPDR;
*byte--=SSP1DR;  // read in second 8 bits from AD converter, decrement pointer by 1
//while((SSP1SR & 0x01)==0){}
SSP1DR=0xAA;
// Wait for transfer to be completed
while((SSP1SR & 0x04)==0){}
//byte1=S0SPDR;
*byte--=SSP1DR;   // read in third 8 bits from AD converter (LSB)

if (ad&0x00800000) // check is sign bit is set, covert from negative value to positive
 {
   ad=~ad;  // invert the value
   ad=ad&0x00ffffff; // strip off uper two bytes (24bit word)
   ad++;  //add 1
   ad=ad*-1; // set to negative
 }

ad = (long) fir_filter(ad);
ad=ad >> 4; // right shift converter to get rid of noisy LSB bit's, only interested in 20 of 24bits.
// 24bit AD data has been read in, ad now holds an updated AD value
PINSEL0 &= 0xFFFC3FFF; // disable SPI from PIN connect block
FIO0SET=0x00000080; // manually pulse SPI clock to reset ready pin
FIO0CLR=0x00000080;
// scale off set calibration 
if (SYSTEM.AD_ZERO_CALIBRATE == true)  // check if flag is set
{
// clock one dummy clock to initiate offset calibration
FIO0SET=0x00000080; // manually pulse SPI clock to reset ready pin
FIO0CLR=0x00000080;  
SYSTEM.AD_ZERO_CALIBRATE = false;
}
SCALE.AD_COUNT++;  // free running ad converter sample count, used for IZSM and ZSM
return(ad);
} // end of function
//----------------------------------------------------------------
// initialize the ads1232 converter
//
//----------------------------------------------------------------
void init_ads1232()
{


        PINSEL0 |=0x00028000; // ssp1
        //ADC setup          
        //Set to highest speed for SPI at 5mhz  (60mhz/12)
        SSP1CR0 =  0x2B87;
        SSP1CR1 =  0x02;
        SSP1CPSR = 0x02;        
        // Configure Pin Connect Block
        PINSEL0 &= 0xFFFC3FFF; // disable SPI from PIN connect block
// Configure Pin Connect Block
FIO0CLR=0x00000080; // make sure SCLK is parked low to prevent converter from going into sleep mode          
FIO0CLR=0x00000040; // setup P0.11 as low     pulse reset
DelayResolution100us(1);
FIO0SET=0x00000040; // setup P0.11 as high       
} // end of function

//-------------------------------------------------------------------------
// ADS1232 low level calibration
//-------------------------------------------------------------------------
void ADoffset_calibration(void)
{
SYSTEM.AD_ZERO_CALIBRATE =true;  // set flag to zerocalibrate 

} //end of function




