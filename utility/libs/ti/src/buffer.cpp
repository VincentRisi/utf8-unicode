#include "versions.h"
#define _VERSION_NO "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
COMPILED_VERSION(_VERSION_NO);

/*  module: buffer.c */
/*  author: b.a.fortune */
/*  date:   6 July 1992 */
/*  date:   7 Apr 1993: ported to AIX */
/*  date:   5 June port from SYBASE to ORACLE */

#include "machine.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#define XTALLOC
#include "xtalloc.h"
#include "osfuncs.h"
#include "log.h"
#pragma hdrstop

#define BUFMAIN
#include "buffer.h"

/*
  structure of the buffer:

    array of records of structure:
      short next, void *data -->
        buffer of size sizeof(short) + record size.
    the next field is used to form a single linked list.
    values of next:
      -2       : the record is allocated
      -1       : the record is at the end of the linked list
      0,1,2... : the next node in the free list

*/

#define BUFENDLIST   -1
#define BUFALLOCATED -2

/*  instance variables ----------------------------------------------- */
struct BufThis
{
  int    LogFile;           /*  log file handle */
  char   SystemName[SYSTEM_NAMELEN + 1];  /*  system */
  ushort NoRecords;         /*  no. of records in buffer */
  short  RecordSize;        /*  size of record */
  ushort NodeSize;          /*  size of node including free field */
  pchar  Buffer;            /*  pointer to buffer */
  short  FreeList;             /*  free list */
};

pSFBuffer BufInit(int LogFile, char *SystemName, ushort NoRecords, short RecordSize)
{
  pchar pch;
  ushort us;
  pSFBuffer Self;
  void *p;
  int Err;

  /*  check parameters */
  assert(NoRecords && RecordSize);

  /*  allocate and intiialise Self */
  Self = (pSFBuffer)calloc(1, sizeof(*Self));
  if (!Self)
  {
    Err = BufCallocCBErr;
    RecordError(LogFile, SystemName, Err, BufErrMsg(Err));
    return 0;
  }

  Self->FreeList = BUFENDLIST;

  /*  store instance variables */
  Self->LogFile = LogFile;
  strncpy(Self->SystemName, SystemName, SYSTEM_NAMELEN);
  Self->NoRecords = NoRecords;
  Self->RecordSize = RecordSize;

  /*  allocate array of nodes */
  Self->NodeSize = (sizeof(void *) + sizeof(short));
  Self->Buffer     = (char *) calloc(NoRecords, Self->NodeSize);
  if (!Self->Buffer)
  {
    Err = BufCallocNodesErr;
    RecordError(LogFile, SystemName, Err, BufErrMsg(Err));
    free(Self);
    return 0;
  }

  /*  set up free store pointers */
  Self->FreeList = 0;

  /*  allocate buffers */
  for (us = 0, pch = Self->Buffer; us < NoRecords; us++,
    pch += Self->NodeSize )
  {
    /*  store the next buffer index at first 2 bytes of each node */
    Lvalue(pch, short) = us < NoRecords - 1 ? us + 1 : BUFENDLIST;

    /*  allocate buffer - an extra short to store recno */
    p = calloc(1, sizeof(short) + RecordSize);
    if (!p)    /*  memory leaks - but Self will probably abort application */
    {
      Err = BufCallocBufferErr;
      RecordError(LogFile, SystemName, Err, BufErrMsg(Err));
      free(Self);
      return 0;
    }

    /*  copy index to the first 2 bytes of the segment */
    Lvalue(p, short) = us;

    /*  copy p pointer to the buffer */
    Lvalue(pch + sizeof(short), void *) = p;
  }

  return Self;
}

void BufDone(pSFBuffer Self)
{
  free(Self->Buffer);
  free(Self);
}

void* BufNew(pSFBuffer Self)
{
  pchar pch;
  bool Error = false;
  void *p;

  if (Self->FreeList == BUFENDLIST)
  {
    Error = true;
    goto label_end;
  }
  pch = Self->Buffer + Self->FreeList * Self->NodeSize;
  Self->FreeList = Lvalue(pch, short);
  Lvalue(pch, short) = BUFALLOCATED;

label_end:
  if (Error)
    return 0;
  p = Lvalue(pch + sizeof(short), void *);

  /*  skip the index at the beginning of the record */
  return ((pchar)p + 2);
}

int BufDelete(pSFBuffer Self, void *Record)
{
  int Err = BufOK;
  short NodeNo;
  pchar pch;

  /*  check for null pointer */
  if (!Record)
  {
    Err = BufRecFreeErr;
    RecordError(Self->LogFile, Self->SystemName, Err, BufErrMsg(Err));
    goto label_end;
  }

  /*  we can here retrieve the stored index */
  /*  (if it has been changed we are inconsistent) */
  NodeNo = ((short *)Record)[-1];
  pch = Self->Buffer + Self->NodeSize * NodeNo;

  if (Lvalue(pch, short) != BUFALLOCATED)
  {
    Err = BufRecFreeErr;
    RecordError(Self->LogFile, Self->SystemName, Err, BufErrMsg(Err));
    goto label_end;
  }
  Lvalue(pch, short) = Self->FreeList;
  Self->FreeList = (pch - Self->Buffer) / Self->NodeSize;

label_end:
  return Err;
}

char* BufErrMsg(int Err)
{
  if (!Inrange(Err, 0, BufLastErr))
    Err = BufLastErr;

  return aszBufErr[Err];
}

void BufStatus(pSFBuffer Self, char *Nodes, char *FreeList)
{
  pchar  pch;
  ushort us;
  char BufStr[20];
  short CurrentNo;

  for (us = 0, pch = Self->Buffer;
       us < Self->NoRecords;
       us++, pch += Self->NodeSize)
  {
    Nodes[us] = (char)
      (Lvalue(pch, short) == BUFALLOCATED ? BufAllocated : BufFree);
  }
  Nodes[us] = 0;
  sprintf(FreeList, "%d", Self->FreeList);
  if (Self->FreeList != BUFENDLIST)
    strcat(FreeList, ", ");
  CurrentNo = Self->FreeList;
  while (CurrentNo != BUFENDLIST)
  {
    CurrentNo = Lvalue(Self->Buffer + Self->NodeSize * CurrentNo, short);
    sprintf(BufStr, "%d", CurrentNo);
    strcat(FreeList, BufStr);
    if (CurrentNo != BUFENDLIST)
      strcat(FreeList, ", ");
  }
}


