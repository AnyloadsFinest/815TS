
#define AUTOBUFF 80       // character RX circular buffer length

void load_stringformat(int select);
char auto_detect_string(void);
char searchchar(char string[], char ch);
char patternmatch(char string[],char frame,char pos,char ch1, char ch2);
char autolearn(void);
char learnstring(void);
void learn_scan(void);
char searchframe( char string[], char frame);
char learnbaud(void);
char assistedlearn(void);
char presetlearn(void);
void test_scan(void);