#include "versions.h"
#define _VERSION_NO "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
COMPILED_VERSION(_VERSION_NO);

// 1.01.00 - added ignore exit and signals code - Vince Risi 9/1/96

#include "ti.h"
#include <signal.h>
#include "tpresock.h"

using namespace std;


tPreSock::tPreSock( tString inifile )
{

  try
  {
    tINI  ini(inifile);

    ini.SetSection("Preallocated Server");
    sockinit = ":socket::ntapsoc1";
    messagesize = 1024;
    timeout = 100;
    lockfile = "x.lock";
    nprocs = 1;
    debugmode = 0;
    servname = "UniServer";
    defaultpath = "/ti/spool/";
    ini.QueryValue("{Socket Initialisation}", sockinit );
    ini.QueryValue("{Socket Timeout}",timeout);
    ini.QueryValue("{Socket Max Message}",messagesize);
    ini.QueryValue("{Server LockFile Name}", lockfile );
    ini.QueryValue("{Server Default Path}", defaultpath );
    ini.QueryValue("{Number Server Procedures}", nprocs );
    ini.QueryValue("{Server Name}", servname );
    ini.QueryValue("{Debug}", debugmode );

    if ( debugmode != 0 )
       cerr  << "DEBUG MODE IS ON" << endl;

    // create the client server Server object with values from ini file
    SockSrv = new tServer ( sockinit, messagesize, timeout );

    // create unix server object for 'good' style server programming
    // concepts
    UniSrv = new tUnixServer (lockfile, defaultpath);
  }
  catch ( xClientServer &x ) { cerr << x.ErrorStr() << endl; }   // // John - 17:48:06 - 27/11/1995 { Display( cerr ); }
  catch ( xINI &x )          { cerr << x.ErrorStr() << endl; }   // // John - 17:48:06 - 27/11/1995 { Display( cerr ); }
}

extern "C" void OnSignal(int status)
{
  static int LastSignal = -1;
  static int count = 0;
  cout << "Signal " << status << endl << flush;
  if (status == LastSignal)
    count++;
  else
  {
    LastSignal = status;
    count = 0;
  }
  if (count < 10)
    signal(status, OnSignal);
}


extern "C" void AtExit()
{
  cout << "exit activated " << endl << flush;
}

void IgnoreSignals()
{
  cout << "Setting up Signal Handling" << endl << flush;
  atexit(AtExit);
  signal(SIGHUP     , OnSignal);
  signal(SIGINT     , OnSignal);
  signal(SIGQUIT    , OnSignal);
  signal(SIGILL     , OnSignal);
  signal(SIGTRAP    , OnSignal);
  signal(SIGIOT     , OnSignal);
  signal(SIGEMT     , OnSignal);
  signal(SIGFPE     , OnSignal);
  signal(SIGBUS     , OnSignal);
  signal(SIGSYS     , OnSignal);
  signal(SIGPIPE    , OnSignal);
  signal(SIGALRM    , OnSignal);
  signal(SIGTERM    , OnSignal);
  signal(SIGURG     , OnSignal);
  signal(SIGSTOP    , OnSignal);
  signal(SIGTSTP    , OnSignal);
  signal(SIGCONT    , OnSignal);
  signal(SIGCHLD    , OnSignal);
  signal(SIGTTIN    , OnSignal);
  signal(SIGTTOU    , OnSignal);
  signal(SIGIO      , OnSignal);
  signal(SIGXCPU    , OnSignal);
  signal(SIGXFSZ    , OnSignal);
  signal(SIGMSG     , OnSignal);
  signal(SIGWINCH   , OnSignal);
  signal(SIGPWR     , OnSignal);
  signal(SIGUSR1    , OnSignal);
  signal(SIGUSR2    , OnSignal);
  signal(SIGPROF    , OnSignal);
  signal(SIGDANGER  , OnSignal);
  signal(SIGVTALRM  , OnSignal);
  signal(SIGMIGRATE , OnSignal);
  signal(SIGPRE     , OnSignal);
  signal(SIGGRANT   , OnSignal);
  signal(SIGRETRACT , OnSignal);
  signal(SIGSOUND   , OnSignal);
  signal(SIGSAK     , OnSignal);
}

void tPreSock::CreateChildren()
{
  int i,
      pid;
  char buf[20];

  try
  {
    if ( debugmode != 0 )
    {
        IgnoreSignals();
        ServiceClient();
        return;
    }
    else
    {
      for ( i=0; i< nprocs ; i++ )
      {
        if ( (pid = fork()) == 0)             // 0 == Fork() means iam child
        {
          // call childs ServiceClient method
          IgnoreSignals();
          ServiceClient();
          // Client has terminated - O NO...
          return;
        }
        else if ( pid < 0 )
        {
        }
        else
        {
          // write pid to file using method x
/* cannot use UniSrv method as UniSrv object construct */
/* was commented out                                   */
/*                  UniSrv->WritePID(pid);            */
        }
      }
    }
  }
  catch ( xClientServer &x )
  {
    char errbuf[200];
    //ostrstream s(errbuf,sizeof(errbuf));
    // John - 17:48:56 - 27/11/1995
    // stder may be closed so writeit to log
    // x.Display(cerr);
    // x.Display(s);
    cerr << x.ErrorStr() << endl;
  }
}


