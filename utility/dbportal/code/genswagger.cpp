#include "machine.h"

#include <stdio.h>
#include "xstring.h"
#include <stdlib.h>

#include "sqldef.h"

#include "global.h"
#include "xdir.h"

#define US Table->UnderScore ? "_" : ""

static FILE *SwaggerFile = 0;

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
        fprintf(SwaggerFile, "      %s:\n", NameOf(&field));
        switch (field.Type)
        {
        case ftypeNChar:
        case ftypeChar:
        case ftypeUserStamp:
        case ftypeDate:
        case ftypeDateTime:
        case ftypeTimeStamp:
            fprintf(SwaggerFile, "        type: string\n");
            fprintf(SwaggerFile, "        maxLength: %d\n", field.Length - 1);
            break;
        case ftypeTinyint:
        case ftypeStatus:
            fprintf(SwaggerFile, "        type: integer\n");
            fprintf(SwaggerFile, "        format: int8\n");
            break;
        case ftypeBinary:
        case ftypeVarNum:
            break;
        case ftypeSmallint:
            fprintf(SwaggerFile, "        type: integer\n");
            fprintf(SwaggerFile, "        format: int16\n");
            break;
        case ftypeBoolean:
        case ftypeInt:
            fprintf(SwaggerFile, "        type: integer\n");
            fprintf(SwaggerFile, "        format: int32\n");
            break;
        case ftypeLong:
            fprintf(SwaggerFile, "        type: integer\n");
            fprintf(SwaggerFile, "        format: int64\n");
            break;
        case ftypeFloat:
            fprintf(SwaggerFile, "        type: number\n");
            fprintf(SwaggerFile, "        format: double\n");
            break;
        case ftypeMoney:
        case ftypeBLOB:
        case ftypeZLOB:
        case ftypeCLOB:
        case ftypeImage:
        case ftypeXMLTYPE:
        case ftypeDynamic:
            fprintf(SwaggerFile, "        type: string\n");
            fprintf(SwaggerFile, "        maxLength: %d\n", field.Length - 1);
            break;
        case ftypeImagePtr:
            break;
        case ftypeHugeCHAR:
            fprintf(SwaggerFile, "        type: string\n");
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
                fprintf(SwaggerFile, "    required:\n");
            }
            fprintf(SwaggerFile, "      - %s\n", NameOf(&field));
            break;
        }
        
    }
}

static void GenerateSwaggerYaml(PYYTable Table)
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
        fprintf(SwaggerFile, "  t%s:\n", work);
        fprintf(SwaggerFile, "    type: object\n");
        fprintf(SwaggerFile, "    properties:\n");
        doFields(Table->noFields, Table->Fields);
        stdDone = 1;
      }
    }
    else if (Proc->noFields)
    {
      sprintf(work, "%s%s%s%s", US, Dehash(Table->Name), US, (strcmp(Proc->Name, "DeleteOne")==0) ? "Key" : Proc->Name);
      fprintf(SwaggerFile, "  t%s:\n", work);
      fprintf(SwaggerFile, "    type: object\n");
      fprintf(SwaggerFile, "    properties:\n");
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

static void MakeSwaggerName(PYYTable Table)
{
  char newpath[512];
  char dir[512];
  char ext[512];
  char fname[512];
  FNameSplit(Table->InputFileName, dir, fname, ext);
  if (strlen(Table->SwaggerDir))
    FNameMerge(newpath, Table->SwaggerDir, fname, Table->SwaggerExt);
  else
    FNameMerge(newpath, dir, fname, Table->SwaggerExt);
  Table->SwaggerFileName = strdup(newpath);
}

void GenerateSwagger(PYYTable Table)
{
  MakeSwaggerName(Table);
  SwaggerFile = OpenFile("Swagger File", Table->SwaggerFileName);
  if (SwaggerFile == 0)
    return;
  fprintf(SwaggerFile, "%%YAML 1.2\n");
  fprintf(SwaggerFile, "---\n");
  fprintf(SwaggerFile, "# This code was generated, do not modify it, modify it at source and regenerate it.\n");
  fprintf(SwaggerFile, "descriptions:\n");
  GenerateSwaggerYaml(Table);
  fprintf(SwaggerFile, "...\n");
  fclose(SwaggerFile);
}
