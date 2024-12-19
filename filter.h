
#ifndef __filter_INCLUDED
#define __filter_INCLUDED


signed long AD_average(signed long newADvalue);
void scan_AD(void);
signed long CAL_ADaverage(void);
void filter_preset(char preset);
char countdown(char mode, char value);

#endif

