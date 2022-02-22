#ifndef _NSWSWIFT_H_
#define _NSWSWIFT_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_NSWSWIFT_H_);

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
//#include "xl_ipc.h"
#include "machine.h"
#include "ti.h"
#include "nswgetsi.h"
#include "../../include/nswswin.sh"

#include "xstring.h"
#define STRNCPY(a,b) (strncpyz(a,b,sizeof(a)-1))
#define RECSET(a) (memset((char*)&a,0,sizeof(a)))

typedef struct {
	char Begin;
	char Id[2];
	char ApplId;
	char DataType[2];
	char LtAddress[9];
	char BranchCode[3];
	char SessionNumber[4];
	char SequenceNumber[6];
	char End;
}tBasicHdr,*pBasicHdr;

typedef struct {
	char Begin;
	char Id[2];
	char InputOutput;
	char MessageType[3];
	char RecipientAddress[9];
	char BranchCode[3];
	char Priority[1];
	char DeliveryMonitor;
	char ObselescencePeriod[3];
	char End;
}tApplicationInHdr,*pApplicationInHdr;

typedef struct {
	char ProdCode[4];
	char MessVersion[8];
	char SwiftNo[10];
}tMessageIn, *pMessageIn;

void getMessages();
void fillSwiftInBlock();
void getSwiftRef();
void combineMessages();

#endif
