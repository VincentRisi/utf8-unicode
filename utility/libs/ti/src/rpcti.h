/******************************************************/
/*                                                    */
/*  System : Treasury International                   */
/*  File   : RPC.H                                    */
/*  Target : RPC.LIB                                  */
/*  Author : Jonnie Gilmore                           */
/*  Date   : Tue 30-08-1994                           */
/*                                                    */
/*  NOTE   : See the end of this file for examples.   */
/*                                                    */
/*  Change History :                                  */
/*                                                    */
/******************************************************/

#ifndef _RPC_H_
#define _RPC_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_RPC_H_);

 #include <errorno.h>

 #ifdef _WINDOWS
    #include <clisock.h>     /* found in t:\dev\include       */
 #else /* _WINDOWS */
    #include <cs.h>          /* found in t:\dev\include       */
 #endif /* _WINDOWS */

 // Valid values for Mtype
 #define RPC_CALL        0
 #define RPC_REPLY       1

 // Valid values for Reply
 #define RPC_SYNC        0
 #define RPC_ASYNC       1

 #define MAX_NAME_LENGTH 128

 typedef struct tagRPC_Header
 {
     // ? short    RPCVersion;
     char    HostId[17];         // Host IP address
     char    NotUsed1[3];
     char    WsId[17];           // Work station IP Address
     char    NotUsed2[3];
     char    Mtype;              // Message type eg. Call or Reply
     char    NotUsed3;
     short   Reply;              // Synch or Asynch
     long    RpcReqID;           // Unique Nedbank wide Procedure Identifier.
     short   ServerVersion;      // Version no of procedure
     short   ClientVersion;      // Version of rpc servoce
     // ? short   AppVersion;         // Version of rpc servoce

     char    Authentication1[20];// RESERVED for security system
     char    Authentication2[20];// RESERVED for security system
     long    ReturnCode;         // Return code from remote procedure
     long    Msize;              // Size of message following
 } tRPCHeader;

char *GetMyIPAddress ( char *Buffer17 );

#ifndef _WINDOWS

// RPC's own exceptions class - used to throw exceptions matching
// return error codes.
class xRPC : public xCept
{
   public :
      long OError(void);
      xRPC(int);
   private:
      long oError;
};

#endif /* _WINDOWS */



 class tRPCClient
 {
    private:
       tRPCHeader _TxHeader;
       DATABLOCK  _pRxBuffer;
       DATALEN    _RxSize;
       int        _MustFreeMemory;
       long       _LastError;
       char       _LastErrorMessage[81];

#ifdef _WINDOWS
       char       _HostName  [MAX_NAME_LENGTH+1];
       char       _SocketName[MAX_NAME_LENGTH+1];
       long       _MaxBlock;             // Maximum block size - msgs with
                                         // longer lengths are file based.
#else  /* _WINDOWS */
       char       _Name  [MAX_NAME_LENGTH+1];
       long       _Size;
       long       _Timeout;
#endif /* _WINDOWS */

       void Initialise(void);

    public:
       tRPCClient();
       ~tRPCClient();
       long GetLastError ( void );
       char *GetLastErrorMessage ( char *ErrorMsg, int cbErrorMsg ); // Returns ErrorMsg ptr to allow function embedding.

#ifdef _WINDOWS
       tRPCClient             ( char *HostName, char *SocketName, long MaxBlock=LONG_MAX);
       long SetHostParameters ( char *HostName, char *SocketName, long MaxBlock);
#else  /* _WINDOWS */
       tRPCClient             ( char *Name, long Size, long Timeout );
       long SetHostParameters ( char *Name, long Size, long Timeout );
#endif /* _WINDOWS */

       void KeepRxBuffer ( void );

       long Call ( long      ReqID,
                   DATABLOCK pTxBuffer = NULL,
                   DATALEN   TxSize = 0 );

       DATABLOCK pRxBuffer ( void );
       DATALEN RxSize ( void );

       long CallFixed ( long      ReqID,
                        DATABLOCK pTxBuffer = NULL,
                        DATALEN   TxSize = 0,
                        DATABLOCK pRxBuffer = NULL,
                        DATALEN   RxSize = 0 );
 };

 #ifndef _WINDOWS
  /******************************************************/
  /** Server defined for AIX only                      **/
  /******************************************************/

  class tRPCServer
  {
     private :
        tServer   *_SockSrv;
        tRPCHeader _Header;
        int        _SocketOpen;
        long       _SvrVersion;
        long       _ReqID;
        long       _RxSize;
        char      *_pRxBuffer;
        long       _LastError;
        char       _WSIPAddress[17];

        void Receive  ( char *pBuffer, long size );
        void Transmit ( char *pBuffer, long size );

     public:
        tRPCServer ( long ServerVersion, tServer *SockSrv );
        ~tRPCServer ();
       long GetLastError ( void );
       char *GetLastErrorMessage ( char *ErrorMsg, int cbErrorMsg ); // Returns ErrorMsg ptr to allow function embedding.

        void  WaitForCall ( void );
        char *pRxBuffer ( void );
        long  RxSize ( void );
        long  ReqID ( void );
        char *pWSIPAddress ( void );
        void  RespondToCall ( long ReturnCode, char *pTxBuffer = NULL, long TxSize = 0 );
#ifdef UPM_SERVER
        tRPCHeader *pRPCHeader ( void );
#endif

        // The following exceptions should be caught :
        //    xClientServer& x
        //    xRPC& x
  };

 #endif /* _WINDOWS */




 /******************************************************/
 /** Some additional useful stuff for Windows         **/
 /******************************************************/
 #ifdef _WINDOWS
  extern "C" void _cdecl PopUp    ( char *Format, ... );
  extern "C" void _cdecl DebugOut ( char *Format, ... );
 #endif /* _WINDOWS */

#endif /* _RPC_H_ */

/****************************************************************************
**
** EXAMPLE: tRPCClient
** -------------------
**
** {
**    char far *pRxBuffer;
**    long     cbRxBuffer;
**    // AIX: tRPCClient RPC ( ":socket:devrs:settle", 0, 10000 ); // 10 secs
**    tRPCClient RPC ("devrs","cc");
**    long Erc;
**
**    // place code to create and initialise pTxBuffer and cbTxBuffer here
**
**    Erc = RPC.Call ( RQ_CC_CALC_CHARGES,
**                     pTxBuffer,
**                     cbTxBuffer );
**    if (Erc)
**    {
**       // Error Handling
**       return Erc;
**    }
**
**    pRxBuffer  = RPC.pRxBuffer();
**    cbRxBuffer = RPC.RxSize();
**
**    // Place code to examine/copy the data at pTxBuffer here
**
**    // The destructor of tRPCClient will free any allocated memory
**    // unless the method KeepRxBuffer() is called, in which case
**    // tRPCClient will not delete the buffer in the destructor and
**    // you must save the pointer before the destructor and delete it
**    // later yourself.
**    }
**
** CallFixed
** ---------
** CallFixed is identical to Call except that the length of the reponse
** must be known.
** If the recieved response is longer than RxSize, the message is
** truncated.  If the response is shorter than RxSize, the message
** is padded with NULL's.
**
** AIX "Name" format
** -----------------
** Named pipes        :  ":pipe:<machine>:<pipe-name>"
** Shared memory AIX  :  ":shm:<key>:<app-name>"
** Shared memory OS/2 :  ":shm:<seg-name>:<app-name>"
** Sockets            :  ":socket:<host-name>:<service>"
**
** where: <service>   must exist in the services file
**        <host-name> must exist in the hosts    file
**
**   - In AIX  these files are in the \tcpip\etc directory.
**   - In OS/2 these files are in the /etc       directory.
**
*****************************************************************************/



/****************************************************************************
**
** EXAMPLE: tRPCServer
** -------------------
**  tServer SockSrv ( ... );
**  ...or...
**  tMyServer : public tPreSock { ... };
**
**  try
**  {
**     tRPCServer RPC(10000,SockSrv);
**     // where:
**     //   1. 10000 is the version number of the server,
**     //      representing 1.00.00.
**     //   2. SockSrv is an instance of tServer declared either
**     //      specifically or internally as a protected member in
**     //      tPreSock.
**
**     while (1)
**     {
**        long       RxSize;
**        char      *pRxBuffer;
**        long       TxSize;
**        char      *pTxBuffer;
**        long       ErrorCode;
**
**        RPC.WaitForCall ();
**
**        pRxBuffer = RPC.pRxBuffer();
**        RxSize    = RPC.RxSize();
**        pTxBuffer = NULL;
**        TxSize    = 0;
**        ErrorCode = 0;
**
**        // Process Request placing the response in,
**        // for example, pTxBuffer.
**        switch (RPC.ReqID())
**        {
**           case RQ_DM_GREETING :
**              if ((RxSize==6) && (strcmp(pRxBuffer, "HELLO") == 0))
**              {
**                 TxSize = 4;
**                 pTxBuffer = new char [TxSize];
**                 strcpy ( pTxBuffer, "BYE" );
**                 ErrorCode = 0;
**              }
**              break;
**        }
**
**        RPC.RespondToCall ( ErrorCode, pTxBuffer, TxSize );
**     }
**  }
**  catch (xClientServer &x)
**  {
**     x.Display ( cerr );
**     Result=x.OError();
**  }
**  catch (xRPC &x)
**  {
**     Result=x.OError();
**  }
**
*****************************************************************************/
