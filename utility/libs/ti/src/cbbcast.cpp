//#include "versions.h"
//#define _VERSION_NO "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
//COMPILED_VERSION(_VERSION_NO);

#include "ti.h"
#include "cbbcast.h"

static const char * BcastErrs[] =
  { "OK ???"
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
  };

#ifdef M_STDLIB
using namespace std;
#endif

xBcastErr::xBcastErr(const char * aFname, int aLine, eError aError, int aNo,
                  char * aMsg, char * aMsg1, char * aMsg2)
: xCept(aFname, aLine, xBcastErrName, aError)
{
   //if (snprintf(errorMsg, sizeof(errorMsg) - 2, "Bcast Socket %s error %d\n1)%s\n2)%s\n3)%s", BcastErrs[aError], aNo, aMsg, aMsg1, aMsg2) == -1)
   //   errorMsg[sizeof(errorMsg) - 1] = 0;

    osErr << "Bcast Socket " << BcastErrs[aError] << " error " << aNo << std::endl;
    if (aMsg)
        osErr << "1) " << aMsg << std::endl;
    if (aMsg1)
        osErr << "2) " << aMsg1 << std::endl;
    if (aMsg2)
        osErr << "3) " << aMsg2 << std::endl;
    osErr << ends;
}

tBcastClient::tBcastClient(char * aHost, char * aService)
{
  int wError = BcastClientInit(&fBcastCB, aHost, aService);
  if (wError)
    throw XBcastErr2(xBcastErr::eError(wError),
                    BcastError(fBcastCB),
                    BcastErrorMsg(fBcastCB),
                    aHost, aService);
}

tBcastClient::~tBcastClient()
{
  int wError = BcastDone(fBcastCB);
  if (wError)
    throw XBcastErr(xBcastErr::eError(wError),
                   BcastError(fBcastCB),
                   BcastErrorMsg(fBcastCB));
}

void tBcastClient::Close()
{
  int wError = BcastClose(fBcastCB);
  if (wError)
    throw XBcastErr(xBcastErr::eError(wError),
                   BcastError(fBcastCB),
                   BcastErrorMsg(fBcastCB));
}

void tBcastClient::ReadDGram(void * aBuffer, unsigned long aLength, unsigned long * aSize)
{
  int wError = BcastDGramRead(fBcastCB, aBuffer, aLength, aSize);
  if (wError)
    throw XBcastErr(xBcastErr::eError(wError),
                   BcastError(fBcastCB),
                   BcastErrorMsg(fBcastCB));
}

void tBcastClient::Write(void * aBuffer, unsigned long aLength)
{
  int wError = BcastWrite(fBcastCB, aBuffer, aLength);
  if (wError)
    throw XBcastErr(xBcastErr::eError(wError),
                   BcastError(fBcastCB),
                   BcastErrorMsg(fBcastCB));
}

tBcastServer::tBcastServer(char * aService)
{
  int wError = BcastServerInit(&fBcastCB, aService);
  if (wError)
    throw XBcastErr1(xBcastErr::eError(wError),
                    BcastError(fBcastCB),
                    BcastErrorMsg(fBcastCB),
                    aService);
}



