#ifndef _TIMER_H_
#define _TIMER_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_TIMER_H_);
// timer.h
// Date Written : Wed May  4 13:27:44 WET 1994
// Author       : Suri Sena
// Description
//  Set a timeout, used to set timeouts for Blocking System Calls
//  e.g.
//
//
//  Timer.Set(5);
//  rc = read (sss,sss);
//  Timer.Clear();
//  if ( rc < 0 )
//   switch ( errno )
//   {
//      EINTR :  // timeout
//
//      default : // other error
//
//   }
//}
//


class tAlarm {

     private :
          static void nullfcn(int);
     public :
          ~tAlarm ();
          static void Set ( const int aMsecs );
          static void Clear ();

};

#endif

