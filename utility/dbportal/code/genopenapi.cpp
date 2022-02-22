#include "machine.h"

#include <stdio.h>
#include "xstring.h"
#include <stdlib.h>

#include "sqldef.h"

#include "global.h"
#include "xdir.h"

#define US Table->UnderScore ? "_" : ""

static FILE *OpenapiFile = 0;

static char *Dehash(const char *Name)
{
    int i;
    static int n = 0;
    static char Buffer[1024];
    char *Work = Buffer + (n * 128);
    n = (n + 1) % 8;
    strcpy(Work, Name);
    for (i = 0; i < (int)strlen(Work); i++)
    {
        if (Work[i] == '#' || Work[i] == '$')
            Work[i] = '_';
    }
    return Work;
}

static char *NameOf(PYYField Field)
{
    return Field->Alias ? Field->Alias : Dehash(Field->Name);
}

static char *NameOf(PYYTable Table)
{
    return Table->Alias ? Table->Alias : Dehash(Table->Name);
}

static void doFields(int32 noFields, PYYField Fields)
{
    int i;
    for (i = 0; i < noFields; i++)
    {
        TYYField &field = Fields[i];
        fprintf(OpenapiFile, "        %s:\n", NameOf(&field));
        switch (field.Type)
        {
        case ftypeNChar:
        case ftypeChar:
        case ftypeUserStamp:
        case ftypeDate:
        case ftypeDateTime:
        case ftypeTimeStamp:
            fprintf(OpenapiFile, "          type: string\n");
            fprintf(OpenapiFile, "          maxLength: %d\n", field.Length - 1);
            break;
        case ftypeTinyint:
        case ftypeStatus:
            fprintf(OpenapiFile, "          type: integer\n");
            fprintf(OpenapiFile, "          format: int8\n");
            break;
        case ftypeBinary:
        case ftypeVarNum:
            break;
        case ftypeSmallint:
            fprintf(OpenapiFile, "          type: integer\n");
            fprintf(OpenapiFile, "          format: int16\n");
            break;
        case ftypeBoolean:
        case ftypeInt:
            fprintf(OpenapiFile, "          type: integer\n");
            fprintf(OpenapiFile, "          format: int32\n");
            break;
        case ftypeLong:
            fprintf(OpenapiFile, "          type: integer\n");
            fprintf(OpenapiFile, "          format: int64\n");
            break;
        case ftypeFloat:
            fprintf(OpenapiFile, "          type: number\n");
            fprintf(OpenapiFile, "          format: double\n");
            break;
        case ftypeMoney:
        case ftypeBLOB:
        case ftypeZLOB:
        case ftypeCLOB:
        case ftypeImage:
        case ftypeXMLTYPE:
        case ftypeDynamic:
            fprintf(OpenapiFile, "          type: string\n");
            fprintf(OpenapiFile, "          maxLength: %d\n", field.Length - 1);
            break;
        case ftypeImagePtr:
            break;
        case ftypeHugeCHAR:
            fprintf(OpenapiFile, "          type: string\n");
            break;
        case ftypeException:
            break;
        default:
            break;
        }
        if (field.noConsts > 0)
        {

        }
    }
    bool notDone = true;
    for (i = 0; i < noFields; i++)
    {
        TYYField &field = Fields[i];
        switch (field.Type)
        {
        case ftypeNChar:
        case ftypeChar:
        case ftypeUserStamp:
        case ftypeDate:
        case ftypeDateTime:
        case ftypeTimeStamp:
        case ftypeMoney:
        case ftypeBLOB:
        case ftypeZLOB:
        case ftypeCLOB:
        case ftypeImage:
        case ftypeXMLTYPE:
        case ftypeDynamic:
        case ftypeImagePtr:
        case ftypeHugeCHAR:
        case ftypeException:
            break;
        default:
            if (notDone == true)
            {
                notDone = false;
                fprintf(OpenapiFile, "      required:\n");
            }
            fprintf(OpenapiFile, "        - %s\n", NameOf(&field));
            break;
        }
    }
}

static void GenerateOpenapiYaml(PYYTable Table)
{
  int i;
  int stdDone = 0;
  char work[256];
  for (i=0; i<Table->noProcs; i++)
  {
    TYYProc* Proc = &Table->Procs[i];
    if (Proc->isData)
      continue;
    if (Proc->useStd != 0)
    {
      if (stdDone == 0)
      {
        sprintf(work, "%s%s", US, Dehash(Table->Name));
        fprintf(OpenapiFile, "    t%s:\n", work);
        fprintf(OpenapiFile, "      type: object\n");
        fprintf(OpenapiFile, "      properties:\n");
        doFields(Table->noFields, Table->Fields);
        stdDone = 1;
      }
    }
    else if (Proc->noFields)
    {
      sprintf(work, "%s%s%s%s", US, Dehash(Table->Name), US, (strcmp(Proc->Name, "DeleteOne")==0) ? "Key" : Proc->Name);
      fprintf(OpenapiFile, "    t%s:\n", work);
      fprintf(OpenapiFile, "      type: object\n");
      fprintf(OpenapiFile, "      properties:\n");
      doFields(Proc->noFields, Proc->Fields);
    }
  }
}

static FILE* OpenFile(const char *FileFlag, const char *FileName, const char *Mode = "wt")
{
  char Work[512];
  sprintf(Work, "%-13.13s: %s\n", FileFlag, FileName);
  yyerror(Work);
  FILE* File = fopen(FileName, Mode);
  if (File == 0)
    yyerror("Cannot open %s : %s", FileFlag, FileName);
  else
    setvbuf(File, 0, _IOFBF, 32768);
  return File;
}

static void MakeOpenapiName(PYYTable Table)
{
  char newpath[512];
  char dir[512];
  char ext[512];
  char fname[512];
  FNameSplit(Table->InputFileName, dir, fname, ext);
  if (strlen(Table->OpenapiDir))
    FNameMerge(newpath, Table->OpenapiDir, fname, Table->OpenapiExt);
  else
    FNameMerge(newpath, dir, fname, Table->OpenapiExt);
  Table->OpenapiFileName = strdup(newpath);
}

void GenerateOpenapi(PYYTable Table)
{
  MakeOpenapiName(Table);
  OpenapiFile = OpenFile("Openapi File", Table->OpenapiFileName);
  if (OpenapiFile == 0)
    return;
  fprintf(OpenapiFile, "%%YAML 1.2\n");
  fprintf(OpenapiFile, "---\n");
  fprintf(OpenapiFile, "# This code was generated, do not modify it, modify it at source and regenerate it.\n");
  fprintf(OpenapiFile, "components:\n");
  fprintf(OpenapiFile, "  schemas:\n");
  GenerateOpenapiYaml(Table);
  fprintf(OpenapiFile, "...\n");
  fclose(OpenapiFile);
}
