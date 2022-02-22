#ifndef _LHCBEXI_H_
#define _LHCBEXI_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_LHCBEXI_H_);

#include "linghead.h"

tSmallint LhLoad(pLingoCB LingoCB, char *buffer);  /*  LOAD LING from BUFFER */
pLingoCB  LhRead(pChar FileName);                  /*  Open and read existing LING */
tSmallint LhClose(pLingoCB LingoCB);               /*  Free up LING (use if not writing) */
pChar     LhGetProcName(pLingoCB LingoCB);
pChar     LhGetPipeName(pLingoCB LingoCB);
tSmallint LhGetNoVars(pLingoCB LingoCB);
pVar      LhGetVar(pLingoCB LingoCB, tSmallint No);
tSmallint LhFindVar(pLingoCB LingoCB, pChar Name);
tSmallint LhFindActualVar(pLingoCB LingoCB, pChar Name);
tChar     LhVarType(pLingoCB LingoCB, tSmallint index);
tSmallint LhFindPipe(pLingoCB LingoCB, pChar Name);
pChar     LhVariableName(pLingoCB LingoCB, tSmallint no);
tSmallint LhVariableLength(pLingoCB LingoCB, tSmallint no);
eVarTag   LhVariableTag(pLingoCB LingoCB, tSmallint no);
eVarType  LhVariableType(pLingoCB LingoCB, tSmallint no);
tSmallint LhVariableOffset(pLingoCB LingoCB, tSmallint no);
pChar     LhUsesProcName(pLingoCB LingoCB, tSmallint no);

#endif
