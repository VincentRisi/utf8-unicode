#include "versions.h"
#define _VERSION_NO "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
COMPILED_VERSION(_VERSION_NO);

// S M Sena
// Wed May 4 15:33:03 WET 1994
// timer.c
//

#include <va_list.h>
#include <stdio.h>
extern "C"
{
#include <unistd.h>
#include <signal.h>
#include <errno.h>
}

#include <math.h>
#include "timer.h"
#include "ti.h"

tAlarm::~tAlarm()
{
  alarm(0);
}

void tAlarm::Set(const int msecs)
{
  if (signal(SIGALRM, nullfcn) == BADSIG)
    fprintf(stderr, "signal BADSIG");
  else
    alarm((int)ceil((double)msecs / 1000)); // convert to seconds
}

void tAlarm::Clear()
{
  alarm(0);
}

void tAlarm::nullfcn(int)
{
}


