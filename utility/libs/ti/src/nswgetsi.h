#ifndef _NSWGETSI_H_
#define _NSWGETSI_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_NSWGETSI_H_);
typedef struct {
	char BranchNo[4];
	char TheirRefNo[16];
	char OurRefNo[16];
	char PrevRefNo[16];
	char MessCode[5];
	char SeqNo[6];
	char DateReceived[8];
	char Priority[1];
	char SessionNo[4];
	char Status[1];
	char SenderSwiftAdd[9];
	char Message[1];
}tGetSwin,*pGetSwin;
#endif
