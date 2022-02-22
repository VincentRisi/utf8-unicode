// **********************************************************************
// *** THIS IS GENERATED CODE (SnapGen Ver 2.11) - DO NOT MODIFY HERE ***
// **********************************************************************
#ifndef _AlertRPCH_
#define _AlertRPCH_

#include "ti.h"
#include "rpcstream.h"

#ifdef _MSC_VER
#pragma pack(push,1)
#endif
//--------------------------------------------------------------
struct tClient
{
  char szIPAddress[16];
  int32 lPort;
  int32 lMask;
  void Swap()
  {
    tRPCBuffer::Swap( &lPort );
    tRPCBuffer::Swap( &lMask );
  }
};

//--------------------------------------------------------------
class tAlertRPC : public tRPCStream
{
public:
  tAlertRPC( char* pszHost, char* pszService )
    : tRPCStream( 58616, new tCBSockClient( pszHost, pszService ), true ) {}
  tAlertRPC( tCBSockClient* pSocket, bool fDelSocket = false )
    : tRPCStream( 58616, pSocket, fDelSocket ) {}
  inline static const char* Version() { return "1.0.0.0"; }
  inline static const char* RPCCompiler() { return "SnapGen Ver 2.11"; }

  // User specified methods
  void Shutdown(  );
  bool Register( int32 lPort, int32 lMask );
  void Deregister( int32 lPort );
  bool Alert( int32 lID, void* pData, int32 lDataSize, int32 lSubMask );
  int32 ClientCount(  );
  bool GetClientList( tClient* pList, short* psMaxArrayLen );

  // For server use only
  bool RpcServer( uint32 ulTimeout );

private:
  tRPCStream* pMyRpc;
};

//--------------------------------------------------------------
#ifdef _MSC_VER
#pragma pack(pop)
#endif
#endif // _AlertRPCH_
