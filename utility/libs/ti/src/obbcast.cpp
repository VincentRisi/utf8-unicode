#include "versions.h"
#define _VERSION_NO "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
COMPILED_VERSION(_VERSION_NO);

// file       : obbcast.c
// programmer : v.risi
// conversion : m.barnard
// description: communications handler for sockets
//              that is object based 'C' code.
//              This has been converted from tcp to udp Broadcast Protocol

#include "machine.h"

#if defined (M_AIX) || (M_DOS) || (M_OS2)

#ifdef M_AIX
#include <strings.h>
#else
#include <alloc.h>
#include <mem.h>
#endif
#include "obbcast.h"

static pBcastCB _SockInit(char * aHost, char * aService, int *rc);
static int _ReadAll(pBcastCB BcastCB, char * Buff, long Size);
static void _SetError(pBcastCB BcastCB);
static char * _GetError(int rc);

static tBcastCB ABNBcastCB;

int APPLAPI BcastServerInit(pBcastCB *BcastCB, char * aService)
{
  int rc;
  *BcastCB = _SockInit(0, aService, &rc);
  return rc;
}

int APPLAPI BcastClientInit(pBcastCB *BcastCB, char * aHost, char * aService)
{
  int rc;
  *BcastCB = _SockInit(aHost, aService, &rc);
  return rc;
}

int APPLAPI BcastDone(pBcastCB BcastCB)
{
  if (BcastCB == &ABNBcastCB) return errSockInit;
  if (BcastCB->CommsPort != -1)
    soclose(BcastCB->CommsPort);
  if (BcastCB->ListenPort != -1)
    soclose(BcastCB->ListenPort);
  return 0;
}


int APPLAPI BcastClose(pBcastCB BcastCB)
{
  if (BcastCB == &ABNBcastCB) return errSockInit;
  if (BcastCB->CommsPort != -1)
    soclose(BcastCB->CommsPort);
  if (BcastCB->ListenPort != -1)
    soclose(BcastCB->ListenPort);
  BcastCB->CommsPort = -1;
  return 0;
}


int APPLAPI BcastDGramRead(pBcastCB BcastCB, void * aBuffer, unsigned long aBufferLen, unsigned long * aSize)
{
  socklen_t Addr_size = sizeof(BcastCB->Addr);
  long n;

  if (BcastCB == &ABNBcastCB) return errSockInit;

  n = recvfrom(BcastCB->ListenPort, (char *) aBuffer, aBufferLen, 0, (struct sockaddr *) &BcastCB->Addr, &Addr_size);
  if (n < 0)
  {
    _SetError(BcastCB);
    return errSockRead;
  }
  *aSize = n;
  return 0;
}


int APPLAPI BcastWrite(pBcastCB BcastCB, void * aBuffer, unsigned long aBufferLen)
{
  socklen_t Addr_size = sizeof(BcastCB->Addr);
  int n;

  if (BcastCB == &ABNBcastCB) return errSockInit;

  n = sendto(BcastCB->ListenPort, (char *)aBuffer, aBufferLen, 0, (struct sockaddr *) &BcastCB->Addr, Addr_size);
  if (n != aBufferLen)
  {
    _SetError(BcastCB);
    return errSockWrite;
  }
  return 0;
}

int APPLAPI BcastError(pBcastCB BcastCB)
{
  return BcastCB->ErrorNo;
}

char * APPLAPI BcastErrorMsg(pBcastCB BcastCB)
{
  return _GetError(BcastCB->ErrorNo);
}

static pBcastCB _SockInit(char * aHost, char * aService, int *rc)
{
  pBcastCB BcastCB = &ABNBcastCB;
  servent F16* ps;
  hostent F16* ph;
  sockaddr_in Addr;
  int ListenPort = -1;
  short aOption, aValue, OptErr;
  int aIOCTLVal = 0;

#ifdef M_OS2
  *rc = sock_init();
  if (*rc)
  {
    _SetError(BcastCB);
    return BcastCB;
  }
#endif
#ifdef M_DOS
  WSADATA  wsadata;
  *rc = WSAStartup(DESIRED_WINSOCK_VERSION, &wsadata);
  if (*rc)
  {
    *rc = BcastCB->ErrorNo = WSAGetLastError();
    return BcastCB;
  }
  /* Check that the windows socket dll version is ok */
  if (wsadata.wVersion < MINIMUM_WINSOCK_VERSION)
  {
    *rc = errSockVersion;
    _SetError(BcastCB);
    return BcastCB;
  }
#endif
  ps = getservbyname(aService, PROTOCOL);
  if (ps == 0)
  {
    *rc = errSockService;
    _SetError(BcastCB);
    return BcastCB;
  }
  Addr.sin_port = ps->s_port;
  if (aHost != 0)
  { // Client Code
    ListenPort = socket(AF_INET, SOCK_DGRAM, 0);
    if (ListenPort == -1)
    {
      *rc = errSockSocket;
      _SetError(BcastCB);
      return BcastCB;
    }

    ph = gethostbyname(aHost);
    if (ph == 0)
    {
      *rc = errSockHost;
      _SetError(BcastCB);
      return BcastCB;
    }
    Addr.sin_family = AF_INET;
    memcpy(&Addr.sin_addr, ph->h_addr, ph->h_length);

    aOption = SO_BROADCAST;
    aValue = TRUE;
    OptErr = setsockopt(ListenPort, SOL_SOCKET, aOption, (char *) &aValue, sizeof(int));
    if (OptErr)
    {
      *rc = errSockSetOpt;
      _SetError(BcastCB);
      return BcastCB;
    }
    aIOCTLVal = 0;
    OptErr = ioctl(ListenPort, FIONBIO, (char*) &aIOCTLVal, sizeof(aIOCTLVal));
    if (OptErr)
    {
      *rc = errSockIOCtl;
      _SetError(BcastCB);
      return BcastCB;
    }

  }
  else
  { // Server Code
    Addr.sin_addr.s_addr = INADDR_ANY;
    Addr.sin_family = AF_INET;
    ListenPort = socket(AF_INET, SOCK_DGRAM, 0);
    if (ListenPort == -1)
    {
      *rc = errSockSocket;
      _SetError(BcastCB);
      return BcastCB;
    }
    if ((*rc=bind(ListenPort, (struct sockaddr*)&Addr, sizeof(Addr))) == -1)
    {
      *rc = errSockBind;
      _SetError(BcastCB);
      return BcastCB;
    }
    aIOCTLVal = 0;
    OptErr = ioctl(ListenPort, FIONBIO, (char*) &aIOCTLVal, sizeof(aIOCTLVal));
    if (OptErr)
    {
      *rc = errSockIOCtl;
      _SetError(BcastCB);
      return BcastCB;
    }

  }
  BcastCB = (pBcastCB) calloc(sizeof(tBcastCB), 1);
  if (BcastCB != 0)
  {
    BcastCB->ListenPort = ListenPort;
    BcastCB->CommsPort = -1;
    BcastCB->Retries = 5;
    BcastCB->TimeOut = 1000;
    BcastCB->Addr = Addr;
    BcastCB->Addr.sin_port = ps->s_port;
    if (aHost != 0)
      BcastCB->Addr.sin_addr.s_addr = *((unsigned long *)ph->h_addr);
    else
      BcastCB->Addr.sin_addr.s_addr = INADDR_ANY;
    BcastCB->Addr.sin_family = AF_INET;
  }
  return BcastCB;
}


#if defined(M_AIX)
static void _SetError(pBcastCB BcastCB)
{
  BcastCB->ErrorNo = errno;
}

static char * _GetError(int rc)
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

#if defined(M_OS2)
static void _SetError(pBcastCB BcastCB)
{
  BcastCB->ErrorNo = sock_errno();
}
static char * _GetError(int rc)
{
  switch (rc)
  {
    case SOCEACCES:                return "Permission denied";
    case SOCEADDRINUSE:            return "Address already in use";
    case SOCEADDRNOTAVAIL:         return "Can't assign requested address";
    case SOCEAFNOSUPPORT:          return "Address family not supported by protocol family";
    case SOCEALREADY:              return "Operation already in progress";
    case SOCEBADF:                 return "Bad file number";
    case SOCECONNABORTED:          return "Software caused connection abort";
    case SOCECONNREFUSED:          return "Connection refused";
    case SOCECONNRESET:            return "Connection reset by peer";
    case SOCEDESTADDRREQ:          return "Destination address required";
    case SOCEFAULT:                return "Bad address";
    case SOCEHOSTDOWN:             return "Host is down";
    case SOCEHOSTUNREACH:          return "No route to host";
    case SOCEINPROGRESS:           return "Operation now in progress";
    case SOCEINTR:                 return "Interrupted system call";
    case SOCEINVAL:                return "Invalid argument";
    case SOCEISCONN:               return "Socket is already connected";
    case SOCELOOP:                 return "Too many levels of symbolic links";
    case SOCEMFILE:                return "Too many open files";
    case SOCEMSGSIZE:              return "Message too long";
    case SOCENAMETOOLONG:          return "File name too long";
    case SOCENETDOWN:              return "Network is down";
    case SOCENETRESET:             return "Network dropped connection on reset";
    case SOCENETUNREACH:           return "Network is unreachable";
    case SOCENOBUFS:               return "No buffer space available";
    case SOCENOPROTOOPT:           return "Protocol not available";
    case SOCENOTCONN:              return "Socket is not connected";
    case SOCENOTEMPTY:             return "Directory not empty";
    case SOCENOTSOCK:              return "Socket operation on non-socket";
    case SOCENXIO:                 return "No such device or address";
    case SOCEOPNOTSUPP:            return "Operation not supported on socket";
    case SOCEOS2ERR:                return "OS/2 Error";
    case SOCEPERM:                 return "Not owner";
    case SOCEPFNOSUPPORT:          return "Protocol family not supported";
    case SOCEPIPE:                 return "Broken pipe";
    case SOCEPROTONOSUPPORT:       return "Protocol not supported";
    case SOCEPROTOTYPE:            return "Protocol wrong type for socket";
    case SOCESHUTDOWN:             return "Can't send after socket shutdown";
    case SOCESOCKTNOSUPPORT:       return "Socket type not supported";
    case SOCESRCH:                 return "No such process";
    case SOCETIMEDOUT:             return "Connection timed out";
    case SOCETOOMANYREFS:          return "Too many references: can't splice";
    case SOCEWOULDBLOCK:           return "Operation would block";
  }
  return "Unknown Error";
}

#endif

#if defined(M_DOS)
static void _SetError(pBcastCB BcastCB)
{
  BcastCB->ErrorNo = WSAGetLastError();
}

static char * _GetError(int rc)
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



#endif
