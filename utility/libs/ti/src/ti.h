// ti.h
// Treasury International Systems - main header file
// Author: Barry Fortune
// Date: 4 May 1994
//
// CLASSDEF macro
// forward class declarations
// common includes
// function Init()

#ifndef _TI_H_
#define _TI_H_ "$Revision: 1.1.1.1 $ $Date: 2005/05/30 14:21:48 $"
#include "versions.h"
HEADER_VERSION(_TI_H_);

  #define STRUCTDEF(Root)       \
  struct t##Root;               \
  typedef t##Root*   p##Root;   \
  typedef t##Root&   r##Root;   \
  typedef t##Root*&  rp##Root

  #define CLASSDEF(Root)       \
  class t##Root;               \
  typedef t##Root*   p##Root;  \
  typedef t##Root&   r##Root;  \
  typedef t##Root*&  rp##Root

  #define XCLASSDEF(Root)      \
  class x##Root;               \
  typedef x##Root*   px##Root; \
  typedef x##Root&   rx##Root; \
  typedef x##Root*&  rpx##Root

XCLASSDEF(Cept);
XCLASSDEF(String);
CLASSDEF(Logstream);
CLASSDEF(String);

  #include "machine.h"
  #include <stdlib.h>
  #include <stddef.h>
  #include "xcept.h"
  #include "tstring.h"

  #if !defined(M_W32)
    #if !defined(_WINDOWS)
      #if defined(M_AIX)
        #define NO_CPP_NEW_ARRAY
      #endif
      #define NEWDEBUG_ON
      #include "newdebug.h"
    #endif
  #endif

void Terminate();
void Unexpected();

// initialization function: must be called at start of main()
void Init();

  #if !defined(M_AIX) && !defined(M_GNU)
    #define True  true
    #define False false
    #include <windows.h>
  #endif

#endif
