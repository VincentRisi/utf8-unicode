
#ifndef __TUNIXSERVER__
#define __TUNIXSERVER__ "$Revision: 1.1.1.1 $ $Date: 2005/05/30 14:21:48 $"
#include "versions.h"
HEADER_VERSION(__TUNIXSERVER__);

const char * const LOCKPATH = "/ti/spool/";
const char * const DIR_DEFAULT = "/tmp/";
typedef void ( HANDLER ) ( int );
void Reaper(int);        // waits on SIGCLD
void Terminate(int);     // catchs SIGHUP

class tUnixServer
{
private:
  char lockfile [255];
  int  fdpid;

public :
  enum eServer
  {
    serDemand,
    serPreAlloc,
    serSimple,
    serHybrid
  };

  // constructor
  tUnixServer (
    const char * filename,                 // lock or pid file name
    const char * aLockPath = LOCKPATH,     // path for lock/pid file
    eServer aServType = serPreAlloc);      // server type

  // desstructor
  virtual ~tUnixServer ();

  // method to write Process ID's to a file
  void WritePID(int aPID);
};

#endif

