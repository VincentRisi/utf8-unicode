#ifndef ubixceptH
#define ubixceptH

#include <stdio.h>

#include "xcept.h"

class TUbiException : public xIdlException
{
public:
  enum eError
  { MEMORY_ALLOCATION_FAILED = 1
  , SEMAPHORE_FOPEN_FAILED
  , SEMAPHORE_FTOK_FAILED
  , SEMAPHORE_SEMGET_FAILED
  };
  TUbiException(int aErrorNo, const char *error, const char *aFile, int aLine)
  : xIdlException(aFile, aLine, "UbiApiException", aErrorNo, error)
  {
    osErr << ends;
  }
  TUbiException(const TUbiException& copy)
  : xIdlException(copy)
  {}
};

#endif