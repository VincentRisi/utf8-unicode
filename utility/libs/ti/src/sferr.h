// FileName: sferr.h
// platform: aix + windows
// Description: header for errors from store and forward service
// SubSystem: infrastructure
// Author: P. Searle
// Date Created: September 1994

#ifndef _SFERR_H_
#define _SFERR_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_SFERR_H_);

#include "machine.h"
#include "errbase.h"

// errors returned by Store and Forward
enum {
  sfOK,                  sfStopped,             sfOverUtilized,
  sfCppException,        sfQFailed,             sfMemory,
  sfItsToLong,           sfNoItem,              sfKeyFilePath,
  sfKeyFileOpen,         sfMCInvalid,           sfTooManyApplKeys,
  sfStatTooSmall,        sfWaitingForTx,        sfDataFileOpen,
  sfDataFileExists,      sfDataFileGone,        sfQidInvalid,
  sfQueueStuck,          sfAlterFailed,         sfLockFailed,
  sfTxOK,                sfTxFailed,            sfNoKeyMatch,
  sfOrderOut,            sfDataFileBad,         sfQueueConstruct,
  sfTaskOversubscribed,  sfAppOverflow,         sfHostOverflow,
  sfServiceOverflow,     sfBuflenMismatch,      sfDropFailed,
  sfInvalidSignature,    sfLastErr
};

char* GetSFError(int aError);

#endif

