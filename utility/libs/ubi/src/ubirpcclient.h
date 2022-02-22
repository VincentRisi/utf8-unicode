#ifndef _UBIRPCCLIENT_H_
#define _UBIRPCCLIENT_H_
#include "xcept.h"

#include "ubidata.h"
#include "ubirpcheader.h"

class UbiRPCClient
{
protected:
  UbiRPCHeader mHeader;
  int32 mRxSize;
  void* mRxBuffer;
  int32 mTimeout;
  int32 mErrSize;
  char *mErrBuffer;
  UbiRPCClient()
  : mRxSize(0)
  , mRxBuffer(0)
  , mErrSize(0)
  , mErrBuffer(0)
  {}
public:
  virtual ~UbiRPCClient() {RxFree();ErrFree();};
  void    *RxBuffer()     {return mRxBuffer;}
  int32    RxSize() {return mRxSize;}
  char    *ErrBuffer()    {return mErrBuffer;}
  char    *ErrBuffer(ulong aSize)
                          {ErrFree();
                           mErrBuffer = new char [mErrSize = aSize];
                           return mErrBuffer;}
  char    *ErrBuffer(const char *ErrStr)
                          {strcpy(ErrBuffer(strlen(ErrStr)+1), ErrStr);
                           return mErrBuffer;}
  int32    ErrSize() {return mErrSize;}
  int32    ErrSize(int32 aValue) {return mErrSize = aValue;}
  void     RxFree()       {if (mRxBuffer) {free(mRxBuffer); mRxBuffer=0;}}
  void     ErrFree()      {if (mErrBuffer) {free(mErrBuffer); mErrBuffer=0;}}
  int32    ReturnCode()   {return mHeader.ReturnCode;}
};

#define xUbiRPCErrName "xUbiRPCErr"

class xUbiRPCErr : public xCept
{
public:
  enum eError
  { errUbiOK
  , errUbiNoHost
  , errUbiNoService
  , errUbiHostInvalid
  , errUbiInvResponse
  , errUbiNoDataRead
  , errUbiInvSignature
  , errUbiDataReadError
  , errUbiDataWriteError
  , errUbiNodeIDError
  , errUbiUserIDError
  , errUbiPasswordError
  , errUbiIPAddrUnavailable
  , errUbiEnvironmentNotSet
  , errUbiKEYFILE
  , errUbiNODEKEYNOTSET
  , errUbiUSERNOTSET
  , errUbiSemaphoreNA
  };
  xUbiRPCErr(char* aFname, int32 aLine, eError aError, int32 aNo=0, char* aMsg=0);
  xUbiRPCErr(const xUbiRPCErr& aX)
  : xCept(aX) {}
};

#define XUbiRPCErr(err) \
        xUbiRPCErr(__FILE__, __LINE__, xUbiRPCErr::err)
#define XUbiRPCErr1(err, no, msg) \
        xUbiRPCErr(__FILE__, __LINE__, xUbiRPCErr::err, no, msg)

struct xUbiException : public xCept
{
  xUbiException(char *file, int line, char *error)
  : xCept(file, line, "UbiException", 0)
  {
    osErr << "Error: " << error << endl;
    osErr << ends;
  }
  xUbiException(const xUbiException& copy)
  : xCept(copy)
  {}
};

#endif
