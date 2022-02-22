#ifndef __secadm_h_
#define __secadm_h_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(__secadm_h_);
#include "usr.sh"
#include "ws.sh"
#include "smart.sh"
#include "sqlapi.h"

#ifdef __cplusplus
extern "C" {
#endif

#define XCFP(aaa) extern "C" aaa APPLAPI __loadds

XCFP(long) scLogQuery (mLogQuery far *pLogQuery, mLogRow far *pLogRow);
XCFP(long) scShutdown (void);
XCFP(long) scGetStatus(tRPCStatus far *stat);
XCFP(long) scRequestEnable (void);
XCFP(long) scRequestDisable (void);
XCFP(long) scLoginEnable(void);
XCFP(long) scLoginDisable (void);
XCFP(long) scRefreshCtlTypes (void);
XCFP(long) scUserSelectOne (tUsr far *pUser);
XCFP(long) scUserInsert (tUsr far *pUser);
XCFP(long) scUserDelete (tUsrKey far *pUserKey);
XCFP(long) scUserUpdate (tUsrUpd far *pUser);
XCFP(long) scUserReset (char far *pResetPasswd);
XCFP(long) scSmartCardSelectOne(tSmartCardSelectUsr far *SmartCardSelectUsr);
XCFP(long) scSmartCardInsert(tSmartCard far *SmartCard);
XCFP(long) scSmartCardDelete(tSmartCardKey far *SmartCardKey);
XCFP(long) scWorkStationSelectOne(tWorkstation far *pWorkstation);
XCFP(long) scWorkStationInsert (tWorkstation far *pWorkStation);
XCFP(long) scWorkStationDelete(tWorkstationKey far *pWorkStationKey);
XCFP(long) scWorkStationUpdate(tWorkstationUpd far *pWorkStation);
XCFP(long) scWorkStationReset (char far *pWSReset);
XCFP(long) scDoMac (tSmartCardSelectUsr far *SmartCardSelectUsr);
XCFP(void) scFormatTime(long date, char* strTime, int len);
XCFP(int)  APPLAPI scVBLogon(pSqlCB *SqlCB, pchar FileName);
XCFP(long) scSuspendCardReads(long howlong);
XCFP(void) Encrpt (PTXT NormIn, PTXT Scram, PTXT EncOut);
XCFP(void) Decrpt (PTXT EncIn, PTXT Scram, PTXT NormOut);


#ifdef __cplusplus
}
#endif

#endif
