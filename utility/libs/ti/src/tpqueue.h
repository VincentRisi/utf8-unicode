//
//  Program Name  : tPqueue  Class definition for windows and AIX
//  Who           : Suriya Sena
//  Date          : 11 May 1994
//
//  Description   : provides an implementation for a persistent queue
//                  Abstract Data Type, implemented as a double linked list
//                  Features Include
//
//                  * User defined message size
//                  * User defined key range
//                  * Automatic garbage collection with backup !!!
// class tLink
//   class tData
//   class tControl
// class tPqueue: public tqueue

#ifndef __TPQUEUE_H__
#define __TPQUEUE_H__ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(__TPQUEUE_H__);

#include "ti.h"

#include <stdlib.h>
#ifdef M_AIX
#include "tsem.h"
#endif
#include "tqueue.h"

#define CONTROLBLK  0L
#define RESERVEDKEY 555
#define EARTH -1L

#ifdef M_AIX
#define FLAGS ios::in|ios::out
#define PATHSEP "/"
#else
#define FLAGS ios::in|ios::out|ios::binary
#define PATHSEP "\\"
#endif

class tLink
{
public:
  virtual ~tLink() {};

  virtual  ostream& Write(
     ostream& os) = 0;

  virtual  istream& Read(
     istream& is) = 0;

  virtual long GetSize() = 0;
};

class tData: public tLink
{
public:
   struct
   {
      int  MsgKey;   // message group id
      long Next,     // offset to next link
           Prev;     // offset to previous link
   } hdr;

   char *ptext;

   int GetNbytes()    { return _nbytes; }
   long GetSize()     { return sizeof(hdr) + (sizeof(char) * _nbytes); }
   tData(int aNbytes);
   ~tData();
    ostream& Write( ostream& aOs);
    istream& Read( istream& aIs);

private:
   int _nbytes;         // size of memory pointed to by ptext
};

class tControl: public tLink
{
public:
  struct
  {
    long FreeList,      // offset to start of freelist
         Head,          // offset to start of list in use
         Tail;          // offset to tail of queue

    int  CountUsed,     // number or Links in use
         CountFree,     // number of Free Links
         Nbytes,        // size of message
         Nkeys;         // number of keys that queue supports
  } hdr;

  struct Keys
  {
    long Head;          // offset to start of this sub list
    int  CountUsed;

    Keys(): Head(EARTH), CountUsed(0) {}
  } *pKey;

  int GetNkeys()      { return _nkeys; }
  long GetSize()      { return sizeof(hdr) + (sizeof(Keys) * _nkeys); }
  tControl(int aNkeys);
  ~tControl();
   ostream& Write( ostream& aOs);
   istream& Read( istream& aIs);

private:
  int _nkeys;
};

class tPqueue: public tqueue
{
public:
  tPqueue(
    const pchar aPath,    // directory to hold queue
    int         aNbytes,  // max size of message in bytes
    int         aNkeys,   // no of keys queue can hold
    short       aKey,     // identifier to this queue
    bool        aGarbageCollect = true);
  ~tPqueue();

  // add item to end of queue
  bool Enqueue(
    const pchar aData,
    const int aKey,
    const int aNbytes = 0);

  bool DequeueByKey(
    pchar  aData,
    const int aKey,
    const int aNbytes = 0);

  bool Dequeue(
    pchar  aData,
    int & aKey,
    const int aNbytes = 0);

  bool View(
    pchar  aData,
    int & aKey,
    const int aNbytes = 0);

  bool ViewByKey(
    pchar  aData,
    const int aKey,
    const int aNbytes = 0);

  bool Alter(
    const pchar aData);

  // alter item at head of subqueue identified by aKey
  bool AlterByKey(
    pchar  aData,
    const int aKey,
    const int aNbytes = 0);

  // get queue size for entire queue
  int GetQSize();

  // get queue size for paricular sub queue
  int GetQSize(const int aKey);

  bool Good();

  // returns text of last error
  pchar GetError();

  // DEBUG FUNCTIONS

  void PrintCtrlBlk();
  void Dump(
    pchar text = "");

private:
  tControl _CtrlBlk;        // Maintains data about the list
  // fstream  _fs;          // File descriptor
  int   _key;               // identifier to queue used to build filename
  bool  _GarbageCollect;    // Remove free items from file YES/NO
  char  _filename[100];
  char  _path[100];
  int   m_size;             // size of message
  bool  m_exception;        // state of queue good or bad
  int   m_ecode;            // error code index to error table

  static char  * m_errortxt []; // error messages

#ifdef M_AIX
  tSemaphore _sem;     // binary semaphore
#endif

  bool GetLink( long offset, tLink & p);
  bool SetLink(long offset, tLink & p);
  void DeallocLink( long Np );
  long AllocLink();

  // Resize file to reduce space wastage
  void GarbageCollect();
  long KeySearch(
    const long aStart,
    const int akey);
};

#endif

