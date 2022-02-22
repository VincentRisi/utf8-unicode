#ifndef _XL_NET_H_
#define _XL_NET_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_XL_NET_H_);

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
#define FUN_GENCS_STRING_TYPE 'S'
#define FUN_GENCS_NO_TYPE 'N'

enum{
XL_ZERO,
XL_IPC_BIG_MESSAGE,
XL_IPC_RPC_RETURN,
XL_IPC_NEXT,
XL_IPC_RPC_RESPONSE_SIZE,
XL_IPC_FROM_SRV_RESPONSE_DATA,
XL_IPC_RPC_ERROR,
XL_IPC_QUERY_EXEC,
XL_IPC_QUERY_FETCH_MULTI,
XL_IPC_QUERY_FETCH_SINGLE,
XL_IPC_QUERY_NAME,
XL_IPC_QUERY_RUN,
XL_IPC_QUERY_CLOSE,
XL_IPC_ROLLBACK,
XL_IPC_COMMIT
};

#define MSGBUFMSGSIZE sizeof(int)+sizeof(int)+120

#endif
