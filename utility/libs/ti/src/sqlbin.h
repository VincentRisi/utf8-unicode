#ifndef _SQLBIN_H_
#define _SQLBIN_H_

#include "machine.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

typedef struct SqlToken tSqlToken, *pSqlToken;
typedef struct SqlQuery tSqlQuery, *pSqlQuery;
typedef struct SqlField tSqlField, *pSqlField;
typedef struct SqlBin    tSqlBin,  *pSqlBin;

enum { fieldIsPrimaryKey   = 0x8000
     , fieldIsNull         = 0x4000
     , fieldIsInput        = 0x2000
     , fieldIsBound        = 0x1000
     , fieldIsOutput       = 0x0800
     , fieldIsWork         = 0x0400
     , fieldIsRowSequence  = 0x0200
     , fieldIsSequence     = 0x0100
     , fieldIsNullable     = 0x0010
     };

struct SqlField
{
  char *  Name;
  short  CType;
  short  SqlType;
  uint32         Size;
  uint32         Offset;
  uint32         Precision;
  uint32         Scale;
  unsigned short isBind;
  unsigned short isDefine;
};

struct SqlQuery
{
  char *     Name;
  unsigned short    ServerNo;
  unsigned short    SchemaNo;
  unsigned short    TableNo;
  unsigned short    NoFields;
  pSqlField Fields;
  unsigned short    Size;
  char *     Command;
  unsigned short    isSql;
  unsigned short    isFetch;
  unsigned short    isMultiFetch;
  unsigned short    isNullEnabled;
};

struct SqlToken
{
  char *     Name;
  char *     Value;
};

struct SqlBin
{
  unsigned short     NoQueries;
  pSqlQuery  Queries;
  unsigned short     NoServers;
  char *      *Servers;
  unsigned short     NoSchemas;
  char *      *Schemas;
  unsigned short     NoTables;
  char *      *Tables;
  short      Error;
  char *      ErrorMsg;
  unsigned short     NoTokens;
  pSqlToken  Tokens;
};

DLLEXPORT_STDCALL(int) SqlBinOpen(pSqlBin *SqlBin, char * FileName);
// Reads Binary File into MEMORY and Closes File.
DLLEXPORT_STDCALL(int) SqlBinQuery(pSqlBin SqlBin, pSqlQuery *SqlQuery, char * QueryName);
// Retrieves the query from MEMORY
DLLEXPORT_STDCALL(int) SqlBinToken(pSqlBin SqlBin, pSqlToken *SqlToken, char * TokenName);
// Retrieves the token from MEMORY
DLLEXPORT_STDCALL(int) SqlBinClose(pSqlBin SqlBin);
// Frees up the MEMORY.

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif

