#include "versions.h"
#define VERSION_NO "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
COMPILED_VERSION(VERSION_NO);

#include "machine.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include "yyerror.h"

char *yyerrsrc;
FILE *yyerrfile;
int yyerrlevel;
int yylineno=1;

void yyerror(char *s)
{
  fprintf(yyerrfile, "%s\n", s);
}

void yyreport(int level, char* s, ...)
{
  va_list argptr;

  if (yyerrlevel < level)
     yyerrlevel = level;
  fprintf(yyerrfile, "%s %d(%d) - ", yyerrsrc, yylineno, level);
  va_start(argptr, s);
  vfprintf(yyerrfile, s, argptr);
  if (level >= FATAL_ERROR)
     exit(level);
}

