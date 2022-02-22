#ifndef _DATABLCK_H_
#define _DATABLCK_H_

// ---------------------------------
// typedefs for large blocks of data
// ---------------------------------

#if defined(_WIN32)
#define HUGELYBIG
#elif defined(_WINDOWS)
#define HUGELYBIG huge
#else
#define HUGELYBIG
#endif

typedef char HUGELYBIG * DATABLOCK;
typedef int32 DATALEN;

#endif
