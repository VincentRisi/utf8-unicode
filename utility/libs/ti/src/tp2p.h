// file       : tp2p.h
// programmer : v.risi
// description: communications handler base class

#ifndef _TP2P_H_
#define _TP2P_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_TP2P_H_);

CLASSDEF(P2P);
class tP2P
{
public:
  virtual void Open() = 0;
  virtual void Close() = 0;
  virtual unsigned long ReadLength() = 0;
  virtual void WriteLength(unsigned long aLength) = 0;
  virtual void Read(void * aBuffer, unsigned long aBufferLen) = 0;
  virtual void Write(void * aBuffer, unsigned long aBufferLen) = 0;
};

#endif


