//FileName: sfapp.h
//Description: application class for store and forward sfapp
//SubSystem: infrastructure
//Author: P. Searle
//Date Created: August 1994

#ifndef _SFAPP_H_
#define _SFAPP_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_SFAPP_H_);

#include "sfs.h"
#include "tpresock.h"
#include "sfcommon.h"

// maximum message size
const int MAX_MSG = 10240;

typedef struct tSfAppData {
  tSfMessage common;
  char p[MAX_MSG + 16];
} tSfAppData;

class tsfApp : public tPreSock
{
private:

  enum sfError { sfOK, sferrQDATA };
  tString pqpath;    // persistent queue path for store and forward init
  int qapplkeys;     // number of application keys for store and forward init
  int qelementsize;  // q size for store and forward init
  tSF * sf;          // store and forward object of class tSF
  tSfAppData data;   // strcuture to receive messages on the socket
  tBuffer buffer;    // client server buffer
  tSfQuery * pqry;   // pointer to a query structure to respond with after query on queue

  void ProcessRx(tSF & sf);        // Receive unsolicited message on socket - queue RX
  void ProcessQQry(tSF & sf);      // Query on a queue - msg on socket
  void ProcessTx(tSF & sf);        // Queue data for TX - msg on socket
  void LoadIni(tString inifile);   // loads config settings using tINI

  void Construct(tString inifile); // code for constrcution of sf application
                                   // called from tsfApp constrcutor

public:

  void ServiceClient();            // implementation of pure virtual function
                                   // inherited from tPreSock

  tsfApp( tString inifile );       // construction starts all
                                   // server processes and does all
                                   // server initialisation

  ~tsfApp(  ) { delete sf; }       // destuctor
};

#endif
