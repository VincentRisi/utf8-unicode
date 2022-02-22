// list.hpp
// template based dynamic array
// barry fortune
// july 94

#ifndef _LIST_HPP_
#define _LIST_HPP_

#include <assert.h>

// usage:
//   T* p = 0;
//   int ind = 0;
//   T rec;
//
//   Addlist(p, ind, rec, DELTA);
//
//   delete[] p;
//
//   NB: the delta parameter must be kept constant (per list)
//   NB: the list owns the record inserted into it, hence the class T
//       must not delete any member pointers on destruction
//   NB: if type T does a -deep- copy on operator= this approach
//       may not perform well, due to excessive construction/destruction
//       One way around this is for T to be a pointer to another type
//   Note: COUNTER must be an integer based type eg size_t, int, short etc

const size_t DELTA = 10;
template <class T, class COUNTER>
void Addlist(T*& start, COUNTER& index, const T& rec, const size_t delta)
{
  assert((start == 0) == (index == 0));

  if (index % delta == 0)
  {
    T* temp = new T[index + delta];
    if (index)
    {
      for (int i = 0; i < index; i++)
        temp[i] = start[i];
      delete[] start;
    }
    start = temp;
  }
  start[index++] = rec;
}

#endif

