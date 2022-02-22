#include "versions.h"
#define _VERSION_NO "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
COMPILED_VERSION(_VERSION_NO);

#include "machine.h"

#include <stdio.h>
#include <stdlib.h>
#if !defined(M_AIX) && !defined(M_GNU)
#include <io.h>
#endif
#include "xtalloc.h"
#include "xstring.h"
#include "sqlbin.h"
#include "binio.h"
#include "xdir.h"

enum
{
  SqlBinOK
, SqlBinMem0Err
, SqlBinAccErr
, SqlBinMem1Err
, SqlBinSignErr
, SqlBinMem2Err
, SqlBinMem3Err
, SqlBinMem4Err
, SqlBinMem5Err
, SqlBinMem6Err
, SqlBinNoCB
, SqlBinNoQuery
, SqlBinMem7Err
, SqlBinNoToken
};

static char * Errors[] =
{
  "No error"
, "Not enough memory for control block!"
, "Access for file failed!"
, "Not enough memory to buffer file!"
, "Invalid buffer file signature incorrect!"
, "No memory for Server list!"
, "No memory for Schema list!"
, "No memory for Table list!"
, "No memory for Query list!"
, "No memory for Query field list!"
, "No Control Block Passed!"
, "Query not found!"
, "No memory for Tokens list!"
, "Token not found!"
};

static int SearchComp(const char * p1, pSqlQuery p2);
static int SearchComp2(const char * p1, pSqlToken p2);
static tSqlBin ABNSqlBin;

static pSqlBin _SqlBinOpen(char * InFileName);

DLLEXPORT_STDCALL(int) SqlBinOpen(pSqlBin *SqlBin, char * InFileName)
{
  *SqlBin = _SqlBinOpen(InFileName);
  return (*SqlBin)->Error;
}

static unsigned short slinkSignOCI   = 0x0C1B;
static unsigned short slinkSignOCI2  = 0x0C2B;
static unsigned short slinkSignODBC  = 0x0DBB;

static pSqlBin _SqlBinOpen(char * aFileName)
{
  unsigned short i,j, sign;
  int RC=SqlBinOK;
  FILE *BinFile=0;
  char InFileName[DIRMAX_PATH];
  pSqlBin SqlBin;
  long FileSize;

  strncpy(InFileName, aFileName, sizeof(InFileName)-1);
  InFileName[sizeof(InFileName)-1];
  SqlBin = (pSqlBin) malloc(sizeof(tSqlBin));
  if (SqlBin == 0)
  {
    SqlBin = &ABNSqlBin;
    RC = SqlBinMem0Err;
    goto Error;
  }
  memset(SqlBin,0,sizeof(tSqlBin));
  BinFile = fopen(InFileName, "rb");
  if (BinFile == 0)
  {
    free (SqlBin);
    SqlBin = &ABNSqlBin;
    RC = SqlBinAccErr;
    goto Error;
  }
  if (setvbuf(BinFile, 0, _IOFBF, 8192))
  {
    RC = SqlBinMem1Err;
    goto Error;
  }
  FileSize = fseek(BinFile, 0L, SEEK_END);
  fseek(BinFile, 0L, SEEK_SET);
  sign = GetUShort(BinFile);

  if (sign != slinkSignOCI
  &&  sign != slinkSignOCI2
  &&  sign != slinkSignODBC)
  {
    RC = SqlBinSignErr;
    goto Error;
  }
  SqlBin->NoServers = GetUShort(BinFile);
  if((SqlBin->Servers = (char**) calloc(sizeof(char *), SqlBin->NoServers))==0)
  {
    RC = SqlBinMem2Err;
    goto Error;
  }
  for (i=0; i<SqlBin->NoServers; i++)
    SqlBin->Servers[i] = GetString(BinFile);
  SqlBin->NoSchemas = GetUShort(BinFile);
  if((SqlBin->Schemas = (char**) calloc(sizeof(char *), SqlBin->NoSchemas))==0)
  {
    RC = SqlBinMem3Err;
    goto Error;
  }
  for (i=0; i<SqlBin->NoSchemas; i++)
    SqlBin->Schemas[i] = GetString(BinFile);
  SqlBin->NoTables = GetUShort(BinFile);
  if((SqlBin->Tables = (char**) calloc(sizeof(char *), SqlBin->NoTables))==0)
  {
    RC = SqlBinMem4Err;
    goto Error;
  }
  for (i=0; i<SqlBin->NoTables; i++)
    SqlBin->Tables[i] = GetString(BinFile);
  SqlBin->NoQueries = GetUShort(BinFile);
  if((SqlBin->Queries = (pSqlQuery) calloc(sizeof(tSqlQuery), SqlBin->NoQueries))==0)
  {
    RC = SqlBinMem5Err;
    goto Error;
  }
  for (i=0; i<SqlBin->NoQueries; i++)
  {
    pSqlQuery Query      = &SqlBin->Queries[i];
    Query->Name          = GetString(BinFile);
    Query->ServerNo      = GetUShort(BinFile);
    Query->SchemaNo      = GetUShort(BinFile);
    Query->isSql         = Query->SchemaNo & 0x8000;
    Query->isFetch       = Query->SchemaNo & 0x4000;
    Query->isMultiFetch  = Query->SchemaNo & 0x2000;
    Query->isNullEnabled = Query->SchemaNo & 0x1000;
    Query->SchemaNo     &= 0x0FFF;
    Query->TableNo       = GetUShort(BinFile);
    Query->NoFields      = GetUShort(BinFile);
    if (Query->NoFields > 0)
    {
      if((Query->Fields = (pSqlField) calloc(sizeof(tSqlField), Query->NoFields))==0)
      {
        RC = SqlBinMem6Err;
        goto Error;
      }
      for (j=0; j<Query->NoFields; j++)
      {
        pSqlField Field  = &Query->Fields[j];
        Field->Name      = GetString(BinFile);
        Field->CType     = GetShort(BinFile);
        Field->SqlType   = GetShort(BinFile);
        if (sign == slinkSignOCI2)
        {
          Field->Size      = GetULong(BinFile);
          Field->Offset    = GetULong(BinFile);
          Field->Precision = GetULong(BinFile);
          Field->Scale     = GetULong(BinFile);
        }
        else
        {
          Field->Size      = GetUShort(BinFile);
          Field->Offset    = GetUShort(BinFile);
          Field->Precision = GetUShort(BinFile);
          Field->Scale     = GetUShort(BinFile);
        }
        Field->isBind    = GetUShort(BinFile);
        Field->isDefine  = GetUShort(BinFile);
      }
    }
    Query->Size    = GetUShort(BinFile);
    Query->Command = GetExact(Query->Size, BinFile);
  }
  if (ftell(BinFile) < FileSize)
  {
    SqlBin->NoTokens = GetUShort(BinFile);
    if((SqlBin->Tokens = (pSqlToken) calloc(sizeof(tSqlToken), SqlBin->NoTokens))==0)
    {
      RC = SqlBinMem7Err;
      goto Error;
    }
    for (i=0; i<SqlBin->NoTokens; i++)
    {
      pSqlToken Token      = &SqlBin->Tokens[i];
      Token->Name          = GetString(BinFile);
      Token->Value         = GetString(BinFile);
    }
  }
  goto Return;

Error:
  SqlBinClose(SqlBin);

Return:
  if (BinFile) fclose(BinFile);
  SqlBin->Error = RC;
  SqlBin->ErrorMsg = Errors[RC];
  return SqlBin;
}

typedef int (*fptr)(const void*, const void*);
DLLEXPORT_STDCALL(int) SqlBinQuery(pSqlBin SqlBin, pSqlQuery *SqlQuery, char * FileName)
{
  *SqlQuery = (pSqlQuery) bsearch(FileName, SqlBin->Queries,
               SqlBin->NoQueries, sizeof(tSqlQuery),
               (fptr)SearchComp);
  if (*SqlQuery)
    SqlBin->Error = SqlBinOK;
  else
    SqlBin->Error = SqlBinNoQuery;
  SqlBin->ErrorMsg = Errors[SqlBin->Error];
  return SqlBin->Error;
}

DLLEXPORT_STDCALL(int) SqlBinToken(pSqlBin SqlBin, pSqlToken *SqlToken, char * FileName)
{
  *SqlToken = (pSqlToken) bsearch(FileName, SqlBin->Tokens,
               SqlBin->NoTokens, sizeof(tSqlToken),
               (fptr)SearchComp2);
  if (*SqlToken)
    SqlBin->Error = SqlBinOK;
  else
    SqlBin->Error = SqlBinNoToken;
  SqlBin->ErrorMsg = Errors[SqlBin->Error];
  return SqlBin->Error;
}

DLLEXPORT_STDCALL(int) SqlBinClose(pSqlBin SqlBin)
{
  unsigned short i, j;

  if (SqlBin == &ABNSqlBin)
    return 0;

  if (SqlBin)
  {
    if (SqlBin->NoTokens)
    {
      for (i=0; i<SqlBin->NoTokens; i++)
      {
        pSqlToken Token = &SqlBin->Tokens[i];
        if (Token->Name)
        {
          free(Token->Name);
          Token->Name = 0;
        }
        if (Token->Value)
        {
          free(Token->Value);
          Token->Value = 0;
        }
      }
      free(SqlBin->Tokens);
      SqlBin->Tokens = 0;
    }
    if (SqlBin->NoQueries)
    {
      for (i=0; i<SqlBin->NoQueries; i++)
      {
        pSqlQuery Query = &SqlBin->Queries[i];
        if (Query->Name)
        {
          free(Query->Name);
          Query->Name = 0;
        }
        if (Query->Fields)
        {
          for (j=0; j<Query->NoFields; j++)
          {
            pSqlField Field = &Query->Fields[j];
            if (Field->Name)
            {
              free(Field->Name);
              Field->Name = 0;
            }
          }
          free(Query->Fields);
          Query->Fields = 0;
          Query->NoFields = 0;
        }
        if (Query->Command)
        {
          free(Query->Command);
          Query->Command = 0;
        }
      }
      free(SqlBin->Queries);
      SqlBin->Queries = 0;
    }
    if (SqlBin->Servers)
    {
      for (i=0; i<SqlBin->NoServers; i++)
      {
        if (SqlBin->Servers[i])
        {
          free(SqlBin->Servers[i]);
          SqlBin->Servers[i] = 0;
        }
      }
      free(SqlBin->Servers);
      SqlBin->Servers = 0;
    }
    if (SqlBin->Schemas)
    {
      for (i=0; i<SqlBin->NoSchemas; i++)
      {
        if (SqlBin->Schemas[i])
        {
          free(SqlBin->Schemas[i]);
          SqlBin->Schemas[i] = 0;
        }
      }
      free(SqlBin->Schemas);
      SqlBin->Schemas = 0;
      SqlBin->NoSchemas = 0;
    }
    if (SqlBin->Tables)
    {
      for (i=0; i<SqlBin->NoTables; i++)
      {
        if (SqlBin->Tables[i])
        {
          free(SqlBin->Tables[i]);
          SqlBin->Tables[i] = 0;
        }
      }
      free(SqlBin->Tables);
      SqlBin->Tables = 0;
      SqlBin->NoTables = 0;
    }
    free(SqlBin);
    SqlBin = 0;
  }
  else
    return SqlBinNoCB;
  return SqlBinOK;
}

static int SearchComp(const char * p1, pSqlQuery p2)
{
  int n = strlen(p1) > strlen(p2->Name) ? strlen(p2->Name) : strlen(p1);
  return strnicmp(p1, p2->Name, n+1);
}

static int SearchComp2(const char * p1, pSqlToken p2)
{
  int n = strlen(p1) > strlen(p2->Name) ? strlen(p2->Name) : strlen(p1);
  return strnicmp(p1, p2->Name, n+1);
}

