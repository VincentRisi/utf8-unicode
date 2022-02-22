//FileName: clisock.h
//Description: windows client socket class
//SubSystem: infrastructure
//Author: P. Searle
//Date Created: June 1994

#ifndef _CLISOCKP_H_
#define _CLISOCKP_H_

#include "machine.h"
#include <windows.h>
#include "winsock.h"
#include <stdio.h>
#include <string.h>
#include "datablck.h"
#include <malloc.h>
#include <limits.h>


enum {
 OURMSGTOSHORT, OURVERINCOMPAT, OURDATAWRITE,
 OURDATAREAD,   OURMEMALLOC,    OURPERMISSION,
 OURSPACE,      OURINVALIDSOCK, OURFILENOTFOUND,
 OURSELECT, OURTIMEOUT
};

#define  DESIRED_WINSOCK_VERSION 0x0101
#define  MINIMUM_WINSOCK_VERSION 1
#define  LONG_TIMEOUT 0
#define  MAX_TRIES 5

//typedef int  far *          int*;
//typedef int32 far *         int*32;
//typedef unsigned int32 far *fpulong;
//typedef void far *          fpvoid;
//typedef char far *          char*;

//#ifndef _WIN32
//typedef int  huge *          hpint;
//typedef int32 huge *         hplong;
//typedef unsigned int32 huge *hpulong;
//typedef void huge *          hpvoid;
//typedef char huge *          hpchar;
//#endif

// Frees data allocated by a Read. Not a member function because memory may
// be used and freed outside the lifetime of the tCliSock object.
void SockFreeMsg(DATABLOCK msg);

class tCliSock
{
  int   last_error;
  int   SockType;
  pchar Host, Service, msg, response;
  long  MaxBlock;
  long  TimeOut;

// John - 12:04:10 - 29/09/1995
private:
  int   sock;
  pchar host, service;

  pchar   _sockerrToString(int rc);
  DATALEN _recvall(int session, DATABLOCK buf, DATALEN size);
  pchar   _vbstrrtrim(pchar s, int len);
  int     _openSocket(char* host, char* service, int* sock, int sock_type);
  int     _writeSocket(int sock, DATABLOCK msg, DATALEN cbmsg);
  int     _readSocket(int sock, DATABLOCK* msgback, DATALEN* bytestoread);
  int     _closeSocket(int sock);
  int     _rawWriteSocket(int sock, DATABLOCK msg, DATALEN cbmsg);

public:

  // will open the socket
  // socket type defaults to SOCK_STREAM
  // If a block is encountered with size > MaxBlock then the name of the
  // file is passed rather than the data itself.
  // John - 10:41:07 - 02/10/1995 - Removed old constructor.
  // tCliSock(pchar aHost, pchar aService, long MaxBlock=LONG_MAX);

  // Leslie - 08:57:11 - 29/09/1995
  // Added timeout to constructor which must be used on a read
  tCliSock(pchar aHost, pchar aService, long MaxBlock=LONG_MAX, long timeout=LONG_TIMEOUT);

  // default constructor - does nothing (why??)
  tCliSock() {}

  // destructor will close the socket if still open
  ~tCliSock();

  // some common 'constrcution' type code
  void Construct(pchar aHost, pchar aService);

  // Leslie - 09:03:46 - 29/09/1995
  // Wait method for timeout implementation
  short tCliSock::Wait();

  // returns last error
  int GetLastError();

  // opens the socket
  int Open(char* aHost, char* aService, int aSockType);

  // writes into the open socket
  int Write(DATABLOCK msg, DATALEN cbmsg);

  // writes into the open socket - no cs length written
  int WriteRaw(DATABLOCK msg, DATALEN cbmsg);

  // will read from open socket for 'bytestoread'
  int Read(DATABLOCK* msgback, DATALEN* bytestoread);

  // will read from open socket for a max of 'bytestoread' - no length as in cs
  long ReadRaw(DATABLOCK msgback, DATALEN bytestoread);

  // close the socket
  void Close();

  // write the data , read a reponse , close the socket
  int   SendMsg(DATABLOCK msg, DATALEN cbmsg, DATABLOCK response,
    DATALEN cbresponse);

  // get error text for specified error
  void  ErrorMsg(int error, char* errormsg, int cberrormsg);
};

#endif

