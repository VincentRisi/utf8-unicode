#ifndef _CBBCAST_H_
#define _CBBCAST_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_CBBCAST_H_);

#include "obbcast.h"

#define xBcastErrName "xBcastErr"
XCLASSDEF(BcastErr);
class xBcastErr : public xCept
{
public:
  enum eError
  { errSockOK
  , errSockInit
  , errSockHost
  , errSockService
  , errSockSocket
  , errSockBind
  , errSockListen
  , errSockConnect
  , errSockClose
  , errSockRead
  , errSockWrite
  , errSockVersion
  , errSockIOCtl
  , errSockLinger
  , errSockKeepAlive
  , errSockTimeout
  , errSockDebug
  , errSockGetOpt
  , errSockSetOpt
  };
  xBcastErr(const char * aFname, int aLine, eError aError, int aNo, char * aMsg,
           char * aMsg2=0, char * aMsg3=0);
  xBcastErr(const xBcastErr& aX)
  : xCept(aX) {}
};

#define XBcastErr(err, no, msg) \
        xBcastErr(__FILE__, __LINE__, err, no, msg)
#define XBcastErr1(err, no, msg, msg1) \
        xBcastErr(__FILE__, __LINE__, err, no, msg, msg1)
#define XBcastErr2(err, no, msg, msg1, msg2) \
        xBcastErr(__FILE__, __LINE__, err, no, msg, msg1, msg2)

CLASSDEF(BcastClient);
class tBcastClient
{
protected:
  pBcastCB fBcastCB;
  unsigned long   fTimeout;
  unsigned short  fRetries;
  tBcastClient() {}
public:
  tBcastClient(char * aHost, char * aService);
  ~tBcastClient();
  void Close();
  void ReadDGram(void * aBuffer, unsigned long aLength, unsigned long * aSize);
  void Write(void * aBuffer, unsigned long aLength);
  void SetTimeout(unsigned long aTimeout) { fTimeout = aTimeout; }
  unsigned long GetTimeout() { return fTimeout; }
  void SetRetries(unsigned long aRetries) { fRetries = aRetries; }
  unsigned short GetRetries() { return fRetries; }
};

CLASSDEF(BcastServer);
class tBcastServer : public tBcastClient
{
  tBcastServer() {}
public:
  tBcastServer(char * aService);
};

#endif

