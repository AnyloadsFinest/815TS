
#define WAIT_FOR_FRAME_START 0
#define COUNT_IN_STRING 1
#define PARSE_STRING 2
#define WAIT_FOR_FRAME_END 3

#define BUFLEN 50
#define CMDLEN 50

#define _PARSE_SCAN    0
#define _PARSE_PRESET  1
#define _PARSE_USER    2

#define _NULL 0
#define _SOH  1
#define _STX  2
#define _ETX  3
#define _EOT  4
#define _CR   13
#define _LF   10


void parsestring(void);
void scan_character(char ch);
void autoscan(void);
void  scan_character_manual(char ch);
void parse_manual(void);
void Manual_Learn(void);
void autoscan2(void);
void autoscan3(void);
void simplescan(void);
char checksum(char length, char string[]);
char parse_command(char cmd);
void parse_traffic(char cmd);
char checksumbyte(char length, char string[]);
char parse_stoploght(void);
