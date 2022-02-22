/////////////////////////////////////////////////////////////////////////
//
//  System : SQLODBC
//
//  File   : sqlhnd.h
//
//  Description : Class for returning handles to data.
//
//  Author : J.Pickup
//
//  Date   : 11/94
//
//  Change History :
//
/////////////////////////////////////////////////////////////////////////
// $Author: vince $
#ifndef _SQLHND_H_
#define _SQLHND_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_SQLHND_H_);

#include <windows.h>       // Needed for the definition of HTASK

#define HSQL long          // The type used for SQL handles,
                           // This only really needs to be a short
                           // but since VB used a long to store
                           // the far pointer orginally, using long
                           // means no recompiles...

// Use a non-zero value for the first handle so that if uninitialised
// VB variables (==0) are passed as handles they will not be valid.
#define HANDLE_BASE 1000

struct tEntry
{
  void  *DataPtr;          // Pointer to the storage area in the
                           // data block
  short Used;              // Used flag (0=No,1=Yes)
  HTASK Caller;            // The caller's task handle - used to
                           // check the validity of a handle
};

class tSqlHandle
{
private:
  void   *Data;                 // Pointer to data block
  tEntry *Entry;                // Pointer to Entries array
  short   ElemSize;             // Size of elements in the data block
  short   ArraySize;            // Max number of entries
  long    HandleBase;           // Base for handles
  HSQL    FindFirstFree(void);  // Returns the index of the first
                                // available handle
public:
  tSqlHandle(short DataSize,short nickMaxHandles=50,long HandleBase=HANDLE_BASE);
  ~tSqlHandle();
  HSQL Create(void *Element);   // Returns a new handle, putting Element
                                // into the data array
  void *Use(HSQL Handle);       // Given a handle, returns the data from
                                // the array (if valid)
  void Release(HSQL Handle);    // Releases the resoures allocated for a
                                // given handle
  int NoInUse();                // Returns the number of handles
                                // currently in use.
};

#endif

