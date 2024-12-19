#ifndef __TICKETEDIT_INCLUDED
#define __TICKETEDIT_INCLUDED

#define TICSTART 200

#define BUFFEND 0x8000
#define PRNCTRL 0x4000
#define TICCTRL 0x2000
#define TICEND  0x1000
#define TICNUM  0x0800

#define ACC_MAX 20     // number of registers

#define READ 0
#define WRITE 1
  

 int   ticketbuffer(int data,int address,char mode);
 void  save_tickets(void);
 void  load_tickets(void);

 void  init_ticketbuffer(void);

 void  mark_tic_end(int address);
 void  mark_buff_end(int address);
 void  mark_ctrl_func (int address,int cmd);
 void  mark_prn_func (int address,int cmd);
 void  mark_tic_start (int address,int number);

 int   searchticket(signed int ticketnumber);
 int   search_buffer_end(void);
 int   search_ticket_end(int ticket);
 void  append_ticket(signed int newticket);
 int   next_available(int start);

 int   ticket_count(void);
 int   insert_data(int data,int address,char overwrite);
 int   overwrite_data(int data,int address);
 void  delete_data(int address);
 char  delete_ticket(int ticketnumber);
 int   ticket_editor(int ticketnumber);
 void  init_factorytickets();
 int   current_tic(int tic,char mode);

 void  add_cmd(int num);
 void  prnctrl(int cmd);
 void  add_str(char string[]);
 void  add_cr(char cnt);
 void  init_factorytickets();
 int  ticket_editor(int ticketnumber);
 void init_factory_ticket(void);

#endif
 

