/*********************************************************************/
/*  System         : Rates Server                                    */
/*  File           : rscli.C                                         */
/*  Description    : The client (AIX) class for the Rates Server     */
/*  Author         : L.Cutting                                       */
/*  Date           : 29 / Jun / 95                                   */
/*  Change History :                                                 */
/*********************************************************************/
// $Author: vince $

#ifndef _INCL_RSCLI
#define _INCL_RSCLI "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_INCL_RSCLI);

#include "nedrpc.h"
#include "rmrates.h"

#define SOCK_TIMEOUT 30000

class tRsCli
{
private:
   tRPCClient     *RPC;
char EuroList[402]; // up to 100 currencies
public:
   tRsCli(tString Host, tString UserID);
   ~tRsCli();
   long rsGetRate(pGetRates Send, pRecRates Receive);
   bool IsEuro(char *CCY);
   long SourceCCY(char *AmtIn, char *CcyIn,
 		   		char  *AmtOut,char  *CcyOut,
 		   		char  *ERIAmtOut,char *ERICcyOut); // test if in amount is legacy
   long CrossConvert( double ZARRate, double XEUate, pchar Curr, double  & aCrossRate, int Decimal = 4);// convert from one ccy to another
   long EuroCCY(char *AmtIn, char *CcyIn, char  *ERIAmtOut,char *ERICcyOut);
   long GetLegCalc(pRecLegCalc Send, tRecEuroCalc Receive);
};
#endif
//$Log: rscli.h,v $
//Revision 412.1  2004/11/18 10:26:50  vince
//no message
//
//Revision 409.1  2004/07/20 09:14:16  vince
//no message
//
//Revision 407.1  2004/06/16 09:41:38  vince
//no message
//
//Revision 405.1  2004/04/16 12:32:53  vince
//no message
//
//Revision 404.1  2004/03/27 09:40:26  vince
//no message
//
//Revision 403.1  2004/02/27 12:10:01  vince
//no message
//
//Revision 401.1  2003/12/02 14:50:09  vince
//no message
//
//Revision 310.1  2003/11/10 10:04:37  vince
//no message
//
//Revision 3.10  2003/11/07 13:54:15  vince
//no message
//
//Revision 1.1.1.1  2003/11/04 14:09:43  vince
//no message
//
//Revision 1.2  2003/09/10 15:47:02  vince
//Added Header Version Markers if not already present
//
//Revision 1.1  2003/09/10 14:29:04  vince
//New source to Version Based Software
//
//Revision 1.1  2003/09/10 13:28:49  vince
//Initial Set for Library Based Versions
//
//Revision 1.2  2002/01/11 13:43:38  vince
//no message
//
//Revision 1.1.1.1  2001/11/09 07:51:46  vince
//no message
//
//Revision 1.2  1999/05/03 07:31:54  adriana
//Revision 1.1  1999/03/01 07:38:42  adriana
//Initial revision

