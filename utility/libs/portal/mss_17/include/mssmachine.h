#if !defined climachineH && !defined mssmachineH
#define mssmachineH

#if defined(__OS2__)
  #define TJ_OS2
#elif defined(__NT__) || defined(__WIN32__) || defined(_WIN32)
  #define TJ_W32
#elif defined(_WINDOWS) || defined(_Windows)
  #define TJ_WIN
#elif defined(_MSDOS) || defined(__MSDOS__) || defined(__DOS__)
  #define TJ_DOS
#elif defined(__GNUC__)
  #define TJ_GNU
#elif defined(_AIX)
  #define TJ_AIX
  #if defined(_AIX51) && !defined(M_HASBOOL)
    #define M_HASBOOL
  #endif
#else
  #define TJ_UNKNOWN
#endif

#ifdef TJ_UNKNOWN
#error Unknown OS and Compiler
#endif

#if defined(TJ_AIX) || defined(TJ_OS2)
  #if !defined(M_HASBOOL) &&  !defined(True) && !defined(true)
    typedef enum bool{false, true};
    #define False false
    #define True  true
  #endif
#endif

#if defined(TJ_AIX)
  #include <va_list.h>
#endif
#include <stdio.h>
#if !defined(TJ_AIX) && !defined(TJ_GNU)
  #if defined(_MSC_VER)
    #include <memory.h>
    #include <malloc.h>
  #else
    #include <mem.h>
    #include <alloc.h>
  #endif
#endif

#ifndef __STDC__
#define __STDC__ 1
#endif

#ifndef __STDC_LIB_EXT1__
#define __STDC_LIB_EXT1__ 1
#endif

#ifndef __STDC_WANT_LIB_EXT1__
#define __STDC_WANT_LIB_EXT1__ 1
#endif

#include <string.h>
#include <stdlib.h>
#include <time.h>
#ifdef UNICODE
#undef UNICODE
#endif
#include <stdlib.h>
#include "sqltypes.h"
#include "msodbcsql.h"
#include <sql.h>
#include "sqlext.h"

#define JP_INTERFACE(_type) extern "C" _type __export __stdcall
#define JP_INTERNAL(_type) static _type
#define JP_EXTERNAL(_type) _type
#define JP_NULL     (short)-1
#define JP_NOT_NULL (short)0
#define JP_MIN_DATE (const char*)"17530101000000"
#define JP_MARK __FILE__,  __LINE__

struct JP_CHAR
{
#if defined(TJ_W32) && defined(UNICODE)
#define _CHAR SQLWCHAR
#else
#define _CHAR SQLCHAR
#endif
  _CHAR* data;
  char* result;
  int size;
  JP_CHAR(const char *x)
  {
	  if (x != 0)
	  {
		  size = (int)(strlen(x) + 1);
		  data = new _CHAR[size];
		  result = 0;
		  set(x);
	  }
	  else
	  {
		  size = 0;
		  data = 0;
		  result = 0;
	  }
  }
  JP_CHAR(const int aSize)
  {
    size = aSize;
    data = new _CHAR[size];
    memset(data, 0, size*sizeof(_CHAR));
    result = 0;
  }
  ~JP_CHAR()
  {
    delete [] data;
    if (result != 0)
      delete [] result;
  }
  operator char*()
  {
    if (result == 0)
      result = new char[size];
    for (int i=0; i<size; i++)
      result[i] = (char)data[i];
    return result;
  }
  void set(const char* x)
  {
    for (int i=0; i<(size-1); i++)
    {
      data[i] = x[i];
      if (x[i] == 0)
        break;
    }
    data[size-1] = 0;
  }
  void resize(const int aSize)
  {
    if (size != aSize)
    {
      _CHAR *oldData = data;
      if (size > 0)
      {
        int copySize = size > aSize ? aSize : size;
        data = new _CHAR[aSize];
        memset(data, 0, aSize*sizeof(_CHAR));
        memcpy(data, oldData, copySize*sizeof(_CHAR));
        size = aSize;
      }
      delete [] oldData;
      if (result != 0)
        delete [] result;
      result = 0;
    }
  }
#undef _CHAR
};

#include <stdio.h>
#include "xcept.h"

struct TCliApiException : public xDBException
{
  TCliApiException(int aErrorNo, char const* state, char const* allow, char const* error, char const* aFile, int aLine)
  : xDBException(aFile, aLine, "CliApiException", aErrorNo, error)
  {
    osErr << "STATE (" << state << ")"  << endl;
    osErr << "ALLOW (" << allow << ")"  << endl;
    osErr << ends;
  }
  TCliApiException(const TCliApiException& copy)
  : xDBException(copy)
  {}
};

#endif

