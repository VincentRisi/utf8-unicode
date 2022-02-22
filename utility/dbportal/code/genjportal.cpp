#include "machine.h"

#include <stdio.h>
#include "xstring.h"
#include <stdlib.h>

#include "sqlcons.h"
#include "sqldef.h"
#include "global.h"
#include "xdir.h"
#ifndef M_AIX
#include "dir.h"
#endif

static FILE *JPORTALFile = 0;

static FILE* OpenFile(const char *FileFlag, const char *FileName)
{
  char Work[512];
  sprintf(Work, "%-13.13s: %s\n", FileFlag, FileName);
  yyerror(Work);
  FILE* File = fopen(FileName, "wt");
  if (File == 0)
    yyerror("Cannot open %s : %s\n", FileFlag, FileName);
  else
    setvbuf(File, 0, _IOFBF, 32768);
  return File;
}

static int OpenJPORTALFile(const char *JPORTALFileName)
{
  JPORTALFile = OpenFile("PARM File", JPORTALFileName);
  if (JPORTALFile == 0)
    return 1;
  return 0;
}

static void MakeJPORTALName(PYYTable Table)
{
  char newpath[512];
  char dir[512];
  char ext[512];
  char fname[512];
  FNameSplit(Table->InputFileName, dir, fname, ext);
  if (Table->ShortName == 0)
    free(Table->ShortName);
  Table->ShortName = strdup(fname);
  if (strlen(Table->JPORTALDir))
    FNameMerge(newpath, Table->JPORTALDir, fname, Table->JPORTALExt);
  else
    FNameMerge(newpath, dir, fname, Table->JPORTALExt);
  Table->JPORTALFileName = strdup(newpath);
}

const char ReservedWords[] =
              ":"
              "all:"
              "ansichar:"
              "application:"
              "boolean:"
              "byte:"
              "cascade:"
              "char:"
              "check:"
              "date:"
              "datetime:"
              "delete:"
              "float:"
              "double:"
              "money:"
              "flags:"
              "int:"
              "integer:"
              "identity:"
              "key:"
              "link:"
              "long:"
              "lookup:"
              "money:"
              "nodomain:"
              "not:"
              "null:"
              "options:"
              "order:"
              "output:"
              "password:"
              "primary:"
              "registry:"
              "relation:"
              "sequence:"
              "server:"
              "short:"
              "show:"
              "smallint:"
              "sizes:"
              "table:"
              "time:"
              "timestamp:"
              "tinyint:"
              "unique:"
              "upper:"
              "uppercase:"
              "use:"
              "user:"
              "userstamp:"
              "value:"
              "version:"
              "viewonly:"
              ;

static char* CheckReserved(char *result, const char* name)
{
  char Work[256];
  result[0] = 0;
  sprintf(Work, ":%s:", name);
  strlwr(Work);
  if (strstr(ReservedWords, Work) != 0)
    sprintf(result, "L'%s'", name);
  else
    strcat(result, name);
  return result;
}

static char* NameOf(char *result, PYYField Field)
{
  char CheckWork[256];
  result[0] = 0;
  strcat(result, CheckReserved(CheckWork, Field->Name));
  if (Field->Alias != 0)
  {
    strcat(result, " (");
    strcat(result, CheckReserved(CheckWork, Field->Alias));
    strcat(result, ")");
  }
  return result;
}

static char* CommentOf(char *result, PYYField Field)
{
  strcpy(result, "//");
  switch (Field->Type)
  {
  case ftypeChar:
  case ftypeNChar:
  case ftypeTimeStamp:
  case ftypeUserStamp:
  case ftypeDate:
  case ftypeDateTime:
  case ftypeBoolean:
  case ftypeTinyint:
  case ftypeSmallint:
  case ftypeInt:
  case ftypeLong:
  case ftypeFloat:
    strcpy(result, "  ");
    break;
  }
  return result;
}
static bool FieldInTable(PYYField Field, PYYTable Table)
{
    for (int i = 0; i < Table->noFields; i++)
    {
        PYYField TField = &Table->Fields[i];
        if (stricmp(Field->Name, TField->Name) != 0)
            continue;
        if (Field->Type != TField->Type
            || Field->Length != TField->Length
            || Field->Precision != TField->Precision
            || Field->Scale != TField->Scale)
            break;
        return true;
    }
    return false;
}
static char* TypeOf(char *result, PYYField Field, PYYTable Table=0)
{
  strcpy(result, "unhandled");
  if (Table != 0 && FieldInTable(Field, Table) == true)
  {
      strcpy(result, "=");
      return result;
  }
  else
  {
      switch (Field->Type)
      {
      case ftypeBinary:
          break;
      case ftypeChar:
      case ftypeNChar:
          sprintf(result, "char(%d)", Field->Length - 1);
          break;
      case ftypeTimeStamp:
          strcpy(result, "timestamp");
          break;
      case ftypeUserStamp:
          strcpy(result, "userstamp");
          break;
      case ftypeStatus:
          break;
      case ftypeMoney:
          break;
      case ftypeDynamic:
          break;
      case ftypeImage:
          break;
      case ftypeDate:
          strcpy(result, "date");
          break;
      case ftypeDateTime:
          strcpy(result, "datetime");
          break;
      case ftypeBoolean:
          strcpy(result, "boolean");
          break;
      case ftypeTinyint:
          strcpy(result, "byte");
          break;
      case ftypeSmallint:
          strcpy(result, "short");
          break;
      case ftypeInt:
          strcpy(result, "int");
          break;
      case ftypeLong:
          strcpy(result, "long");
          break;
      case ftypeFloat:
          strcpy(result, "float");
          break;
      }
      if (Field->isSequence)
          strcpy(result, "sequence");
  }
  if (Table == 0 && Field->noConsts > 0)
  {
    char Work[256];
    char CheckWork[256];
    strcat(result, " ");
    int i;
    char *delim1="(";
    char *delim2=")";
    if (Field->Type == ftypeChar)
    {
      delim1="{"; 
      delim2="}";
    }
    for (i=0; i<Field->noConsts; i++)
    {
      PYYConst Const = &Field->Consts[i];
      if (delim1[0]== '{') // valuelist not an enum
        sprintf(Work, "%s%s", i == 0 ? delim1 : ", ", CheckReserved(CheckWork, Const->Name));
      else
        sprintf(Work, "%s%s=%d", i==0?delim1:", ", CheckReserved(CheckWork, Const->Name), Const->Value);
      strcat(result, Work);
    }
    strcat(result, delim2);
  }
  return result;
}

static int CountOf(PYYTable Table)
{
  int result = Table->noFields, i;
  for (i=0; i<Table->noFields; i++)
  {
    PYYField Field = &Table->Fields[i];
    if (Field->Type == ftypeTimeStamp
    ||  stricmp(Field->Name, "USId") == 0)
      result--;
  }
  return result;
}

static void GenLink(PYYLink Link)
{
  int i;
  char CheckWork[256];
  fprintf(JPORTALFile, "LINK %s", Link->MasterName);
  for (i=0; i<Link->noFields; i++)
  {
    PYYField Field = &Link->Fields[i];
    CheckReserved(CheckWork, Field->Name);
    fprintf(JPORTALFile, "%s%s", i==0?"(":" ", CheckWork);
  }
  fprintf(JPORTALFile, ")\n");
}

static void GenLinks(PYYTable Table)
{
  int i;
  for (i=0; i<Table->noLinks; i++)
  {
    PYYLink Link = &Table->Links[i];
    GenLink(Link);
  }
  fprintf(JPORTALFile, "\n");
}

static void GenKey(PYYKey Key)
{
  int i;
  char CheckWork[256];
  fprintf(JPORTALFile, "KEY %s", Key->Name);
  for (i=0; i<Key->noFields; i++)
  {
    PYYField Field = &Key->Fields[i];
    CheckReserved(CheckWork, Field->Name);
    fprintf(JPORTALFile, "%s%s", i==0?"(":" ", CheckWork);
  }
  fprintf(JPORTALFile, ")");
  if (Key->isPrimary)
    fprintf(JPORTALFile, "  PRIMARY");
  else if (Key->isUnique)
    fprintf(JPORTALFile, "  UNIQUE");
  fprintf(JPORTALFile, "\n");
}

static void GenKeys(PYYTable Table)
{
  int i;
  for (i=0; i<Table->noKeys; i++)
  {
    PYYKey Key = &Table->Keys[i];
    GenKey(Key);
  }
  fprintf(JPORTALFile, "\n");
}

const int worksize = 2048;
static char* _server(char *work, char* server)
{
    char* p = server;
    if (p[0] == '@') p++;
    snprintf(work, worksize, "%s", p);
    return work;
}

static char* _output(char *work, char* name)
{
    strncpyz(work, name, worksize - 1);
    strlwr(work);
    return work;
}

static void GenStdProc(PYYProc proc)
{
    fprintf(JPORTALFile, "PROC %s\n", proc->Name);
}

static void GenProc(PYYProc Proc, PYYTable Table)
{
    if (Proc == 0) return;
    char work[worksize], nameWork[worksize], typeWork[worksize];
    fprintf(JPORTALFile, "\n");
    fprintf(JPORTALFile, "PROC %s\n", Proc->Name);
    if (Proc->noBinds > 0)
    {
        fprintf(JPORTALFile, "INPUT\n");
        for (int i = 0; i < Proc->noBinds; i++)
        {
            int offset = Proc->Binds[i];
            PYYField Field = &Proc->Fields[offset];
            fprintf(JPORTALFile, "  %s %s\n"
                , NameOf(nameWork, Field)
                , TypeOf(typeWork, Field, Table));
        }
    }
    if (Proc->noOutputs > 0)
    {
        fprintf(JPORTALFile, "OUTPUT (%s)\n", Proc->isSingle ? "single" : "100");
        for (int i = 0; i < Proc->noFields; i++)
        {
            PYYField Field = &Proc->Fields[i];
            if (Field->isOutput != 0)
            {
                fprintf(JPORTALFile, "  %s %s\n"
                    , NameOf(nameWork, Field)
                    , TypeOf(typeWork, Field, Table));
            }
        }
    }
    if (Proc->noWiths > 0)
    {
        fprintf(JPORTALFile, "WITH\n");
        for (int i = 0; i < Proc->noWiths; i++)
        {
            fprintf(JPORTALFile, "%s%s", Proc->Withs[i].p, Proc->Withs[i].isEOL ? "" : " ");
            fflush(JPORTALFile);
        }
        fprintf(JPORTALFile, "\n");
    }
    fflush(JPORTALFile);
    fprintf(JPORTALFile, "SQLCODE\n");
    if (Proc->noLines > 0)
    {
        for (int i = 0; i < Proc->noLines; i++)
        {
            fprintf(JPORTALFile, "%s%s", Proc->Lines[i].p, Proc->Lines[i].isEOL ? "" : " ");
            fflush(JPORTALFile);
        }
    }
    fprintf(JPORTALFile, "ENDCODE\n");
    fflush(JPORTALFile);
}

static void GenWiths(PYYTable Table)
{
    if (Table->noWiths > 0)
    {
        for (int i = 0; i < Table->noWiths; i++)
        {
            PYYWith With = &Table->Withs[i];
            fprintf(JPORTALFile, "WITH %s\n", With->Name);
            fprintf(JPORTALFile, "SQLCODE\n");
            for (int j = 0; j < With->noLines; j++)
                fprintf(JPORTALFile, "%s%s", With->Lines[j].p, With->Lines[j].isEOL ? "" : " ");
            fprintf(JPORTALFile, "\n");
            fprintf(JPORTALFile, "ENDCODE\n");
            fprintf(JPORTALFile, "\n");
        }
    }
}

static void GenTable(PYYTable Table)
{
    char work[worksize], nameWork[worksize], typeWork[worksize];
    fprintf(JPORTALFile, "DATABASE db%s\n", _server(work, Table->Server+1));
    fprintf(JPORTALFile, "OUTPUT %s\n",     _output(work, Table->Name));
    fprintf(JPORTALFile, "SERVER %s\n",     _server(work, Table->Server));
    fprintf(JPORTALFile, "SCHEMA %s\n",     Table->ControlDB);
    fprintf(JPORTALFile, "USERID %s\n",     Table->UserID);
    fprintf(JPORTALFile, "PASSWORD %s\n",   Table->PassWD);
    fprintf(JPORTALFile, "\n");
    fprintf(JPORTALFile, "TABLE %s\n", CheckReserved(work, Table->Name));
    for (int i=0; i<Table->noFields; i++)
    {
        PYYField Field = &Table->Fields[i];
        fprintf(JPORTALFile, "  %s %s%s\n"
            , NameOf(nameWork, Field)
            , TypeOf(typeWork, Field)
            , Field->isNull ? " NULL" : "");
    }
    if (Table->noKeys > 0)
    {
        fprintf(JPORTALFile, "\n");
        GenKeys(Table);
    }
    if (Table->noLinks > 0)
    {
        fprintf(JPORTALFile, "\n");
        GenLinks(Table);
    }
    if (Table->noProcs > 0)
    {
        GenWiths(Table);
        for (int i = 0; i < Table->noProcs; i++)
        {
            PYYProc Proc = &Table->Procs[i];
            if (Proc->isData |= 0)
                continue;
            if (Proc->isStd != 0)
                GenStdProc(Proc);
            else
            {
                GenProc(Proc, Table);
            }
        }
    }
}

void GenerateJPORTAL(PYYTable Table)
{
  if (Table->noFields == 0)
    return;
  MakeJPORTALName(Table);
  int rc = OpenJPORTALFile(Table->JPORTALFileName);
  if (rc)
    return;
  GenTable(Table);
  fclose(JPORTALFile);
}
