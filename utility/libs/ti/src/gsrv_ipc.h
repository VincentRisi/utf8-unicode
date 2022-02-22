#ifndef _GSRV_IPC_H_
#define _GSRV_IPC_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_GSRV_IPC_H_);
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <malloc.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "gsrv_net.h" // for struct IPCRet

#include "xstring.h"
#define STRNCPY(a,b) (strncpyz(a,b,sizeof(a)-1))
#define RECSET(a) (memset((char*)&a,0,sizeof(a)))
#define QCOPY(qclass,col,val) STRNCPY(qclass.row.col,val)
#define QMOVE(col,val) Qmove(row.col,val,sizeof(row.col))
#define QCLASS(query) class query{\
   public:\
   t##query row;\
   query()\
   {\
      RECSET(row);\
   }\
   void Qmove(char *dest,char *source,int n)\
   {\
      strncpy(dest,source,n);\
   }\
   void Qmove(short dest,short source,int n)\
   {\
      dest=source;\
   }\
   void Qmove(long *dest,long *source,int n)\
   {\
      dest=source;\
   }\
   void Qmove(void *dest,void *source,int n)\
   {\
      memmove(dest,source,n);\
   }\
   int Query()\
   {\
      return GENSRVIPC.QueryExec(#query, (char*)&row, sizeof(t##query));\
   }\
   int RunProc()\
   {\
      return GENSRVIPC.QueryRun(#query, (char*)&row, sizeof(t##query));\
   }\
}

#ifndef _NXL_SRV_C_
#ifndef _GEN_SRV_C_

class GenSrvIpc
{
public:

	GenSrvIpc();
	~GenSrvIpc();
	char *Init(char *Qid);	// Must be first method called and before time out
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


private :

	int msqid;

	struct msgbufrx{
		long type;
		char mess[MSGBUFMSGSIZE];// 120/128 as size of message
	}msgb;

	char *msgPtr;
	char *memParmPtr;
	char *memPtr;
	char errorText[255];
	long msgSize;
	long messageCount;
	struct IPCRet ipcRet;
	void sendNext(int msgType,int msgParm);
	void sendNext(int msgType,int msgParm,char *msgString);
	int GetTargetData(char *DataTarget,int DataSize,int isFetch);
};

//		A Simple example app
//
//	#include "gsrv_ipc.h"
//
// Allow substitutions of "xlipc" into macros
// #define GENSRVIPC xlipc
// GenSrvIpc xlipc;
//
// #include "MyTable.sh"
// QCLASS(QueryFromMyTable);
//
//	char *returnPtr="This is an example";
//
//	main(int argc, char *argv[])
//	{
//	char *parm;
//	long error=67;
// QueryFromMyTable qry; // row data memset to 0
//		parm=xlipc.Init(argv[argc-1]);         // Don't delay call to Init
//		printf("Parameters Found=%s\n",parm);  // else Server may timeout
//		xlipc.Block(strlen(returnPtr),returnPtr);
//    QCOPY(qry,dataColumn,"Test"); // initial string values
//    qry.row.testNo=1; // initial integer values
//    qry.Query(); // macro fills in structures & sizeof() etc.
//    while(xlipc.QueryMultiFetch())
//    {
//       xlipc.Block(sizeof(qry.row),&qry.row);
//    }
//    xlipc.QueryClose();
//		xlipc.ReturnError(error);
//	}

#endif
#endif
#endif
