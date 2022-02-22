// **********************************************************************
// *** THIS IS GENERATED CODE (SnapGen Ver 2.11) - DO NOT MODIFY HERE ***
// **********************************************************************

#include "ti.h"
#include "rpcstream.h"
#include "alertrpc.h"

//--------------------------------------------------------------
enum eAlertRPCProcID {
      PROCID_Shutdown = 1
    , PROCID_Register
    , PROCID_Deregister
    , PROCID_Alert
    , PROCID_ClientCount
    , PROCID_GetClientList
};
void tAlertRPC::Shutdown(  )
{
  const unsigned short usProcID = PROCID_Shutdown;
  tRPCBuffer InBuffer( 100 );
  tRPCBuffer OutBuffer;
  WriteStream( InBuffer, usProcID, DT_end );
  if ( ClientCall( usProcID, InBuffer, OutBuffer ) )
  {
    ReadStream( OutBuffer, usProcID, DT_end );
    return;
  }
  throw LastError();
}
bool tAlertRPC::Register( int32 lPort, int32 lMask )
{
  const unsigned short usProcID = PROCID_Register;
  tRPCBuffer InBuffer( 100 );
  tRPCBuffer OutBuffer;
  WriteStream( InBuffer, usProcID
             , DT_int4, lPort
             , DT_int4, lMask, DT_end );
  if ( ClientCall( usProcID, InBuffer, OutBuffer ) )
  {
    bool _idl_retVar;
    ReadStream( OutBuffer, usProcID
              , DT_bool, &_idl_retVar, DT_end );
    return _idl_retVar;
  }
  throw LastError();
  return (bool)0;  // Required by some compilers
}
void tAlertRPC::Deregister( int32 lPort )
{
  const unsigned short usProcID = PROCID_Deregister;
  tRPCBuffer InBuffer( 100 );
  tRPCBuffer OutBuffer;
  WriteStream( InBuffer, usProcID
             , DT_int4, lPort, DT_end );
  if ( ClientCall( usProcID, InBuffer, OutBuffer ) )
  {
    ReadStream( OutBuffer, usProcID
, DT_end );
    return;
  }
  throw LastError();
}
bool tAlertRPC::Alert( int32 lID, void* pData, int32 lDataSize, int32 lSubMask )
{
  const unsigned short usProcID = PROCID_Alert;
  tRPCBuffer InBuffer( 100 );
  tRPCBuffer OutBuffer;
  WriteStream( InBuffer, usProcID
             , DT_int4, lID
             , DT_void, pData, (unsigned short)lDataSize
             , DT_int4, lDataSize
             , DT_int4, lSubMask, DT_end );
  if ( ClientCall( usProcID, InBuffer, OutBuffer ) )
  {
    bool _idl_retVar;
    ReadStream( OutBuffer, usProcID
              , DT_bool, &_idl_retVar, DT_end );
    return _idl_retVar;
  }
  throw LastError();
  return (bool)0;  // Required by some compilers
}
int32 tAlertRPC::ClientCount(  )
{
  const unsigned short usProcID = PROCID_ClientCount;
  tRPCBuffer InBuffer( 100 );
  tRPCBuffer OutBuffer;
  WriteStream( InBuffer, usProcID, DT_end );
  if ( ClientCall( usProcID, InBuffer, OutBuffer ) )
  {
    int32 _idl_retVar;
    ReadStream( OutBuffer, usProcID
              , DT_int4, &_idl_retVar, DT_end );
    return _idl_retVar;
  }
  throw LastError();
  return (int32)0;  // Required by some compilers
}
bool tAlertRPC::GetClientList( tClient* pList, short* psMaxArrayLen )
{
  const unsigned short usProcID = PROCID_GetClientList;
  tRPCBuffer InBuffer( 100 );
  tRPCBuffer OutBuffer;
  WriteStream( InBuffer, usProcID
             , DT_int2, *psMaxArrayLen
             , DT_newsize, (unsigned short)(*psMaxArrayLen*sizeof(tClient)), DT_end );
  if ( ClientCall( usProcID, InBuffer, OutBuffer ) )
  {
    bool _idl_retVar;
    ReadStream( OutBuffer, usProcID
              , DT_int2, psMaxArrayLen
              , DT_selvoid, pList, (unsigned short)(*psMaxArrayLen*sizeof(*pList))
              , DT_bool, &_idl_retVar, DT_end );
    int32 _idl_i;
    for( _idl_i=0; _idl_i<*psMaxArrayLen; _idl_i++ )
    {
      pList[_idl_i].Swap();
    }
    return _idl_retVar;
  }
  throw LastError();
  return (bool)0;  // Required by some compilers
}
