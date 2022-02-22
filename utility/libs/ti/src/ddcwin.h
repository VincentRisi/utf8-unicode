//
//  Author          S M Sena
//  Date Written    19-Aug-1994
//  Purpose         Data Distribution Client for Windows
//                  The purpose of thisobject is to provide the following
//                  services. The ability to syncohronise the PCs tables with
//                  the remote hosts tables.
//


#ifndef  _DDCWIN_H_
#define  _DDCWIN_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_DDCWIN_H_);

#include "ddc.h"
#define MSIZE  200
#define UID ""
#define PASSWD ""

class  ddc : public ddclient {

  private :

    int           m_errno;

    short         m_errflags;                 // Maskfield
                     // m_errflags are set to indicate which errors have occured

    enum     {
               errHashTab  = 1,       // Has Table Error
               errSocket   = 2,       // Socket Error
               errCtor     = 4,       // Constructor Failed Error
               errProgram  = 8,       // program error
               errDataBase = 16,      // Database General Error
               errDBCursor = 32,      // Database Cursor Error
               errDBSqlBlk = 64       // Database Sqlblock
             };


    static char * m_errortxt [];      // Error messages
    static int    m_instance;         // Count of instances ONLY 1may exist

    char      m_DBErr[MSIZE],         // db error buffer
              m_SocketErr[MSIZE],     // socket error buffer
              m_hostname[60];         // host name providing dds service
    long      m_MaxRevs;
    // Socket Connection Stuff
    tCliSock      *m_sd;
    // DB connection data members

    HSQL       SqlBlock;
    HSQL       Cursor;

     ofstream  m_logstr;
    struct MessageHdr ReplyHdr,
                      CallHdr;
    char *ReplyBody;

    bool LoadRevTable( tTablelist &);
             // Populate tlist with Local Data

    void ddc::rtrim(char * s1,char c );
            // strip char from s1

    void  SetError ( short aFlags, int aErrno = -1 );
	    //  Set Error Flags and construct embbeded class error messages
	    //  e.g. DB Errors and socket errors

    bool Refresh(char * Tablename, char * FromRevision = UCTime);
             //  refresh from host



  public :


    ddc ( char * hostname,      // host providing dds service
          char * dsn,           // datasource name
          char * binpath,       // path to sqlbin file
          char * logpath,       //  "    " log file
          int  MaxRevs=50      // Max allowed Revs to get a table into synch
        );

    ~ddc ();

    bool  good();
    bool  bad();

   // true if ddc encountered an error
   // false if ddc is OK
   // use Good or Bad after Ctor to check that construction was successful

    bool  SynchTable();
             //  syncronise tablename


    char * Errmsg( char * buf,  int cb );
             //  copies  Cb bytes of last error message into buf

};


#endif
