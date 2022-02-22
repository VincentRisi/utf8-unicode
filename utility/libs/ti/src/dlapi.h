#ifndef _SQLAPI_H_
#define _SQLAPI_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_SQLAPI_H_);

#include "sqlbin.h"

#include "ocidfn.h"
#include "ociapr.h"

#include "SQL.H"
#include "SQLExt.h"

#define APPLAPI FAR PASCAL

enum
{
  SqlApiOK,
  SqlApiMemErr,
  SqlApiBinFileErr,
  SqlApiConnectErr,
  SqlApiNoQueryErr,
  SqlApiOpenErr,
  SqlApiParseErr,
  SqlApiBindErr,
  SqlApiDefineErr,
  SqlApiExecErr,
  SqlApiFetchErr,
  SqlApiCancelErr,
  SqlApiCloseErr,
  SqlApiCommitErr,
  SqlApiRollbackErr,
  SqlApiNoMoreRows,
  SqlApiLastErr,
  SqlApiAllocEnvErr,
  SqlApiAllocConnectErr,
  SqlSetAutoCommitErr,
  SqlApiCursorErr,
  SqlApiCBErr
};

typedef struct tSqlCB     tSqlCB,     *pSqlCB;
typedef struct tSqlCursor tSqlCursor, *pSqlCursor;
typedef struct cda_def    tCDA,       *pCDA;
typedef struct cda_def    tLDA,       *pLDA;

extern ushort ImageMax;

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
};


int APPLAPI DBLogon(pSqlCB *SqlCB, pchar FileName, pchar Server, pchar User, pchar Pwd);
int APPLAPI DBLogoff(pSqlCB SqlCB);
int APPLAPI DBCommit(pSqlCB SqlCB);
int APPLAPI DBRollback(pSqlCB SqlCB);
int APPLAPI DBRunProc(pSqlCB SqlCB, pchar Query, void* Data);

int APPLAPI DBOpen(pSqlCB SqlCB, pSqlCursor *SqlCursor, pchar Query, void* Data);
int APPLAPI DBOpenWQ(pSqlCB SqlCB, pSqlCursor *SqlCursor, pSqlQuery Query, void* Data);
int APPLAPI DBOpenExec(pSqlCB SqlCB, pSqlCursor *SqlCursor, pchar Query, void* Data);
int APPLAPI DBExec(pSqlCursor SqlCursor);
int APPLAPI DBFetch(pSqlCursor SqlCursor);
int APPLAPI DBCancel(pSqlCursor SqlCursor);
int APPLAPI DBClose(pSqlCursor SqlCursor);

int APPLAPI DBEOD(pSqlCursor SqlCursor);

int APPLAPI DBCBError(pSqlCB SqlCB, char * Message, int cbMessage);
int APPLAPI DBCursorError(pSqlCursor SqlCursor, char * Message, int cbMessage);

#endif

