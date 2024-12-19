#ifndef __TICCODE_INCLUDED
#define __TICCODE_INCLUDED


#define _ACC_REGS 6      // how many accumalators we support in the ticket system (ACC0..ACCx)
#define _ACC_CIRCULAR 10  // number of entries in the circular table to reduce wear and tare in the flash memory
#define TEMPBUFF 50   // length of temperary string buffer unsed to generate outputs 

// declaration of ACC registers
// acc[0] = TICKET COUNTER
// acc[1]..acc[5]   acc1 to acc5
// acc[6] check sum used to indicate that an update is required for flash

extern long ACC[_ACC_REGS]; // this is the working array used for managing ACCumalators
extern long ACC_CIRCULAR[_ACC_CIRCULAR][ _ACC_REGS ];     // circular buffer globle table used for accumalators in the ticket system being stored to flash memory 
//extern long ACC_CIRCULAR[_ACC_REGS][ _ACC_CIRCULAR];     // circular buffer globle table used for accumalators in the ticket system being stored to flash memory 



extern void refresh_ACC(void); 




void ticket_control(int data);
long extractvalue (char n);
int get_index(void);
long acc_rw(char acc,long value,int operation);
long acc_r(int acc);
void acc_w(int acc, long val);

#endif