#ifndef _NEDGEN_H_
#define _NEDGEN_H_
#include "machine.h"

// NB - Do not change the gaurd NEDGEN to POPGEN (It is required as _NEDGEN_H_)
// This header is used by the DLL and Server code generated using
// the IDL compiler with the PopGen generator. It implements the
// inline Swap functions

#if !defined(M_AIX)
#define NGSwapMacro(a, b) (a) ^= (b) ^= (a) ^= (b)
#endif

inline void SwapBytes(double &a)
{
#if !defined(M_AIX)
    char *p = (char *)&a;
  NGSwapMacro(p[0], p[7]);
  NGSwapMacro(p[1], p[6]);
  NGSwapMacro(p[2], p[5]);
  NGSwapMacro(p[3], p[4]);
#endif
}

inline void SwapBytes(float &a)
{
#if !defined(M_AIX)
    char *p = (char *)&a;
  NGSwapMacro(p[0], p[3]);
  NGSwapMacro(p[1], p[2]);
#endif
}

inline void SwapBytes(long &a)
{
#if !defined(M_AIX)
    char *p = (char *)&a;
  NGSwapMacro(p[0], p[3]);
  NGSwapMacro(p[1], p[2]);
#endif
}

inline void SwapBytes(int32 &a)
{
#ifndef M_AIX
    char *p = (char *)&a;
  NGSwapMacro(p[0], p[3]);
  NGSwapMacro(p[1], p[2]);
#endif
}

inline void SwapBytes(short &a)
{
#if !defined(M_AIX)
    char *p = (char *)&a;
  NGSwapMacro(p[0], p[1]);
#endif
}

inline void TrimTrailingBlanks(char *data, int length)
{
    for (int i = length - 1; i >= 0 && (data[i] == ' ' || data[i] == 0); i--)
    data[i] = 0;
}

#if !defined(M_AIX)
#undef NGSwapMacro
#endif

#endif
