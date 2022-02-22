#ifndef _LIST_H_
#define _LIST_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_LIST_H_);

// Index and Start initialized to 0
// Start is a pointer to type T (a struct so that assignment is defined)
// Data is of type T
#define DELTA 10

#define Addlist(pStart, Type, Data, Index) Addlist1(pStart, Type, Data, Index, DELTA)
#define Addlist1(pStart, Type, Data, Index, Delta)                     \
  do {                                                                 \
    if (Index % Delta == 0)                                            \
      pStart = (Type*)realloc(pStart, sizeof(Data) * (Index + Delta)); \
    pStart[Index++] = Data;                                            \
  } while (0)

template <class TYPE, class INDEX>
void SnipAddList(TYPE * &List, INDEX & Index, const TYPE & Rec, const INDEX Delta)
{
    if ((List == 0) != (Index == 0))
    {
        printf("Addlist Error List %p out of sync with Index %d", List, Index);
        if (List) free(List);
        List = 0;
        Index = 0;
        return;
    }
    if (Index % Delta == 0)
    {
        TYPE* Resized = (TYPE*)realloc(List, sizeof(Rec) * (Index + Delta));
        if (Resized == 0)
        {
            printf("Realloc ran out of memory");
            if (List) free(List);
            List = 0;
            Index = 0;
            return;
        }
        List = Resized;
    }
    List[Index++] = Rec;
}
#endif
