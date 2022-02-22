#ifndef _SQLAPI_H_
#define _SQLAPI_H_
#include "machine.h"
#include "sqlbin.h"

const unsigned int ImageMax = 64000U;

typedef struct tSqlCB     tSqlCB,     *pSqlCB;
typedef struct tSqlCursor tSqlCursor, *pSqlCursor;

const int32 CBSignature = 0x0FF0BEAD;
const int32 CursorSignature = 0x0FF0DEAD;

enum eSqlErr
{
    SqlApiOK = 0, SqlApiFirstErr = 8001,    //    0
    SqlApiMemErr = SqlApiFirstErr,          //    8001
    SqlApiBinFileErr,                       //    8002
    SqlApiConnectErr,                       //    8003
    SqlApiNoQueryErr,                       //    8004
    SqlApiOpenErr,                          //    8005
    SqlApiParseErr,                         //    8006
    SqlApiBindErr,                          //    8007
    SqlApiDefineErr,                        //    8008
    SqlApiExecErr,                          //    8009
    SqlApiFetchErr,                         //    8010
    SqlApiCancelErr,                        //    8011
    SqlApiCloseErr,                         //    8012
    SqlApiCommitErr,                        //    8013
    SqlApiRollbackErr,                      //    8014
    SqlApiNoMoreRows,                       //    8015
    SqlApiCBErr,                            //    8016
    SqlApiCursorErr,                        //    8017
    SqlApiStateErr,                         //    8018
    SqlApiAllocEnvErr,                      //    8019
    SqlApiAllocConnectErr,                  //    8020
    SqlApiUnsupportedErr,                   //    8021
    SqlApiNoFldname,                        //    8022
    SqlApiKeyFileErr,                       //    8023
    SqlApiNoMoreCBHandles,                  //    8024
    SqlApiNoMoreCursorHandles,              //    8025
    SqlApiLogoffErr,                        //    8026
    SqlApiCommitBarred,                     //    8027
    SqlApiPingErr,                          //    8028
    SqlApiLastErr                           // please leave this one as the last
};

char *GetSqlError(int errNo);

#ifndef __STDC__
#define __STDC__  1
#endif
//typedef void Lda_Def;
#include "oci.h"

#ifdef min
#undef min
#undef max
#endif

struct tSqlCB
{
    int32     Signature;
    pSqlBin   SqlBin;
    char      isVB;
    int       Error;
    sword     RC;
    sb4       ociErrCode;
    char     *ErrorMsg;
    text      *ociBuf;
    OCIEnv    *ociEnv;
    OCIError  *ociError;
    OCISvcCtx *ociSvcCtx;
    int       timeout; // in seconds
    char     *BarCommitsFile;
    uchar    *XUser;
};

struct tSqlCursor
{
    int32      Signature;
    pSqlCB    SqlCB;
    pSqlQuery SqlQuery;
    int       Error;
    int       NoBinds;
    int       NoDefines;
    int       NoLobLocators;
    int32      DataSize;
    int32      RowsCount;
    int32      RowIndex;
    int32      RowsDone;
    int32      RowsRead;
    sword     RC;
    sb4       ociErrCode;
    char     *ErrorMsg;
    char     *Command;
    void      *Data;
    void      *Buffer;
    short    *Indicators;
    OCIBind   **ociBinds;
    OCIDefine **ociDefines;
    OCILobLocator **ociLobLocators;
    OCIStmt   *ociStmt;
    int32      BufferSize;
};

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#if defined(M_WIN_31) || defined (M_WIN_BC4) || defined (M_W32)
#define SqlLogon              DBLogon
#define SqlVBLogon            DBVBLogon
#define SqlNetLogon           DBNetLogon
#define SqlLogonCopy          DBLogonCopy
#define SqlLogoff             DBLogoff
#define SqlCommit             DBCommit
#define SqlRollback           DBRollback
#define SqlPing               DBPing
#define SqlRunProc            DBRunProc
#define SqlOpen               DBOpen
#define SqlOpenWQ             DBOpenWQ
#define SqlOpenExec           DBOpenExec
#define SqlExec               DBExec
#define SqlFetch              DBFetch
#define SqlFetchData          DBFetchData
#define SqlFetchMany          DBFetchMany
#define SqlSetNull            DBSetNull
#define SqlTestNull           DBTestNull
#define SqlCancel             DBCancel
#define SqlClose              DBClose
#define SqlEOD                DBEOD
#define SqlCBEOD              DBCBEOD
#define SqlCBConnected        DBCBConnected    
#define SqlDuplErr            DBDuplErr
#define SqlCBDuplErr          DBCBDuplErr
#define SqlCBError            DBCBError
#define SqlCursorError        DBCursorError
#define SqlCBErrorCode        DBCBErrorCode
#define SqlCursorErrorCode    DBCursorErrorCode
#define SqlVBCursorError      DBVBCursorError
#define SqlVBCBError          DBVBCBError
#define SqlVBCommand          DBVBCommand
#define SqlNetCursorError     DBNetCursorError
#define SqlNetCBError         DBNetCBError
#define SqlNetCommand         DBNetCommand
#define SqlRowsFetched        DBRowsFetched
#define SqlRowExists          DBRowExists
#define SqlXTAlloc            DBXTalloc
#define SqlOneWayCipher       DBOneWayCipher
#define SqlSetBarCommitsFile  DBSetBarCommitsFile
#endif

DLLEXPORT_STDCALL(int) SqlLogon(pSqlCB *SqlCB, char *FileName, char *User);
DLLEXPORT_STDCALL(int) SqlLogoff(pSqlCB SqlCB);
DLLEXPORT_STDCALL(int) SqlCommit(pSqlCB SqlCB);
DLLEXPORT_STDCALL(int) SqlRollback(pSqlCB SqlCB);
DLLEXPORT_STDCALL(int) SqlPing(pSqlCB SqlCB);
DLLEXPORT_STDCALL(int) SqlRunProc(pSqlCB SqlCB, char *Query, void *Data);
DLLEXPORT_STDCALL(int) SqlOpenExec(pSqlCB SqlCB, pSqlCursor *SqlCursor, char *Query, void *Data);
DLLEXPORT_STDCALL(int) SqlOpenWQ(pSqlCB SqlCB, pSqlCursor *SqlCursor, pSqlQuery Query, void *Data);
DLLEXPORT_STDCALL(int) SqlSetTimeout(pSqlCB SqlCB, int seconds);
DLLEXPORT_STDCALL(bool) SqlCBEOD(pSqlCB SqlCB);
DLLEXPORT_STDCALL(bool) SqlCBConnected(pSqlCB SqlCB);
//DLLEXPORT_STDCALL(bool) SqlCBDuplErr(pSqlCB SqlCB);

DLLEXPORT_STDCALL(int) SqlOpen(pSqlCB SqlCB, pSqlCursor *SqlCursor, char *Query, void *Data);
DLLEXPORT_STDCALL(int) SqlExec(pSqlCursor SqlCursor);
DLLEXPORT_STDCALL(int) SqlFetch(pSqlCursor SqlCursor);

DLLEXPORT_STDCALL(int) SqlSetNull(pSqlCursor SqlCursor, char *aName, bool aHow);
DLLEXPORT_STDCALL(int) SqlTestNull(pSqlCursor SqlCursor, char *aName, bool *aHow);
DLLEXPORT_STDCALL(int) SqlFetchMany(pSqlCursor SqlCursor, unsigned short *aNo, void *Data);
DLLEXPORT_STDCALL(int) SqlCancel(pSqlCursor SqlCursor);
DLLEXPORT_STDCALL(int) SqlClose(pSqlCursor SqlCursor);
DLLEXPORT_STDCALL(bool) SqlEOD(pSqlCursor SqlCursor);
DLLEXPORT_STDCALL(bool) SqlDuplErr(pSqlCursor SqlCursor);

DLLEXPORT_STDCALL(int) SqlSetBarCommitsFile(pSqlCB SqlCB, const char *FileName);

DLLEXPORT_STDCALL(pchar) SqlCBError(pSqlCB SqlCB);
DLLEXPORT_STDCALL(pchar) SqlCursorError(pSqlCursor SqlCursor);

DLLEXPORT_STDCALL(ushort) SqlCBErrorCode(pSqlCB SqlCB);
DLLEXPORT_STDCALL(ushort) SqlCursorErrorCode(pSqlCursor SqlCursor);

DLLEXPORT_STDCALL(short) SqlOneWayCipher(char *Input, int InpLen,
        char *Output, int OutLen,
        int Seed);

#if defined(M_WIN_31) || defined (M_WIN_BC4) || defined (M_W32)
DLLEXPORT_STDCALL(int) SqlVBLogon(pSqlCB *SqlCB, char *FileName, char *User);
DLLEXPORT_STDCALL(int) SqlNetLogon(pSqlCB *SqlCB, char *FileName, char *User);
DLLEXPORT_STDCALL(int) SqlVBCursorError(pSqlCursor SqlCursor, char *Message, short cbMessage);
DLLEXPORT_STDCALL(int) SqlVBCommand(pSqlCursor SqlCursor, char *Message, short cbMessage);
DLLEXPORT_STDCALL(int) SqlVBCBError(pSqlCB SqlCB, char *Message, short cbMessage);
DLLEXPORT_STDCALL(int) SqlNetCursorError(pSqlCursor SqlCursor, char *Message, short cbMessage);
DLLEXPORT_STDCALL(int) SqlNetCommand(pSqlCursor SqlCursor, char *Message, short cbMessage);
DLLEXPORT_STDCALL(int) SqlNetCBError(pSqlCB SqlCB, char *Message, short cbMessage);
//DLLEXPORT_STDCALL(int) SqlXTAlloc(bool aHow);
#endif

#if defined(M_W32)
DLLEXPORT_STDCALL(int) SqlFetchData(pSqlCursor SqlCursor, void *Data);
#endif

//void After(tSqlCursor *Cursor);
//void Before(tSqlCursor *Cursor);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif

