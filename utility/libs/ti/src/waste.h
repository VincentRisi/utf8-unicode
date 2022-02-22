// waste.h
// class tWasteMod
//***************************************************************************
// Author : Vernon Le Sueur                                                 *
// Date   : September 1994.                                                 *
//                                                                          *
//  Class to handle waste entries.                                          *
// 12Sept97: Geoff Bruce added WasteNstMsg() for OMNI                       *
// 06April98 Geoff Implement HostName in   tPassWaste                       *
//                                                                          *
//***************************************************************************

#ifndef _WASTE_H_
#define _WASTE_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_WASTE_H_);

#include "nedrpc.h"
#include "logfile.h"
#include "request.h"

// INI file info

#define INI_LOG_SECTION                "[LOG]"
#define INI_LOG_FILE_NAME              "{Log File Name}"
#define INI_LOG_IC_CENTRE_FAF2_PREFIX  "{IC Centre Prefix}"
#define INI_LOG_CREATE_FAF2_LOG        "{Create FAF2 log}"
#define INI_LOG_SEQ_NO_FILENAME        "{SeqNo File Name}"
#define INI_MCS_PARAMETERS_SECTION     "[MCS Parameters]"
#define INI_MCS_PROGRAM_NAME           "{MCS Program Name}"
#define INI_MCS_NAME                   "{MCS Name}"
#define INI_MCS_NET_NO                 "{NetNo}"
#define INI_MCS_LSN                    "{LSN}"
#define INI_MCS_SEND                   "{Send to MCS? (0/1)}"
#define INI_LOG_ERROR_MAX              "{Consecutive Error Max}"

typedef struct
{
   char AcctNum[11];
   char AmtInCents[14];
   char TransCode[5];
   char TransDate[9];
   char TransTime[9];
   char BranchNum[5];
   char TermNum[9];
   char EntryDescr[60];
   char DrCrInd[3];
   char SwiftAddr[12];
   char TheirRef[17];
   char OurRef[17];
   char Filler[1];
} tCreateWasteMsg;

typedef struct
{
   char DrAcctNum[11];
   char CrAcctNum[11];
   char AmtInCents[14];
   char SwiftField72[2];
   char Filler[2];
} tEnqWasteMsg;

#ifdef _WASTEMOD_

class tWasteMod : public tPostSock
{
public:
   tWasteMod( tString inifile) ;
   ~tWasteMod();
   void ServiceClient();

private:
   struct tWaste {
      char   CrCr[6];
      char   WasteIdent[5];
      char   SourceCompany[4];
      char   SourceComputer[2];
      char   SourceProgram[9];
      char   SourceBranch[5];
      char   SourceTerm[9];
      char   SourceTermSeq[10];
      char   SourceMachine[5];
      char   SourceDevice[2];
      char   SourceDeviceName[5];
      char   SourceProduct[7];
      char   SourceMarket[3];
      char   SourceAccountType[3];
      char   SourceFiller[3];
      char   SourceEmp1[7];
      char   SourceEmp2[7];
      char   SourceTrace[10];
      char   AccSystem[4];
      char   AccountNumber[17];
      char   AccBranch[9];
      char   TransType[2];
      char   TransCode[5];
      char   TransDate[9];
      char   BackDatedTo[9];
      char   TransTime[7];
      char   Sign[2];
      char   AmountInCents[14];
      char   Description[97];
      char   SentOnline[2];
      char   OnlineBatch[2];
      char   TmStamp[15];
      };

   long ValidMsg(tCreateWasteMsg &aCreateWasteMsg);
   void ChkDigit    ( char* AcctNumb);
   long AddNormRec  ( char *pRxBuffer );
   long AddSwiftRec ( char *pRxBuffer );
   long AddBatchRec ( char *pRxBuffer );
   long AddNstRec  ( char *pRxBuffer, char *system );
   void formatWaste(tWaste *aWaste);
   void LoadData(tCreateWasteMsg &aCreateWasteMsg , tWaste &aWaste);
   long EndBatch();
   long TestMsg();
   char* Padd( char* Field , int LengthOfField);
   long SendMCS(tWaste *aWaste , char *WasteBuffer);
   long FetchEnq(char *pRxBuffer , char *SARBInd);
   short ReadIniFile(tString IniFile);
   void ShowWasteRec(tWaste *aWaste);
   void GetSeqNoFromFile(long *SeqNo);
   void UpdateSeqNoInFile(long NewSeqNo);


   tString     LogFilename;
   char        ICLogFileName[80];
   tString     ICLogFilePrefix;
   tString     SeqNoFileName;
   FILE        *fICLogFile;
   FILE        *fICSeqFile;
   tString     Program;
   tString     MCSName;
   tString     Service;
   tString     ServerKey;
   long        MCSsend;
   int         NetNo;
   int         LSN;
   int         CreateFAF2Log;
   long        SeqNo;
   tMCS        *MCS;
   tLogFile    *LogFile;
   long        ReplyErrorCount,ReplyErrorMax;
};


#else // _WASTEMOD_

class tPassWaste
{

   public:
      tPassWaste(char *Socket, char *UserID,char *HostName = 0); // new
    //  tPassWaste(char *Socket, char *UserID); // old
      ~tPassWaste();

      long WasteOnlMsg(tCreateWasteMsg &aCreateWasteMsg);
      long WasteOnlSwiftMsg(tCreateWasteMsg &aCreateWasteMsg,char SARBInd);
      long WasteOnlBatchMsg(tCreateWasteMsg &aCreateWasteMsg);
      long WasteBatchEnd();
      long WasteTestMsg();
      long WasteEnq(tEnqWasteMsg &aEnqWasteMsg , char *SARBInd);
      long WasteNstMsg(char *Msg,long Request);/* new for OMNI */

   private:
      short    cbTxBuffer;
      long SendWasteMsg(long MsgType, tCreateWasteMsg &aCreateWasteMsg);
      long tPassWaste::SendWasteMsg(long MsgType, char *Msg);
      tRPCClient     *RPC;
};

#endif   // _WASTEMOD_

#endif   // _WASTE_H_

/****************************************************************************************************/
/*    The calling module must fill the structure provided.                                          */
/* The fields that are not catured for for a particular message type must be left blank.            */
/* The structure is as follows:-                                                                    */
/*    AcctNum[11]      - Clients account number                                                     */
/*    AmtInCents[14]    - The amount in cents.                                                      */
/*    TransCode[5]     - Transaction codes.                                                         */
/*    TransDate[9]     - Transaction date.                                                          */
/*    TransTime[9]     - Time transaction took place                                                */
/*    BranchNum[5]     - Branch that captured the transaction.                                      */
/*    TermNum[9]       - This is a four digit terminal number and not the IP address.               */
/*    EntryDescr[60]    - Valid description.  For SWIFT the description must only be 34 charaters.  */
/*    DrCrInd[3]       - The following 4 fieldsare for swift related entries only ie DR or CR       */
/*    SwiftAddr[12]   - Swift address.                                                              */
/*    TheirRef[17]     - Their reference number.                                                    */
/*    OurRef[17]       - Our reference number.                                                      */
/*                                                                                                  */
/* The message types are as follows:-                                                               */
/*    2401 - Normal online transaction.                                                             */
/*    2402 - Normal online Swift transaction.                                                       */
/*    2403 - Batch transaction.                                                                     */
/*    2404 - Indication that the batch is complete - only AP rest of string is null.                */
/*    2405 - To test whether FAPMOR is upon the MVS machine.                                        */
/****************************************************************************************************/
