#include "versions.h"
#define _VERSION_NO "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
COMPILED_VERSION(_VERSION_NO);

//  tmqueue.c
//  class tmqueue: Unix Message queue class
//  Suriya Sena
//  16 May 1994

#include "ti.h"
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

using namespace std;

#include "tmqueue.h"


//
// Constructor
//

tMqueue::tMqueue(
                  const int   aNbytes,
                  const short aKey,
                  const int aMode
                ) : m_nbytes( aNbytes),
                                         m_locked(false)
{
  char *p;
  int  IpcFlags;

  if ( aMode & NoCreate )
  {
     p="Constructor: msgget failed trying to attach to message queue";
     IpcFlags=0;
     m_owner = false;
  }
  else     // create queue exclusivly ie. if it exist throw error
  {
     p="Constructor : msgget failed trying to create message queue";
     IpcFlags=IPC_CREAT|IPC_EXCL|0660;
     m_owner = true;
  }

  if ( aMode & Private )
     IpcFlags|=IPC_PRIVATE;


  if ((m_qid = msgget( aKey,IpcFlags)) < 0 )
     throw XMqueue(errno,p);


  if (  aMode & NonBlocking )
     m_mode = IPC_NOWAIT;
  else
     m_mode = 0;

  pmbuf = (struct msgbuf * ) new  char [ (sizeof(msgbuf ) -1 ) + m_nbytes];
}


tMqueue::~tMqueue ()
{
   delete [] pmbuf;

cerr  << "destructor time " << endl;

   if ( m_owner )   // if I created the Queue
     msgctl ( m_qid , IPC_RMID , 0 );
}

bool tMqueue::Enqueue(
                       const pchar aData,
                       const int aKey,
                       const int aNbytes
                     )
{
   bool    rc;

   if ( (m_size = (aNbytes == 0 ? m_nbytes : aNbytes))  > m_nbytes )
       throw XMqueue(errno,"Enqueue : bytes requested > declared message size");


   pmbuf->mtype = aKey;
   memcpy (pmbuf->mtext, aData, m_size );


   if ( msgsnd ( m_qid, pmbuf , m_nbytes, m_mode ) <  0 )
      if ( errno == EAGAIN )   // Resource was busy most likely Q is Full
          rc = false;
      else
        throw XMqueue(errno,"Enqueue : msgsnd failed");
   else
      rc = true;

   return(rc);
}



bool tMqueue::Dequeue(
                       pchar  aData,
                       int   & aKey,
                       const int  aNbytes
                     )
{
   bool    rc;

   if ( (m_size = (aNbytes == 0 ? m_nbytes : aNbytes))  > m_nbytes )
       throw XMqueue(errno,"Dequeue : bytes to send > declared message size");


   if ( msgrcv ( m_qid, pmbuf , m_nbytes, 0, m_mode )  < 0 )
       if ( errno == EAGAIN )   // Resource was busy Q empty
          rc = false;
       else
          throw XMqueue(errno,"Dequeue : msgrcv failed");
   else
   {
        rc = true;
        aKey= pmbuf->mtype;
        memcpy (aData, pmbuf->mtext, m_size);
   }

   return(rc);
}




bool tMqueue::DequeueByKey(
                       pchar aData,
                       const int  aKey,
                       const int  aNbytes
                     )
{
   bool    rc;
   //ostrstream x;

   if ( (m_size = (aNbytes == 0 ? m_nbytes : aNbytes))  > m_nbytes )
       throw XMqueue(errno,"Dequeue : bytes to send > declared message size");


   if ( msgrcv ( m_qid, pmbuf , m_nbytes, aKey, m_mode )  < 0 )
     if ( errno == EAGAIN )   // Resource was busy
         rc = false;
     else
         throw XMqueue(errno,"Dequeue by key : msgrcv failed");

   else
   {
      rc = true;
      memcpy (aData, pmbuf->mtext, m_size);
   }

   return ( rc );
}

//
//  return total possible size of queue in bytes
//
int tMqueue::GetMaxQSize()
{

 struct msqid_ds qstats;

 if ( msgctl ( m_qid, IPC_STAT, &qstats) < 0 )
     throw XMqueue(errno,"GetMaxQsize : msgctl IPC_STAT Failed");

 return( qstats.msg_qbytes );

}

//
//  return total sizeof queue
//
int tMqueue::GetQSize()
{
 struct msqid_ds qstats;

 if ( msgctl ( m_qid, IPC_STAT, &qstats) < 0 )
     throw XMqueue(errno,"GetQsize : msgctl IPC_STAT Failed");

 return( qstats.msg_qnum );

}
bool tMqueue::Lock()
{

  if  ( m_locked )    // can't lock a locked queue
    return false;

  m_oldmode = qchmod( IPC_R );
  if ( m_oldmode < 0 )
     return false;

  m_locked = true;
  return true;
}


bool tMqueue::Release()
{
  if  ( !m_locked )  // cant release a unlocked queue
    return false;


  if ( qchmod( m_oldmode )  < 0)
     return false;

  m_locked = false;
  m_oldmode = 0;
  return true;
}

int tMqueue::qchmod( unsigned short aMode )
{
 struct msqid_ds qstats;
 unsigned short  perm;   // previous permission


 if ( m_owner == false )  // only owner of queue is allowed to lock it
   return -1;


 if ( msgctl ( m_qid, IPC_STAT, &qstats) < 0 )      // first read Existing Values
     throw XMqueue(errno,"qchmod : msgctl IPC_STAT Failed");


 perm = qstats.msg_perm.mode;
 qstats.msg_perm.mode = aMode;

 if ( msgctl ( m_qid, IPC_SET, &qstats) < 0 )     // set new values
     throw XMqueue(errno,"qchmod : msgctl IPC_SET Failed");


 return perm;

}

ostream & operator <<( ostream & os, tMqueue & aQ )
{
 struct msqid_ds qstats;

 if ( msgctl ( aQ.m_qid, IPC_STAT, &qstats) < 0 )
 {
     os << "msgctl :: Failed  on qid " << aQ.m_qid << endl;
 }
 else
 {
      os << "MESSAGE QUEUE VITAL STATISTICS"   << endl
      << "No of messages       (msg_qnum) " << qstats.msg_qnum << endl
      << "Max Allowed size   (msg_qbytes) " << qstats.msg_qbytes << endl
      << "Pid of last Enqueue (msg_lspid) " << qstats.msg_lspid << endl
      << "Pid of last Dequeue (msg_lrpid) " << qstats.msg_lrpid << endl
      << "Time of Last Enqueue(msg_stime) " << ctime((time_t *)&qstats.msg_stime)
      << "Time of Last Dequeue(msg_rtime) " << ctime((time_t *)&qstats.msg_rtime)
      << "Time of Last        (msg_ctime) " << ctime((time_t *)&qstats.msg_ctime)
      << "Owners ID        (msg_perm.uid) " << qstats.msg_perm.uid << endl
      << "Owners Group     (msg_perm.gid) " << qstats.msg_perm.gid << endl
      << "Creators ID     (msg_perm.cuid) " << qstats.msg_perm.cuid << endl
      << "Creators Group  (msg_perm.cgid) " << qstats.msg_perm.cgid << endl
      << "Permissions     (msg_perm.mode) " <<  qstats.msg_perm.mode << endl
      << "Slot Sequence No (msg_perm.seq) " << qstats.msg_perm.seq << endl
      << "Key              (msg_perm.key) " << (long ) qstats.msg_perm.key << endl
      << "Read/Write Behaviour   (m_mode) " <<
           (aQ.m_mode == IPC_NOWAIT ? "NonBlocking" : "Blocking")  << endl;

 }

     return os;

}

xMqueue::xMqueue(char * aFile,const int aLine, int aErrno, char * aInfo) :
      xCept( aFile, aLine, "xMqueue",aErrno )
{
  osErr << "Unix Sytem Call Error " << errnomsg(aErrno)
        << endl << aInfo;
}




