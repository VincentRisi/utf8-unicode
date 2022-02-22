#ifndef _SQLODBC_H_
#define _SQLODBC_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_SQLODBC_H_);

#include "sqlbin.h"
#include "sqlhnd.h"
#include "errstr.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#define API_ODBC

#include "SQL.H"
#include "SQLExt.h"

// Enumeration which defines the available possibilities for translation
// to and from network byte ordering.
// The meanings are:
//   eNone    - no translation.
//   eFromNet - all DB records passed to SQLAPI have their numeric fields
//              converted before use.
//   eToNet   - all DB records produced as a result of a query have their
//              numeric fields converted before being returned.
//   eBoth    - both eFromNet and eToNet.
#ifndef _DEF_NETHOW
#define _DEF_NETHOW
enum eNetHow {eNone, eFromNet, eToNet, eBoth};
#endif

//#define APPLAPI FAR PASCAL

typedef struct tSqlCB     tSqlCB,     *pSqlCB;
typedef struct tSqlCursor tSqlCursor, *pSqlCursor;

struct tSqlCB
{
  HENV    henv;
  HDBC    hdbc;
  pSqlBin SqlBin;
  int     Error;
  pchar   ErrorMsg;
  pchar   MsgBuf;
};

struct tSqlCursor
{
  HSTMT     hstmt;
  pSqlCB    SqlCB;
  pSqlQuery SqlQuery;
  void     *Data;
  int       Error;
  pchar     ErrorMsg;
  pchar     MsgBuf;
  pchar     Command;
  int       MaxRows;
  int       RecSize;
  int       RowsFetched;
  void      *Buffer;
};

void APPLAPI DBGetVersionInfo(char *Info, short MaxLen);
int APPLAPI DBLogon(long *CBHandle, pchar FileName, pchar Server, pchar User, pchar Pwd);
int APPLAPI DBLogoff(long *CBHandle);
int APPLAPI DBCommit(long CBHandle);
int APPLAPI DBRollback(long CBHandle);
int APPLAPI DBRunProc(long CBHandle, pchar Query, void* Data);
int APPLAPI DBRunProcMulti(long CBHandle, pchar Query, void* Data,int NoRecs);
int APPLAPI DBOpen(long CBHandle, long *CursorHandle, pchar Query, void* Data);
int APPLAPI DBOpenMulti(long CBHandle, long *CursorHandle, pchar Query, void* Data,int MaxRows);
int APPLAPI DBOpenExec(long CBHandle, long *CursorHandle, pchar Query, void* Data);
int APPLAPI DBOpenWQ(long CBHandle, long *CursorHandle, pSqlQuery Query, void* Data);
int APPLAPI DBExec(long CursorHandle);
int APPLAPI DBFetch(long CursorHandle);
int APPLAPI DBFetchMulti(long CursorHandle);
int APPLAPI DBCancel(long CursorHandle);
int APPLAPI DBCloseMulti(long CursorHandle);
int APPLAPI DBClose(long CursorHandle);
int APPLAPI DBRowsFetched(long CursorHandle);
bool APPLAPI DBRowExists(long CBHandle,pchar aQuery, void* aKey);
bool APPLAPI DBEOD(long hqlCursor);
bool APPLAPI DBCBEOD(long CBHandle);
pchar APPLAPI DBCBError(long CBHandle);
pchar APPLAPI DBCBMsgBuf(long CBHandle);
pchar APPLAPI DBCursorError(long CursorHandle);
ushort APPLAPI DBCBErrorCode(long CBHandle);
ushort APPLAPI DBCursorErrorCode(long CursorHandle);

ushort SqlQuerySize(long SqlCB,pchar aQuery);

// Sets the method of network byte ordering to be used - see enum eNetHow
// for a description of the valid values.
void APPLAPI DBSetNetHow(eNetHow aNetHowNow);

int APPLAPI DBVBLogon(long *CBHandle, pchar FileName, pchar User);
int APPLAPI DBVBCursorError(long CursorHandle, char * Message, int cbMessage);
int APPLAPI DBVBCBError(long SqlCB, char * Message, int cbMessage);
int APPLAPI DBVBCBMsgBuf(long SqlCB, char * Message, int cbMessage);

void APPLAPI DBError(long CursorHandle,char *State, long *Error,char *Msg,short MsgMax);

int APPLAPI DBGetInfo(HSQL hSqlCB, unsigned short InfoType, char *InfoBuff, short MaxLength);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif

