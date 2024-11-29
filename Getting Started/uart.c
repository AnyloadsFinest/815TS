
#include "includes.h"

/*****************************************************************************
 *   uart.c:  UART API file for NXP LPC23xx/24xx Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.07.12  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#include "includes.h"
/*******************************************************************************
UART0= RS-232   COM0
UART1= Radio    COM1
UART2= 20mA     COM2
UART3= RS-485   COM3
*******************************************************************************/
volatile DWORD UART0Status, UART1Status, UART2Status, UART3Status;
volatile BYTE  UART0TxEmpty = 1, UART1TxEmpty = 1, UART2TxEmpty = 1,UART3TxEmpty = 1;
volatile BYTE U0Status, U1Status, U2Status, U3Status; 
volatile BYTE UART0Buffer[BUFSIZE], UART1Buffer[BUFSIZE], UART2Buffer[BUFSIZE], UART3Buffer[BUFSIZE];
volatile DWORD UART0Count = 0, UART1Count = 0, UART2Count = 0 , UART3Count = 0;
/*****************************************************************************
** Function name:   UART0Handler
**
** Descriptions:    UART0 interrupt handler
**
** parameters:      None
** Returned value:    None
** RS-232
*****************************************************************************/
//UART0_IRQHandler
//UART1_IRQHandler
//UART2_IRQHandler
//UART3_IRQHandler
//void UART0Handler (void)
void UART0_IRQHandler(void)
{    
   char IIRValue, LSRValue;
   char ch; 
   // 
    LSRValue = U0LSR;       
    IIRValue = U0IIR;
    IIRValue >>= 1;        /* skip pending bit in IIR */
    IIRValue &= 0x07;      /* check bit 1~3, interrupt identification */
    
    
  if(LSRValue & LSR_FE)
  {
          U0Status = 1;
	  U0SCR = U0RBR;		// Dummy read on RX to clear 
					//interrupt, then bail out
//	   VICVectAddr = 0;            // Acknowledge Interrupt 
            NVIC_ClrPend(NVIC_UART0);  
          __enable_interrupt();         // handles nested interrupt   
           return;
  }
  
    
   if ( IIRValue == IIR_RDA ) // Receive Data Ready       
    {   // 
       ch = U0RBR; // 
       echo_comport(ch);
       
       ch = ch & 0X7F;   
       recevie_comport(ch, _PORT0);
       
    }   // 
 
  else if ( IIRValue == IIR_THRE )	// THRE, transmit holding register empty 
  {	    
        // THRE interrupt 
	// valid data in U0THR or not 
	if ( LSRValue & LSR_THRE )
	{
	  UART0TxEmpty = 1;                   
	}// 
	else
	{
	  UART0TxEmpty = 0;
	} // 
  }       //
  else if ( IIRValue == IIR_CTI ) /* Character timeout indicator */
    {
    /* Character Time-out indicator */
     UART0Status |= 0x100; /* Bit 9 as the CTI error */    
    }//
  else if ( IIRValue == IIR_RLS )		/* Receive Line Status */
  {	
	/* Receive Line Status */
	if ( LSRValue & (LSR_OE|LSR_PE|LSR_FE|LSR_RXFE|LSR_BI) )
	{
	  /* There are errors or break interrupt */
	  /* Read LSR will clear the interrupt */
          U0Status = 1;
	  UART0Status = LSRValue;
	  U0SCR = U0RBR;		/* Dummy read on RX to clear */
	}
        
       }   
   
  NVIC_ClrPend(NVIC_UART0);      
     
 // VICVectAddr = 0;            /* Acknowledge Interrupt */
  __enable_interrupt();   /* handles nested interrupt */  
  return;
}



/*****************************************************************************
** Function name:   UART1Handler
**
** Descriptions:    UART1 interrupt handler
**
** parameters:      None
** Returned value:    None
** 
*****************************************************************************/
//void UART1Handler (void)
void UART1_IRQHandler(void)
{
    char IIRValue, LSRValue;
    char ch; 
         
    LSRValue = U1LSR;
    IIRValue = U1IIR;
    IIRValue >>= 1;        /* skip pending bit in IIR */
    IIRValue &= 0x07;      /* check bit 1~3, interrupt identification */
        
     if(LSRValue & LSR_FE)
  {
          U1Status = 1;
	  U1SCR = U1RBR;		// Dummy read on RX to clear 
					//interrupt, then bail out
//	   VICVectAddr = 0;            // Acknowledge Interrupt 
            NVIC_ClrPend(NVIC_UART1);  
          __enable_interrupt();         // handles nested interrupt   
           return;
  }
  
    if ( IIRValue == IIR_RDA )	// Receive Data Available 
  { // Receive Data Available     
	
        ch = U1RBR; 
        echo_comport(ch);
        
        ch = ch & 0X7F;         
	recevie_comport(ch, _PORT1);
       
      	
  }

  else if ( IIRValue == IIR_THRE )	// THRE, transmit holding register empty 
  {
	// THRE interrupt 
	// LSRValue = U1LSR;		// Check status in the LSR to see if
					// Valid data in U0THR or not 
	if ( LSRValue & LSR_THRE )
       	{//
	  UART1TxEmpty = 1;         
	}//  
	else
	{//
	  UART1TxEmpty = 0;
	}// 
  }
  
 else if ( IIRValue == IIR_CTI ) /* Character timeout indicator */
    {
    /* Character Time-out indicator */
     UART1Status |= 0x100; /* Bit 9 as the CTI error */    
    }// 
 else if ( IIRValue == IIR_RLS )		/* Receive Line Status */
  {	
	/* Receive Line Status */
	if ( LSRValue & (LSR_OE|LSR_PE|LSR_FE|LSR_RXFE|LSR_BI) )
	{
	  /* There are errors or break interrupt */
	  /* Read LSR will clear the interrupt */
	  UART1Status = LSRValue;
	  U1SCR = U1RBR;		/* Dummy read on RX to clear */
	}
        
 }    
        
   NVIC_ClrPend(NVIC_UART1);      
   //VICVectAddr = 0;            /* Acknowledge Interrupt */
  __enable_interrupt();   /* handles nested interrupt */  
  return;
} // 

// 
/*****************************************************************************
** Function name:   UART2Handler
**
** Descriptions:    UART2 interrupt handler
**
** parameters:      None
** Returned value:    None
** 
*****************************************************************************/
//void UART2Handler (void)
void UART2_IRQHandler(void)
{   
    char IIRValue, LSRValue;
    char ch; 
    
    LSRValue = U2LSR;   
    IIRValue = U2IIR;
    IIRValue >>= 1;     /* skip pending bit in IIR */
    IIRValue &= 0x07;     /* check bit 1~3, interrupt identification */
        
    
      if(LSRValue & LSR_FE)
  {
          U2Status = 1;
	  U2SCR = U2RBR;		// Dummy read on RX to clear 
					//interrupt, then bail out
	   //VICVectAddr = 0;            /* Acknowledge Interrupt */
          
            NVIC_ClrPend(NVIC_UART2);    
         __enable_interrupt();   /* handles nested interrupt */  
          return;
  } 
  
   if ( IIRValue == IIR_RDA ) // Receive Data Ready      
   {      
      ch = U2RBR; 
      echo_comport(ch);
      
      ch = ch & 0X7F;    
      recevie_comport(ch, _PORT2);
     
   }// 
  else if ( IIRValue == IIR_THRE )	// THRE, transmit holding register empty 
  {
	// THRE interrupt 
	// LSRValue = U1LSR;		// Check status in the LSR to see if
					// valid data in U0THR or not 
	if ( LSRValue & LSR_THRE )
	{
	  UART2TxEmpty = 1;
	}//  
	else
	{ // 
	  UART2TxEmpty = 0;
	}//    
  }      // 
  else if ( IIRValue == IIR_CTI ) /* Character timeout indicator */
    {
    /* Character Time-out indicator */
     UART2Status |= 0x100; /* Bit 9 as the CTI error */    
    }//
  else if ( IIRValue == IIR_RLS )		/* Receive Line Status */
  {	
	/* Receive Line Status */
	if ( LSRValue & (LSR_OE|LSR_PE|LSR_FE|LSR_RXFE|LSR_BI) )
	{
	  /* There are errors or break interrupt */
	  /* Read LSR will clear the interrupt */
	  U2Status = 1;
          UART2Status = LSRValue;
	  U2SCR = U2RBR;		/* Dummy read on RX to clear */
	}
        
 }    
       NVIC_ClrPend(NVIC_UART2);         
    //VICVectAddr = 0;            /* Acknowledge Interrupt */
  __enable_interrupt();   /* handles nested interrupt */  
  return;
}

/*****************************************************************************
** Function name:   UART2Handler
**
** Descriptions:    UART2 interrupt handler
**
** parameters:      None
** Returned value:    None
** 
*****************************************************************************/
//void UART3Handler (void)
void UART3_IRQHandler(void)
{   
     char IIRValue, LSRValue;
     char ch; 
      
   
    LSRValue = U3LSR;   
    IIRValue = U3IIR;
    IIRValue >>= 1;     /* skip pending bit in IIR */
    IIRValue &= 0x07;     /* check bit 1~3, interrupt identification */
        
    
      if(LSRValue & LSR_FE)
      {
	      U3Status = 1;
	      U3SCR = U3RBR;		// Dummy read on RX to clear 
					    //interrupt, then bail out
		//VICVectAddr = 0;            /* Acknowledge Interrupt */
           NVIC_ClrPend(NVIC_UART3);              
	__enable_interrupt();   /* handles nested interrupt */
       // 
	return; //
      }    
    
    
    
    
     if ( IIRValue == IIR_RDA ) // Receive Data Available 
    {
         // Receive Data Available 
        ch = U3RBR; 
        echo_comport(ch);
        
        ch = ch & 0X7F;    
        recevie_comport(ch, _PORT2);
       
	  
	  } //
  
 else if ( IIRValue == IIR_THRE )	// THRE, transmit holding register empty 
  {
	// THRE interrupt 
	///LSRValue = U1LSR;		// Check status in the LSR to see if
					///valid data in U0THR or not 
	if ( LSRValue & LSR_THRE )
	{
	  UART3TxEmpty = 1;
	}//  
	else
	{ // 
	  UART3TxEmpty = 0;
	}//    
  } // 
 else if ( IIRValue == IIR_CTI ) /* Character timeout indicator */
    {
    /* Character Time-out indicator */
     UART3Status |= 0x100; /* Bit 9 as the CTI error */    
    }//
 else if ( IIRValue == IIR_RLS )		/* Receive Line Status */
  {	
	/* Receive Line Status */
	if ( LSRValue & (LSR_OE|LSR_PE|LSR_FE|LSR_RXFE|LSR_BI) )
	{
	  /* There are errors or break interrupt */
	  /* Read LSR will clear the interrupt */
	  U3Status = 1;
          UART3Status = LSRValue;
	  U3SCR = U3RBR;		/* Dummy read on RX to clear */
	}
        
 }    
      
        NVIC_ClrPend(NVIC_UART3);         
        __enable_interrupt();   /* handles nested interrupt */
   // 
    return; //
}

// 

/*****************************************************************************
** Function name:   UARTInit
**
** Descriptions:    Initialize UART0 port, setup pin select,
**            clock, parity, stop bits, FIFO, etc.
**
** parameters:      portNum(0 or 1) and UART baudrate
** Returned value:    true or false, return false only if the 
**            interrupt handler can't be installed to the 
**            VIC table
** 
*****************************************************************************/
DWORD UARTInit( DWORD PortNum, DWORD baudrate )
{
 // 
  long Fdiv;
  //  
  PINSEL0 |= 0xC050005A; // Configure 0.0 to 0.15 RS-485 and current loop
  PINSEL4 |= 0x0000000A; // Configure 2.0 to 2.15 Radio
 //  
  if ( PortNum == 0 )//RS232
  { //     
    U0LCR = 0x83;   /* 8 bits, no Parity, 1 Stop bit */
    Fdiv = ( Fpclk / 16 ) / baudrate ;	/*baud rate */
    U0DLM = Fdiv / 256;							
    U0DLL = Fdiv % 256;  
    U0LCR = 0x03;   /* DLAB = 0 */
    U0FCR = 0x07;   /* Enable and reset TX and RX FIFO. */
   
    NVIC_IntEnable(NVIC_UART0);
    U0IER = IER_RBR | IER_THRE | IER_RLS; /* Enable UART0 interrupt */
    return (TRUE);
  }
  else if ( PortNum == 1 )//Radio
  {
    U1LCR = 0x83;   /* 8 bits, no Parity, 1 Stop bit */    
    Fdiv = ( Fpclk / 16 ) / baudrate ;	/*baud rate */
    U1DLM = Fdiv / 256;							
    U1DLL = Fdiv % 256;     
      
    //U1DLM = 0;      // baud rate for 12Mhz             
    //U1DLL = 78;     // 
         
    U1LCR = 0x03;   /* DLAB = 0 */
    U1FCR = 0x07;   /* Enable and reset TX and RX FIFO. */
    
    NVIC_IntEnable(NVIC_UART1);// 
    U1IER =  IER_RBR | IER_THRE | IER_RLS;
    return (TRUE);
  } // 
 
else if ( PortNum == 2 )// 20mA
  {   
    //  
    U2LCR = 0x83;   /* 8 bits, no Parity, 1 Stop bit */
    Fdiv = ( Fpclk / 16 ) / baudrate ;	/*baud rate */
    U2DLM = Fdiv / 256;							
    U2DLL = Fdiv % 256;
    U2LCR = 0x03;   /* DLAB = 0 */    
    U2FCR = 0x07;   /* Enable and reset TX and RX FIFO. */
    
     NVIC_IntEnable(NVIC_UART2);
     U2IER = IER_RBR | IER_THRE | IER_RLS; /* Enable UART0 interrupt */ //IER_RBR | IER_RLS; /* Enable UART2 interrupt */
    
    return (TRUE); // 
  }  //
else if ( PortNum == 3 )//RS-485
  {     
    U3LCR = 0x83;   /* 8 bits, no Parity, 1 Stop bit */
    Fdiv = ( Fpclk / 16 ) / baudrate ;	/*baud rate */
    U3DLM = Fdiv / 256;			//				
    U3DLL = Fdiv % 256;
    U3LCR = 0x03;   /* DLAB = 0 */
    U3FCR = 0x07;   /* Enable and reset TX and RX FIFO. */
   
     NVIC_IntEnable(NVIC_UART3);
     U3IER = IER_RBR | IER_THRE | IER_RLS; /* Enable UART2 interrupt */
    return (TRUE);
  } //     

else return( FALSE ); 
 
} //  

DWORD UARTDisable( DWORD PortNum )
{  
   
  
  if ( PortNum == 0 )//RS-232
  { 
    PINSEL0 &=0xFFFFFF0F; // Configure 0.0 to 0.15 RS-485 and current loop// 
    U0LCR = 0;   /* 8 bits, no Parity, 1 Stop bit */
    U0FCR = 0;   /* Enable and reset TX and RX FIFO. */
    U0IER = 0; 
   return (TRUE);
  } // 
  else if ( PortNum == 1 ) // Radio Module
  {   
    U1LCR = 0;   /* 8 bits, no Parity, 1 Stop bit */
    U1FCR = 0;   /* Enable and reset TX and RX FIFO. */
    U1IER = 0; 
    return (TRUE);
  }
else if ( PortNum == 2 )// Current Loop
  { //
    PINSEL0 &=0xFF0FFFFF; // Configure 0.0 to 0.15 
    U2LCR = 0;   /* 8 bits, no Parity, 1 Stop bit */
    U2FCR = 0;   /* Enable and reset TX and RX FIFO. */
    U2IER = 0; 
    return (TRUE);
  }//  
else if ( PortNum == 3 )//Rs-485
  {  
    PINSEL0 &=0xFFFFFFF0; // Configure 0.0 to 0.15
    U3LCR = 0;   /* 8 bits, no Parity, 1 Stop bit */
    U3FCR = 0;   /* Enable and reset TX and RX FIFO. */
    U3IER = 0; 
    return (TRUE);
  } // 
else return( FALSE ); 
 //   
}// 
 // 
 

DWORD UARTEnable( DWORD PortNum)
{  
  
   
  if ( PortNum == 0 )    // Current loop
  { 
    PINSEL0 |=0x00000050; // Configure 0.0 to 0.15 RS-485 and current loop// 
    U0LCR = 3;   /* 8 bits, no Parity, 1 Stop bit */
    U0FCR = 7;   /* Enable and reset TX and RX FIFO. */
   return (TRUE);
  }// 
  else if ( PortNum == 1 )//Current loop
  {   
   U1LCR = 3;   /* 8 bits, no Parity, 1 Stop bit */
   U1FCR = 7;   /* Enable and reset TX and RX FIFO. */
   return (TRUE);
  }// 
  else if ( PortNum == 2 )  //Current loop
  {  // 
    PINSEL0 |=0x00500000; // Configure 0.0 to 0.15 
    U2LCR = 3;   /* 8 bits, no Parity, 1 Stop bit */
    U2FCR = 7;   /* Enable and reset TX and RX FIFO. */
   return (TRUE);
  }
  else if ( PortNum == 3 )  //Current loop
  { 
    PINSEL0 |=0x0000000A; // Configure 0.0 to 0.15 
    U3LCR = 3;   /* 8 bits, no Parity, 1 Stop bit */
    U3FCR = 7;   /* Enable and reset TX and RX FIFO. */
   return (TRUE);
  } // 
else return( FALSE ); 
 
} //

// if there is inehr
/*****************************************************************************
** Function name:   UARTSend
**
** Descriptions:    Send a block of data to the UART 0 port based
**            on the data length
**
** parameters:      portNum, buffer pointer, and data length
** Returned value:    None
** 
*****************************************************************************/
void UARTSend( char portNum, char *BufferPtr, char Length )
{  // 
  if ( portNum == 1 )
  {
      while ( Length != 0 )
      {
        /* THRE status, contain valid data */
        while ( !(UART1TxEmpty & 0x01) ); 
        U1THR = *BufferPtr;
        UART1TxEmpty = 0; /* not empty in the THR until it shifts out */
        BufferPtr++;
        Length--;
      } // 
  }
  else if ( portNum == 0 )
  {
    while ( Length != 0 )
    {
      /* THRE status, contain valid data */
      while ( !(UART0TxEmpty & 0x01) );      
      U0THR = *BufferPtr;
      UART0TxEmpty = 0; /* not empty in the THR until it shifts out */
      BufferPtr++;
      Length--;
    }
  }//  
  return;
}


void uart_txc(char c)
        {       
       while ( !(UART0TxEmpty & 0x01) );        
                                  // check for TXBUF empty and not time out
       U0THR = c;                // send 1 byte to the transmit buffer
       UART0TxEmpty = 0; 
        } // 
          
void uart_txstr(char *c)
      {        
        while (*c) uart_txc(*(c++));           
      } //sending a simple string to comport 

//----------------------------------------------------------
// Controls the reset pin to the scorboard digit panel
//----------------------------------------------------------
void echo_comport(char data)
{
 

} // end of funtion latchbankA
// 
/******************************************************************************
**                            End Of File
******************************************************************************/




//-------------------------------------------------------------------
// initialize com0
//-------------------------------------------------------------------

void init_com0(void)
{

  
 long Fdiv;
  //  
    if(CAL.COM1_MODE > 5)
    {
     UARTDisable(0);
     return; 
    }

UARTEnable(0); 
//PINSEL0 |=0x00000050; // Configure 0.0 to 0.15 RS-485 and current loop
//U0FCR=0x7;
COM[0].BUFF_XMIT=0;

switch(CAL.DATABITS1)
{
case 0: {
         U0LCR=0x83; // 8 data bits no parity
         
        break;
        }
        
case 2: {
          U0LCR=0x8A; // 7 data bits odd parity
         
        break;
        }        
  
case 1: {
         U0LCR=0x9A; // 7 data bits even parity
         
         break;
        } 
  
}  // end of switch 


  Fdiv = ( Fpclk / 16 ) / CAL.BAUD1 ;	/*baud rate */
    U0DLM = Fdiv / 256;							
    U0DLL = Fdiv % 256;
   
// Clear DLAB
U0LCR=U0LCR&0x7F; 

} // end of function


 //-------------------------------------------------------------------
// initialize com0
//-------------------------------------------------------------------

void init_com2(void)
{

  
 long Fdiv;
  
   if(CAL.COM3_MODE > 5)
    {
     UARTDisable(3);
     return; 
    }

UARTEnable(3); 

//PINSEL0 |=0x0000000A; // Configure 0.0 to 0.15 
//U3FCR=0x7;
COM[3].BUFF_XMIT=0;

// Enable FIFO's and reset them
//U3FCR=0x7;
 
switch(CAL.DATABITS3)
{
case 0: {         
         U3LCR=0x83; // 8 data bits no parity
        break;
        }
        
case 2: {          
          U3LCR=0x8A; // 7 data bits odd parity
        break;
        }        
  
case 1: {         
         U3LCR=0x9A; // 7 data bits even parity
         break;
        } 
  
}  // end of switch 


 

   Fdiv = ( Fpclk / 16 ) / CAL.BAUD3 ;	/*baud rate */
   U3DLM = Fdiv / 256;							
   U3DLL = Fdiv % 256;
// Clear DLAB
U3LCR=U3LCR&0x7F; 

} // end of function
//-------------------------------------------------------------------
// initialize com1 (indicator com 2)
//-------------------------------------------------------------------

void init_com1(void)
{

 long Fdiv;
  //  
if(CAL.COM2_MODE > 5)
{
 UARTDisable(2);
 return; 
}

UARTEnable(2); 
//PINSEL0 |=0x00500000; // Configure 0.0 to 0.15 
// U2FCR=0x7;
COM[2].BUFF_XMIT=0;
// Enable FIFO's and reset them
switch(CAL.DATABITS2)
{
case 0: {
         U2LCR=0x83; // 8 data bits no parity
        break;
        }
        
case 2: {
          U2LCR=0x8A; // 7 data bits odd parity
        break;
        }        
  
case 1: {
         U2LCR=0x9A; // 7 data bits even parity
        break;
        } 
  
}  // end of switch 



    Fdiv = ( Fpclk / 16 ) / CAL.BAUD2 ;	/*baud rate */
    U2DLM = Fdiv / 256;							
    U2DLL = Fdiv % 256;


// Clear DLAB
U2LCR=U2LCR&0x7F; 
 
} // end of function

//-------------------------------------------------------------------
//-------------------------------------------------------------------
bool isCOMready(int port)
{
  
    if (port==_PORT0)
   {//1
      if (U0LSR&60)
      {
        FIO0CLR = 0x00000020;
        return( true);
      }     
    
   }//1
   
   
   if (port==_PORT1)          //com port 1
   {//2
      if (U2LSR&60)
      {
        return( true);
      }
    
   }//1
   
   if (port==_PORT2)          //com port 1
   {//2
      if (U3LSR&60)
      {
        return( true);
      }
    
   }//1
   
   FIO0SET = 0x00000020; 
  return(false);  // port not ready
    
  
} // end of function


//----------------------------------------------------------------
// read com X
//----------------------------------------------------------------
char read_COM_RX(int port)
{
  
  if (port==_PORT0)
  {
  //  FIO0CLR = 0x00000020;
    return (U0RBR);  // read from COM0 port
  }
 
  if (port==_PORT1)
  {
    return (U2RBR);  // read from COM1 port 
  }
  
  if (port==_PORT2)
  {   
    return (U3RBR);  // read from COM0 port
  }
  
  return (U0RBR);  // read from COM0 port
}  // end of function


//---------------------------------------------------------------
// write COM x
//---------------------------------------------------------------
void write_COM_TX(char c,int port)
{
  

        if (port==_PORT0)
        {
       //   FIO0CLR = 0x00000020;
          U0THR=c;  // send character to com0
        }
        
        if(port==_PORT1)
        {
          U2THR=c;  // send character to com0
        }
       
        if(port==_PORT2)
        {
          U3THR=c;  // send character to com0
        }
  
 }
//-------------------------------------------------------------
// transmit COM x
//-------------------------------------------------------------
void transmit_COM_char(char c,int port)
{
  
while(1)
{
  if (isCOMready(port)==true)
  {
    break;
  }
}
  
write_COM_TX(c, port);
  
  
} // end of function

