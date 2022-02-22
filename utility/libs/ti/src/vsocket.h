// file       : vsocket.h
// programmer : v.risi
// description: communications handler for sockets
// start date : 14th June, 1994

#ifndef _VSOCKET_H_
#define _VSOCKET_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_VSOCKET_H_);

#ifdef M_OS2
#define OS2
#define BSD_SELECT
extern "C" {
#undef ENOTEMPTY
#include <types.h>
#include <utils.h>
#include <arpa\nameser.h>
#ifndef ALLOW_IOCTL_PROTOTYPE_TO_DEFINED
#define ioctl define_ioctl_as_some_spurious_function
#endif
#include <sys\socket.h>
#ifndef ALLOW_IOCTL_PROTOTYPE_TO_DEFINED
#undef ioctl
#endif
#include <sys\select.h>
#include <sys\ioctl.h>
#include <netdb.h>
#include <netinet\in.h>
#define F16 __far16
#undef ENOTEMPTY
#define errno sock_errno()
}
#endif

#ifdef M_DOS
#include "winsock.h"
#define  F16 far
#define  DESIRED_WINSOCK_VERSION 0x0101
#define  MINIMUM_WINSOCK_VERSION 1
#define soclose closesocket
#define ioctl   ioctlsocket
#endif

#ifdef M_W32

   #ifdef __MSVC__
   
      #include <windows.h>
      #include <winsock2.h>
      
   #elif __BORLANDC__
   
      #include <windows.h>
   
      #if (__BCPLUSPLUS__ == 0x0530)
         #include <windows.h>
         #include <winsock2.h>
      #elif (__BCPLUSPLUS__ > 0x0530)
         #include <winsock2.h>
      #else
         #include <winsock.h>
      #endif
   
   #endif
   
   #define  DESIRED_WINSOCK_VERSION 0x0101
   #define  MINIMUM_WINSOCK_VERSION 1
   #define  soclose closesocket
   #define  ioctl   ioctlsocket
   
#endif

#ifdef M_NT
#include "winsock.h"
#define  F16
#define  DESIRED_WINSOCK_VERSION 0x0101
#define  MINIMUM_WINSOCK_VERSION 1
#define soclose closesocket
#define ioctl   ioctlsocket
#endif

#ifdef M_AIX
extern "C" {
#include "osfuncs.h"
#undef abs
#include <macros.h>
#include <pwd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <strings.h>
}
#define F16
#define soclose close
#define recv(a,b,c,d) read((a), (b), (c))
#define send(a,b,c,d) write((a), (b), (c))
#endif

#ifdef M_GNU
   #include <pwd.h>
   #include <sys/shm.h>
   #include <sys/sem.h>
   #include <sys/types.h>
   #include <sys/socket.h>
   #include <sys/select.h>
   #include <sys/ioctl.h>
   #include <netinet/in.h>
   #include <arpa/inet.h>
   #include <netdb.h>
   #include <errno.h>
   #define F16
   #define soclose close
   #define recv(a,b,c,d) read((a), (b), (c))
   #define send(a,b,c,d) write((a), (b), (c))
#endif


#define PROTOCOL "tcp"
#ifndef SOCKET_QSIZE
#define SOCKET_QSIZE 5
#endif

#include "tstring.h"
#include "tp2p.h"

CLASSDEF(Socket);
class tSocket : public tP2P
{
protected:
  tSocket() {}
  sockaddr_in Addr;   // Socket address
  int CommsPort;      // TCP socket Descriptor for active connection
  unsigned short Retries;
  long TimeOut;
  int ReadAll(char * Buff, long Size);
  bool Wait(unsigned long aMSec=0, bool onRead=true);
  long ReceiveBufferLength;
  long SendBufferLength;
public:
  tSocket(const char * aService,long aTimeOut=1000, unsigned short aRetries=5);
  virtual ~tSocket();
  virtual void Open() = 0;
  virtual void Close();
  virtual unsigned long ReadLength();
  virtual void WriteLength(unsigned long aLength);
  virtual void Read(void * aBuffer, unsigned long aBufferLen);
  virtual void Read(void * aBuffer, unsigned long aBufferLen, unsigned long& aRead);
  virtual int ReadOnce(char * Buff, long Size);
  virtual void Write(void * aBuffer, unsigned long aBufferLen);
  void IoCtl(int aOption, int aValue);
  void SetOpt(int aOption, int aValue);
  void SetOpt(int aOption, long aValue);
  void SetOpt(int aOption, linger aValue);
  int  GetCommsPort() {return CommsPort;}
  void PrintOn(std::ostream& os) {os << *this;}
  friend std::ostream& operator<< (std::ostream& os, const rSocket aSocket);
  void Linger();
  void KeepAlive();
  void Debug();
  void SetSendBufferLength(long aValue);
  void SetReceiveBufferLength(long aValue);
};

CLASSDEF(SockClient);

class tSockClient : public tSocket
{
public:
  tSockClient(const char * aHost, const char * aService,long aTimeOut=1000, unsigned short aRetries=5);
  virtual void Open();
};

CLASSDEF(SockServer);

class tSockServer : public tSocket
{
  int ListenPort;           // TCP socket Descriptor
public:
  tSockServer(const char * aService,long aTimeOut=1000, unsigned short aRetries=5);
  tSockServer(int aListenPort,long aTimeOut=1000, unsigned short aRetries=5);
  virtual ~tSockServer();
  virtual void Open();
  int GetListenPort() {return ListenPort;}
};

XCLASSDEF(VSocket);
class xVSocket : public xCept
{
private:
  int    No;
public:
  enum eError {errSockInit = 1,                 // 1
               errSockHost,                     // 2
               errSockService,                  // 3
               errSockSocket,                   // 4
               errSockBind,                     // 5
               errSockListen,                   // 6
               errSockConnect,                  // 7
               errSockClose,                    // 8
               errSockRead,                     // 9
               errSockWrite,                    // 10
               errSockLinger,                   // 11
               errSockKeepAlive,                // 12
               errSockTimeout,                  // 13
               errSockDebug,                    // 14
               errSockGetOpt,                   // 15
               errSockSetOpt,                   // 16
               errSockTooLate,                  // 17
               errSockReadOnce                  // 18
              };
  xVSocket(char * aFname, int aLine, eError aError, int aNo);
  xVSocket(const xVSocket& aX);
  int ErrNo();
};

inline int xVSocket::ErrNo()
{
   return No;
}

#define XVSocket(err, no) xVSocket(__FILE__, __LINE__, xVSocket::err, no)

#endif

