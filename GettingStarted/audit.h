


#ifndef __audit_INCLUDED
#define __audit_INCLUDED


unsigned int update_checksum(char *  start, char *  end );
unsigned int  crc16(unsigned char ser_data, unsigned int crc);
void display_audit(void);
char check_checksum(void);
#endif



