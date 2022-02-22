#include "machine.h"

#include <stdio.h>
#include <stdlib.h>
#if defined(M_W32)
#include <io.h>
#endif
#include "xstring.h"
#include "xtalloc.h"

#include "xdir.h"
#include "getargs.h"
#include "binio.h"
#include "sqlbin.h"

#include "slink.h"

//#pragma package(smart_init)

static FILE *LogFile      = 0;

static char *ProjBinFileName = "sqlapi";
static char *FileInList      = "";
static char *BinFile         = "";
static char *PyFile          = "";

static ARG argtab[] =
{
  {'f', STRING, (int*)&FileInList,      "FileName of inputs"},
  {'b', STRING, (int*)&BinFile,         "BinFile to dump"},
  {'p', STRING, (int*)&PyFile,          "PyFile of bin"}
};

#define TABSIZE (sizeof(argtab) / sizeof(ARG))

ushort sqldefSign  = 0x0C1A;
ushort slinkSign   = 0x0C1B;
ushort sqldefSign2 = 0x0C2A;
ushort slinkSign2  = 0x0C2B;
ushort sqldefTailMark = 0xBEEF;

void DumpSOFile(pchar InFileName, ushort &slink)
{
  FILE* InFile;
  long FileSize;
  TSqlQuery Query;
  ushort i, j, NoProcs, sign;
  printf("%-13s: %s\n", "Input", InFileName);
  InFile = fopen(InFileName, "rb");
  if (InFile == 0)
  {
    printf("LoadinFile Access for file [%s] failed!\n", InFileName);
    goto Return;
  }
  if (setvbuf(InFile, 0, _IOFBF, 131072))
    printf("Not enough memory to buffer file!\n");
  FileSize = fseek(InFile, 0L, SEEK_END);
  fseek(InFile, 0L, SEEK_SET);
  sign = getUInt16(InFile);
  if (sign != sqldefSign && sign != sqldefSign2)
  {
    printf("Not a valid SO file!\n");
    goto Return;
  }
  else if (sign == sqldefSign2) // if anywhere a binary is sign2
    slink = slinkSign2;         // then the whole bin must be sign2
  printf("Server    : %s\n", getString(InFile));
  printf("Schema    : %s\n", getString(InFile));
  printf("Table     : %s\n", getString(InFile));
  NoProcs = getUInt16(InFile);
  printf("NoProcs   : %d\n\n", NoProcs);  
  for (i=0; i<NoProcs; i++)
  {
    Query.Name = getString(InFile);
    Query.NoFields = getUInt16(InFile);
    Query.isSql = Query.NoFields & 0x8000;
    Query.isSingle = Query.NoFields & 0x4000;
    Query.isManyQuery = Query.NoFields & 0x2000;
    Query.isNullEnabled = Query.NoFields & 0x1000;
    Query.NoFields &= 0x0FFF;
    printf("%2d: %-32s %d %s%s%s\n"
          , i+1, Query.Name, Query.NoFields
          , Query.isSingle ? " Fetch" : ""
          , Query.isManyQuery ? " ManyQuery" : ""
          , Query.isNullEnabled ? " NullEnabled" : ""
          );
    if (Query.NoFields)
    {
      TSqlField Field;
      for (j=0; j<Query.NoFields; j++)
      {
        Field.Name = getString(InFile);
        Field.CType = getInt16(InFile);
        Field.SqlType = getInt16(InFile);
        if (sign == sqldefSign2)
        {
          Field.Size = getUInt32(InFile);
          Field.Offset = getUInt32(InFile);
          Field.Precision = getUInt32(InFile);
          Field.Scale = getUInt32(InFile);
        }
        else
        {
          Field.Size = getUInt16(InFile);
          Field.Offset = getUInt16(InFile);
          Field.Precision = getUInt16(InFile);
          Field.Scale = getUInt16(InFile);
        }
        Field.isBind = getUInt16(InFile);
        Field.isDefine = getUInt16(InFile);
        printf(" %2d: %-32s c:%3d s:%3d (%d, %d, %d, %d) %s%s\n"
              , j+1
              , Field.Name
              , Field.CType
              , Field.SqlType
              , Field.Size
              , Field.Offset
              , Field.Precision
              , Field.Scale
              , Field.isBind ? " Bind" : ""
              , Field.isDefine ? " Define" : ""
              );
      }
    }
    Query.Size = getUInt16(InFile);
    Query.Command = getExact(Query.Size, InFile);
    printf("________________________________________________\n"
           "%s\n"
           "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n", Query.Command);
  }
  if (ftell(InFile) < FileSize)
  {
    sign = getUInt16(InFile);
    if (sign == sqldefTailMark)
      goto Return;
    fseek(InFile, -2L, SEEK_CUR);
    printf("Token (%s=%s)\n", getString(InFile), getString(InFile));
  }
Return:
  if (InFile) fclose(InFile);
}

const int BUFF_LEN = 1024;
static char* dumpFiles(char* buffer, int &len, ushort sign)
{
  char* fp = buffer;
  char work[BUFF_LEN];
  while (true)
  {
    char* semi = strchr(fp, ';');
    if (semi != 0)
    {
      strncpyz(work, fp, semi-fp);
      fp = semi + 1;
      fprintf(LogFile, "Loading in SO: %s\n", work);
      DumpSOFile(work, sign);
      continue;
    }
    char *nl = strchr(fp, '\n');
    if (nl != 0)
    {
      char *cr = strchr(fp, '\r');
      if (cr != 0) 
        nl = cr;
      strncpyz(work, fp, nl-fp);
      fprintf(LogFile, "Loading in SO: %s\n", work);
      DumpSOFile(work, sign);
      len = BUFF_LEN;
      return buffer;
    }
    strcpy(buffer, fp);
    len = BUFF_LEN - strlen(buffer);
    return buffer + strlen(buffer);
  }
}

int main(int argc, char *argv[])
{
  int i;
  argc = GetArgs(argc, argv, argtab, TABSIZE);
  LogFile = stdout;
  if (argc < 2 && strlen(FileInList) == 0 && strlen(BinFile) == 0)
  {
    fprintf(LogFile, "Release(%s) Compiled %s\n", "15.03.0.0", __DATE__);
    fprintf(LogFile, "Usage: pocidump files\n"
                     "       where files is the name of sqldef files to be"
                     " dumped\n");
    PrUsage(argtab, TABSIZE);
    return 1;
  }
  int start = 1;
  ushort sign = slinkSign;
  if (strlen(FileInList) > 0 && access(FileInList, 0) == 0)
  {
    FILE *infile = fopen(FileInList, "rt");
    char buffer[BUFF_LEN+1];
    buffer[BUFF_LEN] = 0;
    char *fp = buffer;
    int len = sizeof(buffer);
    while (!feof(infile))
    {
      memset(fp, 0, len);
      fgets(fp, len, infile);
      if (strlen(buffer) < BUFF_LEN)
        buffer[strlen(buffer)] = '\n';
      fp = dumpFiles(buffer, len, sign);
    }
    fclose(infile);
  }
  if (strlen(BinFile) > 0 && access(BinFile, 0) == 0)
  {
    TSqlBin *sqlBin;
    TSqlQuery *sqlQuery;
    TSqlField *sqlField;
    TSqlToken *sqlToken;
    int rc = SqlBinOpen(&sqlBin, BinFile);
    if (strlen(PyFile) > 0)
    {
      FILE* pyFile = fopen(PyFile, "wt");
      fprintf(pyFile,
        "class Class: pass\n"
        "binFile = Class()\n"
        "binFile.noQueries = %d\n"
        "binFile.queries = []\n"
        "binFile.noServers = %d\n"
        "binFile.servers = []\n"
        "binFile.noSchemas = %d\n"
        "binFile.schemas = []\n"
        "binFile.noTables = %d\n"
        "binFile.tables = []\n"
        "binFile.noTokens = %d\n"
        "binFile.tokens = []\n\n"
        , sqlBin->NoQueries
        , sqlBin->NoServers
        , sqlBin->NoSchemas
        , sqlBin->NoTables
        , sqlBin->NoTokens
        );
      for (int s1 = 0; s1 < sqlBin->NoServers; s1++)
        fprintf(pyFile,
        "binFile.servers.append('%s')\n"
        , sqlBin->Servers[s1]
        );
      for (int s2 = 0; s2 < sqlBin->NoSchemas; s2++)
        fprintf(pyFile,
        "binFile.schemas.append('%s')\n"
        , sqlBin->Schemas[s2]
        );
      for (int t = 0; t < sqlBin->NoTables; t++)
        fprintf(pyFile,
        "binFile.tables.append('%s')\n"
        , sqlBin->Tables[t]
        );
      for (int q = 0; q < sqlBin->NoQueries; q++)
      {
        sqlQuery = &sqlBin->Queries[q];
        int noFields = sqlQuery->NoFields &= 0x0FFF;
        fprintf(pyFile,
          "_q = Class()\n"
          "binFile.queries.append(_q)\n"
          "_q.name = '%s'\n"
          "_q.serverNo = %d\n"
          "_q.schemaNo = %d\n"
          "_q.tableNo = %d\n"
          "_q.noFields = %d\n"
          "_q.fields = []\n"
          "_q.size = %d\n"
          "_q.command = '''\n%s\n'''\n"
          "_q.isSql = %d\n"
          "_q.isSingle = %d\n"
          "_q.isManyQuery = %d\n"
          "_q.isNullEnabled = %d\n\n"
          , sqlQuery->Name
          , sqlQuery->ServerNo
          , sqlQuery->SchemaNo
          , sqlQuery->TableNo
          , noFields
          , sqlQuery->Size
          , sqlQuery->Command
          , sqlQuery->isSql ? 1 : 0
          , sqlQuery->isSingle ? 1 : 0
          , sqlQuery->isManyQuery ? 1 : 0
          , sqlQuery->isNullEnabled ? 1 : 0
          );
        for (int f = 0; f < noFields; f++)
        {
          sqlField = &sqlQuery->Fields[f];
          fprintf(pyFile,
            "_f = Class()\n"
            "_q.fields.append(_f)\n"
            "_f.name = '%s'\n"
            "_f.cType = %d\n"
            "_f.sqlType = %d\n"
            "_f.size = %d\n"
            "_f.offset = %d\n"
            "_f.precision = %d\n"
            "_f.scale = %d\n"
            "_f.isBind = %d\n"
            "_f.isDefine = %d\n\n"
            , sqlField->Name
            , sqlField->CType
            , sqlField->SqlType
            , sqlField->Size
            , sqlField->Offset
            , sqlField->Precision
            , sqlField->Scale
            , sqlField->isBind ? 1 : 0
            , sqlField->isDefine ? 1 : 0
            );
        }
      }
      for (int t2 = 0; t2 < sqlBin->NoTokens; t2++)
      {
        sqlToken = &sqlBin->Tokens[t2];
        fprintf(pyFile,
          "_t = Class()\n"
          "binFile.tokens.append(_t)\n"
          "_t.name = '%s'\n"
          "_t.value = '%s'\n"
          , sqlToken->Name
          , sqlToken->Value
          );
      }
      fclose(pyFile);
    }
    else
    {
      for (int q = 0; q < sqlBin->NoQueries; q++)
      {
        sqlQuery = &sqlBin->Queries[q];
        printf("%s: %s\n%s\n\n"
          , sqlBin->Tables[sqlQuery->TableNo]
          , sqlQuery->Name
          , sqlQuery->Command);
      }
    }
    SqlBinClose(sqlBin);
  }
  for (i = start; i<argc; i++)
  {
    fprintf(LogFile, "Loading in SO: %s\n", argv[i]);
    DumpSOFile(argv[i], sign);
  }
  fprintf(LogFile, "Done\n");
  return 0;
}
