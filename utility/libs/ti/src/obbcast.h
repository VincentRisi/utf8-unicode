#ifndef _OBSCKET_H_
#define _OBSCKET_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_OBSCKET_H_);

#ifdef M_OS2
#define OS2
#define BSD_SELECT
#ifdef __cplusplus
extern "C" {
#endif
#undef ENOTEMPTY
#include <types.h>
#include <utils.h>
#include <arpa\nameser.h>
//#ifndef ALLOW_IOCTL_PROTOTYPE_TO_DEFINED
//#define ioctl define_ioctl_as_some_spurious_function
//#endif
#include <sys\socket.h>
//#ifndef ALLOW_IOCTL_PROTOTYPE_TO_DEFINED
//#undef ioctl
//#endif
#include <sys\select.h>
#include <sys\ioctl.h>
#include <netdb.h>
#include <netinet\in.h>
#define F16 __far16
#undef ENOTEMPTY
#ifdef __cplusplus
}
#endif
#endif

#ifdef M_DOS
#include "winsock.h"
#define  F16 far
#define  DESIRED_WINSOCK_VERSION 0x0101
#define  MINIMUM_WINSOCK_VERSION 1
#define soclose closesocket
#define ioctl   ioctlsocket
#endif

#ifdef M_NT
#include "winsock.h"
#define  F16
#define  DESIRED_WINSOCK_VERSION 0x0101
#define  MINIMUM_WINSOCK_VERSION 1
#define soclose closesocket
#define ioctl   ioctlsocket
#endif

#if defined(M_AIX) || defined(M_GNU)
extern "C" {
#include "osfuncs.h"
#undef abs
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
}
#define F16
#define soclose close
#endif

#define PROTOCOL "udp"
#define SOCKET_QSIZE 5

typedef struct
{
  int ListenPort;     // TCP socket Descriptor for Server
  int CommsPort;      // TCP socket Descriptor for active connection
  struct sockaddr_in Addr;   // Socket address
  unsigned short Retries;     // For Wait
  long TimeOut;       // For Wait
  int ErrorNo;        // Error Code
} tBcastCB, *pBcastCB;

#ifdef __cplusplus
extern "C" {
#endif

enum eError
{
  errSockInit = 1,
  errSockHost,
  errSockService,
  errSockSocket,
  errSockBind,
  errSockListen,
  errSockConnect,
  errSockClose,
  errSockRead,
  errSockWrite,
  errSockVersion,
  errSockIOCtl,
  errSockLinger,
  errSockKeepAlive,
  errSockTimeout,
  errSockDebug,
  errSockGetOpt,
  errSockSetOpt
};

int APPLAPI BcastServerInit(pBcastCB *BcastCB, char * aService);
int APPLAPI BcastClientInit(pBcastCB *BcastCB, char * aHost, char * aService);
int APPLAPI BcastDone(pBcastCB BcastCB);
int APPLAPI BcastClose(pBcastCB BcastCB);
int APPLAPI BcastDGramRead(pBcastCB BcastCB, void * aBuffer, unsigned long aBufferLen, unsigned long * aSize);
int APPLAPI BcastWrite(pBcastCB BcastCB, void * aBuffer, unsigned long aBufferLen);
int APPLAPI BcastError(pBcastCB BcastCB);
pchar APPLAPI BcastErrorMsg(pBcastCB BcastCB);


#ifdef __cplusplus
}
#endif

#endif


