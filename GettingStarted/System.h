


#ifndef __SYSTEM_INCLUDED
#define __SYSTEM_INCLUDED

//#include <intrinsic.h>




void PLL_startup(void);
void setup_interupts(void);
void setup_system_timers(void);
void setup_gpio(void);

void setup_spi(void);
void setup_timers(void);
char read_COM_level(int port);

void TIMER0_ISR ();

#endif  // __SYSTEM_INCLUDED

