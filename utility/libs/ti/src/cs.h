#ifndef _CLISERV_H_
#define _CLISERV_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_CLISERV_H_);

#include "machine.h"
#include "xcept.h"
#include "ti.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#undef abs
#include <macros.h>
#include <pwd.h>
#include <errno.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "xstring.h"
#include "xdir.h"
#include "errnomsg.h"
#include <netinet/in.h>

// constructors -------------------------------------------------------
// for sockets Name is of the format ":socket:<host-name>:<service>"
//   where <service> must exist in the /etc/services file (unix)
//   and <host-name> must exist in the /etc/hosts file (unix)
// --------------------------------------------------------------------

//#define INFINITY -1

/* Types used by RPC class (huge definitions in clisock.h) */
typedef char*      DATABLOCK;
typedef int32       DATALEN;

// ------------------- Buffer structure for the client server API
typedef struct tBuffer
{
  uint32 BytesToRead;
  uint32 BytesRead;
  uint32 BytesToWrite;
  uint32 BytesWritten;
  char* p;
} tBuffer, *PBuffer, *pBuffer, Buffer;

// --------------------- errors
typedef enum { errtypSocket, errtypOther };

// other errors: values of Error when ErrType is errtypOther
typedef enum {
  ERR_GENERAL = -1,
  ERR_MALLOC = -2,
  ERR_TIMEOUT = -3,
  ERR_WRONGSERVER = -4,
  ERR_NOSUCHSERVICE = -5,
  ERR_BADNAME = -6,
  ERR_NOHOST = -7,
  ERR_HOSTTRYAGAIN = -8,
  ERR_HOSTNORECOVERY = -9,
  ERR_HOSTNOADDRESS = -10,
  ERR_IGNORE = -11,
  ERR_DEFAULTREACHED = -12
};

// ---------------------------------------------
class xClientServer: public xCept
{
private:
  int unixErrorNo;       // error number from system call
public:
  xClientServer(char* aFile, const int aLine,
    int aErrtyp, int aErrno, char* aInfo = 0);
};

#define XClientServer(errtyp, errno, mtext) \
  xClientServer(__FILE__,__LINE__, errtyp, errno,mtext)

// ---------------------------------------------
#define MAXPARTS 3      // maximum no. of parts in name passed to Init
class tClientServer
{
public:
  tClientServer(const char* name, int32  size, int32 timeout);
  virtual ~tClientServer();
  virtual void Open() = 0;             // pure virtual
  virtual void Close();
  virtual void Write(PBuffer);
  virtual void Read(PBuffer);
  void Transact(PBuffer bufOut, PBuffer bufIn); // WR
  void Call(PBuffer bufOut, PBuffer bufIn);     // OWRC

protected:
  char*       names[MAXPARTS];
  int32        Size;
  int32        TimeOut;              // msecs
  int         sockno;               // TCP socket
  sockaddr_in sin;                  // socket address
  int         sd;                   // TCP connection

  virtual void Init(char*, char*) = 0; // pure virtual
  bool Wait();
  // short SockReadAll(int session, pchar buff, int32 size);
  // Changed to read large packets - Geoff Bruce 27JUL95
  int32 SockReadAll(int session, pchar buff, int32 size);

private:
  int ParseName(char*, char, char* []);
};

// ---------------------------------------------
class tClient: public tClientServer
{
public:
  tClient(char* name, int32 size, int32 timeout);
  virtual ~tClient();
  virtual void Open();
  virtual void Close()          { tClientServer::Close();  }
  virtual void Write(PBuffer p) { tClientServer::Write(p); }
  virtual void Read(PBuffer  p) { tClientServer::Read(p);  }

protected:
  virtual void Init(char* host, char* service);
};

// ---------------------------------------------
class tServer: public tClientServer
{
public:
  tServer(char* name, int32 size, int32 timeout);
  virtual ~tServer();
  virtual void Open();
  virtual void Close()          { tClientServer::Close();  }
  virtual void Write(PBuffer p) { tClientServer::Write(p); }
  virtual void Read(PBuffer  p) { tClientServer::Read(p);  }

protected:
  virtual void Init(char* host, char* service);
};

#endif

