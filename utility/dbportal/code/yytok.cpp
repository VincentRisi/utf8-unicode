#include "machine.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <ctype.h>

#include "global.h"
#include "yyerror.h"

#define DIM(a) (sizeof(a)/sizeof(a[0]))
#define YYERRCODE 256

char *yyerrsrc;
int yyerrlevel;
int yylineno=1;
int yycolumn=1;
yyhandlerptr yyhandler;

void yyerror(const char* s, ...)
{
  char yyerrbuff[512];
  va_list argptr;
  va_start(argptr, s);
  vsprintf(yyerrbuff, s, argptr);
  if (yyhandler) yyhandler(yyerrbuff);
}

void yyreport(int level, const char* s, ...)
{
  char yyerrbuff[512];
  va_list argptr;
  if (yyerrlevel < level)
     yyerrlevel = level;
  int n=sprintf(yyerrbuff, "%s(%d): %d ", yyerrsrc, yylineno, level);
  va_start(argptr, s);
  vsprintf(yyerrbuff+n, s, argptr);
  if (yyhandler) yyhandler(yyerrbuff);
}

const char *yytoken(int aTokenNo)
{
  const char *Token[] =
  { "ALL"
  , "AUDIT"
  , "BINARY"
  , "BLOB"
  , "BOOLEAN"
  , "BULKINSERT"
  , "BULKUPDATE"
  , "CASCADE"
  , "CHAR"
  , "CHARCONST"
  , "CHECK"
  , "CLOB"
  , "CODE"
  , "CONNECT"
  , "COUNT"
  , "CURSOR"
  , "DATETIME"
  , "DATA"
  , "DATE"
  , "DECLARE"
  , "DELETE"
  , "DELETEALL"
  , "DELETEONE"
  , "ENDCODE"
  , "ENDCURSOR"
  , "ENDDATA"
  , "EXCEPTION"
  , "EXECUTE"
  , "EXISTS"
  , "EXTENDED"
  , "FAST"
  , "FLOAT"
  , "FREELIST"
  , "FREELISTS"
  , "FROM"
  , "FULL"
  , "GRANT"
  , "GROUPS"
  , "HUGECHAR"
  , "IDENT"
  , "INITIAL"
  , "IMAGE"
  , "INITRANS"
  , "INOUT"
  , "INPUT"
  , "INSERT"
  , "INT"
  , "K"
  , "KEY"
  , "LINE"
  , "LINK"
  , "LONG"
  , "M"
  , "MAXEXTENTS"
  , "MAXTRANS"
  , "MINEXTENTS"
  , "MONEY"
  , "MULTIPLE"
  , "NCHAR"
  , "NEXT"
  , "NOC"
  , "NOEXT"
  , "NOIDL"
  , "NOPAS"
  , "NOT"
  , "NOVB"
  , "NSEQUENCE"
  , "NULL"
  , "NUMBER"
  , "OPTIMAL"
  , "OUTPUT"
  , "PCTFREE"
  , "PCTINCREASE"
  , "PCTUSED"
  , "PRIMARY"
  , "PROC"
  , "SCHEMA"
  , "SELECT"
  , "SELECTALL"
  , "SELECTALLUPD"
  , "SELECTONE"
  , "SELECTONEUPD"
  , "SELECTPLUS"
  , "SEQUENCE"
  , "SERVER"
  , "SINGLE"
  , "SMALLINT"
  , "SQL"
  , "SQLCODE"
  , "STRING"
  , "TABLE"
  , "TABLESPACE"
  , "TIMESTAMP"
  , "TINYINT"
  , "TO"
  , "UNIQUE"
  , "UPDATE"
  , "UPPER"
  , "USEDBY"
  , "USERSTAMP"
  , "VARNUM"
  , "VIEW"
  , "WITH"
  , "XMLTYPE"
  , "ZLOB"
  , "BINDIR"
  , "BINEXT"
  , "BLANKPADDED"
  , "CDIR"
  , "CEXT"
  , "CHARZ"
  , "CONNREQD"
  , "CONTROLDB"
  , "CPPHEADER"
  , "EXITREQD"
  , "EIGHTBYTE"
  , "EXTENDEDC"
  , "EXTENDEDPAS"
  , "EXTENDEDVB"
  , "HEADERDIR"
  , "HEADEREXT"
  , "IDLDIR"
  , "IDLEXT"
  , "IDLMODULE"
  , "IDLUBI"
  , "INTERNAL"
  , "VERSION2BIN"
  , "LITTLETRUE"
  , "LOGDIR"
  , "LOGEXT"
  , "NODOMAIN"
  , "ONESQLSCRIPT"
  , "PASDIR"
  , "PASEXT"
  , "PYTHONDIR"
  , "PYTHONEXT"
  , "SHOW"
  , "SODIR"
  , "SOEXT"
  , "SQLAUDITEXT"
  , "SQLCONEXT"
  , "SQLDIR"
  , "SQLEXT"
  , "SQLGRANTEXT"
  , "SQLIDXEXT"
  , "SQLPROCEXT"
  , "SQLSNAPEXT"
  , "SQLTABLEEXT"
  , "SQLVIEWSEXT"
  , "TARGETC"
  , "TARGETIDL"
  , "TARGETPAS"
  , "ARCHIVEDIR"
  , "ARCHIVEEXT"
  , "PARMSDIR"
  , "PARMSDESCR"
  , "PARMSEXT"
  , "PARMSLOOKUP"
  , "TARGETARCHIVE"
  , "TARGETPARMS"
  , "TARGETPYTHON"
  , "TARGETVB"
  , "TARGETVB5"
  , "UCONNECT"
  , "UNDERSCORE"
  , "VB5DIR"
  , "VB5EXT"
  , "VBDIR"
  , "VBEXT"
  , "VBFORIDLDIR"
  , "INVALID"
  , "PREFIXVBCLASSES"
  , "TARGETVBCODE3"
  , "TARGETVBNET7"
  , "TARGETCSNET7"
  , "TARGETCSIDL2"
  , "TARGETCSADONET"
  , "TARGETCSRW"
  , "VBCODE3DIR"
  , "VBCODE3EXT"
  , "VBNET7DIR"
  , "VBNET7EXT"
  , "CSNET7DIR"
  , "CSADONETDIR"
  , "CSRWDIR"
  , "CSIDL2DIR"
  , "CSNET7EXT"
  , "CSADONETEXT"
  , "CSRWEXT"
  , "CSIDL2EXT"
  , "VIEWONLY"
  , "TARGETVBFORIDL"
  , "TARGETVBFORADOR"
  , "LITTLETCSIDL2"
  };
  static char Buf[32];
  switch (aTokenNo)
  {
  case 0:
    return ("[end of file]");
  case YYERRCODE:
    return ("[error]");
  case '\a':
    return "'\\a'";
  case '\b':
    return "'\\b'";
  case '\f':
    return "'\\f'";
  case '\n':
    return "'\\n'";
  case '\r':
    return "'\\r'";
  case '\t':
    return "'\\t'";
  case '\v':
    return "'\\v'";
  }
  if (aTokenNo > YYERRCODE && aTokenNo < YYERRCODE + DIM(Token))
    return Token[aTokenNo-(YYERRCODE+1)];
  if (isascii(aTokenNo) && isprint(aTokenNo))
    sprintf(Buf, "'%c'", aTokenNo);
  else
    sprintf(Buf, "<%d>", aTokenNo);
  return Buf;
}
