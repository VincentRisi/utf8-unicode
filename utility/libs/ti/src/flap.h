#ifndef __FLAP_H
#define __FLAP_H "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(__FLAP_H);

// flap limits
#define MAX_CURSORS                 50
#define MAX_COMMAND_LENGTH        1500

// error messages returned by flap
enum
{
  FLAP_ERROR_FIRST = 32000,
  FLAP_LOGON_ERR,
  FLAP_MALLOC_ERR,
  FLAP_LOADBIN_ERR,
  FLAP_NOQUERY_ERR,
  FLAP_NOCURSOR_ERR,
  FLAP_HASHPARM_ERR,
  FLAP_ATPARM_ERR,
  FLAP_CONVERT_ERROR,
  FLAP_OOPEN_ERR,
  FLAP_OCLOSE_ERR,
  FLAP_OLOGOF_ERR,
  FLAP_ROLLBACK_ERR,
  FLAP_COMMIT_ERR,
  FLAP_PRECOM_ERR,
  FLAP_FETCH_ERR,
  FLAP_EXEC_ERR,
  FLAP_ODEFIN_ERR,
  FLAP_ERROR_LAST
};

#ifdef MAIN
char *ErrorStr[] =
{
  "no error",
  "logging on to oracle",
  "out of memory",
  "loading data dictionary",
  "loading query from data dictionary",
  "out of cursors",
  "flap # parameter out of range",
  "flap @ parameter out of range",
  "Converting binary value to string",
  "opening a cursor",
  "closing a cursor",
  "logging off from oracle",
  "rolling back transaction",
  "committing transaction",
  "precompiling query",
  "fetching next row of query",
  "executing query",
  "binding output fields",
  "error out of range"
};

#endif

// shared global variables
extern HWND            Hwnd;
extern FILE*           File;

// prototypes ----------------------------------------
// initalize access path
ushort AccessPathInit(
  pchar user,              // user-id
  pchar password,          // password
  pchar connectStr,        // "t:<server>-<dbase>", where t = tcp
  int   mustCommit,        // if true commits after every API call that
                           // modifies the database
  HWND  hwnd,              // window handle for user error reporting.
                           // if null output to stdout
  char* fileName,          // file name for full error reporting
  char* binFname);         // binary file name - data dictionary file

// terminate access path
ushort AccessPathDone(void);

// change the commit flag for all subsequent calls
void SetCommit(bool mustCommit); // if true commits after every API call that
                                 // modifies the database

ushort AccessPathReadUnique(
  pchar queryName,             // key to lookup the query
  pchar Param1, pchar Param2, pchar Param3, pchar Param4,   // input parms
  void* data,                  // output buffer
  ushort dataLen);             // buffer length

ushort AccessPathInsert(
  pchar queryName,             // key to lookup the query
  void* data,                  // input buffer
  ushort dataLen);             // buffer length

ushort AccessPathDelete(
  pchar queryName,             // key to lookup the query
  pchar Param1, pchar Param2, pchar Param3, pchar Param4);  // input parms

ushort AccessPathUpdate(
  pchar queryName,             // key to lookup the query
  pchar Param1, pchar Param2, pchar Param3, pchar Param4,   // input parms
  void* data,                  // input buffer
  ushort dataLen);             // buffer length

ushort AccessPathStartSearch(
  ushort* cursor,
  pchar queryName,             // key to lookup the query
  pchar Param1, pchar Param2, pchar Param3, pchar Param4);  // input parms

ushort AccessPathReadNext(
  ushort cursor,
  void* data,                  // output buffer
  ushort dataLen);             // buffer length

ushort AccessPathStopSearch(
  ushort cursor);

ushort AccessPathRollBack(void);

ushort AccessPathCommit(void);

#endif

