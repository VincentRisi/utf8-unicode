#ifndef _STORE_H_
#define _STORE_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_STORE_H_);

#include "cliserv.h"

enum
{
  storePutData,
  storeGetLastRecord,
  forwardGetData,
  forwardMarkComplete
};

typedef struct
{
  tPipeHeader ph;
  char Data[1];
} tPipe, *pPipe;

typedef struct
{
  long SeqNo;
  ushort Len;
  char Data[1];
} tSFEntry, *pSFEntry;

#endif

