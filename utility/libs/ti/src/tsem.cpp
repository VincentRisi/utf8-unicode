#include "versions.h"
#define _VERSION_NO "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
COMPILED_VERSION(_VERSION_NO);

// tsem.c
// Author S M Sena
// Date   20-May-94
//
// Description  :  Class to implement Aquire and Release semaphore primatives
//                 two state semaphore for unix no exceptions

#include <va_list.h>
#include <stdio.h>
#include "tsem.h"

tSemaphore::tSemaphore(const short aKey)
{
#ifdef M_AIX
  /* get or create a semaphore set of size 1 */

  if ((_Sid = semget((key_t) aKey, 1, 0666 | IPC_CREAT)) == -1)
  {
    perror("semget");
    exit (-1);
  }
#endif
}



void tSemaphore::Aquire()
{
#ifdef M_AIX
    if (semop(_Sid, (struct sembuf *)tSemaphore::_Aquire, 2) == -1)
    {
       perror("semop");
       exit(-1);
    }
#endif
}

void tSemaphore::Release()
{
#ifdef M_AIX
   if (semop(_Sid, (struct sembuf *)tSemaphore::_Release, 1) == -1)
   {
      perror("semop");
      exit(-1);
   }
#endif
}


#ifdef M_AIX
struct sembuf tSemaphore::_Aquire[] = {
                       0, 0, 0,        // Wait for semaphore == 0
                       0, 1, SEM_UNDO  // inc semaphore if crash undo
                    },
             tSemaphore::_Release[] = {
                      0, -1, SEM_UNDO  // dec semaphore
                    };
#endif
