// -------------------------------------------------------------------
// tranmsg.h
// Author: N. Friebus
// Account Processor Legacy Transaction Feed Message.
// -------------------------------------------------------------------

#ifndef _TRANMSG_H_
#define _TRANMSG_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_TRANMSG_H_);


// Transaction Message String




typedef struct
{
 char OrigSys[5];          //originating system
 char MsgCde[9];           //message code ex. TFFABKDP
 char TranDate[9];         //transaction date
 char TranRef[17];         //transaction reference number
 char CoNo[3];             //company number
 char AccNo[17];           //account number
 char ClientNo[11];        //client number
 char ProdCde[5];          //product code
 char TranValDate[9];      //transaction value date
 char PortfolioCde[5];     //portfolio code
 char BranchNo[5];         //branch number
 char YldNo[7];            //yield number
 char PurchAccNo[17];      //purchase account number
 char PurchCcyCde[5];      //purchase currency code
 char PurchAmt[23];        //purchase amount
 char PurchTranSubRef[17]; //purchase transaction sub reference
 char PurchTranSubCde[17]; //purchase transaction sub code
 char ExchRte[17];         //exchage rate
 char SaleProdCde[5];      //sale account product code
 char SaleAccNo[17];       //sale account number
 char SaleCcyCde[5];       //sale currency code
 char SaleAmt[23];         //sale amount
 char SaleTranSubRef[17];  //sale transaction sub ref
 char SaleTranSubCde[17];  //sale transaction sub code
 char DueDate[9];          //due date
 char TranCat[5];          //transaction category
 char ContrNo[17];         //contract number
 char ReverseEntry[2];     //reverse entry
 char LateFlg[2];          //late flag

 char PurchNostroTranRef[17]; //purchase nostro transaction ref
 char PurchNostroBrnRef[17];//purchase nostro branch ref
 char SaleNostroTranRef[17];   //sale nostro transaction ref
 char SaleNostroBrnRef[17]; //sale nostro branch ref

 char CptlAccNo[17];        //capital account number
 char CptlCcyCde[5];        //capital currency code
 char CptlAmt[23];          //capital amount
 char IntAccNo[17];         //interest account number
 char IntCcyCde[5];         //interest currency code
 char IntAmt[23];           //interest amount
 char NomAccNo[17];         //nominal account number
 char NomCcyCde[5];         //nominal currency code
 char NomAmt[23];           //nominal amount
 char ConsidAccNo[17];      //consideration account number
 char ConsidCcyCde[5];      //consideration currency code
 char ConsidAmt[23];        //consideration amount
 char InstrNo[10];          //instrument number
 char CoupledConNo [17];
 char CompensationRte[17];
 char CompensationAcc[17];
 char CompensationAmt[23];
 char CompensationCcyCde[5];
 char SpotRate[17];
 char OptionalDueDate[9];
 char ClientName[51];
 char FirstContractCat[5];
 char TranTyp[3];
 char SaleType[3];
 char PurchType[3];
 char Transwith[17];
 char BankDeliv[17];

 char LogNo[6]; // Log msg number

 char TTICRef[5];
 char HomeCcyAmt[23];
 char FormsAEInd[2];

 char SCPurchSaleInd[2];
 char SCZARPurchRate[17];
 char SCZARSaleRate[17];

} tTFtranMsg, *pTFtranMsg;


#endif

