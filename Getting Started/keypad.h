

#ifndef __KEYPAD_INCLUDED
#define __KEYPAD_INCLUDED

#define _TARGET 1
#define _ID     2
#define _PRINT  3
#define _TARE   4
#define _CLEAR  5
#define _UNITS  6
#define _ZERO   7
#define _ENTER  8
#define _DOWN   9
#define _UP     10
#define _LEFT   11
#define _RIGHT  12

#define _TAREZERO 13
#define _ENTERTARE  14
#define _ENTERCLEAR  15
#define _TAREUNITS  16
#define _TAREPRINT  17

// key held for longer than 1 second
#define _ZERO_H     101
#define _TARE_H     102
#define _UNITS_H    103
#define _PRINT_H    104
#define _TARGET_H   106
#define _DOWN_H     107
#define _UP_H       108
#define _LEFT_H     109
#define _RIGHT_H    110
#define _ENTER_H    111
#define _ID_H       112
#define _CLEAR_H    113
#define _TAREZERO_H 114
#define _ENTERTARE_H    115
#define _ENTERCLEAR_H   116
#define _TAREUNITS_H   117
#define _TAREPRINT_H  118
#define _LEFTRIGHT_H  119

#define _EXTENDED 1
#define _NORMAL   0
#define _RESET    3

int scan_keypad(char mode);

#endif

