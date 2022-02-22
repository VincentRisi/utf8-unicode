#ifndef _NXLCRPC_H_
#define _NXLCRPC_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_NXLCRPC_H_);
#ifdef _WINDOWS
#ifdef LIBMAIN
#define EXTERN /* */
#else
#define EXTERN extern
#endif
#ifdef Error
#undef Error
#endif
EXTERN short initialError;
EXTERN char debugString[256];
EXTERN short d_bug;
EXTERN long Error;
EXTERN char xl_Host_Socket[80];

#define VERSIONSEQNO 1

void OutputDebug(char *dbugString);

#endif
#endif
