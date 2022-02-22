#include "versions.h"
#define _VERSION_NO "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
COMPILED_VERSION(_VERSION_NO);

#include "ti.h"

#if defined (M_AIX) || defined(M_GNU)

#include "xcept.h"
#include "logfile.h"
#include "cbsocket.h"
#include "tpostsock.h"

using namespace std;

tPostSock::tPostSock( tString inifile )
{
  try
  {
    ReturnCode = 0;

    tINI  ini(inifile);
    ini.SetSection("Preallocated Server");
    ini.QueryValue("{Socket Initialisation}", sockinit);
    ini.QueryValue("{Socket Timeout}",timeout);
    ini.QueryValue("{Socket Max Message}",messagesize);
    ini.QueryValue("{Server LockFile Name}", lockfile);
    ini.QueryValue("{Server Default Path}", defaultpath);
    ini.QueryValue("{Number Server Procedures}", nprocs);
    ini.QueryValue("{Server Name}", servname);
    ini.QueryValue("{Debug}", debugmode);
    if ( debugmode != 0)
      std::cerr  << "DEBUG MODE IS ON" << std::endl << flush;
    // create the client server Server object with values from ini file
    SockSrv = new tCBSockServer(sockinit);
  }
  catch (xINI &x)
  {
    std::cerr << x.ErrorStr() << std::endl << flush;
  }
  catch (xCept &x)
  {
    std::cerr << x.ErrorStr() << std::endl << flush;
  }
}

void tPostSock::CreateChildren()
{
  int i, pid;
  char buf[20];
  try
  {
    if (debugmode != 0)
    {
      ServiceClient();
      return;
    }
    else
    {
      for (i=0; i < nprocs; i++)
      {
        if ((pid = fork()) == 0)             // 0 == child
        {
          ServiceClient();
          ServiceClientExit();
          return;
        }
        else if (pid < 0)
        {
          std::cerr << "Error on construction of child process " << i << std::endl << flush;
        }
      }
      ServiceServerExit();
    }
  }
  catch (xCept &x)
  {
	DisplayVerbose( x, std::cerr );
    //char errbuf[200];
    //sstream s(errbuf,sizeof(errbuf));
    SLOG->Log(x);
  }
}


//void tPostSock::ServiceClientExit()
//{
//}
//
//void tPostSock::ServiceServerExit()
//{
///*
//* sample code for Parent Server
//*
//int Status;
//
//   if (debugmode) {
//      return;
//   }
//
//   for (int Count = 0; Count < nprocs; ) {
//      int pid = wait3(&Status,WNOHANG | WUNTRACED,(struct rusage *)0);
//
//      if (WIFEXITED(Status)) {
//         std::cout << pid << " has exited with exit status =" << WEXITSTATUS(Status) << std::endl;
//      }
//      else if (pid) {
//         std::cout << pid << " was killed by signal =" << WTERMSIG(Status) << std::endl;
//      }
//
//      if (pid > 0) {
//         Count++;
//         continue;
//      }
//      sleep(10);
//   }
//   ReturnCode = 0;
//   std::cout << "Server Terminated After All Kids" << std::endl;
//*/
//}


#endif
