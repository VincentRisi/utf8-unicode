#ifndef __RPCSTREAM_H__
#define __RPCSTREAM_H__ 
/***************************************************************************
System:       Nedcor Rates Server
Subsystem :   None
Author:       George McFie.
Date:         Tue 22 Jan 1998
Description:  Nedcor Rates Server RPC Interface.

Copyright (c) BB&D 1998.  All rights reserved.
***************************************************************************/
#include "ti.h"
#include "cbsocket.h"

// *************************************************************************
enum eDataType {
    DT_end = 0
  , DT_void
  , DT_iovoid
  , DT_refvoid
  , DT_getvoid
  , DT_selvoid
  , DT_newsize
  , DT_int2
  , DT_int4
  , DT_float
  , DT_double
  , DT_bool
};

// --------------------------------------------------------------------------
class tRPCBuffer
{
  public:
    tRPCBuffer();
    tRPCBuffer( uint32 ulSize );
    tRPCBuffer( char* pBuffer, uint32 ulSize );
    ~tRPCBuffer();
    void SetData( char* pBuffer, uint32 ulSize );
    inline char* c_str() { return pMyBuffer; };
    inline uint32 DataSize() { return ( pMyNext - pMyBuffer ); };
    void Append( unsigned short usParam );
    void Append( uint32 ulParam );
    void Append( double dParam );
    void Append( char* pData, uint32 ulSize );

    void Extract( unsigned short* pusParam );
    void Extract( uint32* pulParam );
    void Extract( float* pfParam );
    void Extract( double* pdParam );
    void Extract( char* pData, uint32 ulSize, uint32 ulMaxSize );

    void Reference( char** ppData, uint32 ulSize );

    static byte2 Swap( short s );
    static byte4 Swap( int32 l );
    static byte4 Swap( float f );
    static byte8 Swap( double d );

    static void Swap( short* ps );
    static void Swap( int32* pl );
    static void Swap( float* pf );
    static void Swap( double* pd );

  private:
    void ExpandBuffer( uint32 ulSize );

  private:
    char* pMyBuffer;
    char* pMyNext;
    uint32 ulMySize;
    uint32 ulMyTotalSize;
};

// --------------------------------------------------------------------------
class tRPCStream
{
  public:
    // Constructor
    tRPCStream( uint32 ulSignature, tCBSockClient* pSocket, bool fDelSocket );

    // Destructor
    // The destructor will also 'Close' an active session
    ~tRPCStream();

    // Open must be called to establish a session with the server
    bool Open( bool fCallback = false );

    // Open must be called to terminate a session with the server
    void Close();

    // Stream a procedure call to the server
    static void WriteStream( tRPCBuffer& Buffer, unsigned short usProcID, ... );

    // Read the procedure response
    static void ReadStream( tRPCBuffer& Buffer, unsigned short usProcID, ... );

    // Set the timeout value
    inline void SetTimeout( uint32 ulTimeout )
      { ulMyTimeout = ulTimeout; };

    // Get the timeout value
    inline uint32 Timeout()
      { return ulMyTimeout; };

    // Get the interface signature
    inline const uint32 Signature()
      { return ulMySignature; }

    // For use by the Client
    bool ClientCall( unsigned short usProcID,
                     tRPCBuffer& InBuffer,
                     tRPCBuffer& OutBuffer );
    bool ClientSend( tRPCBuffer& InBuffer );
    bool ClientReceive( tRPCBuffer& Buffer );

    void SetTag( int32 lTag ) { lMyTag = lTag; }
    int32 GetTag() { return lMyTag; }

    // For use by the Server
    bool ServerSend( tRPCBuffer& InBuffer );
    bool ServerReceive( uint32* pulSignature, tRPCBuffer& Buffer );

    inline const uint32 LastError() { return ulMyLastError; }

    inline int Socket() { return pMySocket->Socket(); }

    inline bool IsOpen()
      { return ( pMySocket && pMySocket->IsOpen() ) ? true : false; }

    enum OpenMode
    {
      MODE_SIMPLE = 1,
      MODE_CALLBACK
    };

    // General interface errors
    enum ErrorCode
    {
      ERR_NONE = 0,           // No error
      ERR_GENERIC,            // Generic (non-specific) interface error
      ERR_BAD_SIGNATURE,      // Client-Server signature mismatch
      ERR_INVALID_PARAMETER,  // Invalid parameter
      ERR_TIMEOUT,            // Timeout on call to the server
      // Place new errors here

      ERR_COUNT,              // Number of predefined error codes
      ERR_USER = 1000         // First of the user defined messages
    };
    static const char* ErrorText( uint32 ulError );

  protected:
    tCBSockClient*      pMySocket;
    uint32       ulMyLastError;

  private:
    const uint32 ulMySignature;
    bool                fMyDelSocket;
    bool                fMyCallback;
    uint32       ulMyTimeout;
    int32                lMyTag;       // For application use
};

//-------------------------------- EOF -------------------------------------
#endif   // __RPCSTREAM_H__
