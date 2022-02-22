#ifndef _MFORMATS_H_
#define _MFORMATS_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_MFORMATS_H_);

typedef struct
    {
    ushort   Dno;
    ushort   NoTrans;
    ushort   NoPVars;
    ushort   PipeDno;
    ushort   ProcDno;
    ushort   Size;
    ushort   FirstPVar;
    ushort   PipeSlot;
    } tSysTab, *pSysTab;

typedef struct
    {
    ushort   Dno;
    ushort   System;
    char     Type;
    ushort   Length;
    } tPVarTab, *pPVarTab;

typedef struct
    {
    ushort   Dno;
    ushort   System;
    ushort   NoVars;
    ushort   NoOccurs;
    ushort   FirstVar;
    } tTranTab, *pTranTab;

typedef enum {NoRule='N', NotZero='Z', NotBlank='B'} OCCRULE;

typedef struct
    {
    ushort   Dno;
    ushort   Tran;
    char     Type;
    ushort   Length;
    ushort   InsLength;
    short    Decs;
    short    Offset;
    ushort   Occurs;
    ushort   Every;
    OCCRULE  Rule;
    short    Vno;
    } tVarTab, *pVarTab;

typedef struct
    {
    ushort      NoDict;
    char**      Dictnames;
    ushort      NoSystems;
    pSysTab     Systems;
    ushort      NoPVars;
    pPVarTab    PVars;
    ushort      NoTrans;
    pTranTab    Trans;
    ushort      NoVars;
    pVarTab     Vars;
    } tMFCB, *pMFCB;

typedef struct
    {
    ushort      NoTrans;
    char**      Trans;
    } tTrList, *pTrList;

pMFCB   LoadInFormats(char *BinFileName, char *MFormatsDir);
void    FreeUpFormats(pMFCB MFCB);
ushort  LookupVar(pMFCB MFCB, ushort Tran, ushort Var);

pTrList LoadTrans(char *BinFileName);
void    FreeUpTrans(pTrList TrList);

#endif
