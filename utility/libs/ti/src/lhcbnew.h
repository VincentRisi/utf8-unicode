#ifndef _LHCBNEW_H_
#define _LHCBNEW_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_LHCBNEW_H_);

#include "linghead.h"

pLingoCB  LhCreate(pChar FileName);    /*  Setup new LING */
tSmallint LhWrite(pLingoCB LingoCB);   /*  Write and close existing or new LING */
tSmallint LhWriteCHeader(pLingoCB LingoCB);
tSmallint LhSetProcName(pLingoCB LingoCB, pChar ProcName);
tSmallint LhSetPipeName(pLingoCB LingoCB, pChar PipeName);
tSmallint LhAddVar(pLingoCB LingoCB, eVarTag Tag, eVarType Type,
                   pChar Name, tByte Flag, tSmallint Length);
tSmallint LhAddAlias(pLingoCB LingoCB, pChar Name, pChar AliasFor);
tSmallint LhAddUses(pLingoCB LingoCB, pChar Name);
tSmallint LhAddOp(pLingoCB LingoCB, pChar Instruction,
                  tSmallint Length, int verbose);

#endif
