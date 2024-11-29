#include "includes.h"
//---------------------------------------------------------------------------------
// read AD converter
// We are reading a 24 bit word from the ads1232 converter through the SPI
// The converter shifts out data MSB first.
// Data is read in 3 blocks of 8 bits each
//---------------------------------------------------------------------------------
signed long readAD(void)
{
long ad;     // ad converter value
char *byte;  // pointer used to read in data as 8bit blocks, type casted as char
//char bytei [3];
// Configure Pin Connect Block to disable the SPI and use the MISO pin as I/O input
//PINSEL0=PINSEL0&0xfffffaff; // disable SPI from PIN connect block, set as I/O
// Configure Pin Connect Block
// Configure Pin Connect Block
//read MISO pin as an I/O, if MISO is low then the AD has data to read
//PINSEL0 &= 0xFFFF0FFF; // disable SPI from PIN connect block  
//FIO0SET=0x00000040; // setup P0.11 as high

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
// pulse manually one SPI clock to reset ADS1232 status flag
// the SPI must be disconnected from the I/O to allow us to manually toggle IO
// Configure Pin Connect Block
// clock one dummy clock to reset SPI status
 //the ad value is a two's compliment value, check if we have a negative weight
if (ad&0x00800000) // check is sign bit is set, covert from negative value to positive
 {
   ad=~ad;  // invert the value
   ad=ad&0x00ffffff; // strip off uper two bytes (24bit word)
   ad++;  //add 1
   ad=ad*-1; // set to negative
 }


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

// P0.4  SCLK
// P0.5  MISO
// P0.6  SPEED
// P1.25 *ADRESET

// P0.6 *ADRESET  
// P0.7  SCLK
// P0.8  MISO
// P0.9  MOSI 
        //PINSEL0 &= 0xFFC03FFF; // disable SPI from PIN connect block
        // Configure Pin Connect Block for MOSI and SCLK connection     
        // Port 0.15 SPI SCK, port0.16 uses GPIO SPI_SEL, port0.18 MOSI /
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
// the next time we access the AD converter for a read, initiate an internal low leval zero function
} //end of function




