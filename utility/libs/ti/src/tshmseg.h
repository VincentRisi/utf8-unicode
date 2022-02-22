// Filename      :  tshmseg.h
// Description   :  shared memory class, providing memory mapped files
//                  and shared mem
// Subsystem     :  ipc
// Author        :  S M Sena
// Date Created  :  Oct 1994
// $LOG$

#ifndef  __TSHMSEG_H__
#define  __TSHMSEG_H__ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(__TSHMSEG_H__);

#include "errnomsg.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "xcept.h"

CLASSDEF(ShmSeg);
/**
* Creation or Access to shared memory
*/
class tShmSeg
{
public:
  enum eIOMode
  {
    Create = 1,
    Private = 2
  };
private:
  enum shmstate
  {
    open, closed
  };
  key_t mKey;
  int  mShmid, mSize, mShmflag;
  int  mIOMode;
  bool mOwner;
  char *mPath;          // path for ftok
  void *mBase;
  shmstate mState;
  void ctor();
public:
  /**
  * Mapped Files to shared memory segment ctor
  */
  tShmSeg(char * aFile , int aIOMode = 0);
  /**
  * Size=0 and Mode=0 implies that you wish to attach to an
  * existing shm segment identified by Key.
  */
//  tShmSeg(long aKey, int aSize = 0, int aIOMode = 0);
  tShmSeg(key_t aKey, int aSize = 0, int aIOMode = 0);
  /**
  * Size=0 and Mode=0 implies that you wish to attach to an
  * existing shm segment identified by ID and Path.
  */
  tShmSeg(char aID, char *aPath, int aSize = 0, int aIOMode = 0);
  /**
  * destructor cleans up
  */
  ~tShmSeg();
  /**
  * returns a pointer to the shared memory
  */
  void *Open();
  /**
  * Resizes the shared memory
  */
  void Resize(int);
  /**
  * returns the size of the shared memory
  */
  int  Size();
  /**
  * Closes the shared memory
  */
  void Close();
  /**
  * Outputs the share memory debug stuff
  */
  friend  std::ostream& operator << ( std::ostream& os, tShmSeg& a);
};

XCLASSDEF(ShmSeg);
class xShmSeg : public xCept
{
private:
  int unixErrorNo;                      // error number from system call
public:
  xShmSeg(char *aFile, const int aLine, int aErrno, char *aInfo = 0);
};

#define XShmSeg(errno,mtext) xShmSeg(__FILE__,__LINE__,errno,mtext)

#endif
