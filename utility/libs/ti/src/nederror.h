#ifndef _UNG_NEDERROR_H_
#define _UNG_NEDERROR_H_

// nederror.h
// barry fortune
// feb 95
// general error message function in static library and dll form

// static library function
void GetErrorMessage(int rc, char *pBuffer, int cbBuffer);

// dll function
#if !defined(M_AIX) && !defined(M_GNU)
#include <windows.h>
extern "C" void PASCAL DllGetErrorMessage(int rc, char *pBuffer, int cbBuffer);
#endif

#endif
