#ifndef _RMRATES_INCLUDE
#define _RMRATES_INCLUDE "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_RMRATES_INCLUDE);
/***********************************************************************/
/*  System         : Rates Server .DLL and AIX Server                  */
/*  File           : rmRates.h                                         */
/*  Description    : The common declaration of constants and RPC stcts */
/*                 : for AIX and DLL                                   */
/*  Author         : L.Cutting                                         */
/*  Date           : 22 / Jun / 1995                                   */
/*  Change History :                                                   */
/***********************************************************************/
#include "errbase.h"
#include "request.h"

// Rate Codes
#define RC_SPOT                   1
#define RC_MID                    RC_SPOT + 1
#define RC_BRN                    RC_SPOT + 2
#define RC_REPLIM                 RC_SPOT + 3
#define RC_AIR                    RC_SPOT + 4
#define RC_SEA                    RC_SPOT + 5
#define RC_BRNPIPS                RC_SPOT + 6
#define RC_FECPIPS                RC_SPOT + 7

// Other Systems Requirements
#define RC_USDMID                 RC_SPOT + 8
#define RC_ZARMID                 RC_SPOT + 9

// RPC Request ( this one is sent TO AIX )
typedef struct
{
  char     CCYCode[11];
  char     Period[11];
  short    RateCode;
} tGetRates, *pGetRates;

// RPC Receive ( this one is received FROM AIX )
typedef struct
{
  double   dRateBuy;
  double   dRateSell;
  char     PeriodDate[11];
  char     Filler1[1];
  short    PeriodDays;
  char     DateCreated[15];
  char     MultiDivInd[1];
  char     Filler2[2];
} tRecRates, *pRecRates;

// RPC Receive Euro ( this one is sent TO and received FROM the server by the DLL )
//
// If this CCY is euro legacy then EURO is returned in dRateBuy etc with this CCY detail in dRateERIBuy
// else dRateERIBuy etc is zero
//
// sqlplus:
//SQL> select to_char(10000000000000000000000.1/3) from dual;
//TO_CHAR(10000000000000000000000.1/3)
//----------------------------------------
//3333333333333333333333.36666666666666667
// ie 40 characters

typedef struct
{
  char     CCYCode[11];    // in
  char     Period[11];     // in
  short    RateCode;   // in
  char     dRateBuy[40];   // out ...
  char     dRateSell[40];
  char     dRateERIBuy[40];
  char     dRateERISell[40];
  char     PeriodDate[11];
  char     Filler1[1];
  char     PeriodDays[4];
  char     DateCreated[15];
  char     MultiDivInd[1];
  char     MultiDivIndERI[1];
} tRecEuroRates, *pRecEuroRates;

typedef struct
{
  char     CCYIn[4];    // in/out
  char     AmountIn[41];    // in/out
  char     CCYEri[4];    // out
  char     AmountEri[41];    // out
} tRecEuroCalc, *pRecEuroCalc;

typedef struct
{
  double    AmountIn;    // in/out
  double    AmountEri;    // out
  char      CCYIn[4];    // in/out
  char      CCYEri[4];    // out
} tDoubleEuroCalc, *pDoubleEuroCalc;

typedef struct
{
  char     CCYEri[4];    // in
  char     AmountEur[41];    // in
  char     AmountEri[41];    // out
} tRecLegCalc, *pRecLegCalc;

typedef struct
{
   double ZARRate;
   double XEUate;
   char Curr[4];
   int Decimal;
   double aCrossRate;
}tCrossRate, *pCrossRate;

#endif

