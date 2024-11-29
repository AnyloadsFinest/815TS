/*****************************************************************************
 *   i2c.c:  I2C C file for NXP LPC23xx/24xx Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.07.19  ver 1.00    Prelimnary version, first Release
 *
*****************************************************************************/

#include "includes.h"

static DWORD I2CMasterState = I2C_IDLE; // dedicated to read temperature from LM75
static DWORD I2CCmd;
static BYTE  I2CMasterBuffer[25];
static DWORD I2CReadLength;
static DWORD I2CWriteLength;
static DWORD RdIndex = 0;
static DWORD WrIndex = 0;

static DWORD I2C1MasterState = I2C_IDLE; // dedicated to read the real time clock
static DWORD I2C1Cmd;                    
static BYTE  I2C1MasterBuffer[25];
static DWORD I2C1ReadLength;
static DWORD I2C1WriteLength;
static DWORD Rd1Index = 0;
static DWORD Wr1Index = 0;              
/* 
From device to device, the I2C communication protocol may vary, 
in the example below, the protocol uses repeated start to read data from or 
write to the device:
For master read: the sequence is: STA,Addr(W),offset,RE-STA,Addr(r),data...STO 
for master write: the sequence is: STA,Addr(W),length,RE-STA,Addr(w),data...STO
Thus, in state 8, the address is always WRITE. in state 10, the address could 
be READ or WRITE depending on the I2CCmd.
*/   
/*****************************************************************************
** Function name:		I2C0MasterHandler
**
** Descriptions:		I2C0 interrupt handler, deal with master mode
**				only.
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void I2C1MasterHandler(void) 
{
 BYTE StatValue;
  /* this handler deals with master read and master write only */
  StatValue = I21STAT;
  __enable_interrupt();  			/* handles nested interrupt */	
   
	switch ( StatValue )
        {
	case 0x08:			/* A Start condition is issued. */
	I21DAT = I2C1MasterBuffer[0];
        I21CONCLR = (I2CONCLR_SIC | I2CONCLR_STAC);
	I2C1MasterState = I2C_STARTED;
	break;
	
	case 0x10:			/* A repeated started is issued */
	if ( I2C1Cmd == MAXIM_SEC )
	{
	  I21DAT = I2C1MasterBuffer[2];
	}
	I21CONCLR = (I2CONCLR_SIC | I2CONCLR_STAC);
	I2C1MasterState = I2C_RESTARTED;
	break;  
	
	case 0x18:			/* Regardless, it's a ACK */
	if ( I2C1MasterState == I2C_STARTED )
	{
	  I21DAT = I2C1MasterBuffer[1+Wr1Index];
	  Wr1Index++;
	  I2C1MasterState = DATA_ACK;
	}
	I21CONCLR = I2CONCLR_SIC;
	break;
	
	case 0x28:	/* Data byte has been transmitted, regardless ACK or NACK */
	case 0x30:
	if ( Wr1Index != I2C1WriteLength )
	{   
	  I21DAT = I2C1MasterBuffer[1+Wr1Index]; /* this should be the last one */
	  Wr1Index++;
	  if ( Wr1Index != I2C1WriteLength )
	  {   
		I2C1MasterState = DATA_ACK;
	  }
	  else
	  {  //  
		I2C1MasterState = DATA_NACK;
		if ( I2C1ReadLength != 0 )
		{
		  I21CONSET = I2CONSET_STA;	/* Set Repeated-start flag */
		  I2C1MasterState = I2C_REPEATED_START;
		}
	  }
	}//
	else
	{      //
	  if ( I2C1ReadLength != 0 )
	  {     //  
		I21CONSET = I2CONSET_STA;	/* Set Repeated-start flag */
		I2C1MasterState = I2C_REPEATED_START;
	  }
	  else
	  {
		I2C1MasterState = DATA_NACK;
	  }    //   
	}
	I21CONCLR = I2CONCLR_SIC;
	break;
	//
	case 0x40:	/* Master Receive, SLA_R has been sent */
	I21CONCLR = I2CONCLR_SIC;
	break;
	// 
	case 0x50:	/* Data byte has been received, regardless following ACK or NACK */
	case 0x58:
	I2C1MasterBuffer[3 + Rd1Index] = I21DAT;
	Rd1Index++;
	if ( Rd1Index != I2C1ReadLength )
	{   
	  I2C1MasterState = DATA_ACK;
	}
	else
	{ // 
	  Rd1Index = 0;
	  I2C1MasterState = DATA_NACK;
	} //
	I21CONSET = I2CONSET_AA;	 /* assert ACK after data is received */
	I21CONCLR = I2CONCLR_SIC;
	break;
	
	case 0x20:		         /* regardless, it's a NACK */
	case 0x48:
	I21CONCLR = I2CONCLR_SIC;
	I2C1MasterState = DATA_NACK;     
	break;
	
	case 0x38:		/* Arbitration lost, in this example, we don't
					deal with multiple master situation */
	default:
	I21CONCLR = I2CONCLR_SIC;	
	break;
  }     // 
  
  VICVectAddr = 0;		/* Acknowledge Interrupt */
} // 

/*****************************************************************************
** Function name:		I2CStart
**
** Descriptions:		Create I2C start condition, a timeout
**				value is set if the I2C never gets started,
**				and timed out. It's a fatal error. 
**
** parameters:			None
** Returned value:		true or false, return false if timed out
** 
*****************************************************************************/
DWORD I2C1Start( void )
{
   DWORD timeout = 0;
   DWORD retVal = FALSE;
 
  /*--- Issue a start condition ---*/
  I21CONSET = I2CONSET_STA;	/* Set Start flag */
  /*--- Wait until START transmitted ---*/
  
  while( 1 )
  {     // 
	if ( I2C1MasterState == I2C_STARTED )
	{retVal = TRUE; break;}
	if ( timeout >= MAX_TIMEOUT )
	{ retVal = FALSE; break;}
	timeout++;
  } //   
  return( retVal ); 
}

/*****************************************************************************
** Function name:		I2CStop
**
** Descriptions:		Set the I2C stop condition, if the routine
**				never exit, it's a fatal bus error.
**
** parameters:			None
** Returned value:		true or never return
** 
*****************************************************************************/
DWORD I2C1Stop( void )
{
  DWORD timeout = 0;
 
  I21CONSET = I2CONSET_STO;      /* Set Stop flag */ 
  I21CONCLR = I2CONCLR_SIC;  /* Clear SI flag */ 
  
  /*--- Wait for STOP detected ---*/
  while(1)  
  {    
     if (!(I21CONSET & I2CONSET_STO) )
      {
      return TRUE;
      }
     
     if ( timeout >= MAX_TIMEOUT )
      {
      return FALSE;
      }
   
     timeout++;   
  } //   
    //return TRUE;
  } // 

/*****************************************************************************
** Function name:		I2CInit
**
** Descriptions:		Initialize I2C controller
**
** parameters:			I2c mode is either MASTER or SLAVE
** Returned value:		true or false, return false if the I2C
**				interrupt handler was not installed correctly
** 
*****************************************************************************/
DWORD I2C1Init( DWORD I2c1Mode ) 
{
  // 
  //PCONP |= (1 << 19); /* function to 01 on both SDA and SCK. */				
  //PINSEL1=0x014003F0; // Configure 0.16 to 0.31 
  
 /*--- Clear flags ---*/
  I21CONCLR = I2CONCLR_AAC | I2CONCLR_SIC | I2CONCLR_STAC | I2CONCLR_I2ENC;    
  /*--- Reset registers ---*/
  I21SCLL   = I2SCLL_SCLL;
  I21SCLH   = I2SCLH_SCLH;
 
  /* Install interrupt handler */	
  if ( install_irq( I2C1_INT, (void *)I2C1MasterHandler, HIGHEST_PRIORITY ) == FALSE )
  {
	return( FALSE );
  } //   
  I21CONSET = I2CONSET_I2EN;
  return( TRUE );
}

/*****************************************************************************
** Function name:		I2CEngine
**
** Descriptions:		The routine to complete a I2C transaction
**				from start to stop. All the intermitten
**				steps are handled in the interrupt handler.
**				Before this routine is called, the read
**				length, write length, I2C master buffer,
**				and I2C command fields need to be filled.
**				see i2cmst.c for more details. 
**
** parameters:			None
** Returned value:		true or false, return false only if the
**				start condition can never be generated and
**				timed out. 
** 
*****************************************************************************/
DWORD I2C1Engine( void ) 
{ 
  I2C1MasterState = I2C_IDLE;
  Rd1Index = 0;
  Wr1Index = 0;
  if ( I2C1Start() != TRUE )
  {I2C1Stop(); return ( FALSE );}
  //  
  timer(MAXIM_TIME,START);  // start timer to detect button held down 
  // 
 while ( 1 )
  {
	if ( I2C1MasterState == DATA_NACK )
	{I2C1Stop(); return(TRUE);}
        if (timer(MAXIM_TIME,TIMEOUT))  // start timer to detect button held down
        {I2C1Stop(); return(FALSE);}        
  }  
//return(TRUE); // 
} //  
/******************************************************************************
**                            End Of File
******************************************************************************/
/*
static char  DaylightSaving_Eu[]={ // 

//1. 2008
//start month, start date,   end month, end date                 
  3  ,          30,         10,           26,

//2. 2009       
//start month, start date,   end month, end date                 
  3  ,          29,         10,           25,
  
//3. 2010
//start month, start date,   end month, end date                 
  3  ,          28,         10,           31,
  
//4. 2011
//start month, start date,   end month, end date                 
  3  ,          27,         10,           30,
 
//5. 2012
//start month, start date,   end month, end date                 
  3  ,          25,         10,           28,
  
//6. 2013
//start month, start date,   end month, end date                 
  3 ,          31,         10,            27,
  
 //7. 2014
//start month, start date,   end month, end date                 
  3  ,          30,         10,           26,
  
//8. 2015
//start month, start date,   end month, end date                 
  3  ,          29,         10,           25,
  
 //9. 2016
//start month, start date,   end month, end date                 
  3  ,          27,         10,           30,
  
  //10. 2017    //
  //start month, start date,   end month, end date                 
  3  ,          26,         10,           29,
  
  //11. 2018
//start month, start date,   end month, end date                 
  3  ,         25,          10,           28,

//12. 2019
//start month, start date,   end month, end date                 
  3  ,          31,         10,           27,
  
 //13. 2020
//start month, start date,   end month, end date                 
  3  ,          29,         10,           25,
};

// 
static char  DaylightSaving_Us[]={

//1. 2008
//start month, start date,   end month, end date                 
  3  ,          9,             11,           2,

//2. 2009
//start month, start date,   end month, end date                 
  3  ,          8,         11,           1,
  
  //3. 2010
//start month, start date,   end month, end date                 
  3  ,          14,         11,           7,
  
  //4. 2011
//start month, start date,   end month, end date                 
  3  ,          13,         11,           6,
 
 //5. 2012
//start month, start date,   end month, end date                 
  3  ,          11,         11,           4,
  
  //6. 2013
//start month, start date,   end month, end date                 
  3 ,          10,         11,           3,
  
  //7. 2014
//start month, start date,   end month, end date                 
 3  ,          9,         11,           2,
  
 //8. 2015
//start month, start date,   end month, end date                 
  3  ,          8,         11,           1,
  
  //9. 2016
//start month, start date,   end month, end date                 
  3  ,          13,         11,           6,
  
  //10. 2017
  //start month, start date,   end month, end date                 
  3  ,          12,         11,           5,
  
  //11. 2018
//start month, start date,   end month, end date                 
  3  ,          11,         11,           4,

//12. 2019
//start month, start date,   end month, end date                 
  3  ,          10,        11,           3,
  
 //13. 2020
//start month, start date,   end month, end date                 
  3  ,          8,         11,           1,
}; //
*/
/******************************************************************************
**                            End Of File
******************************************************************************/

void read_mixim(void)
{// 
char i;

for ( i = 0; i < BUFSIZE; i++ )	
  {
	I2C1MasterBuffer[i] = 0;
  }
  I2C1WriteLength = 1;
  I2C1ReadLength = 0;
  I2C1MasterBuffer[0] = MAXIM_ADDR;
  I2C1MasterBuffer[1] = MAXIM_POINTER;
  I2C1MasterBuffer[2] = MAXIM_ADDR | RD_BIT;	
  I2C1Cmd = MAXIM_CONFIG;
  I2C1MasterState = I2C_IDLE;
  Rd1Index = 0;
  Wr1Index = 0;
  I2C1Engine(); 
  //     
  for ( i = 0; i < BUFSIZE; i++ )	
  {
	I2C1MasterBuffer[i] = 0;
  }
  I2C1WriteLength = 1;//1
  I2C1ReadLength = 19;//2
  I2C1MasterBuffer[0] = MAXIM_ADDR ;
  I2C1MasterBuffer[1] = MAXIM_POINTER;  
  I2C1MasterBuffer[2] = MAXIM_ADDR | RD_BIT;
  I2C1Cmd = MAXIM_SEC;
  I2C1MasterState = I2C_IDLE;
  Rd1Index = 0; // 
  Wr1Index = 0; // 
  I2C1Engine(); // 
  
  BOARD.SEC =   I2C1MasterBuffer[3];   // seconds 
  BOARD.MIN =   I2C1MasterBuffer[4];   // minutes 
  BOARD.HOR =   I2C1MasterBuffer[5];   // hours
  BOARD.DAY =   I2C1MasterBuffer[6];   // days
  BOARD.DATE =  I2C1MasterBuffer[7];   // date  
  BOARD.MONTH = I2C1MasterBuffer[8];   // month
  BOARD.YEAR =  I2C1MasterBuffer[9];   // year
  // format time for display
  
  }// end of function

// 

void write_mixim(void)
{

char i;

for ( i = 0; i < BUFSIZE; i++ )	
  {
  I2C1MasterBuffer[i] = 0;
  }
  I2C1WriteLength = 9;
  I2C1ReadLength = 0;
  I2C1MasterBuffer[0] = MAXIM_ADDR;
  I2C1MasterBuffer[1] = MAXIM_POINTER;
  I2C1MasterBuffer[2] = 0x00;	//0x00
  I2C1MasterBuffer[3] = BOARD.MIN;
  I2C1MasterBuffer[4] = BOARD.HOR | 0x40; // 12 hours mode
 // I2C1MasterBuffer[4] = BOARD.HOR & 0x2F; // 24 hours mode
  I2C1MasterBuffer[5] = BOARD.DAY; 
  I2C1MasterBuffer[6]=  BOARD.DATE; 
  I2C1MasterBuffer[7]=  BOARD.MONTH; 
  I2C1MasterBuffer[8] = BOARD.YEAR;
  I2C1Cmd = MAXIM_CONFIG;
  I2C1MasterState = I2C_IDLE;
  Rd1Index = 0;  // 
  Wr1Index = 0;
  I2C1Engine(); // 
  //  
  for ( i = 0; i < BUFSIZE; i++ )	
  {
	I2C1MasterBuffer[i] = 0;
  }
  I2C1WriteLength = 1;//1
  I2C1ReadLength = 19;//2
  I2C1MasterBuffer[0] = MAXIM_ADDR ;
  I2C1MasterBuffer[1] = MAXIM_SEC;
  I2C1MasterBuffer[2] = MAXIM_ADDR | RD_BIT;
  I2C1Cmd = MAXIM_SEC;
  I2C1MasterState = I2C_IDLE;
  Rd1Index = 0; //
  Wr1Index = 0;
  I2C1Engine();     
  // 
   }// end of function
    // 

////////////////////////////////////////////////////////////////////////////////
///End of RTC module
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/*****************************************************************************
** Function name:		I2C0MasterHandler
**
** Descriptions:		I2C0 interrupt handler, deal with master mode
**				only.
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
__irq __nested __arm void I2C0MasterHandler(void) 
{
 BYTE StatValue;

  /* this handler deals with master read and master write only */
  StatValue = I20STAT;
  
  __enable_interrupt();  			/* handles nested interrupt */	
  
  switch ( StatValue )
  {
	case 0x08:			/* A Start condition is issued. */
          {
          I20DAT = I2CMasterBuffer[0];
	  I20CONCLR = (I2CONCLR_SIC | I2CONCLR_STAC);
	  I2CMasterState = I2C_STARTED;
	  break; //  
          }
	case 0x10:			/* A repeated started is issued */
          { //
        if ( I2CCmd == LM75_TEMP )
	{
	  I20DAT = I2CMasterBuffer[2];
	} // 
	I20CONCLR = (I2CONCLR_SIC | I2CONCLR_STAC);
	I2CMasterState = I2C_RESTARTED;
	break; 
          } // 
	case 0x18:			/* Regardless, it's a ACK */
          {// 
          if ( I2CMasterState == I2C_STARTED )
	{ //
	  I20DAT = I2CMasterBuffer[1+WrIndex];
	  WrIndex++;
	  I2CMasterState = DATA_ACK;
	}
	I20CONCLR = I2CONCLR_SIC;
	break;
          } // 
	case 0x28:	/* Data byte has been transmitted, regardless ACK or NACK */
	case 0x30:
          { //
          if ( WrIndex != I2CWriteLength )
	  {   
	 I20DAT = I2CMasterBuffer[1+WrIndex]; /* this should be the last one */
	 WrIndex++;
	 if ( WrIndex != I2CWriteLength )
	  {   
		I2CMasterState = DATA_ACK;
	  }
	  else
	  {     // 
                I2CMasterState = DATA_NACK;
		if ( I2CReadLength != 0 )
		{
		  I20CONSET = I2CONSET_STA;	/* Set Repeated-start flag */
		  I2CMasterState = I2C_REPEATED_START;
		}
	  }
	}
	else
	{
	  if ( I2CReadLength != 0 )
	  {
		I20CONSET = I2CONSET_STA;	/* Set Repeated-start flag */
		I2CMasterState = I2C_REPEATED_START;
	  }
	  else
	  {
		I2CMasterState = DATA_NACK;
	  }
	} // 
	I20CONCLR = I2CONCLR_SIC;
	break;
          }
	case 0x40:	/* Master Receive, SLA_R has been sent */
          {
          I20CONCLR = I2CONCLR_SIC;
	break; //  
          }
	case 0x50:	/* Data byte has been received, regardless following ACK or NACK */
	case 0x58:
          {  // 
        I2CMasterBuffer[3+RdIndex] = I20DAT;
        RdIndex++;
	if ( RdIndex != I2CReadLength )
	{           
	  I2CMasterState = DATA_ACK;
	} // 
	else
	{
	  RdIndex = 0;
	  I2CMasterState = DATA_NACK;
	} // 
	I20CONSET = I2CONSET_AA;	/* assert ACK after data is received */
	I20CONCLR = I2CONCLR_SIC;
	break;
          }
	case 0x20:		/* regardless, it's a NACK */
	case 0x48:
          {
         I20CONCLR = I2CONCLR_SIC;
	 I2CMasterState = DATA_NACK;
	 break;
          }
	case 0x38:		/* Arbitration lost, in this example, we don't
					deal with multiple master situation */
	default:
	I20CONCLR = I2CONCLR_SIC;	
	break; // 
  }
   
  
  VICVectAddr = 0;		/* Acknowledge Interrupt */
}

/*****************************************************************************
** Function name:		I2CStart
**
** Descriptions:		Create I2C start condition, a timeout
**				value is set if the I2C never gets started,
**				and timed out. It's a fatal error. 
**
** parameters:			None
** Returned value:		true or false, return false if timed out
** 
*****************************************************************************/
DWORD I2CStart( void )
{
   DWORD timeout = 0;
   DWORD retVal = FALSE; 
  /*--- Issue a start condition ---*/
  I20CONSET = I2CONSET_STA;	/* Set Start flag */    
  /*--- Wait until START transmitted ---*/
  while( 1 )
  {
    if ( I2CMasterState == I2C_STARTED )
	{
	  retVal = TRUE;
	  break;	
	} // 
	if ( timeout >= MAX_TIMEOUT )
	{
	  retVal = FALSE;
	  break;
	}
	timeout++;
  }
  return( retVal );
 }

/*****************************************************************************
** Function name:		I2CStop
**
** Descriptions:		Set the I2C stop condition, if the routine
**				never exit, it's a fatal bus error.
**
** parameters:			None
** Returned value:		true or never return
** 
*****************************************************************************/
DWORD I2CStop( void )
{
  DWORD timeout=0;
  I20CONSET = I2CONSET_STO;      /* Set Stop flag */ 
  I20CONCLR = I2CONCLR_SIC;  /* Clear SI flag */ 
            
  /*--- Wait for STOP detected ---*/
 while(1)
 {
   if (!(I20CONSET & I2CONSET_STO) ) return TRUE;
   if ( timeout >= MAX_TIMEOUT ) return FALSE;
	timeout++;  
 }
  //while( I20CONSET & I2CONSET_STO );
  //return TRUE;
}

/*****************************************************************************
** Function name:		I2CInit
**
** Descriptions:		Initialize I2C controller
**
** parameters:			I2c mode is either MASTER or SLAVE
** Returned value:		true or false, return false if the I2C
**				interrupt handler was not installed correctly
** 
*****************************************************************************/
DWORD I2CInit( DWORD I2cMode ) 
{
  //PCONP |= (1 << 19);
				/* function to 01 on both SDA and SCK. */
  //PINSEL1=0x014003F0; // Configure 0.16 to 0.31 
 /*--- Clear flags ---*/
  I20CONCLR = I2CONCLR_AAC | I2CONCLR_SIC | I2CONCLR_STAC | I2CONCLR_I2ENC;    
 // 
  /*--- Reset registers ---*/
  I20SCLL   = I2SCLL_SCLL;
  I20SCLH   = I2SCLH_SCLH;
 
  /* Install interrupt handler */	
  if ( install_irq( I2C0_INT, (void *)I2C0MasterHandler, HIGHEST_PRIORITY ) == FALSE )
  {
  return( FALSE );
  }
  I20CONSET = I2CONSET_I2EN; // 
  
  return( TRUE );
}

/*****************************************************************************
** Function name:		I2CEngine
**
** Descriptions:		The routine to complete a I2C transaction
**				from start to stop. All the intermitten
**				steps are handled in the interrupt handler.
**				Before this routine is called, the read
**				length, write length, I2C master buffer,
**				and I2C command fields need to be filled.
**				see i2cmst.c for more details. 
**
** parameters:			None
** Returned value:		true or false, return false only if the
**				start condition can never be generated and
**				timed out. 
** 
*****************************************************************************/
DWORD I2CEngine( void ) 
{ 
 I2CMasterState = I2C_IDLE;
 RdIndex = 0;
 WrIndex = 0;
 if ( I2CStart() != TRUE )
  {
	I2CStop();
	return ( FALSE );
  }

 timer(LM75_TIME,START);  // start timer to detect button held down 
 
 while ( 1 )   
     { //	
       if ( I2CMasterState == DATA_NACK )
	{I2CStop(); return(TRUE);}
        if (timer(LM75_TIME,TIMEOUT))  // start timer to detect button held down
        {I2CStop(); return(FALSE);}
  // 
  }     
  //return ( TRUE );      
}

/******************************************************************************
**                            End Of File
******************************************************************************/
void show_temperature(void)
{
char i;
long val;  // 
char sign=0;

      I2CInit( (DWORD)I2CMASTER );
      for ( i = 0; i < BUFSIZE; i++ ) I2CMasterBuffer[i] = 0;
      I2CWriteLength = 1;
      I2CReadLength = 0;
      I2CMasterBuffer[0] = LM75_ADDR;
      I2CMasterBuffer[1] = LM75_CONFIG;
      I2CMasterBuffer[2] = 0x00;	
      I2CCmd = LM75_CONFIG;
      I2CEngine(); 
      // 
      for ( i = 0; i < BUFSIZE; i++ ) I2CMasterBuffer[i] = 0;
      I2CWriteLength = 1;
      I2CReadLength = 2;
      I2CMasterBuffer[0] = LM75_ADDR;
      I2CMasterBuffer[1] = LM75_TEMP;
      I2CMasterBuffer[2] = LM75_ADDR | RD_BIT;
      I2CCmd = LM75_TEMP;
      I2CEngine();
      BOARD.TEMPERATURE = 0;
      BOARD.TEMPERATURE = I2CMasterBuffer[3];
             
      if(BOARD.TEMPERATURE > 127)
      {
      BOARD.TEMPERATURE= -1*BOARD.TEMPERATURE;
      sign=1;
      }else sign=0;  // 
      
     if(BOARD.FARENHEIT)
     {  
     if(!sign)
     {  
     val= (1.8 * BOARD.TEMPERATURE) + 32;
     }
     else
     {  
     val= 32 - (1.8 * BOARD.TEMPERATURE);
     }
     if(val < 0)
     {
     val=-1*val;     
     } else sign=0;
     displayvalue(val, 0, 1, " F ");
     } 
     else
     {  
      val= BOARD.TEMPERATURE;
      displayvalue(val, 0, 1, " C ");     
     }    
      if(sign) BOARD.DIGIT[0]='-';  
     
     I20CONCLR = I2CONCLR_AAC | I2CONCLR_SIC | I2CONCLR_STAC | I2CONCLR_I2ENC;
      // disable i2c     
}//end of function



















