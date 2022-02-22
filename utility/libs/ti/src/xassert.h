#ifndef __XASSERT_H
#define __XASSERT_H "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(__XASSERT_H);

#ifdef NDEBUG
#define assert(exp) ((void)0)
#else
void myAssert(char *msg, char *fname, unsigned line);
#define assert(exp) ((exp) ? (void)0 : myAssert(#exp, __FILE__, __LINE__))
#endif

#endif

