#ifndef __CALCMDS_INCLUDED
#define __CALCMDS_INCLUDED

void calibration_command(int cmd);
void prm_scroll(char direction);
int option_select_count (int current, int tmin, int tmax, char str[] );
int menuselect (int current , const int *table ,  const char *string );
int optionselect ( const int *table ,int current , char str[] , char dec);
#endif

