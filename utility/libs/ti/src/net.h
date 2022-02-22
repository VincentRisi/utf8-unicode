#ifndef _NET_H_
#define _NET_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_NET_H_);

#if defined(M_AIX) || defined(M_OS2)
#ifdef __cplusplus
extern "C" {
#endif

// Funtions for converting doubles to and from network byte order
double htond(double x);
double ntohd(double x);

void safe_htond(double &x);
void safe_ntohd(double &x);

#ifdef __cplusplus
}
#endif
#else  /* M_AIX */   // All intel platforms i.e. Windows, DOS & OS/2

/* Jonnie Gilmore, Thu 12-Jan-1995, 16:19:11
   In the Intel environment these functions seem to crash the 80x87
   if x is an illegal number, which is quite possible while it is in
   network byte order.
   I have replaced both these functions with macros in net.h.
   They are now defined identical to the Swap8() macro.  This will
   not use the 80x87 co-processor.

#ifdef __cplusplus
extern "C" {
#endif
// Funtions for converting doubles to and from network byte order
double htond(double x);
double ntohd(double x);
#ifdef __cplusplus
}
#endif
*/

#include "tools.h"
#define htond(x) Swap8((x))
#define ntohd(x) Swap8((x))

#endif /* M_AIX */

#endif
