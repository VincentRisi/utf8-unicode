#include "versions.h"
#define _VERSION_NO "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
COMPILED_VERSION(_VERSION_NO);

#include "machine.h"

#include <stdio.h>
#include <stdarg.h>
#include "trace.h"

int Trace(char *fname, char *format, ...)
{
  FILE *f = fopen(fname, _FILE_MODE);
  va_list argptr;
  int cnt;

  if (!f)
    return 0;

  va_start(argptr, format);
  cnt = vfprintf(f, format, argptr);
  va_end(argptr);
  fclose(f);

  return(cnt);
}

static int  Lines = 0;
static int  Flip  = 0;

int LogTrace(char *fname1, char *fname2, int maxlines, char *format, ...)
{
  FILE *f = fopen(Flip==0?fname1:fname2, Lines==0?"w":"a");
  if (Lines++ > maxlines)
  {
    Lines = 0;
    Flip = (Flip+1)%2;
  }
  va_list argptr;
  int cnt;
  if (!f)
    return 0;
  va_start(argptr, format);
  cnt = vfprintf(f, format, argptr);
  va_end(argptr);
  fclose(f);
  return(cnt);
}

/*  -------------------------------------------------------------- */
#ifdef TRACE_TST

int main(void)
{
  int  i = 100;
  long l = 10000l;

  printf("writing to file called JUNK\n");
  TRACE(("JUNK", "Testing: 100=%d, 10000=%ld\n", i, l));
  return 0;
}

#endif

