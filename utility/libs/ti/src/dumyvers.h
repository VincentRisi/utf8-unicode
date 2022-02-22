#ifndef DUMYVERS_H_INCL
#define DUMYVERS_H_INCL "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(DUMYVERS_H_INCL);

#ifdef M_AIX
   char * __FILE__Dummy (void) {
      return(__COMPILED_VERSION__);
   }

#endif


#endif



