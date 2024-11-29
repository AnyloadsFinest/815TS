/*****************************************************************************
 *   rtc.c:  Realtime clock C file for NXP LPC23xx/34xx Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.07.12  ver 1.00    Prelimnary version, first Release
 *
*****************************************************************************/
#include "includes.h"                        /* LPC23xx/24xx definitions */

volatile DWORD alarm_on = 0;

/*****************************************************************************
** Function name:		RTCHandler
**
** Descriptions:		RTC interrupt handler, it executes based on the
**				the alarm setting
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void RTCHandler (void) 
{  
  RTCILR |= ILR_RTCCIF;		/* clear interrupt flag */
  //alarm_on = 1;
  __enable_interrupt();       /* handles nested interrupt */	
 // VICVectAddr = 0;		/* Acknowledge Interrupt */
}

/*****************************************************************************
** Function name:		RTCInit
**
** Descriptions:		Initialize RTC timer
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void RTCInit( void )
{
  alarm_on = 0;
  /*--- Initialize registers ---*/    
  RTCAMR = 0;
  RTCCIIR = 1;
  RTCCCR = 0;
  //RTC_PREINT = PREINT_RTC;
  //RTC_PREFRAC = PREFRAC_RTC;
 // RTC_PREINT = 0x16D;
 // RTC_PREFRAC = 0x1B00; //12mhz 
  //RTC_PREINT = 0xB6;
  //RTC_PREFRAC = 0xD80; // 6mhz
  //RTCPREINT = 0x98;
  //RTCPREFRAC = 0x34BE;//5mhz
  
  return;
}

/*****************************************************************************
** Function name:		RTCStart
**
** Descriptions:		Start RTC timer
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void RTCStart( void ) 
{
  /*--- Start RTC counters ---*/
  RTCCCR |= CCR_CLKEN;
  RTCILR = ILR_RTCCIF;
  return;
}

/*****************************************************************************
** Function name:		RTCStop
**
** Descriptions:		Stop RTC timer
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void RTCStop( void )
{   
  /*--- Stop RTC counters ---*/
  RTCCCR &= ~CCR_CLKEN;
  return;
} 

/*****************************************************************************
** Function name:		RTC_CTCReset
**
** Descriptions:		Reset RTC clock tick counter
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void RTC_CTCReset( void )
{   
  /*--- Reset CTC ---*/
  RTCCCR |= CCR_CTCRST;
  return;
}

/*****************************************************************************
** Function name:		RTCSetTime
**
** Descriptions:		Setup RTC timer value
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void RTCSetTime( RTCTime Time ) 
{
  RTCSEC = Time.RTC_Sec;
  RTCMIN = Time.RTC_Min;
  RTCHOUR = Time.RTC_Hour;
  RTCDOM = Time.RTC_Mday;
  RTCDOW = Time.RTC_Wday;
  RTCDOY = Time.RTC_Yday;
  RTCMONTH = Time.RTC_Mon;
  RTCYEAR = Time.RTC_Year;    
  return;
}

/*****************************************************************************
** Function name:		RTCSetAlarm
**
** Descriptions:		Initialize RTC timer
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void RTCSetAlarm( RTCTime Alarm ) 
{   
  RTCALSEC = Alarm.RTC_Sec;
  RTCALMIN = Alarm.RTC_Min;
  RTCALHOUR = Alarm.RTC_Hour;
  RTCALDOM = Alarm.RTC_Mday;
  RTCALDOW = Alarm.RTC_Wday;
  RTCALDOY = Alarm.RTC_Yday;
  RTCALMON = Alarm.RTC_Mon;
  RTCALYEAR = Alarm.RTC_Year;    
  return;
}

/*****************************************************************************
** Function name:		RTCGetTime
**
** Descriptions:		Get RTC timer value
**
** parameters:			None
** Returned value:		The data structure of the RTC time table
** 
*****************************************************************************/
RTCTime RTCGetTime( void ) 
{
  RTCTime LocalTime;
    
  LocalTime.RTC_Sec = RTCSEC;
  LocalTime.RTC_Min = RTCMIN;
  LocalTime.RTC_Hour = RTCHOUR;
  LocalTime.RTC_Mday = RTCDOM;
  LocalTime.RTC_Wday = RTCDOW;
  LocalTime.RTC_Yday = RTCDOY;
  LocalTime.RTC_Mon = RTCMONTH;
  LocalTime.RTC_Year = RTCYEAR;
  return ( LocalTime );    
}

/*****************************************************************************
** Function name:		RTCSetAlarmMask
**
** Descriptions:		Set RTC timer alarm mask
**
** parameters:			Alarm mask setting
** Returned value:		None
** 
*****************************************************************************/
void RTCSetAlarmMask( DWORD AlarmMask ) 
{
  /*--- Set alarm mask ---*/    
  RTCAMR = AlarmMask;
  return;
}

/*****************************************************************************
**                            End Of File
******************************************************************************/

