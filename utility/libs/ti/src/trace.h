#ifndef _TRACE_H_
#define _TRACE_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_TRACE_H_);

#define _FILE_MODE "a"
int Trace(char *fname, char *format, ...);
int LogTrace(char *fname1, char *fname2, int maxlines, char *format, ...);

#ifdef DEBUG
/*  arglist is a list of arguments in brackets */
/*  eg. TRACE((fname, "%d%ld", i, l)); */
#define TRACE(arglist) Trace arglist
#else
#define TRACE(arglist)
#endif

#endif

