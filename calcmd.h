
#ifndef __calcmd_INCLUDED
#define __calcmd_INCLUDED


void calibration_command(int cmd);
int optionselect (const int *table , int current, char str[],char dec);
int option_select_count (int current, int tmin, int tmax, char str[]);
//int menuselect ( int current ,const int *table , char str[] );
int menuselect (int current , const int *table , const char *string);

void span_scale(void);
void deadload_scale(void);
void prm_scroll(char direction);
void  enter_cal(char mode);
float spanedit(float span);
void exit_cal(void);
void piece_scale(void);
float gspanedit(float gspan);
void span_scale_linear(void);
#endif
