#ifndef _TSEM_H_
#define _TSEM_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_TSEM_H_);

#include "machine.h"

#if defined(M_AIX) || defined(M_GNU)
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#endif

class tSemaphore {
private :
   static struct sembuf _Aquire[];
   static struct sembuf _Release[];
   char _SemName[13];                // filename for dos implementation
     std::ofstream _Sem;
   int   _Sid;

public :
     tSemaphore( const short aKey );
     void Aquire();
     void Release();
};

#endif
