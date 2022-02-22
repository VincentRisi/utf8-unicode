#ifndef _CLISERV_H_
#define _CLISERV_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_CLISERV_H_);

#ifdef M_OS2
extern "C" {
#include <types.h>
#include <utils.h>
#include <arpa\nameser.h>
#include <sys\socket.h>
#include <netdb.h>
#include <netinet\in.h>
}
#endif

#ifdef M_OS2
#ifndef _OSFUNCS_H_
#define INCL_DOS
#define INCL_DOSERRORS
#include "osfuncs.h"
#else
#ifndef INCL_DOS
#error OSFUNCS for cliserv must define INCL_DOS
#endif
#ifndef INCL_DOSERRORS
#error OSFUNCS for cliserv must define INCL_DOSERRORS
#endif
#endif
#endif

#ifdef M_OS2
#define INBUFSIZE  2048
#define OUTBUFSIZE 2048
#define SHMBUFSIZE 2048
#define INSTANCES 1
#define PIPE_OPENMODE_SERVER NP_ACCESS_DUPLEX | NP_NOWRITEBEHIND
#define PIPEMODE NP_WAIT | \
                 NP_READMODE_MESSAGE | \
                 NP_TYPE_MESSAGE | \
                 INSTANCES
#define PIPE_OPENMODE_CLIENT OPEN_ACCESS_READWRITE | \
                             OPEN_SHARE_DENYNONE | \
                             OPEN_FLAGS_FAIL_ON_ERROR
#endif

#ifdef M_AIX
#include <netinet/in.h>
#define SHM_KEY_BASE 5550000L
#define SHM_NAMELEN 16      /*  a multiple of 4 for alignment */
#define NON_PID -99
#define NO_SEMS 4
#define FLAGS 0666
#define TIMEOUT_TICK 25L

/*  don't do SEM_UNDO */
    #undef  SEM_UNDO
    #define SEM_UNDO 0
#endif

/*  socket stuff */
#define PROTOCOL "tcp"
#define SOCKET_QSIZE 5

/*  ------------------- Buffer structure for the client server API */
typedef struct tBuffer
{
  unsigned long MaxSize;
  unsigned long BytesToRead;
  unsigned long BytesRead;
  unsigned long BytesToWrite;
  unsigned long BytesWritten;
  char *p;
} tBuffer, *PBuffer, *pBuffer, Buffer;

/*  --------------------- errors */
//#define SUCCESS 0
#include "tpipe.h"

/*  int return values for prototypes below */
//#define SUCCESS     0
#define ERROR_RET  -1

/*  handle typedefs ------------------------ */
/*  type of connection */
enum  /*  server and client types */
{
  TYP_NP,                             /*  named pipe */
  TYP_SHM,                            /*  shared memory */
  TYP_SOCKET                          /*  sockets */
};

struct ClientServer
{
  char*   name;
  int     type;
  int     isServer;
  long    Size;
  long    timeout;             /*  msecs */
  eErrTyp errtyp;
  int     LastError;
  union
  {
#ifdef M_OS2
    struct
    {
      HPIPE pipe;
      unsigned long DosOpenAction;
    } np;
    struct
    {
      char *Start;
            HMTX AccessSem;
            HEV  ServWriteSem;
            HEV  ServReadSem;
            HEV  CliWriteSem;
            HEV  CliReadSem;
      int  hasAccess;
    } shm;
    struct
    {
      int sockno;                                       // TCP socket Descriptor
      struct sockaddr_in sin;   // Socket address
      int sd;                   // TCP socket Descriptor for active connection
    } socket;
#endif
#ifdef M_AIX
    struct
    {
      key_t key;
      int id;
      char* start;
      int semid;
      int pid;
      int validSem;
    } shm;
    struct
    {
      int sockno;                /*  TCP socket */
      struct sockaddr_in sin;    /*  socket address */
      int sd;                    /*  TCP connection */
    } socket;
#endif
  } u;
};

typedef struct ClientServer* ServerThis, tServerThis, *pServerThis;
typedef struct ClientServer* ClientThis, tClientThis, *pClientThis;

#ifdef M_OS2
typedef struct ShmHeader
{
  long Size;
  char  Data[1];
} tShmHeader, *pShmHeader;
#endif

#ifdef M_AIX
typedef struct shmHeader
{
  char name[SHM_NAMELEN];
  long size;
  int sid;      /*  server process id */
  int cid;      /*  client process id */
} shmHeader;

//extern int errno;
//extern int h_errno;
#endif

#define MAXPARTS 3    /*  maximum no. of parts in name passed to Init */

/*  constructors ---------------------------------------------------- */
/*  for named pipes Name the format is   ":pipe:<machine>:<pipe-name>" */
/*  for shared memory Name the format is ":shm:<key>:<app-name>"  AIX */
/*  or                                   ":shm:<seg-name>:<app-name>" OS2 */
/*  for sockets Name is of the format    ":socket:<host-name>:<service>" */
/*    where <service> must exist in the /etc/services file (unix) */
/*    and <host-name> must exist in the /etc/hosts file (unix) */
/*    (in os2 these files are in the \tcpip\etc directory) */
/* -------------------------------------------------------------------- */

ServerThis ServInit(char *name, long size, long timeout);
int ServOpen(ServerThis Self);
int ServRead(ServerThis Self, PBuffer buf);
int ServWrite(ServerThis Self, PBuffer buf);
int ServClose(ServerThis Self);
int ServDone(ServerThis Self);
int ServErrno(ServerThis Self, eErrTyp* errtyp);
void ServDisplayErr(ServerThis Self);
int ServSetOpt(ServerThis aSelf, int aOption, long aValue);

ClientThis ClientInit(char *name, long size, long timeout);
int ClientCall(ClientThis Self, PBuffer bufOut, PBuffer bufIn);     /*  O,W,R,C */
int ClientOpen(ClientThis Self);
int ClientTransact(ClientThis Self, PBuffer bufOut, PBuffer bufIn); /*    W,R */
int ClientWrite(ClientThis Self, PBuffer buf);
int ClientRead(ClientThis Self, PBuffer buf);
int ClientClose(ClientThis Self);
int ClientDone(ClientThis Self);
int ClientErrno(ClientThis Self, eErrTyp* errtyp);
void ClientDisplayErr(ClientThis Self);
int ClientSetOpt(ClientThis aSelf, int aOption, long aValue);

#endif

