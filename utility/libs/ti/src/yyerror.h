#ifndef _YYERROR_H_
#define _YYERROR_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_YYERROR_H_);

#define WARNING_ERROR  4
#define CODING_ERROR   8
#define FATAL_ERROR   16

extern char *yyerrsrc;
extern FILE *yyerrfile;
extern int   yyerrlevel;
extern int   yylineno;

#ifdef __cplusplus
extern "C" {
#endif

void yyerror(char* s, ...);
//void yyerror(char *s);

#ifdef __cplusplus
}
#endif

void yyreport(int level, char* s, ...);

#endif

