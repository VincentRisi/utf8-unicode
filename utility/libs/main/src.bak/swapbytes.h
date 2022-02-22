#ifndef swapbytesH
#define swapbytesH
#include "machine.h"

// This header is used by the DLL and Server code generated using
// the IDL compiler with the PopGen generator. It implements the
// inline Swap and Copy functions

// Simon says: 
// inline void BBD_SWAP_MACRO(char &a, char &b) { a ^= b ^= a ^= b; }

inline void BBD_SWAP_MACRO(char &a, char &b) { char t; t=a; a= b; b=t; }
inline void SwapBytes(char* p, int b)
{
  int f = 0;
  while (f < b)
    BBD_SWAP_MACRO(p[f++], p[--b]);
}

const int32 doSwap=0x03020100;
const bool mustDoSwap = (*(char*)&doSwap == 0);
template <class T>
inline void SwapBytes(T &a)
{
  if (mustDoSwap)
    SwapBytes((char*)&a, sizeof(a));
}

inline void TrimTrailingBlanks(char *data, int length)
{
  for (int i=length-1; i >= 0 && (data[i] == ' ' || data[i] == 0); i--)
    data[i] = 0;
}

#ifdef M_AIX
inline void TrimTrailingBlanks(int8 *data, int length)
{
  for (int i=length-1; i >= 0 && (data[i] == ' ' || data[i] == 0); i--)
    data[i] = 0;
}
#endif


#endif
