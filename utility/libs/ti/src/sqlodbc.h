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

#ifndef APPLAPI
#define APPLAPI FAR PASCAL
#endif

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
  int       MaxRows;
  int       RecSize;
  int       RowsFetched;
  void      *Buffer;
};

void APPLAPI DBGetVersionInfo(char *Info, short MaxLen);
int APPLAPI DBLogon(HSQL *hSqlCB, pchar FileName, pchar Server, pchar User, pchar Pwd);
//int APPLAPI DBLogon(HSQL *hSqlCB, pchar FileName, pchar User);
//int APPLAPI DBLogonCopy(HSQL *hSqlCB, pchar FileName, pchar User, pLDA Lda, pchar Hda);
ushort APPLAPI DBCBErrorCode(HSQL hSqlCB);
ushort APPLAPI DBCursorErrorCode(HSQL hSqlCursor);
int APPLAPI DBLogoff(HSQL *hSqlCB);
int APPLAPI DBCommit(HSQL hSqlCB);
int APPLAPI DBRollback(HSQL hSqlCB);
int APPLAPI DBRunProc(HSQL hSqlCB, pchar Query, void* Data);
int APPLAPI DBRunProcMulti(HSQL hSqlCB, pchar Query, void* Data,int NoRecs);
int APPLAPI DBOpen(HSQL hSqlCB, HSQL *hSqlCursor, pchar Query, void* Data);
int APPLAPI DBOpenMulti(HSQL hSqlCB, HSQL *hSqlCursor, pchar Query, void* Data,int MaxRows);
int APPLAPI DBOpenExec(HSQL hSqlCB, HSQL *hSqlCursor, pchar Query, void* Data);
int APPLAPI DBOpenWQ(HSQL hSqlCB, HSQL *hSqlCursor, pSqlQuery Query, void* Data);
int APPLAPI DBExec(HSQL hSqlCursor);
int APPLAPI DBFetch(HSQL hSqlCursor);
int APPLAPI DBFetchMulti(HSQL hSqlCursor);
int APPLAPI DBCancel(HSQL hSqlCursor);
int APPLAPI DBCloseMulti(HSQL hSqlCursor);
int APPLAPI DBClose(HSQL hSqlCursor);
int APPLAPI DBRowsFetched(HSQL hSqlCursor);
bool APPLAPI DBRowExists(HSQL hSqlCB,pchar aQuery, void* aKey);
bool APPLAPI DBEOD(HSQL hqlCursor);
bool APPLAPI DBCBEOD(HSQL hSqlCB);
pchar APPLAPI DBCBError(HSQL hSqlCB);
pchar APPLAPI DBCursorError(HSQL hSqlCursor);

ushort SqlQuerySize(HSQL SqlCB,pchar aQuery);

// Sets the method of network byte ordering to be used - see enum eNetHow
// for a description of the valid values.
void APPLAPI DBSetNetHow(eNetHow aNetHowNow);

int APPLAPI DBVBLogon(HSQL *hSqlCB, pchar FileName, pchar User);
int APPLAPI DBVBCursorError(HSQL hSqlCursor, char * Message, int cbMessage);
int APPLAPI DBVBCBError(HSQL SqlCB, char * Message, int cbMessage);

void APPLAPI DBError(HSQL hSqlCursor,char *State, long *Error,char *Msg,short MsgMax);

void APPLAPI DBUsageCount(short *NoCBs, short *NoCursors);

int APPLAPI DBGetInfo(HSQL hSqlCB, unsigned short InfoType, char *InfoBuff,
   short MaxLength);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif

