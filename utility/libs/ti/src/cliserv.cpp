#include "versions.h"
#define _VERSION_NO "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
COMPILED_VERSION(_VERSION_NO);

#include "machine.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>

#ifdef M_OS2
#ifndef OS2
#define OS2
#endif
#define INCL_DOS
#define INCL_DOSERRORS
#include "osfuncs.h"
#endif

#ifdef M_AIX
extern "C" {
#include "osfuncs.h"
}
#undef abs
#include <macros.h>
#include <pwd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif

#include "xstring.h"
#include "xdir.h"

#ifdef M_AIX
#include "errnomsg.h"
#endif
#include "cliserv.h"

#define default_reached 0

static eErrTyp ErrTyp;
static int ErrorNo;

#ifdef M_OS2
//int errno;
#endif
extern int errno;

static struct ClientServer* CliServInit(int isServer, char *name,
                                        long size, long timeout);
static int ParseName(char *name, char sep, char *names[]);
static int GetErrorNo(struct ClientServer* Self, eErrTyp* errtyp);
static void DisplayError(struct ClientServer* Self);

#ifdef M_AIX
static struct sembuf incServRead    = { 0,  1, SEM_UNDO };
static struct sembuf decServRead    = { 0, -1, SEM_UNDO };
static struct sembuf incServWrite   = { 1,  1, SEM_UNDO };
static struct sembuf decServWrite   = { 1, -1, SEM_UNDO };
static struct sembuf incClientRead  = { 2,  1, SEM_UNDO };
static struct sembuf decClientRead  = { 2, -1, SEM_UNDO };
static struct sembuf incClientWrite = { 3,  1, SEM_UNDO };
static struct sembuf decClientWrite = { 3, -1, SEM_UNDO };

static void _semzero(struct ClientServer* Self);
static int _semop_tmout(int semid, struct sembuf **sops,
                        int nops, long timeout);

static int AixShmInit(struct ClientServer* Self, int isServer,
                      char* AppName, short key, long size, long timeout);
static int AixShmOpen(struct ClientServer* Self);
static int AixShmRead(struct ClientServer* Self, PBuffer buf);
static int AixShmWrite(struct ClientServer* Self, PBuffer buf);

static int AixSockInit(struct ClientServer* Self, int isServer,
                       char* host, char* service, long size, long timeout);
static int AixSockRead(struct ClientServer* Self, PBuffer buf);
static int AixSockWrite(struct ClientServer* Self, PBuffer buf);
#endif

#ifdef M_OS2
static int Os2NpInit(struct ClientServer* Self, int isServer,
                      char* PipeName, long size, long timeout);
static int Os2NpRead(struct ClientServer* Self,  PBuffer buf);
static int Os2NpWrite(struct ClientServer* Self, PBuffer buf);
static void Os2NpWaitForClose(struct ClientServer* Self, PBYTE pBuffer,
                              ULONG cBuffer);

static int Os2ShmInit(struct ClientServer* Self, int isServer,
                      char* SegName, long size, long timeout);
static int Os2ShmOpen(struct ClientServer* Self);
static int Os2ShmRead(struct ClientServer* Self,  PBuffer buf);
static int Os2ShmWrite(struct ClientServer* Self, PBuffer buf);

static int Os2SockInit(struct ClientServer* Self, int isServer,
                       char* host, char* service,
                       long size, long timeout);
static int Os2SockRead(struct ClientServer* Self, PBuffer buf);
static int Os2SockWrite(struct ClientServer* Self, PBuffer buf);
#endif

ServerThis ServInit(char *name, long size, long timeout)
{
  char *copy = strdup(name);
  ServerThis Self = CliServInit(1, copy, size, timeout);
  free(copy);
  return Self;
}

int ServOpen(ServerThis Self)
{
  Self->errtyp = errtypNone;

  switch (Self->type)
  {
#ifdef M_OS2
  case TYP_NP:
    Self->LastError = DosConnectNPipe(Self->u.np.pipe);
    if (Self->LastError)
    {
      Self->errtyp = errtypNp;
      return ERROR_RET;
    }
    return 0;
  case TYP_SHM:
    return Os2ShmOpen(Self);
  case TYP_SOCKET:
    if ((Self->u.socket.sd = accept(Self->u.socket.sockno, 0, 0)) == -1)
    {
      Self->errtyp = errtypSocket;
      Self->LastError = errno;
      return ERROR_RET;
    }
    return 0;
#endif
#ifdef M_AIX
  case TYP_SHM:
    return AixShmOpen(Self);
  case TYP_SOCKET:
    if ((Self->u.socket.sd = accept(Self->u.socket.sockno, 0, 0)) == -1)
    {
      Self->errtyp = errtypSocket;
      Self->LastError = errno;
      return ERROR_RET;
    }
    return 0;
#endif
  default:
    assert(default_reached);
  }
#pragma warn -rvl
}
#pragma warn .rvl

int ServRead(ServerThis Self, PBuffer buf)
{
  Self->errtyp = errtypNone;
  switch (Self->type)
  {
#ifdef M_OS2
  case TYP_NP:
    return Os2NpRead(Self, buf);
  case TYP_SHM:
    return Os2ShmRead(Self, buf);
  case TYP_SOCKET:
    return Os2SockRead(Self, buf);
#endif
#ifdef M_AIX
  case TYP_SHM:
    return AixShmRead(Self, buf);
  case TYP_SOCKET:
    return AixSockRead(Self, buf);
#endif
  default:
    assert(default_reached);
  }
#pragma warn -rvl
}
#pragma warn .rvl

int ServWrite(ServerThis Self, PBuffer buf)
{
  Self->errtyp = errtypNone;
  switch (Self->type)
  {
#ifdef M_OS2
  case TYP_NP:
    return Os2NpWrite(Self, buf);
  case TYP_SHM:
    return Os2ShmWrite(Self, buf);
  case TYP_SOCKET:
    return Os2SockWrite(Self, buf);
#endif
#ifdef M_AIX
  case TYP_SHM:
    return AixShmWrite(Self, buf);
  case TYP_SOCKET:
    return AixSockWrite(Self, buf);
#endif
  default:
    assert(default_reached);
  }
#pragma warn -rvl
}
#pragma warn .rvl

int ServClose(ServerThis Self)
{
#ifdef M_OS2
  char temp[10];
#endif

  Self->errtyp = errtypNone;
  switch (Self->type)
  {
#ifdef M_OS2
  case TYP_NP:
    Os2NpWaitForClose(Self, temp, 10);
    Self->LastError = DosDisConnectNPipe(Self->u.np.pipe);
    if (Self->LastError)
    {
      Self->errtyp = errtypNp;
      return ERROR_RET;
    }
    return 0;
  case TYP_SHM:
    return 0;
  case TYP_SOCKET:
        soclose(Self->u.socket.sd);     /* Close Socket Session */
    return 0;
#endif
#ifdef M_AIX
  case TYP_SHM:
    if (shmdt(Self->u.shm.start))
    {
      Self->errtyp = errtypShm;
      Self->LastError = errno;
      return ERROR_RET;
    }
    return 0;
  case TYP_SOCKET:
        close(Self->u.socket.sd);       /* Close Socket Session */
    return 0;
#endif
  default:
    assert(default_reached);
  }
#pragma warn -rvl
}
#pragma warn .rvl

int ServDone(ServerThis Self)
{
#ifdef M_OS2
  int rc;
#endif
  int ret = 0;
  ErrTyp = errtypNone;
  switch (Self->type)
  {
#ifdef M_OS2
  case TYP_NP:
    if ((rc = DosClose(Self->u.np.pipe)) != 0)
    {
      ErrTyp = errtypNp;
      ErrorNo = rc;
      ret = ERROR_RET;
    }
    break;
  case TYP_SHM:
    rc = DosFreeMem(Self->u.shm.Start);
    if (!rc) rc = DosCloseMutexSem(Self->u.shm.AccessSem);
    if (!rc) rc = DosCloseEventSem(Self->u.shm.ServWriteSem);
    if (!rc) rc = DosCloseEventSem(Self->u.shm.ServReadSem);
    if (!rc) rc = DosCloseEventSem(Self->u.shm.CliWriteSem);
    if (!rc) rc = DosCloseEventSem(Self->u.shm.CliReadSem);
    if (rc)
    {
      ErrTyp = errtypShm;
      ErrorNo = rc;
      ret = ERROR_RET;
    }
    break;
  case TYP_SOCKET:
        soclose(Self->u.socket.sockno); /* Close down Sockets */
    break;
#endif
#ifdef M_AIX
  case TYP_SHM:
    if (semctl(Self->u.shm.semid, 0, IPC_RMID, 0))
    {
      ErrTyp = errtypSem;
      ErrorNo = errno;
      ret = ERROR_RET;
    }
    break;
  case TYP_SOCKET:
        close(Self->u.socket.sockno);   /* Close down Sockets */
    break;
#endif
  default:
    assert(default_reached);
  }
  free(Self->name);
  free(Self);
  return ret;
}

int ServErrno(ServerThis Self, eErrTyp* errtyp)
{
  return GetErrorNo(Self, errtyp);
}

void ServDisplayErr(ServerThis Self)
{
  DisplayError(Self);
}

ClientThis ClientInit(char *name, long size, long timeout)
{
  char *copy = strdup(name);
  ServerThis Self = CliServInit(0, copy, size, timeout);
  free(copy);
  return Self;
}

int ClientCall(ClientThis Self, PBuffer bufOut, PBuffer bufIn)
{
  int rc;

  Self->errtyp = errtypNone;
  switch (Self->type)
  {
#ifdef M_OS2
  case TYP_NP:   /* OUT and IN swapped ??? should Self be ??? */
    Self->LastError = DosCallNPipe(Self->name,
      bufOut->p, bufOut->BytesToWrite,
      bufIn->p, bufIn->BytesToRead, &bufIn->BytesRead,
      Self->timeout);
    if (Self->LastError)
    {
      Self->errtyp = errtypNp;
      return ERROR_RET;
    }
    bufOut->BytesWritten = bufOut->BytesToWrite;  /*  since we don't really know */
    return 0;
  case TYP_SHM:
  case TYP_SOCKET:
    rc = ClientOpen(Self);
    if (rc)
      return rc;
    rc = ClientTransact(Self, bufOut, bufIn);
    if (rc)
      return rc;
    return ClientClose(Self);
#endif
#ifdef M_AIX
  case TYP_SHM:
  case TYP_SOCKET:
    rc = ClientOpen(Self);
    if (rc)
      return rc;
    rc = ClientTransact(Self, bufOut, bufIn);
    if (rc)
      return rc;
    return ClientClose(Self);
#endif
  default:
    assert(default_reached);
  }
#pragma warn -rvl
}
#pragma warn .rvl

int ClientOpen(ClientThis Self)
{
  Self->errtyp = errtypNone;
  switch (Self->type)
  {
#ifdef M_OS2
  case TYP_NP:
    Self->LastError = DosOpen(
      Self->name,                 /* pipe name, includes the name of remote */
      &Self->u.np.pipe,           /* handle returned */
      &Self->u.np.DosOpenAction,  /* open action returned */
      0,                          /* file size for named pipe */
      FILE_NORMAL,                /* read/write */
      FILE_OPEN,                  /* open if pipe exists, fail otherwise */
      PIPE_OPENMODE_CLIENT,       /* open mode flags */
      0);
    if (Self->LastError)
    {
      Self->errtyp = errtypNp;
      return ERROR_RET;
    }
    return 0;
  case TYP_SHM:
    return Os2ShmOpen(Self);
  case TYP_SOCKET:
    /*  Open a Socket */
    if ((Self->u.socket.sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
      Self->errtyp = errtypSocket;
      Self->LastError = errno;
      return ERROR_RET;
    }
    if (connect(Self->u.socket.sd,
                                (struct sockaddr*)&Self->u.socket.sin,
                                sizeof(Self->u.socket.sin)) != SUCCESS)
    {
      Self->errtyp = errtypSocket;
      Self->LastError = errno;
      return ERROR_RET;
    }
    return 0;
#endif
#ifdef M_AIX
  case TYP_SHM:
    return AixShmOpen(Self);
  case TYP_SOCKET:
    /*  open a socket, connect to the server */
    if ((Self->u.socket.sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
      Self->errtyp = errtypSocket;
      Self->LastError = errno;
      return ERROR_RET;
    }
    if (connect(Self->u.socket.sd,
       (struct sockaddr*)&Self->u.socket.sin, sizeof(Self->u.socket.sin)) == -1)
    {
      Self->errtyp = errtypSocket;
      Self->LastError = errno;
      return ERROR_RET;
    }
    return 0;
#endif
  default:
    assert(default_reached);
  }
#pragma warn -rvl
}
#pragma warn .rvl

int ClientTransact(ClientThis Self, PBuffer bufOut, PBuffer bufIn)
{
  int rc;

  Self->errtyp = errtypNone;
  switch (Self->type)
  {
#ifdef M_OS2
  case TYP_NP:
    Self->LastError = DosTransactNPipe(Self->u.np.pipe,
      bufOut->p, bufOut->BytesToWrite,
      bufIn->p, bufIn->BytesToRead, &bufIn->BytesRead);
    if (Self->LastError)
    {
      Self->errtyp = errtypNp;
      return ERROR_RET;
    }
    bufOut->BytesWritten = bufOut->BytesToWrite;  /*  since we don't really know */
    return 0;
  case TYP_SHM:
  case TYP_SOCKET:
    rc = ClientWrite(Self, bufOut);
    if (rc)
      return rc;
    return ClientRead(Self, bufIn);
#endif
#ifdef M_AIX
  case TYP_SHM:
  case TYP_SOCKET:
    rc = ClientWrite(Self, bufOut);
    if (rc)
      return rc;
    return ClientRead(Self, bufIn);
#endif
  default:
    assert(default_reached);
  }
#pragma warn -rvl
}
#pragma warn .rvl

int ClientWrite(ClientThis Self, PBuffer buf)
{
  Self->errtyp = errtypNone;
  switch (Self->type)
  {
#ifdef M_OS2
  case TYP_NP:
    return Os2NpWrite(Self, buf);
  case TYP_SHM:
    return Os2ShmWrite(Self, buf);
  case TYP_SOCKET:
    return Os2SockWrite(Self, buf);
#endif
#ifdef M_AIX
  case TYP_SHM:
    return AixShmWrite(Self, buf);
  case TYP_SOCKET:
    return AixSockWrite(Self, buf);
#endif
  default:
    assert(default_reached);
  }
#pragma warn -rvl
}
#pragma warn .rvl

int ClientRead(ClientThis Self, PBuffer buf)
{
  Self->errtyp = errtypNone;
  switch (Self->type)
  {
#ifdef M_OS2
  case TYP_NP:
    return Os2NpRead(Self, buf);
  case TYP_SHM:
    return Os2ShmRead(Self, buf);
  case TYP_SOCKET:
    return Os2SockRead(Self, buf);
#endif
#ifdef M_AIX
  case TYP_SHM:
    return AixShmRead(Self, buf);
  case TYP_SOCKET:
    return AixSockRead(Self, buf);
#endif
  default:
    assert(default_reached);
  }
#pragma warn -rvl
}
#pragma warn .rvl

int ClientClose(ClientThis Self)
{
  Self->errtyp = errtypNone;
  switch (Self->type)
  {
#ifdef M_OS2
  case TYP_NP:
    Self->LastError = DosClose(Self->u.np.pipe);
    if (Self->LastError)
    {
      Self->errtyp = errtypNp;
      return ERROR_RET;
    }
    return 0;
  case TYP_SHM:
    if (Self->u.shm.hasAccess)
    {
      Self->u.shm.hasAccess = 0;
      return DosReleaseMutexSem(Self->u.shm.AccessSem);
    }
    return 0;
  case TYP_SOCKET:
          soclose(Self->u.socket.sd);
#endif
    return 0;
#ifdef M_AIX
  case TYP_SHM:
    if (shmdt(Self->u.shm.start))
    {
      Self->LastError = errno;
      return ERROR_RET;
    }
    return 0;
  case TYP_SOCKET:
          close(Self->u.socket.sd);
    return 0;
#endif
  default:
    assert(default_reached);
  }
#pragma warn -rvl
}
#pragma warn .rvl

int ClientDone(ClientThis Self)
{
#ifdef M_OS2
  int rc;
#endif
  int ret = 0;
  ErrTyp = errtypNone;
  switch (Self->type)
  {
#ifdef M_OS2
  case TYP_NP:
    break;
  case TYP_SHM:
    if (Self->u.shm.hasAccess)
    {
      Self->u.shm.hasAccess = 0;
      return DosReleaseMutexSem(Self->u.shm.AccessSem);
    }
    rc = DosFreeMem(Self->u.shm.Start);
    if (!rc) rc = DosCloseMutexSem(Self->u.shm.AccessSem);
    if (!rc) rc = DosCloseEventSem(Self->u.shm.ServWriteSem);
    if (!rc) rc = DosCloseEventSem(Self->u.shm.ServReadSem);
    if (!rc) rc = DosCloseEventSem(Self->u.shm.CliWriteSem);
    if (!rc) rc = DosCloseEventSem(Self->u.shm.CliReadSem);
    if (rc)
    {
      ErrTyp = errtypShm;
      ErrorNo = rc;
      ret = ERROR_RET;
    }
    break;
  case TYP_SOCKET:
    break;
#endif
#ifdef M_AIX
  case TYP_SHM:
    if (shmdt(Self->u.shm.start))
    {
      ErrTyp = errtypShm;
      ErrorNo = errno;
      ret = ERROR_RET;
    }
    break;
  case TYP_SOCKET:
    break;
#endif
  default:
    assert(default_reached);
  }
  free(Self->name);
  free(Self);
  return ret;
}

int ClientErrno(ClientThis Self, eErrTyp* errtyp)
{
  return GetErrorNo(Self, errtyp);
}

void ClientDisplayErr(ClientThis Self)
{
  DisplayError(Self);
}

static struct ClientServer*  CliServInit(int isServer,
  char *name, long size, long timeout)
{
  int parts;
  char* names[MAXPARTS];
  struct ClientServer* Self;
  int type;                /*  store type of connection */
  char buf[DIRMAX_PATH];   /*  pipe name for named pipes */
                           /*  or application name for shared memory */
                           /*  or <host name>:<service name> for sockets */
#ifdef M_AIX
  int key;
#endif

  buf[0] = 0;
  ErrTyp = errtypNone;

  /*  parse the name into pieces */
  parts = ParseName(name, ':', names);

  if (!parts)
    goto bad_name_label;

  /*  lookup the type */
#ifdef M_OS2
  if (strieq(names[0], "pipe"))
  {
    type = TYP_NP;
    if (parts != 3)
      goto bad_name_label;

    /*  for the client insert the machine name for remote pipes */
    if (*names[1] && !isServer)
    {
      strcat(buf, "\\\\");
      strcat(buf, names[1]);
    }
    strcat(buf, "\\pipe\\");
    strcat(buf, names[2]);
  }
  else if (strieq(names[0], "shm"))
  {
    type = TYP_SHM;
    if (parts != 2)
      goto bad_name_label;

    if (!*names[1])
      goto bad_name_label;
    strcat(buf, names[1]);
  }
  else if (strieq(names[0], "socket"))
  {
    type = TYP_SOCKET;
    if (parts != 3)
      goto bad_name_label;
    strcat(buf, names[1]);
    strcat(buf, ":");
    strcat(buf, names[2]);
  }
#endif
#ifdef M_AIX
  if (strieq(names[0], "shm"))
  {
    type = TYP_SHM;
    if (parts != 3)
      goto bad_name_label;

    if (!*names[1])
      goto bad_name_label;
    key = atoi(names[1]);
    strcat(buf, names[2]);
  }
  else if (strieq(names[0], "socket"))
  {
    type = TYP_SOCKET;
    if (parts != 3)
      goto bad_name_label;
    strcat(buf, names[1]);
    strcat(buf, ":");
    strcat(buf, names[2]);
  }
#endif
  else
    goto bad_name_label;

  /*  allocate a control block */
  Self = (ClientServer *) calloc(1, sizeof(*Self));

  /*  store the name */
  if (Self == 0 || (Self->name = strdup(buf)) == 0)
  {
    if (Self)
      free(Self);
    ErrTyp = errtypOther;
    ErrorNo = ERR_MALLOC;
    return 0;
  }

  switch (type)
  {
#ifdef M_OS2
  case TYP_NP:
    if (Os2NpInit(Self, isServer, buf, size, timeout))
    {
      free(Self->name);
      free(Self);
      return 0;
    }
    break;
  case TYP_SHM:
    if (Os2ShmInit(Self, isServer, buf, size, timeout))
    {
      free(Self->name);
      free(Self);
      return 0;
    }
    break;
  case TYP_SOCKET:
    if (Os2SockInit(Self, isServer, names[1], names[2], size, timeout))
    {
      free(Self->name);
      free(Self);
      return 0;
    }
    break;
#endif
#ifdef M_AIX
  case TYP_SHM:
    if (AixShmInit(Self, isServer, buf, key, size, timeout))
    {
      free(Self);
      free(Self->name);
      return 0;
    }
    break;
  case TYP_SOCKET:
    if (AixSockInit(Self, isServer, names[1], names[2], size, timeout))
    {
      free(Self);
      free(Self->name);
      return 0;
    }
  break;
#endif
  }

  return Self;

bad_name_label:
  ErrTyp = errtypOther;
  ErrorNo = ERR_BADNAME;
  return 0;
}

static int ParseName(char *name, char sep, char *names[])
{
  /*  --------------------------------------------------------- */
  /*  returns 0 if name does not start with a separator */
  /*  else splits into substrings on the separator and returns */
  /*  the number of parts - at least one */
  /*  --------------------------------------------------------- */
  int i;
  char* seppos;
  /*  .skip the first separator */
  if (*name++ != sep)
    return 0;
  for (i = 0; i < MAXPARTS; i++)
  {
    seppos = strchr(name, sep);
    if (!seppos)
    {
      names[i++] = name;
      break;
    }
    *seppos = 0;
    names[i] = name;
    name = seppos + 1;
  }
  return i;
}

static int GetErrorNo(struct ClientServer* Self, eErrTyp* errtyp)
{
  if (!Self)
  {
    *errtyp = ErrTyp;
    return ErrorNo;
  }
  else
  {
    *errtyp = Self->errtyp;
    return Self->LastError;
  }
}

static void DisplayError(struct ClientServer* Self)
{
  eErrTyp errtyp;
  int error;

  error = GetErrorNo(Self, &errtyp);
  switch (errtyp)
  {
  case errtypNone:
    fprintf(stderr,"No error\n");
    return;
  case errtypOther:
    fprintf(stderr,"Other: ");
    switch (error)
    {
    case ERR_GENERAL:
      fprintf(stderr,"General error.\n");
      break;
    case ERR_MALLOC:
      fprintf(stderr,"Memory allocation error.\n");
      break;
    case ERR_TIMEOUT:
      fprintf(stderr,"Timeout error.\n");
      break;
    case ERR_WRONGSERVER:
      fprintf(stderr,"Incorrect server error.\n");
      break;
    case ERR_NOHOST:
      fprintf(stderr,"No such host.\n");
      break;
    case ERR_HOSTTRYAGAIN:
      fprintf(stderr,"Host busy. Try again later.\n");
      break;
    case ERR_HOSTNORECOVERY:
      fprintf(stderr,"DNS error for host.\n");
      break;
    case ERR_HOSTNOADDRESS:
      fprintf(stderr,"No IP address for host.\n");
      break;
    }
    return;
  case errtypNp:
    fprintf(stderr,"Named pipe: \n");
    return;
  case errtypShm:
    fprintf(stderr,"Shared memory: ");
    break;
  case errtypSem:
    fprintf(stderr,"Semaphore: ");
    break;
  case errtypSocket:
    fprintf(stderr,"Socket: ");
    break;
  }

#ifdef M_AIX
  fprintf(stderr,"%s", errnomsg(Self->LastError));
#else
  fprintf(stderr,"%ld", Self->LastError);
#endif
}

#ifdef M_OS2

static int Os2NpInit(struct ClientServer* Self, int isServer,
  char *PipeName, long size, long timeout)
{
  int rc;
  HPIPE pipe;

  if (isServer)
  {
    rc = DosCreateNPipe(
      PipeName,
      &pipe,
      PIPE_OPENMODE_SERVER,
      PIPEMODE,
      size > OUTBUFSIZE ? size : OUTBUFSIZE,
      size > INBUFSIZE  ? size : INBUFSIZE,
      Self->timeout);
    if (rc)
    {
      ErrTyp = errtypNp;
      ErrorNo = rc;
      return ERROR_RET;
    }
  }
  Self->type      = TYP_NP;
  Self->isServer  = isServer;
  Self->Size      = size;
  Self->timeout   = timeout;
  Self->u.np.pipe = isServer ? pipe : 0;
  return 0;
}

static int Os2NpRead(struct ClientServer* Self, PBuffer buf)
{
  Self->LastError = DosRead(Self->u.np.pipe,
    buf->p, buf->BytesToRead, &buf->BytesRead);
  if (Self->LastError)
  {
    Self->errtyp = errtypNp;
    return ERROR_RET;
  }
  return 0;
}

static int Os2NpWrite(struct ClientServer* Self, PBuffer buf)
{
  Self->LastError = DosWrite(Self->u.np.pipe,
    buf->p, buf->BytesToWrite, &buf->BytesWritten);
  if (Self->LastError)
  {
    Self->errtyp = errtypNp;
    return ERROR_RET;
  }
  return 0;
}

static void Os2NpWaitForClose(struct ClientServer* Self,
  PBYTE pBuffer, ULONG cBuffer )
{
  ULONG State;
  ULONG cbRead;
  AVAILDATA Available;
  SHORT Count;
  Count = 0;

  do
  {
    DosPeekNPipe(Self->u.np.pipe, pBuffer, cBuffer,
      &cbRead, &Available, &State);
    if ( State == NP_CLOSING )
      break;
    DosSleep(10L);
    Count++;
  }
  while (Count < 1000);
}
/* -------------------------- */
/*  OS2 shared memory */
/* -------------------------- */

static int Os2ShmInit(struct ClientServer* Self, int isServer,
                    char *SegName, long size, long timeout)
{
  int rc, aSize = size+sizeof(tShmHeader);
  char  *psz;
  char  SegFullName[DIRMAX_PATH] = "\\SHAREMEM\\";
  char  SemName[DIRMAX_PATH] = "\\SEM32\\";
  pShmHeader ShmHeader=0;

  Self->type         = TYP_SHM;
  Self->isServer   = isServer;
  Self->timeout    = timeout==0?-1:timeout;
  Self->Size       = aSize > SHMBUFSIZE ? aSize : SHMBUFSIZE;
  Self->u.shm.hasAccess = 0;
  if (isServer)
  {
    strcat(SegFullName, SegName);
    rc = DosAllocSharedMem((PPVOID)&ShmHeader, SegFullName,
                          Self->Size,
                          PAG_COMMIT|PAG_READ|PAG_WRITE);
    if (rc)
      goto Error;
    Self->u.shm.Start = (char*)ShmHeader;
    ShmHeader->Size = Self->Size;
    strcat(SemName, SegName);
    strcat(SemName, ".SW");
    rc = DosCreateEventSem(SemName, &Self->u.shm.ServWriteSem, 0, 0);
    if (rc)
      goto Error;
    psz = strchr(SemName, '.');
    strcpy(psz, ".SR");
    rc = DosCreateEventSem(SemName, &Self->u.shm.ServReadSem, 0, 0);
    if (rc)
      goto Error;
    psz = strchr(SemName, '.');
    strcpy(psz, ".CR");
    rc = DosCreateEventSem(SemName, &Self->u.shm.CliReadSem, 0, 0);
    if (rc)
       goto Error;
    psz = strchr(SemName, '.');
    strcpy(psz, ".CW");
    rc = DosCreateEventSem(SemName, &Self->u.shm.CliWriteSem, 0, 1);
    if (rc)
      goto Error;
    psz = strchr(SemName, '.');
    strcpy(psz, ".ACC");
    rc = DosCreateMutexSem(SemName, &Self->u.shm.AccessSem, 0, 0);
    if (rc)
      goto Error;
  }
  else          /*  client init */
  {
    strcat(SegFullName, SegName);
    rc = DosGetNamedSharedMem((PPVOID)&ShmHeader,
                              SegFullName,
                              PAG_READ|PAG_WRITE);
    if (rc)
      goto Error;
    Self->u.shm.Start = (char*)ShmHeader;
    strcat(SemName, SegName);
    strcat(SemName, ".SW");
    rc = DosOpenEventSem(SemName, &Self->u.shm.ServWriteSem);
    if (rc)
      goto Error;
    psz = strchr(SemName, '.');
    strcpy(psz, ".SR");
    rc = DosOpenEventSem(SemName, &Self->u.shm.ServReadSem);
    if (rc)
      goto Error;
    psz = strchr(SemName, '.');
    strcpy(psz, ".CR");
    rc = DosOpenEventSem(SemName, &Self->u.shm.CliReadSem);
    if (rc)
      goto Error;
    psz = strchr(SemName, '.');
    strcpy(psz, ".CW");
    rc = DosOpenEventSem(SemName, &Self->u.shm.CliWriteSem);
    if (rc)
      goto Error;
    psz = strchr(SemName, '.');
    strcpy(psz, ".ACC");
    rc = DosOpenMutexSem(SemName, &Self->u.shm.AccessSem);
    if (rc)
      goto Error;
  }
  return 0;
Error:
  ErrTyp = errtypShm;
  ErrorNo = rc;
  return ERROR_RET;
}

static int Os2ShmOpen(struct ClientServer* Self)
{
  if (!Self->isServer)
  {
    Self->LastError = DosRequestMutexSem(Self->u.shm.AccessSem, Self->timeout);
    if (Self->LastError)
    {
      ErrTyp = errtypShm;
      return ERROR_RET;
    }
    Self->u.shm.hasAccess = 1;
  }
  return 0;
}

static int Os2ShmRead(struct ClientServer* Self, PBuffer buf)
{
  pShmHeader ShmHeader = (pShmHeader)Self->u.shm.Start;
  char* start = ShmHeader->Data;
  ulong Count;
  if (Self->isServer)
  {
    Self->LastError = DosWaitEventSem(Self->u.shm.ServReadSem, Self->timeout);
    if (Self->LastError)
      goto Error;
    Self->LastError = DosResetEventSem(Self->u.shm.ServReadSem, &Count);
    if (Self->LastError)
      goto Error;
	buf->BytesRead = (std::min)(ShmHeader->Size, buf->BytesToRead);
    memcpy(buf->p, start, buf->BytesRead);
    Self->LastError = DosPostEventSem(Self->u.shm.ServWriteSem);
    if (Self->LastError)
      goto Error;
  }
  else
  {
    Self->LastError = DosWaitEventSem(Self->u.shm.CliReadSem, Self->timeout);
    if (Self->LastError)
      goto Error;
    Self->LastError = DosResetEventSem(Self->u.shm.CliReadSem, &Count);
    if (Self->LastError)
      goto Error;
    buf->BytesRead = (std::min)(ShmHeader->Size, buf->BytesToRead);
    memcpy(buf->p, start, buf->BytesRead);
    Self->LastError = DosPostEventSem(Self->u.shm.CliWriteSem);
    if (Self->LastError)
      goto Error;
  }
  return 0;
Error:
  ErrTyp = errtypShm;
  return ERROR_RET;
}

static int Os2ShmWrite(struct ClientServer* Self, PBuffer buf)
{
  pShmHeader ShmHeader = (pShmHeader)Self->u.shm.Start;
  char* start = ShmHeader->Data;
  ulong Count;
  if (Self->isServer)
  {
    Self->LastError = DosWaitEventSem(Self->u.shm.ServWriteSem, Self->timeout);
    if (Self->LastError)
      goto Error;
    Self->LastError = DosResetEventSem(Self->u.shm.ServWriteSem, &Count);
    if (Self->LastError)
      goto Error;
    ShmHeader->Size = (std::min)(Self->Size, buf->BytesToWrite);
    memcpy(start, buf->p, ShmHeader->Size);
    buf->BytesWritten = ShmHeader->Size;
    Self->LastError = DosPostEventSem(Self->u.shm.CliReadSem);
    if (Self->LastError)
      goto Error;
  }
  else
  {
    Self->LastError = DosWaitEventSem(Self->u.shm.CliWriteSem, Self->timeout);
    if (Self->LastError)
      goto Error;
    Self->LastError = DosResetEventSem(Self->u.shm.CliWriteSem, &Count);
    if (Self->LastError)
      goto Error;
    ShmHeader->Size = (std::min)(Self->Size, buf->BytesToWrite);
    memcpy(start, buf->p, ShmHeader->Size);
    buf->BytesWritten = ShmHeader->Size;
    Self->LastError = DosPostEventSem(Self->u.shm.ServReadSem);
    if (Self->LastError)
      goto Error;
  }
  return 0;
Error:
  ErrTyp = errtypShm;
  return ERROR_RET;
}

/*========================================================================== */
/* Socket API */
/*========================================================================== */

extern int h_errno;     /* Used by Sockets gethostbyname() function */
/*  currently igoring the timeout value */
static int Os2SockInit(struct ClientServer* Self, int isServer, char* host,
                                           char* service, long size, long timeout)
{
  struct servent __far16 * ps;         /* service entry */
  struct hostent __far16 * ph;         /* host entry    */

  Self->type      = TYP_SOCKET;
  Self->isServer  = isServer;
  Self->timeout   = timeout;
  Self->Size      = size;

  /* Initialise Sockets */
  if (sock_init())
        goto error_label;

  /* Find the internet address of the host */
  if ((ph = gethostbyname(host))==0)
    goto hosterror_label;

  /*  Get port no based on service */
  if ((ps = getservbyname(service, PROTOCOL))==0)
    goto error_label;

  if (isServer)
  {
    /*  Open a Socket */
    if ((Self->u.socket.sockno = socket(AF_INET, SOCK_STREAM, 0)) == -1)
      goto error_label;

    Self->u.socket.sin.sin_family                 = AF_INET;
    Self->u.socket.sin.sin_port                     = ps->s_port;
    Self->u.socket.sin.sin_addr.s_addr  = INADDR_ANY;

    /* Bind address to local end of socket */
    if (bind(Self->u.socket.sockno,
           (struct sockaddr __far16*)&Self->u.socket.sin,
                 sizeof(Self->u.socket.sin)) == -1)
      goto error_label;

    /* Accept Connections from remote clients */
    if (listen(Self->u.socket.sockno, SOCKET_QSIZE) == -1)
      goto error_label;
  }
  else
  {
    Self->u.socket.sin.sin_family                 = AF_INET;
        Self->u.socket.sin.sin_port                 = ps->s_port;
          Self->u.socket.sin.sin_addr.s_addr    = *((pulong)ph->h_addr);
  }

  goto ok_label;

error_label:
  ErrTyp = errtypSocket;
  ErrorNo = errno;
  return ERROR_RET;

hosterror_label:
  ErrTyp = errtypOther;
  switch (h_errno)
  {
    case HOST_NOT_FOUND:
      ErrorNo = ERR_NOHOST;
      break;
    case TRY_AGAIN:
      ErrorNo = ERR_HOSTTRYAGAIN;
      break;
    case NO_RECOVERY:
      ErrorNo = ERR_HOSTNORECOVERY;
      break;
    case NO_ADDRESS:
      ErrorNo = ERR_HOSTNOADDRESS;
      break;
    default:
      ErrorNo = ERR_GENERAL;
      break;
  }
  return ERROR_RET;

ok_label:
  return 0;
}

static int _recvall(int session, pchar buff, long size)
{
  int BytesRead=0, n;

  while (BytesRead < size)
  {
    n = recv(session, buff, size - BytesRead, 0);
    if (n <= 0)
      return n;
    BytesRead += n;
    buff += n;
  }
  return BytesRead;
}

static int Os2SockRead(struct ClientServer* Self, pBuffer buf)
{
  int n,excess;
  ulong length;

  n = _recvall(Self->u.socket.sd, (char*)&length, sizeof(length));
  if (n != sizeof(buf->BytesRead))
  {
    Self->errtyp = errtypSocket;
    Self->LastError = errno;
    return ERROR_RET;
  }
  buf->BytesRead = ntohl(length);
  excess = buf->BytesRead - buf->BytesToRead;
  if (excess > 0)
    buf->BytesRead -= excess;
  n = _recvall(Self->u.socket.sd, buf->p, buf->BytesRead);
  if (n != buf->BytesRead)
  {
    Self->errtyp = errtypSocket;
    Self->LastError = errno;
    return ERROR_RET;
  }
  if (excess > 0)
  {
    pchar p = (pchar) malloc(excess);
    n = _recvall(Self->u.socket.sd, p, excess);
    if (n != excess)
    {
      Self->errtyp = errtypSocket;
      Self->LastError = errno;
      return ERROR_RET;
    }
    free(p);
  }
  return 0;
}

static int Os2SockWrite(struct ClientServer* Self, pBuffer buf)
{
  ulong length = htonl(buf->BytesToWrite);
  /*  write the length to socket */
  if (send(Self->u.socket.sd, (char *)&length, sizeof(length), 0)
        != sizeof(length))
  {
    Self->errtyp = errtypSocket;
    Self->LastError = errno;
    return ERROR_RET;
  }
  /*  write the message */
  buf->BytesWritten = send(Self->u.socket.sd, buf->p, buf->BytesToWrite, 0);
  if (buf->BytesWritten != buf->BytesToWrite)
  {
    Self->errtyp = errtypSocket;
    Self->LastError = errno;
    return ERROR_RET;
  }
  return 0;
}

#endif

#ifdef M_AIX
static void _semzero(struct ClientServer* Self)
{
  static unsigned short zero_sem[4];
  semctl(Self->u.shm.semid, 0, SETALL, zero_sem);
}

/*  assumes sops array has IPC_NOWAIT flag set already for timeout != 0 */
static int _semop_tmout(int semid, struct sembuf *sops, int nops, long timeout)
{
  if (!timeout)
  {
    return semop(semid, sops, nops);
  }
  else
  {
    int nsleeps;
    for (nsleeps = timeout / TIMEOUT_TICK;
         nsleeps;
         nsleeps--, timeout -= TIMEOUT_TICK)
    {
      if (semop(semid, sops, nops))
        return 0;
      Delay(TIMEOUT_TICK);
    }
    return ERR_TIMEOUT;
  }
}

static int AixShmInit(struct ClientServer* Self, int isServer,
  char* AppName, short key, long size, long timeout)
{
  int id, semid;
  key_t Key = SHM_KEY_BASE + key;
  int rc;

  printf("initializing %s %s, key = %d, size = %ld, timeout = %ld\n",
    isServer ? "server" : "client", AppName, key, size, timeout);

  assert(strlen(AppName) < SHM_NAMELEN);

  if (timeout)
  {
    incServRead.sem_flg |= IPC_NOWAIT;
    decServRead.sem_flg |= IPC_NOWAIT;
    incServWrite.sem_flg |= IPC_NOWAIT;
    decServWrite.sem_flg |= IPC_NOWAIT;
    incClientRead.sem_flg |= IPC_NOWAIT;
    decClientRead.sem_flg |= IPC_NOWAIT;
    incClientWrite.sem_flg |= IPC_NOWAIT;
    decClientWrite.sem_flg |= IPC_NOWAIT;
  }

  id = shmget(Key, size + sizeof(shmHeader),
    isServer ? FLAGS | IPC_CREAT | IPC_EXCL : FLAGS);
  if (id == -1)
  {
    if (!isServer || errno != EEXIST)
      goto Error;
    else
    {
      /*  server and shared memory exists */
      /*  so delete it, and recreate it */

      /* .first get the id - use 1 for size to avoid an error */
      id = shmget(Key, 1, FLAGS);

      /* .now delete the memory */
      if (id == -1 || shmctl(id, IPC_RMID, 0))
        goto shmerror_label;

      /* .and recreate the memory */
      id = shmget(Key, size + sizeof(shmHeader), FLAGS | IPC_CREAT | IPC_EXCL);
      if (id == -1)
        goto shmerror_label;
    }
  }

  goto shmok_label;

shmerror_label:
  ErrTyp = errtypShm;
  ErrorNo = errno;
  return ERROR_RET;

shmok_label:

  /*  get id of semaphore group */
  semid = semget(Key, NO_SEMS,
    isServer ? FLAGS | IPC_CREAT | IPC_EXCL : FLAGS);
  if (semid == -1)
  {
    if (!isServer || errno != EEXIST)
      goto Error;
    else
    {
      /*  server and semaphore exists */
      /*  so delete it, and recreate it */

      /* .first get the semaphore id */
      semid = semget(Key, NO_SEMS, FLAGS);
      if (semid == -1)
        goto Error;

      /* .now remove semaphore group */
      if (semctl(semid, 0, IPC_RMID, 0))
        goto Error;

      /* .and recreate semaphore group */
      semid = semget(Key, NO_SEMS, FLAGS | IPC_CREAT | IPC_EXCL);
      if (semid == -1)
        goto Error;
    }
  }

  goto semok_label;

Error:
  ErrTyp = errtypSem;
  ErrorNo = errno;
  return ERROR_RET;

semok_label:

  if (isServer)
  {
    /*  set semaphore ready for client to write */
    rc = _semop_tmout(semid, &incClientWrite, 1, timeout);
    if (rc == ERR_TIMEOUT)
    {
      ErrTyp = errtypOther;
      ErrorNo = ERR_TIMEOUT;
      semctl(semid, 0, IPC_RMID, 0);
      return ERROR_RET;
    }
    else if (rc == -1)
    {
      ErrTyp = errtypSem;
      ErrorNo = errno;
      semctl(semid, 0, IPC_RMID, 0);
      return ERROR_RET;
    }
  }

  Self->type           = TYP_SHM;
  Self->isServer       = isServer;
  Self->u.shm.key      = Key;
  Self->u.shm.id       = id;
  Self->u.shm.semid    = semid;
  Self->Size           = size;
  Self->u.shm.pid      = getpid();
  Self->timeout        = timeout;
  Self->u.shm.validSem = 1;

  return 0;
}

static int AixShmOpen(struct ClientServer* Self)
{
  char* start;
  shmHeader* head;

  start = (char*)shmat(Self->u.shm.id, 0, 0);
  if ((int)start == -1)
  {
    Self->errtyp = errtypShm;
    Self->LastError = errno;
    return ERROR_RET;
  }
  head = (shmHeader*)start;

  if (Self->isServer)
    strcpy(head->name, Self->name);
  else if (strcmp(head->name, Self->name))
  {
    Self->errtyp = errtypOther;
    Self->LastError = ERR_WRONGSERVER;
    return ERROR_RET;
  }
  Self->u.shm.start = start;
  return 0;
}


static int AixShmRead(struct ClientServer* Self, PBuffer buf)
{
  shmHeader* head = (shmHeader*)Self->u.shm.start;
  char* start     = Self->u.shm.start + sizeof(shmHeader);
  int rc;
  int semid;

  if (!Self->isServer && !Self->u.shm.validSem)
  {
    Delay(Self->timeout);
    semid = semget(Self->u.shm.key, NO_SEMS, FLAGS);
    if (semid == -1)
    {
      ErrTyp = errtypSem;
      ErrorNo = errno;
      return ERROR_RET;
    }
  }

  rc = _semop_tmout(Self->u.shm.semid,
    Self->isServer ? &decServRead : &decClientRead, 1, Self->timeout);
  if (rc == ERR_TIMEOUT)
  {
    Self->errtyp = errtypOther;
    Self->LastError = ERR_TIMEOUT;
    return ERROR_RET;
  }
  else if (rc == -1)
  {
    Self->LastError = errno;
    if (!Self->isServer && Self->LastError) /*  server removed semaphores */
      Self->u.shm.validSem = 0;
    Self->errtyp = errtypSem;

    return ERROR_RET;
  }

  /*  semaphores should now be in state 0,0,0,0 unless crashes have happened */
  /*  so in we defensively set them to 0,0,0,0 */
  _semzero(Self);
  buf->BytesRead = (std::min)(head->size, long(buf->BytesToRead));
  memcpy(buf->p, start, buf->BytesRead);
  Self->LastError = 0;
  if (Self->isServer)
     head->sid = Self->u.shm.pid;
  else
  {
     head->sid = NON_PID;
     head->cid = NON_PID;
  }

  rc = _semop_tmout(Self->u.shm.semid,
    Self->isServer ? &incServWrite : &incClientWrite, 1, Self->timeout);
  if (rc == ERR_TIMEOUT)
  {
    Self->errtyp = errtypOther;
    Self->LastError = ERR_TIMEOUT;
    return ERROR_RET;
  }
  else if (rc == -1)
  {
    Self->LastError = errno;
    if (!Self->isServer && Self->LastError == EIDRM) /*  server removed semaphores */
      Self->u.shm.validSem = 0;
    Self->errtyp = errtypSem;
    return ERROR_RET;
  }
  return 0;
}

static int AixShmWrite(struct ClientServer* Self, PBuffer buf)
{
  shmHeader* head = (shmHeader*)Self->u.shm.start;
  char* start     = Self->u.shm.start + sizeof(shmHeader);
  int rc;
  int semid;

  if (!Self->isServer && !Self->u.shm.validSem)
  {
    Delay(Self->timeout);
    semid = semget(Self->u.shm.key, NO_SEMS, FLAGS);
    if (semid == -1)
    {
      ErrTyp = errtypSem;
      ErrorNo = errno;
      return ERROR_RET;
    }
  }

  rc = _semop_tmout(Self->u.shm.semid,
    Self->isServer ? &decServWrite : &decClientWrite, 1, Self->timeout);
  if (rc == ERR_TIMEOUT)
  {
    Self->errtyp = errtypOther;
    Self->LastError = ERR_TIMEOUT;
    return ERROR_RET;
  }
  else if (rc == -1)
  {
    Self->LastError = errno;
    if (!Self->isServer && Self->LastError == EIDRM) /*  server removed semaphores */
      Self->u.shm.validSem = 0;
    Self->errtyp = errtypSem;
    return ERROR_RET;
  }

  /*  semaphores should now be in state 0,0,0,0 unless crashes have happened */
  /*  so in we defensively set them to 0,0,0,0 */
  _semzero(Self);
  head->size = (std::min)(Self->Size, long(buf->BytesToWrite));
  memcpy(start, buf->p, head->size);
  if (!Self->isServer)
     head->cid = Self->u.shm.pid;
  buf->BytesWritten = head->size;
  Self->LastError = 0;

  rc = _semop_tmout(Self->u.shm.semid,
    Self->isServer ? &incClientRead : &incServRead, 1, Self->timeout);
  if (rc == ERR_TIMEOUT)
  {
    Self->errtyp = errtypOther;
    Self->LastError = ERR_TIMEOUT;
    return ERROR_RET;
  }
  else if (rc == -1)
  {
    Self->LastError = errno;
    if (!Self->isServer && Self->LastError == EIDRM) /*  server removed semaphores */
      Self->u.shm.validSem = 0;
    Self->errtyp = errtypSem;
    return ERROR_RET;
  }

  return 0;
}

/*  currently igoring the timeout value */
static int AixSockInit(struct ClientServer* Self, int isServer,
  char* host, char* service, long size, long timeout)
{
  struct servent *ps;         /*  service entry */
  struct hostent *ph;         /*  host entry */

  printf("initializing %s %s, service = %s, size = %ld, timeout = %ld\n",
    isServer ? "server" : "client", host, service, size, timeout);

  Self->type      = TYP_SOCKET;
  Self->isServer  = isServer;
  Self->timeout   = timeout;
  Self->Size      = size + sizeof(unsigned short);   /*  addition for size */

  /*  get port no based on service */
  if (!(ps = getservbyname(service, PROTOCOL)))
    goto error_label;
  Self->u.socket.sin.sin_port = ps->s_port;

  if (isServer)
  {
    Self->u.socket.sin.sin_addr.s_addr = INADDR_ANY;

    /*  open a socket */
    if ((Self->u.socket.sockno = socket(AF_INET, SOCK_STREAM, 0)) == -1)
      goto error_label;

    /*  bind address to local end of socket */
    if (bind(Self->u.socket.sockno,
      (struct sockaddr*)&Self->u.socket.sin,
      sizeof(Self->u.socket.sin)) == -1)
      goto error_label;

    /*  accept connections from remote clients */
    if (listen(Self->u.socket.sockno, SOCKET_QSIZE) == -1)
      goto error_label;
  }
  else
  {
    /*  find the internet address of the host */
    if (!(ph = gethostbyname(host)))
      goto hosterror_label;
    Self->u.socket.sin.sin_family = ph->h_addrtype;
    memcpy(&Self->u.socket.sin.sin_addr, ph->h_addr, ph->h_length);
  }

  goto ok_label;

error_label:
  ErrTyp = errtypSocket;
  ErrorNo = errno;
  return ERROR_RET;

hosterror_label:
  ErrTyp = errtypOther;
  switch (h_errno)
  {
  case HOST_NOT_FOUND:
    ErrorNo = ERR_NOHOST;
    break;
  case TRY_AGAIN:
    ErrorNo = ERR_HOSTTRYAGAIN;
    break;
  case NO_RECOVERY:
    ErrorNo = ERR_HOSTNORECOVERY;
    break;
  case NO_ADDRESS:
    ErrorNo = ERR_HOSTNOADDRESS;
    break;
  default:
    ErrorNo = ERR_GENERAL;
    break;
  }
  return ERROR_RET;

ok_label:
  return 0;
}

static int _readall(int session, pchar buff, long size)
{
  int BytesRead=0, n;

  while (BytesRead < size)
  {
    n = read(session, buff, size - BytesRead);
    if (n <= 0)
      return n;
    BytesRead += n;
    buff += n;
  }
  return BytesRead;
}

static int AixSockRead(struct ClientServer* Self, PBuffer buf)
{
  int n,excess;
  ulong length;

  n = _readall(Self->u.socket.sd, (char*)&length, sizeof(length));
  if (n != sizeof(buf->BytesRead))
  {
    Self->errtyp = errtypSocket;
    Self->LastError = errno;
    return ERROR_RET;
  }
  buf->BytesRead = ntohl(length);
  excess = buf->BytesRead - buf->BytesToRead;
  if (excess > 0)
    buf->BytesRead -= excess;
  n = _readall(Self->u.socket.sd, buf->p, buf->BytesRead);
  if (n != buf->BytesRead)
  {
    Self->errtyp = errtypSocket;
    Self->LastError = errno;
    return ERROR_RET;
  }
  if (excess > 0)
  {
    pchar p = (pchar) malloc(excess);
    n = _readall(Self->u.socket.sd, p, excess);
    if (n != excess)
    {
      Self->errtyp = errtypSocket;
      Self->LastError = errno;
      return ERROR_RET;
    }
    free(p);
  }
  return 0;
}

static int AixSockWrite(struct ClientServer* Self, PBuffer buf)
{
  ulong length = htonl(buf->BytesToWrite);
  /*  write the length */
  if (write(Self->u.socket.sd, (char*)&length, sizeof(length))
    != sizeof(length))
  {
    Self->errtyp = errtypSocket;
    Self->LastError = errno;
    return ERROR_RET;
  }

  /*  write the message */
  buf->BytesWritten = write(Self->u.socket.sd, buf->p, buf->BytesToWrite);
  if (buf->BytesWritten != buf->BytesToWrite)
  {
    Self->errtyp = errtypSocket;
    Self->LastError = errno;
    return ERROR_RET;
  }

  return 0;
}

#endif

