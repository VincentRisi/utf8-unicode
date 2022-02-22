#ifndef _ERRSTR_H_
#define _ERRSTR_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_ERRSTR_H_);

#include "machine.h"
#include "errbase.h"

enum eSqlErr
{
  SqlApiOK = 0,                           //    0
  SqlApiMemErr = (ERROR_SQLAPI_BASE+1),   //    1
  SqlApiBinFileErr,                       //    2
  SqlApiConnectErr,                       //    3
  SqlApiNoQueryErr,                       //    4
  SqlApiOpenErr,                          //    5
  SqlApiParseErr,                         //    6
  SqlApiBindErr,                          //    7
  SqlApiDefineErr,                        //    8
  SqlApiExecErr,                          //    9
  SqlApiFetchErr,                         //    10
  SqlApiCancelErr,                        //    11
  SqlApiCloseErr,                         //    12
  SqlApiCommitErr,                        //    13
  SqlApiRollbackErr,                      //    14
  SqlApiNoMoreRows,                       //    15
  SqlApiCBErr,                            //    16
  SqlApiCursorErr,                        //    17
  SqlApiStateErr,                         //    18
  SqlApiAllocEnvErr,                      //    19
  SqlApiAllocConnectErr,                  //    20
  SqlApiUnsupportedErr,                   //    21
  SqlApiNoFldname,                        //    22
  SqlApiKeyFileErr,                       //    23
  SqlApiNoMoreCBHandles,                  //    24
  SqlApiNoMoreCursorHandles,              //    25
  SqlApiLastErr          // please leave this one as the last
};

pchar GetSqlError(int errNo);

#endif
