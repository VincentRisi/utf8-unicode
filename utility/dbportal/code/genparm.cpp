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

static int OpenPARMSFile(const char *PARMSFileName)
{
  JPORTALFile = OpenFile("PARM File", PARMSFileName);
  if (JPORTALFile == 0)
    return 1;
  return 0;
}

static void MakePARMSName(PYYTable Table)
{
  char newpath[512];
  char dir[512];
  char ext[512];
  char fname[512];
  FNameSplit(Table->InputFileName, dir, fname, ext);
  if (Table->ShortName == 0)
    free(Table->ShortName);
  Table->ShortName = strdup(fname);
  if (strlen(Table->PARMSDir))
    FNameMerge(newpath, Table->PARMSDir, fname, Table->PARMSExt);
  else
    FNameMerge(newpath, dir, fname, Table->PARMSExt);
  Table->PARMSFileName = strdup(newpath);
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

static char* TypeOf(char *result, PYYField Field)
{
  strcpy(result, "unhandled");
  switch (Field->Type)
  {
  case ftypeBinary:
    break;
  case ftypeChar:
  case ftypeNChar:
      sprintf(result, "char(%d)", Field->Length-1);
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
    strcpy(result, "tinyint");
    break;
  case ftypeSmallint:
    strcpy(result, "smallint");
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
  char Work[256];
  char CheckWork[256];
  if (Field->noConsts > 0)
  {
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

static int GenUses(PYYTable Table)
{
  int i, j;
  int result = 0; //Not A relationship by default
  fprintf(JPORTALFile, "//USES:");
  for (i=0; i<Table->noLinks; i++)
  {
    PYYLink Link = &Table->Links[i];
    fprintf(JPORTALFile, "%s:", Link->MasterName);
  }
  fprintf(JPORTALFile, "\n\n");
  if (Table->noLinks == 2)
  {
    PYYLink Link0 = &Table->Links[0];
    PYYLink Link1 = &Table->Links[1];
    if (stricmp(Link0->MasterName, Link1->MasterName) != 0 && Link0->noFields + Link0->noFields == CountOf(Table))
    {
      result = 1;
      fprintf(JPORTALFile, "// %s vs %s\n", Link0->MasterName, Link1->MasterName);
      for (i=0; i<Link0->noFields; i++)
      {
        PYYField Field0 = &Link0->Fields[i];
        for (j=0; j<Link1->noFields; j++)
        {
          PYYField Field1 = &Link1->Fields[j];
          fprintf(JPORTALFile, "// %s vs %s\n", Field0->Name, Field1->Name);
          if (stricmp(Field0->Name, Field1->Name) == 0)
            return 0;
        }
      }
    }
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

static void GenRelation(PYYTable Table)
{
  int i;
  char CheckWork[256];
  fprintf(JPORTALFile, "RELATION %s", CheckReserved(CheckWork, Table->Name));
  if (strlen(Table->PARMSDescr) > 0)
    fprintf(JPORTALFile, " '%s'", Table->PARMSDescr);
  fprintf(JPORTALFile, "\n");
  PYYLink Link1 = &Table->Links[0];
  PYYLink Link2 = &Table->Links[1];
  fprintf(JPORTALFile, "  %s", Link1->MasterName);
  for (i=0; i<Link1->noFields; i++)
  {
    PYYField Field = &Link1->Fields[i];
    fprintf(JPORTALFile, "%s%s",i==0?"(":" ", Field->Name);
  }
  fprintf(JPORTALFile, ")\n");
  fprintf(JPORTALFile, "  %s", Link2->MasterName);
  for (i=0; i<Link2->noFields; i++)
  {
    PYYField Field = &Link2->Fields[i];
    fprintf(JPORTALFile, "%s%s",i==0?"(":" ", Field->Name);
  }
  fprintf(JPORTALFile, ")\n\n");
}

static void GenTable(PYYTable Table)
{
  int i, viewOnly = Table->ViewOnly;
  char NameWork[8192], TypeWork[8192], CommentWork[8192];
  char CheckWork[8192];
  if (Table->noLinks > 0)
  {
    if (GenUses(Table) == 1)
    {
      GenRelation(Table);
      viewOnly = 1;
    }
  }
  fprintf(JPORTALFile, "TABLE %s", CheckReserved(CheckWork, Table->Name));
  if (strlen(Table->PARMSDescr) > 0)
    fprintf(JPORTALFile, " '%s'", Table->PARMSDescr);
  if (Table->isNullEnabled==1)
    fprintf(JPORTALFile, " NULL");
  if (Table->NoDomain==1)
    fprintf(JPORTALFile, " NODOMAIN");
  if (viewOnly==1)
    fprintf(JPORTALFile, " VIEWONLY");
  fprintf(JPORTALFile, "\n");
  if (Table->Check)
    fprintf(JPORTALFile, "  CHECK \"%s\"\n", Table->Check);
  for (i=0; i<Table->noFields; i++)
  {
    PYYField Field = &Table->Fields[i];
    if (stricmp(Field->Name, "USId") == 0
    ||  stricmp(Field->Name, "TmStamp") == 0)
      continue;
    if (Field->Check)
    {
      strcpy(CheckWork, " CHECK \"");
      strcat(CheckWork, Field->Check);
      strcat(CheckWork, "\"");
    }
    else
      CheckWork[0] = 0;
    fprintf(JPORTALFile, "%s%-28s %s%s%s%s\n"
                     , CommentOf(CommentWork, Field)
                     , NameOf(NameWork, Field)
                     , TypeOf(TypeWork, Field)
                     , Field->isNull ? " NULL" : ""
                     , Field->isUpper ? " UPPER" : ""
                     , CheckWork);
  }
  fprintf(JPORTALFile, "\n");
  PYYKey Key = Table->hasPrimaryKey ? &Table->Keys[Table->PrimaryKey] : 0;
  if (Table->PARMSLookup && strlen(Table->PARMSLookup) > 0)
    fprintf(JPORTALFile, "LOOKUP(%s)\n", Table->PARMSLookup);
  else if (Key!= 0)
  {
    fprintf(JPORTALFile, "LOOKUP");
    for (i=0; i<Key->noFields; i++)
    {
      PYYField Field = &Key->Fields[i];
      CheckReserved(CheckWork, Field->Name);
      fprintf(JPORTALFile, "%s%s", i==0?"(":" ", CheckWork);
    }
    fprintf(JPORTALFile, ")\n");
  }
  if (Table->Show && strlen(Table->Show) > 0)
    fprintf(JPORTALFile, "SHOW(%s)\n", Table->Show);
  if (Table->noKeys > 0)
    GenKeys(Table);
  if (Table->noLinks > 0)
    GenLinks(Table);
}

void GeneratePARMS(PYYTable Table)
{
  if (Table->noFields == 0)
    return;
  MakePARMSName(Table);
  int rc = OpenPARMSFile(Table->PARMSFileName);
  if (rc)
    return;
  GenTable(Table);
  fclose(JPORTALFile);
}


