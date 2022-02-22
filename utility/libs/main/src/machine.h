#ifndef _MACHINE_H_
#define _MACHINE_H_

typedef struct
{
   char p[2];
} byte2;

typedef struct
{
   char p[4];
} byte4;

typedef struct
{
   char p[8];
} byte8;

const unsigned int _MACHINE_FLOAT_SIZE_  = sizeof(float);
const unsigned int _MACHINE_DOUBLE_SIZE_ = sizeof(double);
const unsigned int _MACHINE_LONG_SIZE_   = sizeof(long);
const unsigned int _MACHINE_INT_SIZE_    = sizeof(int);
const unsigned int _MACHINE_SHORT_SIZE_  = sizeof(short);
const unsigned int _MACHINE_CHAR_SIZE_   = sizeof(char);

/* -------------------------- OS/2 ----------------------------- */
#if defined(__OS2__) || defined(__NT__)
   #define M_OS2
   #define APPLAPI
   #define DLLEXPORT_STDCALL(type) type
   #define DLLEXPORT_CDECL(type) type
   #ifdef __BORLANDC__
      #define BORLAND_HEAP_CHK
      #define APC __syscall
      #ifndef _System
         #define _System __syscall
      #endif
   #else
      #define APC
   #endif

/* -------------------------- BC Windows NT -------------------- */
#elif (defined(_WIN32) || defined(__WIN32__)) && defined(__BORLANDC__)
   #define _WINDOWS
   #define M_NT
   #define M_W32
   #define M_HASBOOL
   #ifndef M_STDLIB
      #define M_STDLIB
   #endif
   #define APPLAPI __export __stdcall
   #define DLLEXPORT_STDCALL(type) __export type __stdcall
   #define DLLEXPORT_CDECL(type) type
   #define APC
   #define __RPCASYNC_H__
   #include <winsock2.h>
   typedef __int8  int8;
   typedef __int16 int16;
   typedef __int32 int32, int32_t;
   typedef __int64 int64;
   typedef unsigned __int8  uint8;
   typedef unsigned __int16 uint16;
   typedef unsigned __int32 uint32;
   typedef unsigned __int64 uint64;
   #define atoint64 _atoi64
   //-------- DUMBO FIX FOR BORLAND C--------
   #define _strupr strupr
   #define _strlwr strlwr
   #define _stricmp stricmp
   //----------------------------------------

/* -------------------------- Windows NT and MS VC++-------------------- */
#elif defined(__MSVC__) || _MSC_VER >= 1200
   #ifndef _WINDOWS
      #define _WINDOWS
   #endif
   #ifndef M_STDLIB
      #define M_STDLIB
   #endif
   #define M_NT
   #define M_W32
   #define M_HASBOOL
   #ifdef USE_DECLSPEC_DLLEXPORT
     #define APPLAPI __declspec(dllexport) __stdcall
   #else
     #define APPLAPI __stdcall
   #endif
   #define DLLEXPORT_STDCALL(type) __declspec(dllexport) type __stdcall
   #define DLLEXPORT_CDECL(type) __declspec(dllexport) type
   #define APC
   #define __RPCASYNC_H__
   #include <winsock2.h>

   #define access    _access
   #define unlink    _unlink
#if _MSC_VER >= 1200 && _MSC_VER <= 1900
   #define snprintf  _snprintf
   #define vsnprintf _vsnprintf
#endif
   #define strdup    _strdup
   #define strupr    _strupr
   #define strlwr    _strlwr
   #define tempnam   _tempnam

   typedef __int8  int8;
   typedef __int16 int16;
   typedef __int32 int32, int32_t;
   typedef __int64 int64;
   typedef unsigned __int8  uint8;
   typedef unsigned __int16 uint16;
   typedef unsigned __int32 uint32;
   typedef unsigned __int64 uint64;
   #define atoint64 _atoi64

/* -------------------------- Windows 95/NT--------------------- */
#elif defined(_WIN32)
   #error defined(_WIN32)

/* -------------------------- Windows ------------------ */
#elif defined(_Windows)
   #error defined(_Windows)

/* -------------------------- MsDos ---------------------------- */
#elif defined(_MSDOS) || defined(__MSDOS__) || defined(__DOS__)
   #error defined(_MSDOS) || defined(__MSDOS__) || defined(__DOS__)

/* --------------------- GNUC on Linux-------------------------- */
#elif defined(__GNUC__)
   #include <sys/types.h>
     #define int8   int8_t
     #define int16  int16_t
     #define int32  int32_t
     #define int64  int64_t
     #define uint8  u_int8_t
     #define uint16 u_int16_t
     #define uint32 u_int32_t
     #define uint64 u_int64_t
   //#endif

   #define atoint64 atoll

   #define M_GNU
   #ifndef M_STDLIB
      #define M_STDLIB
   #endif

   #define APPLAPI
   #define DLLEXPORT_STDCALL(type) type
   #define DLLEXPORT_CDECL(type) type
   #define APC
   #define O_BINARY 0
   #define M_HASBOOL
   #include <stdlib.h>
   #ifdef __cplusplus
extern "C" {
#endif
#include <unistd.h>
#ifdef __cplusplus
}
   #endif

/* -------------------------- AIX ------------------------------
// EUREKA WE HAVE FOUNDED IT - AIX51 == VISUALAGE
   ------------------------------------------------------------- */
#elif defined(_AIX)
   #define M_AIX

   #ifndef M_STDLIB
      #define M_STDLIB
   #endif

   #ifdef _AIX51
      #define M_HASBOOL
   #endif

   #define uint8  u_int8
   #define uint16 u_int16
   #define uint32 u_int32
   #define uint64 u_int64
   #define atoint64 atoll

   #define NO_CPP_NEW_ARRAY
   #define APPLAPI
   #define DLLEXPORT_STDCALL(type) type
   #define DLLEXPORT_CDECL(type) type
   #define APC
   #define O_BINARY 0

   #define SYSLOG(priority, msg) syslog(priority, "%s FILE - %s LINE - %d", msg, __FILE__, __LINE__)

   #include <pthread.h>
   #include <stdlib.h>
   #include <sys/inttypes.h>
   #ifdef __cplusplus
      extern "C" {
   #endif
      #include <unistd.h>

   #ifdef __cplusplus
      }
   #endif
#else
   #error UNKNOWN MACHINE
#endif
/* -------------------------------------------------------------
// The following story with bool is a very sad one. The problem
// revolves around false vs false and true vs true. The true
// and false need to be stamped out and the correct version of
// true and false need to be adopted. The change I am making here
// will break code that uses true and false only in as much as the
// source needs to rename them to the correct true and false and
// the sense of them does not change. If this is not done then
// the problem that they cause just keeps on perpetuating. I
// must apologize for the original definition.
//
//    Vincent Risi
//
// You of course can use -DFalse=false and -DTrue=true directives
// for your compiles that you do not wish to change the source
// but this is no longer handled within this source here.
//
  ------------------------------------------------------------- */

#ifdef M_STDLIB
   #include <iostream>
   #include <sstream>
   #include <iomanip>
   #include <fstream>
   //using namespace std;
#else
   #include <iostream.h>
   #include <strstrea.h>
   #include <iomanip.h>
   #include <fstream.h>
#endif

#ifndef M_HASBOOL
typedef enum bool
{
   false, true
} bool, *pbool;
#else
#define True true
#define False false
typedef bool                 *pbool;
#endif

typedef signed   char  schar,  *pschar;
typedef char                   *pchar;
typedef void                   *pvoid;
typedef short                  *pshort;
typedef int                    *pint;
typedef long                   *plong;
typedef float                  *pfloat;
typedef double                 *pdouble;

#ifdef M_AIX
   #define ENDIAN 'M'
typedef unsigned char          *puchar;
typedef unsigned short  ushort,*pushort;
typedef unsigned int           *puint;
typedef unsigned long          *pulong;
#elif defined(M_GNU)
   #define ENDIAN 'I'
typedef unsigned char   uchar, *puchar;
typedef unsigned short  ushort,*pushort;
typedef unsigned int    uint,  *puint;
typedef unsigned long   ulong, *pulong;
#else
   #define ENDIAN 'I'
typedef unsigned char  uchar,  *puchar;
typedef unsigned short ushort, *pushort;
typedef unsigned int   uint,   *puint;
typedef unsigned long  ulong,  *pulong;
typedef long double  ldouble,  *pldouble;
#endif

#if !defined(M_AIX) && !defined(M_GNU)
   #define True  true
   #define False false
   #include <windows.h>
#endif

#endif

