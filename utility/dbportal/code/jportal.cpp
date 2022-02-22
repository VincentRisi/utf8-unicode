#include "machine.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined(M_W32)
#include <io.h>
#endif
#include "xstring.h"
#include "xtalloc.h"
#include "xdir.h"
#include "binio.h"
#include "getargs.h"
#include "sqlcons.h"
#include "jportal.h"

static int Verbose = 0;
static const char *FileInList = "";
static const char *OutDir = ".";
static FILE *LogFile = 0;

static ARG argtab[] =
{
    {'v', BOOLEAN, &Verbose, "Verbose Compile"}, 
    {'o', STRING, (int *)&OutDir, "Outfile directory"}, 
    {'f', STRING, (int *)&FileInList, "FileName of inputs"}
};

#define TABSIZE (sizeof(argtab) / sizeof(ARG))

#define AddList(Pointer, Type, Data, Index, Delta)                        \
  do                                                                      \
  {                                                                       \
    if (Index % Delta == 0)                                               \
      Pointer = (Type *)realloc(Pointer, sizeof(Data) * (Index + Delta)); \
    Pointer[Index++] = Data;                                              \
  } while (0)

template <size_t N>
inline char *zzcopy(char (&target)[N], const char *source)
{
  return strncpyz(target, source, N - 1);
}

static ushort NoQueries = 0;
static PSqlQuery *Queries = 0;

static ushort NoTokens = 0;
static PSqlToken *Tokens;
static ushort NoServers = 0;
static pchar *Servers;
static ushort NoSchemas = 0;
static pchar *Schemas;
static ushort NoTables = 0;
static pchar *Tables;

static ushort NoFields;
static PSqlField Fields;

static FILE *BinFile = 0;

static ushort AddServer(char *Name);
static ushort AddSchema(char *Name);
static ushort AddTable(char *Name);
static void   AddQuery(TSqlQuery Query);

int SortComp(const PSqlQuery *p1, PSqlQuery *p2);

//#include "sqldef.h"
#include "global.h"

ushort sqldefSign = 0x0C1A;
ushort slinkSign = 0x0C1B;
ushort sqldefSign2 = 0x0C2A;
ushort slinkSign2 = 0x0C2B;
ushort sqldefTailMark = 0xBEEF;

int SortComp(const PSqlQuery *p1, PSqlQuery *p2)
{
  return stricmp(p1[0]->Name, p2[0]->Name);
}

const int BUFF_SIZE = 512;

enum
{
  NORMAL,
  BATCH_START,
  IN_BATCH
};
static int state = NORMAL;
static char tableName[BUFF_SIZE];
static char serverName[BUFF_SIZE];
static char schemaName[BUFF_SIZE];
static char batchName[BUFF_SIZE];
static FILE *outFile;

static FILE *OpenOutFile(FILE *outFile, const char *fileName)
{
  char work[BUFF_SIZE];
  if (outFile != 0 && outFile != stdout)
    fclose(outFile);
#ifdef M_W32
  _snprintf(work, sizeof(work), "%s\\%s.si", OutDir, batchName);
#else
  snprintf(work, sizeof(work), "%s/%s.si", OutDir, batchName);
#endif
  FILE *result = fopen(work, "wt");
  return result;
}

static void OutTop()
{
  fprintf(outFile, "database %s\n", serverName + 1);
  fprintf(outFile, "output %s\n", batchName);
  fprintf(outFile, "schema %s\n", schemaName);
  fprintf(outFile, "\n");
}

static int lookFor(const char *tableName, const char *inProcName)
{
  char procName[BUFF_SIZE];
  for (int i = 0; i < NoQueries; i++)
  {
    TSqlQuery &Query = *Queries[i];
    if (strnicmp(tableName, Query.Name, strlen(tableName)) == 0)
      zzcopy(procName, Query.Name + strlen(tableName));
    if (stricmp(procName, inProcName) == 0)
      return i;
  }
  return -1;
}

static void OutField(TSqlField &field)
{
  switch (field.CType)
  {
  case SQL_C_BINARY:
    fprintf(outFile, "  %-31s C_BINARY // %d\n", field.Name + 1, field.Size);
    break;
  case SQL_C_BIT:
    fprintf(outFile, "  %-31s C_BIT // %d\n", field.Name + 1, field.Size);
    break;
  case SQL_C_CHAR:
    fprintf(outFile, "  %-31s char(%d)\n", field.Name + 1, field.Size - 1);
    break;
  case SQL_C_DATE:
    fprintf(outFile, "  %-31s date\n", field.Name + 1);
    break;
  case SQL_C_DOUBLE:
  case SQL_C_FLOAT:
    fprintf(outFile, "  %-31s double(%d,%d)\n", field.Name + 1, field.Precision, field.Scale);
    break;
  case SQL_C_LONG:
    fprintf(outFile, "  %-31s int\n", field.Name + 1);
    break;
  case SQL_C_LONG64:
    fprintf(outFile, "  %-31s long", field.Name + 1);
    break;
  case SQL_C_SHORT:
    fprintf(outFile, "  %-31s short\n", field.Name + 1);
    break;
  case SQL_C_TIME:
    fprintf(outFile, "  %-31s time\n", field.Name + 1);
    break;
  case SQL_C_TIMESTAMP:
    fprintf(outFile, "  %-31s timestamp\n", field.Name + 1);
    break;
  case SQL_C_TINYINT:
    fprintf(outFile, "  %-31s byte\n", field.Name + 1);
    break;
  case SQL_C_CLIMAGE:
    fprintf(outFile, "  %-31s CLOB(%d)\n", field.Name + 1, field.Size);
    break;
  case SQL_C_BLIMAGE:
    fprintf(outFile, "  %-31s BLOB(%d)\n", field.Name + 1, field.Size);
    break;
  case SQL_C_ZLIMAGE:
    fprintf(outFile, "  %-31s C_ZLIMAGE // %d\n", field.Name + 1, field.Size);
    break;
  case SQL_C_HUGECHAR:
    fprintf(outFile, "  %-31s C_HUGECHAR // %d\n", field.Name + 1, field.Size);
    break;
  case SQL_C_XMLTYPE:
    fprintf(outFile, "  %-31s XML (%d)\n", field.Name + 1, field.Size);
    break;
  }
}

static void OutTable(const char *tableName)
{
  NoFields = 0;
  Fields = 0;
  fprintf(outFile, "table %s\n", tableName);
  int no = lookFor(tableName, "selectall");
  if (no == -1)
    no = lookFor(tableName, "selectone");
  if (no == -1)
    no = lookFor(tableName, "update");
  if (no == -1)
    no = lookFor(tableName, "insert");
  if (no == -1)
    return;
  TSqlQuery &Query = *Queries[no];
  NoFields = Query.NoFields;
  Fields = Query.Fields;
  for (int i = 0; i < NoFields; i++)
  {
    TSqlField &field = Fields[i];
    OutField(field);
  }
  fprintf(outFile, "\n");
}

bool UserProc(const char *procName)
{
  if (stricmp(procName, "insert") == 0 
  || stricmp(procName, "update") == 0 
  || stricmp(procName, "selectone") == 0 
  || stricmp(procName, "selectall") == 0 
  || stricmp(procName, "deleteone") == 0 
  || stricmp(procName, "deleteall") == 0 
  || stricmp(procName, "count") == 0 
  || stricmp(procName, "exists") == 0
  )
    return false;
  return true;
}

bool IsTableField(TSqlField &field)
{
  for (int i = 0; i < NoFields; i++)
  {
    TSqlField &tF = Fields[i];
    if (stricmp(field.Name, tF.Name) != 0)
      continue;
    if (field.Size != tF.Size)
      return false;
    if (field.CType != tF.CType)
      return false;
    return true;
  }
  return false;
}

void OutProcInputs(TSqlQuery &Query)
{
  bool done = false;
  for (int i = 0; i < Query.NoFields; i++)
  {
    TSqlField &field = Query.Fields[i];
    if (field.isBind)
    {
      if (done == false)
      {
        fprintf(outFile, "input\n");
        done = true;
      }
      if (IsTableField(field))
      {
        fprintf(outFile, "  %-31s =\n", field.Name);
        continue;
      }
      OutField(field);
    }
  }
}

void OutProcOutputs(TSqlQuery &Query)
{
  bool done = false;
  for (int i = 0; i < Query.NoFields; i++)
  {
    TSqlField &field = Query.Fields[i];
    if (field.isDefine)
    {
      if (done == false)
      {
        fprintf(outFile, "output%s\n", Query.isSingle ? " (single)" : Query.isManyQuery ? " (multiple)" : "");
        done = true;
      }
      if (IsTableField(field))
      {
        fprintf(outFile, "  %-31s =\n", field.Name);
        continue;
      }
      OutField(field);
    }
  }
}

void OutProcSqlCode(TSqlQuery &Query)
{
  if (Query.isSql)
    fprintf(outFile, "sqlcode\n");
  else
    fprintf(outFile, "sqlproc\n");
  fprintf(outFile, "%s\n", Query.Command);
  fprintf(outFile, "endcode\n");
}

void OutProcGuts(TSqlQuery &Query)
{
  OutProcInputs(Query);
  OutProcOutputs(Query);
  OutProcSqlCode(Query);
}

void OutProcs(const char *tableName)
{
  char procName[BUFF_SIZE];
  for (int i = 0; i < NoQueries; i++)
  {
    TSqlQuery &Query = *Queries[i];
    if (strnicmp(tableName, Query.Name, strlen(tableName)) == 0)
      zzcopy(procName, Query.Name + strlen(tableName));
    fprintf(outFile, "proc %s\n", procName);
    if (UserProc(procName))
    {
      OutProcGuts(Query);
      fprintf(outFile, "\n");
    }
  }
}

void LoadInFile(const char *InFileName, ushort &slink)
{
  FILE *InFile;
  TSqlQuery Query;
  PSqlQuery Q;
  ushort i, j, NoProcs, sign;
  printf("%-13s: %s\n", "Input", InFileName);
  InFile = fopen(InFileName, "rb");
  try
  {
    if (InFile == 0)
    {
      printf("LoadinFile Access for file [%s] failed!\n", InFileName);
      throw 1;
    }
    if (setvbuf(InFile, 0, _IOFBF, 8192))
      printf("Not enough memory to buffer file!\n");
    sign = getUInt16(InFile);
    if (sign != sqldefSign && sign != sqldefSign2)
    {
      printf("Not a valid SO file!\n");
      throw 2;
    }
    else if (sign == sqldefSign2) // if anywhere a binary is sign2
      slink = slinkSign2;         // then the whole bin must be sign2
    char *w1, *w2, *w3;
    w1 = getString(InFile);
    zzcopy(serverName, w1);
    Query.ServerNo = AddServer(w1);
    w2 = getString(InFile);
    zzcopy(schemaName, w2);
    Query.SchemaNo = AddSchema(w2);
    w3 = getString(InFile);
    zzcopy(tableName, w3);
    Query.TableNo = AddTable(w3);
    switch (state)
    {
    case BATCH_START:
      zzcopy(batchName, tableName);
      outFile = OpenOutFile(outFile, strlwr(batchName));
      if (outFile == 0)
        outFile = stdout;
      OutTop();
      state = IN_BATCH;
      break;
    case IN_BATCH:
      break;
    case NORMAL:
      zzcopy(batchName, tableName);
      outFile = OpenOutFile(outFile, strlwr(batchName));
      if (outFile == 0)
        outFile = stdout;
      OutTop();
      break;
    }
    int proc_state = NORMAL;
    NoProcs = getUInt16(InFile);
    for (i = 0; i < NoProcs; i++)
    {
      Query.Name = getString(InFile);
      Query.NoFields = getUInt16(InFile);
      Query.isSql = Query.NoFields & 0x8000;
      Query.isSingle = Query.NoFields & 0x4000;
      Query.isManyQuery = Query.NoFields & 0x2000;
      Query.isNullEnabled = Query.NoFields & 0x1000;
      Query.NoFields &= 0x0FFF;
      if (Query.NoFields)
      {
        Query.Fields = (PSqlField)calloc(Query.NoFields, sizeof(TSqlField));
        for (j = 0; j < Query.NoFields; j++)
        {
          PSqlField Field = &Query.Fields[j];
          Field->Name = getString(InFile);
          Field->CType = getInt16(InFile);
          Field->SqlType = getInt16(InFile);
          if (sign == sqldefSign2)
          {
            Field->Size = getUInt32(InFile);
            Field->Offset = getUInt32(InFile);
            Field->Precision = getUInt32(InFile);
            Field->Scale = getUInt32(InFile);
          }
          else
          {
            Field->Size = getUInt16(InFile);
            Field->Offset = getUInt16(InFile);
            Field->Precision = getUInt16(InFile);
            Field->Scale = getUInt16(InFile);
          }
          Field->isBind = getUInt16(InFile);
          Field->isDefine = getUInt16(InFile);
        }
      }
      else
        Query.Fields = 0;
      Query.Size = getUInt16(InFile);
      Query.Command = getExact(Query.Size, InFile);
      Q = (PSqlQuery)malloc(sizeof(Query));
      *Q = Query;
      AddList(Queries, PSqlQuery, Q, NoQueries, 16);
    }
    fclose(InFile);
    InFile = 0;
    OutTable(tableName);
    OutProcs(tableName);
    NoQueries = 0;
    return;
  }
  catch (int no)
  {
  Return:
    if (InFile)
      fclose(InFile);
  }
}

static ushort AddServer(char *Name)
{
  ushort n;
  for (n = 0; n < NoServers; n++)
    if ((stricmp(Name, Servers[n])) == 0)
    {
      free(Name);
      return n;
    }
  n = NoServers;
  AddList(Servers, char *, Name, NoServers, 4);
  return n;
}

static ushort AddSchema(char *Name)
{
  ushort n;
  for (n = 0; n < NoSchemas; n++)
    if ((stricmp(Name, Schemas[n])) == 0)
    {
      free(Name);
      return n;
    }
  n = NoSchemas;
  AddList(Schemas, char *, Name, NoSchemas, 4);
  return n;
}

static ushort AddTable(char *Name)
{
  ushort n;
  for (n = 0; n < NoTables; n++)
    if ((stricmp(Name, Tables[n])) == 0)
    {
      free(Name);
      return n;
    }
  n = NoTables;
  AddList(Tables, char *, Name, NoTables, 16);
  return n;
}
int binio_verbose = Verbose;

const int BUFF_LEN = 1024;

static char *loadFiles(char *buffer, int &len, ushort sign)
{
  char *fp = buffer;
  char work[BUFF_LEN];
  while (true)
  {
    char *semi = strchr(fp, ';');
    if (semi != 0)
    {
      strncpyz(work, fp, semi - fp);
      fp = semi + 1;
      fprintf(LogFile, "Loading in SO: %s\n", work);
      LoadInFile(work, sign);
      continue;
    }
    char *nl = strchr(fp, '\n');
    if (nl != 0)
    {
      char *cr = strchr(fp, '\r');
      if (cr != 0)
        nl = cr;
      strncpyz(work, fp, nl - fp);
      fprintf(LogFile, "Loading in SO: %s\n", work);
      LoadInFile(work, sign);
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
  if (argc < 2 && strlen(FileInList) == 0)
  {
    fprintf(LogFile, "Release(%s) Compiled %s\n", "15.03.0.0", __DATE__);
    fprintf(LogFile, "Usage: pocilink files\n"
                     "       where files is the name of sqldef files to be"
                     " linked\n");
    PrUsage(argtab, TABSIZE);
    return 1;
  }
  int start = 1;
  ushort sign = slinkSign;
  if (strlen(FileInList) > 0 && access(FileInList, 0) == 0)
  {
    FILE *infile = fopen(FileInList, "rt");
    char buffer[BUFF_LEN + 1];
    buffer[BUFF_LEN] = 0;
    char *fp = buffer;
    int len = sizeof(buffer);
    while (!feof(infile))
    {
      memset(fp, 0, len);
      fgets(fp, len, infile);
      if (strlen(buffer) < BUFF_LEN)
        buffer[strlen(buffer)] = '\n';
      fp = loadFiles(buffer, len, sign);
    }
    fclose(infile);
  }
  for (i = start; i < argc; i++)
  {
    if (stricmp(argv[i], "**batch") == 0)
    {
      state = BATCH_START;
      continue;
    }
    fprintf(LogFile, "Loading in SO: %s\n", argv[i]);
    LoadInFile(argv[i], sign);
  }
  fprintf(LogFile, "Done\n");
  LogFile = 0;
  return 0;
}
