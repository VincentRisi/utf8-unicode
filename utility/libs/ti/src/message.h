// -------------------------------------------------------------------
// message.h
// Author: V. Risi
// Account Processor Legacy Data Accounts Add and Modify Feed Message.
// -------------------------------------------------------------------

#ifndef _MESSAGE_H_
#define _MESSAGE_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_MESSAGE_H_);

typedef char tRevDate[9], *pRevDate;

typedef struct
{
 char Language[5];
 char DlvCode[5];
 char RptPeriod[5];
 char SuppressFlag[2];
} tTFMsgStmt, *pTFMsgStmt;

typedef struct
{
   char ODLimitCde[5];
   char ODLimitAmt[23];
   char EffDate[9];
   char ExpDate[9];
} tTFMsgODLimit, *pTFMsgODLimit;

typedef struct
{
 char Code[5];
 char EffDate[9];
 char ExpDate[9];
 char MinAmount[23];
 char MaxAmount[23];
 char Diff[17];
 char DiffPerc[17];
} tTFMsgBand, *pTFMsgBand;

typedef struct
{
 char IntCde[5];
 char IntEffDate[9];
 char IntExpDate[9];
 char BaseRateCde[5];
 char NoBands[5];
} tTFMsgInt, *pTFMsgInt;

typedef struct
{
 char OrigSystem[5];
 char MsgCde[9];
 char TranCde[9];
 char MsgDate[9];
 char MsgRef[17];
 char CoNo[3];
 char AccNo[17];
 char CcyCde[4];
 char ClientNo[11];
 char ProdCde[5];
 char AccName[51];
 char DateOpen[9];
 char DateClosed[9];
 char Contact[51];
 char BlockStatusCde[5];
 char PortfolioCde[5];
 char BranchNo[5];
 char TransferAccNo[17];
 char Title[11];
 char IntFreq[5];
 char YldNo[7];
 char PayMeth[5];        // new additions
 char SARBCde[5];
 char ProdCatCde[5];
 char RevalFlg[2];
 char AccSubGrpCde[5];
 char AccCde[5];
 char DiscYldCde[5];

   //just for transfer
 char PrincAmt[23];
 char FinInstId[11];
 char OthBankAccNo[17];
 char OthAccNo[17];
 char OthYldNo[7];
 char DiaryAmt[23];

 char ResLnkBill[10];
 char FxdVarRteInd[2];
 char LstIntDivDate[9];

 char NxtDivDate[9];   //new fields from here
 char TransferBrnNo[5];
 char NoPrintLines[10];

 char MaturityDate[9];
 char MaturityAmt[23];
 char InvestPeriod[10];
 char NoticeDate[9];
 char RepayAmt[23];
 char RepayPeriod[10];
 char DrawDowns[23];
 char CommitFees[23];
 char ReInvestInd[2];
 char RollOverFlag[2];
 char PledgeFlag[2];
 char NextIntDivDate[9]; //to here

 char EventCde[5];
 char DiaryDate[9];

 char NoODLimit[5];
 char NoRevDates[5];        // 0 or more follow directly after Hdr
 char NoStmtDetails[5];     // 0 or 1    follow after that
 char NoInt[5];             // 0 or more follow after that

} tTFMsgHdr, *pTFMsgHdr;

 pTFMsgStmt MsgStmnt;
 pTFMsgInt MsgInt;
 pRevDate RevDates;


#endif

