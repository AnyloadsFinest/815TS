#include "includes.h"




int ticketbuffer(int data,int address,char mode)
 {


return(0); // dummy return

}// end of function


void init_ticketbuffer(void)
{
 

} // end of funtion







void  mark_tic_end(int address)
{

}// end of function


void mark_buff_end(int address)
{

}// end of function


void  mark_ctrl_func (int address,int cmd)
{


}// end of function



void  mark_prn_func (int address,int cmd)
{



}// end of function



void  mark_tic_start (int address,int number)
{



}// end of function




int searchticket(signed int ticketnumber)
{



    return(-1); // search failed!

} // end of funtion






//--search_buffer_end()-------------------------------------------------------
// search for the end of buffer marker and return the address
//
//---------------------------------------------------------------------------------

int  search_buffer_end(void)
{


} // end of funtion





//--search_ticket_end()-------------------------------------------------------
//  search for the end of a ticket marker for a specific ticket
//
//
//-----------------------------------------------------------------------------
int search_ticket_end(int ticket)
{



}// end of function




//--append_ticket()-----------------------------------------------------------
// append a new ticket to the end of the buffer
//
// this functions inserts a new ticket number followed by a space and end of ticket marker
// the address for where the new ticket starts is returned
//------------------------------------------------------------------------------
void append_ticket(signed int newticket)
{


}

int next_available(int start)
{


return(0);
} // end of function


//--ticket_count()--------------------------------------------------------------
//
// return number of tickets defined in the ticket buffer
//
//
//
//-----------------------------------------------------------------------------

int ticket_count(void)
{


 return(0);

}// end of function



//--insert_data()---------------------------------------------------------------
// This fuction right shifts the entire buffer array from the position "address"
//
// the data that is pointed to by address is shifted to the right
// new data is then written to the address position
//
// This function is used to insert a byte into the ticket buffer

// if the overwrite flag is set then we overwrite the data pointed to by address
//-----------------------------------------------------------------------------

int  insert_data(int data,int address,char overwrite)
{

return(0);

} // end of funtion



int  overwrite_data(int data,int address)
{


return(1);

} // end of funtion




void  delete_data(int address)
{




} // end of funtion




//---delete_ticket()----------------------------------------------------------
char  delete_ticket(int ticketnumber)
{



return(1);
}// end of function

















//------------------------------------------------------------------------
// append a ticket control cmd to a ticket
//------------------------------------------------------------------------
void  add_cmd(int cmd)
{


}// end of function




//------------------------------------------------------------------------
// add int
//------------------------------------------------------------------------
void addint(int cmd)
{

}// end of function







//------------------------------------------------------------------------
// append a printer control cmd to a ticket
//------------------------------------------------------------------------
void  prnctrl(int cmd)
{


}// end of function








//--ticket_editor()-----------------------------------------------------------
// This function is the ticket editor used for editing tickets. A ticket must exist before
// it can be edited. Calling this function changes the display to edit ticket mode.
// Once this function is called, it does not exit until ticket editing is completed.
//
//------------------------------------------------------------------------------------
int  ticket_editor(int ticketnumber)
{

return(0);
} // end of function




//------------------------------------------------------------------------------
// factory defualt ticket
//------------------------------------------------------------------------------
void init_factory_ticket(void)
{
 append_ticket(1);  // create it
 add_cmd(7); //print tine
 add_cmd(8); //print date
 prnctrl(1); // add print control CR/LF
 prnctrl(1); // add print control CR/LF
 add_cmd(1); //print gross
 prnctrl(1); // add print control CR/LF
 add_cmd(2); //print tare
 prnctrl(1); // add print control CR/LF
 add_cmd(3); //print net
 prnctrl(1); // add print control CR/LF
 addint(10) ; // linefeed
 addint(10) ; // linefeed 
 addint(10) ; // linefeed 
 addint(10) ; // linefeed
 addint(10) ; // linefeed 
 addint(10) ; // linefeed 
} // end of function






























