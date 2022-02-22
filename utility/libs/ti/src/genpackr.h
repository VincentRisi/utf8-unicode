#ifndef _GENPACKR_H_
#define _GENPACKR_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_GENPACKR_H_);
#ifndef _WINDOWS
#define WINAPI /* */
#define OutputDebug  puts
#endif
#define KEPT_BY_RPC 1
#define KEPT_BY_FTP 2
#define ERROR_DLL_REMOTE_FILE		16


class NetPacker
{
	tRPCClient RPC;
#ifdef _WINDOWS
	HGLOBAL hGlbSendBuf,hGlbRecvBuf;
	DWORD		totalSize;
	DWORD		cbAlloc;
	char huge	*pRxBuffer;
	char huge	*lpvBuffer;
#else
	long totalSize;
	long cbAlloc;
	char *pRxBuffer;
	char *lpvBuffer;
#endif	
//	char huge	*saveSblock;
	long		packLen;
	long	error;// add ERROR_??_REFERENCE to this
	long	errorBase;
	long		remotePacketSize;
	unsigned long	cbRxBuffer;
	int		parameterPacked;
	int		freeSendMemBuffer;
	int		freeRecvMemBuffer;
	long	recordCount;
	long	recordSize;
	long	serverStatus;



public:
	NetPacker(long *Error,char *HostName, char *SocketName,char *SystemCall,long packSize);
	~NetPacker();
	long Pack(short ParmLen, char *Parm);
	long Pack(long ParmLen, char *Parm);
	long Pack(long LongParm);
	long Send(int RequestNum);
	void ReturnSize(long *Size);
	void Block(char *Block);
	long extractAppError();
	void RecordCount(long *Size);
	void RecordSize(long *Size);
#ifdef _WINDOWS
	long memBlock(long *memBlock);
	long fetchftp(long remotePacketSize,int nodelete);
	void fetchBigPacket();
#else
	long memBlock(char **memBlock);
#endif
	void ServerStatus(long *Size);
} ;

#endif
