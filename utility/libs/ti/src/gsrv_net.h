#ifndef _GSRV_NET_H_
#define _GSRV_NET_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_GSRV_NET_H_);

struct IPCRet{
	long Error;	// return value for the w/s api call
	long Count;	// general purpose count to api call
	long Size;	// general purpose size to api call
	long Status;	// general purpose status to api call
};

enum xlnet{
LIST1,
LIST2
};
//#define XLRPC_LONG_TYPE 'L'
//#define XLRPC_STRING_TYPE 'S'
//#define XLRPC_NO_TYPE 'N'
#define FUN_GENCS_LONG_TYPE 'L'
#define FUN_GENCS_NO_TYPE 'N'
#define FUN_GENCS_STRING_TYPE 'S'

enum{
GENCS_ZERO,
GENCS_IPC_BIG_MESSAGE,
GENCS_IPC_RPC_RETURN,
GENCS_IPC_NEXT,
GENCS_IPC_RPC_RESPONSE_SIZE,
GENCS_IPC_FROM_SRV_RESPONSE_DATA,
GENCS_IPC_RPC_ERROR,
GENCS_IPC_QUERY_EXEC,
GENCS_IPC_QUERY_FETCH_MULTI,
GENCS_IPC_QUERY_FETCH_SINGLE,
GENCS_IPC_QUERY_NAME,
GENCS_IPC_QUERY_RUN,
GENCS_IPC_QUERY_CLOSE,
GENCS_IPC_ROLLBACK,
GENCS_IPC_COMMIT
};

#define MSGBUFMSGSIZE sizeof(int)+sizeof(int)+120

#endif
