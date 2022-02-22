#ifndef _OSFUNCS_H_
#define _OSFUNCS_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_OSFUNCS_H_);

#ifdef M_AIX
#include <unistd.h>
int usleep(unsigned int mu_secs);     /*  should be in unistd.h */
#define Delay(msec) usleep(msec*1000)
#endif

#endif

