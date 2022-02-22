#ifndef _OBSOCKET_H_
#define _OBSOCKET_H_

#include "machine.h"

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
   #ifdef __cplusplus
   }
   #endif
#endif

#ifdef M_DOS
   #include "winsock.h"
   #define  F16 far
   #define  DESIRED_WINSOCK_VERSION 0x0101
   #define  MINIMUM_WINSOCK_VERSION 1
   #define  soclose closesocket
   #define  ioctl   ioctlsocket
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

#ifdef M_AIX
extern "C" {
#include <macros.h>
#include <pwd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <sys/tiuser.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
}
   #define soclose close
   #define recv(a,b,c,d) read((a), (b), (c))
   #define send(a,b,c,d) write((a), (b), (c))
#endif

#ifdef M_GNU
   //#include <macros.h>
   #include <pwd.h>
   #include <sys/shm.h>
   #include <sys/sem.h>
   #include <sys/types.h>
   #include <sys/socket.h>
   #include <sys/select.h>
   #include <sys/ioctl.h>
   //#include <sys/tiuser.h>
   #include <netinet/in.h>
   #include <arpa/inet.h>
   #include <netdb.h>
   #include <errno.h>
   #define soclose close
   #define recv(a,b,c,d) read((a), (b), (c))
   #define send(a,b,c,d) write((a), (b), (c))
#endif

#define PROTOCOL "tcp"
//#define SOCKET_QSIZE 5
#define SOCKET_QSIZE 1024

typedef struct
{
   int ListenPort;     // TCP socket Descriptor for Server
   int CommsPort;      // TCP socket Descriptor for active connection
   struct sockaddr_in Addr;   // Socket address
#ifdef WIN32
   int AddrLen;
#else
   socklen_t AddrLen;
#endif
   unsigned short Retries;     // For Wait
   int TimeOut;               // For Wait
   int ErrorNo;                // Error Code
} tSockCB;

extern "C" 
{
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

   int APPLAPI SockDuplicate(tSockCB   **SockCB,     
                             const int   aCommsPort, 
                             const int   aRetries,   
                             const int  aTimeOut); 
      
   int APPLAPI SockDupServer(tSockCB    **SockCB,     
                             const int    aCommsPort, 
                             const int    aRetries,   
                             const int    aTimeOut); 
   
   int APPLAPI SockServerInit(tSockCB    **SockCB,      
                              const char  *aService); 
   
   int APPLAPI SockServerOpen(tSockCB *SockCB);
   
   int APPLAPI SockWaitServer(const tSockCB        *SockCB, 
                              const unsigned int   aTimeOut, 
                              bool                 *aResult);
   
   int APPLAPI SockClientInit(tSockCB    **SockCB, 
                              const char  *aHost, 
                              const char  *aService);
   
   int APPLAPI SockClientOpen(tSockCB *SockCB);
   
   int APPLAPI SockDone(tSockCB *SockCB);
   
   int APPLAPI SockClose(tSockCB *SockCB);
   
   int APPLAPI SockReadLength(const tSockCB *SockCB, 
                              unsigned int *aSize);
   
   int APPLAPI SockRead(const tSockCB       *SockCB, 
                        void                *aBuffer, 
                        const unsigned int  aBufferLen);
   
   int APPLAPI SockStreamRead(const tSockCB       *SockCB, 
                              void                *aBuffer, 
                              const unsigned int  aBufferLen, 
                              unsigned int       *aSize);
   
   int APPLAPI SockWaitRead(const tSockCB        *SockCB, 
                            const unsigned int   aTimeOut, 
                            bool                 *aResult);
   
   int APPLAPI SockWriteLength(tSockCB             *SockCB, 
                               const unsigned int  aLength);
   
   int APPLAPI SockWrite(tSockCB       *SockCB, 
                         void          *aBuffer, 
                         unsigned int  aBufferLen);
   
   int APPLAPI SockWaitWrite(const tSockCB       *SockCB, 
                             const unsigned int  aTimeOut, 
                             bool                *aResult);
   
   int APPLAPI SockSetTimeOut(tSockCB    *SockCB, 
                              const int   aTimeOut);
   
   int APPLAPI SockSetRetries(tSockCB              *SockCB, 
                              const unsigned short  aRetries);
   
   int APPLAPI SockError(const tSockCB *SockCB);
   
   char* APPLAPI SockErrorText(const int rc);
   
   char* APPLAPI SockErrorMsg(const tSockCB *SockCB);
   
   int APPLAPI SockErrorMsgNet(const tSockCB *SockCB, 
                               char          *Buffer, 
                               int            BufferLen);

}

#endif



