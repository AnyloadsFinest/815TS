/*************************************************
 *  Copyright (C) 1999-2002 by  ZiLOG, Inc.
 *  All Rights Reserved
 *
 * Flash_IO.h : Header file for Flash Driver
 **************************************************/
#ifndef __FLASH_IO_INCLUDED
#define __FLASH_IO_INCLUDED

// base address of r0 register used for parameter passing when calling FLASH IAP routine
//#define IAP_LOCATION 0x7ffffff1 //0x7ffffff1
#define IAP_LOCATION 0x1FFF1FF1 //0x7ffffff1
#define CAL_LOCATION 0x00018000
#define ZERO_LOCATION 0x0000F000

#define _FLASH_CAL_STORE   0x00018000
#define _FLASH_ACC_STORE   0x0000E000
#define _FLASH_ZERO_STORE  0x0000F000


   
   
// Prototype definitions
void load_calibration(void);
void flash_save_cal(void);
void flash_recall_cal(void);
void system_init(char mode);
void string_init(char mode);
void board_system_init(void);
// void WriteByteToFlash(int location, unsigned int value);
// void WriteByteToFlash1(int location, unsigned int value);
// unsigned int ReadByteFromFlash(int location);
 
#endif