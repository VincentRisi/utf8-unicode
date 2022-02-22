#include "versions.h"
#define _VERSION_NO "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
COMPILED_VERSION(_VERSION_NO);

// file       : tsocket.c
// programmer : v.risi
// description: communications handler for sockets

#include "machine.h"
#define ALLOW_IOCTL_PROTOTYPE_TO_DEFINED
#include "tsocket.h"
typedef struct
{
  sockaddr_in Addr;   // Socket address
  int CommsPort;      // TCP socket Descriptor for active connection
  ushort Retries;
  long TimeOut;
  long ReceiveBufferLength;
  long SendBufferLength;
} tSockCB, *pSockCB;

int SocketInit(pSockCB *aSockCB, pchar aService, long aTimeOut, ushort aRetries)
{
  pSockCB SockCB;
  servent F16* ps;
  int rc;
#ifdef M_OS2
  rc = sock_init();
  if (rc)
    return (rc);
#endif
#if defined(M_DOS) || defined(M_W32)
  WSADATA  wsadata;
  rc = WSAStartup(DESIRED_WINSOCK_VERSION, &wsadata);
  if (rc)
  {
    rc = WSAGetLastError();
    return (rc);
  }
  /* Check that the windows socket dll version is ok */
  if (wsadata.wVersion < MINIMUM_WINSOCK_VERSION)
    return ERROR_CLISOC_BASE + OURVERINCOMPAT;
#endif
  SockCB = *aSockCB = malloc(sizeof(tSockCB));
  SockCB->CommsPort = -1;
  SockCB->Retries = aRetries;
  SockCB->TimeOut = aTimeOut;
  SockCB->ReceiveBufferLength = 0;
  SockCB->SendBufferLength = 0;
  ps = getservbyname(aService, PROTOCOL);
  if (ps == 0)
    return -1; // throw XVSocket(errSockService, 0);
  SockCB->Addr.sin_port = ps->s_port;
  SockCB->Addr.sin_addr.s_addr = INADDR_ANY;
  SockCB->Addr.sin_family = AF_INET;
  return 0;
}

int SocketDone(pSockCB SockCB)
{
  if (SockCB->CommsPort != -1)
    soclose(SockCB->CommsPort);
  return 0;
}

int SocketClose(pSockCB SockCB)
{
  if (SockCB->CommsPort != -1)
    soclose(SockCB->CommsPort);
  SockCB->CommsPort = -1;
  return 0;
}

static int Wait(pSockCB SockCB, ulong aMSec, bool onRead, pbool aResult)
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
  fd_set OnSet;
  FD_ZERO(&OnSet);
  FD_SET(SockCB->CommsPort, &OnSet);
  int rc = select(FD_SETSIZE, (onRead==true?&OnSet:0), (onRead==true?0:&OnSet), 0, TOP);
  if (rc == -1)
    return -1; // throw XVSocket(errSockTimeout, errno);
  *aResult = FD_ISSET(SockCB->CommsPort, &OnSet)?true:false;
  return 0;
}

static int ReadAll(pSockCB SockCB, pchar Buff, long Size)
{
  int BytesRead=0, n, Try=0;
  bool Result;

  while (BytesRead < Size)
  {
    if (SockCB->TimeOut)
    {
      if (Wait(SockCB->TimeOut, 0, true, &Result)==0)
      {
        if (Result == false)
        {
          if (Try++ < SockCB->Retries)
            continue;
          else
             return -1;
        }
      }
      else
      {
        return -1;
      }
    }
    n = recv(SockCB->CommsPort, Buff, Size - BytesRead, 0);
    if (n < 0)
      continue;
    if (n == 0)
      return BytesRead;
    BytesRead += n;
    Buff += n;
  }
  return BytesRead;
}

//void tSocket::IoCtl(int aOption, int aValue)
//{
//#ifdef M_DOS
//  int rc = ioctl(CommsPort, aOption, (u_long FAR *)&aValue);
//#else
//  int rc = ioctl(CommsPort, aOption, (char F16 *)&aValue, sizeof(aValue));
//#endif
//  if (rc)
//    throw XVSocket(errSockSetOpt, errno);
//}

//void tSocket::SetOpt(int aOption, int aValue)
//{
//  int rc = setsockopt(CommsPort, SOL_SOCKET, aOption,
//                     (char F16 *)&aValue, sizeof(aValue));
//  if (rc)
//    throw XVSocket(errSockSetOpt, errno);
//}

//void tSocket::SetOpt(int aOption, long aValue)
//{
//  int rc = setsockopt(CommsPort, SOL_SOCKET, aOption,
//                     (char F16 *)&aValue, sizeof(aValue));
//  if (rc)
//    throw XVSocket(errSockSetOpt, errno);
//}

//void tSocket::SetOpt(int aOption, linger aValue)
//{
//  int rc = setsockopt(CommsPort, SOL_SOCKET, aOption,
//                     (char F16 *)&aValue, sizeof(aValue));
//  if (rc)
//    throw XVSocket(errSockSetOpt, errno);
//}

int SocketReadLength(pSockCB SockCB, pulong aSize)
{
  ulong Size;
  int n = ReadAll(SockCB, (pchar)&Size, sizeof(ulong));
  if (n != sizeof(ulong))
    return -1; // throw XVSocket(errSockRead, 0);
  *aSize = ntohl(Size);
  return 0;
}

int SocketRead(pSockCB SockCB, pvoid aBuffer, ulong aBufferLen)
{
  int n = ReadAll(SockCB, (pchar)aBuffer, aBufferLen);
  if (n != aBufferLen)
    return -1; // throw XVSocket(errSockRead, 0);
  return 0;
}

//void tSocket::Read(pvoid aBuffer, ulong aBufferLen, ulong& aRead)
//{
//  int n = ReadAll((pchar)aBuffer, aBufferLen);
//  if (n < 0)
//    throw XVSocket(errSockRead, 0);
//  aRead = n;
//}

void tSocket::WriteLength(ulong aLength)
{
  aLength = htonl(aLength);
  int n = send(CommsPort, (pchar)&aLength, sizeof(ulong), 0);
  if (n != sizeof(ulong))
    throw XVSocket(errSockWrite, 0);
}

void tSocket::Write(pvoid aBuffer, ulong aBufferLen)
{
  int n = send(CommsPort, (pchar)aBuffer, aBufferLen, 0);
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


tSockClient::tSockClient(const pchar aHost, const pchar aService,long aTimeOut, ushort aRetries)
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
    throw XVSocket(errSockConnect, rc);
}

tSockServer::tSockServer(const pchar aService,long aTimeOut, ushort aRetries)
: tSocket(aService, aTimeOut, aRetries)
{
  ListenPort = socket(AF_INET, SOCK_STREAM, 0);
  if (ListenPort == -1)
    throw XVSocket(errSockSocket, 0);
  int rc;
  if ((rc=bind(ListenPort, (struct sockaddr*)&Addr, sizeof(Addr))) == -1)
    throw XVSocket(errSockBind, rc);
  if ((rc=listen(ListenPort, SOCKET_QSIZE)) == -1)
    throw XVSocket(errSockListen, rc);
}

tSockServer::tSockServer(int aListenPort,long aTimeOut, ushort aRetries)
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
  int len = sizeof(data);
  int rc = getsockopt(CommsPort, SOL_SOCKET, aOpt, (char *)&data, &len);
  os << setw(20) << Name << " rc: " << rc << " data " << data << " len " << len << endl;
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
  mOutput(os, aSocket.CommsPort, SO_USELOOPBACK, int(0));
  return os;
}

#define xVSocketName "xVSocket"
xVSocket::xVSocket(const xVSocket& aX)
: xCept(aX)
{}

xVSocket::xVSocket(pchar aFname, int aLine, eError aError, int aNo) :
  xCept(aFname, aLine, xVSocketName, aError)
{
  switch (aError)
  {
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


