//
//  Author          S M Sena
//  Date Written    19-Aug-1994
//  Purpose         Data Distribution Client for UNIX
//                  The purpose of thisobject is to provide the following
//                  services. The ability to syncohronise the unix tables with
//                  the remote hosts tables.
//


#ifndef  _DDCIX_H_
#define  _DDCIX_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_DDCIX_H_);

#include "ddc.h"

class  ddcix : public ddclient {

  private :

    int           m_errno;

    static char *m_errortxt[];

    tSockClient  *SockClient;
    tConnect     *connect;

    struct MessageHdr ReplyHdr,
                      CallHdr;


    char *ReplyBody;

    bool LoadRevTable( tTablelist &);
             // Populate tlist with Local Data

    bool Refresh(char * Tablename, char * FromRevision = UCTime );
             //  refresh from host


  public :


    ddcix ( char * inifile);

    ~ddcix ();

    bool  good();
    bool  bad();

   // true if ddcix encountered an error
   // false if ddcix is OK
   // use Good or Bad after Ctor to check that construction was successful

    bool  SynchTable();
             //  syncronise tablename



};
#endif
