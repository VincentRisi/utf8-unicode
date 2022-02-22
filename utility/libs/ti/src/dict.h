#ifndef _DICT_H_
#define _DICT_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_DICT_H_);

typedef struct
{
   char* Name;
   int   No;
} tDictEntry,* pDictEntry;

typedef struct
{
   int         No;
   int         Size;
   tDictEntry* Elements;
   tDictEntry* Inverted;
} tDictCB,* pDictCB;

pDictCB DictInit(void);
int DictInsert(pDictCB pcb, char *entry);
int DictLookupNo(pDictCB pcb, char *entry);
char *DictLookupName(pDictCB pcb, int no);
char *DictName(pDictCB pcb, int no);
int DictNo(pDictCB pcb, int no);
int DictNoUsed(pDictCB pcb);
int DictDone(pDictCB pcb);

#endif

