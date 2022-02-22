//
//  Author          S M Sena
//  Date Written    19-Aug-1994
//  Purpose         DLL - Data Distribution Client
//                  The purpose of thisobject is to provide the following
//                  services. The ability to syncohronise the PCs tables with
//                  the remote hosts tables.
//


#ifndef  _DDC_H_
#define  _DDC_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_DDC_H_);


class  ddclient {

 private :
    virtual bool LoadRevTable( tTablelist &) = 0;
             // Populate tlist with Local Data


    virtual bool Refresh(char * Tablename, char * FromRevision ) = 0;
             //  refresh from host

 public :

    virtual ~ddclient () {};

    virtual bool  good()=0;
    virtual bool  bad()=0;

   // true if ddc encountered an error
   // false if ddc is OK
   // use Good or Bad after Ctor to check that construction was successful

    virtual  bool  SynchTable()= 0;
             //  syncronise tablename


};
#endif
