#include "versions.h"
#define _VERSION_NO "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
COMPILED_VERSION(_VERSION_NO);

#include "machine.h"
#ifdef M_OS2
extern "C" {
#include <utils.h>
}
#include "tools.h"

unsigned long _System lswap(unsigned long ul)
{
  Swap4(&ul);return ul;
}

unsigned short _System bswap(unsigned short us)
{
  Swap2(&us);return us;
}
#endif
