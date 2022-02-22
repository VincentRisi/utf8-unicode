#include "versions.h"
#define _VERSION_NO "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
COMPILED_VERSION(_VERSION_NO);

// Filename      : parent.c
// Description   : AIX Multiple socket server parent spawn class
// Author        : Suri Sena, modified Vince Risi
 

#include "ti.h"

#if defined (M_AIX) || defined(M_GNU)

#include <stddef.h>
#include <unistd.h>

#include "ini.h"
#include "tstring.h"
#include "vsocket.h"
#include "parent.h"

#define xParentName "xParent"

#ifdef M_STDLIB
using namespace std;
#endif

xParent::xParent(const xParent& aX)
: xCept(aX)
{}

xParent::xParent(char * aFname, int aLine, eError aError) :
  xCept(aFname, aLine, xParentName, aError)
{
  switch (aError)
  {
      /*
  case errParentExec:
    sprintf(errorMsg, "Failed to execute");
    break;
  case errParentFork:
    sprintf(errorMsg, "Failed to fork");
    break;
      */
      case errParentExec:
          osErr << "Failed to execute" << ends;
          break;
      case errParentFork:
          osErr << "Failed to fork" << ends;
          break;
  }
};

tParent::tParent(char *aIniFile)
{
  try
  {
    tINI ini(aIniFile);
    ini.QueryValue("{Server}",   Server,   "Socket Server");
    ini.QueryValue("{Service}",  Service,  "Socket Server");
    ini.QueryValue("{NoProcs}",  NoProcs,  "Socket Server");
  }
  catch(xINI & x)
  {
    std::cout << x.ErrorStr() << std::endl;
    // John - 17:45:29 - 27/11/1995
    // x.Display(cerr);
  };
}

tParent::tParent(tString aServer,
                 tString aService,
                 int     aNoProcs)
: Server(aServer)
, Service(aService)
, NoProcs(aNoProcs)
{}

void tParent::Run(char *aIniFile)
{
  try
  {
    SockServer = new tSockServer(Service);
#if defined(M_STDLIB)
	stringstream arg1, arg2;
#else
    strstream arg1, arg2;
#endif
    arg1 << SockServer->GetListenPort() << ends;
    arg2 << aIniFile << ends;
    for (int i = 0; i < NoProcs; i++)
    {
      int pid = fork();
      if (pid < 0)
        throw XParent(errParentFork);
      else if (pid == 0)  // on child
#if defined(M_STDLIB)
        if (execlp(Server, Server, arg1.str().c_str(), arg2.str().c_str(), 0) < 0)
#else	  
        if (execlp(Server, Server, arg1.str(), arg2.str(), 0) < 0)
#endif		
          throw XParent(errParentExec);
    }
    delete SockServer;                  // we have finished so tidy up
  }
  catch(xVSocket &x)
  {
    std::cerr << x.ErrorStr() << std::endl;
    // John - 17:45:47 - 27/11/1995
    // x.Display(cerr);
  }
}

#endif
