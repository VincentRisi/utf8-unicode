#include "versions.h"
#define _VERSION_NO "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
COMPILED_VERSION(_VERSION_NO);
//
//  Program Name  : tshmseg  Class definition for aix  shared memory
//  Who           : Suriya Sena
//  Date          : 20 oct 1994
//
#include "ti.h"
#include <errno.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>
#include "tshmseg.h"

#ifdef M_STDLIB
using namespace std;
#endif

tShmSeg::tShmSeg(char *aFile, int aIOMode)
: mIOMode(aIOMode)
, mShmflag(SHM_MAP)
{
  mPath = 0;
  mOwner = false;
  if ((mShmid = ::open(aFile, O_SYNC| aIOMode, 0660)) < 0)
    throw XShmSeg(errno, "Contructor : failed to open file to Memory Map");
  mState = closed;
}

tShmSeg::tShmSeg(char aID, char *aPath,int aSize, int aIOMode)
: mKey(aID)
, mSize(aSize)
, mIOMode(aIOMode)
, mShmflag(0)
{
  mPath = strdup(aPath);
  ctor();
}

tShmSeg::tShmSeg(key_t aKey, int aSize,int aIOMode)
: mKey(aKey)
, mSize(aSize)
, mIOMode(aIOMode)
, mShmflag(0)
{
  mPath = 0;
  ctor();
}

tShmSeg::~tShmSeg()
{
  if (mPath)
    free(mPath);
  if (mState == open)
    Close();
  if (mOwner)   //  no error chk cos can't throw exception
    shmctl(mShmid , IPC_RMID , 0);
}

void tShmSeg::ctor()
{
  char *p;
  int  IpcFlags;
  if (mPath)
  {
    mKey = ftok (mPath, mKey);
    free(mPath);
    mPath = 0;
  }
  if (mIOMode & Create) // create exclusivly ie. if it exist throw error
  {
    p="Constructor : shmget failed trying to Create shared memory segment";
    IpcFlags=IPC_CREAT|IPC_EXCL|0660;
    mOwner = true;
  }
  else
  {
    p="Constructor: shmget failed trying to Attach to shared memory segment";
    IpcFlags=0;
    mOwner = false;
  }
  if (mIOMode & Private)
    IpcFlags|=IPC_PRIVATE;
  if ((mShmid = shmget(mKey, mSize, IpcFlags)) < 0)
    throw XShmSeg(errno, p);
  mState = closed;
}


void * tShmSeg::Open()
{
  if (mState == open)
    throw XShmSeg(errno,"Open : shared memory segment already open!!");
  if ((mBase = shmat(mShmid,(char *)  NULL , mShmflag)) < 0)
    throw XShmSeg(errno,"Open : unable to attach to shared memory segment");
  mState = open;
  return mBase;
}

void tShmSeg::Close()
{
  if (shmdt((char *) mBase)  < 0)
    throw XShmSeg(errno,"Close : unable to detach from shared memory segment");
  mState = closed;
}

void tShmSeg::Resize(int aSize)
{
  struct shmid_ds stats;
  stats.shm_segsz = aSize;
  if (shmctl (mShmid, SHM_SIZE, &stats) < 0)
    throw XShmSeg(errno,"Resize : shmctl error ");
}

int tShmSeg::Size()
{
  struct shmid_ds stats;
  if (shmctl (mShmid, IPC_STAT, &stats) < 0)
    throw XShmSeg(errno,"Size : shmctl error ");
  return stats.shm_segsz;
}

ostream & operator <<(ostream & os, tShmSeg & aS)
{
  struct shmid_ds stats;
  if (shmctl (aS.mShmid, IPC_STAT, &stats) < 0)
  {
    os << "shmctl :: Failed on mShmid" << aS.mShmid << endl;
  }
  else
  {
    os << "SHARED MEMORY SEGMENT VITAL STATISTICS" << endl
       << "Segment Size            (shm_segsz) " << stats.shm_segsz                   << endl
       << "Pid of last shmop        (shm_lpid) " << stats.shm_lpid                    << endl
       << "Pid of creator           (shm_cpid) " << stats.shm_cpid                    << endl
       << "Current No Attached    (shm_nattch) " << stats.shm_nattch                  << endl
       << "In Memory No Attached (shm_cnattch) " << stats.shm_cnattch                 << endl
       << "Time of Last attach     (shm_atime) " << ctime((time_t *)&stats.shm_atime) << endl
       << "Time of Last detach     (shm_dtime) " << ctime((time_t *)&stats.shm_dtime) << endl
       << "Time of Last change     (shm_ctime) " << ctime((time_t *)&stats.shm_ctime) << endl
       << "Owners ID            (shm_perm.uid) " << stats.shm_perm.uid                << endl
       << "Owners Group         (shm_perm.gid) " << stats.shm_perm.gid                << endl
       << "Creators ID         (shm_perm.cuid) " << stats.shm_perm.cuid               << endl
       << "Creators Group      (shm_perm.cgid) " << stats.shm_perm.cgid               << endl
       << "Permissions         (shm_perm.mode) " << stats.shm_perm.mode               << endl
       << "Slot Sequence No     (shm_perm.seq) " << stats.shm_perm.seq                << endl
       << "Key                  (shm_perm.key) " << (long) stats.shm_perm.key         << endl;
  }
  return os;
}

xShmSeg::xShmSeg(char * aFile,const int aLine, int aErrno, char * aInfo) :
xCept(aFile, aLine, "xShmSeg",aErrno)
{
  //AssignError("Unix Sytem Call Error %s\n%s\n", errnomsg(aErrno), aInfo);
    osErr << "Unix Sytem Call Error " << errnomsg(aErrno) << endl;
    osErr << aInfo << endl;
    osErr << ends;
}





