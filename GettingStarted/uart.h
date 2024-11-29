/*****************************************************************************
 *   uart.h:  Header file for NXP LPC23xx Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.09.01  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
//#define COM0 0                          //RS-232
//#define COM1 1                          //radio  
//#define COM2 2                          //20mA
//#define COM3 3                          //RS-485

#define NONE 0
#define EVEN 1
#define ODD  2                          //Parity select

#define _PORT0 0
#define _PORT1 2
#define _PORT2 3
//#define _PORT3 1

#define IER_RBR   0x01
#define IER_THRE  0x02
#define IER_RLS   0x04

#define IIR_PEND  0x01
#define IIR_RLS   0x03
#define IIR_RDA   0x02
#define IIR_CTI   0x06
#define IIR_THRE  0x01

#define LSR_RDR   0x01
#define LSR_OE    0x02
#define LSR_PE    0x04
#define LSR_FE    0x08
#define LSR_BI    0x10
#define LSR_THRE  0x20
#define LSR_TEMT  0x40
#define LSR_RXFE  0x80

#define BUFSIZE   0x10

DWORD UARTInit( DWORD portNum, DWORD Baudrate );
DWORD UARTDisable( DWORD PortNum );
DWORD UARTEnable( DWORD PortNum);
void uart_txc(char c);
void uart_txstr(char *c);
void echo_comport(char data);
//void UART0Handler( void );
//void UART1Handler( void );
//void UART2Handler( void );
//void UART3Handler( void );
void UARTSend( char portNum, char *BufferPtr, char Length );
void UART0_IRQHandler(void);
void UART1_IRQHandler(void);
void UART2_IRQHandler(void);
void UART3_IRQHandler(void);
/*****************************************************************************
**                            End Of File
******************************************************************************/
void init_com2(void);
void init_com0(void);
void init_com1(void);
bool isCOMready(int port);
char read_COM_RX(int port);
void write_COM_TX(char c,int port);
void transmit_COM_char(char c,int port);


