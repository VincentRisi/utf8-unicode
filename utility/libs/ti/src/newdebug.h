//  newdebug.hpp
//  debugging code for new/delete
//  copyright Barry Fortune 1994
//
//  class TDebugFStream;
//  class TNewDebug
//  debugging versions of operator: new, new[], delete, delete[]
//  non-debugging version of operator new[](void*) - using realloc
//    and supporting operator new and delete.
//  macro to insert file name, line nos
//
//  defines used below for conditional compilation
//  NEWDEBUG_ON: defined to turn debug mode on
//  NO_CPP_NEW_ARRAY: defined if the version of C++ does -not- support
//    operator new[], and operator delete[]
//  BORLAND_HEAP_CHK: defined if using borland compiler with heap checking

#if !defined(_NEWDEBUG_HPP_) && !defined(FORTIFY)
#define _NEWDEBUG_HPP_ 

#ifndef LOGNAME
#define LOGNAME "new.log"
#endif

#ifdef NEWDEBUG_ON

#include "ti.h"

#include <stdlib.h>

// -------------------------------------------------
class TDebugFStream: public std::ofstream
{
public:
  static int isopen;
  TDebugFStream(const char *name = LOGNAME);
  ~TDebugFStream();
};

// -------------------------------------------------


class TNewDebug
{
public:
  typedef uint32 align;           // worst case alignment type
  typedef const uint32 calign;    // worst case alignment type

  enum
  {
    START_FILL_LEN = 8,           // length of filler area at start
    UNDERRUN_LEN = 12,            // length of underrun area
    OVERRUN_LEN = 12              // length of overrun area
  };

  // error codes
  enum Errors
  {
    EF_OKAY,
    EF_NULLPTRDEL,
    EF_ZEROSIZE,
    EF_DELETE_TWICE,
    EF_INVALID,
    EF_ARRAYMISMATCH,
    EF_OVERRUN,
    EF_UNDERRUN,

    EF_NOTALLOC,
    EF_UNDERFLOW,
    EF_TOOMANY,
    EF_HEAPCORRUPT,
    EF_HEAPOTHER
  };

  static void outmainheader();
  static void outheader(char* name, const char* file = "none", int line = 0);
  static void outsize(uint32 size = 0);
  static void outptr(void* p = 0);
  static void outok();
  static void outtotal();
  static void outerror(Errors err);

  static TDebugFStream dout;           // log file
  static char* ErrorStr[];             // error messages

  // return no of active allocations
  static int32 GetCount();

  // returns number of bytes allocated
  static int32 GetBytes();

  // returns the current error code
  static Errors GetError();

  // sets the error to EF_OKAY
  static void ClearError();

  static int isDynamic(void* ptr, size_t& size);

  // uses heapwalk to check heap
  enum verbose_t { not_verbose, verbose };

#ifdef BORLAND_HEAP_CHK
  static void heap_dump(char* name, verbose_t isverbose);
#endif

  // new replacement function
  static void* _new(size_t size, char *file, int line, int isarray);

  // delete replacement function
  static void _delete(void* ptr, char *file, int line, int isarray);

private:
  // header for tracking allocations
  struct header
  {
    TNewDebug::align startfill[START_FILL_LEN];
    int isarray;
    size_t bytes;
    char* file;
    int line;
    TNewDebug::align marker;
    TNewDebug::align underrun[UNDERRUN_LEN];
  };

  struct footer
  {
    TNewDebug::align overrun[OVERRUN_LEN];
  };

  static int32 Count;                  // no of items allocated
  static int32 Bytes;                  // no of bytes allocated
  static int32 HeapBytesStart;         // no of bytes allocated in heapwalk
  static int32 HeapBytesEnd;           // no of bytes allocated in heapwalk
  static Errors ErrorFlag;            // current error code
  static calign CorrectMark;          // allocation marker
  static calign UnderFill;            // under-run area filler value
  static calign OverFill;             // over-run area filler value
  static const unsigned char NewFill; // new memory filler value
  static const unsigned char DelFill; // deleted memory filler value

  // zero    invalid
  // nonzero valid
  static int isvalidDel(header* head, int isarray, char* file, int line);
};

inline int32 TNewDebug::GetCount()
{
  return Count;
}

inline int32 TNewDebug::GetBytes()
{
  return Bytes;
}

inline TNewDebug::Errors TNewDebug::GetError()
{
  return ErrorFlag;
}

inline void TNewDebug::ClearError()
{
  ErrorFlag = EF_OKAY;
}

// -------------------------------------------------
// .debug version of new - maps to operator new with placement
#define newd new(__FILE__, __LINE__)

void* operator new(size_t size, char *file, int line);

#ifndef NO_CPP_NEW_ARRAY
void* operator new[](size_t size, char *file, int line);
#endif // NO_CPP_NEW_ARRAY

template <class T>
void deleteDbg(T* ptr, char* file, int line, int isArray)
{
  ptr->T::~T(); // if T is a built in type - hopefully this is ignored
  TNewDebug::_delete(ptr, file, line, isArray);
}

// .debug versions of delete - non-array and array
#define del(p) deleteDbg(p, __FILE__, __LINE__, 0)
#ifdef NO_CPP_NEW_ARRAY
#define delv(p) deleteDbg(p, __FILE__, __LINE__, 0)
#else
#define delv(p) deleteDbg(p, __FILE__, __LINE__, 1)
#endif

#else  // NEWDEBUG_ON
#define newd    new
#define del(p)  delete p
#define delv(p) delete[] p
#endif // NEWDEBUG_ON

#endif // _NEWDEBUG_H_

