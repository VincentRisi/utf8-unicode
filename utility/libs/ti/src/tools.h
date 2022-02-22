/*
 * $Header: /cvsroot/libs/libti/src/tools.h,v 412.1 2004/11/18 10:26:54 vince Exp $
 * $Author: vince $
 * $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $
 * $Revision: 92 $
*/
#ifndef  _TOOLS_H_
#define  _TOOLS_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_TOOLS_H_);

// macros to test real numbers for 0, up to a small real value epsilon
#define EPSILON 1E-10
#define ISZERO(val) IsZero(val, EPSILON)
#define IsZero(val, epsilon) (-(epsilon) < (val) && (val) < (epsilon))

// macro to check that xmin <= x <= xmax
#define Inrange(x, xmin, xmax) (((xmin) <= (x)) && ((x) <= (xmax)))

#define Swap(a, b) ((a) ^= (b) ^= (a) ^= (b))

#if !defined(M_AIX)

#define Swap2(a)\
  do\
  {\
    char* p = (char*)a;\
    Swap(p[0], p[1]);\
  } while (0)

#define Swap4(a)\
  do\
  {\
    char* p = (char*)a;\
    Swap(p[0], p[3]);\
    Swap(p[1], p[2]);\
  } while (0)

#define Swap8(a)\
  do\
  {\
    char* p = (char*)a;\
    Swap(p[0], p[7]);\
    Swap(p[1], p[6]);\
    Swap(p[2], p[5]);\
    Swap(p[3], p[4]);\
  } while (0)

#else

#define Swap2(a)
#define Swap4(a)
#define Swap8(a)

#endif

// renamed from Clear as this breaks vendor includes -- Vince.Risi
// you will have to rename the Clear in your application
// or put a #define MemoryClearMacro Clear after
// include "tools.h"
#define MemoryClearMacro(lvalue) memset(&(lvalue), 0, sizeof(lvalue))
#define Pad(array) strrpad(array, sizeof(array) - 1, ' ')
#define strcpytrim(s, t) strrtrim(strcpy(s, t))

// returns the size of a field in a struct
#define SizeofFld( type, fld ) sizeof(((type *) 0)->fld)

#define NoElems(a) (sizeof(a) / sizeof(a[0]))

// converts a pointer into an lvalue of a particular type
#define Lvalue(pData, type) (*(type *)(pData))

#ifdef M_OS2

// typedef for a thread function
typedef void(_cdecl *THREADFN)(void *);

// error return from _beginthread function
#define THREAD_ERROR -1
#endif

// macro to blank a string to nulls
#define BlankStr(sz) memset(sz, 0, sizeof(sz))

// macro to blank a variable to nulls
#define BlankVar(v) memset(&v, 0, sizeof(v))

// macro to write a single debug argument to a file
#define WriteDebug(fname, format, arg) \
  do\
  { \
    FILE* f = fopen(fname, "a"); \
    fprintf(f, format, arg); \
    fclose(f); \
  } while(0)

// PETER 3/7/97 - removed as i cannot get valuesys.h to compile with
// this line in conflicts with xcept.h method Error
//#define Error(str) printf("\a%s\n", str)

#define Fatal(str)\
  do {\
    printf("\a%s\n", str);\
    exit(EXIT_FAILURE);\
  }\
  while (0)

#endif

/*
 * $Log: tools.h,v $
 * Revision 412.1  2004/11/18 10:26:54  vince
 * no message
 *
 * Revision 409.1  2004/07/20 09:14:20  vince
 * no message
 *
 * Revision 407.1  2004/06/16 09:41:41  vince
 * no message
 *
 * Revision 405.1  2004/04/16 12:32:57  vince
 * no message
 *
 * Revision 404.1  2004/03/27 09:40:30  vince
 * no message
 *
 * Revision 403.1  2004/02/27 12:10:06  vince
 * no message
 *
 * Revision 401.1  2003/12/02 14:50:13  vince
 * no message
 *
 * Revision 310.1  2003/11/10 10:04:41  vince
 * no message
 *
 * Revision 3.10  2003/11/07 13:54:19  vince
 * no message
 *
 * Revision 1.1.1.1  2003/11/04 14:09:44  vince
 * no message
 *
 * Revision 1.3  2003/09/19 08:54:29  vince
 * no message
 *
 * Revision 1.3  2003/09/18 16:52:13  vince
 * no message
 *
 * Revision 1.2  2002/01/11 13:43:39  vince
 * no message
 *
 * Revision 1.1.1.1  2001/11/09 07:51:46  vince
 * no message
 *
 *
 *    Rev 2.0   18 Jan 1995 14:35:46   unknown
 * Client Server Version
 *
 *    Rev 1.1   14 Nov 1994 09:00:52   unknown
 * Added PVCS Keywords
*/
