#include "versions.h"
#define _VERSION_NO "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
COMPILED_VERSION(_VERSION_NO);

#include "ti.h"

#if defined (M_AIX) || defined (M_GNU)

#include "tpostsock.h"

void tPostSock::ServiceClientExit()
{
}

void tPostSock::ServiceServerExit()
{
//
// sample code for Parent Server
//
//int Status;
//
//if (debugmode)
//{
//  return;
//}
//
// for (int Count = 0; Count < nprocs; ) {
//    int pid = wait3(&Status,WNOHANG | WUNTRACED,(struct rusage *)0);
//
//    if (WIFEXITED(Status)) {
//       cout << pid << " has exited with exit status =" << WEXITSTATUS(Status) << endl;
//    }
//    else if (pid) {
//       cout << pid << " was killed by signal =" << WTERMSIG(Status) << endl;
//    }
//
//    if (pid > 0) {
//       Count++;
//       continue;
//    }
//    sleep(10);
// }
// ReturnCode = 0;
// cout << "Server Terminated After All Kids" << endl;
}


#endif

