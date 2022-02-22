// acb.h
// class tACBMod

// Author : Vernon Le Sueur
// Date   : February 1995.

//  Class to handle acb entries.

#ifndef _ACB_H_
#define _ACB_H_ "$Revision: 91 $ $Date: 2005-04-21 11:30:22 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_ACB_H_);

/* ERROR CODES FOR ACB */

const long  VALID_ACB_SUCCESSFUL    					    = ERROR_OK;
const long  ERROR_ACB_ACCOUNT_NOT_NUMERIC    	    = ERROR_ACB_BASE + 1;
const long  ERROR_ACB_ACCOUNT_INVALID             = ERROR_ACB_BASE + 2;
const long  ERROR_ACB_BRANCH_NOT_NUMERIC          = ERROR_ACB_BASE + 3;
const long  ERROR_ACB_BRANCH_INVALID              = ERROR_ACB_BASE + 4;
const long  ERROR_ACB_AMOUNT_NOT_NUMERIC          = ERROR_ACB_BASE + 5;
const long  ERROR_ACB_AMOUNT_INVALID              = ERROR_ACB_BASE + 6;
const long  ERROR_ACB_SOURCE_ACCOUNT_NOT_NUMERIC  = ERROR_ACB_BASE + 7;
const long  ERROR_ACB_SOURCE_ACCOUNT_INVALID      = ERROR_ACB_BASE + 8;
const long  ERROR_ACB_INVALID_LENGTH              = ERROR_ACB_BASE + 9;
const long  ERROR_ACB_TRAN_TYPE_INVALID           = ERROR_ACB_BASE + 10;
const long  ERROR_ACB_ONLINE_DOWN                 = ERROR_ACB_BASE + 11;
const long  ERROR_ACB_INVALID_CRCR_CODE           = ERROR_ACB_BASE + 12;
const long  ERROR_ACB_UNIDENTIFIED_ERROR          = ERROR_ACB_BASE + 13;
const long  ERROR_ACB_WRONG_CLEARING_NO           = ERROR_ACB_BASE + 14;
const long  ERROR_ACB_WRONG_NEDBANK_ACC_TYPE      = ERROR_ACB_BASE + 15;
const long  ERROR_ACB_NED_BRANCH_NEQL_CLEARING_NO = ERROR_ACB_BASE + 16;
const long  ERROR_ACB_INVALID_ACCOUNT_TYPE        = ERROR_ACB_BASE + 17;
const long  ERROR_ACB_ACCNO_NOT_MODULAR           = ERROR_ACB_BASE + 18;
const long  ERROR_ACB_FNB_ACCNO_NOT_MOD_10        = ERROR_ACB_BASE + 19;
const long  ERROR_ACB_TRACE_NO_NOT_NUMERIC        = ERROR_ACB_BASE + 20;
const long  ERROR_ACB_INVALID_MSG_TYPE            = ERROR_ACB_BASE + 21;
const long  ERROR_ACB_UNSPEC_EXCEPTION            = ERROR_ACB_BASE + 22;
const long  ERROR_ACB_INVALID_REQUEST             = ERROR_INVALID_REQUEST;

/* REQUEST CODES FOR ACB */

const long	REQ_ACB_MSG                           = REQ_ACB_BASE + 1;
const long	REQ_ACB_END_BATCH                     = REQ_ACB_BASE + 2;

typedef struct
{
	char DestBrnNo[7];
	char DestAccNo[17];
	char AmtInCents[13];
	char Reference[31];
	char SourceAccNo[21];
	char TranType[3];
} tCreateACBMsg;


#ifdef _ACBMOD_

class tACBMod : public tPreSock
{
public:

	tACBMod( tString inifile) : tPreSock ( inifile ) {};
	~tACBMod();
	void ServiceClient();
private:
struct tACB
{
  char   CrCr[6];
  char   RecIdent[3];
  char   DestBrnNo[7];
  char   DestAccNo[17];
  char   AmtInCents[13];
  char   Reference[31];
  char   TraceNo[11];
  char   SourceAccNo[21];
  char   EntryClass[3];
};

	long ValidMsg(tCreateACBMsg &aCreateACBMsg);
	long AddACBRec  ( char *pRxBuffer );
	void LoadData(tCreateACBMsg &aCreateACBMsg , tACB &aACB);
	long EndBatch();
	char* Padd( char* Field , int LengthOfField);
	long SendMCS(tACB *aACB);
};

#else // _ACBMOD_

class tPassACB
{
	
	public:
		tPassACB(char *Socket, char *UserID);
		~tPassACB();

		long SendACBMsg(unsigned short MsgType , tCreateACBMsg &aCreateACBMsg);
  private:
    char   _Socket[80];
    char   _UserID[80];
};

#endif // _ACBMOD_

#endif // _ACBMOD_H_

/**************************************************************************************************************************/
/* 	The calling module must fill the structure provided.                                                                  */
/*	The fields that are not catured for for a particular message type must be left blank.                                 */
/*	The structure is as follows:-                                                                                         */
/*  DestBrnNo    - Clients account that will be credited                                                                  */
/*  DestAccNo    - The first six digits of the branch to be credited. The digits on the top right hand corner of a cheque */
/*  AmtInCents   - The amount in cents.                                                                                   */
/*  Reference    - must contain a valid explanation of the transaction.                                                   */
/*  SourceAccNo  - This is the ned bank account number that will be debited to create this transaction.                   */
/*  TranType     - For the meantime this will only be a credit                                                            */
/*	                                                                                                                      */
/*	The message types are as follows:-                                                                                    */
/*		2501 - Normal ACB transaction.                                                                                         */
/**************************************************************************************************************************/
