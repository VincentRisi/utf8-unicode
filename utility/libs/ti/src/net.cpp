#include "versions.h"
#define _VERSION_NO "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
COMPILED_VERSION(_VERSION_NO);

#include "machine.h"
#include "tools.h"
#include "net.h"
#ifdef __WATCOMC__
#include <mem.h>
#else
#include <memory.h>
#endif

#if defined(M_AIX)
#include <netinet/in.h>

double htond(double x)
{
    return x;
}

double ntohd(double x)
{
    return x;
}

void safe_htond(double &x)
{
    ;
}

void safe_ntohd(double &x)
{
    ;
}

#else            // All intel platforms i.e. Windows, DOS & OS/2

/* Jonnie Gilmore, Thu 12-Jan-1995, 16:19:11
   In the Intel environment these functions seem to crash the 80x87
   if x is an illegal number, which is quite possible while it is in
   network byte order.
   I have replaced both these functions with macros in net.h.
   They are now defined identical to the Swap8() macro.  This will
   not use the 80x87 co-processor.

// 80x87 seems to crash if x is an illegal number in htond and ntohd.

#ifdef M_WIN_31
#include "winsock.h"
#endif

*/

#if defined(M_OS2)
extern "C" {
#include "utils.h"
}

double htond(double x)
{
    Swap8(&x);
    return x;
}

double ntohd(double x)
{
    Swap8(&x);
    return x;
}

void safe_htond(double &x)
{
    Swap8(&x);
}

void safe_ntohd(double &x)
{
    Swap8(&x);
}

#endif

#endif
