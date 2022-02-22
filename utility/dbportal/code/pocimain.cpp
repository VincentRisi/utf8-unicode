#include "machine.h"

#include <stdio.h>
#include <stdlib.h>
#include "xstring.h"
#include "addlist.h"

#include "global.h"
#include "getargs.h"
#include "xdir.h"
#include "tbuffer.h"

const char* Others="";
TBChar allOthers;
int Help = 0;
int ReturnZero = 0;
const char *SwitchFile   = "";       //s
const char *LogFile      = "";       //s

static ARG argtab[] =
{ {'l', STRING,  (int*)  &LogFile,      "Logging file"}
, {'s', STRING,  (int*)  &SwitchFile,   "Switches file"}
, {'z', BOOLEAN,         &ReturnZero,   "Return Zero even if errors"}
, {'?', BOOLEAN,         &Help,         "This Help"}
};
#define TABSIZE (sizeof(argtab) / sizeof(ARG))

static FILE* logout = stdout;
static FILE * yyerrfile;

#ifndef M_NT
 long filelength(int infile)
 {
    long result = lseek(infile ,0, SEEK_END);
    lseek(infile ,0, SEEK_SET);
    return result;
 }
#endif

static void OutputHandler(const char *Line)
{
  fprintf(yyerrfile, "%s\n", Line);
  fflush(yyerrfile);
}

static char* StripSwitch(const char* key, char* work, int worksize)
{
  char pk[512];
  strncpyz(pk, key, sizeof(pk)-1);
  char *oth = allOthers.data;
  char delim = oth[0];
  strlwr(pk);
  strlwr(oth);
  char* fp = strstr(oth, pk);
  if (fp == 0) return 0;
  if (fp > oth && fp[-1] != delim) return 0;
  if (fp[strlen(pk)] != '=') return 0;
  fp += (strlen(pk)+1);
  char* p2 = strchr(fp, delim);
  if (p2 != 0 && (p2-fp) < worksize)
  {
    strncpy(work, fp, p2-fp);
    work[p2-fp] = 0;
    return work;
  }
  else if (p2 == 0 && (int)strlen(fp) < worksize)
  {
    strcpy(work, fp);
    return work;
  }
  return 0;
}

static void GetOther(int &i, const char* key, int def=0)
{
  char work[256];
  char *p;
  p = StripSwitch(key, work, sizeof(work));
  if (p)
    i = atoi(p);
  else
    i = def;
}

static void GetOther(char *&s, const char* key, const char* def="")
{
  char work[256];
  char *p;
  p = StripSwitch(key, work, sizeof(work));
  if (p)
    s = strdup(p);
  else
    s = strdup(def);
}

static void ShowOther(int &i, const char* key, int def=0)
{
  char work[256];
  char *p;
  p = StripSwitch(key, work, sizeof(work));
  fprintf(yyerrfile, " %s(%d)=%d\n", key, def, p ? atoi(p) : def);
}

static void ShowOther(char *&s, const char* key, const char* def="")
{
  char work[256];
  char *p;
  p = StripSwitch(key, work, sizeof(work));
  fprintf(yyerrfile, " %s(%s)=%s\n", key, def, p ? p : def);
}

static void SetSwitches()
{
#if defined(M_GNU)
  #define OTHER_OF_0(name) GetOther(Table->name, #name)
  #define OTHER_OF_1(name) GetOther(Table->name, #name, name)
  #define OTHER_OF_2(name, def) GetOther(Table->name, #name, def)
#else
  #define OTHER_OF_0(name) GetOther(Table->##name, #name)
  #define OTHER_OF_1(name) GetOther(Table->##name, #name, ##name)
  #define OTHER_OF_2(name, def) GetOther(Table->##name, #name, def)
#endif
  #include "switches2.h"
}

static int CheckBuffer(char* buffer)
{
  int rc = 0;
  int line = 1;
  int column = 1;
  for (unsigned char* ch = (unsigned char*)buffer; *ch != 0; ch++)
  {
    if (*ch > 127) 
    {
      fprintf(yyerrfile, "Line %d column %d: invalid character %u(%c)\n", line, column, *ch, *ch);
      *ch = '?';
      rc++;
    }
    if (*ch == '\n') { line++; column = 0;}
    column++;
  }
  return rc;
}

void get_line(char* buffer, char *work, int worksize, int lineno)
{
    int no = 0;
    char* b = buffer;
    char* e;
    int size = worksize - 1;
    for (;;)
    {
        e = strchr(b, '\n');
        if (e == 0)
        {
            e = b + strlen(b);
            break;
        }
        if (no < lineno)
        {
            no++;
            b = e + 1;
            continue;
        }
        break;
    }
    int s = (e - b);
    if (s > 0 && s < size)
    {
        strncpy(work, b, s);
        work[s] = 0;
    }
    else
    {
        strncpy(work, b, size);
        work[s] = 0;
    }
}

int main(int argc, char *argv[])
{
  for (int i=0; i<argc; i++) 
    fprintf(logout, "%s ", argv[i]); 
  fprintf(logout, "\n");
  fflush(logout);
  argc = GetArgs(argc, argv, argtab, TABSIZE);
  if (LogFile[0] != 0)
  {
      FILE* logger = fopen(LogFile, "wt");
      if (logger != 0)
          logout = logger;
  }
  fprintf(logout, "POCIOCI: version: %s %s\n", __DATE__, __TIME__);
  fprintf(logout, "POCIOCI: ");
  fflush(logout);
  allOthers.set(Others);
  if (argc > 2 || strlen(SwitchFile) > 0)
  {
    if (strlen(allOthers.data) == 0)
      allOthers.set("!");
    char delim[2]; 
    delim[0] = allOthers.data[0];
    delim[1] = 0;
    for (int i=2; i<argc; i++)
    {
      allOthers.append(argv[i]);
      allOthers.append(delim);
    }
    if (strlen(SwitchFile) > 0)
    {
      FILE* inp = fopen(SwitchFile, "rt");
      if (inp != 0)
      while (!feof(inp))
      {
        char work[256];work[255] = 0;
        fgets(work,255,inp);
        int n = strlen(work);
        if (work[n-1] == '\n' || work[n-1] == '\r') work[n-1] = 0;
        if (work[n-2] == '\n' || work[n-2] == '\r') work[n-2] = 0;
        allOthers.append(work);
        allOthers.append(delim);
      }
    }
  }
  yyerrfile = stdout;
  if (argc < 2 || Help == 1)
  {
    fprintf(yyerrfile, "Release(%s) Compiled %s\n", "19.10.0.0", __DATE__);
    fprintf(yyerrfile, "Usage: pocioci <options> filename <switches>\n"
                       "where filename is fully qualified path\n");
    PrUsage(argtab, TABSIZE);
    Table = new TYYTable;
#if defined(M_GNU)
    #define OTHER_OF_0(name) ShowOther(Table->name, #name)
    #define OTHER_OF_1(name) ShowOther(Table->name, #name, name)
    #define OTHER_OF_2(name, def) ShowOther(Table->name, #name, def)
#else
    #define OTHER_OF_0(name) ShowOther(Table->##name, #name)
    #define OTHER_OF_1(name) ShowOther(Table->##name, #name, ##name)
    #define OTHER_OF_2(name, def) ShowOther(Table->##name, #name, def)
#endif
    #include "switches2.h"
    return 1;
  }
  yyerrsrc = argv[1];
  char *Buffer = 0;
  FILE *infile = fopen(yyerrsrc, "rt");
  if (infile == 0)
  {
    fprintf(logout, "POCIOCI: %s - file does not open. Check it to see if it exists.", argv[1]);
    return 1;
  }
  fseek(infile, 0, SEEK_END);
  long size = ftell(infile) + 1024;
  Buffer = (char*)calloc(1, size);
  fseek(infile, 0, SEEK_SET);
  // Check for unwanted BOM's
  size_t o = fread(Buffer, 1, 3, infile);
  if ((unsigned char)Buffer[0] >= 128 && (unsigned char)Buffer[1] >= 128 && (unsigned char)Buffer[2] >= 128)
    o = 0;
  size_t n = fread(Buffer+o, 1, size, infile);
  Buffer[n+o] = 0;
  int rc = CheckBuffer(Buffer);
  if (rc == 0)
  {
    fprintf(logout, "POCIOCI: Code Buffer for %s passes CheckBuffer\n", yyerrsrc); 
    fflush(logout);
    Table = new TYYTable;
    Table->InputFileName = strdup(yyerrsrc);
    SetSwitches();
    yyhandler = OutputHandler;
    rc = inbuf_parse(Table, Buffer);
    if (rc == 0) 
    {
      fprintf(logout, "%s: parses ok, generating\n", yyerrsrc); 
      fflush(logout);
      if (logout != stdout)
      {
        fprintf(stdout, "%s: parses ok, generating\n", yyerrsrc); 
        fflush(stdout);
      }
      GenerateOCI(Table);
    }
    else
    {
      char work[1024];
      get_line(Buffer, work, sizeof(work), yylineno-1);
      fprintf(logout, "%s(%d,%d): error 8055 - fails yyparse - '%s'\n", yyerrsrc, yylineno, yycolumn, work);
      fflush(logout);
      if (logout != stdout)
      {
#if defined(M_W32)
        char full[_MAX_PATH];
        char* x = _fullpath(full, yyerrsrc, _MAX_PATH);
        fprintf(stdout, "%s(%d,%d): error 8055 - fails yyparse - '%s'\n", x ? full : yyerrsrc, yylineno, yycolumn, work);
#else
        fprintf(stdout, "%s(%d,%d): error 8055 - fails yyparse - '%s'\n", yyerrsrc, yylineno, yycolumn, work);
        fprintf(stdout, "NB - use fullpath in order to go to code line\n");
#endif
        fflush(stdout);
      }
    }
  }
  if (ReturnZero == 1)
    return 0;
  return rc;
}

