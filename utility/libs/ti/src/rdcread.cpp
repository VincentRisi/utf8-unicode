#include "versions.h"
#define _VERSION_NO "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
COMPILED_VERSION(_VERSION_NO);

#include "machine.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <memory.h>

#ifdef M_W32
#include <io.h>
#endif

#include <fcntl.h>
#include <sys/stat.h>

#include "tools.h"
#include "rdc.h"

static uchar InBuff[8192];
static uchar OutBuff[8192];

#if defined(M_AIX) || defined(M_GNU)
long filelength(int handle)
{
  long current = lseek(handle, 0, SEEK_CUR);
  long size = lseek(handle, 0, SEEK_END);
  lseek(handle, current, SEEK_SET);
  return size;
}
#endif

int main(int argc, char *argv[])
{
  long Size;
  ushort Read, ToWrite, Length, Sign;
  int fin, fout;
  if (argc < 3)
  {
    printf("usage - rdcread infile outfile\n");
    return -1;
  }
  fin = open(argv[1], O_RDONLY|O_BINARY);
  if (fin == -1)
  {
    printf("error opening input %s\n", argv[1]);
    return -1;
  }
  fout = open(argv[2], O_WRONLY|O_CREAT|O_TRUNC|O_BINARY, S_IREAD|S_IWRITE);
  if (fout == -1)
  {
    printf("error opening output %s\n", argv[2]);
    return -1;
  }
  Size = filelength(fin);
  Size -= read(fin, &Sign, sizeof(Sign));
  assert(Sign == 0xDCDC);
  while (Size)
  {
     Size -= read(fin, &ToWrite, sizeof(ToWrite));
#if defined(M_OS2) || defined(M_DOS) || defined(M_W32)
     Swap2(&ToWrite);
#endif
     Size -= read(fin, &Read, sizeof(Read));
#if defined(M_OS2) || defined(M_DOS) || defined(M_W32)
     Swap2(&Read);
#endif
     if (Read)
     {
       Size -= read(fin, InBuff, Read);
       Length = RdcDecompress(InBuff, Read, OutBuff);
       assert(Length == ToWrite);
     }
     else
       Size -= read(fin, OutBuff, ToWrite);
     write(fout, OutBuff, ToWrite);
  }
  close(fin);
  close(fout);
  return 0;
}
