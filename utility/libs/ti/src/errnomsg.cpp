#include "versions.h"
#define _VERSION_NO "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
COMPILED_VERSION(_VERSION_NO);

#include "machine.h"

#include <errno.h>

#include "errnomsg.h"

//#if (__BCPLUSPLUS__ < 0x0530)
//extern int sys_nerr;
//extern char * sys_errlist[];
//#endif

#ifdef M_AIX
char* errnomsg(int error)
{
   if ( error < sys_nerr )
     return ( sys_errlist[error]);
   return 0;
}
#endif
