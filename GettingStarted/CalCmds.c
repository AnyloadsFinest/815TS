#include "includes.h"

int VALUE;
char CAL_PARAMETER;
extern long lTIMERTICS;

const char DONEmsg[]="-DONE-"; // message done
const char ERORmsg[]="-EROR-"; // message 
const char FAILmsg[]="-FAIL-"; // message 
const char SAVEmsg[]="-SAVE-"; // message 
const char SCANmsg[]="-SCAN-"; // message 

// values for baudrate P5.1
const  int BAUDprm[]={9,300,600,1200,2400,4800,9600,19200,32800,57600}; // baud rate values for UART DIVISOR registor
const  char BAUDstr[][6]={"   300","   600","  1200","  2400","  4800","  9600"," 19200"," 32800"," 57600"};  // displayed settings

const  int WBAUDprm[]={4,2400,4800,9600,19200}; // baud rate values for UART DIVISOR registor
const  char WBAUDstr[][6]={"  2400","  4800","  9600"," 19200"};  // displayed settings

// values for scale COM1 P5.1
const  int  BITSprm[]={3,0,1,2};
const  char BITSstr[][6] ={"8-NONE","7-EVEN"," 7-ODD"};  // displayed settings

// values for parameter 1.4
const  int  FRAMEprm[]={6,01,02,03,04,10,13}; //0.5,1,2,3
const  char FRAMEstr[][6] ={"    01","    02","    03","    04","    10","    13"};  // displayed settings

// values for parameter 1.4
const  int  DISPLAYprm[]={4,1,2,3,4}; //0.5,1,2,3
const  char DISPLAYstr[][6] ={"  0.25","  0.50","  0.75","  1.00"};  // displayed settings

// values for parameter 1.4
const  int  TIMOUTprm[]={6,1,2,3,4,5,6}; //0.5,1,2,3
const  char TIMOUTstr[][6] ={"    10","    20","    30","    40","    50","    60"};  // displayed settings

// values for parameter 1.4
const  int  TIMOUTCprm[]={5,0,2,3,6,9,10}; //0.5,1,2,3
const  char TIMOUTCstr[][6] ={"  0.00", "  0.25","  0.50","  1.00","  1.50"};  // displayed settings

//values for decimals menu parameter P1.1
const int   DECprm[]={4,0,1,2,3};             // decimal position menu
const  char DECstr[][6] ={"  AUTO","   0.0","  0.00","  NONE"};  // displayed settings

//values for decimals menu parameter P1.1
const int   UNITprm[]={4,0,1,2,3};             // decimal position menu
const  char UNITstr[][6] ={"  AUTO","    LB","    KG","  NONE"};  // displayed settings

//values for decimals menu parameter P1.1
const int   TAREprm[]={4,0,1,2,3};             // decimal position menu
const  char TAREstr[][6] ={"  AUTO","    GR","    NT","  NONE"};  // displayed settings

//values for decimals menu parameter P1.1
const int   LEADprm[]={3,0,1,2};             // decimal position menu
const  char LEADstr[][6] ={"  AUTO","    00","   000"};  // displayed settings

// values for scale over P1.5
const  int  SCLOVERprm[]={4,0,1,2,3}; // 0,off,1d,2d,2%
const  char SCLOVERstr[][6] ={"  OCAP","  OVER"," RANGE"," ERROR"};  // displayed settings

// values for scale over P1.5
const  int  PSAVEprm[]={4,0,1,2,3}; // 0,off,1d,2d,2%
const  char PSAVEstr[][6] ={"  NONE","  LEV1","  LEV2","  LEV3"};  // displayed settings

// values for scale over P1.5
const  int  ESAVEprm[]={2,0,1}; // 0,off,1d,2d,2%
const  char ESAVEstr[][6] ={"     1","     0"};  // displayed settings

// values for scale over P1.5
const  int  CYCLEprm[]={2,0,1}; // 0,off,1d,2d,2%
const  char CYCLEstr[][6] ={"     1","     0"};  // displayed settings

// values for scale over P1.5
const  int  BRIGHTprm[]={4,0,1,2,3}; // 0,off,1d,2d,2%
const  char BRIGHTstr[][6] ={"    20","    50","    80","   100"};  // displayed settings

// values for scale over P1.5t
const  int  PORTprm[]={4,0,3,2,1}; // 0,off,1d,2d,2%
const  char PORTstr[][6] ={"RS-232","RS-422","CL-020","RF-900"};  // displayed settings

// values for scale over P1.5
const  int  PORTIprm[]={4,0,2,3,1}; // 0,off,1d,2d,2%
const  char PORTIstr[][6] ={"PORT 1","PORT 2","PORT 3"," RADIO"};  // displayed settings

// values for scale over P1.5
const  int  PORTOprm[]={4,0,1,2,3}; // 0,off,1d,2d,2%
const  char PORTOstr[][6] ={"  NONE","RS-232","RS-422","  BOTH"};  // displayed settings

// values for scale over P1.5
const  int  STOPprm[]={4,0,1,2,3}; // 0,off,1d,2d,2%
const  char STOPstr[][6] ={"  AUTO","   RED"," GREEN","  NONE"};  // displayed settings

const  int  LEARNprm[]={3,0,1,2}; // 0,off,1d,2d,2%
const  char LEARNstr[][6] ={"   ART","   DIR","   SET"};  // displayed settings

const  int  BEARNprm[]={2,0,1}; // 0,off,1d,2d,2%
const  char BEARNstr[][6] ={"  AUTO","  BAUD"};  // displayed settings

const  int  COLONprm[]={2,0,1}; // 0,off,1d,2d,2%
const  char COLONstr[][6] ={"   DOT"," COLON"};  // displayed settings

const  int  DASHprm[]={2,0,1}; // 0,off,1d,2d,2%
const  char DASHstr[][6] ={"  DASH"," BLANC"};  // displayed settings

const  int  ACTIVprm[]={2,0,1}; // 0,off,1d,2d,2%
const  char ACTIVstr[][6] ={" PASIV"," ACTIV"};  // displayed settings


const  int  OCAPprm[]={5,0,1,2,3,4}; // 0,off,1d,2d,2%
const  char OCAPstr[][6] ={"  AUTO","    OU","    CC","    EE","  NONE", };  // displayed settings

// values for filtering P4.0,P4.1,P4.2,P5.3
const int CAL1prm[]={6,10,20,30,40,50,60};
const int CAL2prm[]={6,2,4,8,16,32,64};
const int CAL3prm[]={6,1,5,10,20,40,80};

const char CALPRM[]={ 
                     // parameters related to General Setup
                     10, // factory setting    
                     11, // config lock out
		     12, // day night auto
                     13, // day time britghness
		     14, // night brightness   
                     15, // no startup		     
		     16, // power saver      
                     17, // timer dealy com time out
                     18, // timer dealy refresh time
		     19, // mirror digits        
                                           
                     // paramters related to string setting
                     20,  // utility program
                     21,  // mutidrop id
                     22,
                     23,
                     24,
                     25,
                     26,
                     27,
                     28,
                     29,
                     
                     // paramters related to string setting
                     30,  // 
                     31,  // String Length 
                     32,  // Weight position
                     33,  // Weight Length 
                     34,  // Framing Character
                     35,  // Decimal
                     36,  // Status
                     37,  // Unit
                     38,  // Leading Zero
                     39,  // Over Limit 
		   
                     // paramters related to serial communication
		     40, // Auto Learn
                     41, // Baudrate
                     42, // parity
                     43, // Echo Out
                     44, // no dash
                     45, // active and passive
                     46, // radio
                     47, // comport
                     48, // baud
                     49, // string counter       
                     
		     // paramters related to serial communication
		     50, // Auto Learn
                     51, // Baudrate
                     52, // parity
                     53, // Echo Out
                     54, // no dash
                     55, // active and passive
                     56, // radio
                     57, // comport
                     58, // baud
                     59, // string counter                                       
                  
                     // paramters related to serial communication
		     90, // Auto Learn
                     91, // Baudrate
                     92, // parity
                     93, // Echo Out
                     94, // no dash
                     95, // active and passive
                     96, // radio
                     97, // comport
                     98, // baud
                     99, // string counter  
                     
                       };


void calibration_command(int cmd)
{ 
  
 cmd = CALPRM[CAL_PARAMETER];  // get calibration value we have selected  
 
 switch(cmd)
  { // 
    // #####################  General Setup Sub block 1.x ##################################  // 
    case 10: {  // 1.0  default setting 
               
               VALUE = 0;
               VALUE = option_select_count(VALUE,0,1,"");  
                         
               if( VALUE == 1 )
                {  
                 CAL.FirstTime= 0x55;  // flag to indicate that the FLASH needs to be initialized                
             
                 system_init(1);                
                 string_init(1);  // reset string
                 text_message("FACTRY",1);      
                }//  
               
                break;               
             }   //        
  
  
  case 11: {  // 1.1   lock up config 
            CAL.Config = option_select_count( CAL.Config,0,1,""); 
        
            if(  CAL.Config == 1 )
                {  
                 text_message("PRVENT",1);      
                }             
            break;
            }// 
            
      case 12: { // 1.4 atutomatic power save
             //CAL.Psave=menuselect(  CAL.Psave,(int *) &PSAVEprm ,&PSAVEstr[0][0]);   
             CAL.Psave = option_select_count(CAL.Psave,0,3,"");
             BOARD.PSAVE = CAL.Psave; 
             break;
             }//                
  
    case 13: {  // 1.2   timer seting 
            CAL.BatterySaver=menuselect( CAL.BatterySaver,(int *) &ESAVEprm ,&ESAVEstr[0][0]);        
            //CAL.BatterySaver = option_select_count(CAL.BatterySaver,0,1,"");             
            BOARD.BATTERYSAVER = CAL.BatterySaver;    
            break;
            }//CAL.ComTimeout
            
  case 14: {  // 1.2  daytime brightness
             CAL.Dbright=menuselect( CAL.Dbright,(int *) &BRIGHTprm ,&BRIGHTstr[0][0]);          
            // CAL.Dbright = option_select_count(CAL.Dbright,0,3,"");    
             BOARD.DBRIGHT = CAL.Dbright;
             break; // 
             } // 
             
    case 15: {  // 1.3  night time brightness             
             CAL.Nbright=menuselect( CAL.Nbright,(int *) &BRIGHTprm ,&BRIGHTstr[0][0]);   
             // CAL.Nbright = option_select_count(CAL.Nbright,0,3,"");    
             BOARD.NBRIGHT = CAL.Nbright; 
             break;
             }//  
	          
  case 16: {  // 1.5  no start up
             CAL.NoStartup=menuselect( CAL.NoStartup,(int *) &CYCLEprm ,&CYCLEstr[0][0]); 
            // CAL.NoStartup= option_select_count(CAL.NoStartup,0,1,"");  
             BOARD.NOSTARTUP = CAL.NoStartup;          
             break; 
             } //          
	     	     
    case 17: {  // 3.0   cycle time                
             CAL.Nomotion= option_select_count(CAL.Nomotion,0,1,"");                         
             BOARD.NOMOTION = CAL.Nomotion;                  
             break;
             }//         
   
  case 18: {  // 1.8 mirror 
             CAL.Mirror= option_select_count(CAL.Mirror,0,2,"");
             BOARD.MIRROR = 0; 
             break; // 
             }             
             
   case 19: {  // 1.8   com time out for command mode
             CAL.RefreshTime=menuselect(CAL.RefreshTime,(int *) &DISPLAYprm ,&DISPLAYstr[0][0]); 
            //VALUE = option_select_count(CAL.RefreshTime,0,4,"");
           // CAL.RefreshTime = VALUE;       
            break;
            }// CAL.ComTimeout        	     
    	        
    // ####################  Additional setup Sub block 2.x ###########################
    case 20: {  // 2.0  programm                 
             CAL.Pgm = option_select_count(CAL.Pgm,0,10,"");    
             BOARD.PGM = CAL.Pgm;                     
             break;
             }// 
	       
    case 21: {  // 2.1  multi drop
           //    CAL.Id=menuselect(  CAL.Id,(int *) &IDprm ,&IDstr[0][0]);   
             CAL.Id= option_select_count(CAL.Id,0,9,"");    
             BOARD.ID = CAL.Id;    
             break;    // 
             }//
             
     case 22: {  // 1.6   com time out
            CAL.ComTimeout=menuselect( CAL.ComTimeout,(int *) &TIMOUTprm ,&TIMOUTstr[0][0]); 
           // VALUE = option_select_count(CAL.ComTimeout,0,4,"");
           // CAL.ComTimeout = VALUE;
	   // CAL.CommandTimeout = VALUE;
            break;
            }//CAL.ComTimeout  
   
  case 23: {  // 1.6   com time out    
            CAL.CommandTimeout=menuselect( CAL.CommandTimeout,(int *) &TIMOUTCprm ,&TIMOUTCstr[0][0]); 
         //  VALUE = option_select_count(CAL.ComTimeout,0,4,"");
           // CAL.ComTimeout = VALUE;
	   // CAL.CommandTimeout = VALUE;
            break;
            }//CAL.ComTimeout                    
             
 case 24: {  // 1.6   com time out    
            CAL.MessageTimeout=menuselect( CAL.MessageTimeout,(int *) &TIMOUTCprm ,&TIMOUTCstr[0][0]); 
         //  VALUE = option_select_count(CAL.ComTimeout,0,4,"");
           // CAL.ComTimeout = VALUE;
	   // CAL.CommandTimeout = VALUE;
            break;
            }//CAL.ComTimeout    
             
   case 25: {  // 5.6  no dashes
            // CAL.NoDashes=menuselect( CAL.NoDashes,(int *) &DASHprm ,&DASHstr[0][0]); 
             CAL.NoDashes = option_select_count(CAL.NoDashes,0,1,"");  
             BOARD.NODASHES = CAL.NoDashes;
             break;
             }  //   
             
   case 26: {  // 3.8   zero suppress
         // CAL.Zsuppress=menuselect(CAL.Zsuppress,(int *) &LEADprm ,&LEADstr[0][0]); 
            CAL.Zsuppress= option_select_count(CAL.Zsuppress,0,1,""); //  
            BOARD.ZSUPPRESS= CAL.Zsuppress;   
            break;
             }//
          
      case 27: {  // 3.8   zero suppress         
            CAL.Deadzero= option_select_count(CAL.Deadzero,0,2,""); //  
            CAL.Deadzero= CAL.Deadzero;                  
             break;
             }//  
            
     case 28: {  // 8.9  decimal  
            //  CAL.Comma=menuselect( CAL.Comma,(int *) &COLONprm ,&COLONstr[0][0]); 
             CAL.Comma = option_select_count(CAL.Comma,0,1,"");    
             BOARD.COMMA = CAL.Comma;
             break;
             }  //

  case 29: { // 3.9 over capacity 
            CAL.OverCap = parameter_entry(CAL.OverCap,6,1,"", 0);
            BOARD.OVERCAP = 0;
            break;
             }//
            
                         
   // #####################  string Sub block 3.x ##################################
          /*  
         P5.0 Set 0    :   Normal 
         P5.0 Set Baud :    1- Set baud rate and learn string
                            2- Set baud rate and skip learn string if in command mode or freeze mode 
                            3- Set baud if autolearn disabled (read from flash) 
         Pressing LEARN button enforces P5.0 to 0
         Entering LEARN in config mode does not affect P5.0
         */  
    case 30: {  // 5.0  startup learn
            //   CAL.Nolearn=menuselect( CAL.Nolearn,(int *) &LEARNprm ,&LEARNstr[0][0]);
             CAL.Nolearn = option_select_count(CAL.Nolearn,0,2,"");    
             BOARD.NOLEARN = CAL.Nolearn;
             break; // 
             }// 
             
     case 31: {  // 3.1   CNT            
            
            VALUE = option_select_count(PARSE.CNT,6,BUFLEN,"");  
            
            if(VALUE)
            { //  
             CAL.ParseCnt = VALUE;
             PARSE.CNT = VALUE;
            } //    
            else
	    {
	     CAL.ParseCnt = BUFLEN;
             PARSE.CNT = BUFLEN; 
	    } // 
            break;
            }//  
	    
  case 32: {// 3.2   weight         
            
            VALUE = option_select_count(PARSE.WEIGHT,1,PARSE.CNT,"");
            
            if(VALUE)
            {  
             CAL.ParseWeight = VALUE;
             PARSE.WEIGHT = VALUE;
            }
	    else
	    {
	     CAL.ParseWeight = 1;
             PARSE.WEIGHT = 1;  
	    }
             break;
            }// 
            	    
  case 33: {  // 3.3   length           
      
            VALUE = option_select_count(PARSE.LENGTH, 6,PARSE.CNT,"");
            
            if(VALUE)
            {  
             CAL.ParseLength = VALUE;
             PARSE.LENGTH = VALUE;
            }
	    else
	    {
	    CAL.ParseLength = 6;
            PARSE.LENGTH = 6;  
	    }
            // error             
            break;
            }// 66
	    
  case 34: {// 3.4   frame          
            CAL.ParseFrame=menuselect(CAL.ParseFrame,(int *) &FRAMEprm ,&FRAMEstr[0][0]);
            PARSE.FRAME = CAL.ParseFrame;
            break;
            }// 
   
   case 35: {// 3.4   frame          
            CAL.Frame=menuselect(CAL.Frame,(int *) &FRAMEprm ,&FRAMEstr[0][0]);
            BOARD.FRAME = CAL.Frame;            
            break;
            }// 
	    
    case 36: {  // 3.5  decimal point
             CAL.Dec=menuselect(CAL.Dec,(int *) &DECprm ,&DECstr[0][0]);
             //CAL.Dec= option_select_count(CAL.Dec,0,3,"");    
             BOARD.DEC = CAL.Dec;
             break;
             }  //
                   
     case 37: { // 3.6 scoreboard NT/GR 
             CAL.Tare=menuselect(CAL.Tare,(int *) &TAREprm ,&TAREstr[0][0]); 
            // CAL.Tare= option_select_count(CAL.Tare,0,3,""); 
             BOARD.TARE = CAL.Tare; 
             break;
             }// 
               
   case 38: { // 3.7 scpreboard units
             CAL.Unit=menuselect(CAL.Unit,(int *) &UNITprm ,&UNITstr[0][0]); 
            // CAL.Unit= option_select_count(CAL.Unit,0,3,"");
             BOARD.UNIT = CAL.Unit; 
             break;
             }//
        
                  
   case 39: { // 3.9 scpreboard ocap
             CAL.Ocap = menuselect(CAL.Ocap,(int *) &OCAPprm ,&OCAPstr[0][0]);
             BOARD.OCAP = 0; 
             break;
             }//          
      
   //#####################  Additional setup Sub block 4.x #####################            
       
         /*  
         P5.0 Set 0    :   Normal 
         P5.0 Set Baud :    1- Set baud rate and learn string
                            2- Set baud rate and skip learn string if in command mode or freeze mode 
                            3- Set baud if autolearn disabled (read from flash) 
         Pressing LEARN button enforces P5.0 to 0
         Entering LEARN in config mode does not affect P5.0
         */  
           
    case 40: {  // 5.0  startup learn
            CAL.BaudSet =menuselect( CAL.BaudSet ,(int *) &BEARNprm ,&BEARNstr[0][0]);
           //  CAL.BaudSet = option_select_count(  CAL.BaudSet,0,1,"");    
             break; // 
             }// 
             
     case 41: {  // 5.4   active / passive 
	     BOARD.COMPORT=menuselect(BOARD.COMPORT,(int *) &PORTprm ,&PORTstr[0][0]);   
            // CAL.Comport = option_select_count(CAL.Comport,0,3,""); 
             CAL.Comport= BOARD.COMPORT;
	      break;
		 } //        
                          
   case 42: {  // 5.1  baud rate setting      
            CAL.BaudRate=menuselect(CAL.BaudRate,(int *) &BAUDprm ,&BAUDstr[0][0]);
            UARTInit( COM0, CAL.BaudRate );  // RS-232
            UARTInit( COM2, CAL.BaudRate );  // C-loop
            UARTInit( COM3, CAL.BaudRate);   // RS-485       
            break;
            } //                            

  case 43: {  // 5.1  baud rate setting      
            CAL.BaudRadio=menuselect(CAL.BaudRadio,(int *) &WBAUDprm ,&WBAUDstr[0][0]);
            UARTInit( COM1, CAL.BaudRadio );  // RS-232                
            break;
            } //  
            
   case 44: {  // 5.3   echoo off 
            CAL.EchoOff=menuselect(CAL.EchoOff,(int *) &PORTOprm ,&PORTOstr[0][0]);   
            //CAL.EchoOff= option_select_count(CAL.EchoOff,0,3,""); 
            BOARD.ECHOFF = CAL.EchoOff;
            break;
               }   

     case 45: {  // 5.3  active or passive
                CAL.Active=menuselect( CAL.Active,(int *) &ACTIVprm ,&ACTIVstr[0][0]); 
                // CAL.Active = option_select_count(CAL.Active,0,1,"");    
                 BOARD.ACTIVE = CAL.Active;
                 currentloop_relay(BOARD.ACTIVE);
                 break;
                 }  // 
                                  
    case 46: {  // 5.5  radio channel
         //    CAL.Radiohop= option_select_count(CAL.Radiohop,0,3,"");    
           //  BOARD.RADIOHOP = CAL.Radiohop;               
             break; // 
             }// 
     
       
    //#####################  port setup Sub block 5.x #####################              
    case 50: {  // 5.0  switch format
            CAL.Switch = option_select_count(CAL.Switch,0,2,""); 
            BOARD.SWITCH = CAL.Switch;
            switch_control();       // traffic light switch 
            relay_control(RELAY);   // relay control            
            break;
            }//             
  
      case 51: {  // 5.3  active or passive
              
                 CAL.Relay = option_select_count(CAL.Relay,0,1,""); 
                 BOARD.ARELAY = CAL.Relay;
                switch_control();       // traffic light switch 
                relay_control(RELAY);   // relay control                       
                 break;
                 }  // 
                 
     case 52: {  // 5.1  Trafic light
             CAL.Traffic= option_select_count(CAL.Traffic,0,9,"");    
             BOARD.TRAFFIC = CAL.Traffic;
             switch_control();       // traffic light switch 
             relay_control(RELAY);   // relay control        
             break;
             } //           
            
     case 53: {  // 5.2  switch enable for commands
             BOARD.SWITCHCOMMAND1[6] = CAL.SwitchCommand1[6]; 
             BOARD.SWITCHCOMMAND1[6] = option_select_count(  BOARD.SWITCHCOMMAND1[6],0,4,"");
          
           switch(  BOARD.SWITCHCOMMAND1[6])
                    { //                     
                               
                       case 1:  {// 
                                sprintf(CAL.SwitchCommand1,"CLOSED");  
                                sprintf(CAL.SwitchCommand2," OPEN ");   
                                break;    
                               }//
                      
                       case 2:  {// 
                                sprintf(CAL.SwitchCommand1,"DRIVE ");  
                                sprintf(CAL.SwitchCommand2," STOP ");   
                                break;    
                               }//
                      
                      case 3:  {// 
                                sprintf(CAL.SwitchCommand1,"START ");  
                                sprintf(CAL.SwitchCommand2," HALT ");   
                                break;    
                               }// 
                      case 4:  {// 
                                sprintf(CAL.SwitchCommand1,"ENTER ");  
                                sprintf(CAL.SwitchCommand2," QUIT ");   
                                break;    
                               }// 
                     
                    default:  {//
                                 for (char i=0;i!=6;i++) 
                                  {  
                                  CAL.SwitchCommand1[i] = 0; 
                                  CAL.SwitchCommand2[i] = 0;      
                                 }            
                               
                                break;
                                }// 
                                //  
                    }  // end of switch
             CAL.SwitchCommand1[6]= BOARD.SWITCHCOMMAND1[6]; 
             CAL.SwitchCommand2[6]= BOARD.SWITCHCOMMAND1[6]; 
             CAL.Welcome = BOARD.SWITCHCOMMAND1[6];                              
             break;
            }//    
             
  case 54: {  // 5.2  switch enable for commands
            BOARD.SWITCHCOMMAND2[6] = CAL.SwitchCommand2[6];   
            BOARD.SWITCHCOMMAND2[6]  = option_select_count(  BOARD.SWITCHCOMMAND2[6] ,0,4,"");  
            //BOARD.WELCOME = CAL.Welcome;
            
            switch( BOARD.SWITCHCOMMAND2[6] )
                    { //                     
                               
                       case 1:  {// 
                             //   sprintf(CAL.SwitchCommand1,"CLOSED");  
                                sprintf(CAL.SwitchCommand2," OPEN ");   
                                break;    
                               }//
                      
                       case 2:  {// 
                               // sprintf(CAL.SwitchCommand1,"DRIVE ");  
                                sprintf(CAL.SwitchCommand2," STOP ");   
                                break;    
                               }//
                      
                      case 3:  {// 
                             //   sprintf(CAL.SwitchCommand1,"START ");  
                                sprintf(CAL.SwitchCommand2," HALT ");   
                                break;    
                               }// i 
                      case 4:  {// 
                             //   sprintf(CAL.SwitchCommand1,"START ");  
                                sprintf(CAL.SwitchCommand2," QUIT ");   
                                break;    
                               }// i 
                     
                    default:  {//
                                 for (char i=0;i!=6;i++) 
                              {  
                              // CAL.SwitchCommand1[i] = 0; 
                                 CAL.SwitchCommand2[i] = 0;      
                             }            
                           
                                break;
                                }//
                                //  
                    }  // end of switch
            CAL.SwitchCommand2[6]= BOARD.SWITCHCOMMAND2[6];   
             break;
            }// 
            
      
   //#####################  General Setup Sub block 1.x  ###################  // 
            
      case 90: {  // 9.0  default setting 
                display_audit( _COMCONT); // show checksum and counter  
                break;               
             }   //
             
     
  case 91: {  // 9.1  default setting 
               display_audit( _NUMCONT); // show checksum and counter
                break;               
             }   // 
             
    case 92: {  // 9.2  default setting 
                display_audit( _STRINGS); // show checksum and counter
                break;               
             }   // 
             
             
   case 93: {  // 4.3  string
             display_audit( _CHEKSUM); // show checksum and counter
             break; // 
             }//    
             
             
    case 94: {  // 4.3  string
             display_audit(_RADIOSS); // show checksum and counter
             break; // 
             }//              
             
     case 95: {  // 4.3  string
             display_audit( _SENSOR); // show checksum and counter
             break; // 
             }// 
              
             
     case 96: {  // 8.7   parse manual               
                           
                display_audit( _VERSION); // show checksum and counter
                  break;
                  }// s        
                  
    case 97: {  // 8.7   parse manual               
                                    
                  display_audit( _MODEL); // show checksum and counter
                  break;
                  }// k        
             
               
    case 98: {  // 8.7   parse manual               
                  VALUE = option_select_count(CAL.StringType,0,40,"");                    
                 
                  if(VALUE)
                    {  
                  CAL.StringType = VALUE;                    
                    }
                  break;
                  }//         
           
    case 99: {  // 9.9  default setting 
               
               VALUE = 0;
               VALUE = option_select_count(VALUE,0,1,"");  
                         
               if( VALUE == 1 )
                {  
                 CAL.FirstTime= 0x55;  // flag to indicate that the FLASH needs to be initialized                
             
                 system_init(0);                
                 text_message("INIT  ",1);      
                }//   
               
                break;               
             }   //         
              
/*             
  

    case 44: { //cutout sensitivity
              CAL.FILTERSENS= optionselect((int *) &SENSprm ,CAL.FILTERSENS,"",0);
              break;
             }


    case 45: { // display update rate

              CAL.DISPLAY_RATE=menuselect(CAL.DISPLAY_RATE,(int *) &DISPLAY_RATEprm ,&DISPLAY_RATEstr[0][0]);
              
              break;
              }	

   case 30: {  // 3.0   cycle time                
             CAL.TimeMode= option_select_count(CAL.TimeMode,0,2,"");            
             if(CAL.TimeMode) CAL.Option=1;                    
             BOARD.OPTION = 0;                  
             break;
             }//  
     case 31: {  // 3.1   cycle date
             CAL.DateMode= option_select_count(CAL.DateMode,0,3,"");    
             if(CAL.DateMode) CAL.Option=1;
             BOARD.OPTION = 0;     
             break;
             }//          
      case 32: {  // 3.2   cycle temperature
            CAL.TempMode= option_select_count(CAL.TempMode,0,2,""); 
            if(CAL.TempMode==1) BOARD.FARENHEIT=1;
            if(CAL.TempMode==2) BOARD.FARENHEIT=0;
            CAL.Farenheit = BOARD.FARENHEIT;
            if(CAL.TempMode) CAL.Option=1;
            BOARD.OPTION = 0;   
            break;
                } // l 
      case 33: {  // 3.3   cycle weight
             CAL.WeightMode= option_select_count(CAL.WeightMode,0,2,"");
             if(CAL.WeightMode==0) CAL.Option= 1;
             BOARD.OPTION = 0;   
             break; //  
             }//             
       case 34: {  // 3.4   timer seting         
            VALUE = option_select_count(CAL.DisplayTime,0,60,"");             
            CAL.DisplayTime = VALUE;
           
            if(VALUE == 0)
            {             
            CAL.Option = 0;    // enables and disalbes time and date
            CAL.TimeMode = 0;  // changes the time mode 
            CAL.DateMode = 0;  // changes the dat mode to US or EU format
            CAL.WeightMode = 2;// changes the wieght mode
            CAL.TempMode = 0;  // changes the tmeperature mode to F or C
            //CAL.Farenheit = 0; // celsius or farenheit
            } // o
            BOARD.OPTION = 0;   
                              
            CAL.BatterySaver = VALUE;  
            BOARD.BATTERYSAVER = CAL.BatterySaver;            
                        
            break;
            }//CAL.ComTimeout
      
    
       case 86: {  // 8.6   auto scan
               VALUE = PARSE.AUTOSCAN;
                 
            //   if(CAL.ParseManual ==1)
             if(BOARD.MANUAL ==1)
               {
                        VALUE = 2;
               }              
                   
             VALUE= option_select_count(VALUE,0,2,"");
              
              BOARD.MANUAL = 0;
              PARSE.AUTOSCAN = VALUE;
                               
             if(VALUE == 2)
                {
                 PARSE.AUTOSCAN = 1;
                 BOARD.MANUAL = 1;
                }
               
                   CAL.AutoScan = PARSE.AUTOSCAN ;  
                   CAL.ParseManual = BOARD.MANUAL ;
             
                   break;
                   }//      
          
        case 87: {  // 8.7   parse manual               
                  VALUE = option_select_count(CAL.StringType,0,30,"");                    
                 
                  if(VALUE)
                    {  
                  CAL.StringType = VALUE;                    
                    }
                  break;
                  }// s 
                  
       case 88: {  // 8.8  checksum          
                 CAL.Chk = option_select_count(CAL.Chk,0,1,"");    
                 BOARD.CHK = CAL.Chk;
                 break;
                 }  //          
              
  
     */
     

   default: break; 

  } // end of switch
// prm_scroll(_UP);// keeps in the loop
 
 
} // end of function end of function 
 

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
int option_select_count (int current, int tmin, int tmax, char str[] )
{
int tcount=current;
char key;
char cmd = CALPRM[CAL_PARAMETER];  // get calibration value we have selected 

if (current > tmax) current=tmin;
 
while(1) // loop until enter key is pressed
{

 key = scan_keypad(_NORMAL);  // scan the keypad for keypresses

 
  if((cmd !=11) & (CAL.Config == 1)) // config lock out
  {//1 
                switch (key)
      {//3    

      case _ENTER: {// ENTER key is pressed, process calibration command that we are currrently pointing to in the menu        
                 return(current); // 
                   } // 
      } // end of switch               
                
 }//1
 else
 {//4
      switch (key)
      {//3
       case _UP:
                { // incrememnt parameter
                if (tcount < tmax)
                {
                  tcount++;
                } //
               break;
                } // 

       case _DOWN:
                 { // decrement parameter
                if (tcount > tmin)
                { // 
                  tcount--;
                } // c 
              
                 break;
                 }
      case _ENTER: {// ENTER key is pressed, process calibration command that we are currrently pointing to in the menu          
                   return(tcount);
                   }
                   
     case _ENTER_SET: {// ENTER key is pressed, process calibration command that we are currrently pointing to in the menu          
                   return(current);
                   }
      } // end of switch
 }//4
      
// build the display to display the value being edited
WDTFeed( );          // watch dog timer
displayvalue(tcount,0,0,"");
BOARD.DEC = 0;
transmit_digits(); // update the display on the board
      } //3 end of while
}// end of function
//  
//---------------------------------------------------------------------------------------------------------
// scroll through calibration parameters
//
//---------------------------------------------------------------------------------------------------------
void prm_scroll(char direction)
{
char prm;
char string[8];
char i;
int val;
int result;
int max; // 
// 
max = sizeof(CALPRM); // get the number of parameters that are in the table
max--; // end of table is 1 back
//       
if (BOARD.MODE==_CONFIG) // check if we are not yet displaying calibration parameters
{//5  
  
  // on first keypress we do not increment/decrement parameter number
  switch (direction)
      {//3
       case _UP:
                { // incrememnt parameter
                if (CAL_PARAMETER < max)
		    {//21    
		      CAL_PARAMETER++;
		    }//21
		  
                break;
                } // 

       case _DOWN:
                 { // decrement parameter
                 if (CAL_PARAMETER > 0)
		     {//2        
		       CAL_PARAMETER--;
		     }//2
		   
                 break;
                 }  // 
   case _UP_H:
                { // incrememnt parameter
                    if (timer(SCROLL_U,TIMEOUT))
		     {//33     
		     for (i=CAL_PARAMETER+1;i!=max;i++)
		     {//22
			 if (i>max)
			 {
			  CAL_PARAMETER=max;
			   break;
			 }// 
		       val= CALPRM[i];
		       result=val/10;
		       val=(val-result*10);
			if (val==0)
			{CAL_PARAMETER=i;
			  break;
			} //  
		      }   // 22
		     timer(SCROLL_U,START);
		     }//33
				break;
                } // 

       case _DOWN_H:
                 { // decrement parameter
                 if (timer(SCROLL_D,TIMEOUT))
		   {//44     
		  if (CAL_PARAMETER >0)
		  {  //  
		  for (i=CAL_PARAMETER-1;i!=0;i--)
		   {//55 
		      if (i==1) // i<1
		       {
			CAL_PARAMETER=0;
			break;
		       }
		     val= CALPRM[i];
		     // 
		     result=val/10;

		     val=(val-result*10);

		      if (val==0)
		      {
			CAL_PARAMETER=i;
			break; // a
		      }
		    }//55
		  }
		   timer(SCROLL_D,START);
		  
		   }//44
                 break;
                 }
		 
		 
  
      } // end of switch
}// if config


 for (i=0;i!=6;i++)
           { // 
            BOARD.DIGIT[i]=' '; // blank display
	     string[i]=' '; // fill with spaces
           } // 
 // build the string to update the display
    prm=CAL_PARAMETER;        
    val= CALPRM[prm]; // get current cal parameter
    // convert 2 digit integer to formatted string
    result=val/10;
    val = val-(result*10); // subtract result to get remainder
       
    string[0]= 0;       // insert P character
    string[1]= 0;       // insert P character
    string[2]='F';       // insert P character
    string[3]= result + 48;    // convert result to ascii
    string[4]= val + 48;    // convert result to ascii
    string[5]= 0;    // convert result to ascii 
    //   
    WDTFeed( );          // watch dog timer  
    for (i=0;i!=6;i++)  
    { // 
     BOARD.DIGIT[i]=string[i]; // copy string over to display
    } //   
  
  BOARD.DEC=2;
  transmit_digits();      // update the display on the board 
  } // end of function
 
//------------------------------------------------------------------------
//  option select from a table passed to it
//  tables holds all the values that can be selected
//
//------------------------------------------------------------------------
int optionselect ( const int *table ,int current , char str[] , char dec)
{

int tcount;       //  number of table entries
char tpos;        //  position in the table
char i;
int key;
char cmd = CALPRM[CAL_PARAMETER];  // get calibration value we have selected 

tcount=table[0]; // get the number of options there are to display
tpos=1;

for (i=1;i!=tcount+1;i++)  // search through for current value in table, initialize table position
{
 if (current == table[i])
 {
   tpos=i; // current table position is now set
   break;  // break out of loop
 }
} //    


while(1) // loop until enter key is pressed
{

 key = scan_keypad(_NORMAL);  // scan the keypad for keypresses

 if((cmd !=11) & (CAL.Config == 1)) // config lock out
  {//1 
                switch (key)
      {//3    

      case _ENTER: {// ENTER key is pressed, process calibration command that we are currrently pointing to in the menu        
                   return table[tpos];
                   } // 
      } // end of switch               
                
 }//1
 else
 {//2
      switch (key)
      {//3

       case _UP:
                { // incrememnt table pointer                 
                 if (tpos < tcount)
                 {
                  tpos++;
                 }
                 
                break;
                }


       case _DOWN:
                 { // decrement table pointer                
                 if (tpos > 1)
                 {
                  tpos--;
                 }

                 break;
                 }





      case _ENTER: {  // ENTER key is pressed, process calibration command that we are currrently pointing to in the menu
                   return(table[tpos]);
                   }

  
      } // end of switch

 }//2
 WDTFeed( );          // watch dog timer  
// build the display to display the value being edited
 displayvalue( table[tpos],dec,0,str);
 transmit_digits();      // update the display on the board 

      } //3 end of while





}// end of function





//------------------------------------------------------------------------
//  menu select from a table passed to it
//  tables holds all the values to be selected, and the string to be displayed
//
//------------------------------------------------------------------------
int menuselect (int current , const int *table ,  const char *string )
{

int tcount;       //  number of table entries
char tpos;        //  position in the table
char i,j,k;
int key;
char dec;
char cmd = CALPRM[CAL_PARAMETER];  // get calibration value we have selected 

const char *str; // pointer to array pasted


tcount=table[0]; // get the number of options there are to display
tpos=1;


str=string; // str holds pointer


for (i=1;i!=tcount+1;i++)  // search through for current value in table, initialize table position
{
 if (current == table[i])
 {
   tpos=i; // current table position is now set
   break;  // break out of loop
 }
}






 while(1) // loop until enter key is pressed
{

 key = scan_keypad(_EXTENDED);  // scan the keypad for keypresses

  if((cmd !=11) & (CAL.Config == 1)) // config lock out
  {//1 
                switch (key)
      {//3    

      case _ENTER: {// ENTER key is pressed, process calibration command that we are currrently pointing to in the menu        
                   return table[tpos];
                   } // 
      } // end of switch               
                
 }//1
 else
 {//2

      switch (key)
      {//3

       case _UP:
                { // incrememnt table pointer


                
                 if (tpos<tcount)
                 {
                    tpos++;
                 }


                break;
                }


       case _DOWN:
                 { // decrement table pointer

                 
                 if (tpos>1)
                 {
                  tpos--;
                 }


                 break;
                 }





      case _ENTER: {  // ENTER key is pressed, process calibration command that we are currrently pointing to in the menu

                 return(table[tpos]);
                   }
    
    
      } // end of switch

 }//2

// build the display to display the value being edited
str = string + (tpos-1) *6 ; // calculate pointer to string in array

j=0;
dec=0;


 for (i=0;i!=6;i++)
 {//1
        if (  *(str  + i)  =='.')  // search for decimal (if any)
         {
          dec=i; // if there is a decimal, mark decimal position
          }
         else
         {
          BOARD.DIGIT[j]= *(str  + i);  // copy string over to display
          j++;
         }

 }//1

   if (dec!=0) // if there is a dcimal
   {//2
    for (k=6;k!=0;k--)  // right shift entire string to remove space in string from decimal
            {
             BOARD.DIGIT[k]=BOARD.DIGIT[k-1];
            }
  BOARD.DEC = 5 - dec;
   }//2
else
{
  BOARD.DEC = 0; 
}

  WDTFeed( );          // watch dog timer  
  
  transmit_digits();      // update the display on the board 

  } //3 end of while

}// end of function




