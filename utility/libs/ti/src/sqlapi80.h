//#ifndef _SQLAPI80_H_
//#define _SQLAPI80_H_
#error You should not be including this version of the header,
#error change your include path to ...\dev\portal\oci7 for Oracle 7.x.x
#error                          or ...\dev\portal\oci8 for Oracle 8.x.x
#error         before the standard ...\dev\include
//
//#include "sqlbin.h"
//
//enum eSqlErr
//{
//  SqlApiOK = 0, SqlApiFirstErr = 8001,    //    0
//  SqlApiMemErr = SqlApiFirstErr,          //    8001
//  SqlApiBinFileErr,                       //    8002
//  SqlApiConnectErr,                       //    8003
//  SqlApiNoQueryErr,                       //    8004
//  SqlApiOpenErr,                          //    8005
//  SqlApiParseErr,                         //    8006
//  SqlApiBindErr,                          //    8007
//  SqlApiDefineErr,                        //    8008
//  SqlApiExecErr,                          //    8009
//  SqlApiFetchErr,                         //    8010
//  SqlApiCancelErr,                        //    8011
//  SqlApiCloseErr,                         //    8012
//  SqlApiCommitErr,                        //    8013
//  SqlApiRollbackErr,                      //    8014
//  SqlApiNoMoreRows,                       //    8015
//  SqlApiCBErr,                            //    8016
//  SqlApiCursorErr,                        //    8017
//  SqlApiStateErr,                         //    8018
//  SqlApiAllocEnvErr,                      //    8019
//  SqlApiAllocConnectErr,                  //    8020
//  SqlApiUnsupportedErr,                   //    8021
//  SqlApiNoFldname,                        //    8022
//  SqlApiKeyFileErr,                       //    8023
//  SqlApiNoMoreCBHandles,                  //    8024
//  SqlApiNoMoreCursorHandles,              //    8025
//  SqlApiLogoffErr,                        //    8026
//  SqlApiLastErr                           // please leave this one as the last
//};
//
//pchar GetSqlError(int errNo);
//
//#ifndef __STDC__
//#define __STDC__
//#endif
//#include "oci.h"
//
//extern ushort ImageMax;
//
//typedef struct tSqlCB     tSqlCB,     *pSqlCB;
//typedef struct tSqlCursor tSqlCursor, *pSqlCursor;
//
//const long CBSignature = 0x0FF0BEAD;
//const long CursorSignature = 0x0FF0DEAD;
//
//struct tSqlCB
//{
//  long      Signature;
//  pSqlBin   SqlBin;
//  char      isVB;
//  int       Error;
//  sword     RC;
//  sb4       ociErrCode;
//  pchar     ErrorMsg;
//  text      *ociBuf;
//  OCIEnv    *ociEnv;
//  OCIError  *ociError;
//  OCISvcCtx *ociSvcCtx;
//};
//
//struct tSqlCursor
//{
//  long      Signature;
//  pSqlCB    SqlCB;
//  pSqlQuery SqlQuery;
//  int       Error;
//  int       NoBinds;
//  int       NoDefines;
//  long      DataSize; // used for VB4(32) and VB5(32)
//  long      RowsCount;
//  long      RowIndex;
//  long      RowsDone;
//  long      RowsRead;
//  sword     RC;
//  sb4       ociErrCode;
//  pchar     ErrorMsg;
//  pchar     Command;
//  void      *Data;
//  void      *Buffer;
//  pshort    Indicators;
//  OCIBind   **ociBinds;
//  OCIDefine **ociDefines;
//  OCIStmt   *ociStmt;
//};
//
//#if defined (M_W32)
//#define SqlLogon              DBLogon
//#define SqlVBLogon            DBVBLogon
//#define SqlLogonCopy          DBLogonCopy
//#define SqlLogoff             DBLogoff
//#define SqlCommit             DBCommit
//#define SqlRollback           DBRollback
//#define SqlRunProc            DBRunProc
//#define SqlOpen               DBOpen
//#define SqlOpenWQ             DBOpenWQ
//#define SqlOpenExec           DBOpenExec
//#define SqlExec               DBExec
//#define SqlFetch              DBFetch
//#define SqlFetchData          DBFetchData
//#define SqlFetchMany          DBFetchMany
//#define SqlSetNull            DBSetNull
//#define SqlTestNull           DBTestNull
//#define SqlCancel             DBCancel
//#define SqlClose              DBClose
//#define SqlEOD                DBEOD
//#define SqlCBEOD              DBCBEOD
//#define SqlDuplErr            DBDuplErr
//#define SqlCBDuplErr          DBCBDuplErr
//#define SqlCBError            DBCBError
//#define SqlCursorError        DBCursorError
//#define SqlCBErrorCode        DBCBErrorCode
//#define SqlCursorErrorCode    DBCursorErrorCode
//#define SqlVBCursorError      DBVBCursorError
//#define SqlVBCBError          DBVBCBError
//#define SqlVBCommand          DBVBCommand
//#define SqlRowsFetched        DBRowsFetched
//#define SqlRowExists          DBRowExists
//#define SqlXTAlloc            DBXTalloc
//#define SqlOneWayCipher       DBOneWayCipher
//#endif
//
//#ifdef __cplusplus
//extern "C" {
//#endif  /* __cplusplus */
//
//int APPLAPI SqlLogon(pSqlCB *SqlCB, pchar FileName, pchar User);
//int APPLAPI SqlLogoff(pSqlCB SqlCB);
//int APPLAPI SqlCommit(pSqlCB SqlCB);
//int APPLAPI SqlRollback(pSqlCB SqlCB);
//int APPLAPI SqlRunProc(pSqlCB SqlCB, pchar Query, void* Data);
//int APPLAPI SqlOpenExec(pSqlCB SqlCB, pSqlCursor *SqlCursor, pchar Query, void* Data);
//int APPLAPI SqlOpenWQ(pSqlCB SqlCB, pSqlCursor *SqlCursor, pSqlQuery Query, void* Data);
//bool APPLAPI SqlCBEOD(pSqlCB SqlCB);
//bool APPLAPI SqlCBDuplErr(pSqlCB SqlCB);
//
//int APPLAPI SqlOpen(pSqlCB SqlCB, pSqlCursor *SqlCursor, pchar Query, void* Data);
//int APPLAPI SqlExec(pSqlCursor SqlCursor);
//int APPLAPI SqlFetch(pSqlCursor SqlCursor);
//
//int APPLAPI SqlSetNull(pSqlCursor SqlCursor, pchar aName, bool aHow);
//int APPLAPI SqlTestNull(pSqlCursor SqlCursor, pchar aName, bool *aHow);
//int APPLAPI SqlFetchMany(pSqlCursor SqlCursor, pushort aNo, void *Data);
//int APPLAPI SqlCancel(pSqlCursor SqlCursor);
//int APPLAPI SqlClose(pSqlCursor SqlCursor);
//bool APPLAPI SqlEOD(pSqlCursor SqlCursor);
//bool APPLAPI SqlDuplErr(pSqlCursor SqlCursor);
//
//pchar APPLAPI SqlCBError(pSqlCB SqlCB);
//pchar APPLAPI SqlCursorError(pSqlCursor SqlCursor);
//
//ushort APPLAPI SqlCBErrorCode(pSqlCB SqlCB);
//ushort APPLAPI SqlCursorErrorCode(pSqlCursor SqlCursor);
//
//short APPLAPI SqlOneWayCipher(pchar Input, ushort InpLen, pchar Output, ushort OutLen, ushort Seed);
//
//#if defined (M_W32)
//int APPLAPI SqlVBLogon(pSqlCB *SqlCB, pchar FileName, pchar User);
//int APPLAPI SqlVBCursorError(pSqlCursor SqlCursor, char * Message, short cbMessage);
//int APPLAPI SqlVBCommand(pSqlCursor SqlCursor, char * Message, short cbMessage);
//int APPLAPI SqlVBCBError(pSqlCB SqlCB, char * Message, short cbMessage);
//int APPLAPI SqlXTAlloc(bool aHow);
//int APPLAPI SqlFetchData(pSqlCursor SqlCursor, void *Data);
//#endif

void After(tSqlCursor *Cursor);
void Before(tSqlCursor *Cursor);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif

