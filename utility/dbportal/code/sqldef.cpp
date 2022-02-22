
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0

/* "%code top" blocks.  */

/* Line 171 of yacc.c  */
#line 1 "C:/vlab/utility/dbportal/code/sqldef.y"

  #include "machine.h"

  #include <stdio.h>
  #include <stdlib.h>
  #include <stdarg.h>
  #include <ctype.h>

  #include "xstring.h"
  #include "global.h"
  #include "sqltok.h"

  PYYTable Table;
/* ------------------------ */
/*  static global variables */
/* ------------------------ */
static eState    GrantState;
static eState    ProjectState;
static eState    FieldState;
static eState    LineState;
static eState    ColumnState;
static int32     ExtState;
static TYYConst  Const;
static TYYField  Field;
static TYYKey    Key;
static TYYLink   Link;
static TYYGrant  Grant;
static TYYCursor Cursor;
static TYYProc   Proc;
static TYYView   View;
static TYYWith   With;
static TYYLine   UseWith;
static TYYLine   Line;
static TYYLine   Perm;
static TYYLine   User;
static char      NumberSize;



/* Line 171 of yacc.c  */
#line 111 "C:/vlab/utility/dbportal/code/sqldef.cpp"


/* Copy the first part of user declarations.  */


/* Line 189 of yacc.c  */
#line 118 "C:/vlab/utility/dbportal/code/sqldef.cpp"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     sqldefALL = 258,
     sqldefAUDIT = 259,
     sqldefBINARY = 260,
     sqldefBLOB = 261,
     sqldefBOOLEAN = 262,
     sqldefBULKINSERT = 263,
     sqldefBULKUPDATE = 264,
     sqldefCASCADE = 265,
     sqldefCHAR = 266,
     sqldefCHARCONST = 267,
     sqldefCHECK = 268,
     sqldefCLOB = 269,
     sqldefCODE = 270,
     sqldefCONNECT = 271,
     sqldefCOUNT = 272,
     sqldefCURSOR = 273,
     sqldefDATETIME = 274,
     sqldefDATA = 275,
     sqldefDATE = 276,
     sqldefDECLARE = 277,
     sqldefDELETE = 278,
     sqldefDELETEALL = 279,
     sqldefDELETEONE = 280,
     sqldefENDCODE = 281,
     sqldefENDCURSOR = 282,
     sqldefENDDATA = 283,
     sqldefEXCEPTION = 284,
     sqldefEXECUTE = 285,
     sqldefEXISTS = 286,
     sqldefEXTENDED = 287,
     sqldefFAST = 288,
     sqldefFLOAT = 289,
     sqldefFREELIST = 290,
     sqldefFREELISTS = 291,
     sqldefFROM = 292,
     sqldefFULL = 293,
     sqldefGRANT = 294,
     sqldefGROUPS = 295,
     sqldefHUGECHAR = 296,
     sqldefIDENT = 297,
     sqldefINITIAL = 298,
     sqldefIMAGE = 299,
     sqldefINITRANS = 300,
     sqldefINOUT = 301,
     sqldefINPUT = 302,
     sqldefINSERT = 303,
     sqldefINT = 304,
     sqldefK = 305,
     sqldefKEY = 306,
     sqldefLINE = 307,
     sqldefLINK = 308,
     sqldefLONG = 309,
     sqldefM = 310,
     sqldefMAXEXTENTS = 311,
     sqldefMAXTRANS = 312,
     sqldefMINEXTENTS = 313,
     sqldefMONEY = 314,
     sqldefMULTIPLE = 315,
     sqldefNCHAR = 316,
     sqldefNEXT = 317,
     sqldefNOC = 318,
     sqldefNOEXT = 319,
     sqldefNOIDL = 320,
     sqldefNOPAS = 321,
     sqldefNOT = 322,
     sqldefNOVB = 323,
     sqldefNSEQUENCE = 324,
     sqldefNULL = 325,
     sqldefNUMBER = 326,
     sqldefOPTIMAL = 327,
     sqldefOUTPUT = 328,
     sqldefPCTFREE = 329,
     sqldefPCTINCREASE = 330,
     sqldefPCTUSED = 331,
     sqldefPRIMARY = 332,
     sqldefPROC = 333,
     sqldefSCHEMA = 334,
     sqldefSELECT = 335,
     sqldefSELECTALL = 336,
     sqldefSELECTALLUPD = 337,
     sqldefSELECTONE = 338,
     sqldefSELECTONEUPD = 339,
     sqldefSELECTPLUS = 340,
     sqldefSEQUENCE = 341,
     sqldefSERVER = 342,
     sqldefSINGLE = 343,
     sqldefSMALLINT = 344,
     sqldefSQL = 345,
     sqldefSQLCODE = 346,
     sqldefSTRING = 347,
     sqldefTABLE = 348,
     sqldefTABLESPACE = 349,
     sqldefTIMESTAMP = 350,
     sqldefTINYINT = 351,
     sqldefTO = 352,
     sqldefUNIQUE = 353,
     sqldefUPDATE = 354,
     sqldefUPPER = 355,
     sqldefUSEDBY = 356,
     sqldefUSERSTAMP = 357,
     sqldefVARNUM = 358,
     sqldefVIEW = 359,
     sqldefWITH = 360,
     sqldefXMLTYPE = 361,
     sqldefZLOB = 362,
     switchBINDIR = 363,
     switchBINEXT = 364,
     switchBLANKPADDED = 365,
     switchCDIR = 366,
     switchCEXT = 367,
     switchCHARZ = 368,
     switchCONNREQD = 369,
     switchCONTROLDB = 370,
     switchCPPHEADER = 371,
     switchEXITREQD = 372,
     switchEIGHTBYTE = 373,
     switchEXTENDEDC = 374,
     switchEXTENDEDPAS = 375,
     switchEXTENDEDVB = 376,
     switchHEADERDIR = 377,
     switchHEADEREXT = 378,
     switchIDLDIR = 379,
     switchIDLEXT = 380,
     switchIDLMODULE = 381,
     switchIDLUBI = 382,
     switchINTERNAL = 383,
     switchVERSION2BIN = 384,
     switchLITTLETRUE = 385,
     switchLOGDIR = 386,
     switchLOGEXT = 387,
     switchNODOMAIN = 388,
     switchONESQLSCRIPT = 389,
     switchPASDIR = 390,
     switchPASEXT = 391,
     switchPYTHONDIR = 392,
     switchPYTHONEXT = 393,
     switchSHOW = 394,
     switchSODIR = 395,
     switchSOEXT = 396,
     switchSQLAUDITEXT = 397,
     switchSQLCONEXT = 398,
     switchSQLDIR = 399,
     switchSQLEXT = 400,
     switchSQLGRANTEXT = 401,
     switchSQLIDXEXT = 402,
     switchSQLPROCEXT = 403,
     switchSQLSNAPEXT = 404,
     switchSQLTABLEEXT = 405,
     switchSQLVIEWSEXT = 406,
     switchTARGETC = 407,
     switchTARGETIDL = 408,
     switchTARGETPAS = 409,
     switchARCHIVEDIR = 410,
     switchARCHIVEEXT = 411,
     switchPARMSDIR = 412,
     switchPARMSDESCR = 413,
     switchPARMSEXT = 414,
     switchPARMSLOOKUP = 415,
     switchTARGETARCHIVE = 416,
     switchTARGETPARMS = 417,
     switchTARGETPYTHON = 418,
     switchTARGETVB = 419,
     switchTARGETVB5 = 420,
     switchUCONNECT = 421,
     switchUNDERSCORE = 422,
     switchVB5DIR = 423,
     switchVB5EXT = 424,
     switchVBDIR = 425,
     switchVBEXT = 426,
     switchVBFORIDLDIR = 427,
     switchINVALID = 428,
     switchPREFIXVBCLASSES = 429,
     switchTARGETVBCODE3 = 430,
     switchTARGETVBNET7 = 431,
     switchTARGETCSNET7 = 432,
     switchTARGETCSIDL2 = 433,
     switchTARGETCSADONET = 434,
     switchTARGETCSRW = 435,
     switchVBCODE3DIR = 436,
     switchVBCODE3EXT = 437,
     switchVBNET7DIR = 438,
     switchVBNET7EXT = 439,
     switchCSNET7DIR = 440,
     switchCSADONETDIR = 441,
     switchCSRWDIR = 442,
     switchCSIDL2DIR = 443,
     switchCSNET7EXT = 444,
     switchCSADONETEXT = 445,
     switchCSRWEXT = 446,
     switchCSIDL2EXT = 447,
     switchVIEWONLY = 448,
     switchTARGETVBFORIDL = 449,
     switchTARGETVBFORADOR = 450,
     switchLITTLETCSIDL2 = 451
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 39 "C:/vlab/utility/dbportal/code/sqldef.y"

    int   i;
    char* s;



/* Line 214 of yacc.c  */
#line 357 "C:/vlab/utility/dbportal/code/sqldef.cpp"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 369 "C:/vlab/utility/dbportal/code/sqldef.cpp"
/* Unqualified %code blocks.  */

/* Line 265 of yacc.c  */
#line 44 "C:/vlab/utility/dbportal/code/sqldef.y"

    /*  ---------- */
    /*  prototypes */
    /*  ---------- */
    static void SetLink(char* s);
    static void AddLink(void);
    static void DeleteCascadeLink(bool val);
    static void AddField(char* s);
    static void AddCharConst(char* s, int i);
    static void AddConst(char* s, int i);
    static void AddConstIdent(char* s);
    static void AddStringConst(char* s, char* d);
    static void UseStdRecOutput();
    static void SetFieldType(eFieldType Type, int Length);
    static void SetSameFromTable();
    static void SetFromTable(char *s);
    static void AddPermission(char* s);
    static void AddUser(char* s);
    static void AddProject(char* s);
    static void AddGrant(void);
    static void SetKey(char* s);
    static void AddKey(void);
    static void AddColumn(char* s);
    static void SetProc(char *s);
    static void SetFlag(char *s);
    static void SetView(char *s);
    static void SetWith(char *s);
    static void AddProc(void);
    static void AddView(void);
    static void AddLine(char *s);
    static void AddWith(void);
    static void AddUseWith(char* s);
    static PYYField FindField(char* s);
    static char *Concat(char *s1, char *s2);
    static char *Combine(char *s1, char *s2);
    static void CreateDelVer(void);
    static void CreateRowSeqExists(void);
    static void CreateCount(void);
    static void CreateDeleteAll(void);
    static void CreateDeleteOne(void);
    static void CreateInsert(void);
    static void CreateInsertUsingReturning(void);
    static void CreateSelectAll(int Update);
    static void CreateSelectOne(int Update);
    static void CreateSelectPlus(int size);
    static void CreateUpdate(void);
    static void CreateUpdateUsingReturning(void);
    static void CreateExists(void);
    static pchar Qualify(pchar TableName, pchar FieldName);
    static pchar InputConversion(TYYField Field, int Extended = 0);
    static pchar OutputConversion(TYYField Field);
    static void AddRowUser(void);
    static void AddRowUpdated(void);
    static void AddRowStatus(void);
    static int32 Hash(char *s, int m);
    static void SetSwitch(char *&Switch, char *value);
    static void SetSwitch(int  &Switch,  int value);

    #define AddList(Pointer, Type, Data, Index, Delta) do {\
      if (Index % Delta == 0) \
        Pointer = (Type*) realloc(Pointer, sizeof(Data)*(Index + Delta)); \
      Pointer[Index++] = Data; \
      } while(0)

    char *OracleReservedWords = ":"
      "ABS:ACCESS:ACOS:ADD:ADD_MONTHS:ALL:ALTER:AND:ANY:AS:ASC:ASCII:ASCIISTR:ASIN:"
      "ATAN:ATAN2:AUDIT:AVG:BETWEEN:BFILENAME:BIN_TO_NUM:BITAND:BY:CAST:CEIL:CHAR:"
      "CHARTOROWID:CHECK:CHR:CLOB:CLUSTER:COALESCE:COLUMN:COMMENT:COMPOSE:COMPRESS:"
      "CONCAT:CONNECT:CONVERT:CORR:COS:COSH:COUNT:COVAR_POP:COVAR_SAMP:CREATE:"
      "CUME_DIST:CURRENT:CURRENT_DATE:CURRENT_TIMESTAMP:DATE:DBTIMEZONE:DECIMAL:"
      "DECODE:DECOMPOSE:DEFAULT:DELETE:DENSE_RANK:DEREF:DESC:DISTINCT:DROP:DUMP:ELSE:"
      "EMPTY_BLOB:EMPTY_:EXCLUSIVE:EXISTS:EXISTSNODE:EXP:EXTRACT:FILE:FIRST:FIRST:"
      "FIRST_VALUE:FLOAT:FLOOR:FOR:FROM:FROM_TZ:GRANT:GREATEST:GROUP:GROUPING:"
      "GROUPING_ID:GROUP_ID:HAVING:HEXTORAW:IDENTIFIED:IMMEDIATE:IN:INCREMENT:"
      "INDEX:INITCAP:INITIAL:INSERT:INSTR:INTEGER:INTERSECT:INTO:IS:LAG:LAST:LAST:"
      "LAST_DAY:LAST_VALUE:LEAD:LEAST:LENGTH:LEVEL:LIKE:LN:LOCALTIMESTAMP:LOCK:LOG:"
      "LONG:LOWER:LPAD:LTRIM:MAKE_REF:MAX:MAXEXTENTS:MIN:MINUS:MLSLABEL:MOD:MODE:"
      "MODIFY:MONTHS_BETWEEN:NCHAR:NEW_TIME:NEXT_DAY:NLSSORT:NLS_CHARSET_DECL_LEN:"
      "NLS_CHARSET_ID:NLS_CHARSET_NAME:NLS_INITCAP:NLS_LOWER:NLS_UPPER:NOAUDIT:"
      "NOCOMPRESS:NOT:NOWAIT:NSEQUENCE:NTILE:NULL:NULLIF:NUMBER:NUMTODSINTERVAL:"
      "NUMTODSINTERVAL:NUMTOYMINTERVAL:NUMTOYMINTERVAL:NVL:"
      "NVL2:OF:OFFLINE:ON:ONLINE:OPTION:OR:ORDER:PCTFREE:PERCENTILE_CONT:"
      "PERCENTILE_DISC:PERCENT_RANK:POWER:PRIOR:PRIVILEGES:PUBLIC:RANK:"
      "RATIO_TO_REPORT:RAW:RAWTOHEX:RAWTONHEX:REF:REFTOHEX:REGR_:RENAME:REPLACE:"
      "RESOURCE:REVOKE:ROUND:ROUND:ROW:ROWID:ROWIDTOCHAR:ROWIDTONCHAR:ROWNUM:ROWS:"
      "ROW_NUMBER:RPAD:RTRIM:SELECT:SESSION:SESSIONTIMEZONE:SET:SHARE:SIGN:SIN:SINH:"
      "SIZE:SMALLINT:SOUNDEX:SQRT:START:STDDEV:STDDEV_POP:STDDEV_SAMP:SUBSTR:"
      "SUCCESSFUL:SUM:SYNONYM:SYSDATE:SYSTIMESTAMP:SYS_CONNECT_BY_PATH:"
      "SYS_CONTEXT:SYS_DBURIGEN:SYS_EXTRACT_UTC:SYS_GUID:SYS_TYPEID:SYS_XMLAGG:"
      "SYS_XMLGEN:TABLE:TAN:TANH:THEN:TO:TO_CHAR:TO_CLOB:TO_DATE:TO_DSINTERVAL:"
      "TO_LOB:TO_MULTI_BYTE:TO_NCHAR:TO_NCLOB:TO_NUMBER:TO_SINGLE_BYTE:TO_TIMESTAMP:"
      "TO_TIMESTAMP_TZ:TO_YMINTERVAL:TRANSLATE:TREAT:TRIGGER:TRIM:TRUNC:TZ_OFFSET:"
      "UID:UNION:UNIQUE:UNISTR:UPDATE:UPPER:USER:USERENV:USING:VALIDATE:VALUE:VALUES:"
      "VARCHAR:VARCHAR2:VARIANCE:VAR_POP:VAR_SAMP:VIEW:VSIZE:WHENEVER:WHERE:WITH:"
      ;



/* Line 265 of yacc.c  */
#line 473 "C:/vlab/utility/dbportal/code/sqldef.cpp"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   692

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  208
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  115
/* YYNRULES -- Number of rules.  */
#define YYNRULES  362
/* YYNRULES -- Number of states.  */
#define YYNSTATES  626

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   452

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     197,   198,   207,     2,   203,     2,     2,   199,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   202,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   200,     2,   201,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   204,     2,   205,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   206
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,    13,    14,    15,    18,    21,    24,    27,
      30,    33,    36,    39,    42,    45,    48,    51,    54,    57,
      60,    63,    66,    69,    72,    75,    78,    81,    84,    87,
      90,    93,    96,    99,   102,   105,   108,   111,   114,   117,
     120,   123,   126,   129,   132,   135,   138,   141,   144,   147,
     150,   153,   156,   159,   162,   165,   168,   171,   174,   177,
     180,   183,   186,   189,   192,   195,   198,   201,   204,   207,
     210,   213,   216,   219,   222,   225,   228,   231,   234,   237,
     240,   243,   246,   249,   252,   255,   258,   261,   264,   267,
     270,   273,   276,   279,   281,   282,   284,   287,   289,   291,
     292,   295,   299,   301,   304,   309,   310,   313,   317,   320,
     324,   327,   330,   333,   336,   340,   344,   347,   350,   353,
     356,   358,   359,   361,   363,   367,   368,   373,   375,   377,
     378,   383,   388,   389,   392,   393,   396,   397,   404,   405,
     406,   410,   414,   417,   420,   422,   425,   428,   431,   434,
     437,   439,   442,   445,   447,   450,   453,   456,   459,   462,
     465,   468,   471,   474,   476,   479,   480,   484,   485,   487,
     493,   494,   496,   497,   499,   501,   505,   509,   510,   513,
     515,   518,   520,   523,   524,   528,   530,   534,   536,   537,
     541,   545,   549,   551,   555,   556,   558,   562,   566,   570,
     571,   574,   576,   578,   580,   582,   584,   590,   591,   593,
     596,   598,   600,   602,   604,   606,   608,   610,   613,   615,
     620,   622,   623,   626,   628,   630,   633,   637,   640,   644,
     647,   650,   653,   656,   660,   664,   667,   670,   672,   676,
     678,   684,   685,   688,   689,   691,   693,   695,   699,   702,
     703,   705,   706,   709,   715,   721,   724,   726,   727,   728,
     729,   732,   735,   738,   741,   744,   747,   750,   753,   756,
     759,   763,   766,   769,   770,   772,   778,   779,   781,   786,
     787,   790,   792,   797,   803,   808,   809,   820,   824,   826,
     828,   831,   833,   835,   837,   839,   841,   842,   845,   848,
     851,   854,   857,   860,   863,   866,   869,   872,   875,   878,
     881,   884,   887,   890,   893,   896,   898,   899,   902,   904,
     907,   909,   911,   912,   916,   917,   921,   922,   923,   927,
     928,   929,   934,   936,   938,   939,   943,   947,   948,   949,
     952,   954,   957,   959,   960,   963,   967,   972,   973,   976,
     978,   982,   983,   984,   991,   997,  1002,  1003,  1004,  1006,
    1009,  1011,  1016
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     209,     0,    -1,   211,   214,   216,   219,   210,   220,   224,
     251,   272,    -1,    -1,    -1,   211,   212,    -1,   108,    92,
      -1,   109,    92,    -1,   111,    92,    -1,   112,    92,    -1,
     110,   213,    -1,   113,   213,    -1,   114,   213,    -1,   115,
      92,    -1,   118,   213,    -1,   117,   213,    -1,   119,   213,
      -1,   120,   213,    -1,   121,   213,    -1,   122,    92,    -1,
     123,    92,    -1,   124,    92,    -1,   125,    92,    -1,   126,
      92,    -1,   128,   213,    -1,   129,   213,    -1,   131,    92,
      -1,   132,    92,    -1,   133,   213,    -1,   134,   213,    -1,
     155,    92,    -1,   156,    92,    -1,   158,    92,    -1,   157,
      92,    -1,   159,    92,    -1,   160,    92,    -1,   135,    92,
      -1,   136,    92,    -1,   137,    92,    -1,   138,    92,    -1,
     139,    92,    -1,   140,    92,    -1,   141,    92,    -1,   142,
      92,    -1,   143,    92,    -1,   144,    92,    -1,   145,    92,
      -1,   146,    92,    -1,   147,    92,    -1,   148,    92,    -1,
     149,    92,    -1,   150,    92,    -1,   151,    92,    -1,   152,
     213,    -1,   153,   213,    -1,   161,   213,    -1,   162,   213,
      -1,   154,   213,    -1,   163,   213,    -1,   164,   213,    -1,
     165,   213,    -1,   175,   213,    -1,   176,   213,    -1,   177,
     213,    -1,   178,   213,    -1,   196,   213,    -1,   179,   213,
      -1,   180,   213,    -1,   194,   213,    -1,   195,   213,    -1,
     174,   213,    -1,   166,    92,    -1,   167,   213,    -1,   168,
      92,    -1,   169,    92,    -1,   181,    92,    -1,   182,    92,
      -1,   183,    92,    -1,   184,    92,    -1,   185,    92,    -1,
     186,    92,    -1,   187,    92,    -1,   189,    92,    -1,   190,
      92,    -1,   191,    92,    -1,   188,    92,    -1,   192,    92,
      -1,   172,    92,    -1,   170,    92,    -1,   171,    92,    -1,
     193,   213,    -1,   130,   213,    -1,   116,   213,    -1,   173,
      -1,    -1,   241,    -1,    87,   215,    -1,    42,    -1,    32,
      -1,    -1,   216,   217,    -1,   197,   218,   198,    -1,    42,
      -1,    79,   218,    -1,    16,   218,   199,   218,    -1,    -1,
     220,   221,    -1,    43,   222,   223,    -1,    45,   222,    -1,
      35,    40,   222,    -1,    36,   222,    -1,    58,   222,    -1,
      57,   222,    -1,    56,   222,    -1,    62,   222,   223,    -1,
      72,   222,   223,    -1,    74,   222,    -1,    76,   222,    -1,
      75,   222,    -1,    94,   218,    -1,    71,    -1,    -1,    50,
      -1,    55,    -1,   226,   225,   230,    -1,    -1,    93,   218,
     228,   229,    -1,    70,    -1,   218,    -1,    -1,   228,   200,
     227,   201,    -1,   228,   197,   227,   198,    -1,    -1,    13,
      92,    -1,    -1,   230,   231,    -1,    -1,   218,   233,   232,
     234,   235,   242,    -1,    -1,    -1,   197,   218,   198,    -1,
      11,   239,   244,    -1,    61,   239,    -1,     5,   239,    -1,
       7,    -1,    96,   247,    -1,    89,   247,    -1,    49,   247,
      -1,   202,   236,    -1,    54,   247,    -1,    21,    -1,    95,
     240,    -1,   102,   240,    -1,    86,    -1,    69,   239,    -1,
      34,   237,    -1,    59,   238,    -1,     6,   241,    -1,    14,
     241,    -1,   107,   241,    -1,    41,   240,    -1,    44,   239,
      -1,   106,   241,    -1,    19,    -1,   103,   237,    -1,    -1,
     197,   218,   198,    -1,    -1,   238,    -1,   197,   241,   203,
     241,   198,    -1,    -1,   241,    -1,    -1,   241,    -1,   222,
      -1,   200,   222,   201,    -1,   197,   222,   198,    -1,    -1,
     242,   243,    -1,    70,    -1,    67,    70,    -1,   100,    -1,
      13,    92,    -1,    -1,   204,   245,   205,    -1,   246,    -1,
     245,   249,   246,    -1,   218,    -1,    -1,   200,   248,   201,
      -1,   197,   248,   198,    -1,   204,   248,   205,    -1,   250,
      -1,   248,   249,   250,    -1,    -1,   203,    -1,   218,   202,
     241,    -1,   218,   202,    12,    -1,   218,   202,    92,    -1,
      -1,   251,   252,    -1,   253,    -1,   259,    -1,   265,    -1,
     270,    -1,   279,    -1,    39,   254,   255,    97,   257,    -1,
      -1,   256,    -1,   255,   256,    -1,     3,    -1,    23,    -1,
      48,    -1,    80,    -1,    99,    -1,    30,    -1,   258,    -1,
     257,   258,    -1,   218,    -1,    51,   260,   261,   263,    -1,
     218,    -1,    -1,   261,   262,    -1,    98,    -1,    77,    -1,
      94,   218,    -1,    43,   222,   223,    -1,    45,   222,    -1,
      35,    40,   222,    -1,    36,   222,    -1,    58,   222,    -1,
      57,   222,    -1,    56,   222,    -1,    62,   222,   223,    -1,
      72,   222,   223,    -1,    74,   222,    -1,    75,   222,    -1,
     264,    -1,   263,   249,   264,    -1,   218,    -1,    53,   267,
     268,   263,   266,    -1,    -1,    23,    10,    -1,    -1,   269,
      -1,     1,    -1,    42,    -1,   269,   206,    42,    -1,     4,
     271,    -1,    -1,    38,    -1,    -1,   272,   273,    -1,    78,
     274,   277,   298,   294,    -1,    78,   274,   275,   276,   288,
      -1,   105,   284,    -1,   321,    -1,    -1,    -1,    -1,    17,
     289,    -1,    24,   289,    -1,    25,   289,    -1,    31,   289,
      -1,     8,   289,    -1,    48,   289,    -1,    81,   289,    -1,
      83,   289,    -1,    82,   289,    -1,    84,   289,    -1,    85,
     278,   289,    -1,     9,   289,    -1,    99,   289,    -1,    -1,
     241,    -1,   104,   281,   280,   298,   282,    -1,    -1,    42,
      -1,    15,   283,   319,    26,    -1,    -1,   285,   286,    -1,
      42,    -1,    15,   287,   319,    26,    -1,    90,    15,   287,
     319,    26,    -1,    91,   287,   319,    26,    -1,    -1,   297,
     289,   298,   294,   299,   301,   303,   307,   310,   316,    -1,
     197,   290,   198,    -1,   292,    -1,   291,    -1,   290,   291,
      -1,    63,    -1,    66,    -1,    68,    -1,    64,    -1,    65,
      -1,    -1,   292,   293,    -1,   119,   213,    -1,   120,   213,
      -1,   121,   213,    -1,   152,   213,    -1,   153,   213,    -1,
     154,   213,    -1,   163,   213,    -1,   164,   213,    -1,   165,
     213,    -1,   175,   213,    -1,   176,   213,    -1,   177,   213,
      -1,   179,   213,    -1,   180,   213,    -1,   178,   213,    -1,
     194,   213,    -1,   195,   213,    -1,   173,    -1,    -1,   101,
     295,    -1,   296,    -1,   295,   296,    -1,   218,    -1,    42,
      -1,    -1,    39,    97,   257,    -1,    -1,    47,   300,   230,
      -1,    -1,    -1,    46,   302,   230,    -1,    -1,    -1,    73,
     306,   305,   304,    -1,   207,    -1,   230,    -1,    -1,   197,
      60,   198,    -1,   197,    88,   198,    -1,    -1,    -1,   105,
     308,    -1,   309,    -1,   308,   309,    -1,   218,    -1,    -1,
     310,   311,    -1,    22,   315,   312,    -1,    18,   314,   319,
      27,    -1,    -1,   312,   313,    -1,   231,    -1,   218,   233,
      29,    -1,    -1,    -1,    90,   318,    15,   317,   319,    26,
      -1,    91,   318,   317,   319,    26,    -1,    15,   317,   319,
      26,    -1,    -1,    -1,   320,    -1,   319,   320,    -1,    52,
      -1,    20,   322,   319,    28,    -1,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   353,   353,   357,   380,   382,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   454,   455,   456,   457,   458,   459,
     460,   461,   462,   463,   464,   465,   466,   467,   468,   469,
     470,   471,   472,   473,   482,   485,   492,   498,   502,   517,
     519,   523,   529,   535,   540,   546,   548,   552,   557,   561,
     565,   569,   573,   577,   581,   586,   591,   595,   599,   603,
     609,   616,   619,   623,   629,   633,   638,   646,   650,   656,
     658,   659,   664,   667,   672,   674,   678,   678,   684,   690,
     693,   699,   703,   707,   711,   715,   719,   723,   727,   728,
     732,   736,   740,   744,   755,   766,   770,   774,   782,   790,
     798,   806,   814,   822,   826,   833,   836,   844,   848,   852,
     861,   864,   872,   875,   882,   886,   890,   895,   897,   901,
     905,   909,   913,   920,   922,   926,   927,   931,   937,   939,
     940,   941,   945,   946,   949,   951,   955,   959,   963,   969,
     971,   975,   976,   977,   978,   979,   983,   989,   995,   996,
    1000,  1006,  1010,  1014,  1019,  1023,  1029,  1030,  1034,  1040,
    1046,  1052,  1054,  1058,  1062,  1067,  1071,  1076,  1080,  1084,
    1088,  1092,  1096,  1100,  1105,  1110,  1114,  1120,  1121,  1125,
    1131,  1138,  1141,  1148,  1153,  1157,  1163,  1167,  1173,  1178,
    1181,  1187,  1189,  1193,  1194,  1195,  1196,  1200,  1205,  1210,
    1215,  1224,  1233,  1244,  1255,  1260,  1274,  1283,  1294,  1303,
    1314,  1323,  1327,  1346,  1349,  1355,  1361,  1366,  1372,  1376,
    1381,  1387,  1393,  1394,  1395,  1399,  1404,  1411,  1412,  1416,
    1417,  1421,  1425,  1429,  1433,  1437,  1442,  1444,  1448,  1453,
    1458,  1463,  1468,  1473,  1478,  1483,  1488,  1493,  1498,  1503,
    1508,  1513,  1518,  1523,  1528,  1533,  1540,  1542,  1546,  1547,
    1551,  1557,  1562,  1564,  1567,  1569,  1574,  1578,  1580,  1585,
    1589,  1591,  1595,  1599,  1604,  1608,  1613,  1621,  1625,  1627,
    1631,  1632,  1636,  1642,  1644,  1648,  1649,  1654,  1656,  1660,
    1661,  1669,  1676,  1681,  1682,  1683,  1688,  1694,  1701,  1702,
    1706,  1714,  1722
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "sqldefALL", "sqldefAUDIT",
  "sqldefBINARY", "sqldefBLOB", "sqldefBOOLEAN", "sqldefBULKINSERT",
  "sqldefBULKUPDATE", "sqldefCASCADE", "sqldefCHAR", "sqldefCHARCONST",
  "sqldefCHECK", "sqldefCLOB", "sqldefCODE", "sqldefCONNECT",
  "sqldefCOUNT", "sqldefCURSOR", "sqldefDATETIME", "sqldefDATA",
  "sqldefDATE", "sqldefDECLARE", "sqldefDELETE", "sqldefDELETEALL",
  "sqldefDELETEONE", "sqldefENDCODE", "sqldefENDCURSOR", "sqldefENDDATA",
  "sqldefEXCEPTION", "sqldefEXECUTE", "sqldefEXISTS", "sqldefEXTENDED",
  "sqldefFAST", "sqldefFLOAT", "sqldefFREELIST", "sqldefFREELISTS",
  "sqldefFROM", "sqldefFULL", "sqldefGRANT", "sqldefGROUPS",
  "sqldefHUGECHAR", "sqldefIDENT", "sqldefINITIAL", "sqldefIMAGE",
  "sqldefINITRANS", "sqldefINOUT", "sqldefINPUT", "sqldefINSERT",
  "sqldefINT", "sqldefK", "sqldefKEY", "sqldefLINE", "sqldefLINK",
  "sqldefLONG", "sqldefM", "sqldefMAXEXTENTS", "sqldefMAXTRANS",
  "sqldefMINEXTENTS", "sqldefMONEY", "sqldefMULTIPLE", "sqldefNCHAR",
  "sqldefNEXT", "sqldefNOC", "sqldefNOEXT", "sqldefNOIDL", "sqldefNOPAS",
  "sqldefNOT", "sqldefNOVB", "sqldefNSEQUENCE", "sqldefNULL",
  "sqldefNUMBER", "sqldefOPTIMAL", "sqldefOUTPUT", "sqldefPCTFREE",
  "sqldefPCTINCREASE", "sqldefPCTUSED", "sqldefPRIMARY", "sqldefPROC",
  "sqldefSCHEMA", "sqldefSELECT", "sqldefSELECTALL", "sqldefSELECTALLUPD",
  "sqldefSELECTONE", "sqldefSELECTONEUPD", "sqldefSELECTPLUS",
  "sqldefSEQUENCE", "sqldefSERVER", "sqldefSINGLE", "sqldefSMALLINT",
  "sqldefSQL", "sqldefSQLCODE", "sqldefSTRING", "sqldefTABLE",
  "sqldefTABLESPACE", "sqldefTIMESTAMP", "sqldefTINYINT", "sqldefTO",
  "sqldefUNIQUE", "sqldefUPDATE", "sqldefUPPER", "sqldefUSEDBY",
  "sqldefUSERSTAMP", "sqldefVARNUM", "sqldefVIEW", "sqldefWITH",
  "sqldefXMLTYPE", "sqldefZLOB", "switchBINDIR", "switchBINEXT",
  "switchBLANKPADDED", "switchCDIR", "switchCEXT", "switchCHARZ",
  "switchCONNREQD", "switchCONTROLDB", "switchCPPHEADER", "switchEXITREQD",
  "switchEIGHTBYTE", "switchEXTENDEDC", "switchEXTENDEDPAS",
  "switchEXTENDEDVB", "switchHEADERDIR", "switchHEADEREXT", "switchIDLDIR",
  "switchIDLEXT", "switchIDLMODULE", "switchIDLUBI", "switchINTERNAL",
  "switchVERSION2BIN", "switchLITTLETRUE", "switchLOGDIR", "switchLOGEXT",
  "switchNODOMAIN", "switchONESQLSCRIPT", "switchPASDIR", "switchPASEXT",
  "switchPYTHONDIR", "switchPYTHONEXT", "switchSHOW", "switchSODIR",
  "switchSOEXT", "switchSQLAUDITEXT", "switchSQLCONEXT", "switchSQLDIR",
  "switchSQLEXT", "switchSQLGRANTEXT", "switchSQLIDXEXT",
  "switchSQLPROCEXT", "switchSQLSNAPEXT", "switchSQLTABLEEXT",
  "switchSQLVIEWSEXT", "switchTARGETC", "switchTARGETIDL",
  "switchTARGETPAS", "switchARCHIVEDIR", "switchARCHIVEEXT",
  "switchPARMSDIR", "switchPARMSDESCR", "switchPARMSEXT",
  "switchPARMSLOOKUP", "switchTARGETARCHIVE", "switchTARGETPARMS",
  "switchTARGETPYTHON", "switchTARGETVB", "switchTARGETVB5",
  "switchUCONNECT", "switchUNDERSCORE", "switchVB5DIR", "switchVB5EXT",
  "switchVBDIR", "switchVBEXT", "switchVBFORIDLDIR", "switchINVALID",
  "switchPREFIXVBCLASSES", "switchTARGETVBCODE3", "switchTARGETVBNET7",
  "switchTARGETCSNET7", "switchTARGETCSIDL2", "switchTARGETCSADONET",
  "switchTARGETCSRW", "switchVBCODE3DIR", "switchVBCODE3EXT",
  "switchVBNET7DIR", "switchVBNET7EXT", "switchCSNET7DIR",
  "switchCSADONETDIR", "switchCSRWDIR", "switchCSIDL2DIR",
  "switchCSNET7EXT", "switchCSADONETEXT", "switchCSRWEXT",
  "switchCSIDL2EXT", "switchVIEWONLY", "switchTARGETVBFORIDL",
  "switchTARGETVBFORADOR", "switchLITTLETCSIDL2", "'('", "')'", "'/'",
  "'['", "']'", "'='", "','", "'{'", "'}'", "\".\"", "'*'", "$accept",
  "SqlDef", "SetModDefaults", "Switches", "Switch", "SwitchNumber",
  "ServerName", "FullServerIdent", "Flags", "Flag", "AnIdent",
  "ConnectName", "OptTableMods", "TableMod", "ANumber", "NumberSize",
  "TableDef", "SetStateTable", "TableName", "AnIdentNull", "OptTableAlias",
  "OptTableCheck", "FieldDef", "Field", "$@1", "SetField", "OptAlias",
  "FieldType", "OptRefs", "OptPrecision", "Precision", "OptNumber",
  "Opt0Number", "Number", "OptNullUppers", "OptNullUpper",
  "OptConstIdents", "ConstIdents", "ConstIdent", "OptConsts", "Consts",
  "OptComma", "Const", "OptExtras", "Extra", "GrantDef", "SetGrant",
  "Permissions", "Permission", "Users", "User", "KeyDef", "KeyName",
  "Modifiers", "Modifier", "Columns", "Column", "Link", "OptDeleteCascade",
  "NewLink", "MasterName", "QualifiedIdent", "Audit", "AuditFull",
  "ProcDef", "Proc", "SetExt", "SetProcGrant", "SetProjectState",
  "StdProc", "OptPlusNumber", "ViewDef", "SetViewGrant", "ViewName",
  "ViewCode", "SetView", "WithProc", "WithName", "WithCode", "SetWith",
  "UserProc", "NoExt", "NoNos", "NoNo", "ProcSwitches", "ProcSwitch",
  "UsedBy", "Projects", "Project", "ProcName", "ToUsers", "Input",
  "SetStateInput", "InOut", "SetStateInOut", "Output", "ReUse",
  "OptMultiSingle", "SetStateOutput", "WithList", "Withs", "With",
  "Declares", "Declare", "DeclareFieldDef", "DeclareField", "SetCursor",
  "SetStateDeclare", "Code", "SetCode", "SetSql", "Lines", "Line", "Data",
  "SetData", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   417,   418,   419,   420,   421,   422,   423,   424,
     425,   426,   427,   428,   429,   430,   431,   432,   433,   434,
     435,   436,   437,   438,   439,   440,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,    40,    41,    47,
      91,    93,    61,    44,   123,   125,   452,    42
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   208,   209,   210,   211,   211,   212,   212,   212,   212,
     212,   212,   212,   212,   212,   212,   212,   212,   212,   212,
     212,   212,   212,   212,   212,   212,   212,   212,   212,   212,
     212,   212,   212,   212,   212,   212,   212,   212,   212,   212,
     212,   212,   212,   212,   212,   212,   212,   212,   212,   212,
     212,   212,   212,   212,   212,   212,   212,   212,   212,   212,
     212,   212,   212,   212,   212,   212,   212,   212,   212,   212,
     212,   212,   212,   212,   212,   212,   212,   212,   212,   212,
     212,   212,   212,   212,   212,   212,   212,   212,   212,   212,
     212,   212,   212,   212,   213,   213,   214,   215,   215,   216,
     216,   217,   218,   219,   219,   220,   220,   221,   221,   221,
     221,   221,   221,   221,   221,   221,   221,   221,   221,   221,
     222,   223,   223,   223,   224,   225,   226,   227,   227,   228,
     228,   228,   229,   229,   230,   230,   232,   231,   233,   234,
     234,   235,   235,   235,   235,   235,   235,   235,   235,   235,
     235,   235,   235,   235,   235,   235,   235,   235,   235,   235,
     235,   235,   235,   235,   235,   236,   236,   237,   237,   238,
     239,   239,   240,   240,   241,   241,   241,   242,   242,   243,
     243,   243,   243,   244,   244,   245,   245,   246,   247,   247,
     247,   247,   248,   248,   249,   249,   250,   250,   250,   251,
     251,   252,   252,   252,   252,   252,   253,   254,   255,   255,
     256,   256,   256,   256,   256,   256,   257,   257,   258,   259,
     260,   261,   261,   262,   262,   262,   262,   262,   262,   262,
     262,   262,   262,   262,   262,   262,   262,   263,   263,   264,
     265,   266,   266,   267,   268,   268,   269,   269,   270,   271,
     271,   272,   272,   273,   273,   273,   273,   274,   275,   276,
     277,   277,   277,   277,   277,   277,   277,   277,   277,   277,
     277,   277,   277,   278,   278,   279,   280,   281,   282,   283,
     284,   285,   286,   286,   286,   287,   288,   289,   289,   290,
     290,   291,   291,   291,   291,   291,   292,   292,   293,   293,
     293,   293,   293,   293,   293,   293,   293,   293,   293,   293,
     293,   293,   293,   293,   293,   293,   294,   294,   295,   295,
     296,   297,   298,   298,   299,   299,   300,   301,   301,   302,
     303,   303,   304,   304,   305,   305,   305,   306,   307,   307,
     308,   308,   309,   310,   310,   311,   311,   312,   312,   313,
     313,   314,   315,   316,   316,   316,   317,   318,   319,   319,
     320,   321,   322
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     9,     0,     0,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     1,     0,     1,     2,     1,     1,     0,
       2,     3,     1,     2,     4,     0,     2,     3,     2,     3,
       2,     2,     2,     2,     3,     3,     2,     2,     2,     2,
       1,     0,     1,     1,     3,     0,     4,     1,     1,     0,
       4,     4,     0,     2,     0,     2,     0,     6,     0,     0,
       3,     3,     2,     2,     1,     2,     2,     2,     2,     2,
       1,     2,     2,     1,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     1,     2,     0,     3,     0,     1,     5,
       0,     1,     0,     1,     1,     3,     3,     0,     2,     1,
       2,     1,     2,     0,     3,     1,     3,     1,     0,     3,
       3,     3,     1,     3,     0,     1,     3,     3,     3,     0,
       2,     1,     1,     1,     1,     1,     5,     0,     1,     2,
       1,     1,     1,     1,     1,     1,     1,     2,     1,     4,
       1,     0,     2,     1,     1,     2,     3,     2,     3,     2,
       2,     2,     2,     3,     3,     2,     2,     1,     3,     1,
       5,     0,     2,     0,     1,     1,     1,     3,     2,     0,
       1,     0,     2,     5,     5,     2,     1,     0,     0,     0,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       3,     2,     2,     0,     1,     5,     0,     1,     4,     0,
       2,     1,     4,     5,     4,     0,    10,     3,     1,     1,
       2,     1,     1,     1,     1,     1,     0,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     1,     0,     2,     1,     2,
       1,     1,     0,     3,     0,     3,     0,     0,     3,     0,
       0,     4,     1,     1,     0,     3,     3,     0,     0,     2,
       1,     2,     1,     0,     2,     3,     4,     0,     2,     1,
       3,     0,     0,     6,     5,     4,     0,     0,     1,     2,
       1,     4,     0
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       4,     0,     0,     1,     0,     0,     0,    94,     0,     0,
      94,    94,     0,    94,    94,    94,    94,    94,    94,     0,
       0,     0,     0,     0,    94,    94,    94,     0,     0,    94,
      94,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,    94,
      94,     0,     0,     0,     0,     0,     0,    94,    94,    94,
      94,    94,     0,    94,     0,     0,     0,     0,     0,    93,
      94,    94,    94,    94,    94,    94,    94,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
      94,    94,    94,     5,    99,    98,    97,    96,     6,     7,
     120,     0,     0,    10,   174,    95,     8,     9,    11,    12,
      13,    92,    15,    14,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    91,    26,    27,    28,    29,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    57,    30,
      31,    33,    32,    34,    35,    55,    56,    58,    59,    60,
      71,    72,    73,    74,    88,    89,    87,    70,    61,    62,
      63,    64,    66,    67,    75,    76,    77,    78,    79,    80,
      81,    85,    82,    83,    84,    86,    90,    68,    69,    65,
       0,     0,     0,     0,     0,     0,   100,     3,   176,   175,
     102,     0,   103,     0,   105,     0,   101,     0,   104,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   106,   199,   125,     0,   110,   121,   108,
     113,   112,   111,   121,   121,   116,   118,   117,   129,   119,
     251,   134,   109,   122,   123,   107,   114,   115,   132,   249,
     207,     0,   243,     0,   200,   201,   202,   203,   204,     2,
     205,   124,     0,     0,     0,   126,   250,   248,     0,   220,
     221,     0,   277,   276,   362,   257,     0,   252,   256,   138,
     135,   133,   127,   128,     0,     0,   210,   211,   215,   212,
     213,   214,     0,   208,     0,   245,   246,     0,   244,   322,
       0,   258,   281,   255,     0,   136,   131,   130,     0,   209,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   224,     0,   223,   239,   222,   219,   237,   241,     0,
       0,     0,   360,     0,   358,   296,   296,   296,   296,   296,
     296,   296,   296,   296,   296,   296,   273,   296,   259,   322,
     285,     0,   285,   280,   139,   218,   206,   216,     0,   229,
     121,   227,   232,   231,   230,   121,   121,   235,   236,   225,
     195,     0,     0,   240,   247,     0,   279,   275,   361,   359,
       0,   264,   288,   271,   260,   261,   262,   263,   265,   266,
     268,   267,   269,   274,   296,   272,     0,   316,     0,   285,
       0,     0,     0,   217,   228,   226,   233,   234,   238,   242,
     323,     0,   291,   294,   295,   292,   293,     0,   289,    94,
      94,    94,    94,    94,    94,    94,    94,    94,   315,    94,
      94,    94,    94,    94,    94,    94,    94,   297,   270,   321,
     254,   296,     0,   253,     0,     0,     0,     0,   170,     0,
     144,   170,     0,   163,   150,   167,   172,   170,   188,   188,
       0,   170,   170,   153,   188,   172,   188,   172,   167,     0,
       0,   165,   177,     0,   287,   290,   298,   299,   300,   301,
     302,   303,   304,   305,   306,   307,   308,   309,   312,   310,
     311,   313,   314,   322,   320,   317,   318,   282,     0,   284,
     140,   143,   171,   157,   183,   158,     0,   155,   168,   160,
     173,   161,     0,     0,     0,   147,   149,   156,   142,   154,
     146,   151,   145,   152,   164,   162,   159,     0,   148,   137,
     278,   316,   319,   283,     0,   141,     0,     0,   194,   192,
     194,   194,     0,     0,     0,   179,   181,   178,   324,   187,
     194,   185,     0,     0,   190,     0,   189,   191,   166,   182,
     180,   326,   327,   184,     0,     0,   197,   198,   196,   193,
     134,   329,   330,   186,   169,   325,   134,   337,   338,   328,
     334,     0,   343,     0,   134,   342,   339,   340,     0,     0,
       0,   332,   333,   331,   341,   356,   351,   352,   357,   357,
     344,   286,   335,   336,     0,     0,   347,     0,   356,     0,
       0,   345,   356,     0,   355,   346,   138,   349,   348,     0,
       0,   136,     0,   354,   350,   353
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,   204,     2,    93,   103,    94,    97,   190,   196,
     279,   197,   207,   223,   104,   245,   224,   241,   225,   284,
     248,   265,   261,   280,   354,   305,   402,   472,   528,   507,
     508,   501,   509,   105,   529,   547,   535,   550,   551,   515,
     538,   555,   539,   240,   254,   255,   268,   292,   293,   356,
     357,   256,   270,   294,   325,   326,   327,   257,   373,   271,
     297,   298,   258,   267,   259,   277,   301,   348,   396,   349,
     394,   260,   299,   273,   377,   411,   303,   304,   353,   398,
     440,   381,   417,   418,   382,   437,   443,   495,   496,   441,
     331,   562,   570,   572,   576,   578,   593,   584,   580,   582,
     586,   587,   588,   600,   611,   618,   605,   606,   601,   604,
     607,   333,   334,   278,   300
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -501
static const yytype_int16 yypact[] =
{
    -501,    89,   367,  -501,    65,    33,    48,   -41,    52,    76,
     -41,   -41,    77,   -41,   -41,   -41,   -41,   -41,   -41,    81,
      85,    91,    97,   104,   -41,   -41,   -41,   117,   134,   -41,
     -41,   139,   152,   153,   164,   165,   166,   167,   168,   170,
     171,   173,   174,   175,   177,   178,   180,   182,   -41,   -41,
     -41,   183,   184,   185,   188,   189,   192,   -41,   -41,   -41,
     -41,   -41,   193,   -41,   195,   200,   202,   203,   204,  -501,
     -41,   -41,   -41,   -41,   -41,   -41,   -41,   208,   209,   210,
     212,   215,   216,   217,   218,   220,   222,   223,   224,   -41,
     -41,   -41,   -41,  -501,  -501,  -501,  -501,  -501,  -501,  -501,
    -501,    25,    25,  -501,  -501,  -501,  -501,  -501,  -501,  -501,
    -501,  -501,  -501,  -501,  -501,  -501,  -501,  -501,  -501,  -501,
    -501,  -501,  -501,  -501,  -501,  -501,  -501,  -501,  -501,  -501,
    -501,  -501,  -501,  -501,  -501,  -501,  -501,  -501,  -501,  -501,
    -501,  -501,  -501,  -501,  -501,  -501,  -501,  -501,  -501,  -501,
    -501,  -501,  -501,  -501,  -501,  -501,  -501,  -501,  -501,  -501,
    -501,  -501,  -501,  -501,  -501,  -501,  -501,  -501,  -501,  -501,
    -501,  -501,  -501,  -501,  -501,  -501,  -501,  -501,  -501,  -501,
    -501,  -501,  -501,  -501,  -501,  -501,  -501,  -501,  -501,  -501,
      -4,   -55,   -39,   275,   275,   275,  -501,  -501,  -501,  -501,
    -501,   120,  -501,   122,  -501,   275,  -501,   598,  -501,   281,
      25,    25,    25,    25,    25,    25,    25,    25,    25,    25,
      25,   275,   275,  -501,  -501,  -501,    25,  -501,    73,  -501,
    -501,  -501,  -501,    73,    73,  -501,  -501,  -501,  -501,  -501,
      66,  -501,  -501,  -501,  -501,  -501,  -501,  -501,     5,   284,
    -501,   275,  -501,   282,  -501,  -501,  -501,  -501,  -501,    13,
    -501,   275,   231,   -15,   -15,  -501,  -501,  -501,   238,  -501,
    -501,    44,  -501,  -501,  -501,  -501,   283,  -501,  -501,  -501,
    -501,  -501,  -501,  -501,   129,   127,  -501,  -501,  -501,  -501,
    -501,  -501,   191,  -501,   570,  -501,  -501,   275,   123,   291,
     279,   348,  -501,  -501,    10,  -501,  -501,  -501,   275,  -501,
     294,    25,    25,    25,    25,    25,    25,    25,    25,    25,
      25,  -501,   275,  -501,  -501,  -501,   -19,  -501,   -13,   290,
     241,   320,  -501,    70,  -501,   142,   142,   142,   142,   142,
     142,   142,   142,   142,   142,   142,   -41,   142,  -501,   291,
    -501,   325,  -501,  -501,   145,  -501,   275,  -501,    25,  -501,
      73,  -501,  -501,  -501,  -501,    73,    73,  -501,  -501,  -501,
    -501,   275,   333,  -501,  -501,   275,  -501,  -501,  -501,  -501,
     159,  -501,   445,  -501,  -501,  -501,  -501,  -501,  -501,  -501,
    -501,  -501,  -501,  -501,   142,  -501,   302,   244,   279,  -501,
     279,   275,   292,  -501,  -501,  -501,  -501,  -501,  -501,  -501,
     275,   279,  -501,  -501,  -501,  -501,  -501,   -22,  -501,   -41,
     -41,   -41,   -41,   -41,   -41,   -41,   -41,   -41,  -501,   -41,
     -41,   -41,   -41,   -41,   -41,   -41,   -41,  -501,  -501,  -501,
    -501,   142,   275,  -501,     2,   279,    26,   149,   -41,   -41,
    -501,   -41,   -41,  -501,  -501,   151,   -41,   -41,     7,     7,
     151,   -41,   -41,  -501,     7,   -41,     7,   -41,   151,   -41,
     -41,   155,  -501,    51,  -501,  -501,  -501,  -501,  -501,  -501,
    -501,  -501,  -501,  -501,  -501,  -501,  -501,  -501,  -501,  -501,
    -501,  -501,  -501,   291,  -501,   275,  -501,  -501,    57,  -501,
    -501,  -501,  -501,  -501,   146,  -501,   -41,  -501,  -501,  -501,
    -501,  -501,   275,   275,   275,  -501,  -501,  -501,  -501,  -501,
    -501,  -501,  -501,  -501,  -501,  -501,  -501,   275,  -501,     4,
    -501,   244,  -501,  -501,   275,  -501,   156,   147,   -74,  -501,
      12,    15,   157,   262,   293,  -501,  -501,  -501,   313,  -501,
      27,  -501,   -41,     1,  -501,   275,  -501,  -501,  -501,  -501,
    -501,  -501,   316,  -501,   275,   169,  -501,  -501,  -501,  -501,
    -501,  -501,   295,  -501,  -501,   275,  -501,  -501,   259,   275,
     172,   275,  -501,    -1,   163,  -501,   275,  -501,    84,   176,
     186,  -501,   275,  -501,  -501,  -501,  -501,  -501,  -501,  -501,
    -501,  -501,  -501,  -501,   279,   279,  -501,   351,  -501,    64,
      40,   275,  -501,   279,  -501,  -501,  -501,  -501,  -501,   279,
      68,   342,    69,  -501,  -501,  -501
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -501,  -501,  -501,  -501,  -501,   -10,  -501,  -501,  -501,  -501,
    -137,  -501,  -501,  -501,   -65,  -202,  -501,  -501,  -501,   116,
    -501,  -501,  -404,  -229,  -501,  -233,  -501,  -501,  -501,   -83,
     -71,  -245,  -225,  -270,  -501,  -501,  -501,  -501,  -178,  -256,
    -403,  -317,  -164,  -501,  -501,  -501,  -501,  -501,   100,    18,
    -322,  -501,  -501,  -501,  -501,   103,    30,  -501,  -501,  -501,
    -501,  -501,  -501,  -501,  -501,  -501,  -501,  -501,  -501,  -501,
    -501,  -501,  -501,  -501,  -501,  -501,  -501,  -501,  -501,  -326,
    -501,  -206,  -501,   -14,  -501,  -501,  -129,  -501,   -91,  -501,
    -328,  -501,  -501,  -501,  -501,  -501,  -501,  -501,  -501,  -501,
    -501,  -181,  -501,  -501,  -501,  -501,  -501,  -501,  -501,  -500,
    -193,  -376,  -331,  -501,  -501
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -195
static const yytype_int16 yytable[] =
{
     108,   109,   379,   111,   112,   113,   114,   115,   116,   371,
     372,   371,   193,   566,   122,   123,   124,   543,   262,   127,
     128,   397,   444,  -194,   446,   350,   400,   200,   497,  -194,
     100,   246,   247,   274,   403,   473,   191,   192,   146,   147,
     148,   412,   413,   414,   415,   295,   416,   155,   156,   157,
     158,   159,   499,   161,   332,   282,   201,   202,   203,   589,
     167,   168,   169,   170,   171,   172,   173,   615,   208,   498,
     249,   544,   100,   445,   545,   194,   393,   530,   332,   186,
     187,   188,   189,   533,   238,   239,   296,   590,   403,     3,
     614,   275,   332,   567,   623,   625,   100,    95,   378,   595,
     351,   352,   596,   332,   546,   250,   597,    96,   613,   332,
     540,   541,   619,   379,   269,   379,   332,   251,   276,   252,
     332,   332,   332,   243,   554,    98,   283,   283,   244,   370,
     383,   384,   385,   386,   387,   388,   389,   390,   391,   392,
      99,   395,   379,   198,   106,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   101,   324,   405,   102,
     324,   242,   199,   406,   407,   531,   575,   379,   107,   110,
     253,   355,   579,   117,   598,   599,   474,   118,   502,   503,
     592,   502,   505,   119,   370,   369,   510,   502,   438,   120,
     370,   502,   502,   195,   286,   510,   121,   510,   101,   525,
     526,   102,   263,   516,   512,   264,   504,   513,   520,   125,
     522,   514,   511,   556,   287,   370,   518,   519,   370,   355,
     557,   288,   412,   413,   414,   415,   126,   416,   609,   610,
     370,   129,   563,   564,   324,   493,   536,   620,   355,   289,
     521,   286,   523,   622,   130,   131,   359,   360,   361,   362,
     363,   364,   365,   366,   367,   368,   132,   133,   134,   135,
     136,   287,   137,   138,   447,   139,   140,   141,   288,   142,
     143,   290,   144,   355,   145,   149,   150,   151,   379,   379,
     152,   153,   565,   568,   154,   160,   289,   162,   308,   379,
     291,   379,   163,   404,   164,   165,   166,   448,   449,   450,
     174,   175,   176,   451,   177,   494,   452,   178,   179,   180,
     181,   453,   182,   454,   183,   184,   185,   200,   290,   205,
     206,   226,   266,   281,   272,   302,   455,   306,   307,   329,
     330,   332,   374,   456,   358,   376,   457,   291,   375,   380,
     399,   458,   401,   409,   439,   442,   459,   500,   506,   553,
     534,   460,   527,   461,   559,   558,   335,   336,   494,   552,
     561,   462,   571,   560,   581,   337,   612,   574,   577,   583,
     591,   624,   338,   339,   602,   537,   537,   537,   463,   340,
     285,   464,   617,   621,   603,   524,   573,   465,   466,   517,
     542,   569,   309,   410,   467,   468,   341,   549,   469,   470,
     328,   408,   548,   475,   532,   594,   608,     0,     0,   476,
     477,   478,   479,   480,   481,   482,   483,   484,   537,   485,
     486,   487,   488,   489,   490,   491,   492,   549,     0,   342,
     343,   344,   345,   346,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   585,     0,     0,   347,     0,   585,
       0,     0,     0,     0,     4,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   616,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,   471,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,   419,   420,   421,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   422,   423,   424,
       0,     0,     0,     0,     0,   310,   311,     0,   425,   426,
     427,     0,   200,   312,     0,   313,     0,     0,   428,     0,
     429,   430,   431,   432,   433,   434,   314,   315,   316,     0,
       0,     0,   317,   209,   210,     0,     0,     0,     0,   435,
     436,   211,   318,   212,   319,   320,     0,   321,     0,     0,
       0,     0,     0,     0,   213,   214,   215,     0,     0,     0,
     216,     0,     0,     0,   322,     0,     0,     0,   323,     0,
     217,     0,   218,   219,   220,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   221,   222
};

static const yytype_int16 yycheck[] =
{
      10,    11,   333,    13,    14,    15,    16,    17,    18,   326,
      23,   328,    16,    12,    24,    25,    26,    13,    13,    29,
      30,   349,   398,    42,   400,    15,   352,    42,    26,    42,
      71,   233,   234,    20,   356,   411,   101,   102,    48,    49,
      50,    63,    64,    65,    66,     1,    68,    57,    58,    59,
      60,    61,    26,    63,    52,    70,   193,   194,   195,    60,
      70,    71,    72,    73,    74,    75,    76,    27,   205,   445,
       4,    67,    71,   399,    70,    79,   346,    26,    52,    89,
      90,    91,    92,    26,   221,   222,    42,    88,   410,     0,
      26,    78,    52,    92,    26,    26,    71,    32,    28,    15,
      90,    91,    18,    52,   100,    39,    22,    42,   608,    52,
     513,   514,   612,   444,   251,   446,    52,    51,   105,    53,
      52,    52,    52,    50,   198,    92,   263,   264,    55,   203,
     336,   337,   338,   339,   340,   341,   342,   343,   344,   345,
      92,   347,   473,   198,    92,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   197,   294,   360,   200,
     297,   226,   201,   365,   366,   493,   570,   498,    92,    92,
     104,   308,   576,    92,    90,    91,   198,    92,   448,   449,
     584,   451,   452,    92,   203,   322,   456,   457,   394,    92,
     203,   461,   462,   197,     3,   465,    92,   467,   197,   469,
     470,   200,   197,   459,   197,   200,   451,   200,   464,    92,
     466,   204,   457,   201,    23,   203,   461,   462,   203,   356,
     205,    30,    63,    64,    65,    66,    92,    68,   604,   605,
     203,    92,   205,   550,   371,   441,   506,   613,   375,    48,
     465,     3,   467,   619,    92,    92,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,    92,    92,    92,    92,
      92,    23,    92,    92,   401,    92,    92,    92,    30,    92,
      92,    80,    92,   410,    92,    92,    92,    92,   609,   610,
      92,    92,   552,   553,    92,    92,    48,    92,    97,   620,
      99,   622,    92,   358,    92,    92,    92,     5,     6,     7,
      92,    92,    92,    11,    92,   442,    14,    92,    92,    92,
      92,    19,    92,    21,    92,    92,    92,    42,    80,   199,
     198,    40,    38,    92,    42,    42,    34,   198,   201,   206,
      39,    52,    42,    41,    40,    15,    44,    99,    97,   197,
      15,    49,   197,    10,    42,   101,    54,   198,   197,   202,
     204,    59,   197,    61,    92,   198,     8,     9,   495,   203,
      47,    69,    46,    70,   105,    17,    15,   198,    73,   197,
     207,    29,    24,    25,   198,   512,   513,   514,    86,    31,
     264,    89,   611,   616,   198,   468,   564,    95,    96,   460,
     527,   555,   292,   375,   102,   103,    48,   534,   106,   107,
     297,   371,   531,   417,   495,   586,   599,    -1,    -1,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   555,   429,
     430,   431,   432,   433,   434,   435,   436,   564,    -1,    81,
      82,    83,    84,    85,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   581,    -1,    -1,    99,    -1,   586,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   611,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   202,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   119,   120,   121,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,   153,   154,
      -1,    -1,    -1,    -1,    -1,    35,    36,    -1,   163,   164,
     165,    -1,    42,    43,    -1,    45,    -1,    -1,   173,    -1,
     175,   176,   177,   178,   179,   180,    56,    57,    58,    -1,
      -1,    -1,    62,    35,    36,    -1,    -1,    -1,    -1,   194,
     195,    43,    72,    45,    74,    75,    -1,    77,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    57,    58,    -1,    -1,    -1,
      62,    -1,    -1,    -1,    94,    -1,    -1,    -1,    98,    -1,
      72,    -1,    74,    75,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    93,    94
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   209,   211,     0,    87,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   212,   214,    32,    42,   215,    92,    92,
      71,   197,   200,   213,   222,   241,    92,    92,   213,   213,
      92,   213,   213,   213,   213,   213,   213,    92,    92,    92,
      92,    92,   213,   213,   213,    92,    92,   213,   213,    92,
      92,    92,    92,    92,    92,    92,    92,    92,    92,    92,
      92,    92,    92,    92,    92,    92,   213,   213,   213,    92,
      92,    92,    92,    92,    92,   213,   213,   213,   213,   213,
      92,   213,    92,    92,    92,    92,    92,   213,   213,   213,
     213,   213,   213,   213,    92,    92,    92,    92,    92,    92,
      92,    92,    92,    92,    92,    92,   213,   213,   213,   213,
     216,   222,   222,    16,    79,   197,   217,   219,   198,   201,
      42,   218,   218,   218,   210,   199,   198,   220,   218,    35,
      36,    43,    45,    56,    57,    58,    62,    72,    74,    75,
      76,    93,    94,   221,   224,   226,    40,   222,   222,   222,
     222,   222,   222,   222,   222,   222,   222,   222,   218,   218,
     251,   225,   222,    50,    55,   223,   223,   223,   228,     4,
      39,    51,    53,   104,   252,   253,   259,   265,   270,   272,
     279,   230,    13,   197,   200,   229,    38,   271,   254,   218,
     260,   267,    42,   281,    20,    78,   105,   273,   321,   218,
     231,    92,    70,   218,   227,   227,     3,    23,    30,    48,
      80,    99,   255,   256,   261,     1,    42,   268,   269,   280,
     322,   274,    42,   284,   285,   233,   198,   201,    97,   256,
      35,    36,    43,    45,    56,    57,    58,    62,    72,    74,
      75,    77,    94,    98,   218,   262,   263,   264,   263,   206,
      39,   298,    52,   319,   320,     8,     9,    17,    24,    25,
      31,    48,    81,    82,    83,    84,    85,    99,   275,   277,
      15,    90,    91,   286,   232,   218,   257,   258,    40,   222,
     222,   222,   222,   222,   222,   222,   222,   222,   222,   218,
     203,   249,    23,   266,    42,    97,    15,   282,    28,   320,
     197,   289,   292,   289,   289,   289,   289,   289,   289,   289,
     289,   289,   289,   241,   278,   289,   276,   298,   287,    15,
     287,   197,   234,   258,   222,   223,   223,   223,   264,    10,
     257,   283,    63,    64,    65,    66,    68,   290,   291,   119,
     120,   121,   152,   153,   154,   163,   164,   165,   173,   175,
     176,   177,   178,   179,   180,   194,   195,   293,   289,    42,
     288,   297,   101,   294,   319,   287,   319,   218,     5,     6,
       7,    11,    14,    19,    21,    34,    41,    44,    49,    54,
      59,    61,    69,    86,    89,    95,    96,   102,   103,   106,
     107,   202,   235,   319,   198,   291,   213,   213,   213,   213,
     213,   213,   213,   213,   213,   213,   213,   213,   213,   213,
     213,   213,   213,   289,   218,   295,   296,    26,   319,    26,
     198,   239,   241,   241,   239,   241,   197,   237,   238,   240,
     241,   239,   197,   200,   204,   247,   247,   238,   239,   239,
     247,   240,   247,   240,   237,   241,   241,   197,   236,   242,
      26,   298,   296,    26,   204,   244,   241,   218,   248,   250,
     248,   248,   218,    13,    67,    70,   100,   243,   294,   218,
     245,   246,   203,   202,   198,   249,   201,   205,   198,    92,
      70,    47,   299,   205,   249,   241,    12,    92,   241,   250,
     300,    46,   301,   246,   198,   230,   302,    73,   303,   230,
     306,   105,   307,   197,   305,   218,   308,   309,   310,    60,
      88,   207,   230,   304,   309,    15,    18,    22,    90,    91,
     311,   316,   198,   198,   317,   314,   315,   318,   318,   319,
     319,   312,    15,   317,    26,    27,   218,   231,   313,   317,
     319,   233,   319,    26,    29,    26
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 3:

/* Line 1455 of yacc.c  */
#line 357 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
    Table->Count.Name = strdup("NoOfRecs");
    Table->Count.Type = ftypeInt;
    Table->Count.Length = 4;
    Table->Count.Padding = Table->EightByte == 1 ? 8 : 4;
    Table->ABSize = 12;
    Table->Space = 0;
    Table->Free = 0;
    Table->Used = 0;
    Table->Increase = 0;
    Table->IniTrans = 0;
    Table->MaxTrans = 0;
    Table->Initial = 0;
    Table->StorageNext = 0;
    Table->MinExtents = 0;
    Table->MaxExtents = 0;
    Table->Optimal = 0;
    Table->FreeLists = 0;
    Table->FreeListGroups = 0;
    Table->Alias = 0;
    Table->isNullEnabled = 0;
;}
    break;

  case 6:

/* Line 1455 of yacc.c  */
#line 386 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->SODir           , (yyvsp[(2) - (2)].s)); ;}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 387 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->SOExt           , (yyvsp[(2) - (2)].s)); ;}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 388 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->CDir            , (yyvsp[(2) - (2)].s)); ;}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 389 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->CExt            , (yyvsp[(2) - (2)].s)); ;}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 390 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->BlankPadded     , (yyvsp[(2) - (2)].i)); ;}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 391 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->CharZ           , (yyvsp[(2) - (2)].i)); ;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 392 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->ConnReqd        , (yyvsp[(2) - (2)].i)); ;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 393 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->ControlDB       , (yyvsp[(2) - (2)].s)); ;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 394 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->EightByte       , (yyvsp[(2) - (2)].i)); ;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 395 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->ExitReqd        , (yyvsp[(2) - (2)].i)); ;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 396 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->ExtendedC       , (yyvsp[(2) - (2)].i)); ;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 397 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->ExtendedPAS     , (yyvsp[(2) - (2)].i)); ;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 398 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->ExtendedVB      , (yyvsp[(2) - (2)].i)); ;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 399 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->CDir            , (yyvsp[(2) - (2)].s)); ;}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 400 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->CExt            , (yyvsp[(2) - (2)].s)); ;}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 401 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->IDLDir          , (yyvsp[(2) - (2)].s)); ;}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 402 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->IDLExt          , (yyvsp[(2) - (2)].s)); ;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 403 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->IDLModule       , (yyvsp[(2) - (2)].s)); ;}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 404 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->Internal        , (yyvsp[(2) - (2)].i)); ;}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 405 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->Version2Bin     , (yyvsp[(2) - (2)].i)); ;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 406 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->LogDir          , (yyvsp[(2) - (2)].s)); ;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 407 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->LogExt          , (yyvsp[(2) - (2)].s)); ;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 408 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->NoDomain        , (yyvsp[(2) - (2)].i)); ;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 409 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->OneSQLScript    , (yyvsp[(2) - (2)].i)); ;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 410 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->ArchiveDir      , (yyvsp[(2) - (2)].s)); ;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 411 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->ArchiveExt      , (yyvsp[(2) - (2)].s)); ;}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 412 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->PARMSDescr      , (yyvsp[(2) - (2)].s)); ;}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 413 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->PARMSDir        , (yyvsp[(2) - (2)].s)); ;}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 414 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->PARMSExt        , (yyvsp[(2) - (2)].s)); ;}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 415 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->PARMSLookup     , (yyvsp[(2) - (2)].s)); ;}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 416 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->PASDir          , (yyvsp[(2) - (2)].s)); ;}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 417 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->PASExt          , (yyvsp[(2) - (2)].s)); ;}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 418 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->PythonDir       , (yyvsp[(2) - (2)].s)); ;}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 419 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->PythonExt       , (yyvsp[(2) - (2)].s)); ;}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 420 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->Show            , (yyvsp[(2) - (2)].s)); ;}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 421 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->SODir           , (yyvsp[(2) - (2)].s)); ;}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 422 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->SOExt           , (yyvsp[(2) - (2)].s)); ;}
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 423 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->SqlAuditExt     , (yyvsp[(2) - (2)].s)); ;}
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 424 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->SqlConExt       , (yyvsp[(2) - (2)].s)); ;}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 425 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->SqlDir          , (yyvsp[(2) - (2)].s)); ;}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 426 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->SqlExt          , (yyvsp[(2) - (2)].s)); ;}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 427 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->SqlGrantExt     , (yyvsp[(2) - (2)].s)); ;}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 428 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->SqlIdxExt       , (yyvsp[(2) - (2)].s)); ;}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 429 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->SqlProcExt      , (yyvsp[(2) - (2)].s)); ;}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 430 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->SqlSnapExt      , (yyvsp[(2) - (2)].s)); ;}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 431 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->SqlTableExt     , (yyvsp[(2) - (2)].s)); ;}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 432 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->SqlViewsExt     , (yyvsp[(2) - (2)].s)); ;}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 433 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->TargetC         , (yyvsp[(2) - (2)].i)); ;}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 434 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->TargetIDL       , (yyvsp[(2) - (2)].i)); ;}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 435 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->TargetARCHIVE   , (yyvsp[(2) - (2)].i)); ;}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 436 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->TargetPARMS     , (yyvsp[(2) - (2)].i)); ;}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 437 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->TargetPAS       , (yyvsp[(2) - (2)].i)); ;}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 438 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->TargetPython    , (yyvsp[(2) - (2)].i)); ;}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 439 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->TargetVB        , (yyvsp[(2) - (2)].i)); ;}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 440 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->TargetVB5       , (yyvsp[(2) - (2)].i)); ;}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 441 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->TargetVBCode3   , (yyvsp[(2) - (2)].i)); ;}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 442 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->TargetVBNet7    , (yyvsp[(2) - (2)].i)); ;}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 443 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->TargetCSNet7    , (yyvsp[(2) - (2)].i)); ;}
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 444 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->TargetCSIDL2    , (yyvsp[(2) - (2)].i)); ;}
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 445 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->LittleTCSIDL2   , (yyvsp[(2) - (2)].i)); ;}
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 446 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->TargetCSAdoNet  , (yyvsp[(2) - (2)].i)); ;}
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 447 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->TargetCSRW      , (yyvsp[(2) - (2)].i)); ;}
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 448 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->TargetVBforIDL  , (yyvsp[(2) - (2)].i)); ;}
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 449 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->TargetVBforADOR , (yyvsp[(2) - (2)].i)); ;}
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 450 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->PrefixVBClasses , (yyvsp[(2) - (2)].i)); ;}
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 451 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->UConnect        , (yyvsp[(2) - (2)].s)); ;}
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 452 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->UnderScore      , (yyvsp[(2) - (2)].i)); ;}
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 453 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->VB5Dir          , (yyvsp[(2) - (2)].s)); ;}
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 454 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->VB5Ext          , (yyvsp[(2) - (2)].s)); ;}
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 455 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->VBCode3Dir      , (yyvsp[(2) - (2)].s)); ;}
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 456 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->VBCode3Ext      , (yyvsp[(2) - (2)].s)); ;}
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 457 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->VBNet7Dir       , (yyvsp[(2) - (2)].s)); ;}
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 458 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->VBNet7Ext       , (yyvsp[(2) - (2)].s)); ;}
    break;

  case 79:

/* Line 1455 of yacc.c  */
#line 459 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->CSNet7Dir       , (yyvsp[(2) - (2)].s)); ;}
    break;

  case 80:

/* Line 1455 of yacc.c  */
#line 460 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->CSAdoNetDir     , (yyvsp[(2) - (2)].s)); ;}
    break;

  case 81:

/* Line 1455 of yacc.c  */
#line 461 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->CSRWDir         , (yyvsp[(2) - (2)].s)); ;}
    break;

  case 82:

/* Line 1455 of yacc.c  */
#line 462 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->CSNet7Ext       , (yyvsp[(2) - (2)].s)); ;}
    break;

  case 83:

/* Line 1455 of yacc.c  */
#line 463 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->CSAdoNetExt     , (yyvsp[(2) - (2)].s)); ;}
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 464 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->CSRWExt         , (yyvsp[(2) - (2)].s)); ;}
    break;

  case 85:

/* Line 1455 of yacc.c  */
#line 465 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->CSIDL2Dir       , (yyvsp[(2) - (2)].s)); ;}
    break;

  case 86:

/* Line 1455 of yacc.c  */
#line 466 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->CSIDL2Ext       , (yyvsp[(2) - (2)].s)); ;}
    break;

  case 87:

/* Line 1455 of yacc.c  */
#line 467 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->VBforIDLDir     , (yyvsp[(2) - (2)].s)); ;}
    break;

  case 88:

/* Line 1455 of yacc.c  */
#line 468 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->VBDir           , (yyvsp[(2) - (2)].s)); ;}
    break;

  case 89:

/* Line 1455 of yacc.c  */
#line 469 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->VBExt           , (yyvsp[(2) - (2)].s)); ;}
    break;

  case 90:

/* Line 1455 of yacc.c  */
#line 470 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->ViewOnly        , (yyvsp[(2) - (2)].i)); ;}
    break;

  case 91:

/* Line 1455 of yacc.c  */
#line 471 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->LittleTrue      , (yyvsp[(2) - (2)].i)); ;}
    break;

  case 92:

/* Line 1455 of yacc.c  */
#line 472 "C:/vlab/utility/dbportal/code/sqldef.y"
    { SetSwitch(Table->CppHeader       , (yyvsp[(2) - (2)].i)); ;}
    break;

  case 93:

/* Line 1455 of yacc.c  */
#line 474 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  yyreport(CODING_ERROR, "%s is not a valid switch\n", (yyvsp[(1) - (1)].s));
  free((yyvsp[(1) - (1)].s));
;}
    break;

  case 94:

/* Line 1455 of yacc.c  */
#line 482 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
    (yyval.i) = 1;
;}
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 486 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  (yyval.i) = (yyvsp[(1) - (1)].i);
;}
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 493 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  Table->Server = (yyvsp[(2) - (2)].s);
;}
    break;

  case 97:

/* Line 1455 of yacc.c  */
#line 499 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  (yyval.s) = (yyvsp[(1) - (1)].s);
;}
    break;

  case 98:

/* Line 1455 of yacc.c  */
#line 503 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  (yyval.s) = (yyvsp[(1) - (1)].s);
;}
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 524 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
    SetFlag((yyvsp[(2) - (3)].s));
;}
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 530 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  (yyval.s) = (yyvsp[(1) - (1)].s);
;}
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 536 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  Table->UserID = (yyvsp[(2) - (2)].s);
  Table->PassWD = strdup((yyvsp[(2) - (2)].s));
;}
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 541 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  Table->UserID = (yyvsp[(2) - (4)].s);
  Table->PassWD = (yyvsp[(4) - (4)].s);
;}
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 553 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  Table->Initial = (int32)(yyvsp[(2) - (3)].i);
  Table->InitialSize = NumberSize;
;}
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 558 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  Table->IniTrans = (int32)(yyvsp[(2) - (2)].i);
;}
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 562 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  Table->FreeListGroups = (int32)(yyvsp[(3) - (3)].i);
;}
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 566 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  Table->FreeLists = (int32)(yyvsp[(2) - (2)].i);
;}
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 570 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  Table->MinExtents = (int32)(yyvsp[(2) - (2)].i);
;}
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 574 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  Table->MaxTrans = (int32)(yyvsp[(2) - (2)].i);
;}
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 578 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  Table->MaxExtents = (int32)(yyvsp[(2) - (2)].i);
;}
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 582 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  Table->StorageNext = (int32)(yyvsp[(2) - (3)].i);
  Table->NextSize = NumberSize;
;}
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 587 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  Table->Optimal = (int32)(yyvsp[(2) - (3)].i);
  Table->OptimalSize = NumberSize;
;}
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 592 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  Table->Free = (int32)(yyvsp[(2) - (2)].i);
;}
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 596 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  Table->Used = (int32)(yyvsp[(2) - (2)].i);
;}
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 600 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  Table->Increase = (int32)(yyvsp[(2) - (2)].i);
;}
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 604 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  Table->Space = (yyvsp[(2) - (2)].s);
;}
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 610 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  (yyval.i) = (yyvsp[(1) - (1)].i);
;}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 616 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
    NumberSize = ' ';
;}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 620 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  NumberSize = 'K';
;}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 624 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  NumberSize = 'M';
;}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 633 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
    FieldState = stateTable;
;}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 639 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  Table->Name = (yyvsp[(2) - (4)].s);
  Table->Token = Hash(Table->Name, 13);
  Table->ABSize += (strlen(Table->Name) + 8);
;}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 647 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  Table->isNullEnabled = 1;
;}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 651 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  Table->Alias = (yyvsp[(1) - (1)].s);
;}
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 664 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
    Table->Check = 0;
;}
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 668 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  Table->Check = (yyvsp[(2) - (2)].s);
;}
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 678 "C:/vlab/utility/dbportal/code/sqldef.y"
    { Field.Name = (yyvsp[(1) - (2)].s); ;}
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 679 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  AddField((yyvsp[(1) - (6)].s));
;}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 684 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
    Field.Clear();
;}
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 690 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
    Field.Alias = 0;
;}
    break;

  case 140:

/* Line 1455 of yacc.c  */
#line 694 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
    Field.Alias = (yyvsp[(2) - (3)].s);
;}
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 700 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
    SetFieldType(ftypeChar, (yyvsp[(2) - (3)].i));
;}
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 704 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
    SetFieldType(ftypeNChar, (yyvsp[(2) - (2)].i));
;}
    break;

  case 143:

/* Line 1455 of yacc.c  */
#line 708 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
    SetFieldType(ftypeBinary, (yyvsp[(2) - (2)].i));
;}
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 712 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
    SetFieldType(ftypeBoolean, 0);
;}
    break;

  case 145:

/* Line 1455 of yacc.c  */
#line 716 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
    SetFieldType(ftypeTinyint, 0);
;}
    break;

  case 146:

/* Line 1455 of yacc.c  */
#line 720 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
    SetFieldType(ftypeSmallint, 0);
;}
    break;

  case 147:

/* Line 1455 of yacc.c  */
#line 724 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
    SetFieldType(ftypeInt, 0);
;}
    break;

  case 149:

/* Line 1455 of yacc.c  */
#line 729 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
    SetFieldType(ftypeLong, 0);
;}
    break;

  case 150:

/* Line 1455 of yacc.c  */
#line 733 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
    SetFieldType(ftypeDate, 0);
;}
    break;

  case 151:

/* Line 1455 of yacc.c  */
#line 737 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
    SetFieldType(ftypeTimeStamp, (yyvsp[(2) - (2)].i));
;}
    break;

  case 152:

/* Line 1455 of yacc.c  */
#line 741 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
    SetFieldType(ftypeUserStamp, (yyvsp[(2) - (2)].i));
;}
    break;

  case 153:

/* Line 1455 of yacc.c  */
#line 745 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
    SetFieldType(ftypeInt, 0);
    if (Table->hasSequence)
        yyreport(CODING_ERROR, "Table already has sequence\n");
    else
    {
        Field.isSequence = 1;
        Table->hasSequence = 1;
    }
;}
    break;

  case 154:

/* Line 1455 of yacc.c  */
#line 756 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
    SetFieldType(ftypeNChar, (yyvsp[(2) - (2)].i));
    if (Table->hasSequence)
        yyreport(CODING_ERROR, "Table already has sequence\n");
    else
    {
        Field.isSequence = 1;
        Table->hasSequence = 1;
    }
;}
    break;

  case 155:

/* Line 1455 of yacc.c  */
#line 767 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  SetFieldType(ftypeFloat, 0);
;}
    break;

  case 156:

/* Line 1455 of yacc.c  */
#line 771 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  SetFieldType(ftypeMoney, 0);
;}
    break;

  case 157:

/* Line 1455 of yacc.c  */
#line 775 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  if (FieldState == stateTable)
    Table->hasLOB = 1;
  else
    Proc.hasLOB = 1;
  SetFieldType(ftypeBLOB, (yyvsp[(2) - (2)].i));
;}
    break;

  case 158:

/* Line 1455 of yacc.c  */
#line 783 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  if (FieldState == stateTable)
    Table->hasLOB = 1;
  else
    Proc.hasLOB = 1;
  SetFieldType(ftypeCLOB, (yyvsp[(2) - (2)].i));
;}
    break;

  case 159:

/* Line 1455 of yacc.c  */
#line 791 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  if (FieldState == stateTable)
    Table->hasLOB = 1;
  else
    Proc.hasLOB = 1;
  SetFieldType(ftypeZLOB, (yyvsp[(2) - (2)].i));
;}
    break;

  case 160:

/* Line 1455 of yacc.c  */
#line 799 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  if (FieldState == stateTable)
    Table->hasHugeCHAR = 1;
  else
    Proc.hasHugeCHAR = 1;
  SetFieldType(ftypeHugeCHAR, (yyvsp[(2) - (2)].i));
;}
    break;

  case 161:

/* Line 1455 of yacc.c  */
#line 807 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  if (FieldState == stateTable)
    Table->hasImage = 1;
  else
    Proc.hasImage = 1;
  SetFieldType(ftypeImage, (yyvsp[(2) - (2)].i));
;}
    break;

  case 162:

/* Line 1455 of yacc.c  */
#line 815 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  if (FieldState == stateTable)
    Table->hasXMLTYPE = 1;
  else
    Proc.hasXMLTYPE = 1;
  SetFieldType(ftypeXMLTYPE, (yyvsp[(2) - (2)].i));
;}
    break;

  case 163:

/* Line 1455 of yacc.c  */
#line 823 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  SetFieldType(ftypeDateTime, 0);
;}
    break;

  case 164:

/* Line 1455 of yacc.c  */
#line 827 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  SetFieldType(ftypeVarNum, 0);
;}
    break;

  case 165:

/* Line 1455 of yacc.c  */
#line 833 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
    SetSameFromTable();
;}
    break;

  case 166:

/* Line 1455 of yacc.c  */
#line 837 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
    SetFromTable((yyvsp[(2) - (3)].s));
;}
    break;

  case 167:

/* Line 1455 of yacc.c  */
#line 844 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
    Field.Precision = 0;
    Field.Scale = 0;
;}
    break;

  case 169:

/* Line 1455 of yacc.c  */
#line 853 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
    Field.Precision = (yyvsp[(2) - (5)].i);
    Field.Scale = (yyvsp[(4) - (5)].i);
;}
    break;

  case 170:

/* Line 1455 of yacc.c  */
#line 861 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
    (yyval.i) = 1;
;}
    break;

  case 171:

/* Line 1455 of yacc.c  */
#line 865 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  (yyval.i) = (yyvsp[(1) - (1)].i);
;}
    break;

  case 172:

/* Line 1455 of yacc.c  */
#line 872 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
    (yyval.i) = 0;
;}
    break;

  case 173:

/* Line 1455 of yacc.c  */
#line 876 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  (yyval.i) = (yyvsp[(1) - (1)].i);
;}
    break;

  case 174:

/* Line 1455 of yacc.c  */
#line 883 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  (yyval.i) = (yyvsp[(1) - (1)].i);
;}
    break;

  case 175:

/* Line 1455 of yacc.c  */
#line 887 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
    (yyval.i) = (yyvsp[(2) - (3)].i);
;}
    break;

  case 176:

/* Line 1455 of yacc.c  */
#line 891 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
    (yyval.i) = (yyvsp[(2) - (3)].i);
;}
    break;

  case 179:

/* Line 1455 of yacc.c  */
#line 902 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  Field.isNull = 1;
;}
    break;

  case 180:

/* Line 1455 of yacc.c  */
#line 906 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  Field.isNull = 0;
;}
    break;

  case 181:

/* Line 1455 of yacc.c  */
#line 910 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  Field.isUpper = 1;
;}
    break;

  case 182:

/* Line 1455 of yacc.c  */
#line 914 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  if (Field.Check) free(Field.Check);
  Field.Check = (yyvsp[(2) - (2)].s);
;}
    break;

  case 187:

/* Line 1455 of yacc.c  */
#line 932 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  AddConstIdent((yyvsp[(1) - (1)].s));
;}
    break;

  case 196:

/* Line 1455 of yacc.c  */
#line 956 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  AddConst((yyvsp[(1) - (3)].s), (yyvsp[(3) - (3)].i));
;}
    break;

  case 197:

/* Line 1455 of yacc.c  */
#line 960 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  AddCharConst((yyvsp[(1) - (3)].s), (yyvsp[(3) - (3)].i));
;}
    break;

  case 198:

/* Line 1455 of yacc.c  */
#line 964 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  AddStringConst((yyvsp[(1) - (3)].s), (yyvsp[(3) - (3)].s));
;}
    break;

  case 206:

/* Line 1455 of yacc.c  */
#line 984 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  AddGrant();
;}
    break;

  case 207:

/* Line 1455 of yacc.c  */
#line 989 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
    GrantState = stateTableGrant;
    memset(&Grant, 0, sizeof(TYYGrant));
;}
    break;

  case 210:

/* Line 1455 of yacc.c  */
#line 1001 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  AddPermission(strdup("all"));
  Table->hasExecute = 1;
  Table->hasSelect = 1;
;}
    break;

  case 211:

/* Line 1455 of yacc.c  */
#line 1007 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  AddPermission(strdup("delete"));
;}
    break;

  case 212:

/* Line 1455 of yacc.c  */
#line 1011 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  AddPermission(strdup("insert"));
;}
    break;

  case 213:

/* Line 1455 of yacc.c  */
#line 1015 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  AddPermission(strdup("select"));
  Table->hasSelect = 1;
;}
    break;

  case 214:

/* Line 1455 of yacc.c  */
#line 1020 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  AddPermission(strdup("update"));
;}
    break;

  case 215:

/* Line 1455 of yacc.c  */
#line 1024 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  Table->hasExecute = 1;
;}
    break;

  case 218:

/* Line 1455 of yacc.c  */
#line 1035 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  AddUser((yyvsp[(1) - (1)].s));
;}
    break;

  case 219:

/* Line 1455 of yacc.c  */
#line 1041 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  AddKey();
;}
    break;

  case 220:

/* Line 1455 of yacc.c  */
#line 1047 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  SetKey((yyvsp[(1) - (1)].s));
  ColumnState = stateKey;
;}
    break;

  case 223:

/* Line 1455 of yacc.c  */
#line 1059 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  Key.isUnique = 1;
;}
    break;

  case 224:

/* Line 1455 of yacc.c  */
#line 1063 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  Key.isPrimary = 1;
  Table->hasPrimaryKey = 1;
;}
    break;

  case 225:

/* Line 1455 of yacc.c  */
#line 1068 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  Key.Space = (yyvsp[(2) - (2)].s);
;}
    break;

  case 226:

/* Line 1455 of yacc.c  */
#line 1072 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  Key.Initial = (int32)(yyvsp[(2) - (3)].i);
  Key.InitialSize = NumberSize;
;}
    break;

  case 227:

/* Line 1455 of yacc.c  */
#line 1077 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  Key.IniTrans = (int32)(yyvsp[(2) - (2)].i);
;}
    break;

  case 228:

/* Line 1455 of yacc.c  */
#line 1081 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  Key.FreeListGroups = (int32)(yyvsp[(3) - (3)].i);
;}
    break;

  case 229:

/* Line 1455 of yacc.c  */
#line 1085 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  Key.FreeLists = (int32)(yyvsp[(2) - (2)].i);
;}
    break;

  case 230:

/* Line 1455 of yacc.c  */
#line 1089 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  Key.MinExtents = (int32)(yyvsp[(2) - (2)].i);
;}
    break;

  case 231:

/* Line 1455 of yacc.c  */
#line 1093 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  Key.MaxTrans = (int32)(yyvsp[(2) - (2)].i);
;}
    break;

  case 232:

/* Line 1455 of yacc.c  */
#line 1097 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  Key.MaxExtents = (int32)(yyvsp[(2) - (2)].i);
;}
    break;

  case 233:

/* Line 1455 of yacc.c  */
#line 1101 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  Key.StorageNext = (int32)(yyvsp[(2) - (3)].i);
  Key.NextSize = NumberSize;
;}
    break;

  case 234:

/* Line 1455 of yacc.c  */
#line 1106 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  Key.Optimal = (int32)(yyvsp[(2) - (3)].i);
  Key.OptimalSize = NumberSize;
;}
    break;

  case 235:

/* Line 1455 of yacc.c  */
#line 1111 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  Key.Free = (int32)(yyvsp[(2) - (2)].i);
;}
    break;

  case 236:

/* Line 1455 of yacc.c  */
#line 1115 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  Key.Increase = (int32)(yyvsp[(2) - (2)].i);
;}
    break;

  case 239:

/* Line 1455 of yacc.c  */
#line 1126 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  AddColumn((yyvsp[(1) - (1)].s));
;}
    break;

  case 240:

/* Line 1455 of yacc.c  */
#line 1132 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  AddLink();
;}
    break;

  case 241:

/* Line 1455 of yacc.c  */
#line 1138 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
    DeleteCascadeLink(false);
;}
    break;

  case 242:

/* Line 1455 of yacc.c  */
#line 1142 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  DeleteCascadeLink(true);
;}
    break;

  case 243:

/* Line 1455 of yacc.c  */
#line 1148 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
    memset(&Link, 0, sizeof(TYYLink));
;}
    break;

  case 244:

/* Line 1455 of yacc.c  */
#line 1154 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  SetLink((yyvsp[(1) - (1)].s));
;}
    break;

  case 245:

/* Line 1455 of yacc.c  */
#line 1158 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  yyreport(CODING_ERROR, "The link table name expected\n");
;}
    break;

  case 246:

/* Line 1455 of yacc.c  */
#line 1164 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  (yyval.s) = (yyvsp[(1) - (1)].s);
;}
    break;

  case 247:

/* Line 1455 of yacc.c  */
#line 1168 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  (yyval.s) = Concat((yyvsp[(1) - (3)].s), (yyvsp[(3) - (3)].s));
;}
    break;

  case 249:

/* Line 1455 of yacc.c  */
#line 1178 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
    Table->hasAudit = 1;
;}
    break;

  case 250:

/* Line 1455 of yacc.c  */
#line 1182 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  Table->hasAudit = 1;
  Table->hasAuditFull = 1;
;}
    break;

  case 257:

/* Line 1455 of yacc.c  */
#line 1200 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
    ExtState = 0x00000000;
;}
    break;

  case 258:

/* Line 1455 of yacc.c  */
#line 1205 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
    GrantState = stateProcGrant;
;}
    break;

  case 259:

/* Line 1455 of yacc.c  */
#line 1210 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
    ProjectState = stateProcProject;
;}
    break;

  case 260:

/* Line 1455 of yacc.c  */
#line 1216 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  if (Table->hasCount == 0)
    CreateCount();
  Table->hasCount = 1;
  Table->extCount = ExtState;
  GrantState = stateCountProcGrant;
  ProjectState = stateCountProcProject;
;}
    break;

  case 261:

/* Line 1455 of yacc.c  */
#line 1225 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  if (Table->hasDeleteAll == 0)
    CreateDeleteAll();
  Table->hasDeleteAll = 1;
  Table->extDeleteAll = ExtState;
  GrantState = stateDeleteAllProcGrant;
  ProjectState = stateDeleteAllProcProject;
;}
    break;

  case 262:

/* Line 1455 of yacc.c  */
#line 1234 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  if (!Table->hasPrimaryKey)
    yyreport(CODING_ERROR, "No primary key for DeleteOne\n");
  if (Table->hasDeleteOne == 0)
     CreateDeleteOne();
  Table->hasDeleteOne = 1;
  Table->extDeleteOne = ExtState;
  GrantState = stateDeleteOneProcGrant;
  ProjectState = stateDeleteOneProcProject;
;}
    break;

  case 263:

/* Line 1455 of yacc.c  */
#line 1245 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  if (!Table->hasPrimaryKey)
    yyreport(CODING_ERROR, "No primary key for Exists\n");
  if (Table->hasExists == 0)
    CreateExists();
  Table->hasExists = 1;
  Table->extExists = ExtState;
  GrantState = stateExistsProcGrant;
  ProjectState = stateExistsProcProject;
;}
    break;

  case 264:

/* Line 1455 of yacc.c  */
#line 1256 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
    yyreport(WARNING_ERROR, "BulkInsert not currently supported\n");

;}
    break;

  case 265:

/* Line 1455 of yacc.c  */
#line 1261 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  if (Table->hasInsert == 0)
  {
    if (Table->hasLOB == 1 || Table->Version2Bin == 1)
      CreateInsertUsingReturning();
    else
      CreateInsert();
  }
  Table->hasInsert = 1;
  Table->extInsert = ExtState;
  GrantState = stateInsertProcGrant;
  ProjectState = stateInsertProcProject;
;}
    break;

  case 266:

/* Line 1455 of yacc.c  */
#line 1275 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  if (Table->hasSelectAll == 0)
    CreateSelectAll(0);
  Table->hasSelectAll = 1;
  Table->extSelectAll = ExtState;
  GrantState = stateSelectAllProcGrant;
  ProjectState = stateSelectAllProcProject;
;}
    break;

  case 267:

/* Line 1455 of yacc.c  */
#line 1284 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  if (!Table->hasPrimaryKey)
    yyreport(CODING_ERROR, "No primary key for SelectOne\n");
  if (Table->hasSelectOne == 0)
    CreateSelectOne(0);
  Table->hasSelectOne = 1;
  Table->extSelectOne = ExtState;
  GrantState = stateSelectOneProcGrant;
  ProjectState = stateSelectOneProcProject;
;}
    break;

  case 268:

/* Line 1455 of yacc.c  */
#line 1295 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  if (Table->hasSelectAllUpd == 0)
    CreateSelectAll(1);
  Table->hasSelectAllUpd = 1;
  Table->extSelectAllUpd = ExtState;
  GrantState = stateSelectAllUpdProcGrant;
  ProjectState = stateSelectAllUpdProcProject;
;}
    break;

  case 269:

/* Line 1455 of yacc.c  */
#line 1304 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  if (!Table->hasPrimaryKey)
    yyreport(CODING_ERROR, "No primary key for SelectOne\n");
  if (Table->hasSelectOneUpd == 0)
    CreateSelectOne(1);
  Table->hasSelectOneUpd = 1;
  Table->extSelectOneUpd = ExtState;
  GrantState = stateSelectOneUpdProcGrant;
  ProjectState = stateSelectOneUpdProcProject;
;}
    break;

  case 270:

/* Line 1455 of yacc.c  */
#line 1315 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  if (Table->hasSelectPlus == 0)
    CreateSelectPlus((yyvsp[(2) - (3)].i));
  Table->hasSelectPlus = 1;
  Table->extSelectPlus = ExtState;
  GrantState = stateSelectPlusProcGrant;
  ProjectState = stateSelectPlusProcProject;
;}
    break;

  case 271:

/* Line 1455 of yacc.c  */
#line 1324 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
    yyreport(WARNING_ERROR, "BulkUpdate not currently supported\n");
;}
    break;

  case 272:

/* Line 1455 of yacc.c  */
#line 1328 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  if (!Table->hasPrimaryKey)
    yyreport(CODING_ERROR, "No primary key for Update\n");
  if (Table->hasUpdate == 0)
  {
    if (Table->hasLOB == 1 || Table->Version2Bin == 1)
      CreateUpdateUsingReturning();
    else
      CreateUpdate();
  }
  Table->hasUpdate = 1;
  Table->extUpdate = ExtState;
  GrantState = stateUpdateProcGrant;
  ProjectState = stateUpdateProcProject;
;}
    break;

  case 273:

/* Line 1455 of yacc.c  */
#line 1346 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
    (yyval.i) = 128;
;}
    break;

  case 274:

/* Line 1455 of yacc.c  */
#line 1350 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  (yyval.i) = (yyvsp[(1) - (1)].i);
;}
    break;

  case 275:

/* Line 1455 of yacc.c  */
#line 1356 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  AddView();
;}
    break;

  case 276:

/* Line 1455 of yacc.c  */
#line 1361 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
    GrantState = stateViewGrant;
;}
    break;

  case 277:

/* Line 1455 of yacc.c  */
#line 1367 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  SetView((yyvsp[(1) - (1)].s));
;}
    break;

  case 279:

/* Line 1455 of yacc.c  */
#line 1376 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
    LineState = stateView;
;}
    break;

  case 280:

/* Line 1455 of yacc.c  */
#line 1382 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  AddWith();
;}
    break;

  case 281:

/* Line 1455 of yacc.c  */
#line 1388 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  SetWith((yyvsp[(1) - (1)].s));
;}
    break;

  case 285:

/* Line 1455 of yacc.c  */
#line 1399 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
    LineState = stateWith;
;}
    break;

  case 286:

/* Line 1455 of yacc.c  */
#line 1405 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  Proc.extProc = ExtState;
  AddProc();
;}
    break;

  case 291:

/* Line 1455 of yacc.c  */
#line 1422 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  ExtState |= noC;
;}
    break;

  case 292:

/* Line 1455 of yacc.c  */
#line 1426 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  ExtState |= noPAS;
;}
    break;

  case 293:

/* Line 1455 of yacc.c  */
#line 1430 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  ExtState |= noVB;
;}
    break;

  case 294:

/* Line 1455 of yacc.c  */
#line 1434 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  ExtState |= noEXT;
;}
    break;

  case 295:

/* Line 1455 of yacc.c  */
#line 1438 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  ExtState |= noIDL;
;}
    break;

  case 298:

/* Line 1455 of yacc.c  */
#line 1449 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  if ((yyvsp[(2) - (2)].i) != 0) { ExtState |= doEXTC; Table->MarkedExtC = 1; }
  else          ExtState |= noEXTC;
;}
    break;

  case 299:

/* Line 1455 of yacc.c  */
#line 1454 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  if ((yyvsp[(2) - (2)].i) != 0) { ExtState |= doEXTPAS; Table->MarkedExtPAS = 1; }
  else          ExtState |= noEXTPAS;
;}
    break;

  case 300:

/* Line 1455 of yacc.c  */
#line 1459 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  if ((yyvsp[(2) - (2)].i) != 0) { ExtState |= doEXTVB; Table->MarkedExtVB = 1; }
  else          ExtState |= noEXTVB;
;}
    break;

  case 301:

/* Line 1455 of yacc.c  */
#line 1464 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  if ((yyvsp[(2) - (2)].i) != 0) { ExtState |= doC; Table->MarkedC = 1; }
  else          ExtState |= noC;
;}
    break;

  case 302:

/* Line 1455 of yacc.c  */
#line 1469 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  if ((yyvsp[(2) - (2)].i) != 0) { ExtState |= doIDL; Table->MarkedIDL = 1; }
  else          ExtState |= noIDL;
;}
    break;

  case 303:

/* Line 1455 of yacc.c  */
#line 1474 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  if ((yyvsp[(2) - (2)].i) != 0) { ExtState |= doPAS; Table->MarkedPAS = 1; }
  else          ExtState |= noPAS;
;}
    break;

  case 304:

/* Line 1455 of yacc.c  */
#line 1479 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  if ((yyvsp[(2) - (2)].i) != 0) { ExtState |= doPYTHON; Table->MarkedPython = 1; }
  else          ExtState |= noPYTHON;
;}
    break;

  case 305:

/* Line 1455 of yacc.c  */
#line 1484 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  if ((yyvsp[(2) - (2)].i) != 0) { ExtState |= doVB; Table->MarkedVB = 1; }
  else          ExtState |= noVB;
;}
    break;

  case 306:

/* Line 1455 of yacc.c  */
#line 1489 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  if ((yyvsp[(2) - (2)].i) != 0) { ExtState |= doVB5; Table->MarkedVB5 = 1; }
  else          ExtState |= noVB5;
;}
    break;

  case 307:

/* Line 1455 of yacc.c  */
#line 1494 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  if ((yyvsp[(2) - (2)].i) != 0) { ExtState |= doVBCODE3; Table->MarkedVBCode3 = 1; }
  else          ExtState |= noVBCODE3;
;}
    break;

  case 308:

/* Line 1455 of yacc.c  */
#line 1499 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  if ((yyvsp[(2) - (2)].i) != 0) { ExtState |= doVBNET7; Table->MarkedVBNet7 = 1; }
  else          ExtState |= noVBNET7;
;}
    break;

  case 309:

/* Line 1455 of yacc.c  */
#line 1504 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  if ((yyvsp[(2) - (2)].i) != 0) { ExtState |= doCSNET7; Table->MarkedCSNet7 = 1; }
  else          ExtState |= noCSNET7;
;}
    break;

  case 310:

/* Line 1455 of yacc.c  */
#line 1509 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  if ((yyvsp[(2) - (2)].i) != 0) { ExtState |= doCSADONET; Table->MarkedCSAdoNet = 1; }
  else          ExtState |= noCSADONET;
;}
    break;

  case 311:

/* Line 1455 of yacc.c  */
#line 1514 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  if ((yyvsp[(2) - (2)].i) != 0) { ExtState |= doCSRW; Table->MarkedCSRW = 1; }
  else          ExtState |= noCSRW;
;}
    break;

  case 312:

/* Line 1455 of yacc.c  */
#line 1519 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  if ((yyvsp[(2) - (2)].i) != 0) { ExtState |= doCSIDL2; Table->MarkedCSIDL2 = 1; }
  else          ExtState |= noCSIDL2;
;}
    break;

  case 313:

/* Line 1455 of yacc.c  */
#line 1524 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  if ((yyvsp[(2) - (2)].i) != 0) { ExtState |= doVBFORIDL; Table->MarkedVBforIDL = 1; }
  else          ExtState |= noVBFORIDL;
;}
    break;

  case 314:

/* Line 1455 of yacc.c  */
#line 1529 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  if ((yyvsp[(2) - (2)].i) != 0) { ExtState |= doVBFORADOR; Table->MarkedVBforADOR = 1; }
  else          ExtState |= noVBFORADOR;
;}
    break;

  case 315:

/* Line 1455 of yacc.c  */
#line 1534 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  yyreport(CODING_ERROR, "%s is not a valid switch\n", (yyvsp[(1) - (1)].s));
  free((yyvsp[(1) - (1)].s));
;}
    break;

  case 320:

/* Line 1455 of yacc.c  */
#line 1552 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  AddProject((yyvsp[(1) - (1)].s));
;}
    break;

  case 321:

/* Line 1455 of yacc.c  */
#line 1558 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  SetProc((yyvsp[(1) - (1)].s));
;}
    break;

  case 326:

/* Line 1455 of yacc.c  */
#line 1574 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
    FieldState = stateInput;
;}
    break;

  case 329:

/* Line 1455 of yacc.c  */
#line 1585 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
    FieldState = stateInOut;
;}
    break;

  case 332:

/* Line 1455 of yacc.c  */
#line 1596 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
    UseStdRecOutput();
;}
    break;

  case 334:

/* Line 1455 of yacc.c  */
#line 1604 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
    Proc.isManyQuery = 1;
    Proc.isSingle = 0;
;}
    break;

  case 335:

/* Line 1455 of yacc.c  */
#line 1609 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
    Proc.isManyQuery = 1;
    Proc.isSingle = 0;
;}
    break;

  case 336:

/* Line 1455 of yacc.c  */
#line 1614 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
    Proc.isManyQuery = 0;
    Proc.isSingle = 1;
;}
    break;

  case 337:

/* Line 1455 of yacc.c  */
#line 1621 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
    FieldState = stateOutput;
;}
    break;

  case 342:

/* Line 1455 of yacc.c  */
#line 1637 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  AddUseWith((yyvsp[(1) - (1)].s));
;}
    break;

  case 346:

/* Line 1455 of yacc.c  */
#line 1650 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  AddList(Proc.Cursors, TYYCursor, Cursor, Proc.noCursors, 8);
;}
    break;

  case 350:

/* Line 1455 of yacc.c  */
#line 1662 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  SetFieldType(ftypeException, 0);
  AddField((yyvsp[(1) - (3)].s));
;}
    break;

  case 351:

/* Line 1455 of yacc.c  */
#line 1669 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
    memset(&Cursor, 0, sizeof(TYYCursor));
    LineState = stateCursor;
;}
    break;

  case 352:

/* Line 1455 of yacc.c  */
#line 1676 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
    FieldState = stateWork;
;}
    break;

  case 356:

/* Line 1455 of yacc.c  */
#line 1688 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
    LineState = stateCode;
;}
    break;

  case 357:

/* Line 1455 of yacc.c  */
#line 1694 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
    Proc.isSql = 1;
    if (Proc.noWorkFields)
        yyreport(CODING_ERROR, "%s SQL code should not have DECLARE work fields\n", Proc.Name);
;}
    break;

  case 360:

/* Line 1455 of yacc.c  */
#line 1707 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  AddLine((yyvsp[(1) - (1)].s));
  if (LineState == stateCode)
    AddLine(strdup("\n"));
;}
    break;

  case 361:

/* Line 1455 of yacc.c  */
#line 1715 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
  Table->noDatas++;
  AddProc();
;}
    break;

  case 362:

/* Line 1455 of yacc.c  */
#line 1722 "C:/vlab/utility/dbportal/code/sqldef.y"
    {
    SetProc(0);
    LineState = stateData;
    Proc.isData = 1;
;}
    break;



/* Line 1455 of yacc.c  */
#line 4812 "C:/vlab/utility/dbportal/code/sqldef.cpp"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1675 of yacc.c  */
#line 1728 "C:/vlab/utility/dbportal/code/sqldef.y"

static char* InBuf;
static int InBufSoFar;

int yylex()
{
    return inbuf_lex(InBuf, InBufSoFar);
}

int inbuf_parse(PYYTable aTable, char* aInBuf)
{
    Table = aTable;
    InBuf = aInBuf;
    InBufSoFar = 0;
    return yyparse();
}

static void SetSwitch(int &Switch, int value)
{
    Switch = value;
}

static void SetSwitch(char *&Switch, char *value)
{
    if (Switch) free(Switch);
    Switch = strdup(value);
}

static char *Concat(char *s1, char *s2)
{
    char *s = (char*)calloc(strlen(s1) + strlen(s2) + 2, 1);
    strcpy(s, s1);
    strcat(s, ".");
    strcat(s, s2);
    free(s1);
    free(s2);
    return s;
}

static char *Combine(char *s1, char *s2)
{
    char *s = (char*)calloc(strlen(s1) + strlen(s2) + 2, 1);
    strcpy(s, s1);
    strcat(s, s2);
    free(s1);
    free(s2);
    return s;
}

static int32 Hash(char *s, int m)
{
    int i;
    int32 result = 0;
    for (i = 0; i < (int)strlen(s); i++)
        result += (s[i] * m * (i + 1));
    return result;
}

static int32  FieldPadding()
{
    if (Field.Padding == 1
        && Field.Type != ftypeTinyint
        && NullableField(&Field))
        Field.Padding = Table->EightByte == 1 ? 8 : 2;
    return Field.Padding;
}

static void AddTableField(char* s)
{
    int32  n;
    Field.Name = s;
    if ((n = Table->Length % FieldPadding()) != 0)
        Field.Filler = FieldPadding() - n;
    else
        Field.Filler = 0;
    Table->Length += Field.Filler;
    Field.Offset = Table->Length;
    Table->Length += Field.Length;
    if (Table->isNullEnabled && Field.isNull && NullableField(&Field))
        Table->Length += 2;
    Table->Token += Hash(Field.Name, 23) * (Field.Offset + 1) * Field.Length;
    Table->ABSize += strlen(Field.Name) + 8;
    AddList(Table->Fields, TYYField, Field, Table->noFields, 8);
}

static void AddRowUser(void)
{
    memset(&Field, 0, sizeof(TYYField));
    SetFieldType(ftypeUserStamp, 0);
    AddTableField(strdup("RowUser"));
}

static void AddRowStatus(void)
{
    memset(&Field, 0, sizeof(TYYField));
    SetFieldType(ftypeStatus, 0);
    AddTableField(strdup("RowStatus"));
}

static void AddRowUpdated(void)
{
    memset(&Field, 0, sizeof(TYYField));
    SetFieldType(ftypeTimeStamp, 6);
    AddTableField(strdup("RowUpdated"));
}

static void AddRowSeq(void)
{
    Field.Alias = 0;
    Field.Type = ftypeFloat;
    Field.Length = sizeof(double);
    Field.Padding = sizeof(double);
    Field.Precision = 15;
    Field.Scale = 0;
    Field.isPrimaryKey = 0;
    Field.isNull = 0;
    Field.isInput = 0;
    Field.isBound = 0;
    Field.isOutput = 0;
    Field.isWork = 0;
    Field.isRowSequence = 1;
    AddTableField(strdup("RowSeq"));
}

static void AddInputField(char* s)
{
    int32  n;
    Field.Name = s;
    if ((n = Proc.Length % FieldPadding()) != 0)
        Field.Filler = FieldPadding() - n;
    else
        Field.Filler = 0;
    Proc.Length += Field.Filler;
    Field.Offset = Proc.Length;
    Field.isInput = 1;
    Proc.Length += Field.Length;
    if (Table->isNullEnabled && Field.isNull && NullableField(&Field))
        Proc.Length += 2;
    Proc.ABSize += (strlen(Field.Name) + 8);
    AddList(Proc.Fields, TYYField, Field, Proc.noFields, 8);
}

static void AddOutputField(char* s)
{
    int32  FldPos;
    int32  n;
    for (FldPos = 0; FldPos < Proc.noFields; FldPos++)
    {
        if ((stricmp(Proc.Fields[FldPos].Name, s)) == 0)
            break;
    }
    if (FldPos < Proc.noFields)
    {
        Proc.Fields[FldPos].isOutput = 1;
        free(s);
    }
    else
    {
        Field.Name = s;
        if ((n = Proc.Length % FieldPadding()) != 0)
            Field.Filler = FieldPadding() - n;
        else
            Field.Filler = 0;
        Proc.Length += Field.Filler;
        Field.Offset = Proc.Length;
        Field.isOutput = 1;
        Proc.Length += Field.Length;
        if (Table->isNullEnabled && Field.isNull && NullableField(&Field))
            Proc.Length += 2;
        Proc.ABSize += (strlen(Field.Name) + 8);
        AddList(Proc.Fields, TYYField, Field, Proc.noFields, 8);
    }
    AddList(Proc.Binds, int32, FldPos, Proc.noBinds, 16);
    Proc.noOutputs++;
}

static void AddInOutField(char* s)
{
    int32  n;
    Field.Name = s;
    if ((n = Proc.Length % FieldPadding()) != 0)
        Field.Filler = FieldPadding() - n;
    else
        Field.Filler = 0;
    Proc.Length += Field.Filler;
    Field.Offset = Proc.Length;
    Field.isInput = 1;
    Field.isOutput = 1;
    Proc.Length += Field.Length;
    if (Table->isNullEnabled && Field.isNull && NullableField(&Field))
        Proc.Length += 2;
    Proc.ABSize += (strlen(Field.Name) + 8);
    AddList(Proc.Fields, TYYField, Field, Proc.noFields, 8);
}

static void AddStdOutputField(char* s)
{
    int32  FldPos;
    int32  n;
    if ((n = Proc.Length % FieldPadding()) != 0)
        Field.Filler = FieldPadding() - n;
    else
        Field.Filler = 0;
    Proc.Length += Field.Filler;
    for (FldPos = 0; FldPos < Proc.noFields; FldPos++)
    {
        if ((stricmp(Proc.Fields[FldPos].Name, s)) == 0)
            break;
    }
    if (FldPos < Proc.noFields)
    {
        Proc.Length += Field.Length;
        if (Table->isNullEnabled && Field.isNull && NullableField(&Field))
            Proc.Length += 2;
        Proc.Fields[FldPos].isOutput = 1;
        free(s);
    }
    else
    {
        Field.Name = s;
        Field.Offset = Proc.Length;
        Field.isOutput = 1;
        Proc.Length += Field.Length;
        if (Table->isNullEnabled && Field.isNull && NullableField(&Field))
            Proc.Length += 2;
        Proc.ABSize += (strlen(Field.Name) + 8);
        AddList(Proc.Fields, TYYField, Field, Proc.noFields, 8);
    }
    AddList(Proc.Binds, int32, FldPos, Proc.noBinds, 16);
    Proc.noOutputs++;
}

static void UseStdRecOutput()
{
    int t, p, useStd = 1;
    for (p = 0; p < Proc.noFields; p++)
    {
        for (t = 0; t < Table->noFields; t++)
        {
            if ((stricmp(Proc.Fields[p].Name, Table->Fields[t].Name)) == 0
                && Proc.Fields[p].Type == Table->Fields[t].Type
                && Proc.Fields[p].Length == Table->Fields[t].Length)
                break;
        }
        if (t >= Table->noFields)
            useStd = 0;
    }
    Proc.useStd = useStd;
    if (useStd == 1)
    {
        Proc.Length = 0;
        for (t = 0; t < Table->noFields; t++)
        {
            for (p = 0; p < Proc.noFields; p++)
            {
                if ((stricmp(Proc.Fields[p].Name, Table->Fields[t].Name)) == 0)
                {
                    Proc.Fields[p].Offset = Table->Fields[t].Offset;
                    Proc.Fields[p].Filler = Table->Fields[t].Filler;
                    Proc.Fields[p].Padding = Table->Fields[t].Padding;
                }
            }
        }
    }
    for (t = 0; t < Table->noFields; t++)
    {
        Field = Table->Fields[t]; Field.noConsts = 0;
        if (Field.Alias) Field.Alias = strdup(Field.Alias);
        if (useStd == 1)
            AddStdOutputField(strdup(Field.Name));
        else
            AddOutputField(strdup(Field.Name));
    }
}

static void AddDynamicField(char* s)
{
    int i;
    int32  n;
    Field.Name = s;
    Field.Alias = 0;
    for (i = 0; i < Proc.noFields; i++)
    {
        if ((strcmp(Proc.Fields[i].Name, Field.Name)) == 0) // see sqlapi.c
        {                                                 // using exact match
            if (Proc.Fields[i].Type == ftypeDynamic)
            {
                free(s);
                return;
            }
            yyreport(CODING_ERROR, "Dynamic field already used as non dynamic\n");
        }
    }
    if ((n = Proc.Length % FieldPadding()) != 0)
        Field.Filler = FieldPadding() - n;
    else
        Field.Filler = 0;
    Field.isInput = 0;
    Field.isOutput = 0;
    Proc.Length += Field.Filler;
    Field.Offset = Proc.Length;
    Proc.Length += Field.Length;
    Proc.ABSize += (strlen(Field.Name) + 8);
    AddList(Proc.Binds, int32, Proc.noFields, Proc.noBinds, 16);
    AddList(Proc.Fields, TYYField, Field, Proc.noFields, 8);
}

static void AddWorkField(char* s)
{
    Field.Name = s;
    Field.isWork = 1;
    Proc.ABSize += (strlen(Field.Name) + 8);
    AddList(Proc.WorkFields, TYYField, Field, Proc.noWorkFields, 4);
}

static void AddConst(char* s, int i)
{
    Const.Name = s;
    Const.Value = i;
    Const.Type = 0;
    AddList(Field.Consts, TYYConst, Const, Field.noConsts, 8);
}

static void AddCharConst(char* s, int i)
{
    Const.Name = s;
    Const.Value = i;
    Const.Type = 1;
    AddList(Field.Consts, TYYConst, Const, Field.noConsts, 8);
}

static void AddStringConst(char* s, char* d)
{
    int i = atoi(d);
    free(d);
    AddConst(s, i);
}

static void AddConstIdent(char* s)
{
    Const.Name = s;
    Const.Value = Field.noConsts;
    Const.Type = 0;
    AddList(Field.Consts, TYYConst, Const, Field.noConsts, 8);
}

static void AddField(char* s)
{
    switch (FieldState)
    {
    case stateTable:
        AddTableField(s);
        break;
    case stateInput:
        AddInputField(s);
        break;
    case stateInOut:
        AddInOutField(s);
        break;
    case stateOutput:
        AddOutputField(s);
        break;
    case stateWork:
        AddWorkField(s);
        break;
    }
}

static void SetFieldType(eFieldType Type, int Length)
{
    Field.Type = Type;
    Field.Padding = 1;
    switch (Type)
    {
    case ftypeChar:
        Field.Scale = 0;
        Field.Precision = Length;
        Field.Length = Length + 1;
        break;
    case ftypeNChar:
        Field.Scale = 0;
        Field.Precision = Length;
        Field.Length = Length + 1;
        break;
    case ftypeBinary:
        Field.Scale = 0;
        Field.Precision = Length;
        Field.Length = Length + 1;
        break;
    case ftypeStatus:
        Field.Scale = 0;
        Field.Precision = 1;
        Field.Length = 2;
        break;
    case ftypeDate:
        Field.Scale = 0;
        Field.Length = 9;
        Field.Precision = 8;
        break;
    case ftypeTimeStamp:
        Field.Scale = Length;
        Field.Length = 15;
        Field.Precision = 14;
        break;
    case ftypeDateTime:
        Field.Scale = 0;
        Field.Length = 15;
        Field.Precision = 14;
        break;
    case ftypeUserStamp:
        Field.Scale = 0;
        if (Length <= 1)
        {
            Field.Length = 9;
            Field.Precision = 8;
        }
        else
        {
            Field.Length = Length + 1;
            Field.Precision = Length;
        }
        break;
    case ftypeBoolean:
        Field.Scale = 0;
        Field.Length = sizeof(char);
        Field.Precision = 1;
        break;
    case ftypeTinyint:
        Field.Scale = 0;
        Field.Length = 1;
        Field.Precision = 3;
        break;
    case ftypeSmallint:
        Field.Scale = 0;
        Field.Padding = Table->EightByte == 1 ? 8 : 2;
        Field.Length = 2;
        Field.Precision = 5;
        break;
    case ftypeInt:
        Field.Scale = 0;
        Field.Padding = Table->EightByte == 1 ? 8 : 4;
        Field.Length = 4;
        Field.Precision = 10;
        break;
    case ftypeLong:
        Field.Scale = 0;
        Field.Padding = 8;
        Field.Length = 8;
        Field.Precision = 19;
        break;
    case ftypeFloat:
        if (Field.Precision <= 15)
        {
            Field.Padding = sizeof(double);
            Field.Length = sizeof(double);
            break;
        }
        Field.Type = ftypeMoney;
    case ftypeMoney:
        Field.Length = Field.Precision + 3;
        break;
    case ftypeVarNum:
        Field.Length = 22;
        break;
    case ftypeBLOB:
    case ftypeCLOB:
        if (Length >= 65532 && Table->Version2Bin == 0)
            Table->Version2Bin = 1;
        Field.Padding = Table->EightByte == 1 ? 8 : sizeof(int);
        Field.Length = Length + 4;
        Field.Precision = Length + 4;
        break;
    case ftypeXMLTYPE:
        if (Length >= 65532 && Table->Version2Bin == 0)
            Table->Version2Bin = 1;
        Field.Padding = Table->EightByte == 1 ? 8 : sizeof(int);
        Field.Length = Length + 4;
        Field.Precision = Length + 4;
        break;
    case ftypeZLOB:
        if (Length >= 65528 && Table->Version2Bin == 0)
            Table->Version2Bin = 1;
        Field.Padding = Table->EightByte == 1 ? 8 : sizeof(int);
        Field.Length = Length + 8;
        Field.Precision = Length + 8;
        break;
    case ftypeImage:
        if (Length == 1)
        {
            Field.Type = ftypeImagePtr;
            Field.Padding = Table->EightByte == 1 ? 8 : sizeof(void*);
            Field.Length = sizeof(void*);
        }
        else
        {
            Field.Padding = Table->EightByte == 1 ? 8 : sizeof(short);
            Field.Length = Length + 2;
            Field.Precision = Length + 2;
        }
        break;
    case ftypeHugeCHAR:
        if (Length == 0)
        {
            Field.Length = 8;
            Field.Padding = 8;
            Field.Precision = 8;
        }
        else
        {
            Field.Length = Length + 4;
            Field.Precision = Length + 4;
        }
        break;
    case ftypeException:
        Field.Length = 0;
        break;
    case ftypeDynamic:
        Field.Length = Length;
        break;
    }
}

static void SetFromRef(PYYField RefField)
{
    if (RefField)
    {
        if (RefField->Alias && !Field.Alias) Field.Alias = strdup(RefField->Alias);
        Field.Type = RefField->Type;
        Field.Scale = RefField->Scale;
        Field.Precision = RefField->Precision;
        Field.Length = RefField->Length;
        Field.Padding = RefField->Padding;
    }
    else
        yyreport(CODING_ERROR, "Reference Field %s not found\n", RefField);
}

static void SetSameFromTable()
{
    SetFromRef(FindField(Field.Name));
}

static void SetFromTable(char *s)
{
    SetFromRef(FindField(s));
    free(s);
}

static void AddPermission(char* s)
{
    Perm.p = s;
    AddList(Grant.Perms, TYYLine, Perm, Grant.noPerms, 4);
}

static void AddUser(char* s)
{
    User.p = s;
    switch (GrantState)
    {
    case stateTableGrant:
        AddList(Grant.Users, TYYLine, User, Grant.noUsers, 4);
        if (Table->hasExecute)
        {
            User.p = strdup(s);
            AddList(Table->ExecuteUsers, TYYLine, User, Table->noExecuteUsers, 4);
        }
        if (Table->hasSelect)
        {
            User.p = strdup(s);
            AddList(Table->SelectUsers, TYYLine, User, Table->noSelectUsers, 4);
        }
        break;
    case stateProcGrant:
        AddList(Proc.Users, TYYLine, User, Proc.noUsers, 4);
        break;
    case stateViewGrant:
        AddList(View.Users, TYYLine, User, View.noUsers, 4);
        break;
    case stateCountProcGrant:
        AddList(Table->CountUsers, TYYLine, User, Table->noCountUsers, 4);
        break;
    case stateDeleteAllProcGrant:
        AddList(Table->DeleteAllUsers, TYYLine, User, Table->noDeleteAllUsers, 4);
        break;
    case stateDeleteOneProcGrant:
        AddList(Table->DeleteOneUsers, TYYLine, User, Table->noDeleteOneUsers, 4);
        break;
    case stateExistsProcGrant:
        AddList(Table->ExistsUsers, TYYLine, User, Table->noExistsUsers, 4);
        break;
    case stateInsertProcGrant:
        AddList(Table->InsertUsers, TYYLine, User, Table->noInsertUsers, 4);
        break;
    case stateSelectAllProcGrant:
        AddList(Table->SelectAllUsers, TYYLine, User, Table->noSelectAllUsers, 4);
        break;
    case stateSelectOneProcGrant:
        AddList(Table->SelectOneUsers, TYYLine, User, Table->noSelectOneUsers, 4);
        break;
    case stateSelectAllUpdProcGrant:
        AddList(Table->SelectAllUpdUsers, TYYLine, User, Table->noSelectAllUpdUsers, 4);
        break;
    case stateSelectOneUpdProcGrant:
        AddList(Table->SelectOneUpdUsers, TYYLine, User, Table->noSelectOneUpdUsers, 4);
        break;
    case stateSelectPlusProcGrant:
        AddList(Table->SelectPlusUsers, TYYLine, User, Table->noSelectPlusUsers, 4);
        break;
    case stateUpdateProcGrant:
        AddList(Table->UpdateUsers, TYYLine, User, Table->noUpdateUsers, 4);
        break;
    }
}

static void AddProject(char* s)
{
    User.p = s;
    switch (ProjectState)
    {
    case stateProcProject:
        AddList(Proc.Projects, TYYLine, User, Proc.noProjects, 4);
        break;
    case stateCountProcProject:
        AddList(Table->CountProjects, TYYLine, User, Table->noCountProjects, 4);
        break;
    case stateDeleteAllProcProject:
        AddList(Table->DeleteAllProjects, TYYLine, User, Table->noDeleteAllProjects, 4);
        break;
    case stateDeleteOneProcProject:
        AddList(Table->DeleteOneProjects, TYYLine, User, Table->noDeleteOneProjects, 4);
        break;
    case stateExistsProcProject:
        AddList(Table->ExistsProjects, TYYLine, User, Table->noExistsProjects, 4);
        break;
    case stateInsertProcProject:
        AddList(Table->InsertProjects, TYYLine, User, Table->noInsertProjects, 4);
        break;
    case stateSelectAllProcProject:
        AddList(Table->SelectAllProjects, TYYLine, User, Table->noSelectAllProjects, 4);
        break;
    case stateSelectOneProcProject:
        AddList(Table->SelectOneProjects, TYYLine, User, Table->noSelectOneProjects, 4);
        break;
    case stateSelectAllUpdProcProject:
        AddList(Table->SelectAllUpdProjects, TYYLine, User, Table->noSelectAllUpdProjects, 4);
        break;
    case stateSelectOneUpdProcProject:
        AddList(Table->SelectOneUpdProjects, TYYLine, User, Table->noSelectOneUpdProjects, 4);
        break;
    case stateSelectPlusProcProject:
        AddList(Table->SelectPlusProjects, TYYLine, User, Table->noSelectPlusProjects, 4);
        break;
    case stateUpdateProcProject:
        AddList(Table->UpdateProjects, TYYLine, User, Table->noUpdateProjects, 4);
        break;
    }
}

static void AddGrant(void)
{
    AddList(Table->Grants, TYYGrant, Grant, Table->noGrants, 4);
}

static void SetKey(char* s)
{
    memset(&Key, 0, sizeof(TYYKey));
    Key.Name = s;
}

static void AddKey(void)
{
    AddList(Table->Keys, TYYKey, Key, Table->noKeys, 4);
    if (Key.isPrimary)
    {
        Table->PrimaryKeyFields++;
        Table->PrimaryKey = Table->noKeys - 1;
    }
}

static void SetLink(char* s)
{
    Link.MasterName = s;
    ColumnState = stateLink;
}

static void DeleteCascadeLink(bool val)
{
    Link.DeleteCascade = val;
}

static void AddLink(void)
{
    AddList(Table->Links, TYYLink, Link, Table->noLinks, 4);
}

static void AddKeyColumn(char* s)
{
    /* const int32  DELTA = 4; */
    short n;
    PYYField Field = FindField(s);
    if (!Field)
        yyreport(CODING_ERROR, "Key Field %s not found\n", s);
    else
    {
        TYYField Copy = *Field;
        if (Copy.Alias)
            Copy.Alias = strdup(Copy.Alias);
        if ((n = Key.Length % Copy.Padding) != 0)
            Copy.Filler = Copy.Padding - n;
        else
            Copy.Filler = 0;
        Key.Length += Copy.Filler;
        Copy.Offset = Key.Length;
        AddList(Key.Fields, TYYField, Copy, Key.noFields, 4);
        Key.Length += Copy.Length;
        Key.ABSize += (strlen(Field->Name) + 8);
        if (Key.isPrimary)
            Field->isPrimaryKey = 1;
        if (Field->isNull == 1 && (Key.isPrimary || Key.isUnique))
            Field->isNull = 0;
    }
    free(s);
}

static void AddLinkColumn(char* s)
{
    PYYField Field = FindField(s);
    if (!Field)
        yyreport(CODING_ERROR, "Key Field %s not found\n", s);
    else
    {
        AddList(Link.Fields, TYYField, *Field, Link.noFields, 4);
    }
    free(s);
}

static void AddColumn(char* s)
{
    switch (ColumnState)
    {
    case stateKey:
        AddKeyColumn(s);
        break;
    case stateLink:
        AddLinkColumn(s);
        break;
    }
}

static PYYField FindField(char* s)
{
    int32  i;
    for (i = 0; i < Table->noFields; i++)
        if (stricmp(s, Table->Fields[i].Name) == 0)
            return &Table->Fields[i];
    return 0;
}

static void SetFlag(char *s)
{
    if (stricmp(s, "VB5IO") == 0)
        Table->hasVB5IO = 1;
    free(s);
}

static void SetProc(char *s)
{
    memset(&Proc, 0, sizeof(TYYProc));
    Proc.Name = s;
}

static void SetView(char *s)
{
    memset(&View, 0, sizeof(TYYView));
    View.Name = s;
}

static void SetWith(char *s)
{
    memset(&With, 0, sizeof(TYYWith));
    With.Name = s;
}

static void AddProc(void)
{
    Proc.extProc = ExtState;
    if (Proc.isManyQuery && !Proc.isSql)
    {
        if (Proc.isManyQuery == 1)
            yyreport(CODING_ERROR, "Only SQL CODE procs may use (multiple)\n");
        Proc.isManyQuery = 0;
    }
    AddList(Table->Procs, TYYProc, Proc, Table->noProcs, 4);
}

static void AddView(void)
{
    AddList(Table->Views, TYYView, View, Table->noViews, 4);
}

static void AddWith(void)
{
    AddList(Table->Withs, TYYWith, With, Table->noWiths, 4);
}

static void AddUseWith(char* s)
{
    int i;
    for (i = 0; i < Proc.noWiths; i++)
    {
        if (stricmp(Proc.Withs[i].p, s) == 0)
        {
            yyreport(CODING_ERROR, "With %s %s is already used as %d argument.", Proc.Withs[i].p, s, i);
            free(s);
            return;
        }
    }
    for (i = 0; i < Table->noWiths; i++)
    {
        if (stricmp(Table->Withs[i].Name, s) == 0)
        {
            UseWith.p = s;
            AddList(Proc.Withs, TYYLine, UseWith, Proc.noWiths, 4);
            return;
        }
    }
    yyreport(CODING_ERROR, "With %s is not defined yet.", s);
    free(s);
}

static void QuestionThis(char *s)
{
    int32  i;

    for (i = 0; i < Proc.noFields; i++)
        if (stricmp(s + 1, Proc.Fields[i].Name) == 0)
            break;
    if (i >= Proc.noFields)
        yyreport(CODING_ERROR, "Proc :Field %s not found\n", s + 1);
    else if (!Proc.isSql)
    {
        if (Table->UnderScore)
        {
            char *w = (char *)malloc(strlen(s) + 2);
            strcpy(w, "a_");
            strcat(w, s + 1);
            Line.p = w;
            free(s);
        }
        else
        {
            *s = 'a';
            Line.p = s;
        }
        AddList(Proc.Lines, TYYLine, Line, Proc.noLines, 4);
    }
    else
    {
        Line.p = s;
        AddList(Proc.Lines, TYYLine, Line, Proc.noLines, 4);
        if (!Proc.Fields[i].isBound)
            AddList(Proc.Binds, int32, i, Proc.noBinds, 16);
        Proc.Fields[i].isBound = 1;
    }
}

static void Questions(char *s)
{
    int i, n, len;
    pchar t = s;
    pchar word;
    char w[128];
    while (t[0])
    {
        t = SqlToken(t, w, sizeof(w));
        switch (w[0])
        {
        case ':':
            if (strlen(w) == 1)
            {
                if (t[0] == ':')
                    strcpy(t, t + 1);
                break;
            }
            n = (int)strlen(w) * -1;
            t[n] = 0;//t[-strlen(w)] = 0;
            Line.p = strdup(s);
            AddList(Proc.Lines, TYYLine, Line, Proc.noLines, 4);
            word = strdup(w);
            QuestionThis(word);
            s = t;
            break;
        case '$':
            len = 255;
            if (strlen(w) == 1)
            {
                if (t[0] == '$')
                    strcpy(t, t + 1);
                break;
            }
            if (t[0] == '(')
            {
                for (i = 0; t[i]; i++)
                {
                    if (t[i] == ')')
                    {
                        for (n = 1; n < i; n++)
                            if (isdigit(t[n]))
                                break;
                        if (n == i)
                            break;
                        t[i] = 0;
                        len = atoi(t + n);
                        t[i] = ' ';
                        strcpy(t, t + i);
                        break;
                    }
                }
            }
            word = strdup(w + 1);
            SetFieldType(ftypeDynamic, len);
            AddDynamicField(word);
            break;
        }
    }
    Line.p = strdup(s);
    Line.isEOL = 1;
    AddList(Proc.Lines, TYYLine, Line, Proc.noLines, 4);
    Line.isEOL = 0;
}

static void AddLine(char *s)
{
    Line.p = s;
    switch (LineState)
    {
    case stateCode:
        Questions(s);
        free(s);
        break;
    case stateData:
        AddList(Proc.Lines, TYYLine, Line, Proc.noLines, 4);
        break;
    case stateView:
        AddList(View.Lines, TYYLine, Line, View.noLines, 4);
        break;
    case stateCursor:
        AddList(Cursor.Lines, TYYLine, Line, Cursor.noLines, 4);
        break;
    case stateWith:
        AddList(With.Lines, TYYLine, Line, With.noLines, 4);
        break;
    }
}

static pchar InputConversion(TYYField Field, int Extended)
{
    // Extended = 0 (Normal) = 1 (Insert) = 2 (Update)
    static char Work[256];
    if (Extended == 1 && Field.isSequence)
    {
        sprintf(Work, "%sSeq.NEXTVAL", Table->Name);
        return Work;
    }
    switch (Field.Type)
    {
    case ftypeDate:
        sprintf(Work, "to_date(:%s, 'YYYYMMDD')", Field.Name);
        return Work;
    case ftypeDateTime:
        sprintf(Work, "to_date(:%s, 'YYYYMMDDHH24MISS')", Field.Name);
        return Work;
    case ftypeTimeStamp:
        if (Extended == 0)
            sprintf(Work, "to_date(:%s, 'YYYYMMDDHH24MISS')", Field.Name);
        else if (Field.Scale != 0)
            sprintf(Work, "CURRENT_TIMESTAMP");
        else
            sprintf(Work, "SYSDATE");
        return Work;
    case ftypeMoney:
        sprintf(Work, "to_number(:%s)", Field.Name);
        return Work;
    case ftypeUserStamp:
        if (Extended == 0)
            break;
        sprintf(Work, "USER");
        return Work;
    }
    sprintf(Work, ":%s", Field.Name);
    return Work;
}

static pchar OutputConversion(TYYField Field)
{
    static char Work[256];
    switch (Field.Type)
    {
    case ftypeDate:
        sprintf(Work, "to_char(%s, 'YYYYMMDD')", Qualify(Table->Name, Field.Name));
        return Work;
    case ftypeDateTime:
    case ftypeTimeStamp:
        sprintf(Work, "to_char(%s, 'YYYYMMDDHH24MISS')", Qualify(Table->Name, Field.Name));
        return Work;
    case ftypeMoney:
        sprintf(Work, "to_char(%s)", Qualify(Table->Name, Field.Name));
        return Work;
    }
    sprintf(Work, "%s", Field.Name);
    return Work;
}

static void CreateCount(void)
{
    char Work[512];
    SetProc(strdup("Count"));
    memset(&Field, 0, sizeof(TYYField));
    SetFieldType(ftypeInt, 0);
    AddOutputField(strdup("NoOf"));
    Proc.isSql = 1;
    Proc.isSingle = 1;
    Proc.isStd = 1;
    LineState = stateCode;
    sprintf(Work, "select count(*) from %s\n", Table->Name);
    AddLine(strdup(Work));
    AddProc();
}

static void CreateDeleteAll(void)
{
    char Work[512];
    SetProc(strdup("DeleteAll"));
    Proc.isSql = 1;
    Proc.isStd = 1;
    LineState = stateCode;
    sprintf(Work, "delete from %s\n",
        Table->Name);
    AddLine(strdup(Work));
    AddProc();
}

static pchar Qualify(pchar TableName, pchar FieldName)
{
    static char Work[256];
    char Work1[256];
    Work1[0] = ':';
    strncpy(Work1 + 1, FieldName, sizeof(Work1) - 3);
    Work1[sizeof(Work1) - 1] = 0;
    strcat(Work1, ":");
    strupr(Work1);
    char* p = strstr((char*)OracleReservedWords, Work1);
    if (p == 0)
    {
        strncpy(Work, FieldName, sizeof(Work) - 1);
        Work[sizeof(Work) - 1] = 0;
    }
    else
    {
        strncpy(Work, TableName, sizeof(Work) - strlen(FieldName) - 2);
        Work[sizeof(Work) - strlen(FieldName) - 2] = 0;
        strcat(Work, ".");
        strcat(Work, FieldName);
    }
    return Work;
}

static void CreateDeleteOne(void)
{
    char Work[512];
    int32  i, first = 1;
    SetProc(strdup("DeleteOne"));
    Proc.isSql = 1;
    Proc.useKey = 1;
    Proc.isStd = 1;
    LineState = stateCode;
    sprintf(Work, "delete from %s\n", Table->Name);
    AddLine(strdup(Work));
    for (i = 0; i < Table->noFields; i++)
    {
        Field = Table->Fields[i]; Field.noConsts = 0;
        if (Field.isPrimaryKey)
        {
            if (Field.Alias) Field.Alias = strdup(Field.Alias);
            AddInputField(strdup(Field.Name));
            sprintf(Work, "%s %s = %s\n", first ? "where" : "and"
                , Qualify(Table->Name, Field.Name), InputConversion(Field));
            AddLine(strdup(Work));
            first = 0;
        }
    }
    AddProc();
}

static void CreateInsertUsingReturning(void)
{
    char Work[512];
    int32  i, first, hasReturn = 0;
    SetProc(strdup("Insert"));
    Proc.isSql = 1;
    Proc.useStd = 1;
    Proc.isStd = 1;
    LineState = stateCode;
    for (i = 0; i < Table->noFields; i++)
    {
        Field = Table->Fields[i];
        Field.noConsts = 0;
        if (Field.Alias)
            Field.Alias = strdup(Field.Alias);
        if (Field.Type == ftypeUserStamp
            || Field.Type == ftypeTimeStamp
            || Field.isSequence)
            Field.isOutput = hasReturn = 1;
        AddInputField(strdup(Field.Name));
    }
    sprintf(Work, "insert into %s (\n", Table->Name);
    AddLine(strdup(Work));
    for (i = 0, first = 1; i < Table->noFields; i++)
    {
        Field = Table->Fields[i];
        sprintf(Work, "%s%s\n", first ? "" : ",", Qualify(Table->Name, Field.Name));
        AddLine(strdup(Work));
        first = 0;
    }
    sprintf(Work, ") values (\n");
    AddLine(strdup(Work));
    for (i = 0, first = 1; i < Table->noFields; i++)
    {
        Field = Table->Fields[i];
        sprintf(Work, "%s%s\n", first ? "" : ",", InputConversion(Field, 1));
        AddLine(strdup(Work));
        first = 0;
    }
    if (hasReturn == 1)
    {
        AddLine(strdup(") returning\n"));
        for (i = 0, first = 1; i < Table->noFields; i++)
        {
            Field = Table->Fields[i];
            if (Field.Type == ftypeUserStamp
                || Field.Type == ftypeTimeStamp
                || Field.isSequence)
            {
                sprintf(Work, "%s%s\n", first ? "" : ",", OutputConversion(Field));
                AddLine(strdup(Work));
                first = 0;
            }
        }
        AddLine(strdup("into\n"));
        for (i = 0, first = 1; i < Table->noFields; i++)
        {
            Field = Table->Fields[i];
            if (Field.Type == ftypeUserStamp
                || Field.Type == ftypeTimeStamp
                || Field.isSequence)
            {
                sprintf(Work, "%s:%s\n", first ? "" : ",", Qualify(Table->Name, Field.Name));
                AddLine(strdup(Work));
                first = 0;
            }
        }
    }
    else
        AddLine(strdup(")\n"));
    AddProc();
}

static void CreateInsert(void)
{
    char Work[512];
    int32  i, first, done = 0;
    SetProc(strdup("Insert"));
    Proc.isSql = 1;
    Proc.useStd = 1;
    Proc.isStd = 1;
    LineState = stateCode;
    AddLine(strdup("BEGIN\n"));
    for (i = 0; i < Table->noFields; i++)
    {
        Field = Table->Fields[i]; Field.noConsts = 0;
        if (Field.Alias) Field.Alias = strdup(Field.Alias);
        AddInputField(strdup(Field.Name));
        done = 1;
        if (Field.Type == ftypeUserStamp
            || Field.Type == ftypeTimeStamp
            || Field.isSequence)
        {
            Field.isOutput = 1;
            if (Field.isSequence)
                sprintf(Work, "select %sSeq.NEXTVAL into :%s from dual;\n", Table->Name, Qualify(Table->Name, Field.Name));
            else if (Field.Type == ftypeUserStamp)
                sprintf(Work, "select SUBSTR(USER, 1, 8) into :%s from dual;\n", Qualify(Table->Name, Field.Name));
            else
                sprintf(Work, "select to_char(SYSDATE, 'YYYYMMDDHH24MISS')"
                    " into :%s from dual;\n", Qualify(Table->Name, Field.Name));
            AddLine(strdup(Work));
        }
    }
    sprintf(Work, "insert into %s (\n", Table->Name);
    AddLine(strdup(Work));
    for (i = 0, first = 1; i < Table->noFields; i++)
    {
        Field = Table->Fields[i]; Field.noConsts = 0;
        sprintf(Work, "%s%s\n", first ? "" : ",", Qualify(Table->Name, Field.Name));
        AddLine(strdup(Work));
        first = 0;
    }
    sprintf(Work, ") values (\n");
    AddLine(strdup(Work));
    for (i = 0, first = 1; i < Table->noFields; i++)
    {
        Field = Table->Fields[i]; Field.noConsts = 0;
        if (done == 0)
        {
            if (Field.Alias) Field.Alias = strdup(Field.Alias);
            AddInputField(strdup(Field.Name));
        }
        sprintf(Work, "%s%s\n", first ? "" : ",", InputConversion(Field));
        AddLine(strdup(Work));
        first = 0;
    }
    AddLine(strdup(");\n"));
    AddLine(strdup("END;\n"));
    AddProc();
}

static void CreateSelectAll(int Update)
{
    char Work[512];
    int32  i, first;
    if (Update)
        SetProc(strdup("SelectAllUpd"));
    else
        SetProc(strdup("SelectAll"));
    Proc.isSql = 1;
    Proc.isManyQuery = 1; /*(Table->hasImage == 1 || Table->hasLOB == 1) ? 0 :*/
    Proc.useStd = 1;
    Proc.isStd = 1;
    LineState = stateCode;
    sprintf(Work, "select\n");
    AddLine(strdup(Work));
    for (i = 0, first = 1; i < Table->noFields; i++)
    {
        Field = Table->Fields[i]; Field.noConsts = 0;
        if (Field.Alias) Field.Alias = strdup(Field.Alias);
        AddOutputField(strdup(Field.Name));
        sprintf(Work, "%s%s\n", first ? "" : ", ", OutputConversion(Field));
        AddLine(strdup(Work));
        first = 0;
    }
    sprintf(Work, "from %s\n", Table->Name);
    AddLine(strdup(Work));
    if (Update)
    {
        sprintf(Work, "for update\n");
        AddLine(strdup(Work));
    }
    AddProc();
}

static void CreateSelectPlus(int size)
{
    char Work[512];
    int32  i, first;
    SetProc(strdup("SelectPlus"));
    Proc.isSql = 1;
    Proc.isManyQuery = 1;
    Proc.isStd = 1;
    Proc.hasImage = Table->hasImage;
    Proc.hasLOB = Table->hasLOB;
    LineState = stateCode;
    sprintf(Work, "select\n");
    AddLine(strdup(Work));
    for (i = 0, first = 1; i < Table->noFields; i++)
    {
        Field = Table->Fields[i]; Field.noConsts = 0;
        if (Field.Alias) Field.Alias = strdup(Field.Alias);
        AddOutputField(strdup(Field.Name));
        sprintf(Work, "%s%s\n", first ? "" : ", ", OutputConversion(Field));
        AddLine(strdup(Work));
        first = 0;
    }
    sprintf(Work, "from %s $UPlusU\n", Table->Name);
    AddLine(strdup(Work));
    SetFieldType(ftypeDynamic, size < 32 ? 32 : size);
    AddDynamicField(strdup("UPlusU"));
    AddProc();
}

static void CreateSelectOne(int Update)
{
    char Work[512];
    int32  i, first;
    if (Update)
        SetProc(strdup("SelectOneUpd"));
    else
        SetProc(strdup("SelectOne"));
    Proc.isSql = 1;
    Proc.isSingle = 1;
    Proc.useStd = 1;
    Proc.isStd = 1;
    LineState = stateCode;
    sprintf(Work, "select\n");
    AddLine(strdup(Work));
    for (i = 0; i < Table->noFields; i++)
    {
        Field = Table->Fields[i]; Field.noConsts = 0;
        if (Field.Alias) Field.Alias = strdup(Field.Alias);
        if (!Field.isPrimaryKey)
            AddOutputField(strdup(Field.Name));
        else
            AddInputField(strdup(Field.Name));
    }
    for (i = 0, first = 1; i < Table->noFields; i++)
    {
        Field = Table->Fields[i]; Field.noConsts = 0;
        if (!Field.isPrimaryKey)
        {
            sprintf(Work, "%s%s\n", first ? "" : ", ", OutputConversion(Field));
            AddLine(strdup(Work));
            first = 0;
        }
    }
    sprintf(Work, "from %s\n", Table->Name);
    AddLine(strdup(Work));
    for (i = 0, first = 1; i < Table->noFields; i++)
    {
        Field = Table->Fields[i]; Field.noConsts = 0;
        if (Field.isPrimaryKey)
        {
            sprintf(Work, "%s %s = %s\n", first ? "where" : "and"
                , Qualify(Table->Name, Field.Name), InputConversion(Field));
            AddLine(strdup(Work));
            first = 0;
        }
    }
    if (Update)
    {
        sprintf(Work, "for update\n");
        AddLine(strdup(Work));
    }
    AddProc();
}

static void CreateUpdateUsingReturning(void)
{
    char Work[512];
    int32  i, first = 1, hasReturn = 0;
    SetProc(strdup("Update"));
    Proc.isSql = 1;
    Proc.useStd = 1;
    Proc.isStd = 1;
    LineState = stateCode;
    for (i = 0; i < Table->noFields; i++)
    {
        Field = Table->Fields[i];
        Field.noConsts = 0;
        if (Field.Alias)
            Field.Alias = strdup(Field.Alias);
        if (Field.Type == ftypeUserStamp
            || Field.Type == ftypeTimeStamp)
            Field.isOutput = hasReturn = 1;
        AddInputField(strdup(Field.Name));
    }
    sprintf(Work, "update %s set\n", Table->Name);
    AddLine(strdup(Work));
    for (i = 0, first = 1; i < Table->noFields; i++)
    {
        Field = Table->Fields[i]; Field.noConsts = 0;
        if (!Field.isPrimaryKey)
        {
            sprintf(Work, "%s%s = %s\n", first ? "" : ", ",
                Qualify(Table->Name, Field.Name), InputConversion(Field, 2));
            AddLine(strdup(Work));
            first = 0;
        }
    }
    for (i = 0, first = 1; i < Table->noFields; i++)
    {
        Field = Table->Fields[i]; Field.noConsts = 0;
        if (Field.isPrimaryKey)
        {
            sprintf(Work, "%s %s = %s\n", first ? "where" : "and",
                Qualify(Table->Name, Field.Name), InputConversion(Field));
            AddLine(strdup(Work));
            first = 0;
        }
    }
    if (hasReturn == 1)
    {
        AddLine(strdup(" returning\n"));
        for (i = 0, first = 1; i < Table->noFields; i++)
        {
            Field = Table->Fields[i];
            if (Field.Type == ftypeUserStamp
                || Field.Type == ftypeTimeStamp)
            {
                sprintf(Work, "%s%s\n", first ? "" : ",", OutputConversion(Field));
                AddLine(strdup(Work));
                first = 0;
            }
        }
        AddLine(strdup("into\n"));
        for (i = 0, first = 1; i < Table->noFields; i++)
        {
            Field = Table->Fields[i];
            if (Field.Type == ftypeUserStamp
                || Field.Type == ftypeTimeStamp)
            {
                sprintf(Work, "%s:%s\n", first ? "" : ",", Qualify(Table->Name, Field.Name));
                AddLine(strdup(Work));
                first = 0;
            }
        }
    }
    AddProc();
}

static void CreateUpdate(void)
{
    char Work[512];
    int32  i, first = 1, done = 0;
    SetProc(strdup("Update"));
    Proc.isSql = 1;
    Proc.useStd = 1;
    Proc.isStd = 1;
    LineState = stateCode;
    AddLine(strdup("BEGIN\n"));
    for (i = 0; i < Table->noFields; i++)
    {
        Field = Table->Fields[i]; Field.noConsts = 0;
        if (Field.Alias) Field.Alias = strdup(Field.Alias);
        AddInputField(strdup(Field.Name));
        done = 1;
        if (Field.Type == ftypeUserStamp
            || Field.Type == ftypeTimeStamp)
        {
            Field.isOutput = 1;
            if (Field.Type == ftypeUserStamp)
                sprintf(Work, "select SUBSTR(USER, 1, 8) into :%s from dual;\n", Qualify(Table->Name, Field.Name));
            else
                sprintf(Work, "select to_char(SYSDATE, 'YYYYMMDDHH24MISS')"
                    " into :%s from dual;\n", Qualify(Table->Name, Field.Name));
            AddLine(strdup(Work));
        }
    }
    if (first == 0)
        AddLine(strdup("from dual;\n"));
    sprintf(Work, "update %s set\n", Table->Name);
    AddLine(strdup(Work));
    if (done == 0)
    {
        for (i = 0; i < Table->noFields; i++)
        {
            Field = Table->Fields[i]; Field.noConsts = 0;
            if (Field.Alias) Field.Alias = strdup(Field.Alias);
            AddInputField(strdup(Field.Name));
        }
    }
    for (i = 0, first = 1; i < Table->noFields; i++)
    {
        Field = Table->Fields[i]; Field.noConsts = 0;
        if (!Field.isPrimaryKey)
        {
            sprintf(Work, "%s%s = %s\n", first ? "" : ", ",
                Qualify(Table->Name, Field.Name), InputConversion(Field));
            AddLine(strdup(Work));
            first = 0;
        }
    }
    for (i = 0, first = 1; i < Table->noFields; i++)
    {
        Field = Table->Fields[i]; Field.noConsts = 0;
        if (Field.isPrimaryKey)
        {
            sprintf(Work, "%s %s = %s\n", first ? "where" : "and",
                Qualify(Table->Name, Field.Name), InputConversion(Field));
            AddLine(strdup(Work));
            first = 0;
        }
    }
    AddLine(strdup(";\nEND;\n"));
    AddProc();
}

static void CreateExists(void)
{
    char Work[512];
    int32  i, first = 1;
    SetProc(strdup("Exists"));
    memset(&Field, 0, sizeof(TYYField));
    SetFieldType(ftypeInt, 0);
    AddOutputField(strdup("Count"));
    Proc.isSql = 1;
    Proc.isSingle = 1;
    Proc.isStd = 1;
    LineState = stateCode;
    sprintf(Work, "select count(*) from %s\n", Table->Name);
    AddLine(strdup(Work));
    for (i = 0; i < Table->noFields; i++)
    {
        Field = Table->Fields[i]; Field.noConsts = 0;
        if (Field.isPrimaryKey)
        {
            if (Field.Alias) Field.Alias = strdup(Field.Alias);
            AddInputField(strdup(Field.Name));
            sprintf(Work, "%s %s = %s\n", first ? "where" : "and"
                , Qualify(Table->Name, Field.Name)
                , InputConversion(Field));
            AddLine(strdup(Work));
            first = 0;
        }
    }
    AddProc();
}



