//******************************************************************//
//***                          crc_16_CCITT                      ***//
//------------------------------------------------------------------//
//***                    www.drdobbs.com/199904926               ***//
//***                                                            ***//
//***                                  16  12  5                 ***//
//***  The CCITT CRC 16 polynomial is X + X + X + 1.             ***//
//***  In binary, this is : 1 0001 0000 0010 0001 = 0x11021.     ***//
//***  A 17 bit register is simulated by testing the MSB before  ***//
//***  shifting the data, which affords us the luxury of         ***//
//***  specifying the polynomial as a 16 bit value, 0x1021.      ***//
//***  Due to the way in which we process the CRC, the bits of   ***//
//***  the polynomial are stored in reverse order. This makes    ***//
//***  the polynomial 0x8408.                                    ***//
//******************************************************************//


//       16  15  2
//      X + X + X + 1
//      => 1 1000 0000 0000 0101 => 0x8005 
//      reversed => 0xa001

#define POLY 0x8408
//#define POLY 0xa001

//******************************************************************//
//***  note: when the crc is included in the message, the valid  ***//
//***  crc is:                                                   ***//
//***      0xF0B8, before the compliment and byte swap,          ***//
//***      0x0F47, after compliment, before the byte swap,       ***//
//***      0x470F, after the compliment and the byte swap.       ***//
//***                                                            ***//
//***  Test string = "T"                                         ***//
//***    => crc = 0xd9e4                                         ***//
//***                                                            ***//
//***  Test string = "THE,QUICK,BROWN,FOX,0123456789"            ***//
//***    => crc = 0x6e20                                         ***//
//***                                                            ***//
//******************************************************************//

//extern  crc_ok;
//int     crc_ok = 0x470F;

//******************************************************************//
//***                                                            ***//
//***  CALLING SEQUENCE                                          ***//
//***      crc = crc16(data, len);                               ***//
//***                                                            ***//
//***  PARAMETERS                                                ***//
//***      data    <-- address of start of data block            ***//
//***      len     <-- length of data block                      ***//
//***                                                            ***//
//***  RETURNED VALUE                                            ***//
//***      crc16 value. Calculated using the 16 bit ccitt polynomial. ***//
//***                                                            ***//
//***  NOTES                                                     ***//
//***      The CRC is preset to all 1's to detect errors         ***//
//***        involving a loss of leading zero's.                 ***//
//***      The CRC 16 is generated in LSB MSB order.             ***//
//***      Two ways to verify the integrity of a received msg    ***//
//***        or block of data:                                   ***//
//***        1) Calculate the crc on the data, and compare it to ***//
//***           the crc calculated previously. The location of   ***//
//***           the saved crc must be known.                     ***//
//***        2) Append the calculated crc to the end of the      ***//
//***           data. Now calculate the crc of the data and its  ***//
//***           crc. If the new crc equals the value in          ***//
//***           "crc_ok", the data is valid.                     ***//
//***                                                            ***//
//*** PSEUDO CODE:                                               ***//
//***      initialize crc (-1)                                   ***//
//***      DO WHILE count NE zero                                ***//
//***        DO FOR each bit in the data byte, from LSB to MSB   ***//
//***          IF (LSB of crc) EOR (LSB of data)                 ***//
//***            crc := (crc / 2) EOR polynomial                 ***//     
//***          ELSE                                              ***//
//***            crc := (crc / 2)                                ***//
//***          FI                                                ***//
//***        OD                                                  ***//     
//***      OD                                                    ***//
//***      1's compliment and swap bytes in crc                  ***//
//***      RETURN crc                                            ***//
//***                                                            ***//
//******************************************************************//
WORD crc16(unsigned char *data_p, int length)
{
       int i;
       WORD data;
       WORD crc = 0xffff ;
       
       if (length == 0)
         return (~crc);
       
       do 
       {
         data = (WORD)0xff & *data_p++ ;
         for (i = 0 ; i < 8; i++) 
         {
           if ((crc & 0x0001) ^ (data & 0x0001))
             crc = (crc >> 1) ^ POLY;
           else
             crc >>= 1;
           data >>= 1 ;
         }
       } while (--length);
       
       crc = ~crc;
       
       data = crc;
       crc = (crc << 8) | (data >> 8 & 0xFF);
       
       return crc;
}
