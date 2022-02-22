// ***************************************************
// *** THIS IS GENERATED CODE - DO NOT MODIFY HERE ***
// ***************************************************
#ifndef _RateServerRPCH_
#define _RateServerRPCH_"$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_RateServerRPCH_);
#include "ti.h"
#include "rpcstream.h"

//--------------------------------------------------------------
struct tRate
{
  char szBand[15];
  char fValid;
  long lDate;
  long lDays;
  double dRate;
  void Swap()
  {
    tRPCBuffer::Swap( &lDate );
    tRPCBuffer::Swap( &lDays );
    tRPCBuffer::Swap( &dRate );
  }
};

struct tCurve
{
  char szName[15];
  char szDescription[50];
  char szCurrID[4];
  char cCurveType;
  char cExtrapolate;
  char cValid;
  short sConventionID;
  short sRateType;
  short sInterpType;
  short sBandCount;
  void Swap()
  {
    tRPCBuffer::Swap( &sConventionID );
    tRPCBuffer::Swap( &sRateType );
    tRPCBuffer::Swap( &sInterpType );
    tRPCBuffer::Swap( &sBandCount );
  }
};

struct tRateStats
{
  short sCurveCount;
  short sManualCount;
  short sReutersCount;
  short sTelerateCount;
  void Swap()
  {
    tRPCBuffer::Swap( &sCurveCount );
    tRPCBuffer::Swap( &sManualCount );
    tRPCBuffer::Swap( &sReutersCount );
    tRPCBuffer::Swap( &sTelerateCount );
  }
};

//--------------------------------------------------------------
class tRateServerRPC : public tRPCStream
{
public:
  tRateServerRPC( char* pszHost, char* pszService )
    : pszMyVersion( "1.0.0.0" )
    , tRPCStream( new tCBSockClient( pszHost, pszService ), true ) {}
  tRateServerRPC( tCBSockClient* pSocket )
    : pszMyVersion( "1.0.0.0" )
    , tRPCStream( pSocket, false ) {}
  const char* Version() { return pszMyVersion; }

  // User specified methods
  bool GetStats( tRateStats* pRateStats );
  bool GetCurveList( char* pCurveList, long lBuffSize );
  bool IsHoliday( long lDate, char* pszCurrency );
  void GetExchRate( char* pszCurveName, long lDate, double* pRate );
  bool GetCurveRate( char* pszCurveName, long RateType, long lDate, double* pRate );
  bool GetManualRate( char* pszName, long RateType, long lDays, double* pdRate );
  bool GetReutersRate( char* pszName, long RateType, long lDays, double* pdRate );
  bool GetCurveInfo( tCurve* pCurve );
  bool GetCurve( char* pszCurveName, tRate* pRateArray, long* plMaxArrayLen, long RateType );
  bool WriteDumpFile( char* pszFilename );
  void TestCall( char* pBuffer, long lMaxSize );
  bool GetCurveInfo2( char* pszCurveName, char* pszCurrency, long lCurrSize, long* pRateType, long* plBandCount );
  bool GetCurve2( char* pszCurveName, char* pszCurrency, long lCurrSize, tRate* pRateArray, long* plMaxArrayLen, long RateType );

  // For server use only
  bool RpcServer( unsigned long ulTimeout );

private:
  const char* pszMyVersion;
  tRPCStream* pMyRpc;
};

//--------------------------------------------------------------
#endif // _RateServerRPCH_
