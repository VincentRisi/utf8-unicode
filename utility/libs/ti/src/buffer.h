#ifndef  _BUFFER_H_
#define  _BUFFER_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_BUFFER_H_);

/*  -------------------------------------------------------------------- */
/*  */
/*  author: b.a.fortune */
/*  date:   6 July 1992 */
/*  date:   7 Apr 1993: ported to AIX */
/*  date:   15 Apr: added logging for errors */
/*  holds a finite buffer of records, all of a fixed size */
/*  prefix: Buf */
/*  handles locking/unlocking internally */
/*  */
/*  -------------------------------------------------------------------- */

#ifndef  _TOOLS_H_
#include "tools.h"
#endif

#define SYSTEM_NAMELEN 10

enum       /*  used for debugging in BufStatus */
{
  BufFree = 'F',
  BufAllocated = 'A'
};

enum     /*  error codes */
{
  BufOK,
  BufFreeErr,
  BufRecFreeErr,
  BufCallocCBErr,
  BufSemErr,
  BufCallocNodesErr,
  BufCallocBufferErr,
  BufLastErr
};

#ifdef BUFMAIN
static char *aszBufErr[] =
{
  "Buffer: ok",
  "Buffer: free buffer error",
  "Buffer: record already freed error",
  "Buffer: allocating control block error",
  "Buffer: AIX semaphore error",
  "Buffer: allocating nodes error",
  "Buffer: allocating buffer error",
  "Buffer: error out of range error"
};
#endif

struct BufThis;
typedef struct BufThis *pSFBuffer;

/*  constructor */
pSFBuffer BufInit(
  int   LogFile,           /*  handle to a log file */
  char *SystemName,        /*  system name */
  ushort NoRecords,        /*  no. of records in the buffer */
  short RecordSize);       /*  size of each record */

/*  destructor */
void BufDone(pSFBuffer Self);

/*  malloc equivalent */
/*  return pointer to new record, 0 if buffer full */
void* BufNew(pSFBuffer Self);

/*  free equivalent */
int BufDelete(pSFBuffer Self, void *Rec);

/*  return an error message */
char* BufErrMsg(int Err);

/*  for debugging - quick and dirty (strings must be long enough). */
/*  does not check for initialization, or lock the monitor */
/*  not to be used in production environment */

void BufStatus(pSFBuffer Self,
  char *Nodes,             /*  string to fill with nodes' status */
  char *FreeList);         /*  string to fill with free list */

#endif
