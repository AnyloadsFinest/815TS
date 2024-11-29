
//---------------------------------------------------------------------------
 // Calculate a running CRC16 value
 // CITT CRC16 polynomial ^16 + ^12 + ^5 + 1 */
 //---------------------------------------------------------------------------
 unsigned short  hello(unsigned char b, unsigned short acc)
 {

   acc ^= b;
   acc  = (acc >> 8) | (acc << 8);
   acc ^= (acc & 0xff00) << 4;
   acc ^= (acc >> 8) >> 4;
   acc ^= (acc & 0xff00) >> 5;
   return (acc);
 }  // end of function

