
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
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

/* Line 1676 of yacc.c  */
#line 39 "C:/vlab/utility/dbportal/code/sqldef.y"

    int   i;
    char* s;



/* Line 1676 of yacc.c  */
#line 255 "C:/vlab/utility/dbportal/code/sqldef.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


