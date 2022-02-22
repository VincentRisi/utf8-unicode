#include "versions.h"
#define _VERSION_NO "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
COMPILED_VERSION(_VERSION_NO);

#include "machine.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <memory.h>
#include <time.h>

#ifdef M_W32
#include <io.h>
#endif

#include <fcntl.h>
#include <sys/stat.h>

#include "tools.h"
#include "rdc.h"

static uchar InBuff[8192];
static uchar OutBuff[8192];
static uchar CtlBuff[8192];

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
  long Size, TotalCompressed=0, TotalUncompressed=0;
  clock_t Start;
  double CompressTime, DecompressTime, e;
  ushort Read, ToWrite, Length, Sign;
  int fin, fout, n;
  if (argc < 3)
  {
    printf("usage - rdctest infile outfile\n");
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
  Sign = 0xDCDC;
  write(fout, &Sign, sizeof(Sign));
  for (Size = filelength(fin); Size; Size -= Read)
  {
     Read = read(fin, InBuff, 8192);
     TotalUncompressed += Read;
     for (Start = clock(), n=0; n<500; n++)
       ToWrite = RdcCompress(InBuff, Read, OutBuff);
     e = double(clock()-Start) / CLOCKS_PER_SEC / n;
     printf("+");
     CompressTime += e;
     Length = Read;
     TotalCompressed += ToWrite;
#if defined(M_OS2) || defined(M_DOS) || defined(M_W32)
     Swap2(&Length);
#endif
     write(fout, &Length, sizeof(Length));
     Length = ToWrite;
#if defined(M_OS2) || defined(M_DOS) || defined(M_W32)
     Swap2(&Length);
#endif
     write(fout, &Length, sizeof(Length));
     write(fout, OutBuff, ToWrite ? ToWrite : Read);
     if (ToWrite)
     {
       for (Start = clock(), n=0; n<500; n++)
         Length = RdcDecompress(OutBuff, ToWrite, CtlBuff);
       e = double(clock()-Start) / CLOCKS_PER_SEC / n;
       printf("-");fflush(stdout);
       DecompressTime += e;
       assert(Length == Read);
       assert(memcmp(InBuff, CtlBuff, Read) == 0);
     }
  }
  close(fin);
  close(fout);
  printf("\nUnCompressed:%lu Compressed:%lu CompressTime:%0.6f(%0.6f) DecompressTime:%0.6f(%0.6f)\n"
                    , TotalUncompressed
                    , TotalCompressed
                    , CompressTime
                    , CompressTime / (TotalUncompressed / 1024.0)
                    , DecompressTime
                    , DecompressTime / (TotalUncompressed / 1024.0)
                    );
  return 0;
}
