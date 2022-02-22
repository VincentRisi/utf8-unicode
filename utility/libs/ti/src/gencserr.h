#ifndef WHY_NO_GUARD_GENCSERR_H_
#define WHY_NO_GUARD_GENCSECR_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(WHY_NO_GUARD_GENCSECR_H_);

// Generic Client Server Errors
#include "errbase.h"
#include "request.h"
enum {
 ERROR_GENCS_APP_TIMEOUT=ERROR_GENCS_BASE+1, // 4101 - App not initialised
 ERROR_GENCS_IPC_RPC_ERROR,                  // 4102 not used ?
 ERROR_GENCS_TEMPFILE,                       // Can't read/create temp file
 ERROR_GENCS_FILE,
 ERROR_GENCS_LONG_PACKET,
 ERROR_GENCS_MEMORY,                         // Server out of Memory
 ERROR_GENCS_FUNCTION_PARAMS,
 ERROR_GENCS_DLL_REMOTE_FILE,
 ERROR_GENCS_DLL_OUT_OF_MEMORY,
 ERROR_GENCS_SEE_ERROR_LOG
};

#endif
