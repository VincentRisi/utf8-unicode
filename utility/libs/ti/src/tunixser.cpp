#include "versions.h"
#define _VERSION_NO "$Revision: 1.1.1.1 $ $Date: 2005/05/30 14:21:48 $"
COMPILED_VERSION(_VERSION_NO);

#include "ti.h"
#include "tunixser.h"

#include <fcntl.h>
//#include <unistd.h>
//#include <stdlib.h>
//#include <string.h>
//#include <stdio.h>
//#include <errno.h>
//#include <signal.h>
//#include <sys/types.h>
//#include <sys/ioctl.h>
//#ifdef M_STDLIB
//	#include <iostream>
//#else
//	#include <iostream.h>
//#endif
//#include <syslog.h>

extern "C" wait3 ( int *, int, struct rusage * );
#define WNOHANG  0x1

using namespace std;

// static tLogstream uniLog("UNISRV", &local); -- local ??
// changed barry 31/3/95
static tLogstream uniLog("UNISRV");
#define DBG debug << __FILE__ << '(' << __LINE__ << ')' << ' '



tUnixServer::tUnixServer (const char * filename, const char * aLockPath,
  eServer aServType)
{
  int i, fd, pid;

  // must be removed when we fix this class to work with ORACLE
  return;
/*
Since there is a return here and i am hassling to compile the code for wallstreet
I will comment out the unused code
  
  // create lock file
  // not treated as a lock file - just recreate if exists
  // parent always responsible for writing PID's
  // will contain PID's of all the preallocated server processes

  strcat ( lockfile, aLockPath );
  strcat ( lockfile, filename );

  if ((fdpid = open(lockfile, O_RDWR | O_CREAT| O_TRUNC, 0640)) < 0)
  {
    uniLog << DBG << "Server : unable to create file for PIDS" << endl;
    exit(1);
  }

  // fork to force server to operate in the background
  // this is for simple and demand servers
  if ( ( aServType == serDemand) || ( aServType == serSimple ) )
  {
    if ((pid = fork()) != 0)
    {
       if (pid < 0)
       {
         uniLog << alert << "Failed to fork server" << endl;
       }
       exit(0);
    }
  }

  // adjust servers group
  pid = getpid();
  (void) setpgrp ( );

  // set mask to ensure that all files created have
  // safe permissions
  (void) umask(027);

  // for demand and hybrid servers only
  // catch death of child with the fn Reaper()
  if ( ( aServType == serDemand) || ( aServType == serHybrid ) )
    ( void ) signal ( SIGCLD, Reaper );

  // this is the standard signal to use to
  // close the servers - ignore all other signals
  for( i = 1; i < SIGMAX; i++)
  {
    (void ) signal ( i, SIG_IGN ); // ignore signals of this number
  }
  (void ) signal ( SIGTERM, Terminate );

  // change path to a well known directory
  // so files are not created all over the place
  if (chdir(LOCKPATH))
  {
    (void) chdir(DIR_DEFAULT);
  }
*/
}

inline tUnixServer::~tUnixServer ()
{
  close(fdpid);
}

void tUnixServer::WritePID(int aPID)
{
  char pbuf[10];

  (void) sprintf(pbuf, "%6d\n", aPID );     // write pid to lockfile
  (void) write(fdpid, pbuf, strlen(pbuf));
}

// can't be members of the class because it conflicts with signal
void Reaper(int)
{
  int  status;

  while  ( wait3 ( &status, WNOHANG, ( struct rusage *)0 ) >= 0 ); //EMPTY

  ( void ) signal(SIGCLD,Reaper);
}


void Terminate(int)
{
  exit(0);
}


/*
  ********** BITS NOT IMPLEMENTED YET***************

  // detatch tty to ensure that signals from a user's
  // terminal do not affect the server running in the background
  // the server must detach itself from its controlling terminal
  if ( (fd = open ("/dev/tty",O_RDWR)) < 0 )
     perror ("Failed to open /dev/tty");
  if(  ioctl ( fd , TIOCNOTTY ) < 0 )
     perror ("Failed to detach from tty");
  close (fd);

  // close all open descriptors
  for ( i = getdtablesize()-1; i >=0; i-- )
     (void) close(i);

*/

