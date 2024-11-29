#include "includes.h"

char str[AUTOBUFF + 1]; // string buffer allocated in EDATA

/*
 LOOKUP TABLE FOR STRING DEFINITIONS STORED IN ROM MEMORY
 String format defintions are in a table below. A string format defintion tells how to parse a string.
 All status character must in UPPER case.

 FRAME;    character used to mark beginning of character frame
 WEIGHT;   position in string where the weight string starts
 CNT;      how many characters to sample after the FRAME character
 LENGTH;   length of weight string

 UPOS;     position of units character (K=Kgs and L=Lbs)
 SPOS;     status character position, G=gross, N=net
 PPOS;     polarity character position 
 OPOS;     position of over cap
 MPOS;     position of motion character

 BINARY;   flag that indicates that the string uses binary status bits (i.e. TOLEDO strings)

 KGmask;   KG mask to identify KG status character
 LBmask;   LB mask to identify LB status character
 Nmask;    NET mask to idesntify  NET mode character
 Gmask;    GROSS mask to identify GROSS mode character
 Pmask;    Polarity mask to identify negative weight sign character
 Omask;    Over capacity
 Mmask;    Motion detection

  char FRAME;   // character used to mark beginning of character frame
  char WEIGHT;  // position in string where the weight string starts
  char CNT;     // how many characters to sample in after the FRAME character
  char LENGTH;  // length of weight string
  char UPOS;    // position of units character (K=Kgs and L=Lbs)
  char SPOS;    // status character position, G=gross, N=net
  char PPOS;    // polarity character position - = negative
  char BINARY;  // flag that indicates that the string uses binary status bits
  char KGmask;  // KG mask to identify KG status
  char LBmask;  // LB mask to identify LB status
  char Nmask;   // NET mask to idesntify NET mode status
  char Gmask;   // GROSS mask to identify GROSS mode status
  char Pmask;   // Polarity mask to identify negative weight
  char OPOS;    // Scale over status position
  char Omask;   // Scale motion character
  char MPOS;    // Motion character position
  char Mmask;   // Motion character mask
  char AUTOSCAN;// flag to indicate that we are auto scanning the string instead of using a table to parse
*/ 

// string formats are stored in rom                             
// to add a new string format, add a new line at the end of the table below.
static char  stringformat[]={

//0. anyload 805
//FRAME, WEIGHT,  CNT, LEN, UPOS, SPOS, PPOS, BIN, KGmask, LBmask, Nmask, Gmask, Pmask  , OPOS, Omask, MPOS, Mmask  
   0x02  ,    3,    14,   7,    10,   11,    2,   0,    'K',    'L',   'N',   'G',   '-',  12  ,   'O'  ,  12  ,  'M',
  
//1.  DF1500
//FRAME, WEIGHT,  CNT, LEN, UPOS, SPOS, PPOS, BIN, KGmask, LBmask, Nmask, Gmask, Pmask  , OPOS, Omask, MPOS, Mmask                    
  0x02  ,    3,    19,   7,    11,   14,    2,   0,    'K',    'L',   'N',   'G',   '-',  17  ,   'O'  ,  17  ,  'M',

//2.  CONDEC
//FRAME, WEIGHT,  CNT,  LEN, UPOS, SPOS, PPOS, BIN, KGmask, LBmask, Nmask, Gmask, Pmask  , OPOS, Omask, MPOS, Mmask                       
  0x02  ,    3,    20,    7,    10,   11,    2,   0,    'K',    'L',   'N',   'G',   '-',  12  ,   'O'  ,  12,   'M',

//3.  TOLEDO 8140,8142
// also added to pickup decimals from status byte in parse_string
//FRAME, WEIGHT,  CNT,  LEN, UPOS, SPOS, PPOS, BIN, KGmask, LBmask, Nmask, Gmask, Pmask  , OPOS, Omask, MPOS, Mmask                       
  0x02   ,    5,    20,    6,    3,   3,    3,   1,     16,     0,     1,     0,    2    ,  3  ,   4 ,  3  ,      8,
  
//4.  CARDINAL SB-200  
//FRAME, WEIGHT,  CNT,  LEN, UPOS, SPOS, PPOS, BIN, KGmask, LBmask, Nmask, Gmask, Pmask  , OPOS, Omask, MPOS, Mmask                       
  0x0D   ,    3,    17,    7,   12,   15,    2,   0,    'K',    'L',   'N',   'G',   '-' ,   10  ,   'O'  ,  0 , 'M',

//5.  CARDINAL SB-400 with decimals
//FRAME, WEIGHT,  CNT,  LEN, UPOS, SPOS, PPOS, BIN, KGmask, LBmask, Nmask, Gmask, Pmask  , OPOS, Omask, MPOS, Mmask                       
  0X0D   ,    3,    18,    7,   11,   14,    2,   0,    'K',    'L',   'N',   'G',   '-' ,  17  ,   'C'  ,  0,  'M', 

//6.  CARDINAL 748 without decimals
//FRAME, WEIGHT,  CNT,  LEN, UPOS, SPOS, PPOS, BIN, KGmask, LBmask, Nmask, Gmask, Pmask  , OPOS, Omask, MPOS, Mmask                      
  0X0D   ,     3,    18,    7,   10,   14,    2,   0,    'K',    'L',   'N',   'G',   '-' ,  0  ,   'O'  ,  0  , 'M',
  
//7.  FAIRBANKS scoreboard string 
//FRAME, WEIGHT,  CNT,  LEN, UPOS, SPOS, PPOS, BIN, KGmask, LBmask, Nmask, Gmask, Pmask  , OPOS, Omask, MPOS, Mmask                       
  0x02   ,   5,    11,    6,    3,    3,    4,   1,      0,      0,     0,    0,   '-'    ,  0  ,   'O'  ,  0  ,  'M',

//8.  CARDINAL SMA format
//FRAME, WEIGHT,  CNT,  LEN, UPOS, SPOS, PPOS, BIN, KGmask, LBmask, Nmask, Gmask, Pmask  , OPOS, Omask, MPOS, Mmask                      
  0X0D   ,     11,    20,    7,   18,   5,    2,   0,    'K',    'L',   'N',   'G',   '-'  ,  3  ,   'O' ,  6  , 'M',

//9.  RINSTRUM 1500 format B
//FRAME, WEIGHT,  CNT,  LEN, UPOS, SPOS, PPOS, BIN, KGmask, LBmask, Nmask, Gmask, Pmask  , OPOS, Omask, MPOS, Mmask                      
  0x02   ,     5,    14,    7,   12,   2,    3,   0,    'K',    'L',   'N',   'G',  '-'  ,  2  ,   '0' ,  2  ,  'M',

//10.  Western Scalelink Advance
//FRAME, WEIGHT,  CNT,  LEN, UPOS, SPOS, PPOS, BIN, KGmask, LBmask, Nmask, Gmask, Pmask  , OPOS, Omask, MPOS, Mmask                      
  0X02,     3,     24,    7,   11,   14,    2,   0,    'K',    'L',   'N',   'G',   '-'   ,  17  ,  'O' ,  17 , 'M',

//11.  Western M1 Advance (old)
//FRAME, WEIGHT,  CNT,  LEN, UPOS, SPOS, PPOS, BIN, KGmask, LBmask, Nmask, Gmask, Pmask  , OPOS, Omask, MPOS, Mmask                      
  0X02,     3,     31,    7,   19,   22,    2,   0,    'K',    'L',   'N',   'G',   '-',     25,   'O',   25,   'M',

//12.  Western M1 Advance ( new id and checksum)
//FRAME, WEIGHT,  CNT,  LEN, UPOS, SPOS, PPOS, BIN, KGmask, LBmask, Nmask, Gmask, Pmask  , OPOS, Omask, MPOS, Mmask                      
  0X02,     3,    33,    7,   19,   22,    2,   0,    'K',    'L',   'N',   'G',   '-',     25,   'O',   25,   'M',

//13.  Weghtronix 120
//FRAME, WEIGHT,  CNT,  LEN, UPOS, SPOS, PPOS, BIN, KGmask, LBmask, Nmask, Gmask, Pmask  , OPOS, Omask, MPOS, Mmask                      
   0X0D,     4,     15,    6,   11,   2,    3,   0,    'K',    'L',   'N',   'G',   '-',   0,     'O',     0,    'M',

//14.  Analogix 5316
//FRAME, WEIGHT,  CNT,  LEN, UPOS, SPOS, PPOS, BIN, KGmask, LBmask, Nmask, Gmask, Pmask  , OPOS, Omask, MPOS, Mmask                      
   0x02,     2,     18,    9,   0,   0,    0,   1,    0,    0,      0,       0,    '-',     0,    'O',     0,    'M',

//15.  Cardinal 738
//FRAME, WEIGHT,  CNT,  LEN, UPOS, SPOS, PPOS, BIN, KGmask, LBmask, Nmask, Gmask, Pmask  , OPOS, Omask, MPOS, Mmask                      
  0X0D   ,     3,    20,    7,   13,   16,    2,   0,    'K',    'L',   'N',   'G',   '-' ,   0  ,   'O' ,  0  ,  'M',

//16.  Cardinal 777
//FRAME, WEIGHT,  CNT,  LEN, UPOS, SPOS, PPOS, BIN, KGmask, LBmask, Nmask, Gmask, Pmask  , OPOS, Omask, MPOS, Mmask                      
  0x01   ,    5,   15,   4,   9,    11,    0,   0,    'K',    'L',   'N',   'G',   '-' ,    0  ,   'O'  ,  0  ,  'M',

//17.  FMT.E Ultrahawk
//FRAME, WEIGHT,  CNT,  LEN, UPOS, SPOS, PPOS, BIN, KGmask, LBmask, Nmask, Gmask, Pmask  , OPOS, Omask, MPOS, Mmask                      
  0x02   ,    3,    19,   7,   12,   15,    2,   0,    'K',    'L',   'N',   'G',   '-' ,   10  ,  'C'  ,  10 , 'M',

//18.  FMT.B Ultrahawk
//FRAME, WEIGHT,  CNT,  LEN, UPOS, SPOS, PPOS, BIN, KGmask, LBmask, Nmask, Gmask, Pmask  , OPOS, Omask, MPOS, Mmask                      
  0x02   ,    4,   16,   7,   12,    2,    3,   0,    'K',    'L',   'N',   'G',   '-' ,    2  ,  'O'  ,  2 , 'M',
  
//19.  FMT.C Ultrahawk
//FRAME, WEIGHT,  CNT,  LEN, UPOS, SPOS, PPOS, BIN, KGmask, LBmask, Nmask, Gmask, Pmask  , OPOS, Omask, MPOS, Mmask                      
  0x02   ,    3,   18,   7,   15,   10,    2,   0,    'K',    'L',   'N',   'G',   '-' ,    10  ,  'O'  ,  11 , 'M',  

//20.  Weighshark
//FRAME, WEIGHT,  CNT,  LEN, UPOS, SPOS, PPOS, BIN, KGmask, LBmask, Nmask, Gmask, Pmask  , OPOS, Omask, MPOS, Mmask                      
  0X0D   ,    2,     10,   6,   8,     0,     0,   0,   'K',    'L',   'N',   'G',   '-',      0,    '0' ,  0 ,  'M',  

//21.  Fairbanks new
//FRAME, WEIGHT,  CNT,  LEN, UPOS, SPOS, PPOS, BIN, KGmask, LBmask, Nmask, Gmask, Pmask  , OPOS, Omask, MPOS, Mmask                      
  0x02,   5,      11,    6,   0,   0,     4,    1,      0,      0,    0,      0,   '-'  ,  0  ,   'O'  ,  0  ,  'M',

//22.  DF2000 and M2000 multi channel
//FRAME, WEIGHT,  CNT,  LEN, UPOS, SPOS, PPOS, BIN, KGmask, LBmask, Nmask, Gmask, Pmask  , OPOS, Omask, MPOS, Mmask                      
  0X02  ,    4,    19,   6,   11,    14,   2,   0,   'K',    'L',    'N',     'G',  '-',     17  ,  'O' ,  17 , 'M',  

//23.  Bilanchai / Analogx with tare
//FRAME, WEIGHT,  CNT,  LEN, UPOS, SPOS, PPOS, BIN, KGmask, LBmask, Nmask, Gmask, Pmask  , OPOS, Omask, MPOS, Mmask                      
  0X02  ,    2,    24,   9,   21,    22,  2,    0,   'K',    'L',   'N',     'G',   '-',     0  ,  'O' ,  0 ,   'M',  

//24.  CAS
//FRAME, WEIGHT,  CNT,  LEN, UPOS, SPOS, PPOS, BIN, KGmask, LBmask, Nmask, Gmask, Pmask  , OPOS, Omask, MPOS, Mmask                      
  0X0D  ,    10,    22,   8,  19,    4,   10,   0,   'K',    'L',    'N',     'G',  '-',      1  ,   'O' , 1 ,   'U',  

//25.  A&D 5000
//FRAME, WEIGHT,  CNT,  LEN, UPOS, SPOS, PPOS, BIN, KGmask, LBmask, Nmask, Gmask, Pmask  , OPOS, Omask, MPOS, Mmask                      
  0X0D  ,    10,    22,   8,   19,    4,   10,   0,   'K',    'L',    'N',     'G',  '-',     1  ,  'O' ,  1 ,  'U',  

//26.  Ranger.A
//FRAME, WEIGHT,  CNT,  LEN, UPOS, SPOS, PPOS, BIN, KGmask, LBmask, Nmask, Gmask, Pmask  , OPOS, Omask, MPOS, Mmask                     
  0x02,      3,    11,   7,   10,   10,    2,   0,   'K',     'L',   'N',    'G',   '-'   ,  10  , 'O'  ,  10,  'M',  

//27.  Ranger.B 
//FRAME, WEIGHT,  CNT,  LEN, UPOS, SPOS, PPOS, BIN, KGmask, LBmask, Nmask, Gmask, Pmask  , OPOS, Omask, MPOS, Mmask                     
  0x02,      4,    14,   7,   12,   2,    3,    0,     'K',    'L',   'N',    'G',  '-' ,    2,   'O' ,  2,    'M',  
  
//28.  Ranger.C 
//FRAME, WEIGHT,  CNT,  LEN, UPOS, SPOS, PPOS, BIN, KGmask, LBmask, Nmask, Gmask, Pmask  , OPOS, Omask, MPOS, Mmask                     
  0x02,      3,    17,   7,   15,   9,    2,    0,     'K',   'L',   'N',     'G',  '-'  ,  10  , 'O'  , 10  ,  'U',  
  
//29.  Ranger.D 
//FRAME, WEIGHT,  CNT,  LEN, UPOS, SPOS, PPOS, BIN, KGmask, LBmask, Nmask, Gmask, Pmask  , OPOS, Omask, MPOS, Mmask                     
  0x02,      3,    10,   7,   0,    0,    2,    0,     0,      0,     0,      0 ,  '-'  ,    0  ,  'O'  ,   0 ,  'M',  

//30.  Schenk without DP 
//FRAME, WEIGHT,  CNT,  LEN, UPOS, SPOS, PPOS, BIN, KGmask, LBmask, Nmask, Gmask, Pmask  , OPOS, Omask, MPOS, Mmask                     
  0x02,      6,    24,   6,   0,    0,    5,    0,     0,      0,     0,      0 ,  '-'  ,    0  ,   'O' ,   0 ,  'M',  

//31.  Schenk with DP 
//FRAME, WEIGHT,  CNT,  LEN, UPOS, SPOS, PPOS, BIN, KGmask, LBmask, Nmask, Gmask, Pmask  , OPOS, Omask, MPOS, Mmask                     
  0x02,      6,    25,   6,   0,    0,    5,    0,     0,      0,     0,      0 ,  '-'  ,    0  ,    'O',   0 ,  'M',  

//32.  AWTX 125 M1-12
//FRAME, WEIGHT,  CNT,  LEN, UPOS, SPOS, PPOS, BIN, KGmask, LBmask, Nmask, Gmask, Pmask  , OPOS, Omask, MPOS, Mmask                      
   0X0D,     7,     19,    6,   15,   5,    6,    0,    'K',    'L',   'N',   'G',   '-',     17,     'O',  17,   'M',

//33.  Rice lake 920i command format
//FRAME, WEIGHT,  CNT,  LEN, UPOS, SPOS, PPOS, BIN, KGmask, LBmask, Nmask, Gmask, Pmask  , OPOS, Omask, MPOS, Mmask                      
  0X21,    7,     14,    7,   0,   0,     7,     0,    'K',    'L',   'N',   'G',   '-',     0,     'O',  0,   'M',
  
//34.  Rice lake 920i Display message command format
//FRAME, WEIGHT,  CNT,  LEN, UPOS, SPOS, PPOS, BIN, KGmask, LBmask, Nmask, Gmask, Pmask  , OPOS, Omask, MPOS, Mmask                      
  0X21,    20,     29,   7,   0,   0,     20,     0,    'K',    'L',   'N',   'G',   '-',     0,     'O',  0,   'M',  

//35.  Baykon
// also added to pickup decimals from status byte in parse_string
//FRAME, WEIGHT,  CNT,  LEN, UPOS, SPOS, PPOS, BIN, KGmask, LBmask, Nmask, Gmask, Pmask  , OPOS, Omask, MPOS, Mmask                       
  0x02   ,    5,    20,    6,    0,   0,    0,   1,     0,     0,     1,     0,    0    ,  0  ,   0 ,  0  ,      0,

//36.  Dini
// also added to pickup decimals from status byte in parse_string
//FRAME, WEIGHT,  CNT,  LEN, UPOS, SPOS, PPOS, BIN, KGmask, LBmask, Nmask, Gmask, Pmask  , OPOS, Omask, MPOS, Mmask                       
  0x0D   ,    9,    21,    8,  18,   0,    9,   0,     'K',     'L',    'N',   'G',    '-'    ,  4,   'V' ,  3, 'U',  

  
// reserved 33 to 39
//40.  NORAC special scoreboard string 
//FRAME, WEIGHT,  CNT,  LEN, UPOS, SPOS, PPOS, BIN, KGmask, LBmask, Nmask, Gmask, Pmask  , OPOS, Omask, MPOS, Mmask                     
  0x2A ,     2,    13,    8,   10,   10,    2,   1,     1,      0,     8,     0,   '-'  ,  0  ,   'O'  ,  0  ,   'M',  
  
  
/*
Fairbanks <STX><A><B><C><GGGGGG><TTTTTT><CR>
Toledo <STX><A><B><C><GGGGGG><TTTTTT><CR>
Cardinal <CR><P><WWWWWW><m><SP><U><SP><g><SP><SP><ETX>
Weightronix < ><M><WWWWWW>< ><U><CR><LF>
Condec <STX><P><WWWWWWW><U><G><M><C

ASCII Table
HEX DEC CHAR HEX DEC CHAR HEX DEC CHAR
00 000 NUL   2B 043  +   56  086   V
01 001 SOH   2C 044  ,   57  087   W
02 002 STX   2D 045   _  58  088   X
03 003 ETX   2E 046  .   59  089   Y
04 004 EOT   2F 047  /   5A  090  Z
05 005 ENQ   30 048  0   5B  091  [
06 006 AKC   31 049  1  5C  092   \
07 007 BEL   32 050  2  5D  093  ]
08 008 BS    33 051  3 5E   094   ^
09 009 HT   34 052 4 5F 095 -
0A 010 LF   35 053 5 60 096 `
0B 011 VT   36 054 6 61 097 a
0C 012 FF   37 055 7 ~ 62 098 b
0D 013 CR   38 056 8 63 099 c
0E 014 S0   39 057 9 64 100 d
0F 015 S1   3A 058 : 65 101 e
10 016 DLE  3B 059 ; 66 102 f
11 017 DC1   3C 060 < 67 103 g
12 018 DC2  3D 061 = 68 104 h
13 019 DC3  3E 062 > 69 105 I
14 020 DC4  3F 063 ? 6A 106 j
15 021 NAK  40 064 @ 6B 107 k
16 022 SYN  41 065 A 6C 108 l
17 023 ETB  42 066 B 6D 109 m
18 024 CAN  43 067 C 6E 110 n
19 025 EM   44 068 D 6F 111 o
1A 026 SUB  45 069 E 70 112 p
1B 027 ESC  46 070 F 71 113 q
1C 028 FS   47 071 G 72 114 r
1D 029 GS   48 072 H 73 115 s
1E 030 RS   49 073 I 74 116 t
1F 031 US   4A 074 J 75 117 u
20 032 Space 4B 075 K 76 118 v
21 033 !  4C 076 L 77 119 w
22 034 “  4D 077 M 78 120 x
23 035 #  4E 078 N 79 121 y
24 036 $  4F 079 O 7A 122 z
25 037 %  50 080 P 7B 123 {
26 038 &  51 081 Q 7C 124 |
27 039 ‘  52 082 R 7D 125 }
28 040 (  53 083 S 7E 126 ~
29 041 )  54 084 T 7F 127 Delete
2A 042 *  55 085 U

A&D Stream String:
<HDR1>,<HDR2>,SWWWWWWWUU<CR><LF>
HDR1 = Header 1 – OL (over/under load), ST (stable) or US (unstable)
HDR2 = Header 2 – GS (Gross mode), NT (Net mode) or TR (tare data)
(Note TR not supported in R420 main display switching, only as a fixed output)
< , > = comma 2CHEX
S = Sign either + or -
W = 7 Character weight data including the decimal point. If no decimal point then last
character is a period. Data sent with leading zero’s.
5000 String for A&D stream:
ASCII: 182,187,222,044,213,083,044,201,032,116,000
Start CHAR1 000, End CHAR1 013 and End CHAR2 010.

Western Stream string:

sprintf(outstring,"%c%c%7.7s %7.7s %s %s %c %c%c%c%c",0x02,polarity,wstring,tstring,units,mode,status,cz,sl,0x0d,0x0a);    
sprintf(outstring,"%c%c%7.7s %s %s %c%c%c%c%c",0x02,polarity,wstring,units,mode,status,cz,sl,0x0D,0x0A);       

//10.  Western (Scalelink) advance
//FRAME, WEIGHT,  CNT,  LEN, UPOS, SPOS, PPOS, BIN, KGmask, LBmask, Nmask, Gmask, Pmask  , OPOS, Omask, MPOS, Mmask                      
  0X02,     3,     24,    7,   11,   14,    2,   0,    'K',    'L',   'N',   'G',   '-'   ,  17  ,  'O' ,  17 , 'M',

//11.  Fairbanks new
//FRAME, WEIGHT,  CNT,  LEN, UPOS, SPOS, PPOS, BIN, KGmask, LBmask, Nmask, Gmask, Pmask  , OPOS, Omask, MPOS, Mmask                      
  0x02,   5,     11,    6,   0,   0,     4,    1,      0,      0,    0,      0,   '-'  ,  0  ,   'O'  ,  0  ,     0,

//12.  Western new Advance
//FRAME, WEIGHT,  CNT,  LEN, UPOS, SPOS, PPOS, BIN, KGmask, LBmask, Nmask, Gmask, Pmask  , OPOS, Omask, MPOS, Mmask                      
  0X02,     3,     31,    7,   19,   22,    2,   0,    'K',    'L',   'N',   'G',   '-',     25,   'O',   25,   'M',
//////////////////////////////////////////////////////////////////////////////////////////////////////////
P5.3 Data String Emulation COM1

0 = Western DF1500 <
1 = Toledo
2 = Toledo w/ checksum
3 = Toledo w/ checksum & leading zeros
4 = RiceLake / Condec
5 = Transcell
6 = Cardinal SB400
7 = Cardinal SB200
8 = SMA
9 = A&D 4323
10 = Ohaus CW11
11 = Fairbanks R2500
12 = Weightronix WI125
13 = Transmit value in ACC5
14 = Western M1

Data String emulations for COM1:
Western DF1500 format (19 characters):
<STX>PWWWWWWWUU_MM_S<CR><LF>
<STX> = Start of Text character (ASCII 02)
P = Polarity: ‘-‘ for negative, space for positive.
W = Weight: 7 characters incl. decimal.
UU = Units: ‘KG’, ‘LB’, ‘OZ’, ‘ G’
_ = Space (ASCII 32)
MM = Mode: “GR” or “NT”
S = Status: ‘M’ for motion, ‘O’ for scale over, or
===’space’ for valid
<CR> = Carriage Return (ASCII 13)
<LF> = Line Feed (ASCII 10)
///////////////////////////////////////////////////
*/

}; // 
  
 
//------------------------------------------------------------------------------------------------------------
//this function is an endless loop that exits when the string is learned
//------------------------------------------------------------------------------------------------------------
char learnbaud(void)
{
// char len;
int stat;

    WDTFeed( );       // watch dog timer
    
    stat = auto_learn_baud();   // scan through all baud rates to detect baud rate
   
    if (stat) 
    {      
     return(1);
    } 
    
    error_message(1); // send error message that no baudrate could be detected
    return(0);
 
// 
} // end of function



char learnstring(void)
{
// char len;
char stat;

    
    WDTFeed( );       // watch dog timer
    
    stat = auto_detect_string();       // scan and detect strings    
     
    if (stat) 
    {      
      return(1);
    }  //
   
   error_message(2); // send error message that no baudrate could be detected
   return(0);
  
} // end of function



void learn_scan()
{// start of learn
    
                if(scan_learn())  // are we in learn mode
                 { //               
                 text_message("LEARN ",1);
                 //                
                 if(autolearn())                   // auto learn the scoreboard
                 {//1                  
                  BOARD.NOLEARN = 1;
                  
                  CAL.Nolearn =1;
                  CAL.BaudSet = 1;
                                  
                  flash_save_cal();              // save the settings to flash 
                  text_message("LOCED ",1);       // 
                  BOARD.BLINK = 3;  
                  }//1
                       
                 }// if there           
               
}// end of learn function

void test_scan()
{// start of learnt
                
                if(scan_fen())  // are we in learn mode
                 { //                 
                  text_message("TESTD ",1);     
                  demo_display();
                  BOARD.BLINK = 3; 
                 //  autoparse_string_simple_scan();//                                    
                 }//        
                 
 // 
}// end of learn function
//-----------------------------------------------------------------------------------------------------------
// This function will loop and try to autolearn the string to setup the scoreboard
// Returns a 1 if the autolearn was successfull, and a 0 for fail
//
//------------------------------------------------------------------------------------------------------------
char presetlearn(void)
{
// we are not autolearning on startup 
  if (BOARD.NOLEARN == _PARSE_USER)       // auto learn the scoreboard on startup
  {  
  CAL.AutoScan = 1;
  PARSE.AUTOSCAN = 1; // set flag to identify auto scanning the string  
  PARSE.CNT =    CAL.ParseCnt;   // set how many characters there are in a string
  PARSE.FRAME =  CAL.ParseFrame; // set the character used to frame the string
  PARSE.WEIGHT = CAL.ParseWeight;  
  PARSE.LENGTH = CAL.ParseLength; 
  return(1);
  } // 
  
  CAL.AutoScan = 0;
  PARSE.AUTOSCAN = 0; // set flag to identify auto scanning the string  
  load_stringformat(CAL.StringType); // load the string        
  return(0);
  
} 
//-----------------------------------------------------------------------------------------------------------
// This function will loop and try to autolearn the string to setup the scoreboard
// Returns a 1 if the autolearn was successfull, and a 0 for fail
//
//------------------------------------------------------------------------------------------------------------
char autolearn(void)
{
int stat;
// disable UART interupts as we are reading characters directly from the UART, and throught the circular buffer
// CHECK BAUDRATE

while(1)  // endless loop to check for baud rate
{       
  // we scan the characters to look for digits and spaces in a row for baudrate detection
  // in some scoreboard modes we do not need to check the baudrate as they are fixed 
  // check if we are aborting autodetect
 WDTFeed( );          // watch dog timer
   
 if (BOARD.RADIO)// we have a radio installed, no need to check com ports further
  { //1  
   break;     // we are using the radio, no need to check baud rate as it is fixed at 9600n81
                // break out of while loop
  }// 1  
  
 if(CAL.BaudSet)
  {// manula baud rate setting
   break;  
  } // 
  
  // analyze baudratesssssss
 stat = auto_learn_baud();   // scan through all baud rates to detect baud rate
   
  if (stat)                // if baudrate scan failed
  { // k 
  break; // break out of loop, baudrate scan complete, stay in loop until baudrate detected 
  } // s


  error_message(1); // send error message that no baudrate could be detected
} // end of while baud rate detection loop

// detecting a baudrate will cause a break out of the while loop above
// we now have detected the baudrate, idetify the string
// we continue with the next while loop for detecting string
// AUTODETECT STRING
 stat = auto_detect_string();       // scan and detect strings  

 if (stat)
  {//1  
   return(1);  // could not detect weight string format
  }//1

   
 error_message(2);    
 return(0);

 }//5  end of while auto detect

char assistedlearn(void)
{
int stat;
char ch;  
   
   while(1)
    {
      ch = readcom0buffer();   // read the serial RX0 circular buffer 
      
      if(ch == 0)
      {
       break; 
      }
    }
  
     
  if (BOARD.RADIO) // we have a radio installed, no need to check com ports further
  { //1   
  stat = 1;                // break out of while loop
  }// 1  
  else
  {  
  // analyze baudrate
  stat = learnbaud();  
  }
  
  if (stat==0)               
  {  //   
   error_message(1); // send error message that no baudrate could be detected
   return(0);
   } // 
 
// detecting a baudrate will cause a break out of the while loop above
// we now have detected the baudrate, idetify the string
// we continue with the next while loop for detecting string
  // AUTODETECT STRING 
 stat = learnstring();   

 if (stat==0)
  {//1   
   error_message(2);
   return(0);
  }//1

 return(1);

} // end of function
 


//------------------------------------------------------------------------
//  Copy string parsing information for selected indicator
// "select" is the value passed to select the string format, startin from 1 and up
//  This functions calculated the index in the string parsing table and copies
//  the string parsing information to the parse structure used for parsing strings
//  The string parsing is stored as a table array in ROM memory.
//  Only the string parse information for a particular indicator is copied from the ROM table.
//------------------------------------------------------------------------
void load_stringformat(int select)
{
char * src;  // pointer to string tables in ROM
char * dst;  // pointer to PARSE structure where we are copying to
char i;
int index;
//  
src = &stringformat[0]; // get pointers
dst = &PARSE.FRAME;
//  
//index = select - 1;
index = select;
// there are 15 parameters used for string lookup
index=index * 17; // calculate index 13
// 
src=src+index; // set index to selected string format
//
for (i=0;i!=17;i++) *dst++=*src++; // copy the string lookup to the PARSE sructure
//        
} // end of function

// 
//--auto_detect_string()---------------------------------------------------------------------------------------
//  Load 80 characters from the com port and analyze the string to determine a valid string format.
//  Different string patterns are looked for different indicators.
//  This is part of the autolearn feature. It is used to analyze the indicator string and determine which 
//  string format to use to parse the string.
//-------------------------------------------------------------------------------------------------------------
char auto_detect_string(void)
{
// 
char ch=0; //  
char count=0;
char pos=0;
char match=0;
char frame=0;
char status=0;
//char length=0;
//char wieght=0;
//char len=0;
//char i=0;
           
// flush the buffer
while(1)
{
  ch = readcom0buffer();   // read the serial RX0 circular buffer 
  
  if(ch == 0)
  {
   break; 
  }
}// flush buffer first


// identify the string from the serial input, or the RADIO, depending which is used

for (pos=0; pos!=AUTOBUFF;pos++) str[pos]='8'; // init string
// fill string with characters from com port
timer(SCANTIMEOUT,START);  // start the scan timeout timer
                           // break out of loop if the no characters are detected
while(1) // endless loop, load in 80 characters
{ // 
WDTFeed( );          // watch dog timer
display_timer(START);
com_timeout_timer(START);
scan_system();             // breaks the loop to config 
status_light(STATUSLEARN);  // flash the status light fast to indicate autolearn

ch = readcom0buffer();   // read the serial RX0 circular buffer 
  // 
  if (ch) // check for receive flag
   {//1  
   ch=ch&0x7f;        // strip 8th bit to ignore odd or even parity
   str[count++]=ch;   // copy character to string
   } //1
  
  if (count==AUTOBUFF)  // check if we have filled string array with characters
   {
   str[count]=0; // null terminate
   break; // break out of loop
   } // 

if ( timer(SCANTIMEOUT,TIMEOUT) ) // check if we have timed out 
{
return(0);  // return with error
}

} // end of while


//start identifing different strings here.
string_init(0);

CAL.AutoScan=0;
PARSE.AUTOSCAN=0;  // set this flag to indicate that we are not parsing the string from a table lookup
                   // we will be parsing the string on the fly 
//************************ strings that have an STX in them *********************************************  
pos = searchchar(str,0x02);  // search string for STX character

  if (pos) // if there is a STX parse the string for and indicator match, for strings with STX
  {//1  
//test for Anyload 805--------------------------------------------------------------------------------
   match=0;

 if (patternmatch(str,pos,2 ,' ','-')) match++; // check for  polarity or space
 if (patternmatch(str,pos,2 ,'^',']')) match++; // check for  polarity or space
 if (patternmatch(str,pos,10,'K','L')) match++; // check for K or L
 if (patternmatch(str,pos,11,'G','N')) match++; // chck for G and N
 if (patternmatch(str,pos,12,'O','M')) match++; // status
 if (patternmatch(str,pos,12,' ','I')) match++; // status
  
   if (match == 4) // check if we have a pattern match for the Western scale series of indicators
    {
    load_stringformat(0); // load the string westernscale DF series
    CAL.StringType=0;
    return(1);
	}
    
// test for Western Scale--------------------------------------------------------------------------------
   match=0;

 if (patternmatch(str,pos,2 ,' ','-')) match++; // check for  polarity or space
 if (patternmatch(str,pos,11,'K','L')) match++; // check for K or L
 if (patternmatch(str,pos,16,' ',  0)) match++; // space
 if (patternmatch(str,pos,18, 13,  0)) match++; // CR

   if (match == 4) // check if we have a pattern match for the Western scale series of indicators
    {
    load_stringformat(1); // load the string westernscale DF series
    CAL.StringType=1;
    return(1);
	}
   
// test for Consolodated Controls------------------------------------------------
   match=0;
 // 
 if (patternmatch(str,pos,2 ,' ','-'))   match++; // check for  polarity or space
 if (patternmatch(str,pos,10,'K','L'))   match++; // check for K or L
 if (patternmatch(str,pos,12,' ', 0 ))   match++; // space
 if (patternmatch(str,pos,11,'G','N'))   match++; // CR
 //  
 if (match==4) // check if we have a pattern match for the Western scale series of indicators
    {
    load_stringformat(2); // load the string consolodated controls
    CAL.StringType = 2; //
    return(1);
    } // 
 
 /*
 // test forBaykon--------------------------------------------------
   match=0;
  if (patternmatch(str,pos,4 ,'0',0)) match++; // check for  polarity or space
  if (patternmatch(str,pos,17,13,0)) match++;    // check for CR
   // 
   if (match==2) // check if we have a pattern match for TOLEDO series of indicators
    {
    load_stringformat(35); // load the string Baykon
    CAL.StringType=35;
    return(1);
    } //   
 */
 
  // test for TOLEDO 8140, 8142 ------------------------------------------------
   match=0;
  //if (patternmatch(str,pos,2 ,' ','-')) match++; // check for  polarity or space
  if (patternmatch(str,pos,17,13,0)) match++;    // check for CR
   // 
   if (match==1) // check if we have a pattern match for TOLEDO series of indicators
    {
    load_stringformat(3); // load the string TOLEDO
    CAL.StringType=3;
    return(1);
    } // 
     
  // test for Rinstrum 5100-------------------------------------------------
   match=0;
   // if (patternmatch(str,pos,2 ,' ','-')) match++; // check for  polarity or space
   // if (patternmatch(str,pos,17,13,0)) match++;    // check for CR
   // match = searchchar(str,0x3);  // search string for STX character   
   if (patternmatch(str,pos,14,0x03,0)) match++;    // etx
   
   if (match==1) // check if we have a pattern match for Rinstrum series of indicators
    {
     load_stringformat(9); // load the string Rinstrum
     CAL.StringType=9;
     return(1);
    } // return(9); 
      //  
 
   // test for Advance Western String------------------------------------
  match=0;  
  if (patternmatch(str,pos, 2,' ','-')) match++; // check for  polarity or space
  if (patternmatch(str,pos,11,'K','L')) match++; // check for K or L
  if (patternmatch(str,pos,14,'G','N')) match++; // check for G or N
  if (patternmatch(str,pos,19,' ','&')) match++; // space
  if (patternmatch(str,pos,19,' ','*')) match++; // space
   
  
   if (match > 3) // check if we have a pattern match for the Western scale series of indicators
    {
    load_stringformat(10); // load the string westernscale DF series
    CAL.StringType=10;
    return(1);
	} //
 
 
  // test for Advance Western String M1 new with id------------------------------------
  match=0;
  if (patternmatch(str,pos, 2,' ','-')) match++; // check for  polarity or space
  if (patternmatch(str,pos,19,'K','L')) match++; // check for K or L
  if (patternmatch(str,pos,22,'G','N')) match++; // check for G or N
  if (patternmatch(str,pos,28,'&','*')) match++; // space
  if (patternmatch(str,pos,28,' ', 0 )) match++; // space
   
  if (patternmatch(str,pos,29,'0','1')) match++; // space 
  if (patternmatch(str,pos,29,'2','3')) match++; // space 
  if (patternmatch(str,pos,29,'4','5')) match++; // space 
  if (patternmatch(str,pos,29,'5','6')) match++; // space 
  if (patternmatch(str,pos,29,'7','8')) match++; // space 
  if (patternmatch(str,pos,29,'9',' ')) match++; // space 
   
  if (match > 4) // check if we have a pattern match for the Western scale series of indicators
    {
    load_stringformat(12); // load the string westernscale DF series
    CAL.StringType=12;
    return(1);
     } //   
   
     
  // test for Advance Western String M1 old no id-----------------------------------
  match=0;
  if (patternmatch(str,pos, 2,' ','-')) match++; // check for  polarity or space
  if (patternmatch(str,pos,19,'K','L')) match++; // check for K or L
  if (patternmatch(str,pos,22,'G','N')) match++; // check for G or N  
  if (patternmatch(str,pos,28,'&','*')) match++; // space
  if (patternmatch(str,pos,28,' ', 0 )) match++; // space   
  
   if (match > 3) // check if we have a pattern match for the Western scale series of indicators
    {
    load_stringformat(11); // load the string westernscale DF series
    CAL.StringType=11;
    return(1);
    } //   
   
   
 // Comment out to debug with fairbanks   
 // start  
 // test for FAIRBANKS scoreboard string -----------------------------------------------------
 match=0; // 
 //
 if (patternmatch(str,pos,4 ,' ','-')) match++; // check for  polarity or space 
 if (patternmatch(str,pos,1  ,0x02,0)) match++; // check for ETX character on 10th or 11th character
 if (patternmatch(str,pos,11 ,0x03,0)) match++; // check for ETX character 
 
 if (patternmatch(str,pos,2,'4',0)) match++; // check for first character
    if (patternmatch(str,pos,2,'5',0)) match++; // or check for first character
     if (patternmatch(str,pos,2,'6',0)) match++; // or check for first character
      if (patternmatch(str,pos,2,'7',0)) match++; // or check for first character
        if (patternmatch(str,pos,2,'8',0)) match++; // or check for first character
   
        //
        
   if (match > 3) // check if we have 2 pattern matches for Fairbanks scoreboard string
    {
  load_stringformat(7); // load the string FAIRBANKS
  CAL.StringType=7;                     
  return(1);
	} //      
   
  // test for New Fairbankls String-------------------------------------------------
  match=0;
  
  if (patternmatch(str,pos,4 ,' ','-')) match++; // check for  polarity or space
  if (patternmatch(str,pos,1 ,0x02,0)) match++; // check for STX
  if (patternmatch(str,pos,11,0x03,0)) match++; // check for ETX
   
   if (patternmatch(str,pos,2,'A','a')) match++; // check for first character
    if (patternmatch(str,pos,2,'B','b')) match++; // or check for first character
     if (patternmatch(str,pos,2,'C','c')) match++; // or check for first character
      if (patternmatch(str,pos,2,'D','d')) match++; // or check for first character
       if (patternmatch(str,pos,2,'E','e')) match++; // or check for first character
             //    
   if (match  > 3) // check if we have a pattern match for the Western scale series of indicators
    {
    load_stringformat(21); // load the string westernscale DF series
    CAL.StringType = 21;
    return(1);
	} //  
    
  // test for Analogix 5316----------------------------------
  match=0;
  if (patternmatch(str,pos,11,'1','2')) match++; // check for  polarity or space
  if (patternmatch(str,pos,11,'4','8')) match++; // check for K or L
  if (patternmatch(str,pos,12,'0','1')) match++; // check for G or N
  if (patternmatch(str,pos,14,13,0)) match++; // space
   
   if (match > 3) // check if we have a pattern match for the Western scale series of indicators
    {
    load_stringformat(14); // load the string westernscale DF series
    CAL.StringType=14;
    return(1);
	} // 
   
  // FM.E Ultra hawk----------------------------------
  match=0;
  
  if (patternmatch(str,pos,14,'_',0)) match++; // check for G or N
  if (patternmatch(str,pos,15,'G','N')) match++; // check for  polarity or space
  if (patternmatch(str,pos,10,'M','C')) match++; // check for K or L
  if (patternmatch(str,pos,12,'K','L')) match++; // check for G or N
  if (patternmatch(str,pos,18, 0x03,0)) match++; // space
   
   if (match > 2) // check if we have a pattern match for the Western scale series of indicators
    {
    load_stringformat(17); // load the string westernscale DF series
    CAL.StringType=17;
    return(1);
	} //  
   

     // FM.B Ultra hawk----------------------------------
  match=0;
  if (patternmatch(str,pos, 2,'G','N')) match++; // check for  polarity or space
  if (patternmatch(str,pos, 2,'M',' ')) match++; // check for K or L
  if (patternmatch(str,pos,12,'K','L')) match++; // check for G or N
  if (patternmatch(str,pos,15, 0x03,0)) match++; // space
   
   if (match > 3) // check if we have a pattern match for the Western scale series of indicators
    {
    load_stringformat(18); // load the string westernscale DF series
    CAL.StringType=18;
    return(1);
	} //  
   
  
   // FM.C Ultra hawk----------------------------------
  match=0;
  if (patternmatch(str,pos,10,'G','N')) match++; // check for  polarity or space
  if (patternmatch(str,pos,11,'M',' ')) match++; // check for K or L
  if (patternmatch(str,pos,15,'K','L')) match++; // check for G or N
  if (patternmatch(str,pos,17, 0x03,0)) match++; // space
   
   if (match > 3) // check if we have a pattern match for the Western scale series of indicators
    {
    load_stringformat(19); // load the string westernscale DF series
    CAL.StringType=19;
    return(1);
	} //  
  
   
  } //1              end of strings that have STX in them for framing
 
 pos = searchchar(str,0X01);  // search string for 0X01 character

 if (pos) // if there is a CR parse the string for and indicator match, for strings with CR
  {//2     
  match=0; // cardinla 777
 // 
 if (patternmatch(str,pos,1,0x01,0)) match++; // check for STX character on 1th 
 if (patternmatch(str,pos,15,0x04,0)) match++; // check for ETX character on 14th 
 if (patternmatch(str,pos,9,'L','K')) match++; // check for STX character on 1th 
 if (patternmatch(str,pos,11,'G','N')) match++; // check for ETX character on 14th 
 if (patternmatch(str,pos,12,'C',0)) match++; // check for ETX character on 14th
 
  if (match == 2) // check if we have 2 pattern matches for Fairbanks scoreboard string
    {
  load_stringformat(16); // load the string FAIRBANKS
  CAL.StringType=16;                  
  return(1);
	}//     
   
  }//2
 
 pos = searchchar(str,0X21);  // search string for 0X21 character

 if (pos) // if there is a CR parse the string for and indicator match, for strings with CR
  {//2     
   //test for Rice lake 920 format    
 match=0; // Rice lake 920  Command Format (7-Segment):|<AA><CC><Data>!
 // 
 if (patternmatch(str,pos,2,'|',0)) match++; // check for STX character on 1th 
 if (patternmatch(str,pos,5,'D',0)) match++; // check for ETX character on 14th 
 if (patternmatch(str,pos,6,'M',0)) match++; // check for ETX character on 14th
 

  if (match > 2) // check if we have 2 pattern matches for Fairbanks scoreboard string
    {
  load_stringformat(33); // load the string FAIRBANKS
  CAL.StringType=33;                  
  return(1);
	}//   

/*
  match=0; // Rice lake 920  Command Format (7-Segment):|<AA><CC><Data>!
 // 
 if (patternmatch(str,pos,2,'|',0)) match++; // check for STX character on 1th 
 if (patternmatch(str,pos,5,'D',0)) match++; // check for ETX character on 14th 
 if (patternmatch(str,pos,6,'M',0)) match++; // check for ETX character on 14th
 if (patternmatch(str,pos,7,'|',0)) match++; // check for ETX character on 14th 

  if (match > 3) // check if we have 2 pattern matches for Fairbanks scoreboard string
    {
  load_stringformat(34); // load the string FAIRBANKS
  CAL.StringType=34;                  
  return(1);
	}//   

*/ 
   
  }//2
  
  // end 
 //************************ strings that have an CR in them for framing ****************** 
  pos = searchchar(str,13);  // search string for CR character

  if (pos) // if there is a CR parse the string for and indicator match, for strings with CR
  {//2     
    //test for CARDINAL SMA format ---------------------------------------------------------------------
   match=0;
    //  
    if (patternmatch(str,pos,2 ,10,0)) match++;   // check for  polarity or space
    if (patternmatch(str,pos,21,13,0)) match++;   // space
    //  
    if (match==2) // check if we have a pattern match 
    {
    load_stringformat(8); // load the string for cardinal 738
    CAL.StringType=8;
    return(1);
	}//  
     
  //test for CARDINAL SB-200  738/748  scoreboard format------------------------
   match=0;

    if (patternmatch(str,pos,2 ,' ','-')) match++; // check for polarity or space
    if (patternmatch(str,pos,15,'G','N')) match++; // space
    if (patternmatch(str,pos,18,0x03,0)) match++;    // etx
    //  
    if (match==3) // check if we have a pattern match 
    {// 
    load_stringformat(4); // load the string for cardinal 738
    CAL.StringType=4;
    return(1); //
    } // 
   
 //test for CARDINAL SB-400    with decimal--------------------------------
   match=0;
    // 
    if (patternmatch(str,pos,2 ,' ','-')) match++; // check for  polarity or space
    if (patternmatch(str,pos,12,'G','N')) match++; // G/N
    if (patternmatch(str,pos,19,13,0))    match++; // cr
    // 
    if (match==3) // check if we have a pattern match 
    {
    load_stringformat(5); // load the string for cardinal 738    
    CAL.StringType=5;
  //  CAL.StringType=4; // changed in 4-24 and after
    return(1);
	} // 


//test for CARDINAL 748  without decimals --------------------------------------
    match=0;

    if (patternmatch(str,pos,2 ,' ','-')) match++; // check for  polarity or space
    if (patternmatch(str,pos,9 ,' ', 0 )) match++;   // space
    if (patternmatch(str,pos,10,'L','K')) match++;   // units
    if (patternmatch(str,pos,13,'G','N')) match++;  // gross net
  
    if (match==4) // check if we have a pattern match for the Western scale series of indicators
    {
    load_stringformat(5); // load the string for cardinal 748
    CAL.StringType=5;
    return(1);
	}//

//test for CARDINAL 748  with decimals -----------------------------------------
    match=0;

    if (patternmatch(str,pos,2 ,' ','-')) match++; // check for  polarity or space
    if (patternmatch(str,pos,10 ,' ',0))  match++;   // space
    if (patternmatch(str,pos,11,'L','K')) match++;   // units
    if (patternmatch(str,pos,14,'G','N')) match++;  // gross net
						
    if (match==4) // check if we have a pattern match for the Western scale series of indicators
    {
    load_stringformat(6); // load the string for cardinal 748
    CAL.StringType=6;
    return(1);
	}

    //test for AWTX 120 --------------------------------------------------------
   match=0;
    //  
    if (patternmatch(str,pos,11,'L','K')) match++;   // units
    if (patternmatch(str,pos,2,'G','N')) match++;  // gross net
    if (patternmatch(str,pos,3,'+','-')) match++;  // gross net
    //  
    if (match==3) // check if we have a pattern match 
    {
    load_stringformat(13); // load the string for cardinal 738
    CAL.StringType=13;
    return(1);
	}//  
 
    //test for Cardinal 738-----------------------------------------------------
    match=0;
    //  
    if (patternmatch(str,pos,19,0x03,0))  match++;   // units
    if (patternmatch(str,pos,16,'G','N')) match++;  // gross net
    if (patternmatch(str,pos, 2,'+','-')) match++;  // gross net
    if (patternmatch(str,pos,13,'L','K')) match++;   // units
    //  
    if (match==4) // check if we have a pattern match 
    {
    load_stringformat(15); // load the string for cardinal 738
    CAL.StringType = 15;
    return(1);
	}//  
   
    //test for Wigh Shark-------------------------------------------------------
    match=0;
    //      
    if (patternmatch(str,pos,8,'K','L')) match ++;  // gross net
    if (patternmatch(str,pos,9, 13, 0)) match ++;   // units
    //  
    if (match==2) // check if we have a pattern match 
    {
    load_stringformat(20); // load the string for cardinal 738
    CAL.StringType = 20;
    return(1);
	} // 
 
   //test for AWTX 125 ---------------------------------------------------------
   match=0;
    //  
    if (patternmatch(str,pos,15,'L','K')) match++;   // units
    if (patternmatch(str,pos, 5,'G','N')) match++;  // gross net
    if (patternmatch(str,pos, 6,' ','-')) match++;  // gross net
    //  
    if (match==3) // check if we have a pattern match 
    {
    load_stringformat(32); // load the string for cardinal 738
    CAL.StringType = 32;
    return(1);
	}//  
    
    
    //test for Dini ---------------------------------------------------------
   match=0;
    //  
    if (patternmatch(str,pos, 3,'S','U')) match++;   // units
    if (patternmatch(str,pos, 4,'S','T')) match++;   // units
    if (patternmatch(str,pos, 5,',',0)) match++;  // gross net
    if (patternmatch(str,pos, 6,'0',0)) match++;  // gross net
    //  
    if (match==4) // check if we have a pattern match 
    {
    load_stringformat(36); // load the string for cardinal 738
    CAL.StringType = 36;
    return(1);
	}//    
    
    
  }//2  // end of strings that use CR for framing

// autoscan for strings that we do not have in our lookup table  
//****************************************************************************
//  No match could be found at his point, so we set up the system for autoscan 
//  Scan for STX or CR and use those characters for framing
//**************************************************************************
CAL.AutoScan=1;
PARSE.AUTOSCAN=1;  // set this flag to indicate that we are not parsing the string from a table lookup
                   // we will be parsing the string on the fly 
status=0;

frame = 0x02;// STX
status =  searchframe( str, frame);
if(status !=0)
    return(1);

frame = 0X0D; // CR
status =  searchframe( str, frame);
if(status !=0)
    return(1);

frame = 0x01; // STH
status =  searchframe( str, frame);
if(status !=0)
    return(1);

frame = 0x03; // ETX
status =  searchframe( str, frame);
if(status !=0)
    return(1);

frame = 0x04; // EOT
status =  searchframe( str, frame);
if(status !=0)
    return(1);
    
frame = 0x0A; // LF
status =  searchframe( str, frame);
if(status !=0)
    return(1);
    
    //start identifing different strings here.
    string_init(1);    
    return(0); // fail, could not match string
    
} // end of function

//------------------------------------------------------------------------------
// character match
// Scan the string until it matches a character 
// retuns 0 for no match, otherwise it returns the character position
//------------------------------------------------------------------------------
char searchchar( char string[], char ch)
{
char i;

for (i=1;i!=AUTOBUFF;i++)
 {
  if (string[i]==ch) return(i);
 }
//  
 return(0); // search failed
// 

} // end of function
//  
// 
//-----------------------------------------------------------------------------------------------
// pattern match
// string: string to search through
// frame:  position in string where the string starts
// pos:    character position in the string to check
// ch1:    character 1 to compare to (uppercase)
// ch2:    character 2 to compare to (0 if not used)
// This function is used to identify that there is a pattern of characters that match in a string
//-----------------------------------------------------------------------------------------------
char patternmatch(char string[], char frame,char pos,char ch1, char ch2)
{
char match;
char index;
char chu;
//
match=0;
index = frame + pos - 1;
chu = toupper(string[index]);
// chu holds the uppercase character that we are checking for
// check if ch1 or ch2 matches the character pointed to by index
if (chu==ch1) match=1;  // check for a match on ch1
else
 if (ch2!=0) // check if ch2 is being used
   if (chu==ch2) match=1; // 
 
return(match);
} // end of function


//------------------------------------------------------------------------------
// character match
// Scan the string until it matches a character 
// retuns 0 for no match, otherwise it returns the character position
//------------------------------------------------------------------------------
char searchframe( char string[], char frame)
{
char pos=0;
char i=0;
char len=0;

pos = searchchar(str,frame);  // search string for CR character
  // 
if (pos) // seach for a repeat of the stx character
 {//3
   pos++;//  
   
   for (i=pos;i!=AUTOBUFF;i++)
   {//2
      if (str[i]==frame) 
      {//1 
      len = i - pos; // we have the second STX and we now calculate the length              
      PARSE.CNT   = len + 2; // immportant for the strings without STX on Feb 2011       
      PARSE.FRAME = frame; // frame character is CR
      PARSE.WEIGHT    = 1;
      PARSE.LENGTH    = PARSE.CNT;
                
      CAL.ParseCnt    = PARSE.CNT;
      CAL.ParseFrame  = PARSE.FRAME;
      CAL.ParseWeight = PARSE.WEIGHT;  
      CAL.ParseLength = PARSE.LENGTH;  
      return(1); // we are done searching for CR character
      }//1
   }//2
 }//3

 return(0); // search failed

} // end of function


