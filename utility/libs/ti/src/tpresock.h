#ifndef __TPRESOCK__
#define __TPRESOCK__ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(__TPRESOCK__);
#include "ti.h"
#include "ini.h"
#include <syslog.h>
#include "cs.h"
#include "tstring.h"
#include "tunixser.h"

class tPreSock
{
public:
  tPreSock(tString inifile );
  ~tPreSock() { closelog();};
  virtual void ServiceClient() = 0;
  void CreateChildren();

protected:
  tServer * SockSrv;
  tUnixServer * UniSrv;

private :
  tString   sockinit;      //  socket initialisation string
  tString   lockfile;      //  lockfile name
  tString   servname;      //  server name
  tString   defaultpath;   //  path to change to - pid /lock file on this path
  int       timeout;       //  timeout for reads/writes to socket
  int       nprocs;        //  no of preallocated processes to fork
  int       debugmode;     //  0 OFF != 0 ON
  int       messagesize;   //  max size of message for socket server
                           //  object

  // if debug mode is set to  > 0 nprocs will NOT be used
  // instead the server will run in single server mode
  // this is useful because we can't get DBX to attach to a
  // forked child

};

#endif

