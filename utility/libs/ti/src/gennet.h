#ifndef _GEN_NET_H_
#define _GEN_NET_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_GEN_NET_H_);

struct IPCRet{
	long Error;	// return value for the w/s api call
	long Count;	// general purpose count to api call
	long Size;	// general purpose size to api call
	long Status;	// general purpose status to api call
};

enum gennet{
LIST1,
LIST2
};
//#define XLRPC_LONG_TYPE 'L'
//#define XLRPC_STRING_TYPE 'S'
//#define XLRPC_NO_TYPE 'N'
#define FUN_GENCS_LONG_TYPE 'L'
#define FUN_GENCS_STRING_TYPE 'S'
#define FUN_GENCS_NO_TYPE 'N'

enum{
GEN_ZERO,
GEN_IPC_BIG_MESSAGE,
GEN_IPC_RPC_RETURN,
GEN_IPC_NEXT,
GEN_IPC_RPC_RESPONSE_SIZE,
GEN_IPC_FROM_SRV_RESPONSE_DATA,
GEN_IPC_RPC_ERROR,
GEN_IPC_QUERY_EXEC,
GEN_IPC_QUERY_FETCH_MULTI,
GEN_IPC_QUERY_FETCH_SINGLE,
GEN_IPC_QUERY_NAME,
GEN_IPC_QUERY_RUN,
GEN_IPC_QUERY_CLOSE,
GEN_IPC_ROLLBACK,
GEN_IPC_COMMIT
};

#define MSGBUFMSGSIZE sizeof(int)+sizeof(int)+120

#endif
