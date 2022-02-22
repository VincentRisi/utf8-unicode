#ifndef _LINGHEAD_H_
#define _LINGHEAD_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_LINGHEAD_H_);

typedef char tChar;
typedef tChar* pChar;
typedef double tFloat;
typedef tFloat* pFloat;
typedef long tInt;
typedef tInt* pInt;
typedef short tSmallint;
typedef tSmallint* pSmallint;
typedef signed char tTinyint;
typedef tTinyint* pTinyint;
typedef unsigned char tByte;
typedef tByte* pByte;
typedef unsigned tWord;
typedef tWord* pWord;

typedef enum
{
  opCode  = 0x00, opEnd      = 0x01, opExit  = 0x02, opNop   = 0x0F,
  opAdd   = 0x10, opSub      = 0x11, opMul   = 0x12, opDiv   = 0x13,
  opAnd   = 0x20, opOr       = 0x21, opNot   = 0x22,
  opAssign= 0x30, opUMin     = 0x31,
  opEq    = 0x40, opGe       = 0x41, opGt    = 0x42,
  opLe    = 0x43, opLt       = 0x44, opNe    = 0x45,
  opPop   = 0x4F,
  opBR    = 0x50, opBZ       = 0x51, opBN    = 0x52,
  opCall  = 0x60, opRun      = 0x61, opEach  = 0x62,
  opCommit= 0x63, opRollback = 0x64,
  opLit   = 0x70, opVar      = 0x71, opRef   = 0x72
}eOpCode;

typedef enum
{
  tagPipe='P',     tagMemory='M',    tagAlias='A',
  tagUndef='U'
} eVarTag;

typedef enum
{
  typeChar='C',    typeFloat='F',    typeInt='I',     typeSmallint='S',
  typeTinyint='T', typeAlias='A',    typeRef='R',
  typeUndef='U'
} eVarType;

#ifdef M_OS2
# pragma pack(1)
#endif

typedef struct
{
  tInt  Signature;
  tByte ProcNameLength;
  tByte PipeNameLength;
  tSmallint NoVars;
  tSmallint NoUses;
  tSmallint PipeSize;
  tSmallint WorkSize;
  tSmallint CodeSize;
  tSmallint StackSize;
  tChar Filler[14];     /*  reserved for expansion without recompilation */
} tLingHeader, *pLingHeader;

typedef struct
{
  eVarTag  Tag;
  eVarType Type;
  tSmallint Index;      /*  Alias point upwards Uses to Var */
  tSmallint Length;
  tSmallint Offset;
  tByte Flag;
  pChar Name;
} tVar, *pVar;

typedef struct
{
  pChar Name;
  pChar ProcName;
  pChar PipeName;
  tSmallint NoVars;
  tSmallint PipeSize;
  pVar Vars;
  int PrimNo;
} tUses, *pUses;

typedef struct
{
  eVarType Type;
  union
  {
    pChar     C;
    tFloat    F;
    tInt      I;
    tSmallint S;
    tTinyint  T;
  } Var;
} tStack,* pStack;

typedef struct tStackCB
{
  int Index;
  pStack Stack;
} tStackCB,* pStackCB;

typedef struct LingoCB
{
  tLingHeader LH;
  pChar FileName;
  pChar ProcName;
  pChar PipeName;
  pVar  Vars;
  pUses Uses;
  pChar Pipe;
  pChar Work;
  pChar Code;
  pStackCB StackCB;
  int LogFile;
  int JournalFile;
  tSmallint NoAllocVars;
  tSmallint NoAllocUses;
  tSmallint AllocCodeSize;
} tLingoCB, *pLingoCB;

typedef struct
{
  tByte Opcode;
  tByte fill;
  tSmallint Value;
} tOpcodeValue, *pOpcodeValue;

typedef struct
{
  tByte Opcode;
  tByte Count;
  tSmallint Value;
} tOpcodeCountValue, *pOpcodeCountValue;

typedef struct
{
  tByte Opcode;
  tByte Literal;
} tOpcodeLiteral, *pOpcodeLiteral;

extern char* LogId;
#error PLEASE Speak to simon 20-09-2000
//extern int   LogFile;

#ifdef M_OS2
# pragma pack()
#endif

#endif
