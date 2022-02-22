#include "machine.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>

#include "ascify.h"

uint APPLAPI In3Out4(puchar input, uint inlen, puchar output, uint outlen)
{
  uchar in[3];
  uint i, o, n;
  for (i = o = 0; i < inlen; i += 3)
  {
    n = inlen - i;
    memset(in, 0, 3);
    memcpy(in, input + i, n > 3 ? 3 : n);
    output[o++] = (in[0] >> 2) + 32;
    if (o >= outlen) break;
    output[o++] = (((in[0] & 0x03) << 4) | (in[1] >> 4)) + 32;
    if (o >= outlen) break;
    output[o++] = (((in[1] & 0x0F) << 2) | (in[2] >> 6)) + 32;
    if (o >= outlen) break;
    output[o++] = (in[2] & 0x3F) + 32;
    if (o >= outlen) break;
  }
  return o;
}

uint APPLAPI In4Out3(puchar input, uint inlen, puchar output, uint outlen)
{
  uchar in[4];
  uint i, o, n;
  for (i = o = 0; i < inlen; i += 4)
  {
    n = inlen - i;
    memset(in, 0, 4);
    memcpy(in, input + i, n > 4 ? 4 : n);
    in[0] -= 32;
    in[1] -= 32;
    output[o] = ((in[0] & 0x3F) << 2) | ((in[1] & 0x30) >> 4);
    if (++o >= outlen) break;
    in[2] -= 32;
    output[o] = ((in[1] & 0x0F) << 4) | ((in[2] & 0x3c) >> 2);
    if (++o >= outlen) break;
    in[3] -= 32;
    output[o] = ((in[2] & 0x03) << 6) | (in[3] & 0x3F);
    if (++o >= outlen) break;
  }
  return o;
}

uint APPLAPI Encode12To8(puchar input, // Expects an 12 DIGIT String
   puchar output) // Returns an 8 ASCII String
   {
     union
     {
       char c[6];
       uint u[3];
     }
     un;
     char work[5];
     work[4] = 0;
     memcpy(work, input, 4);
     un.u[0] = atoi(work);
     memcpy(work, input + 4, 4);
     un.u[1] = atoi(work);
     memcpy(work, input + 8, 4);
     un.u[2] = atoi(work);
     In3Out4((puchar)un.c, 6, output, 8);
     return 0;
}

uint APPLAPI Decode8To12(puchar input, // Expects an 8 ASCII String
   puchar output) // Returns an 12 DIGIT String
   {
     union
     {
       char c[6];
       ushort u[3];
     }
     un;
     char work[6];
     In4Out3(input, 8, (puchar)un.c, 6);
     sprintf(work, "%04u", un.u[0]);
     memcpy(output, work, 4);
     sprintf(work, "%04u", un.u[1]);
     memcpy(output + 4, work, 4);
     sprintf(work, "%04u", un.u[2]);
     memcpy(output + 8, work, 4);
     return 0;
}

#if defined(MAKEMAIN)
int main(void)
{
  unsigned char inbuff[1497], outbuff[1996];
  int i, no;
  for (i = 0; i < 1498; i++)
    inbuff[i] = i % 256;
  inbuff[0] = 123;
  inbuff[1] = 0;
  no = In3Out4(inbuff, sizeof(inbuff), outbuff, sizeof(outbuff));
  printf("Outsize = %d\n", no);
  for (i = 0; i < no; i++)
    printf("%c%s", outbuff[i], i % 79 == 78 ? "\n" : "");
  printf("\n\n");
  no = In4Out3(outbuff, sizeof(outbuff), inbuff, sizeof(inbuff));
  printf("Outsize = %d\n", no);
  for (i = 0; i < no; i++)
    printf("%02x%s", inbuff[i], i % 39 == 38 ? "\n" : "");
  printf("\n\n");
  Encode12To8("012345678901", outbuff);
  printf("Outsize = %d\n", 8);
  for (i = 0; i < 8; i++)
    printf("%c", outbuff[i]);
  printf("\n\n");
  Decode8To12(outbuff, inbuff);
  printf("Outsize = %d\n", 12);
  for (i = 0; i < 12; i++)
    printf("%c", inbuff[i]);
  printf("\n\n");
  Encode12To8("123456789010", outbuff);
  printf("Outsize = %d\n", 8);
  for (i = 0; i < 8; i++)
    printf("%c", outbuff[i]);
  printf("\n\n");
  Decode8To12(outbuff, inbuff);
  printf("Outsize = %d\n", 12);
  for (i = 0; i < 12; i++)
    printf("%c", inbuff[i]);
  printf("\n\n");
  Encode12To8("111111111111", outbuff);
  printf("Outsize = %d\n", 8);
  for (i = 0; i < 8; i++)
    printf("%c", outbuff[i]);
  printf("\n\n");
  Decode8To12(outbuff, inbuff);
  printf("Outsize = %d\n", 12);
  for (i = 0; i < 12; i++)
    printf("%c", inbuff[i]);
  printf("\n\n");
}
#endif


