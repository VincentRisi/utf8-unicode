#ifndef _XL_IPC_H_
#define _XL_IPC_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_XL_IPC_H_);
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <malloc.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "xlnet.h" // for struct IPCRet

//#define XL_IPC_SOCKET_API	0x0001
//#define XL_IPC_KEY_API		0x0002
//#define XL_IPC_SQL_API		0x0004
#ifndef _NXL_SRV_C_
#ifndef _GEN_SRV_C_
class ExportsIpc
{
private :

	int msqid;

	struct msgbufrx{
		long type;
		char mess[MSGBUFMSGSIZE];// 120/128 as size of message
	}msgb;

	char *msgPtr;
	char *memPtr;
	char errorText[255];
	long msgSize;
	long messageCount;
	struct IPCRet ipcRet;
	void sendNext(int msgType,int msgParm);
	void sendNext(int msgType,int msgParm,char *msgString);
	int GetTargetData(char *DataTarget,int DataSize,int isFetch);

public:

	ExportsIpc();
	~ExportsIpc();
	char *Init(char *Qid);	// Must be first method called
	void Block(int Len,char *ReturnPtr);// Each call appends block to returned message
	void ReturnError(long ErrorValue);	// If called more than once, only last call is used
	void ReturnCount(long CountValue);	// If called more than once, only last call is used
	void ReturnSize(long SizeValue);	// If called more than once, only last call is used
	void ReturnStatus(long StatusValue);// If called more than once, only last call is used
	int QueryRun(char *QueryName,char *DataTarget,int DataSize);
	int QueryExec(char *QueryName,char *DataTarget,int DataSize);
	int QueryClose();
	int Commit();
	int Rollback();
//	int QuerySingleFetch(char *DataTarget,int DataSize);
	int QueryMultiFetch(char *DataTarget,int DataSize);
	char *getErrorText();
	char *memParmPtr;

}
// create an instance of the class - except for xl_ipc.C
#ifndef _XL_IPC_C_
xlipc
#endif

;// keep
//		A Simple example
//
//	#include "xlipc.h"
//
//	char *returnPtr="This is an example";
//	main(int argc, char *argv[])
//	{
//	char *parm;
//	long error=67;
//		parm=xlipc.Parm(argv[1]);
//		printf("Parameters Found=%s\n",parm);
//		xlipc.Block(strlen(returnPtr),returnPtr);
//		xlipc.ReturnError(error);
//	}
#endif
#endif
#endif
