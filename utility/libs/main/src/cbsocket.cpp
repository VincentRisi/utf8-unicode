#include "cbsocket.h"
#include "obsocket.h"


/*-----------------------------------
   Error macros
-----------------------------------*/
#define XSOCKERR(err, scb, no, msg) \
        xSockErr(__FILE__, __LINE__, err, scb, no, msg)
#define XSOCKERR1(err, scb, no, msg, msg1) \
        xSockErr(__FILE__, __LINE__, err, scb, no, msg, msg1)
#define XSOCKERR2(err, scb, no, msg, msg1, msg2) \
        xSockErr(__FILE__, __LINE__, err, scb, no, msg, msg1, msg2)


static const char *SockErrs[] =
{ 
     "OK ???"
   , "SockInit"
   , "SockHost"
   , "SockService"
   , "SockSocket"
   , "SockBind"
   , "SockListen"
   , "SockConnect"
   , "SockClose"
   , "SockRead"
   , "SockWrite"
   , "SockVersion"
   , "SockIOCtl"
   , "SockLinger"
   , "SockKeepAlive"
   , "SockTimeout"
   , "SockDebug"
   , "SockGetOpt"
   , "SockSetOpt"
   , "SockParse"
#if defined(USE_OPENSSL)
   , "SockSSLContext"
#endif
};

xSockErr::xSockErr(const char   *aFname, 
                   const int     aLine, 
                   const eError  aError, 
                   const tSockCB *fSockCB,
                   const int     aNo, 
                   const char   *aMsg,
                   const char   *aMsg2, 
                   const char   *aMsg3)
         :xCept(aFname, aLine, "xSockErr", aError)
{
   osErr << "Socket " << SockErrs[aError] << " error " << aNo << std::endl;

   if (fSockCB)
      osErr << "IP:" << inet_ntoa(fSockCB->Addr.sin_addr) << std::endl;
   
   if (aMsg)
      osErr << "1) " << aMsg << std::endl;

   if (aMsg2)
      osErr << "2) " << aMsg2 << std::endl;

   if (aMsg3)
      osErr << "3) " << aMsg3 << std::endl;

   osErr << std::ends;
}

int tCBSockClient::ParseName(char *name, const char sep, char* names[])
{
   // ---------------------------------------------------------
   // returns 0 if name does not start with a separator
   // else splits into substrings on the separator and returns
   // the number of parts - at least one
   // ---------------------------------------------------------
   int   i;
   char *seppos;

   // .skip the first separator
   if (*name++ != sep)
      return 0;

   for (i = 0; i < 3; i++)
   {
      seppos = strchr(name, sep);
      if (!seppos)
      {
         names[i++] = name;
         break;
      }
      *seppos  = 0;
      names[i] = name;
      name     = seppos + 1;
   }

   return i;
}

tCBSockClient::tCBSockClient(const char *aHost, const char *aService)
{
   int wError = SockClientInit(&fSockCB, aHost, aService);
   if (wError)
      throw XSOCKERR2(xSockErr::eError(wError), fSockCB,
                      SockError(fSockCB),
                      SockErrorMsg(fSockCB),
                      aHost, aService);
}

tCBSockClient::tCBSockClient(const char *aName)
{
   char *names[3];
   char  temp[128];

   fSockCB = 0;

   strncpy(temp, aName, sizeof(temp));

   int parts = ParseName(temp, ':', names);
   if (parts != 3 || (strcmp(names[0], "socket")) != 0)
      throw XSOCKERR1(xSockErr::errSockParse, 0, 0, ":socket:<host>:<service> expected", aName);

   int wError = SockClientInit(&fSockCB, names[1], names[2]);

   if (wError)
      throw XSOCKERR2(xSockErr::eError(wError), fSockCB,
                      SockError(fSockCB),
                      SockErrorMsg(fSockCB),
                      names[1], 
                      names[2]);
}

tCBSockClient::tCBSockClient(const int aCommsPort, const int aRetries, const int aTimeOut)
{
   int wError = SockDuplicate(&fSockCB, aCommsPort, aRetries, aTimeOut);
   if (wError)
      throw XSOCKERR(xSockErr::eError(wError), fSockCB,
                     SockError(fSockCB),
                     SockErrorMsg(fSockCB));
}

#if defined(USE_OPENSSL)
tCBSockClient::tCBSockClient(const char *aHost, const char  *aService, const char *CertFile, const char *CAfile, const char *CApath, const char *KeyFile, const char *KeyPassword)
{
    int wError = SockClientInit(&fSockCB, aHost, aService);
    if (wError == 0)
        wError = SSLClientInit(fSockCB, CertFile, CAfile, CApath, KeyFile, KeyPassword);
    if (wError)
    {
        fSockCB->CommsPort = -1;
        throw XSOCKERR(xSockErr::eError(wError), fSockCB,
            SockError(fSockCB),
            SockErrorMsg(fSockCB));
    }
}
#endif

tCBSockClient::~tCBSockClient()
{
   if (fSockCB == 0)
      return;

   int wError = SockDone(fSockCB);

   if (wError != errSockInit)
   {
      free(fSockCB);
   }
}

void tCBSockClient::Open()
{
   int wError = SockClientOpen(fSockCB);
   if (wError)
   {
      fSockCB->CommsPort = -1;
      throw XSOCKERR(xSockErr::eError(wError), fSockCB,
                     SockError(fSockCB),
                     SockErrorMsg(fSockCB));
   }
}

void tCBSockClient::Close()
{
#if defined(SD_BOTH)
shutdown(fSockCB->CommsPort, SD_BOTH); // Vince adding this to see if WAITS reduce faster
#elif defined(SHUT_RDWR)
shutdown(fSockCB->CommsPort, SHUT_RDWR); // Vince adding this to see if WAITS reduce faster
#endif

   int wError = SockClose(fSockCB);
   if (wError)
      throw XSOCKERR(xSockErr::eError(wError), fSockCB,
                     SockError(fSockCB),
                     SockErrorMsg(fSockCB));
}

unsigned int tCBSockClient::ReadLength()
{
   unsigned int wResult;
   int wError = SockReadLength(fSockCB, &wResult);
   if (wError)
      throw XSOCKERR(xSockErr::eError(wError), fSockCB,
                     SockError(fSockCB),
                     SockErrorMsg(fSockCB));
   return wResult;
}

void tCBSockClient::Read(void *aBuffer, const unsigned int aLength)
{
   int wError = SockRead(fSockCB, aBuffer, aLength);
   if (wError)
      throw XSOCKERR(xSockErr::eError(wError), fSockCB,
                     SockError(fSockCB),
                     SockErrorMsg(fSockCB));
}

void tCBSockClient::Read(void *aBuffer, const unsigned int aToRead, unsigned int &aRead)
{
   aRead = ReadLength();
   if (aRead <= aToRead)
      Read(aBuffer, aRead);
   else
   {
      Read(aBuffer, aToRead);
      char soak[512];
      while (ReadStream((void *) soak, sizeof(soak)));
   }
}

void tCBSockClient::Read(tCBSockBuffer *aBuffer)
{
   Read(aBuffer->p, aBuffer->BytesToRead, aBuffer->BytesRead);
}

unsigned int tCBSockClient::ReadStream(void *aBuffer, const unsigned int aLength)
{
   unsigned int wActual;
   int wError = SockStreamRead(fSockCB, aBuffer, aLength, &wActual);
   if (wError)
      throw XSOCKERR(xSockErr::eError(wError), fSockCB,
                     SockError(fSockCB),
                     SockErrorMsg(fSockCB));
   return wActual;
}

void tCBSockClient::WriteLength(const unsigned int aLength)
{
   int wError = SockWriteLength(fSockCB, aLength);
   if (wError)
      throw XSOCKERR(xSockErr::eError(wError), fSockCB,
                     SockError(fSockCB),
                     SockErrorMsg(fSockCB));
}

void tCBSockClient::Write(void *aBuffer, const unsigned int aLength)
{
   int wError = SockWrite(fSockCB, aBuffer, aLength);
   if (wError)
      throw XSOCKERR(xSockErr::eError(wError), fSockCB,
                     SockError(fSockCB),
                     SockErrorMsg(fSockCB));
}

void tCBSockClient::Write(tCBSockBuffer *aBuffer)
{
   WriteLength(aBuffer->BytesToWrite);
   Write((void *)aBuffer->p, aBuffer->BytesToWrite);
   aBuffer->BytesWritten = aBuffer->BytesToWrite;
}

void tCBSockClient::Transact(tCBSockBuffer *bufOut, tCBSockBuffer *bufIn)
{
   Write(bufOut);
   Read(bufIn);
}

void tCBSockClient::Call(tCBSockBuffer *bufOut, tCBSockBuffer *bufIn)
{
   Open();
   Transact(bufOut, bufIn);
   Close();
}

bool tCBSockClient::WaitRead(const unsigned int aTimeout)
{
   bool wResult;
   int  wError = SockWaitRead(fSockCB, aTimeout, &wResult);

   if (wError == errSockTimeout && errno != EINTR)
      return false;

   if (wError)
      throw XSOCKERR(xSockErr::eError(wError), fSockCB,
                     SockError(fSockCB),
                     SockErrorMsg(fSockCB));
   return wResult;
}

bool tCBSockClient::WaitWrite(const unsigned int aTimeout)
{
   bool wResult;
   int  wError = SockWaitWrite(fSockCB, aTimeout, &wResult);

   if (wError == errSockTimeout && errno != EINTR)
      return false;

   if (wError)
      throw XSOCKERR(xSockErr::eError(wError), fSockCB,
                     SockError(fSockCB),
                     SockErrorMsg(fSockCB));
   return wResult;
}

char *tCBSockClient::ClientIPAddress() const
{
   return inet_ntoa(fSockCB->Addr.sin_addr);
}


tCBSockServer::tCBSockServer(const char *aService)
{
   AutoSem = 0;

   char *names[3];
   char  temp[128];

   strncpy(temp, aService, sizeof(temp) - 1);

   int parts = ParseName(temp, ':', names);
   int wError;

   if (parts == 0)
      wError = SockServerInit(&fSockCB, aService);
   else if (parts != 3 || (strcmp(names[0], "socket")) != 0)
      throw XSOCKERR1(xSockErr::errSockParse, 0,
                      0, ":socket:<host>:<service> or <service> expected", aService);
   else
      wError = SockServerInit(&fSockCB, names[2]);

   if (wError)
      throw XSOCKERR1(xSockErr::eError(wError), fSockCB,
                      SockError(fSockCB),
                      SockErrorMsg(fSockCB),
                      aService);
}

tCBSockServer::tCBSockServer(const int aCommsPort, const int aRetries, const int aTimeOut)
{
   AutoSem = 0;

   int wError = SockDupServer(&fSockCB, aCommsPort, aRetries, aTimeOut);
   if (wError)
      throw XSOCKERR(xSockErr::eError(wError), fSockCB,
                     SockError(fSockCB),
                     SockErrorMsg(fSockCB));
}

tCBSockServer::tCBSockServer(const tCBSockServer &Sock)
{
   AutoSem = Sock.AutoSem;
   fSockCB = Sock.fSockCB;
}

#if defined(USE_OPENSSL)
tCBSockServer::tCBSockServer(const char  *aService, const char *CertFile, const char *CAfile, const char *CApath, const char *KeyFile, const char *KeyPassword)
{
    AutoSem = 0;
    int wError = SockServerInit(&fSockCB, aService);
    if (wError == 0)
        wError = SSLServerInit(fSockCB, CertFile, CAfile, CApath, KeyFile, KeyPassword);
    if (wError)
        throw XSOCKERR(xSockErr::eError(wError), fSockCB,
                       SockError(fSockCB),
                       SockErrorMsg(fSockCB));
}
#endif

void tCBSockServer::Open()
{
   int wError = SockServerOpen(fSockCB);
   if (wError)
      throw XSOCKERR(xSockErr::eError(wError), fSockCB,
                     SockError(fSockCB),
                     SockErrorMsg(fSockCB));
}

bool tCBSockServer::WaitConnect(const unsigned int aTimeout)
{
   bool wResult;
   int wError = SockWaitServer(fSockCB, aTimeout, &wResult);
   if (wError == errSockTimeout && errno == EINTR)
      return false;
   if (errno == EINTR)
   {
      fprintf(stderr, "WaitConnect\n"
                      "  wResult: %s\n"
                      "  wError: %d\n"
                      "  errno: %d\n"
                      , wResult?"TRUE":"FALSE"
                      , wError
                      , errno);
      return false;
   }
   return wResult;
}

