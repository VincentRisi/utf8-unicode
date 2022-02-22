#include "versions.h"
#define _VERSION_NO "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
COMPILED_VERSION(_VERSION_NO);

#include "machine.h"

#ifdef M_AIX
#include <unistd.h>
       #include <sys/time.h>
       #include <sys/types.h>

extern "C" {
#include </usr/include/sys/timers.h>
}
double Millisecs(void)
{
  //struct itimerstruc_t Tp;
  struct itimerspec Tp;
  Tp.it_value.tv_sec = 0;
  Tp.it_value.tv_nsec = 1000;
  gettimer(TIMEOFDAY, &Tp);
  return (double)Tp.it_interval.tv_sec*1000.0
       + (double)Tp.it_interval.tv_nsec/1000000.0;
}
#endif

#if defined(M_OS2) || defined(M_W32) || defined(M_DOS)
#include <time.h>
double Millisecs(void)
{
  return ((double) clock() * 1000.0) / (double) CLOCKS_PER_SEC;
}
#endif

