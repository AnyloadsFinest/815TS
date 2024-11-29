/*************************************************
 *  Copyright (C) 1999-2002 by  ZiLOG, Inc.
 *  All Rights Reserved
 *
 * Flash_IO.h : Header file for Flash Driver
 **************************************************/
#ifndef __FLASH_INCLUDED
#define __FLASH_INCLUDED

// base address of r0 register used for parameter passing when calling FLASH IAP routine
//#define IAP_LOCATION 0x7ffffff1 //0x7ffffff1
#define IAP_LOCATION 0x1FFF1FF1 //0x7ffffff1
//#define CAL_LOCATION 0x00018000
//#define ZERO_LOCATION 0x0000F000
#define _FLASH_CAL_STORE   0x00018000
//#define _FLASH_ACC_STORE   0x0000E000
#define _FLASH_ZERO_STORE  0x00010000//0x0000F000
   
void write_calibration(void);
void read_calibration(void);

void read_zero_data(void);
void storezero(void);
void write_checkweigh(void);
char write_circular_flash(void);

void unlock_flash_sector(char sector);
void erase_flash_sector(char sector);
char read_circular_flash(void);
void load_nvm(void);

//extern long   ZERO_SAVE[_ZERO_TABLE];
//extern struct nvmem NVMEM;     // non volatile memory stored seperatly it's own flash sector flash

#endif

