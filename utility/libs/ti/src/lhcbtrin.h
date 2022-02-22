#ifndef _LHCBTRIN_H_
#define _LHCBTRIN_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_LHCBTRIN_H_);

#include "linghead.h"

typedef enum
{
  procDateAdd,
  procDate6to8,
  procNumber,
  procPos,
  procSubString,
  procCurrentDate,
  procTimeNo,
  procLog,
  procWrite,
  procWriteLn,
  procCurrentTime,
  procString,
  procGetFapCurrCode,
  procChr,
  procLength,
  procFixComma,
  proc_Last
} eIntrinsicProcs;

tByte  LhIntrinsicRoutine(pChar s, pChar callargs, pSmallint routine);
pChar LhIntrinsicRoutineError(tSmallint no);
pChar LhIntrinsicRoutineName(tSmallint no);

#endif
