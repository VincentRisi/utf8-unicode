
/***************************************************************************
System:       Nedcor Rates Server
Subsystem :   None
Author:       George McFie.
Date:         Tue 22 Jan 1998
Description:  Nedcor Rates Server RPC Interface.

Copyright (c) BB&D 1998.  All rights reserved.
***************************************************************************/
//----------------------------- Includes -----------------------------------
#include "ti.h"
#include <stdarg.h>

#include "rpcstream.h"

// *************************************************************************
#ifndef M_AIX
#define SwapEnds(a, b) (a) ^= (b) ^= (a) ^= (b) // reduced version
#else
#define SwapEnds(a, b)
#endif

// *************************************************************************
tRPCBuffer::tRPCBuffer()
  : pMyBuffer( NULL )
  , pMyNext( NULL )
  , ulMyTotalSize( 0 )
  , ulMySize( 0 )
{
}

// -------------------------------------------------------------------------
tRPCBuffer::tRPCBuffer( char* pBuffer, uint32 ulSize )
{
  ulMyTotalSize = ulSize;
  ulMySize = 0;
  pMyBuffer = pBuffer;
  pMyNext = pMyBuffer;
}

// -------------------------------------------------------------------------
tRPCBuffer::tRPCBuffer( uint32 ulSize )
{
  ulMyTotalSize = ulSize;
  pMyBuffer = new char[ulSize];
  pMyNext = pMyBuffer;
  ulMySize = 0;
}

// -------------------------------------------------------------------------
tRPCBuffer::~tRPCBuffer()
{
  delete[] pMyBuffer;
}

// -------------------------------------------------------------------------
void tRPCBuffer::SetData( char* pBuffer, uint32 ulSize )
{
  if ( pMyBuffer )
    delete[] pMyBuffer;
  ulMySize = ulSize;
  ulMyTotalSize = ulSize;
  pMyBuffer = pBuffer;
  pMyNext = pMyBuffer;
}

// -------------------------------------------------------------------------
void tRPCBuffer::Append( unsigned short usParam )
{
  usParam = htons( usParam );
  if ( ( pMyNext - pMyBuffer + sizeof( usParam ) ) > ulMyTotalSize )
    ExpandBuffer( sizeof( usParam ) );
  memcpy( pMyNext, &usParam, sizeof( usParam ) );
  ulMySize += sizeof( usParam );
  pMyNext = pMyBuffer + ulMySize;
}

// -------------------------------------------------------------------------
void tRPCBuffer::Append( uint32 ulParam )
{
  ulParam = htonl( ulParam );
  if ( ( pMyNext - pMyBuffer + sizeof( ulParam ) ) > ulMyTotalSize )
    ExpandBuffer( sizeof( ulParam ) );
  memcpy( pMyNext, &ulParam, sizeof( ulParam ) );
  ulMySize += sizeof( ulParam );
  pMyNext = pMyBuffer + ulMySize;
}

// -------------------------------------------------------------------------
void tRPCBuffer::Append( double dParam )
{
  byte8 Byte8 = Swap( dParam );
  if ( ( pMyNext - pMyBuffer + sizeof( dParam ) ) > ulMyTotalSize )
    ExpandBuffer( sizeof( dParam ) );
  memcpy( pMyNext, &Byte8, sizeof( dParam ) );
  ulMySize += sizeof( dParam );
  pMyNext = pMyBuffer + ulMySize;
}

// -------------------------------------------------------------------------
void tRPCBuffer::Append( char* pData, uint32 ulSize )
{
  if ( ( pMyNext - pMyBuffer + ulSize ) > ulMyTotalSize )
    ExpandBuffer( ulSize );
  memcpy( pMyNext, pData, ulSize );
  ulMySize += ulSize;
  pMyNext = pMyBuffer + ulMySize;
}

// -------------------------------------------------------------------------
void tRPCBuffer::ExpandBuffer( uint32 ulSize )
{
  ulMyTotalSize += ulSize;
  char* pNewBuffer = new char[ ulMyTotalSize ];
  memcpy( pNewBuffer, pMyBuffer, ulMySize );
  delete[] pMyBuffer;
  pMyBuffer = pNewBuffer;
  pMyNext = pMyBuffer + ulMySize;
}

// -------------------------------------------------------------------------
void tRPCBuffer::Extract( unsigned short* pusParam )
{
  *pusParam = ntohs( *( unsigned short* )pMyNext );
  pMyNext += sizeof( unsigned short );
}

// -------------------------------------------------------------------------
void tRPCBuffer::Extract( uint32* pulParam )
{
  *pulParam = ntohl( *( uint32* )pMyNext );
  pMyNext += sizeof( uint32 );
}

// -------------------------------------------------------------------------
void tRPCBuffer::Extract( float* pfParam )
{
  byte4 Byte4 = Swap( *( float* )pMyNext );
  *pfParam = *( float* )&Byte4;
  pMyNext += sizeof( float );
}

// -------------------------------------------------------------------------
void tRPCBuffer::Extract( double* pdParam )
{
  byte8 Byte8 = Swap( *( double* )pMyNext );
  *pdParam = *( double* )&Byte8;
  pMyNext += sizeof( double );
}

// -------------------------------------------------------------------------
void tRPCBuffer::Extract( char* pData, uint32 ulSize, uint32 ulMaxSize )
{
  if ( ulMaxSize < ulSize )
    memcpy( pData, pMyNext, ulMaxSize );
  else
    memcpy( pData, pMyNext, ulSize );
  pMyNext += ulSize;
}

// -------------------------------------------------------------------------
void tRPCBuffer::Reference( char** ppData, uint32 ulSize )
{
  *ppData = pMyNext;
  pMyNext += ulSize;
}

// -------------------------------------------------------------------------
byte2 tRPCBuffer::Swap( short s )
{
  union {short s; byte2 b;} u;
  u.s = s;
  SwapEnds(u.b.p[0], u.b.p[1]);
  return u.b;
}

// -------------------------------------------------------------------------
byte4 tRPCBuffer::Swap( int32 l )
{
  union {int32 l; byte4 b;} u;
  u.l = l;
  SwapEnds(u.b.p[0], u.b.p[3]);
  SwapEnds(u.b.p[1], u.b.p[2]);
  return u.b;
}

// -------------------------------------------------------------------------
byte4 tRPCBuffer::Swap( float f )
{
  union {float f; byte4 b;} u;
  u.f = f;
  SwapEnds(u.b.p[0], u.b.p[3]);
  SwapEnds(u.b.p[1], u.b.p[2]);
  return u.b;
}

// -------------------------------------------------------------------------
byte8 tRPCBuffer::Swap( double d )
{
  union {double d; byte8 b;} u;
  u.d = d;
  SwapEnds(u.b.p[0], u.b.p[7]);
  SwapEnds(u.b.p[1], u.b.p[6]);
  SwapEnds(u.b.p[2], u.b.p[5]);
  SwapEnds(u.b.p[3], u.b.p[4]);
  return u.b;
}

// -------------------------------------------------------------------------
void tRPCBuffer::Swap( short* ps )
{
  union tUnion {short s; byte2 b;};
  tUnion* pu = (tUnion*)ps;
  SwapEnds(pu->b.p[0], pu->b.p[1]);
}

// -------------------------------------------------------------------------
void tRPCBuffer::Swap( int32* pl )
{
  union tUnion {int32 l; byte4 b;};
  tUnion* pu = (tUnion*)pl;
  SwapEnds(pu->b.p[0], pu->b.p[3]);
  SwapEnds(pu->b.p[1], pu->b.p[2]);
}

// -------------------------------------------------------------------------
void tRPCBuffer::Swap( float* pf )
{
  union tUnion {float f; byte8 b;};
  tUnion* pu = (tUnion*)pf;
  SwapEnds(pu->b.p[0], pu->b.p[3]);
  SwapEnds(pu->b.p[1], pu->b.p[2]);
}

// -------------------------------------------------------------------------
void tRPCBuffer::Swap( double* pd )
{
  union tUnion {double d; byte8 b;};
  tUnion* pu = (tUnion*)pd;
  SwapEnds(pu->b.p[0], pu->b.p[7]);
  SwapEnds(pu->b.p[1], pu->b.p[6]);
  SwapEnds(pu->b.p[2], pu->b.p[5]);
  SwapEnds(pu->b.p[3], pu->b.p[4]);
}

// *************************************************************************
tRPCStream::tRPCStream( uint32 ulSignature, tCBSockClient* pSocket, bool fDelSocket )
  : ulMySignature( ulSignature )
  , ulMyLastError( ERR_NONE )
  , pMySocket( pSocket )
  , ulMyTimeout( 10000 )
  , fMyDelSocket( fDelSocket )
  , fMyCallback( false )
  , lMyTag( 0 )
{
}

// -------------------------------------------------------------------------
tRPCStream::~tRPCStream()
{
  Close();
  if ( fMyDelSocket )
  {
    delete pMySocket;
    pMySocket = NULL;
  }
}

// -------------------------------------------------------------------------
bool tRPCStream::Open( bool fCallback )
{
  if ( fCallback )
  {
  }
  else
  {
    if ( pMySocket && pMySocket->IsOpen() )
      return true;
    int iRetry = 0;
    while ( iRetry++ < 3 )
    {
      try
      {
        #ifdef _MSC_VER
          // Force a thread context switch
          // ** Why does this work ?
          Sleep( 10 );
        #endif
        pMySocket->Open();
        if ( pMySocket->IsOpen() )
        {
          pMySocket->WriteLength( MODE_SIMPLE );
          return true;
        }
      }
      catch (...) {}
    }
    #ifdef _MSC_VER
//      Beep( 100, 1000 );
    #endif
  }
  return false;
}

// -------------------------------------------------------------------------
void tRPCStream::Close()
{
  if ( !pMySocket )
    return;
  try
  {
    pMySocket->Close();
  }
  catch (...) {}
}

// -------------------------------------------------------------------------
void tRPCStream::WriteStream( tRPCBuffer& Buffer,
                              unsigned short usProcID, ... )
{
  va_list argptr;
  unsigned short usLength;
  char* pCharParam;

  Buffer.Append( usProcID );

  // Construct the transmit buffer
  va_start( argptr, usProcID );

  char DataType = (unsigned char)va_arg( argptr, int );
  while ( DataType )
  {
    if ( DataType )
    {
      switch( DataType )
      {
        case DT_void:
          pCharParam = va_arg( argptr, char* );
          usLength = va_arg( argptr, unsigned short );
          Buffer.Append( usLength );
          Buffer.Append( pCharParam, usLength );
          break;

        case DT_iovoid:
          pCharParam = va_arg( argptr, char* );
          usLength = va_arg( argptr, unsigned short );
          Buffer.Append( usLength );
          Buffer.Append( pCharParam, usLength );
          usLength = va_arg( argptr, unsigned short );
          Buffer.Append( usLength );
          break;

        case DT_newsize:
        case DT_int2:
          Buffer.Append( va_arg( argptr, unsigned short ) );
          break;

        case DT_int4:
          Buffer.Append( va_arg( argptr, uint32 ) );
          break;

        case DT_float:
          Buffer.Append( va_arg( argptr, float ) );
          break;

        case DT_double:
          Buffer.Append( va_arg( argptr, double ) );
          break;

        case DT_bool:
          Buffer.Append( (unsigned short)va_arg( argptr, bool ) );
          break;

        default:
          break;
      }
      DataType = (unsigned char)va_arg( argptr, int );
    }
  }
	va_end( argptr );
}

// -------------------------------------------------------------------------
void tRPCStream::ReadStream( tRPCBuffer& Buffer, unsigned short usProcID, ... )
{
  unsigned short* pusParam;
  uint32* pulParam;
  unsigned short usParam;
  unsigned short usMaxSize;
  float* pfParam;
  double* pdParam;
  bool* pbParam;
  char* pCharParam;
  char** ppCharParam;
  va_list argptr;

  // Reconstruct the call parameters
  va_start( argptr, usProcID );

  unsigned char DataType = (unsigned char)va_arg( argptr, int );
  while ( DataType )
  {
    switch( DataType )
    {
      case DT_iovoid:
        ppCharParam = va_arg( argptr, char** );
        Buffer.Extract( &usParam );
        Buffer.Reference( ppCharParam, usParam );
        {
          unsigned short usMaxOutSize;
          Buffer.Extract( &usMaxOutSize );
          if ( usMaxOutSize < usParam )
            usMaxOutSize = usParam;
          {
            char* pTemp = new char[usMaxOutSize];
            memset( pTemp, 0, usMaxOutSize );
            memcpy( pTemp, *ppCharParam, usParam );
            *ppCharParam = pTemp;
          }
        }
        break;

      case DT_refvoid:
        ppCharParam = va_arg( argptr, char** );
        Buffer.Extract( &usParam );
        Buffer.Reference( ppCharParam, usParam );
        break;

      case DT_getvoid:
        pCharParam = va_arg( argptr, char* );
        Buffer.Extract( &usParam );
        Buffer.Extract( pCharParam, usParam, usParam );
        break;

      case DT_selvoid:
        pCharParam = va_arg( argptr, char* );
        usMaxSize = va_arg( argptr, unsigned short );
        Buffer.Extract( &usParam );
        Buffer.Extract( pCharParam, usParam, usMaxSize );
        break;

      case DT_newsize:
        ppCharParam = va_arg( argptr, char** );
        Buffer.Extract( &usMaxSize );
        *ppCharParam = new char[usMaxSize];
        memset( *ppCharParam, 0, usMaxSize );
        break;

      case DT_int2:
        pusParam = va_arg( argptr, unsigned short* );
        Buffer.Extract( pusParam );
        break;

      case DT_int4:
        pulParam = va_arg( argptr, uint32* );
        Buffer.Extract( pulParam );
        break;

      case DT_float:
        pfParam = va_arg( argptr, float* );
        Buffer.Extract( pfParam );
        break;

      case DT_double:
        pdParam = va_arg( argptr, double* );
        Buffer.Extract( pdParam );
        break;

      case DT_bool:
        pbParam = va_arg( argptr, bool* );
        Buffer.Extract( &usParam );
        *pbParam = (bool)usParam;
        break;

      default:
        break;
    }
    DataType = (unsigned char)va_arg( argptr, int );
  }
  va_end( argptr );
}

// -------------------------------------------------------------------------
bool tRPCStream::ClientCall( unsigned short usProcID,
                             tRPCBuffer& InBuffer,
                             tRPCBuffer& OutBuffer )
{
  if ( !pMySocket || !pMySocket->IsOpen() )
    return false;

  if ( ClientSend( InBuffer ) )
  {
    if ( ClientReceive( OutBuffer ) )
    {
      unsigned short usParam;
      OutBuffer.Extract( &usParam );
      return ( usParam == usProcID ) ? true : false;
    }
  }
  return false;
}

// -------------------------------------------------------------------------
bool tRPCStream::ClientSend( tRPCBuffer& InBuffer )
{
  pMySocket->WriteLength( InBuffer.DataSize() + sizeof(ulMySignature) );
  // Send the signature
  pMySocket->WriteLength( ulMySignature );
  // Send the stream
  pMySocket->Write( InBuffer.c_str(), InBuffer.DataSize() );
  return true;
}

// -------------------------------------------------------------------------
bool tRPCStream::ClientReceive( tRPCBuffer& Buffer )
{
  bool fSuccess = false;

  // Wait for response
  if ( pMySocket->WaitRead( 3000 )) //ulMyTimeout ) )
  {
    uint32 ulRxSize;
    uint32 ulMsgSize;

    // Read the size of the next incomming message
    ulMsgSize = pMySocket->ReadLength();
    if ( ulMsgSize >= sizeof( uint32 ) )
    {
      // Read the last error
      ulMyLastError = pMySocket->ReadLength();
      ulMsgSize -= sizeof( ulMyLastError );

      // Read the message (data)
      char* pBuffer = new char[ulMsgSize];

      ulRxSize = 0;
      while ( ulRxSize < ulMsgSize )
      {
        if ( pMySocket->WaitRead( ulMyTimeout ) )
        {
          ulRxSize += pMySocket->ReadStream( &pBuffer[ulRxSize], ulMsgSize - ulRxSize );
        }
        else
        {
          ulMyLastError = ERR_TIMEOUT;
          delete[] pBuffer;
          return false;     // *** RETURN ***
        }
      }
      Buffer.SetData( pBuffer, ulRxSize );
    }
    fSuccess = true;
  }
  else
    ulMyLastError = ERR_TIMEOUT;

  return fSuccess;
}

// -------------------------------------------------------------------------
bool tRPCStream::ServerSend( tRPCBuffer& InBuffer )
{
  pMySocket->WriteLength( InBuffer.DataSize() + sizeof(ulMyLastError) );
  // Send the last error
  pMySocket->WriteLength( ulMyLastError );
  // Send the stream
  pMySocket->Write( InBuffer.c_str(), InBuffer.DataSize() );
  return true;
}

// -------------------------------------------------------------------------
bool tRPCStream::ServerReceive( uint32* pulSignature, tRPCBuffer& Buffer )
{
  bool fSuccess = false;

  // Wait for response
  if ( pMySocket->WaitRead( ulMyTimeout ) )
  {
    uint32 ulRxSize;
    uint32 ulMsgSize;

    // Read the size of the next incomming message
    ulMsgSize = pMySocket->ReadLength();
    if ( ulMsgSize >= sizeof( uint32 ) )
    {
      // Read the signature
      *pulSignature = pMySocket->ReadLength();
      ulMsgSize -= sizeof( *pulSignature );

      // Read the message (data)
      char* pBuffer = new char[ulMsgSize];

      ulRxSize = 0;
      while ( ulRxSize < ulMsgSize )
      {
        if ( pMySocket->WaitRead( ulMyTimeout ) )
          ulRxSize += pMySocket->ReadStream( &pBuffer[ulRxSize], ulMsgSize - ulRxSize );
        else
        {
          delete[] pBuffer;
          return false;     // *** RETURN ***
        }
      }
      Buffer.SetData( pBuffer, ulRxSize );
    }
    fSuccess = true;
  }
  return fSuccess;
}

// -------------------------------------------------------------------------
const char* tRPCStream::ErrorText( uint32 ulError )
{
  switch ( ulError )
  {
    case ERR_NONE:
      return "No error";
    case ERR_GENERIC:
      return "Generic (non-specific) interface error";
    case ERR_BAD_SIGNATURE:
      return "Interface signature mismatch";
    case ERR_INVALID_PARAMETER:
      return "Invalid parameter";
    case ERR_TIMEOUT:
      return "Timeout on call to the server";
    default:
      if ( ulError >= ERR_USER )
        return "User defined error";
  }
  return "Unknown error";
}


