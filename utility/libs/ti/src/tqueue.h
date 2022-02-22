//
//  Program Name  : tqueue  ABSTRACT Class definition for windows and AIX
//  Who           : Suriya Sena
//  Date          : 16 June 1994
//
//                  Abstract Data Type of a queue

#ifndef __TQUEUE_H__
#define __TQUEUE_H__ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(__TQUEUE_H__);

#include "machine.h"

#define QUEUEALL -1

class tqueue
{
public :
   enum iomode { NoCreate=1, Private, NonBlocking };

   virtual ~tqueue () {};

   // add item to end of queue
   virtual bool Enqueue(
     const pchar aData,
     const int aKey,
     const int aNbytes) = 0;

   virtual bool Dequeue(
     pchar aData,
     int & aKey,
     const int aNbytes) = 0;


  virtual bool DequeueByKey(
    pchar  aData,
    const int aKey,
    const int aNbytes) = 0;

  virtual int GetQSize() = 0;
};

#endif

