#include "versions.h"
#define _VERSION_NO "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
COMPILED_VERSION(_VERSION_NO);

// Filename      :  tSem2.c
// Description   :  Exclusive/Shared mode Semaphore
// Subsystem     :  ipc
// Author        :  Suriya Sena  $AUTHOR$
// Date Created  :  1 Nov 1994
// $LOG$

//#define DEBUG_ON

#include <sys/types.h>
#include <sys/sem.h>
#include <errno.h>
#include "ti.h"
#include "errnomsg.h"
#include "tsem2.h"

//   How it Works
//
//   Q) Why all this ?
//   A) Because semaphore operations are not queued. This means that one or
//      more processes can 'starve' if one process is hogging the semaphore
//      controlled resource. This implementation trys to prevent hogging
//      by the intoducing a Request Semaphore. (Processes may get hungry but
//      they won't starve).
//      REMEMBER -- an array of semadjusts are executed as an Atomic operation
//      if one adjust fails, the preceding adjusts are undone and the enrire
//      list retried.
//
//   Three semaphores
//
//   Request (RQ) -- Binary Semaphore
//   Reader  (R)  -- Counting Semaphore
//   Writer  (W)  -- Binary Semaphore
//
//              READER                         WRITER
//
//  AQUIRE      1) wait RQ == 0                1) wait RQ == 0
//                 inc  RQ                        inc RQ
//
//              2) inc  R                      2) wait R == 0
//                 wait W == 0                    wait W == 0
//                 dec  RQ                        inc  W
//                                                dec  RQ
//
//  Notice : after an aquire RQ semaphore is zero so any other process
//  can set it
//
//  RELEASE     1) dec R                       2) dec W
//
//  What could be simpler!!!
//

#ifdef M_STDLIB
using namespace std;
#endif

sembuf tSem2::QR[]={ 0,0,0,
         0,1,SEM_UNDO
                   };

sembuf tSem2::AX[]={ 1,0,0,          // Wait for readers == 0
                     2,0,0,          // wait for writers == 0
                     2,1,SEM_UNDO,   // INC writers
                     0,-1,SEM_UNDO
        };
                          // Release EXCLUSIVE lock
sembuf tSem2::RX[]={2,-1,SEM_UNDO}; // DEC writers
                          // Aquire SHARE lock
sembuf tSem2::AS[]={
        1,1,SEM_UNDO,   // INC readers
                    2,0,0,           // wait writer == 0
                    0,-1,SEM_UNDO
       };
                           // Release SHARE lock
sembuf tSem2::RS[]={1,-1,SEM_UNDO}; // DEC reader



tSem2::tSem2( long aKey,  int aMode ) : m_state(None)
{
    int Flags=0;


    m_stream_mode=1;

    if ( aMode == Create )
    {
       m_owner =true;
       Flags=IPC_CREAT;
    }
    else
    {
       m_owner =false;
       Flags=0;
    }

    if ( ( m_semid =semget(aKey,3,Flags | 0770 )) < 0 )
       throw XSem2(errno,"failed on semget()");

}


tSem2::~tSem2()
{
     if (m_owner==true)
         semctl ( m_semid ,0,IPC_RMID , 0 );
}

ostream & operator << ( ostream & os, tSem2 & s)
{
   int   buf[3];
   short arr[3];
   static  int LineCount=24;

     if ( semctl ( s.m_semid ,0, GETALL, &arr ) < 0 )
  throw XSem2(errno,"semctl : failed on GETALL");

     if ( ( buf[0] = semctl ( s.m_semid ,0, GETZCNT, 0 )) < 0 )
  throw XSem2(errno,"semctl : failed on GETZCNT");
     if ( ( buf[1] = semctl ( s.m_semid ,1, GETZCNT, 0 )) < 0 )
  throw XSem2(errno,"semctl : failed on GETZCNT");
     if ( ( buf[2] = semctl ( s.m_semid ,2, GETZCNT, 0 )) < 0 )
  throw XSem2(errno,"semctl : failed on GETZCNT");



      if  ( s.m_stream_mode ==1 )
      {
        os <<  "Semaphore Status" << endl
        <<  "No of Wait Queue        0 or 1  " << arr[0] << endl
        <<  "No of Share locks       (>= 0)  " << arr[1] << endl
        <<  "No of Exclusive locks (0 Or 1)  " << arr[2] << endl
        <<  "No of Procesess Waiting on Zero Wait Queue      " << buf[0] << endl
        <<  "No of Procesess Waiting on Zero Share Locks     " << buf[1] << endl
        <<  "No of Procesess Waiting on Zero Exclusive Locks " << buf[2] << endl;
      }
      else
      {
       if ( LineCount >=24 )
       {
    LineCount=1;
          cout << setw(12) << "Semvals"
         <<setw(5)   << " "
         <<setw(12)  << "Procs Blocked" << endl
               << setw(4) << "WQ"
               << setw(4) << "R"
               << setw(4) << "W"
         <<setw(5)   << " "
               << setw(4) << "WQ"
               << setw(4) << "R"
               << setw(4) << "W" << endl << endl;
       }
       else
     LineCount++;

       os << setw(4) << arr[0]
    << setw(4) << arr[1]
    << setw(4) << arr[2]
    << setw(5) << " "
    << setw(4) << buf[0]
    << setw(4) << buf[1]
    << setw(4) << buf[2];

       }
     return os;
}


void  tSem2::AquireEXCL()
{

     if (m_state != None)
  throw XSem2(errno,"Aquire : object ALREADY holds a semaphore");

     debug(cout << "Aquire Exclusive wait( Exclusive == 0 )... " << endl);

     if (semop ( m_semid , QR, 2 ) < 0 )
  throw XSem2(errno,"Aquire : semop failed");

     debug(cout << "Aquire Exclusive wait( Shared == 0 )... " << endl);

     if (semop ( m_semid , AX, 4 ) < 0 )  // wait zero reader
  throw XSem2(errno,"Aquire : semop failed");


     m_state=Exclusive;

     debug(cout << "Aquire Exclusive OK " << endl);

}

void tSem2::AquireSHR()
{
     if (m_state != None)
  throw XSem2(errno,"Aquire : object ALREADY holds a semaphore");

     if (semop ( m_semid , QR, 2 ) < 0 )
  throw XSem2(errno,"Aquire : semop failed");

     debug(cout << "Aquire Share  (Inc Readers )..." << endl);
     if (semop ( m_semid , AS, 3 ) < 0 )
  throw XSem2(errno,"Aquire : semop failed");


     debug(cout << "Aquire Share OK" << endl);

     m_state=Share;
}

void tSem2::Release()
{

     struct sembuf *p;

     debug(cout << "Release m_state ==" << m_state << endl);
     switch ( m_state )
     {
         case Exclusive :
    p = RX;
    break;

         case Share :
    p = RS;
          break;

         case None :
    throw XSem2(errno,"Release : object doesn't hold a semaphore");
    break;

         default :
    throw XSem2(errno,"Release : object in an inconsistent state");
    break;
     }

     if ( semop ( m_semid , p, 1 ) < 0 )
        throw XSem2(errno,"Release : semop() failed");

     m_state=None;
}


xSem2::xSem2(char * aFile,const int aLine, int aErrno, char * aInfo) :
      xCept( aFile, aLine, "xSem2",aErrno )
{
  //AssignError("Unix Sytem Call Error %s\n%s", errnomsg(aErrno), aInfo);
    osErr << "Unix Sytem Call Error " << errnomsg(aErrno) << endl << aInfo;
}
