#include "includes.h"

extern long lTIMERTICS;

long VALUE;

const char DONEmsg[]=" DONE "; // message done
const char DEADmsg[]=" DEAD "; // message deadload
const char SPANmsg[]=" SPAN "; // message deadload

//values for grad size parametr P1.0
const int GRADprm[]={8,1,2,5,10,20,50,100,200}; // grad size menu
const  char GRADstr[][6] ={"    1D","    2D","    5D","   10D","   20D","   50D","  100D","  200D"};  // displayed settings

//values for decimals menu parameter P1.1
const int   DECprm[]={5,0,1,2,3,4};             // decimal position menu
const  char DECstr[][6] ={"  NONE","   0.0","  0.00"," 0.000","0.0000"};  // displayed settings

// values for zero tracking menu parameter P2.1
const  int  ZEROTRKprm []={5,0,50,100,200,300}; // 0,0.5d,1.0d,2.0d,3.0d
const  char ZEROTRKstr[][6] ={"   OFF","  0.5D","    1D","    2D","    3D"};  // displayed settings

// values for zero range menu parameter P2.0
const  int  ZERORANGEprm []={5,2,3,5,10,90}; // 2% ,10% ,90%
const  char ZERORANGEstr[][6] ={"   2PC","   3PC","   5PC","  10PC","  90PC"};  // displayed settings

// values for scale over P1.5
const  int  SCLOVERprm[]={5,0,1,2,9,10}; // 0,off,1d,2d,2%
const  char SCLOVERstr[][6] ={"    0D","    1D","    2D","    9D","   2PC"};  // displayed settings

// values for scale motion parameter P2.3
const  int  MOTprm []={6,0,1,2,3,5,10}; //0,off,1d,2d,3d,10d
const  char MOTstr[][6] ={"   OFF","    1D","    2D","    3D","    5D","   10D"};  // displayed settings

// values for scale motion parameter P2.3
const  int  MOTIprm []={6,1,2,4,8,12,20}; //0,off,1d,2d,3d,10d
const  char MOTIstr[][6] ={"  0.25","  0.50","   1.0", "   2.0","   3.0","   5.0"};  // displayed settings

// values for scale motion parameter P2.3
const  int  PEAKTprm []={6,0,1,2,4,8,20}; //0,off,1d,2d,3d,10d
const  char PEAKTstr[][6] ={"     0","  0.25","  0.50","   1.0", "   2.0","   5.0"};  // displayed settings

// values for baudrate P5.0
const  int BAUDprm[]={6,1200,2400,4800,9600,19200, 38400}; // baud rate values for UART DIVISOR registor
const  char BAUDstr[][6] = {"  1200","  2400","  4800","  9600"," 19200"," 38400"};  // displayed settings

// values for scale COM1 P5.1
const  int  BITSprm []={3,0,1,2};
const  char BITSstr[][6] ={"8-NONE","7-EVEN","7-ODD "};  // displayed settings

// values for checkweigh transaction threshold
const  int  CTRESHprm []={6,10,20,30,40,50,60};
const  char CTRESHstr[][6] ={"  10PC","  20PC","  30PC","  40PC","  50PC","  60PC"};  // displayed settings

// values for filtering P4.0,P4.1,P4.2,P5.3
const int AVERAGEprm[]={6,10,50,75,100,150,200};
const int THRESHprm[]={6,2,4,8,12,14,18};
const int SENSprm[]={6,2,5,8,10,12,15};

const  int  CORNERprm []={4,1,2,3,4}; //0.5,1,2,3
const  char CORNERstr[][6] ={"   0.5","   1.0","   2.0","   3.0"};  // displayed settings

// values for parameter 1.4
const  int  DISPLAY_RATEprm []={5,0,1,2,3,4}; //0.5,1,2,3
const  char DISPLAY_RATEstr[][6] ={"     0","  0.25","  0.50","  0.75","   1.0"};  // displayed settings

// values for parameter 1.4
const  int  COUNTprm []={7,2,5,10,20,50,100,200}; //0.5,1,2,3
const  char COUNTstr[][6] ={"PUT  2","PUT  5","PUT 10","PUT 20","PUT 50","PUT100","PUT200"};  // displayed settings

// values for scale over P1.5
const  int  TAREprm[]={4,0,1,2,3}; // 0,off,1d,2d,2%
const  char TAREstr[][6] ={"  NONE","   CAN","  NTEP","  OinL"};  // displayed settings

// values for scale motion parameter P2.3
const  int  GFORCEprm []={2,0,1}; //0,off,1d,2d,3d,10d
const  char GFORCEstr[][6] ={"   OFF","   SET"};  // displayed settings

// values for scale motion parameter P2.3
const  int  LEGALprm []={2,0,1}; //0,off,1d,2d,3d,10d
const  char LEGALstr[][6] ={" AUDiT","   LFT"};  // displayed settings

// values for scale motion parameter P2.3
const  int  CALIBprm []={2,0,1}; //0,off,1d,2d,3d,10d
const  char CALIBstr[][6] ={" S-CAL"," L-CAL"};  // displayed settings

// values for scale over P1.5
const  int  SPANprm[]={4,0,1,2,3}; // 0,off,1d,2d,2%
const  char SPANstr[][6] ={" C-25 ", " C-50 "," C-75 "," C-99 "};  // displayed settings

// values for scale over P1.5
const  int  TONprm[]={3,0,1,2}; // 0,off,1d,2d,2%
const  char TONstr[][6] ={"  ton","  ston","  lton"};  // displayed settings

// parameter numbers used for scrolling though calibration
// parameter numbers used for scrolling though calibration
// add calibration parameters to list below
const char CALPRM[]={ 0, // exit and save calibration

                     10, // grad size   1,2,5,10,20,50,100
                     11, // decimals    1,2,3
                     12, // deadload scale
                     13, // span scale      - weight
                     14, // scale capacity  - weight
                     15, // scale over   - graduations
                     16, // primary units
                     17, // power up units
                     18, // alt units 1
                     19, // alt units 2

                     20, // push button zero range % of capacity  - 1% to 99%
                     21, // zero tracking  - 0.6d  1d, 2d, 3d
                     22, // power up auto zero enable  - 0  or 1
                     23, // motion window  1d - 5d
                     24, // motion timer  - 0.25sec increments
                     25, // blank display on motion  - 0 or 1
                     26,
                     27,
                     28,
                     29,

                     30, // Regulatory
                     31, // lockoout tare
                     32, // auto tare
                     33, // auto tare clear
                     34,
                     35,
                     36,
                     37,
                     38,
                     39,
                     
                     40, // filter preset
                     41, // filter frequency
                     42, // average reg
                     43, // sens 1
                     44, // sens 2
                     45, // display update
                     46,
                     
                     50, // Baud Rate COM1
                     51, // Data Bits/Parity COM1
                     52, // COM1 mode
                     53, // COM1 stream emulation 
                     54, // stream delay COM1/COM2
                     55, // Auto Print COM1
                     56, // Baud Rate COM2 
                     57, // Data Bits/Parity COM2
                     58, // COM2 stream emulation
                     59, // COM2 Alternate function
                                          
                     60, // peak detect enable
                     61, // peak detect threshold
                     62, // peak autoclear
                     63, // peak timer
                     64, // peak detect enable
                     65, // peak detect threshold
                     66, // peak autoclear
                     67, // peak timer
                     68, // peak detect threshold
                     69, // peak autoclear
                     
                     70,
                     71,
                     72,
                     73,
                     
                     90, // internal AD converter counts
                     91, // display or edit span
                      };



void calibration_command(int cmd)
{

long pass;

cmd = CALPRM[SYSTEM.CAL_PARAMETER];  // get calibration value we have selected

  switch(cmd)
  {
  case 0:  {  // 0.0  exit and save calibration

             exit_cal();
// 
             return;

             }

   //#####################  Scale Calibration Sub block 1.x ##################################
   // ###########################  Scale Diagnostics sub block  9.x #####################################################
case 10: {  // 1.0  grad size
          CAL.LINEAR = menuselect(CAL.LINEAR,(int *) &CALIBprm,&CALIBstr[0][0]); 
          break;
          }

 
  case 11: {  // 1.0  grad size             
             CAL.GRADd=optionselect((int *) &GRADprm ,CAL.GRADd,"D",0);
                // from the GRADd we determine grad size and number of dead zeros

                if (CAL.GRADd > 50)  // 100
                {
                 CAL.DEADZERO=2;
                 CAL.GRAD=CAL.GRADd/100;
                }
                else
                if (CAL.GRADd > 5)  // 10,20,50
                {
                  CAL.DEADZERO=1;
                  CAL.GRAD=CAL.GRADd/10;
                }
                else
                {
                  CAL.DEADZERO=0;    //1,2,5
                  CAL.GRAD=CAL.GRADd;
                }

             SCALE.DIS_GRAD=CAL.GRAD;

             break;
             }

    case 12: {  // 1.1  decimals

             CAL.DEC=menuselect(CAL.DEC,(int *) &DECprm ,&DECstr[0][0]);
             
              if (CAL.DEADZERO !=0)  // 10,20,50
                {
                   CAL.DEC=0;
                } 
             
             break;
             }

    
  case 13: {  // 1.6  scale calibration units

             CAL.UNITS = option_select_count (CAL.UNITS, _KG, _LB ,"M");
             SCALE.DIS_UNITS=CAL.UNITS;

             break;
             }


      case 14: {  // 1.7  scale default units units (power up)

                CAL.D_UNITS[0] = option_select_count (CAL.D_UNITS[0], 1, 4 ,"M");

                 break;
             }


      case 15: {  // 1.8  scale alternate units 1

                 CAL.D_UNITS[1] = option_select_count (CAL.D_UNITS[1], 0, 4 ,"M");
                 break;
             }


      case 16: {  // 1.9  scale alternate units 2
               CAL.D_UNITS[2] = option_select_count (CAL.D_UNITS[2], 0, 4 ,"M"); 
              // CAL.D_UNITS[2] = option_select_count (CAL.D_UNITS[2], 0, 7 ,"M"); // 5 as TON with g anninciator
               break;
               }
    case 17: {  // 1.4  scale capacity

             display_message(" CAP  ",2);
             display_message(" SET  ",2);
             VALUE = parameter_entry(CAL.SCALECAP,6,1,"      ",0);

              if (VALUE < 1 ) // check for valid entry
               {
                break; // abort
               } // 
             
                CAL.SCALECAP=VALUE;  // update new scale capacity
                SCALE.ZERORANGE=calc_zerorange(); // calculate the zero range of the indcator
                calc_scaleover(); // calculate scale over value           
                
             break;
             }
     case 18: {  // 1.5  scale over divisions

             CAL.SCLOVER=menuselect(CAL.SCLOVER,(int *) &SCLOVERprm ,&SCLOVERstr[0][0]);
             calc_scaleover(); // calculate scale over value
             break;
             }


      case 19: {  // 1.5  scale over divisions
             CAL.LEGAL = menuselect(CAL.LEGAL,(int *) &LEGALprm,&LEGALstr[0][0]); 
           // CAL.LEGAL= option_select_count(CAL.LEGAL,0,1,"");
          //    CAL.LEGAL = menuselect(CAL.LEGAL,(int *) &LEGALprm,&LEGALstr[0][0]); 
             // if there is mirroinig inerhe e
             break;
             }

   //#####################  Zero and Motion Sub block 2.x ##################################

      case 20: {  // 2.0  push button zero range of capacity
             CAL.ZERORANGE=menuselect(CAL.ZERORANGE,(int *) &ZERORANGEprm ,&ZERORANGEstr[0][0]);
             SCALE.ZERORANGE=calc_zerorange(); // calculate the zero range of the indcator
             break;
             }


      case 21: {  // 2.1  zero tracking %
              CAL.ZEROTRACK=menuselect(CAL.ZEROTRACK,(int *) &ZEROTRKprm ,&ZEROTRKstr[0][0]);
              set_trkval(); // calculate the zero tracking range
              break;
              }


     case 22: {  // 2.1  power up  auto zero enable

              CAL.PWRZERO= option_select_count(CAL.PWRZERO,0,1,"");
              // 
              break;
              }


    case 23: {  // 2.2  motion window
                
              CAL.MOTION=menuselect(CAL.MOTION,(int *) &MOTprm ,&MOTstr[0][0]);
              break;
              }

    case 24: {  // 2.3  motion timer

               CAL.MOTION_TIME=menuselect( CAL.MOTION_TIME,(int *) &MOTIprm ,&MOTIstr[0][0]);         
         //     CAL.MOTION_TIME= option_select_count(CAL.MOTION_TIME,1,20,"");
              break;
              }

    case 25:  {  // 2.5  blank display on motion

               CAL.MOTION_BLANK= option_select_count(CAL.MOTION_BLANK,0,1,"");
               break;
              }

  case 26: { // regulatory
              //    CAL.REGULA= option_select_count(CAL.REGULA,0,3,"");
             CAL.REGULA=menuselect(CAL.REGULA,(int *) &TAREprm ,&TAREstr[0][0]);
                  break;
             }   

    case 27: { //lockout tare
                  CAL.TARELOC= option_select_count(CAL.TARELOC,0,1,"");
              break;
             }

    case 28: { // Auto Tare
                  CAL.AUTOTARE= option_select_count(CAL.AUTOTARE,0,1,"");
              break;
             }

    case 29: { // Auto Clear
                  CAL.AUTOCLEAR= option_select_count(CAL.AUTOCLEAR,0,1,"");
              break;
             }

   //
   //
   //#####################  TARE Settings Sub block 3.x ##################################
case 30: {  // 1.2  deadload indicator
           
             CAL.GFORCE = menuselect(CAL.GFORCE,(int *) &GFORCEprm,&GFORCEstr[0][0]); 
            
              if(CAL.GFORCE != 1)
             {
            //  CAL.GSPAN = 1.0000; 
              break; 
             }        
            
             
             
             // 
             
             display_message(" GRAv ",2);
             display_message(" EDIT ",2);
             CAL.GFORCES = gspanedit(CAL.GFORCES); 
             CAL.GFORCED = CAL.GFORCES;
             CAL.GFSPAN = CAL.GFORCES / CAL.GFORCED;
             break;

             }

 case 31: {  // 1.2  deadload indicator
             display_message( (char *) &DEADmsg,2);
             deadload_scale();
             prm_scroll(_UP);
             SYSTEM.DISPLAY_MODE=_WEIGHT; // switch back to weight on display 
             return;
             }


 case 32: {  // 1.3  span indicator
             display_message( (char *) &SPANmsg,2);
             
             if(CAL.LINEAR == 1)
             {
             span_scale_linear(); 
             prm_scroll(_UP);
             SYSTEM.DISPLAY_MODE=_WEIGHT; // switch back to weight on display 
             return;
             }         
                         
             span_scale();                         
             prm_scroll(_UP);
             SYSTEM.DISPLAY_MODE=_WEIGHT; // switch back to weight on display 
             return;
             }
 
case 33: {  // 1.2  deadload indicator                      
            
             if(CAL.GFORCE != 1)
             {
            //  CAL.GSPAN = 1.0000; 
              break; 
             }         
             
             // 
             display_message(" GRAv ",2);
             display_message(" EDIT ",2);
             CAL.GFORCED = gspanedit(CAL.GFORCED); 
             CAL.GFSPAN = CAL.GFORCES / CAL.GFORCED;
            
             prm_scroll(_UP);
             SYSTEM.DISPLAY_MODE=_WEIGHT; // switch back to weight on display 
             return;
             
             //break;
             } 
             
 

case 34:  {  // Display AD raw counts
             display_message(" ATOD ",2);
             display_message("COUNT ",2);
             prm_scroll(_UP);
             SYSTEM.DISPLAY_MODE=_RAWCOUNTS;            
             return;
             }

             
case 35:  {  // Span Edit
              display_message(" SPAN ",2);
              display_message(" EDIT ",2);
              CAL.SPAN =  spanedit(CAL.SPAN);
              break;

             }             
case 36:  {  // Password change edit
             display_message(" PASS ",2);
             display_message(" EDIT ",2);
             VALUE = parameter_entry(CAL.PASSWORD,4,2,"-0000-",1);  
               
              if (VALUE < 1 ) // check for valid entry
               {
                break; // abort
               }               
               CAL.PASSWORD = VALUE;
              break;
              
                }

 
  
 case 37:  {  // Factory Reset
                display_message( "SETUP ",2);
                display_message( "RESET ",2);

                pass = parameter_entry(0,4,2,"-0000-",1);

                if (pass==CAL.PASSWORD)
                {
                   factory_init();   // initilize factory values
                   display_message( (char *) &DONEmsg,2);
                }
                else
                {
                    display_message( " FAIL ",2);
                }
              
                break;
              } 
  
  
              
  case 38: {  
               break;
               }
        
               
               
      case 39: {  // recieve calibration
               
               break;
               }
    

   //############# FILTER settings subset block 4.x #####################################
   case 40: { //Filter preset
             CAL.FLTPRESET= option_select_count(CAL.FLTPRESET,1,5,"");
             filter_preset(CAL.FLTPRESET);
             initialize_FIR(CAL.FIR);
              break;
             }


    case 41: { //FIR filter select
              CAL.FIR=menuselect(CAL.FIR,(int *) &CORNERprm ,&CORNERstr[0][0]);
              initialize_FIR(CAL.FIR);
              break;
             }

    case 42: { //Averaging filter length
              CAL.FILTER= optionselect((int *) &AVERAGEprm ,CAL.FILTER,"",0);

              break;
             }

    case 43: { //cutout threshold
              CAL.FILTERTHRESH= optionselect((int *) &THRESHprm ,CAL.FILTERTHRESH,"D",0);
              break;
             }

    case 44: { //cutout sensitivity
              CAL.FILTERSENS= optionselect((int *) &SENSprm ,CAL.FILTERSENS,"",0);
              break;
             }


    case 45: { // display update rate
              //  
              //CAL.DISPLAY_RATE= option_select_count(CAL.DISPLAY_RATE,0,4,"");
               CAL.DISPLAY_RATE=menuselect(CAL.DISPLAY_RATE,(int *) &DISPLAY_RATEprm ,&DISPLAY_RATEstr[0][0]);
              
              break;
              }
  case 46: { // display update rate
               CAL.STARTUP = option_select_count(CAL.STARTUP ,0,1,"");
             // 
              break;
              }
   

   //#####################  Serial Comunications Sub block 5.x ##################################
   case 50:  {  // buad rate com1
        //        CAL.BAUD1=optionselect((int *) &BAUDprm ,CAL.BAUD1,"",0);
                 CAL.BAUD1=menuselect(CAL.BAUD1,(int *) &BAUDprm ,&BAUDstr[0][0]);
                 init_com0();       // set up and initialize com 0
             break;
             }


   case 51:  {  // data bits com1
                CAL.DATABITS1=menuselect(CAL.DATABITS1,(int *) &BITSprm ,&BITSstr[0][0]);
                init_com0();       // set up and initialize com 0
             // 
             break;
             }


   case 52:  {  // com1 mode
              CAL.COM1_MODE = option_select_count (CAL.COM1_MODE, 0, 9 ,"");
             init_com0();       // set up and initialize com 0
             break;
             }

 /*
   case 54:  {  // string emulation                 
             CAL.STREAM1 = option_select_count (CAL.STREAM1, 0, 5 ,"");
             break;
             }



  */

   case 53:  { // Baud Rate COM2
                   CAL.BAUD2=menuselect(CAL.BAUD2,(int *) &BAUDprm ,&BAUDstr[0][0]);
                 // CAL.BAUD2=optionselect((int *) &BAUDprm ,CAL.BAUD2,"",0);
                  init_com1();       // set up and initialize com 1 (indicator com 2)
             //    
             break;
             }


   case 54:  {  // Data bits COM2
                  CAL.DATABITS2=menuselect(CAL.DATABITS2,(int *) &BITSprm ,&BITSstr[0][0]);
                  init_com1();        // set up and initialize com 1 (indicator com 2)
             //  
             break;
             }

 

    case 55:  {  // COM 2 mode
               CAL.COM2_MODE = option_select_count ( CAL.COM2_MODE, 0, 9 ,"");
              init_com1();        // set up and initialize com 1 (indicator com 2)
             break;
             }

/*
    case 58:  { // String emulation COM2
                  CAL.STREAM2 = option_select_count (CAL.STREAM2, 0, 5 ,"");

             break;
             }         
             
*/
  case 56:  { // Baud Rate COM2
              CAL.BAUD3=menuselect(CAL.BAUD3,(int *) &BAUDprm ,&BAUDstr[0][0]);
              init_com2();       // set up and initialize com 1 (indicator com 2)
             //  
             break;
             }


   case 57:  {  // Data bits COM2
                  CAL.DATABITS3=menuselect(CAL.DATABITS3,(int *) &BITSprm ,&BITSstr[0][0]);
                  init_com2();        // set up and initialize com 1 (indicator com 2)

             break;
             }


  case 58:  {  //Auto Print
             
              CAL.COM3_MODE = option_select_count ( CAL.COM3_MODE, 0, 5 ,"");
              init_com2();        // set up and initialize com 1 (indicator com 2)
              break;
             }         
             
  case 59:  {  // stream delay
              //    CAL.STREAMDELAY = option_select_count (CAL.STREAMDELAY, 0, 4 ,"");
                CAL.STREAMDELAY=menuselect(CAL.STREAMDELAY,(int *) &DISPLAY_RATEprm ,&DISPLAY_RATEstr[0][0]);            
             //             
             break;
             }           
              // ########################### peak detect subplock 8.x ####################################################
  
             
  case 60: {     // enable peakdetect
          
          CAL.PEAK_EN=  option_select_count (CAL.PEAK_EN, 0, 2 ,""); 
          
          if( CAL.PEAK_EN < 1)
          {
      //    display_message( "PHOLD ",2);    
      //    display_message( "  OFF ",2);    
          CAL.PEAKTHRESH =0;
          CAL.PEAKRESET = 0;
          CAL.PEAKDELAY = 0;
          }          
           // l         
           break;
           } //     
           
  case 61: {     // set trigger threshhold
                        
           CAL.PEAKTHRESH = parameter_entry(CAL.PEAKTHRESH,6,1,"      ",0); 
          
          //   CAL.PEAKTHRESH  =  CAL.PEAKTHRESH *CAL.GRAD;
            break;
           }
           
           
  case 62: {     //enable auto reset 
             CAL.PEAKRESET= option_select_count (CAL.PEAKRESET, 0, 1 ,"");
           break;
           }
           
  case 63: {     //peak delay timer
            
            CAL.PEAKDELAY=menuselect( CAL.PEAKDELAY,(int *) &PEAKTprm ,&PEAKTstr[0][0]);      
        //   CAL.PEAKDELAY=option_select_count(CAL.PEAKDELAY,0,20,"");
           break;
           }            
 
  
   case 64: {     // enable peakdetect
          CAL.PIECE_EN =  option_select_count (CAL.PIECE_EN, 0, 1 ,"");  
          
          if( CAL.PIECE_EN < 1)
          { 
      //    display_message( "COUNT ",2);    
      //    display_message( "  OFF ",2);    
          CAL.PIECE_CNT = 5;
          }            
            break;
           }
             
   case 65: {     // enable peakdetect
         
             if( CAL.PIECE_EN < 1)
          {
          display_message( "COUNT ",2);    
          display_message( "  OFF ",2);  
          break;            
          // 
          }
          CAL.PIECE_CNT = menuselect(CAL.PIECE_CNT,(int *) &COUNTprm ,&COUNTstr[0][0]);
         
          if( CAL.PIECE_CNT > 1 )
           {
           piece_scale();
           display_message( (char *) &DONEmsg,2);
           }                 
          break;
           }
    
             
  case 66: {     // enable peakdetect
           CAL.RELAY_EN=  option_select_count ( CAL.RELAY_EN, 0, 1 ,"");  
           
           if(CAL.RELAY_EN < 1)
           {
       //     display_message( " SETP ",2);    
        //    display_message( "  OFF ",2);  
            CAL.RELAY_SET1 = 0;  
            CAL.RELAY_SET2 = 0; 
            FIO1SET = 0x00030000; // disable output
           }
           relays_function(CAL.RELAY_SET1,CAL.RELAY_SET2);
           
           break;
           }
             
  case 67: {     // enable peakdetect
        
           CAL.RELAY_SET1  = parameter_entry( CAL.RELAY_SET1 ,6,1,"      ",0);         
           // 
            relays_function(CAL.RELAY_SET1,CAL.RELAY_SET2);
           break;
           }         
  
 case 68: {     // enable peakdetect   
          
          CAL.RELAY_SET2   = parameter_entry( CAL.RELAY_SET2 ,6,1,"      ",0);
             
          relays_function(CAL.RELAY_SET1,CAL.RELAY_SET2);
           break;
           } 
      
             
    case 69: {     // enable peakdetect
           CAL.BUZZ_EN =  option_select_count ( CAL.BUZZ_EN, 0, 2 ,"");  
           break;
           }        
             // 
             // ##########################   Checkweigher sub block 6.x   ####################

/*
   case 60:  {  // enable checkweigher mode of operation
              CAL.CHECK_MODE = option_select_count (CAL.CHECK_MODE, 0, 1 ,"");   // turn on/off the checkweigher

             break;
             }


   case 61:  {  // checkweigher bargraph mode
              CAL.BAR_MODE = option_select_count (CAL.BAR_MODE, 0, 1 ,"");   // bar mode

             break;
             }


   case 62:  {  // enable checkweigher ID memory
              CAL.CHECK_ID=option_select_count (CAL.CHECK_ID, 0, 1 ,"");   // turn on/off the checkweigher ID key

             break;
             }

   case 63:  {
     
                display_message( "   ID ",2);
                display_message( "RESET ",2);

                pass = parameter_entry(0,4,2,"-0000-",1);

                if (pass==CAL.PASSWORD)
                {
                   // enable checkweigher ID memory
                   init_checkweigh();  // init memory
                   display_message( (char *) &DONEmsg,2);
                }
                else
                {
                    display_message( " FAIL ",2);
                }

     //      

             break;
             }
           
   case 64:  {  // blank light bar on motion
                CAL.BAR_BLANK=  option_select_count (CAL.BAR_BLANK, 0, 1 ,"");     
   
             break;
             }
             
            
   case 65:  {  // transaction range
                    
                CAL.CHECK_THRESH=menuselect(CAL.CHECK_THRESH,(int *) &CTRESHprm ,&CTRESHstr[0][0]);
             break;
             }                       
     
             
    case 66:  {  // auto print
                   
                  CAL.AUTO_PRINT=  option_select_count (CAL.AUTO_PRINT, 0, 20 ,"");  
                
             break;
             }                       
                 
           
    case 67:  {  // enable statistics printing
                   
                  CAL.STATS=  option_select_count (CAL.STATS, 0, 1 ,"");  
                
             break;
             }              
             
     */         

 // ########################## Ticket editor subblock ##################################################################
 
   case 70:  {  //Auto Print
              CAL.ADDRESS = option_select_count (CAL.ADDRESS, 0, 9 ,"  id");
              break;
             }             
         
  case 71:  { // String emulation COM2
                  CAL.STREAM1 = option_select_count (CAL.STREAM1, 0, 10 ,"");
               
             break;
             } 
             
   case 72:  { // String emulation COM2
                  CAL.STREAM2 = option_select_count (CAL.STREAM2, 0, 10 ,"");
                 
             break;
             }            
             
  case 73:  { // String emulation COM2
                  CAL.STREAM3 = option_select_count (CAL.STREAM3, 0, 10 ,"");
                 
             break;
             }            
             
             
             
           
  /*           
  case 70:  {  // edit ticket
            
               SCALE.EX_PRINT_EDIT = option_select_count (SCALE.EX_PRINT_EDIT, 0, 1 ,"");   
    
   
    SCALE.TICKET=  option_select_count (SCALE.TICKET, 1, 5 ,"EDIT");
               if (SCALE.TICKET==0)
               {
                 break;  // operation cancelled by user 
               }
               else
               {  
                 ticket_editor(SCALE.TICKET);
               }  
  
            break;
                      
            }
              
            
  case 71:   {  // delete ticket
              SCALE.TICKET=  option_select_count (SCALE.TICKET, 0, 5 ,"DEL");
                 if (SCALE.TICKET < 1 )
               {
                 break;  // operation cancelled by user 
               }
               else
               {  
                 delete_ticket(SCALE.TICKET);
                  display_message( (char *) &DONEmsg,2);
                  build_menu();          // build the menu for the menu key
               }
               
              break;
              }
             
             
  case 72:   {  // init ticket
    
                display_message( "  TIC ",2);
                display_message( "RESET ",2);

                pass = parameter_entry(0,4,2,"-0000-",1);

                if (pass==CAL.PASSWORD)
                {
                   init_ticketbuffer();
                   display_message( (char *) &DONEmsg,2);
                   init_menu(); // initialize menu
                   build_menu();          // build the menu for the menu key
                }
                else
                {
                    display_message( " FAIL ",2);
                }   
    
              break;
              }           
              
                   
 case 73:  {  // print lockout threshold
              
             CAL.PRINTLOCKOUT    = parameter_entry( CAL.PRINTLOCKOUT ,6,1,"      ",0);
  
            break;
                      
            }      
              
    // 
              
              
  */            
              
    
 
           // ########################### peak detect subplock 8.x ####################################################

 
              
   case 90:  {  // enable clock

              CAL.CLOCKMODE = option_select_count (CAL.CLOCKMODE, 0, 4 ,"");
              
                              switch (CAL.CLOCKMODE)
                              {
                        
                              case 1: { 
                                        CAL.CLOCK_US_INT=0; //  0=international  1=US format
                                        CAL.CLOCK_12_24=1;   // 0=24hour 1=12hour
                                      break;
                                      }
                                      
                              case 2: {
                                        CAL.CLOCK_US_INT=1; //  0=international  1=US format
                                        CAL.CLOCK_12_24=1;   // 0=24hour 1=12hour
                                      break;
                                      }
                                      
                              case 3: {
                                        CAL.CLOCK_US_INT=0; //  0=international  1=US format
                                        CAL.CLOCK_12_24=0;   // 0=24hour 1=12hour
                                      break;
                                      }
                                      
                              case 4: {
                                        CAL.CLOCK_US_INT=1; //  0=international  1=US format
                                        CAL.CLOCK_12_24=0;   // 0=24hour 1=12hour
                                      break;
                                      }             
                                             
                                
                              default: {
                                       //  CAL.CLOCKMODE=1;
                                         CAL.CLOCK_US_INT=0; //  0=international  1=US format
                                        CAL.CLOCK_12_24=1;   // 0=24hour 1=12hour
                                        break;
                                       }
                 
                                
                              } // end of switch clockmode
  
              break;
              }
              
          case 91:  {  // Display Test
                display_version();
                cycle_display();
              break;

             }

      
  default:
    {
      // 
      
      break;
    }



  } // end of switch

prm_scroll(_UP);
//SYSTEM.DISPLAY_MODE=_WEIGHT; // switch back to weight on display

} // end of function

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
int max;
char quickjump=0;

max = sizeof(CALPRM); // get the number of parameters that are in the table
max--; // end of table is 1 back

if (SYSTEM.DISPLAY_MODE == _CAL) // check if we are not yet displaying calibration parameters
{//5                             // on first keypress we do not increment/decrement parameter number


 if (direction==_UP)  //are we scrolling up ------------------------------------------
 {//1

  if (SYSTEM.CAL_PARAMETER!=max)
  {//21
    SYSTEM.CAL_PARAMETER++;
  }//21
  else
  {//22
    SYSTEM.CAL_PARAMETER=1;  // roll over to P1.0
  }//22


  
 }//1
 else // we are scrolling down ------------------------------------------------------
  if (direction==_DOWN)  //are we scrolling up
 { //3 direction is down

   if (SYSTEM.CAL_PARAMETER!=1)
   {//2
     SYSTEM.CAL_PARAMETER--;
   }//2
    else
  {//23
 //   SYSTEM.CAL_PARAMETER=1;  // jump current position  
    SYSTEM.CAL_PARAMETER=max;  // roll backover to P9.x
  }//23

 }//3
 else // we are scrolling UP fast ---------------------------------------------------
  if (direction==_UP_H)  //are we scrolling up
 {//11
     if (timer(SCROLL_U,TIMEOUT))
     {//33
     for (i=SYSTEM.CAL_PARAMETER+1;i!=max;i++) // search through to find the next 10
     {//22

         if (i>max)
         {
          SYSTEM.CAL_PARAMETER=max;
          
           break;
         }


       val= CALPRM[i];

       result=val/10;

       val=(val-result*10);

        if (val==0)  // a zero means we found a number diviable with 10   
        {
          quickjump=1; // set flag to indicate quick jump
          SYSTEM.CAL_PARAMETER=i;  // jump current position            
          break; // break out of loop
        }
        
            

      }//22  end of loop search for quick jump up
     
     if (quickjump==0) // if quickjump failed, that means at end of table , loop  around
     {
       SYSTEM.CAL_PARAMETER=1;  // jump current position  
     }
     

     timer(SCROLL_U,START);

     }//33
 } //11
  else
  if (direction==_DOWN_H)  //are we scrolling down fast ---------------------------------
 {//66

     if (timer(SCROLL_D,TIMEOUT))
     {//44
     for (i=SYSTEM.CAL_PARAMETER-1;i!=0;i--)
     {//55

        if (i<1)
         {
        //  SYSTEM.CAL_PARAMETER=1;
           SYSTEM.CAL_PARAMETER=max;
           break;
         }



       val= CALPRM[i];

       result=val/10;

       val=(val-result*10);

        if (val==0)
        {
          quickjump=1;
          SYSTEM.CAL_PARAMETER=i;
          break;
        }
      }//55

      if (quickjump==0) // if quickjump failed, that means at end of table , loop  around
     {
      //  SYSTEM.CAL_PARAMETER=1;
       SYSTEM.CAL_PARAMETER=max;  // jump current position  
     }     
          
     timer(SCROLL_D,START);

     }//44

 }//66

}//5







 for (i=0;i!=7;i++)
       {
        DISPLAY.DIGITS[i]=' '; // blank display
       }









prm=SYSTEM.CAL_PARAMETER;


 // build the string to update the display
 // parameters are displayed as

    for (i=0;i!=6;i++) string[i]=' '; // fill with spaces

    val= CALPRM[prm]; // get current cal parameter

    // convert 2 digit integer to formatted string
    result=val/10;

    string[1]='F';       // insert P character
    string[2]=result+48;    // convert result to ascii
    val=val-(result*10); // subtract result to get remainder
    string[3]=val+48;    // convert result to ascii

     for (i=0;i!=5;i++)
    {
     DISPLAY.DIGITS[i]=string[i]; // copy string over to display
    }


 SYSTEM.DISPLAY_MODE=_CAL;   // set the display mode to show the current calibration parameter we are pointing to
 timer(DISPLAY_PRM, START);  // start the display timer for displaying the CAL parameter number

}   // end of function








//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
int option_select_count (int current, int tmin, int tmax, char str[] )
{

char string[8];
char i;  
char unitsflag=0;  
if (current > tmax) current=tmin;


int tcount=current;
char key;


if (str[0]=='M')
{

 unitsflag=1;
}


while(1) // loop until enter key is pressed
{

 key = scan_keypad(_EXTENDED);  // scan the keypad for keypresses



      switch (key)
      {//3

       case _UP:
                { // incrememnt parameter


                if (tcount < tmax)
                {
                  tcount++;
                }


                break;
                }


       case _DOWN:
                 { // decrement parameter

                if (tcount > tmin)
                {
                  tcount--;
                }


                 break;
                 }



      case _ENTER: {  // ENTER key is pressed, process calibration command that we are currrently pointing to in the menu



                   return(tcount);
                    }

                 
      case _LEFT: 
      case _CLEAR: {  // CLEAR or LEFT arrow aborts



                 return(current);  // return initial value with no changes        
                 
                 
                    }

      } // end of switch

// build the display to display the value being edited

//displayvalue(tcount,0,0,"");
sprintf(string,"%6d",tcount);

  for(i=0;i!=6;i++)
  {
   if (str[i]==0)
   {
    break; 
   }
   
   string[i]=str[i];
   
  }
    
  for (i=0;i!=6;i++)  // copy ticket editor to display, and refresh 
  {
    DISPLAY.DIGITS[i]=string[i];
  }
  
  //   update status to reflect display
  update_status(1);
  
  if (unitsflag==1)
  {//1
  
   switch(tcount)

    {

 case _KG:  {
               DISPLAY.STATUS1 = DISPLAY.STATUS1 | 0x04;  //  kg
              break;
              }


    case _LB:  {
               DISPLAY.STATUS1 = DISPLAY.STATUS1 | 0x01;  //  lb
              break;
              }

    case _G:  {
               DISPLAY.STATUS2 = DISPLAY.STATUS2 | 0x20;  //  g
              break;
              }

    case _OZ:  {
              DISPLAY.STATUS1 = DISPLAY.STATUS1 | 0x02;  //  oz
              break;
              }
      
  case _TON:  {
                // DISPLAY.STATUS1 = DISPLAY.STATUS1 | 0x04;  //  kg
               DISPLAY.STATUS2 = DISPLAY.STATUS2 | 0x20;  //  g
              break;
              }
    case _STON:  {
      //DISPLAY.STATUS1 = DISPLAY.STATUS1 | 0x02;  //  oz
                // DISPLAY.STATUS1 = DISPLAY.STATUS1 | 0x04;  //  kg
               DISPLAY.STATUS2 = DISPLAY.STATUS2 | 0x20;  //  g
              break;
              }  
      
      
         case _LTON:  {
      //DISPLAY.STATUS1 = DISPLAY.STATUS1 | 0x02;  //  oz
                // DISPLAY.STATUS1 = DISPLAY.STATUS1 | 0x04;  //  kg
               DISPLAY.STATUS2 = DISPLAY.STATUS2 | 0x20;  //  g
              break;
              }  
    } // end of switch
  
  update_status(0);
  }//1
 
   output_display(0) ; // update display
  
      } //3 end of while

}// end of function





//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
int option_select_ticket (int current, int tmin, int tmax, char str[] )
{

char string[8];
char i;  
  
if (current > tmax) current=tmin;


int tcount=current;
char key;

 
while(1) // loop until enter key is pressed
{

 key = scan_keypad(_EXTENDED);  // scan the keypad for keypresses



      switch (key)
      {//3

       case _UP:
                { // incrememnt parameter


                if (tcount < tmax)
                {
                  tcount++;
                }


                break;
                }


       case _DOWN:
                 { // decrement parameter

                if (tcount > tmin)
                {
                  tcount--;
                }


                 break;
                 }



      case _ENTER: {  // ENTER key is pressed, process calibration command that we are currrently pointing to in the menu



                   return(tcount);
                    }

                 
      case _LEFT: 
      case _CLEAR: {  // CLEAR or LEFT arrow aborts



                 return(-1);  // indicate user abort
                 
                 
                   }





      } // end of switch



// build the display to display the value being edited



//displayvalue(tcount,0,0,"");

sprintf(string,"%6d",tcount);
   

  for(i=0;i!=6;i++)
  {
   if (str[i]==0)
   {
    break; 
   }
   
   string[i]=str[i];
   
  }
    
  for (i=0;i!=6;i++)  // copy ticket editor to display, and refresh 
  {
    DISPLAY.DIGITS[i]=string[i];
  }
  
  
   output_display(0) ; // update display


      } //3 end of while




}// end of function









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


tcount=table[0]; // get the number of options there are to display




tpos=1;




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



      switch (key)
      {//3

       case _UP:
                { // incrememnt table pointer


                 tpos++;
                 if (tpos>tcount)
                 {
                   tpos=tcount;
                 }


                break;
                }


       case _DOWN:
                 { // decrement table pointer

                 tpos--;
                 if (tpos<1)
                 {
                   tpos=1;
                 }


                 break;
                 }





      case _ENTER: {  // ENTER key is pressed, process calibration command that we are currrently pointing to in the menu



                 return(table[tpos]);


                   }

                 
     case _CLEAR:            
     case _LEFT: {  // ABORT key is pressed, process calibration command that we are currrently pointing to in the menu



                 return(current);


                   }



      } // end of switch



// build the display to display the value being edited



 displayvalue( table[tpos],dec,0,str);
 output_display(dec); // update seven segment display

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



      switch (key)
      {//3

       case _UP:
                { // incrememnt table pointer


                 tpos++;
                 if (tpos>tcount)
                 {
                   tpos=tcount;
                 }


                break;
                }


       case _DOWN:
                 { // decrement table pointer

                 tpos--;
                 if (tpos<1)
                 {
                   tpos=1;
                 }


                 break;
                 }





      case _ENTER: {  // ENTER key is pressed, process calibration command that we are currrently pointing to in the menu
            

                 return(table[tpos]);


                   }
     case _CLEAR:
     case _LEFT: {  // ABORT key is pressed, process calibration command that we are currrently pointing to in the menu


       //  

                 return(current);  // return initial value, no change

                   }
//

      } // end of switch

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
          DISPLAY.DIGITS[j]= *(str  + i);  // copy string over to display
          j++;
         }

 }//1

   if (dec!=0) // if there is a dcimal
   {//2
    for (k=6;k!=0;k--)  // right shift entire string to remove space in string from decimal
            {
             DISPLAY.DIGITS[k]=DISPLAY.DIGITS[k-1];
            }
    dec=5-dec;
   }//2

  
   output_display(dec); // update seven segment display


  } //3 end of while



}// end of function

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
void span_scale(void)
{

float advalue;
float weight;
float span;
long ad;
float gradsize;
char string[6]= "SCALE ";
char string1[6]=" LOAD ";
char i;
char key;
char toggle;

// display_message(" LOAD ",4);
// display_message("SCALE ",4);

 while(1)
 {      
   
  if (timer(DEAD,TIMEOUT) )   // blink "E SCL" on the display
   {//1
     timer(DEAD,START);

       if (toggle==0)
       {
         toggle=1;
             for (i=0;i!=6;i++)
          {
           DISPLAY.DIGITS[i]=string1[i];
          }
          
       }
        else
        {
          toggle=0;
            for (i=0;i!=6;i++)
          {
           DISPLAY.DIGITS[i]=string[i];
          }         
        }

        }//1

      scan_AD();        // read AD converter value and filter  
      get_weight();     // get AD values and calculate weights
  
      update_status(2); // blank status display
      output_display(0);

      key=scan_keypad(_EXTENDED);


       if (key==_ENTER)
       {//2
          break;
        }//2       
        
       if ( (key==_CLEAR) || (key ==_LEFT) )  // abort with LEFT or CLEAR key
          {
           display_message( "ABORT ",2);  // error outside of zero range  
           return; // abort deadloading 
          }             
 } // end of while
  
 VALUE = parameter_entry(CAL.CALVALUE,6,1,"      ",0);

              if (VALUE < 1 ) // check for valid entry
               {               
               return; // abort calibration
               }
            
            
                CAL.CALVALUE=VALUE;  // update new test weight value
                displayvalue(CAL.CALVALUE,0,0,"");
                scale_status();   // update scale status anunciators
                output_display(CAL.DEC);
            
                gradsize = CAL.GRAD;

              if (CAL.DEADZERO==1)
              {
                gradsize=gradsize*10;
              }
              else
              if (CAL.DEADZERO==2)
              {
                gradsize=gradsize*100;
              }     
              //       
              
ad = CAL_ADaverage(); // get averaged scale AD value
ad = ad-SCALE.ZERO; // calculate ad counts ref to scale zero
advalue = (float) ad; //*CAL.GSPAN;
weight= (float)( CAL.CALVALUE / gradsize);   // divide by grad size ie: 1,2,5,10,20,50,100
span = weight/advalue;  // do floating point calculations
CAL.SPAN=span;
CAL.D_UNITS[0]= CAL.UNITS;  // set powerup default unit
display_message( (char *) &DONEmsg,2);

}  // end of funtion

//-----------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
void span_scale_linear(void)
{

float advalue;
float weight;
float span;
long ad;
float gradsize;
char string[6]= " SCALE";
char string1[6]=" LOAD ";
char string2[6]=" CP-A ";
char string3[6]=" CP-B ";
char string4[6]=" CP-C ";
char string5[6]=" CP-D ";

char i;
char key;
char toggle;

long value=0;
// display_message(" LOAD ",4);
// display_message("SCALE ",4);

 while(1)
 {      
   
  if (timer(DEAD,TIMEOUT) )   // blink "E SCL" on the display
   {//1
     timer(DEAD,START);

       if (toggle==0)
       {
         toggle=1;
             for (i=0;i!=6;i++)
          {
           DISPLAY.DIGITS[i]=string1[i];
          }
          
       }
        else
        {
          toggle=0;
            for (i=0;i!=6;i++)
          {
           DISPLAY.DIGITS[i]=string2[i];
          }
         
        }


        }//1

      scan_AD();        // read AD converter value and filter  
      get_weight();     // get AD values and calculate weights
  
      update_status(2); // blank status display
      output_display(0);

      key=scan_keypad(_EXTENDED);


       if (key==_ENTER)
       {//2
          break;
        }//2       
        
       if ( (key==_CLEAR) || (key ==_LEFT) )  // abort with LEFT or CLEAR key
          {
           display_message( "ABORT ",2);  // error outside of zero range  
           return; // abort deadloading 
          }             
 } // end of while
  
 VALUE = parameter_entry(CAL.CALVALUE,6,1,"      ",0);
              
    if (VALUE < 1 ) // check for valid entry
               {               
               return; // abort calibration
               }
 
                CAL.CALVALUE = VALUE;  // update new test weight value
                displayvalue(CAL.CALVALUE,0,0,"");
                scale_status();   // update scale status anunciators
                output_display(CAL.DEC);
            
                gradsize = CAL.GRAD;

              if (CAL.DEADZERO==1)
              {
                gradsize=gradsize*10;
              }
              else
              if (CAL.DEADZERO==2)
              {
                gradsize=gradsize*100;
              } //      
 
ad = CAL_ADaverage(); // get averaged scale AD value
ad = ad-SCALE.ZERO; // calculate ad counts ref to scale zero
CAL.AD1 = ad;
advalue = (float) ad; //*CAL.GSPAN;
weight= (float)( CAL.CALVALUE / gradsize);   // divide by grad size ie: 1,2,5,10,20,50,100
span = weight/advalue;  // do floating point calculations
CAL.SPAN1=span;
value=VALUE;  // update new test weight value
  
  
 while(1)
 {      
   
  if (timer(DEAD,TIMEOUT) )   // blink "E SCL" on the display
   {//1
     timer(DEAD,START);

       if (toggle==0)
       {
         toggle=1;
             for (i=0;i!=6;i++)
          {
           DISPLAY.DIGITS[i]=string1[i];
          }
          
       }
        else
        {
          toggle=0;
            for (i=0;i!=6;i++)
          {
           DISPLAY.DIGITS[i]=string3[i];
          }
         
        }


        }//1

      scan_AD();        // read AD converter value and filter  
      get_weight();     // get AD values and calculate weights
  
      update_status(2); // blank status display
      output_display(0);

      key=scan_keypad(_EXTENDED);


       if (key==_ENTER)
       {//2
          break;
        }//2       
        
   
 } // end of while
  
 VALUE = parameter_entry(CAL.CALVALUE,6,1,"      ",0);       
                
                CAL.CALVALUE=VALUE;  // update new test weight value
                displayvalue(CAL.CALVALUE,0,0,"");
                scale_status();   // update scale status anunciators
                output_display(CAL.DEC);
            
                gradsize = CAL.GRAD;

              if (CAL.DEADZERO==1)
              {
                gradsize=gradsize*10;
              }
              else
              if (CAL.DEADZERO==2)
              {
                gradsize=gradsize*100;
              }     
              //       
 if(VALUE > value)
   {             
ad = CAL_ADaverage(); // get averaged scale AD value
ad = ad-SCALE.ZERO; // calculate ad counts ref to scale zero
CAL.AD2= ad;
advalue = (float) ad; //*CAL.GSPAN;
weight= (float)( CAL.CALVALUE / gradsize);   // divide by grad size ie: 1,2,5,10,20,50,100
span = weight/advalue;  // do floating point calculations
CAL.SPAN2=span;
value=VALUE;  // update new test weight value
   }
    else
   {
   CAL.AD2= CAL.AD1;
   CAL.SPAN2= CAL.SPAN1;
   }
  
while(1)
 {         
  if (timer(DEAD,TIMEOUT) )   // blink "E SCL" on the display
   {//1
     timer(DEAD,START);

       if (toggle==0)
       {
         toggle=1;
             for (i=0;i!=6;i++)
          {
           DISPLAY.DIGITS[i]=string1[i];
          }
          
       } // 
        else
        {
          toggle=0;
            for (i=0;i!=6;i++)
          {
           DISPLAY.DIGITS[i]=string4[i];
          }
         
        } // 


        }//1

      scan_AD();        // read AD converter value and filter  
      get_weight();     // get AD values and calculate weights
  
      update_status(2); // blank status display
      output_display(0);

      key=scan_keypad(_EXTENDED);

       if (key==_ENTER)
       {//2
          break;
        }//2       
        
       if ( (key==_CLEAR) || (key ==_LEFT) )  // abort with LEFT or CLEAR key
          {
           display_message( "ABORT ",2);  // error outside of zero range  
           return; // abort deadloading 
          }             
 } // end of while
  
 VALUE = parameter_entry(CAL.CALVALUE,6,1,"      ",0);
           
            
                CAL.CALVALUE=VALUE;  // update new test weight value
                displayvalue(CAL.CALVALUE,0,0,"");
                scale_status();   // update scale status anunciators
                output_display(CAL.DEC);
            
                gradsize = CAL.GRAD;

              if (CAL.DEADZERO==1)
              {
                gradsize=gradsize*10;
              }
              else
              if (CAL.DEADZERO==2)
              {
                gradsize=gradsize*100;
              }     
              //       
  if(VALUE > value)
   {                    
ad = CAL_ADaverage(); // get averaged scale AD value
ad = ad-SCALE.ZERO; // calculate ad counts ref to scale zero
CAL.AD3=ad;
advalue = (float) ad; //*CAL.GSPAN;
weight= (float)( CAL.CALVALUE / gradsize);   // divide by grad size ie: 1,2,5,10,20,50,100
span = weight/advalue;  // do floating point calculations
CAL.SPAN3=span;
value = VALUE;
   }
   else
   {
   CAL.AD3= CAL.AD2;
   CAL.SPAN3= CAL.SPAN2;
   }
  
while(1)
 {      
   
  if (timer(DEAD,TIMEOUT) )   // blink "E SCL" on the display
   {//1
     timer(DEAD,START);

       if (toggle==0)
       {
         toggle=1;
             for (i=0;i!=6;i++)
          {
           DISPLAY.DIGITS[i]=string1[i];
          }
          
       }
        else
        {
          toggle=0;
            for (i=0;i!=6;i++)
          {
           DISPLAY.DIGITS[i]=string5[i];
          }
         
        } //   

        }//1

      scan_AD();        // read AD converter value and filter  
      get_weight();     // get AD values and calculate weights
  
      update_status(2); // blank status display
      output_display(0);

      key=scan_keypad(_EXTENDED);

       if (key==_ENTER)
       {//2
          break;
        }//2       
        
       if ( (key==_CLEAR) || (key ==_LEFT) )  // abort with LEFT or CLEAR key
          {
           display_message( "ABORT ",2);  // error outside of zero range  
           return; // abort deadloading 
          }             
 } // end of while
  
 VALUE = parameter_entry(CAL.CALVALUE,6,1,"      ",0);
            
                CAL.CALVALUE=VALUE;  // update new test weight value
                displayvalue(CAL.CALVALUE,0,0,"");
                scale_status();   // update scale status anunciators
                output_display(CAL.DEC);
            
                gradsize = CAL.GRAD;         
                                          

              if (CAL.DEADZERO==1)
              {
                gradsize=gradsize*10;
              }
              else
              if (CAL.DEADZERO==2)
              {
                gradsize=gradsize*100;
              }     
              //       
 if(VALUE > value)
   {                                   
ad = CAL_ADaverage(); // get averaged scale AD value
ad = ad-SCALE.ZERO; // calculate ad counts ref to scale zero
advalue = (float) ad; //*CAL.GSPAN;
weight= (float)( CAL.CALVALUE / gradsize);   // divide by grad size ie: 1,2,5,10,20,50,100
span = weight/advalue;  // do floating point calculations
CAL.SPAN=span;
   }
   else
   {   
   CAL.SPAN = CAL.SPAN3;
   }
       
CAL.D_UNITS[0]= CAL.UNITS;  // set powerup default unit
display_message( (char *) &DONEmsg,2);

}  // end of funtion

//-----------------------------------------------------------------------------
void deadload_scale(void)
{
char string[6]= "SCALE ";
char string1[6]="CLEAR ";
char i;
char key;
char toggle;

 while(1)
 {      
   
  if (timer(DEAD,TIMEOUT) )   // blink "E SCL" on the display
   {//1
     timer(DEAD,START);

       if (toggle==0)
       {
         toggle=1;
             for (i=0;i!=6;i++)
          {
           DISPLAY.DIGITS[i]=string1[i];
          }
          
       }
        else
        {
          toggle=0;
            for (i=0;i!=6;i++)
          {
           DISPLAY.DIGITS[i]=string[i];
          }         
        }
        }//1
  
      scan_AD();        // read AD converter value and filter  
      get_weight();     // get AD values and calculate weights
    
      update_status(2); // blank status display
      output_display(0);

      key=scan_keypad(_EXTENDED);

       if (key==_ENTER)
       {//2
         
            break;
       }//2
       
       
       if ( (key==_CLEAR) || (key ==_LEFT) )  // abort with LEFT or CLEAR key
          {
            display_message( "ABORT ",2);  // error outside of zero range    
           return; // abort deadloading 
          }
             
 } // end of while

            CAL.DEADLOAD = CAL_ADaverage();
            SCALE.ZERO=CAL.DEADLOAD;
           
            CAL.AD3=CAL.DEADLOAD;
           CAL.AD2=CAL.DEADLOAD;
           CAL.AD1=CAL.DEADLOAD;
          
                      
           storezero(); // save the zero to non volatile memory
            SCALE.CENTEROFZERO=calc_centerofzero();  // calculate the center of zero range
            SCALE.ZERORANGE=calc_zerorange(); // calculate the zero range of the indcator            
            display_message( (char *) &DONEmsg,2);
  
}  // end of function
// 
//----------------------------------------------------------------------------------------------------
void piece_scale(void)
{
char string[6]= "PIECE ";
char string1[6]=" LOAD ";
char i;
char key;
char toggle;
float advalue;
float weight;
float span;
long ad;
float gradsize;
long VALUE;

 while(1)
 { 
   
  if (timer(DEAD,TIMEOUT) )   // blink "E SCL" on the display
   {//1
     timer(DEAD,START);

       if (toggle==0)
       {
         toggle=1;
             for (i=0;i!=6;i++)
          {
           DISPLAY.DIGITS[i]=string1[i];
          }
          
       }
        else
        {
          toggle=0;
            for (i=0;i!=6;i++)
          {
           DISPLAY.DIGITS[i]=string[i];
          }
     
        }

        }//1

      scan_AD();        // read AD converter value and filter  
      get_weight();     // get AD values and calculate weights
  
      update_status(2); // blank status display
      output_display(0);

      key=scan_keypad(_EXTENDED);

       if (key==_ENTER)
       {//2
          break;
        }//2       
        
       if ( (key==_CLEAR) || (key ==_LEFT) )  // abort with LEFT or CLEAR key
          {
           display_message( "ABORT ",2);  // error outside of zero range  
           return; // abort deadloading 
          }             
 
 } // end of while
       
 
       if( SCALE.GROSSWEIGHT !=0)
    {    
    //if( CAL.PIECE_CNT > 0)
   // {    
      
      /* 
        ad = CAL_ADaverage(); // get averaged scale AD value
        ad=ad-SCALE.ZERO; // calculate ad counts ref to scale zero
        advalue = (float) ad;
        weight= (float) CAL.PIECE_CNT;   // divide by grad size ie: 1,2,5,10,20,50,100
        span = weight/advalue;  // do floating point calculations
        CAL.PIECE_AVG = span;      
     */     
        CAL.PIECE_AVG = (float) ( CAL.PIECE_CNT /(SCALE.GROSSWEIGHT * CAL.GRAD));          
       // dweight=(long) weight;   //typecast as integer strips integer value from float
       //  fraction=weight-dweight; //get the remaining fractional value from float
       //fraction now holds the fractional part of the weight
       // if (fraction> 0.5) dweight++;  //if the fractional value is > 0.5 then round up the integer value
       // dweight now holds a rounded integer value of the weight                      
       // CAL.PIECE_AVG = dweight;         
        display_message( (char *) &DONEmsg,2);
    }        
}  // end of funtion

//--------------------------------------------------------------------------------
//  enter calibration
//  mode = 0  password
//  mode = 1  switch
//--------------------------------------------------------------------------------
void  enter_cal(char mode)
{
double f1;
double f2;
double fraction;
double pass;
long l;
char goodpass=0;
int i;
 
display_message("ENTER ",2);
display_message("SETUP ",2);

 if(CAL.LEGAL == 1) 
             {//1 by pass pass word            
             
if(read_legal()) // disable output
 {
   goodpass=1; // password is good   
 }
             }//1
 else
 {
 // 
 pass = parameter_entry(0,4,2,"-0000-",1);
            
 if (pass==CAL.PASSWORD)
 {//1
   goodpass=1; // password is good
 }//1
 else // check for back door entry
 {//2
     if (pass==9999)  // go into password bypass authoration mode
     {//3

        // display a random number on display and call factory technical support
        // factory takes number and divides it by 3.14
        // first digits after the decimal is the temp backdoor password
        l= lTIMERTICS;

        // make sure the value is less than 9999, which is the max we can display
        while ( l > 9998)
        {
          l=l>>1;  // shift right to reduce value
        }
        
        f1=l;      // now take random number, and divide it with 3.14 to create a float
        f2=3.14;
        f1=f1/f2;

        fraction=modf(f1,&pass); //seperate fraction part, discard integer
        // the first 4 digits after the decimal is the temp password
        fraction=fraction*10000;  // left shift decimal 4 positions
        fraction=modf(fraction,&pass); //seperate fraction part, discard integer
        // pass now holds the back door value
        // l holds the randoum number
        // display random number, and wait for backddor entry
        i = parameter_entry(l,4,2,"-    -",1);

        if (i==pass)  // backdoor accepted
        {
         CAL.PASSWORD=1;  // reset password to factory
         goodpass=1; // password is good
        }

     }//3

 }//2

 } //  
  
 if (goodpass==1)   // if the password is correct then enter calibration
 {
    
                //  
                SCALE.TAREWEIGHT = 0;
                SCALE.NETMODE=0;  // clear the netmode flag
                SCALE.TAREMODE=0;  // clear the netmode flag
                SCALE.DIS_TAREWEIGHT=0;
                SCALE.DIS_NETWEIGHT=0;   
                SCALE.DIS_UNITS=CAL.UNITS;
                SYSTEM.CAL=1;
                display_message(" PASS ",2);
                SYSTEM.DISPLAY_MODE = _CAL;
                prm_scroll(0);
              
 }
 else // password fail
        {
          // 
          display_message(" FAIL ",2);
        }

}  // end of function

//--------------------------------------------------------------------
float spanedit(float span)
{
float dspan;
unsigned long value;
unsigned long value2;

dspan=span;
dspan=dspan * 1000000; // left shift decimal positiion

value= (long ) dspan; // convert to long

value2 = parameter_entry(value,6,1,"      ",1); // display or edit span

if (value2==value)  // if no change
  {
   return(span); // return un modified span value
  }

  dspan=value2;
  dspan=dspan/1000000; // right shift decimal position
  return(dspan);

} // end of function

//-----------------------------------------------------------------------------------------
//  exit calibration
//-----------------------------------------------------------------------------------------
void exit_cal(void)
{
unsigned int checksum;

             //update CAL audit trail
             checksum = update_checksum((char *) &CAL.CAL_START, (char *) &CAL.CAL_CHECKSUM);
             //checksum = update_checksum((char *) &CAL.CAL_START, (char *) &CAL.CAL_END);
             
             if (checksum!=CAL.CAL_CHECKSUM)
             {//1
              CAL.AUDIT_CAL++; // increment audit trail count
              
              if (CAL.AUDIT_CAL < 1) // check for rollover
               {
                 CAL.AUDIT_CAL = 1;
               }

              if (CAL.AUDIT_CAL > 999) // check for rollover
               {
                 CAL.AUDIT_CAL = 1;
               }

               CAL.CAL_CHECKSUM=checksum; // update with nre checksum
             }//1

             //update Config audit trail
             checksum=update_checksum((char *) &CAL.CFG_START, (char *) &CAL.CFG_CHECKSUM);

             if (checksum!=CAL.CFG_CHECKSUM)
             {//1
              CAL.AUDIT_CFG++; // increment audit trail count
              
              if (CAL.AUDIT_CFG < 1) // check for rollover
               {
                 CAL.AUDIT_CFG= 1;
               }

              if (CAL.AUDIT_CFG > 999) // check for rollover
               {
                 CAL.AUDIT_CFG= 1;
               }

               CAL.CFG_CHECKSUM=checksum; // update with new checksum
             }//1
             
             SCALE.ZERORANGE=calc_zerorange(); // calculate the zero range of the indcator
             calc_scaleover(); // calculate scale over value
             
             write_calibration();             
             display_message( "SAvED ",2);
             display_message( "SETUP ",2);                     
             SYSTEM.CAL=0;
              // initialize scale system from calibration 
             SYSTEM.DISPLAY_MODE=_WEIGHT; // switch back to weight on display
// 
}  // end of function

//--------------------------------------------------------------------
float gspanedit(float gspan)
{
float dspan;
unsigned long value;
unsigned long value2;

dspan=gspan;
dspan = dspan * 10000; // left shift decimal positiion

value= (unsigned long ) dspan; // convert to long
SCALE.DIS_DEC = 4;
value2 = parameter_entry(value,6,1,"      ",0); // display or edit span

if (value2 > 99000)  // if no change
  {
   return(gspan); // return un modified span value
  }
 if (value2 < 97000)  // if no change
  {
   return(gspan); // return un modified span value
  }
 if (value2==value)  // if no change
  {
   return(gspan); // return un modified span value
  }
 
  dspan=value2;
  dspan=dspan / 10000; // right shift decimal position
  return(dspan);

} // end of function


