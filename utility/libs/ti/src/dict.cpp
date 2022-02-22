#include "versions.h"
#define _VERSION_NO "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
COMPILED_VERSION(_VERSION_NO);

#include "machine.h"

#include <string.h>
#include <stdlib.h>

#ifdef M_OS2
#include <search.h>
#else
#include "xstring.h"
#endif

#include "dict.h"

typedef int (*fptr)(const void*, const void*);

pDictCB DictInit()
{
  return (pDictCB) calloc(sizeof(tDictCB),1);
}

static int SearchComp(const char *p1, const pDictEntry p2)
{
  return stricmp(p1, p2->Name);
}

static int SortComp(const pDictEntry p1, const pDictEntry p2)
{
  return stricmp(p1->Name, p2->Name);
}

int DictInsert(pDictCB pcb, char *entry)
{
  pDictEntry pe=0;
  int i, n;

  if (pcb->No > 0)
    pe = (pDictEntry) bsearch(entry, pcb->Elements, pcb->No,
                    sizeof(tDictEntry), (fptr)SearchComp);
  if (pe) return pe->No;
  n = pcb->No++;
  if (pcb->No > pcb->Size)
  {
    pcb->Size += 16;
    pcb->Elements = (pDictEntry) realloc(pcb->Elements, sizeof(tDictEntry)*pcb->Size);
    pcb->Inverted = (pDictEntry) realloc(pcb->Inverted, sizeof(tDictEntry)*pcb->Size);
  }
  pcb->Elements[n].Name = strdup(entry);
  pcb->Inverted[n].Name = pcb->Elements[n].Name;
  pcb->Elements[n].No   = n;
  qsort(pcb->Elements, pcb->No, sizeof(tDictEntry), (fptr)SortComp);
  for (i=0; i<pcb->No; i++)
     pcb->Inverted[pcb->Elements[i].No].No = i;
  return n;
}

int DictLookupNo(pDictCB pcb, char *entry)
{
  pDictEntry pe=0;

  if (pcb->No > 0)
    pe = (pDictEntry) bsearch(entry, pcb->Elements, pcb->No,
                    sizeof(tDictEntry), (fptr)SearchComp);
  if (pe) return pe->No;
  return -1;
}

char *DictLookupName(pDictCB pcb, int no)
{
  if (no < 0 || no >= pcb->No) return 0;
  return pcb->Inverted[no].Name;
}

char *DictName(pDictCB pcb, int no)
{
  if (no < 0 || no >= pcb->No) return 0;
  return pcb->Elements[no].Name;
}

int DictNo(pDictCB pcb, int no)
{
  if (no < 0 || no >= pcb->No) return -1;
  return pcb->Inverted[no].No;
}

int DictNoUsed(pDictCB pcb)
{
  return pcb->No;
}

int DictDone(pDictCB pcb)
{
  int n;

  for (n=0; n<pcb->No; n++)
  {
    free(pcb->Elements[n].Name);
  }
  if (pcb->Elements)
    free(pcb->Elements);
  if (pcb->Inverted)
    free(pcb->Inverted);
  free(pcb);
  return n;
}


