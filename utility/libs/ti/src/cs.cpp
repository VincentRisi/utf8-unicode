#include "versions.h"
#define _VERSION_NO "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
COMPILED_VERSION(_VERSION_NO);

//
//
//
// WAS: short tClientServer::SockReadAll(int session, pchar buff, int32 size)
// NOW: int32 tClientServer::SockReadAll(int session, pchar buff, int32 size)
// Changed by Geoff Bruce 27 Jul '95
//
//
#include "ti.h"
#include "cs.h"
#include <sys/select.h>
#include <strings.h>
//#include "timer.h"

#ifdef DOTRACE
#include "trace.h"
#define LOGTRACE(name) \
  LogTrace("trace.lg0", "trace.lg1", 5000, \
           "%s:%d %s\n", __FILE__,  __LINE__, name)
#else
#define LOGTRACE(name)
#endif

// socket stuff
#define PROTOCOL "tcp"
#define SOCKET_QSIZE 5
#define MAX_RETRIES 5

// ---------------------------------------------
xClientServer::xClientServer(char* aFile,const int aLine,
  int aErrtyp,int aErrno, char* aInfo)
  : xCept(aFile, aLine, "xClientServer",aErrno)
{
  LOGTRACE("xClientServer::xClientServer");
  switch (aErrtyp)
  {
  case errtypOther:
    osErr << endl << "Other: ";
    switch (aErrno)
    {
    case ERR_GENERAL:
      osErr << "General error. ";
      break;
    case ERR_MALLOC:
      osErr << "Memory allocation error. ";
      break;
    case ERR_TIMEOUT:
      osErr << "Timeout error. ";
      break;
    case ERR_WRONGSERVER:
      osErr << "Incorrect server error. ";
      break;
    case ERR_NOSUCHSERVICE:
      osErr << "No such service error. ";
      break;
    case ERR_NOHOST:
      osErr << "No such host. ";
      break;
    case ERR_HOSTTRYAGAIN:
      osErr << "Host busy. Try again later. ";
      break;
    case ERR_HOSTNORECOVERY:
      osErr << "DNS error for host. ";
      break;
    case ERR_HOSTNOADDRESS:
      osErr << "No IP address for host. ";
      break;
    case ERR_BADNAME:
      osErr << "Bad construction String. ";
      break;
    case ERR_DEFAULTREACHED:
      osErr << "Default Reached. ";
      break;
    }
    break;
  case errtypSocket:
    osErr << endl << "Socket: ";
    if (unixErrorNo == EINTR)
       osErr << " TimeOut";
    else
       osErr << " Unix Error " << errnomsg(aErrno);
    break;
  }
  osErr << endl << aInfo;
}

// ---------------------------------------------
tClientServer::tClientServer(const char* constr, int32 size, int32 timeout)
{
  LOGTRACE("tClientServer::tClientServer");
  char msgbuf[100];
  ostrstream es(msgbuf, sizeof(msgbuf));
  char temp[50];
  strcpy(temp, constr);

  // parse the name into pieces
  int parts = ParseName(temp, ':', names);
  if (!parts)
  {
     es << "Expected a colon Separated list of format in constructor and got"
        << temp << endl
        << "Usage " <<endl
        << " <ipctype>:<key>:[name]:<size>:timeout";
     throw XClientServer(errtypOther, ERR_BADNAME, msgbuf);
  }

  Size = size;
  TimeOut = timeout;

  // lookup the type
  if (strieq(names[0], "socket"))
  {
    if (parts != 3)
    {
      es << "Expected socket:<host>:<service> but found " << temp;
      throw XClientServer(errtypOther, ERR_BADNAME, msgbuf);
    }

    if (timeout == 0)
      TimeOut = 1;
    else
      TimeOut = timeout;
  }
  else
  {
    es << "Unknown IPC specifier ["<< names[0] <<"]. Use socket";
    throw XClientServer(errtypOther, ERR_BADNAME, msgbuf);
  }
}

tClientServer::~tClientServer()
{
  LOGTRACE("tClientServer::~tClientServer");
}

void tClientServer::Close()
{
  LOGTRACE("tClientServer::Close");
  close(sd); // Close Socket Session
}

static int _Wait(int aPort, uint32 aMSec, bool onRead, pbool aResult)
{
  LOGTRACE("_Wait");
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
  FD_SET(aPort, &OnSet);
  int rc = select(FD_SETSIZE, (onRead==true?&OnSet:0), (onRead==true?0:&OnSet), 0, TOP);
  if (rc == -1)
    return errtypSocket;
  *aResult = FD_ISSET(aPort, &OnSet)?true:false;
  return 0;
}

void tClientServer::Write(PBuffer buf)
{
  LOGTRACE("tClientServer::Write 0");
  uint32 length = htonl(buf->BytesToWrite);
  bool Result;
  int rc;

  // write the length
  if (_Wait(sd, TimeOut, false, &Result)==0)
  {
    LOGTRACE("tClientServer::Write 1");
    if (Result == true)
    {
      rc = write(sd, (char*)&length, sizeof(length));
      LOGTRACE("tClientServer::Write 1b");
    }
    else
    {
      LOGTRACE("tClientServer::Write 1c");
      throw XClientServer(errtypSocket, errno,"TIMEOUT on write");
      LOGTRACE("tClientServer::Write 1d");
    }
  }
  else
  {
    LOGTRACE("tClientServer::Write 0a");
    throw XClientServer(errtypSocket, errno, "TIMEOUT on write failed");
    LOGTRACE("tClientServer::Write 0b");
  }
  LOGTRACE("tClientServer::Write 2");
  if (rc != sizeof(length))
  {
    LOGTRACE("tClientServer::Write 2a");
    if (errno == EINTR)
    {
      LOGTRACE("tClientServer::Write 2b");
      throw XClientServer(errtypSocket, errno,"TIMEOUT on write LENGTH");
      LOGTRACE("tClientServer::Write 2c");
    }
    else
    {
      LOGTRACE("tClientServer::Write 2d");
      throw XClientServer(errtypSocket, errno,"Failed to write LENGTH");
      LOGTRACE("tClientServer::Write 2e");
    }
  }
  LOGTRACE("tClientServer::Write 3");
  // write the message
  if (_Wait(sd, TimeOut, false, &Result)==0)
  {
    LOGTRACE("tClientServer::Write 4");
    if (Result == true)
    {
      buf->BytesWritten = write(sd, buf->p, buf->BytesToWrite);
      LOGTRACE("tClientServer::Write 4b");
    }
    else
    {
      LOGTRACE("tClientServer::Write 4d");
      throw XClientServer(errtypSocket, errno,"TIMEOUT on write");
      LOGTRACE("tClientServer::Write 4e");
    }
  }
  else
  {
    LOGTRACE("tClientServer::Write 3a");
    throw XClientServer(errtypSocket, errno, "TIMEOUT on write failed");
    LOGTRACE("tClientServer::Write 3b");
  }
  LOGTRACE("tClientServer::Write 5");
  if (buf->BytesWritten != buf->BytesToWrite)
  {
    LOGTRACE("tClientServer::Write 5a");
    if (errno == EINTR)
    {
      LOGTRACE("tClientServer::Write 5b");
      throw XClientServer(errtypSocket, errno,"TIMEOUT on write DATA");
      LOGTRACE("tClientServer::Write 5c");
    }
    else
    {
      LOGTRACE("tClientServer::Write 5d");
      throw XClientServer(errtypSocket, errno,"Failed to write DATA");
      LOGTRACE("tClientServer::Write 5e");
    }
  }
  LOGTRACE("tClientServer::Write 6");
}

void tClientServer::Read(PBuffer buf)
{
  LOGTRACE("tClientServer::Read");
  // message is <ULONG length><char * Buff>
  // first read message length

  uint32 length;
  int32 n = SockReadAll(sd, (char*)&length, sizeof(length));
  if (n != sizeof(length))
  {
    if (n == ERR_TIMEOUT)
      throw XClientServer(errtypOther,ERR_TIMEOUT,
        "failed to read Length timeout");
    else
     throw XClientServer(errtypSocket, errno, "failed to read Length");
  }

  buf->BytesRead = ntohl(length);

  // if client sends more than what server expects this 'excess' is ignored
  int excess = buf->BytesRead - buf->BytesToRead;
  if (excess > 0)
     buf->BytesRead -= excess;

  n = SockReadAll(sd, buf->p, buf->BytesRead);
  if (n != buf->BytesRead)
  {
    if (n == ERR_TIMEOUT)
      throw XClientServer(errtypOther, ERR_TIMEOUT, "read timed out");
    else
      throw XClientServer(errtypSocket, errno, "read failed");
  }
  if (excess > 0)                                // read and trash excess bytes
  {
    pchar p;
    p = new char[excess];
    n = SockReadAll(sd, p, excess);
    if (n != excess)
    {
    if (n == ERR_TIMEOUT)
      throw XClientServer(errtypOther, ERR_TIMEOUT, "read timed out");
    else
       throw XClientServer(errtypSocket, errno, "read failed");
    }
    delete[] p;
  }
}

void tClientServer::Transact(PBuffer bufOut, PBuffer bufIn)
{
  LOGTRACE("tClientServer::Transact");
  Write(bufOut);
  Read(bufIn);
}

void tClientServer::Call(PBuffer bufOut, PBuffer bufIn)
{
  LOGTRACE("tClientServer::Call");
  Open();
  Transact(bufOut, bufIn);
  Close();
}

bool tClientServer::Wait()
{
  LOGTRACE("tClientServer::Wait");
  timeval TempTimeOut;
  timeval* TOP;
  fd_set OnSet;

  if (TimeOut)
  {
    TempTimeOut.tv_sec = TimeOut / 1000;
    TempTimeOut.tv_usec = (TimeOut % 1000) * 1000;
    TOP = &TempTimeOut;
  }
  else
    TOP = 0;

  FD_ZERO(&OnSet);
  FD_SET(sd, &OnSet);
  int rc = select(FD_SETSIZE, &OnSet, 0, 0, TOP);
  if (rc == -1)
     XClientServer(errtypOther,ERR_GENERAL,"Socket select Failed");
  return FD_ISSET(sd, &OnSet)?true:false;
}

//short tClientServer::SockReadAll(int session, pchar buff, int32 size)
int32    // Changed by Geoff Bruce 27 Jul '95
tClientServer::SockReadAll(int session, pchar buff, int32 size)
{
  LOGTRACE("tClientServer::SockReadAll");
  int BytesRead = 0, n, Try = 0;
  short rc = 0;

  while (BytesRead < size)
  {
    if (TimeOut && Wait() == false)
    {
      if (Try++ < MAX_RETRIES)
        continue;
      else
        return ERR_TIMEOUT;
    }
    n = read(session, buff, size - BytesRead);
    if (n < 0)
      continue;
    if (n == 0)
      return BytesRead;
    BytesRead += n;
    buff += n;
  }
  return BytesRead;
}

int tClientServer::ParseName(char* name, char sep, char* names[])
{
  LOGTRACE("tClientServer::ParseName");
  // ---------------------------------------------------------
  // returns 0 if name does not start with a separator
  // else splits into substrings on the separator and returns
  // the number of parts - at least one
  // ---------------------------------------------------------
  int i;
  char* seppos;

  // .skip the first separator
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

// ---------------------------------------------
tClient::tClient(char* name, int32 size, int32 timeout)
  : tClientServer(name, size, timeout)
{
  LOGTRACE("tClient::tClient");
  Init(names[1], names[2]);
}

tClient::~tClient()
{
  LOGTRACE("tClient::~tClient");
}

void tClient::Open()
{
  LOGTRACE("tClient::Open");
  //tAlarm timer;
  int rc;
  // open a socket, connect to the server
  if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
  {
    throw XClientServer(errtypSocket, errno, "failed to open");
  }

  //if (TimeOut != INFINITY)
  //  timer.Set(TimeOut);
  rc = connect(sd, (struct sockaddr*)&sin, sizeof(sin));
  //if (TimeOut != INFINITY)
  //  timer.Clear();
  if (rc < 0)
  {
    char msgbuf[100];
    ostrstream es(msgbuf, sizeof(msgbuf));
    es << "failed to connect, timeout value == " << TimeOut;
    throw XClientServer(errtypSocket, errno, msgbuf);
  }
}

// currently ignoring the timeout value
void tClient::Init(char* host, char* service)
{
  LOGTRACE("tClient::Init");
  Size += sizeof(unsigned short);  // addition for size

  // get port no based on service
  struct servent* ps;         // service entry
  if (!(ps = getservbyname(service, PROTOCOL)))
    throw XClientServer(errtypOther, ERR_NOSUCHSERVICE,
      "check /etc/services");
  sin.sin_port = ps->s_port;

  // find the internet address of the host
  struct hostent* ph;         // host entry
  int ErrorNo;
  if (!(ph = gethostbyname(host)))
  {
     //extern int h_errno;
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
    throw XClientServer(errtypOther, ErrorNo, "check /etc/hosts");
  }
  sin.sin_family = ph->h_addrtype;
  memcpy(&sin.sin_addr, ph->h_addr, ph->h_length);
}

// ---------------------------------------------
tServer::tServer(char* name, int32 size, int32 timeout)
  : tClientServer(name, size, timeout)
{
  LOGTRACE("tServer::tServer");
  Init(names[1], names[2]);
}

void tServer::Open()
{
  LOGTRACE("tServer::Open");
  while ((sd = accept(sockno,0,0)) < 0 && errno == EINTR)
    ;
  // EINTR could mean user has caught a signal so it's not an error

  if (sd == -1)         // was < 0
    throw XClientServer(errtypSocket, errno, "accept");
}

tServer::~tServer()
{
  LOGTRACE("tServer::~tServer");
  close(sockno); // Close down Sockets
}

// currently ignoring the timeout value
void tServer::Init(char* host, char* service)
{
  LOGTRACE("tServer::Init");
  Size += sizeof(unsigned short);  // addition for size

  // get port no based on service
  struct servent* ps;         // service entry
  if (!(ps = getservbyname(service, PROTOCOL)))
    throw XClientServer(errtypOther, ERR_NOSUCHSERVICE,
      "check /etc/services");
  sin.sin_port = ps->s_port;
  sin.sin_addr.s_addr = INADDR_ANY;

  // open a socket
  if ((sockno = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    throw XClientServer(errtypSocket, errno, "server failed to open socket");

  // bind address to local end of socket
  int count=10;
  for (;;)
  {
    if (bind(sockno, (struct sockaddr*)&sin, sizeof(sin)) == -1)
    {
      if (count--)
      {
        sleep(1);
        continue;
      }
      throw XClientServer(errtypSocket, errno, "server failed to bind socket");
    }
    break;
  }

  // accept connections from remote clients
  if (listen(sockno, SOCKET_QSIZE) == -1)
    throw XClientServer(errtypSocket, errno,
      "server failed to listen on socket");
}
