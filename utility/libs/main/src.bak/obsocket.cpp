// file       : obsocket.c
// programmer : v.risi
// description: communications handler for sockets
//              that is object based 'C' code.

#include <stdlib.h>

#include "machine.h"
#include "obsocket.h"

#if defined(M_AIX) || defined(M_GNU)

   #include <strings.h>
#else
   #ifdef _MSC_VER
      #include <malloc.h>
   #else
      #include <alloc.h>
      #include <mem.h>
   #endif

#endif


static tSockCB *_SockInit(const char *aHost, const char *aService, int *rc);
static int _Wait(int aPort, unsigned int aMSec, bool onRead, bool *aResult);
static int _ReadAll(const tSockCB *SockCB, char *Buff, const int Size);
static void _SetError(tSockCB *SockCB);

static tSockCB ABNSockCB;

int APPLAPI SockDuplicate(tSockCB    **SockCB,
                          const int    aCommsPort,
                          const int    aRetries,
                          const int   aTimeOut)
{
   *SockCB = (tSockCB *) malloc(sizeof(tSockCB));

   if (*SockCB)
   {
      (*SockCB)->ListenPort = -1;
      (*SockCB)->CommsPort  = aCommsPort;
      (*SockCB)->Retries    = (unsigned short)aRetries;
      (*SockCB)->TimeOut    = aTimeOut;
      return 0;
   }

   return errSockInit;
}

int APPLAPI SockDupServer(tSockCB     **SockCB,
                          const int     aListenPort,
                          const int     aRetries,
                          const int    aTimeOut)
{
   *SockCB = (tSockCB *) malloc(sizeof(tSockCB));

   if (*SockCB)
   {
      (*SockCB)->ListenPort = aListenPort;
      (*SockCB)->CommsPort  = -1;
      (*SockCB)->Retries    = (unsigned short)aRetries;
      (*SockCB)->TimeOut    = aTimeOut;
      return 0;
   }
   return errSockInit;
}

int APPLAPI SockServerInit(tSockCB **SockCB, const char *aService)
{
   int rc;

   *SockCB = _SockInit(0, aService, &rc);

   return rc;
}

int APPLAPI SockServerOpen(tSockCB *SockCB)
{
   if (SockCB == &ABNSockCB)
      return errSockInit;

   SockCB->AddrLen   = sizeof(SockCB->Addr);
   SockCB->CommsPort = accept(SockCB->ListenPort, (sockaddr*)&SockCB->Addr, &SockCB->AddrLen);

   if (SockCB->CommsPort == -1)
   {
      _SetError(SockCB);
      return errSockSocket;
   }

#if defined(TCP_NODELAY)

   int on = 1;

   setsockopt(SockCB->CommsPort, SOL_SOCKET, TCP_NODELAY,(char*)&on, sizeof(on));

#endif

   return 0;
}

int APPLAPI SockClientInit(tSockCB **SockCB, const char *aHost, const char *aService)
{
   int rc;

   *SockCB = _SockInit(aHost, aService, &rc);

   return rc;
}

static void _shutdown_close(int &port)
{
#if defined(SD_BOTH)
  shutdown(port, SD_BOTH); // Vince adding this to see if WAITS reduce faster
#elif defined(SHUT_RDWR)
  shutdown(port, SHUT_RDWR); // Vince adding this to see if WAITS reduce faster
#endif
  soclose(port);
  port = -1;
}

static void _ForceClose(tSockCB *SockCB)
{
   if (SockCB->CommsPort != -1)
      _shutdown_close(SockCB->CommsPort);
}

int APPLAPI SockClientOpen(tSockCB *SockCB)
{
   int rc;

   if (SockCB == &ABNSockCB)
      return errSockInit;

   SockCB->CommsPort = socket(AF_INET, SOCK_STREAM, 0);

   if (SockCB->CommsPort == -1)
   {
      _SetError(SockCB);
      return errSockSocket;
   }

   rc = connect(SockCB->CommsPort, (sockaddr*)&SockCB->Addr, sizeof(SockCB->Addr));

   if (rc)
   {
      _SetError(SockCB);
      _ForceClose(SockCB);
      return errSockConnect;
   }

#if defined(TCP_NODELAY)
   int dealy_on = 1;

   setsockopt(SockCB->CommsPort, SOL_SOCKET, TCP_NODELAY,(char*)&dealy_on, sizeof(dealy_on));
#endif

#if defined(SO_USELOOPBACK)
   int loop_on = 1;

   setsockopt(SockCB->CommsPort, SOL_SOCKET, SO_USELOOPBACK, (char*)&loop_on, sizeof(loop_on));
#endif

   return 0;
}

int APPLAPI SockSetTimeOut(tSockCB *SockCB, const int aTimeOut)
{
   if (SockCB == &ABNSockCB)
      return errSockInit;

   SockCB->TimeOut = aTimeOut;
   return 0;
}

int APPLAPI SockSetRetries(tSockCB *SockCB, const unsigned short aRetries)
{
   if (SockCB == &ABNSockCB)
      return errSockInit;

   SockCB->Retries = aRetries;
   return 0;
}

int APPLAPI SockDone(tSockCB *SockCB)
{
   if (SockCB == &ABNSockCB)
      return errSockInit;

   if (SockCB->CommsPort != -1)
      _shutdown_close(SockCB->CommsPort);

   if (SockCB->ListenPort != -1)
      _shutdown_close(SockCB->ListenPort);

   return 0;
}

int APPLAPI SockClose(tSockCB *SockCB)
{
   if (SockCB == &ABNSockCB)
      return errSockInit;
#if defined(SO_LINGER)
   struct linger x = {1, 5};
   setsockopt(SockCB->CommsPort, SOL_SOCKET, SO_LINGER,(char*)&x, sizeof(x));
#endif
   if (SockCB->CommsPort != -1)
     _shutdown_close(SockCB->CommsPort);
   return 0;
}

int APPLAPI SockReadLength(const tSockCB *SockCB, unsigned int *aSize)
{
   unsigned int Size;

   if (SockCB == &ABNSockCB)
      return errSockInit;

   int n = _ReadAll(SockCB, (char *)&Size, sizeof(Size));

   if (n != sizeof(unsigned int))
      return errSockRead;

   *aSize = ntohl(Size);
   return 0;
}

int APPLAPI SockRead(const tSockCB       *SockCB,
                     void                *aBuffer,
                     const unsigned int  aBufferLen)
{
   if (SockCB == &ABNSockCB)
      return errSockInit;

   int n = _ReadAll(SockCB, (char *)aBuffer, aBufferLen);

   if ((unsigned int)n != aBufferLen)
      return errSockRead;

   return 0;
}

int APPLAPI SockStreamRead(const tSockCB       *SockCB,
                           void                *aBuffer,
                           const unsigned int  aBufferLen,
                           unsigned int       *aSize)
{
   if (SockCB == &ABNSockCB)
      return errSockInit;

   int n = recv(SockCB->CommsPort, (char *) aBuffer, aBufferLen, 0);
   if (n < 0)
      return errSockRead;
   *aSize = n;
   return 0;
}

int APPLAPI SockWaitRead(const tSockCB *SockCB, const unsigned int aTimeOut, bool *aResult)
{
   if (SockCB == &ABNSockCB)
      return errSockInit;

   return _Wait(SockCB->CommsPort, aTimeOut, true, aResult);
}

int APPLAPI SockWaitWrite(const tSockCB *SockCB, const unsigned int aTimeOut, bool *aResult)
{
   if (SockCB == &ABNSockCB)
      return errSockInit;

   return _Wait(SockCB->CommsPort, aTimeOut, false, aResult);
}

int APPLAPI SockWaitServer(const tSockCB *SockCB, const unsigned int aTimeOut, bool *aResult)
{
   if (SockCB == &ABNSockCB)
      return errSockInit;

   return _Wait(SockCB->ListenPort, aTimeOut, true, aResult);
}

int APPLAPI SockWriteLength(tSockCB *SockCB, const unsigned int aLength)
{
   unsigned int writeLength = htonl(aLength);

   int n;

   if (SockCB == &ABNSockCB)
      return errSockInit;

   n = send(SockCB->CommsPort, (char *)&writeLength, sizeof(writeLength), 0);

   if (n != sizeof(writeLength))
   {
      _SetError(SockCB);
      return errSockWrite;
   }

   return 0;
}

int APPLAPI SockWrite(tSockCB *SockCB, void *aBuffer, const unsigned int aBufferLen)
{
   int n;

   if (SockCB == &ABNSockCB)
      return errSockInit;

   n = send(SockCB->CommsPort, (char *)aBuffer, aBufferLen, 0);

   if ((unsigned int)n != aBufferLen)
   {
      _SetError(SockCB);
      return errSockWrite;
   }

   return 0;
}

int APPLAPI SockError(const tSockCB *SockCB)
{
   return SockCB->ErrorNo;
}

char *APPLAPI SockErrorMsg(const tSockCB *SockCB)
{
   return SockErrorText(SockCB->ErrorNo);
}

int APPLAPI SockErrorMsgNet(const tSockCB *SockCB, char *Buffer, int BufferLen)
{
   int i;

   strncpy(Buffer, SockErrorMsg(SockCB), BufferLen-1);

   Buffer[BufferLen-1] = 0;

   for (i = strlen(Buffer); i < BufferLen; i++)
      Buffer[i] = ' ';

   return SockCB->ErrorNo;
}

static tSockCB *_SockInit(const char *aHost, const char *aService, int *rc)
{
   tSockCB         *SockCB = &ABNSockCB;
   servent         *ps;
   hostent         *ph;
   sockaddr_in      Addr;

   int ListenPort = -1;
   int isNo, i;

#if defined(M_DOS) || defined(M_W32)

   WSADATA  wsadata;

   *rc = WSAStartup(DESIRED_WINSOCK_VERSION, &wsadata);

   if (*rc)
   {
      *rc = SockCB->ErrorNo = WSAGetLastError();
      return SockCB;
   }
   /* Check that the windows socket dll version is ok */
   if (wsadata.wVersion < MINIMUM_WINSOCK_VERSION)
   {
      *rc = errSockVersion;
      _SetError(SockCB);
      return SockCB;
   }
#endif
   *rc = 0;

   for (isNo = 1, i = 0; aService[i]; i++)
   {
      if (aService[i] < '0' || aService[i] > '9')
      {
         isNo = 0;
         break;
      }
   }

   if (isNo == 1)
      Addr.sin_port = htons((short)atoi(aService));
   else
   {
      ps = getservbyname(aService, PROTOCOL);
      if (ps == 0)
      {
         *rc = errSockService;
         _SetError(SockCB);
         return SockCB;
      }
      Addr.sin_port = ps->s_port;
   }

   if (aHost != 0)
   {
      ph = gethostbyname(aHost);
      if (ph == 0)
      {
         *rc = errSockHost;
         _SetError(SockCB);
         return SockCB;
      }
      Addr.sin_family = ph->h_addrtype;
      memcpy(&Addr.sin_addr, ph->h_addr, ph->h_length);
   }
   else
   {
      Addr.sin_addr.s_addr = INADDR_ANY;
      Addr.sin_family = AF_INET;
      ListenPort = socket(AF_INET, SOCK_STREAM, 0);
      if (ListenPort == -1)
      {
         *rc = errSockSocket;
         _SetError(SockCB);
         return SockCB;
      }
      int on=1;
#if defined(SO_REUSEADDR)
      if (setsockopt(ListenPort,SOL_SOCKET,SO_REUSEADDR,(char*)&on,
                     sizeof(on))<0)
      {
         *rc = errSockSocket;
         _SetError(SockCB);
         return SockCB;
      }
#endif
#if defined(NO_SO_REUSEPORT)
      if (setsockopt(ListenPort,SOL_SOCKET,SO_REUSEPORT,(char*)&on,
                     sizeof(on))<0)
      {
         *rc = errSockSocket;
         _SetError(SockCB);
         return SockCB;
      }
#endif
      *rc=bind(ListenPort, (struct sockaddr*)&Addr, sizeof(Addr));
      if (*rc == -1)
      {
         *rc = errSockBind;
         _SetError(SockCB);
         return SockCB;
      }
      *rc=listen(ListenPort, SOCKET_QSIZE);
      if (*rc == -1)
      {
         *rc = errSockListen;
         _SetError(SockCB);
         return SockCB;
      }
      if (Addr.sin_port == 0)
      {
         
#if defined(M_AIX) || defined(M_GNU)
		 socklen_t len = sizeof(Addr);
         getsockname(ListenPort, (struct sockaddr*)&Addr, &len);
#else
         int len = sizeof(Addr);
		 getsockname(ListenPort, (struct sockaddr*)&Addr, (int*)&len);
#endif
      }
   }
   SockCB = (tSockCB *) malloc(sizeof(tSockCB));

   if (SockCB != 0)
   {
      SockCB->ListenPort = ListenPort;
      SockCB->CommsPort = -1;
      SockCB->Retries = 5;
      SockCB->TimeOut = 10000;
      SockCB->Addr = Addr;
      if (aHost != 0)
         SockCB->Addr.sin_addr.s_addr = *((unsigned int *)ph->h_addr);
      else
         SockCB->Addr.sin_addr.s_addr = INADDR_ANY;
      SockCB->Addr.sin_family = AF_INET;
   }
   return SockCB;
}

static int _Wait(const int            aPort,
                 const unsigned int  aMSec,
                 const bool           onRead,
                 bool                *aResult)
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
   FD_SET((unsigned)aPort, &OnSet);
   int rc = select(FD_SETSIZE, (onRead == true?&OnSet:0), (onRead==true?0:&OnSet), 0, TOP);
   if (rc <= 0)
      return errSockTimeout;
   *aResult = FD_ISSET(aPort, &OnSet) ? true : false;
   return 0;
}

static int _ReadAll(const tSockCB *SockCB, char *Buff, const int Size)
{
   int BytesRead = 0, n, Try = 0;
   bool Result;

   while (BytesRead < Size)
   {
      if (SockCB->TimeOut)
      {
         if (_Wait(SockCB->CommsPort, SockCB->TimeOut, true, &Result)==0)
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

      if (n < 0) // underflow ??
         return n;

      if (n == 0)
         return BytesRead;

      BytesRead += n;
      Buff      += n;
   }

   return BytesRead;
}

#if defined(M_AIX)
static void _SetError(tSockCB *SockCB)
{
   SockCB->ErrorNo = errno;
}

char *APPLAPI SockErrorText(const int rc)
{
   switch (rc)
   {
      case EPERM:            return "Operation not permitted";
      case ENOENT:           return "No such file or directory";
      case ESRCH:            return "No such process";
      case EINTR:            return "interrupted system call";
      case EIO:              return "I/O error";
      case ENXIO:            return "No such device or address";
      case E2BIG:            return "Arg list too long";
      case ENOEXEC:          return "Exec format error";
      case EBADF:            return "Bad file descriptor";
      case ECHILD:           return "No child processes";
      case EAGAIN:           return "Resource temporarily unavailable";
      case ENOMEM:           return "Not enough space";
      case EACCES:           return "Permission denied";
      case EFAULT:           return "Bad address";
      case ENOTBLK:          return "Block device required";
      case EBUSY:            return "Resource busy";
      case EEXIST:           return "File exists";
      case EXDEV:            return "Improper link";
      case ENODEV:           return "No such device";
      case ENOTDIR:          return "Not a directory";
      case EISDIR:           return "Is a directory";
      case EINVAL:           return "Invalid argument";
      case ENFILE:           return "Too many open files in system";
      case EMFILE:           return "Too many open files";
      case ENOTTY:           return "Inappropriate I/O control operation";
      case ETXTBSY:          return "Text file busy";
      case EFBIG:            return "File too large";
      case ENOSPC:           return "No space left on device";
      case ESPIPE:           return "Invalid seek";
      case EROFS:            return "Read only file system";
      case EMLINK:           return "Too many links";
      case EPIPE:            return "Broken pipe";
      case EDOM:             return "Domain error within math function";
      case ERANGE:           return "Result too large";
      case ENOMSG:           return "No message of desired type";
      case EIDRM:            return "Identifier removed";
      case ECHRNG:           return "Channel number out of range";
      case EL2NSYNC:         return "Level 2 not synchronized";
      case EL3HLT:           return "Level 3 halted";
      case EL3RST:           return "Level 3 reset";
      case ELNRNG:           return "Link number out of range";
      case EUNATCH:          return "Protocol driver not attached";
      case ENOCSI:           return "No CSI structure available";
      case EL2HLT:           return "Level 2 halted";
      case EDEADLK:          return "Resource deadlock avoided";
      case ENOTREADY:        return "Device not ready";
      case EWRPROTECT:       return "Write-protected media";
      case EFORMAT:          return "Unformatted media";
      case ENOLCK:           return "No locks available";
      case ENOCONNECT:       return "no connection";
      case ESTALE:           return "no filesystem";
      case EDIST:            return "old, currently unused AIX errno" ;
      case EINPROGRESS:      return "Operation now in progress";
      case EALREADY:         return "Operation already in progress";
      case ENOTSOCK:         return "Socket operation on non-socket";
      case EDESTADDRREQ:     return "Destination address required";
      case EMSGSIZE:         return "Message too long";
      case EPROTOTYPE:       return "Protocol wrong type for socket";
      case ENOPROTOOPT:      return "Protocol not available";
      case EPROTONOSUPPORT:  return "Protocol not supported";
      case ESOCKTNOSUPPORT:  return "Socket type not supported";
      case EOPNOTSUPP:       return "Operation not supported on socket";
      case EPFNOSUPPORT:     return "Protocol family not supported";
      case EAFNOSUPPORT:     return "Address family not supported by protocol family";
      case EADDRINUSE:       return "Address already in use";
      case EADDRNOTAVAIL:    return "Can't assign requested address";
      case ENETDOWN:         return "Network is down";
      case ENETUNREACH:      return "Network is unreachable";
      case ENETRESET:        return "Network dropped connection on reset";
      case ECONNABORTED:     return "Software caused connection abort";
      case ECONNRESET:       return "Connection reset by peer";
      case ENOBUFS:          return "No buffer space available";
      case EISCONN:          return "Socket is already connected";
      case ENOTCONN:         return "Socket is not connected";
      case ESHUTDOWN:        return "Can't send after socket shutdown";
      case ETIMEDOUT:        return "Connection timed out";
      case ECONNREFUSED:     return "Connection refused";
      case EHOSTDOWN:        return "Host is down";
      case EHOSTUNREACH:     return "No route to host";
      case ERESTART:         return "restart the system call";
      case EPROCLIM:         return "Too many processes";
      case EUSERS:           return "Too many users";
      case ELOOP:            return "Too many levels of symbolic links";
      case ENAMETOOLONG:     return "File name too long";
      case EREMOTE:          return "Item is not local to host";
      case ENOSYS:           return "Function not implemented  POSIX";
      case EMEDIA:           return "media surface error";
      case ESOFT:            return "I/O completed, but needs relocation";
      case ENOATTR:          return "no attribute found";
      case ESAD:             return "security authentication denied";
      case ENOTRUST:         return "not a trusted program" ;
      case ETOOMANYREFS:     return "Too many references: can't splice";
      case EILSEQ:           return "Invalid wide character";
      case ECANCELED:        return "asynchronous i/o cancelled";
      case ENOSR:            return "temp out of streams resources";
      case ETIME:            return "I_STR ioctl timed out";
      case EBADMSG:          return "wrong message type at stream head";
      case EPROTO:           return "STREAMS protocol error";
      case ENODATA:          return "no message ready at stream head";
      case ENOSTR:           return "fd is not a stream";
      case ENOTSUP:          return "POSIX threads unsupported value";
      case EMULTIHOP:        return "multihop is not allowed";
      case ENOLINK:          return "the link has been severed";
      case EOVERFLOW:        return "value too large to be stored in data type";
   }
   return "Unknown Error";
}
#endif

#if defined(M_GNU)
static void _SetError(tSockCB *SockCB)
{
   SockCB->ErrorNo = errno;
}

char *APPLAPI SockErrorText(const int rc)
{
   switch (rc)
   {
      case EPERM:            return "Operation not permitted";
      case ENOENT:           return "No such file or directory";
      case ESRCH:            return "No such process";
      case EINTR:            return "interrupted system call";
      case EIO:              return "I/O error";
      case ENXIO:            return "No such device or address";
      case E2BIG:            return "Arg list too long";
      case ENOEXEC:          return "Exec format error";
      case EBADF:            return "Bad file descriptor";
      case ECHILD:           return "No child processes";
      case EAGAIN:           return "Resource temporarily unavailable";
      case ENOMEM:           return "Not enough space";
      case EACCES:           return "Permission denied";
      case EFAULT:           return "Bad address";
      case ENOTBLK:          return "Block device required";
      case EBUSY:            return "Resource busy";
      case EEXIST:           return "File exists";
      case EXDEV:            return "Improper link";
      case ENODEV:           return "No such device";
      case ENOTDIR:          return "Not a directory";
      case EISDIR:           return "Is a directory";
      case EINVAL:           return "Invalid argument";
      case ENFILE:           return "Too many open files in system";
      case EMFILE:           return "Too many open files";
      case ENOTTY:           return "Inappropriate I/O control operation";
      case ETXTBSY:          return "Text file busy";
      case EFBIG:            return "File too large";
      case ENOSPC:           return "No space left on device";
      case ESPIPE:           return "Invalid seek";
      case EROFS:            return "Read only file system";
      case EMLINK:           return "Too many links";
      case EPIPE:            return "Broken pipe";
      case EDOM:             return "Domain error within math function";
      case ERANGE:           return "Result too large";
      case ENOMSG:           return "No message of desired type";
      case EIDRM:            return "Identifier removed";
      case ECHRNG:           return "Channel number out of range";
      case EL2NSYNC:         return "Level 2 not synchronized";
      case EL3HLT:           return "Level 3 halted";
      case EL3RST:           return "Level 3 reset";
      case ELNRNG:           return "Link number out of range";
      case EUNATCH:          return "Protocol driver not attached";
      case ENOCSI:           return "No CSI structure available";
      case EL2HLT:           return "Level 2 halted";
      case EDEADLK:          return "Resource deadlock avoided";
      case ENOLCK:           return "No locks available";
      case ESTALE:           return "no filesystem";
      case EINPROGRESS:      return "Operation now in progress";
      case EALREADY:         return "Operation already in progress";
      case ENOTSOCK:         return "Socket operation on non-socket";
      case EDESTADDRREQ:     return "Destination address required";
      case EMSGSIZE:         return "Message too long";
      case EPROTOTYPE:       return "Protocol wrong type for socket";
      case ENOPROTOOPT:      return "Protocol not available";
      case EPROTONOSUPPORT:  return "Protocol not supported";
      case ESOCKTNOSUPPORT:  return "Socket type not supported";
      case EOPNOTSUPP:       return "Operation not supported on socket";
      case EPFNOSUPPORT:     return "Protocol family not supported";
      case EAFNOSUPPORT:     return "Address family not supported by protocol family";
      case EADDRINUSE:       return "Address already in use";
      case EADDRNOTAVAIL:    return "Can't assign requested address";
      case ENETDOWN:         return "Network is down";
      case ENETUNREACH:      return "Network is unreachable";
      case ENETRESET:        return "Network dropped connection on reset";
      case ECONNABORTED:     return "Software caused connection abort";
      case ECONNRESET:       return "Connection reset by peer";
      case ENOBUFS:          return "No buffer space available";
      case EISCONN:          return "Socket is already connected";
      case ENOTCONN:         return "Socket is not connected";
      case ESHUTDOWN:        return "Can't send after socket shutdown";
      case ETIMEDOUT:        return "Connection timed out";
      case ECONNREFUSED:     return "Connection refused";
      case EHOSTDOWN:        return "Host is down";
      case EHOSTUNREACH:     return "No route to host";
      case ERESTART:         return "restart the system call";
      case EUSERS:           return "Too many users";
      case ELOOP:            return "Too many levels of symbolic links";
      case ENAMETOOLONG:     return "File name too long";
      case EREMOTE:          return "Item is not local to host";
      case ENOSYS:           return "Function not implemented  POSIX";
      case ETOOMANYREFS:     return "Too many references: can't splice";
      case EILSEQ:           return "Invalid wide character";
      case ECANCELED:        return "asynchronous i/o cancelled";
      case ENOSR:            return "temp out of streams resources";
      case ETIME:            return "I_STR ioctl timed out";
      case EBADMSG:          return "wrong message type at stream head";
      case EPROTO:           return "STREAMS protocol error";
      case ENODATA:          return "no message ready at stream head";
      case ENOSTR:           return "fd is not a stream";
#if !defined(M_GNU)
      case ENOTSUP:          return "POSIX threads unsupported value";
#endif
      case EMULTIHOP:        return "multihop is not allowed";
      case ENOLINK:          return "the link has been severed";
      case EOVERFLOW:        return "value too large to be stored in data type";
   }
   return "Unknown Error";
}
#endif

#if defined(M_DOS) || defined(M_W32)
static void _SetError(tSockCB *SockCB)
{
   SockCB->ErrorNo = WSAGetLastError();
}

char *APPLAPI SockErrorText(const int rc)
{
   switch (rc)
   {
      case WSAEACCES:           return "Access denied";
      case WSAEADDRINUSE:       return "Address already in use";
      case WSAEADDRNOTAVAIL:    return "Can't assign requested address";
      case WSAEAFNOSUPPORT:     return "Address family not supported by protocol family";
      case WSAEALREADY:         return "Operation already in progress";
      case WSAEBADF:            return "Bad file number";
      case WSAECONNABORTED:     return "Software caused connection abort";
      case WSAECONNREFUSED:     return "Connection refused";
      case WSAECONNRESET:       return "Connection reset by peer";
      case WSAEDESTADDRREQ:     return "Destination address required";
      case WSAEDISCON:          return "Disconnected";
      case WSAEDQUOT:           return "Disk quota exceeded";
      case WSAEFAULT:           return "Bad address";
      case WSAEHOSTDOWN:        return "Host is down";
      case WSAEHOSTUNREACH:     return "Host is unreachable";
      case WSAEINPROGRESS:      return "Operation now in progress";
      case WSAEINTR:            return "Interrupted system call";
      case WSAEINVAL:           return "Invalid argument";
      case WSAEISCONN:          return "Socket is already connected";
      case WSAELOOP:            return "Too many levels of symbolic links";
      case WSAEMFILE:           return "Too many open files";
      case WSAEMSGSIZE:         return "Message too long";
      case WSAENAMETOOLONG:     return "Name too long";
      case WSAENETDOWN:         return "Network is down";
      case WSAENETRESET:        return "Network was reset";
      case WSAENETUNREACH:      return "ICMP network unreachable";
      case WSAENOBUFS:          return "No buffer space is supported";
      case WSAENOPROTOOPT:      return "Bad protocol option";
      case WSAENOTCONN:         return "Socket is not connected";
      case WSAENOTEMPTY:        return "Directory not empty";
      case WSAENOTSOCK:         return "Socket operation on non-socket";
      case WSAEOPNOTSUPP:       return "Operation not supported on socket";
      case WSAEPFNOSUPPORT:     return "Protocol family not supported";
      case WSAEPROCLIM:         return "EPROCLIM returned";
      case WSAEPROTONOSUPPORT:  return "Protocol not supported";
      case WSAEPROTOTYPE:       return "Protocol is wrong type for socket";
      case WSAEREMOTE:          return "The object is remote";
      case WSAESHUTDOWN:        return "Can't send after socket shutdown";
      case WSAESOCKTNOSUPPORT:  return "Socket type not supported";
      case WSAESTALE:           return "ESTALE returned";
      case WSAETIMEDOUT:        return "Connection timed out";
      case WSAETOOMANYREFS:     return "Too many references";
      case WSAEUSERS:           return "EUSERS returned";
      case WSAEWOULDBLOCK:      return "Operation would block";
      case WSAHOST_NOT_FOUND:   return "Host not found";
      case WSANOTINITIALISED:   return "Not initialized";
      case WSANO_DATA:          return "No data record available";
      case WSANO_RECOVERY:      return "Non-recoverable error";
      case WSASYSNOTREADY:      return "System not ready";
      case WSATRY_AGAIN:        return "Try again";
      case WSAVERNOTSUPPORTED:  return "Version is not supported";
   }
   return "Unknown Error";
}
#endif

