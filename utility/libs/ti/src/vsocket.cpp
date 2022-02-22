#include "versions.h"
#define _VERSION_NO "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
COMPILED_VERSION(_VERSION_NO);

// file       : vsocket.c
// programmer : v.risi
// description: communications handler for sockets
// start date : 14th June, 1994

#include "ti.h"

//#if defined (M_AIX) || defined (M_GNU)

#define ALLOW_IOCTL_PROTOTYPE_TO_DEFINED
#include "vsocket.h"
#include <string.h>

#ifdef M_STDLIB
using namespace std;
#endif

tSocket::tSocket(const char * aService, long aTimeOut, unsigned short aRetries)
: CommsPort(-1)
, Retries(aRetries)
, TimeOut(aTimeOut)
, ReceiveBufferLength(0)
, SendBufferLength(0)
{
#ifdef M_OS2
  int rc = sock_init();
  if (rc)
    throw XVSocket(errSockInit, errno); // rc out
#endif
#if defined(M_DOS) || defined(M_W32)
  WSADATA  wsadata;
  int rc = WSAStartup(DESIRED_WINSOCK_VERSION, &wsadata);
  if (rc)
    throw XVSocket(errSockInit, rc);
//  {
//    rc = WSAGetLastError();
//    return (rc);
//  }

  /* Check that the windows socket dll version is ok */
  if (wsadata.wVersion < MINIMUM_WINSOCK_VERSION)
    throw XVSocket(errSockInit, -1);
//  {
//    return (ERROR_CLISOC_BASE + OURVERINCOMPAT);
//  }

#endif
  servent F16* ps = getservbyname(aService, PROTOCOL);
  if (ps == 0)
    throw XVSocket(errSockService, 0);
  Addr.sin_port = ps->s_port;
  Addr.sin_addr.s_addr = INADDR_ANY;
  Addr.sin_family = AF_INET;
}

tSocket::~tSocket()
{
  if (CommsPort != -1)
    soclose(CommsPort);
}

void tSocket::Close()
{
  if (CommsPort != -1)
    soclose(CommsPort);
  CommsPort = -1;
}

int tSocket::ReadAll(char * Buff, long Size)
{
  int BytesRead=0, n, Try=0;

  while (BytesRead < Size)
  {
    if (TimeOut && Wait(TimeOut) == false)
    {
      if (Try++ < Retries)
        continue;
      else
        return -1;
    }
    n = recv(CommsPort, Buff, Size - BytesRead, 0);
    if (n < 0)
      continue;
    if (n == 0)
      return BytesRead;
    BytesRead += n;
    Buff += n;
  }
  return BytesRead;
}

int tSocket::ReadOnce(char * Buff, long Size)
{
  int n;

  n =  recv(CommsPort, Buff, Size, 0);
  if (n < 0) {
    throw XVSocket(errSockReadOnce, n);
  }
  return n;
}

bool tSocket::Wait(unsigned long aMSec, bool onRead)
{
  struct timeval TimeOut;
  struct timeval *TOP;
  if (aMSec)
  {
    TimeOut.tv_sec  = aMSec / 1000;
    TimeOut.tv_usec = (aMSec % 1000) * 1000;
    TOP = &TimeOut;
  }
  else
    TOP = 0;
  fd_set  OnSet;
  FD_ZERO(&OnSet);
  FD_SET(CommsPort, &OnSet);
  int rc = select(FD_SETSIZE, (onRead==true?&OnSet:0), (onRead==true?0:&OnSet), 0, TOP);
  if (rc == -1)
    throw XVSocket(errSockTimeout, errno);
  return FD_ISSET(CommsPort, &OnSet)?true:false;
}

void tSocket::IoCtl(int aOption, int aValue)
{
#if defined(M_DOS) || defined(M_W32)
  int rc = ioctl(CommsPort, aOption, (u_long FAR *)&aValue);
#else
  int rc = ioctl(CommsPort, aOption, (char F16 *)&aValue, sizeof(aValue));
#endif
  if (rc)
    throw XVSocket(errSockSetOpt, errno);
}

void tSocket::SetOpt(int aOption, int aValue)
{
  int rc = setsockopt(CommsPort, SOL_SOCKET, aOption,
                     (char F16 *)&aValue, sizeof(aValue));
  if (rc)
    throw XVSocket(errSockSetOpt, errno);
}

void tSocket::SetOpt(int aOption, long aValue)
{
  int rc = setsockopt(CommsPort, SOL_SOCKET, aOption,
                     (char F16 *)&aValue, sizeof(aValue));
  if (rc)
    throw XVSocket(errSockSetOpt, errno);
}

void tSocket::SetOpt(int aOption, linger aValue)
{
  int rc = setsockopt(CommsPort, SOL_SOCKET, aOption,
                     (char F16 *)&aValue, sizeof(aValue));
  if (rc)
    throw XVSocket(errSockSetOpt, errno);
}

unsigned long tSocket::ReadLength()
{
  unsigned long Size;
  int n = ReadAll((char *)&Size, sizeof(unsigned long));
  if (n != sizeof(unsigned long))
    throw XVSocket(errSockRead, 0);
  return ntohl(Size);
}

void tSocket::Read(void * aBuffer, unsigned long aBufferLen)
{
  int n = ReadAll((char *)aBuffer, aBufferLen);
  if (n != aBufferLen)
    throw XVSocket(errSockRead, 0);
}

void tSocket::Read(void * aBuffer, unsigned long aBufferLen, unsigned long& aRead)
{
  int n = ReadAll((char *)aBuffer, aBufferLen);
  if (n < 0)
    throw XVSocket(errSockRead, 0);
  aRead = n;
}

void tSocket::WriteLength(unsigned long aLength)
{
  aLength = htonl(aLength);
  int n = send(CommsPort, (char *)&aLength, sizeof(unsigned long), 0);
  if (n != sizeof(unsigned long))
    throw XVSocket(errSockWrite, 0);
}

void tSocket::Write(void * aBuffer, unsigned long aBufferLen)
{
  int n = send(CommsPort, (char *)aBuffer, aBufferLen, 0);
  if (n != aBufferLen)
    throw XVSocket(errSockWrite, 0);
}

void tSocket::Linger()
{
  linger Linger = {0,1};
  SetOpt(SO_LINGER, Linger);
}

void tSocket::KeepAlive()
{
  SetOpt(SO_KEEPALIVE, 1);
}

void tSocket::Debug()
{
  SetOpt(SO_DEBUG, 1);
}

void tSocket::SetSendBufferLength(long aValue)
{
  if (CommsPort == -1)
    SendBufferLength = aValue;
  else
    throw XVSocket(errSockTooLate, 0);
}

void tSocket::SetReceiveBufferLength(long aValue)
{
  if (CommsPort == -1)
    ReceiveBufferLength = aValue;
  else
    throw XVSocket(errSockTooLate, 0);
}


tSockClient::tSockClient(const char * aHost, const char * aService,long aTimeOut, unsigned short aRetries)
: tSocket(aService, aTimeOut, aRetries)
{
  hostent F16* ph = gethostbyname(aHost);
  if (ph == 0)
    throw XVSocket(errSockHost, 0);
  Addr.sin_family = ph->h_addrtype;
  memcpy(&Addr.sin_addr, ph->h_addr, ph->h_length);
}

void tSockClient::Open()
{
  CommsPort = socket(AF_INET, SOCK_STREAM, 0);
  if (CommsPort == -1)
    throw XVSocket(errSockSocket, 0);
  if (ReceiveBufferLength)
    SetOpt(SO_RCVBUF, ReceiveBufferLength);
  if (SendBufferLength)
    SetOpt(SO_SNDBUF, SendBufferLength);
  int rc = connect(CommsPort, (sockaddr*)&Addr, sizeof(Addr));
  if (rc)
    throw XVSocket(errSockConnect, errno); // rc out
}

tSockServer::tSockServer(const char * aService,long aTimeOut, unsigned short aRetries)
: tSocket(aService, aTimeOut, aRetries)
{
  ListenPort = socket(AF_INET, SOCK_STREAM, 0);
  if (ListenPort == -1)
    throw XVSocket(errSockSocket, 0);
  int rc;
  if ((rc=bind(ListenPort, (struct sockaddr*)&Addr, sizeof(Addr))) == -1)
    throw XVSocket(errSockBind, errno); // rc out
  if ((rc=listen(ListenPort, SOCKET_QSIZE)) == -1)
    throw XVSocket(errSockListen, errno); // rc out
}

tSockServer::tSockServer(int aListenPort,long aTimeOut, unsigned short aRetries)
: tSocket()
{
  ListenPort = aListenPort;
  Retries = aRetries;
  TimeOut = aTimeOut;
}

tSockServer::~tSockServer()
{
  if (ListenPort != -1)
    soclose(ListenPort);
}

void tSockServer::Open()
{
  CommsPort = accept(ListenPort, 0, 0);
  if (CommsPort == -1)
    throw XVSocket(errSockSocket, 0);
  if (ReceiveBufferLength)
    SetOpt(SO_RCVBUF, ReceiveBufferLength);
  if (SendBufferLength)
    SetOpt(SO_SNDBUF, SendBufferLength);
}

struct tLinger : public linger
{
  friend ostream& operator<< (ostream& os, tLinger ling)
  {
    os << ling.l_onoff << ":" << ling.l_linger;
    return os;
  }
};

template <class T>
inline void Output(ostream &os, int CommsPort, char* Name, int aOpt, T)
{
  T data;
#if defined(M_AIX) || defined(M_GNU)
  socklen_t len = sizeof(data);
#else
  int len = sizeof(data);
#endif
  int rc = getsockopt(CommsPort, SOL_SOCKET, aOpt, (char *)&data, &len);
  os << setw(20) << Name << " rc: " << rc << " data " << data << " len " << len << std::endl;
}
#define mOutput(a, b, c, d) Output(a, b, #c, c, d)

ostream& operator<<(ostream& os, const rSocket aSocket)
{
  tLinger linger;
  mOutput(os, aSocket.CommsPort, SO_BROADCAST,   int(0));
  mOutput(os, aSocket.CommsPort, SO_DEBUG,       int(0));
  mOutput(os, aSocket.CommsPort, SO_DONTROUTE,   int(0));
  mOutput(os, aSocket.CommsPort, SO_ERROR,       int(0));
  mOutput(os, aSocket.CommsPort, SO_KEEPALIVE,   int(0));
  mOutput(os, aSocket.CommsPort, SO_LINGER,      linger);
  mOutput(os, aSocket.CommsPort, SO_OOBINLINE,   int(0));
  mOutput(os, aSocket.CommsPort, SO_RCVBUF,      long(0));
  mOutput(os, aSocket.CommsPort, SO_RCVLOWAT,    int(0));
  mOutput(os, aSocket.CommsPort, SO_REUSEADDR,   int(0));
  mOutput(os, aSocket.CommsPort, SO_SNDBUF,      long(0));
  mOutput(os, aSocket.CommsPort, SO_SNDLOWAT,    int(0));
  mOutput(os, aSocket.CommsPort, SO_SNDTIMEO,    int(0));
  mOutput(os, aSocket.CommsPort, SO_TYPE,        int(0));
#if defined(M_AIX)
  mOutput(os, aSocket.CommsPort, SO_USELOOPBACK, int(0));
#endif
  return os;
}

#define xVSocketName "xVSocket"
xVSocket::xVSocket(const xVSocket& aX)
: xCept(aX)
{}

xVSocket::xVSocket(char * aFname, int aLine, eError aError, int aNo) :
  xCept(aFname, aLine, xVSocketName, aError), No(aNo)
{
  switch (aError)
  {/*
  case errSockInit:
    sprintf(errorMsg, "Socket initialisation error(%d)", aNo);
    break;
  case errSockHost:
    sprintf(errorMsg, "Socket get host by name error(%d)", aNo);
    break;
  case errSockService:
    sprintf(errorMsg, "Socket get service by name error(%d)", aNo);
    break;
  case errSockSocket:
    sprintf(errorMsg, "Socket socket error(%d)", aNo);
    break;
  case errSockBind:
    sprintf(errorMsg, "Socket bind error(%d)", aNo);
    break;
  case errSockListen:
    sprintf(errorMsg, "Socket listen error(%d)", aNo);
    break;
  case errSockConnect:
    sprintf(errorMsg, "Socket connect error(%d)", aNo);
    break;
  case errSockClose:
    sprintf(errorMsg, "Socket close error(%d)", aNo);
    break;
  case errSockRead:
    sprintf(errorMsg, "Socket read error(%d)", aNo);
    break;
  case errSockWrite:
    sprintf(errorMsg, "Socket write error(%d)", aNo);
    break;
  case errSockLinger:
    sprintf(errorMsg, "Socket setsockopt(Linger) error(%d)", aNo);
    break;
  case errSockKeepAlive:
    sprintf(errorMsg, "Socket setsockopt(KeepAlive) error(%d)", aNo);
    break;
  case errSockTimeout:
    sprintf(errorMsg, "Socket timeout error(%d)", aNo);
    break;
  case errSockDebug:
    sprintf(errorMsg, "Socket setsockopt(Debug) error(%d)", aNo);
    break;
  case errSockGetOpt:
    sprintf(errorMsg, "Socket getsockopt() error(%d)", aNo);
    break;
  case errSockSetOpt:
    sprintf(errorMsg, "Socket setsockopt() error(%d)", aNo);
    break;
  case errSockTooLate:
    sprintf(errorMsg, "Socket set lengths be%d()", aNo);
    break;
  case errSockReadOnce:
    sprintf(errorMsg, "ReadOne error (%d) Errno (%d)", aNo, errno);
    break;
    */
      case errSockInit:
          osErr << "Socket initialisation error(" << aNo << ")" << ends;
          break;
      case errSockHost:
          osErr << "Socket get host by name error(" << aNo << ")" << ends;
          break;
      case errSockService:
          osErr << "Socket get service by name error(" << aNo << ")" << ends;
          break;
      case errSockSocket:
          osErr << "Socket socket error(" << aNo << ")" << ends;
          break;
      case errSockBind:
          osErr << "Socket bind error(" << aNo << ")" << ends;
          break;
      case errSockListen:
          osErr << "Socket listen error(" << aNo << ")" << ends;
          break;
      case errSockConnect:
          osErr << "Socket connect error(" << aNo << ")" << ends;
          break;
      case errSockClose:
          osErr << "Socket close error(" << aNo << ")" << ends;
          break;
      case errSockRead:
          osErr << "Socket read error(" << aNo << ")" << ends;
          break;
      case errSockWrite:
          osErr << "Socket write error(" << aNo << ")" << ends;
          break;
      case errSockLinger:
          osErr << "Socket setsockopt(Linger) error(" << aNo << ")" << ends;
          break;
      case errSockKeepAlive:
          osErr << "Socket setsockopt(KeepAlive) error(" << aNo << ")" << ends;
          break;
      case errSockTimeout:
          osErr << "Socket timeout error(" << aNo << ")" << ends;
          break;
      case errSockDebug:
          osErr << "Socket setsockopt(Debug) error(" << aNo << ")" << ends;
          break;
      case errSockGetOpt:
          osErr << "Socket getsockopt() error(" << aNo << ")" << ends;
          break;
      case errSockSetOpt:
          osErr << "Socket setsockopt() error(" << aNo << ")" << ends;
          break;
      case errSockTooLate:
          osErr << "Socket set lengths before Open()" << ends;
          break;
      case errSockReadOnce:
          osErr << "ReadOne error (" << aNo << ") "
                << "Errno (" << errno << ")" << ends;
          break;
  }
};

//#endif
