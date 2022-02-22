#ifndef __TMQUEUE_H__
#define __TMQUEUE_H__ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(__TMQUEUE_H__);
//
//  Program Name  : tMqueue  Class definition for and AIX of a queue in memory
//  Who           : Suriya Sena
//  Date          : 16 June 1994
//
//                  Abstract Data Type of a queue
//


#include "machine.h"
#include "tqueue.h"
#include "xcept.h"
#include "errnomsg.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>



class xMqueue : public xCept
{

   private :
      int  unixErrorNo;       // error number from system call
   public :
      xMqueue(char* aFile, const int aLine, int aErrno, char * aInfo = 0 ) ;

};


#define XMqueue(errno,mtext) xMqueue(__FILE__,__LINE__,errno,mtext )

class tMqueue : public tqueue {

   private :
      key_t m_qid;        // internal queue id
      bool  m_owner;      // flag indicating if this process is owner of QUEUE
      int   m_nbytes;     // maximum message size
      int   m_size;       // size of message
      int   m_mode;       // IPC_WAIT or NOT
      struct  msgbuf * pmbuf;


      // LOCK RELEASE DECLARATIONS
      // implemented via Permissions on the mqueue
      // Only the creator of the Queue may lock it , there is only one creator so
      // there is no need for semaphores unless the process forks!!
      // beware of this

      bool  m_locked;     // flag indicating if this process has locked queue
      unsigned short m_oldmode;   // previous permisions of queue used to release lock
      int   qchmod ( unsigned short aMode );

   public :

       friend  ostream & operator<< (  ostream & os, tMqueue & a );


       tMqueue(
                        const int   aNbytes,  // size of message in bytes
                        const short aKey,     // identifier to this queue
                        const int aMode
                      );
                      // queue constructor

       ~tMqueue ();
                      // queue destructor

       bool Enqueue( const pchar aData,
                             const int aKey,
                             const int aNbytes = 0
                           ) ;
                      // add item to end of queue


       bool Dequeue(
                             pchar  aData,
                             int & aKey,
                             const int aNbytes = 0
                           );

       bool DequeueByKey(
                             pchar  aData,
                             const int  aKey,
                             const int aNbytes = 0
                           );


       int  GetQSize();
       int  GetMaxQSize();    // Maxium size of a message queue in bytes
       bool SetMode( int mode);  // alter the R/W mode on q ie. Blocking NonBlocking



       //
       // these two methods are only valid for the process who owns the queue
       // used for housekeeping
       bool Lock();
       bool Release();

      	
};

#endif

