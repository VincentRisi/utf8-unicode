#include "versions.h"
#define _VERSION_NO "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
COMPILED_VERSION(_VERSION_NO);

#include "machine.h"

#include <stdio.h>
#include <stdarg.h>

#include "condprnt.h"

int CondPrintf(int cond, char *format, ...)
{
  int count=0;
  va_list argptr;

  if (cond)
  {
    va_start(argptr, format);
    count = vfprintf(stdout, format, argptr);
    fflush(stdout);
    va_end(argptr);
  }
  return count;
}
