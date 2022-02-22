// cxcept.h
// barry fortune
// July 1994
// Windows MFC exception class holding a string pointer

#ifndef _XCEPT_H_
#define _XCEPT_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_XCEPT_H_);

// NB: One module must have the following line of code
// IMPLEMENT_DYNAMIC(CStringException, CException)

class CStringException : public CException
{
  char buf[256];
  enum { BUFLEN = 256 };
  DECLARE_DYNAMIC(CStringException)

public:
  CStringException();
  ~CStringException();
  void SetBuf(char* aBuf);
  char* Buf();
};

inline CStringException::CStringException()
{
  memset(buf, 0, BUFLEN);
}

inline CStringException::~CStringException()
{
}

inline void CStringException::SetBuf(char* aBuf)
{
  strncpy(buf, aBuf, BUFLEN - 1);
}

inline char* CStringException::Buf()
{
  return buf;
}

#endif

