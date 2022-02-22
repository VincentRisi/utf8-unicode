// sarb.h
// class tSARB

// Author : Vernon Le Sueur
// Date   : October 1994.

//  Class to handle form A and E entries.

#ifndef _PASSSARB_H_
#define _PASSSARB_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_PASSSARB_H_);

//#ifdef M_AIX
//#include "nedrpc.h"
//#endif
#include "request.h"
#include "mcs.h"
#include "logfile.h"

// INI file info

#define INI_LOG_SECTION                "[LOG]"
#define INI_LOG_FILE_NAME              "{Log File Name}"
#define INI_MCS_PARAMETERS_SECTION     "[MCS Parameters]"
#define INI_MCS_PROGRAM_NAME           "{MCS Program Name}"
#define INI_MCS_NAME                   "{MCS Name}"
#define INI_MCS_NET_NO                 "{NetNo}"
#define INI_MCS_LSN                    "{LSN}"

/* Category constants */
const char *REVERSAL_CATEGORY     = "99";

typedef struct
{
    char Form[5];
    char SendingBranch[4];
    char ApplSurname[36];
    char ApplInitials[5];
    char ApplBirthdate[9];
    char Currency[4];
    char ForeignAmt[14];
    char RandAmt[14];
    char Category[3];
    char SubCategory[2];
    char DepartureDate[9];
    char FromDate[9];
    char ToDate[9];
    char Details[29];
    char ApplRefNo[7];
    char ReplyDate[9];
    char BeneSurname[36];
    char BeneInit[5];
    char BeneStatus[2];
    char GeogArea[3];
    char FormDate[9];
    char ValueDate[9];
    char AuthDealer[3];
    char CrNonResAccInd[2];
    char MicrCode[9];
    char SubCatCat1[3];
    char Residence[3];
    char EntityName[31];
} tCreateAE;


typedef struct
{
    char Form[5];
    char Branch[4];
    char SequenceNo[10];
} tReverseAE;

#ifdef _SARB_

class tSARB : public tPreSock
{
public:

//   tSARB( tString inifile) : tPreSock ( inifile ) {} ;
   tSARB( tString inifile) ;
   ~tSARB();
   void ServiceClient();
private:
   struct tResBank
   {
      char   ResCode[4];
      char   ResError[4];
      char   TranCode[5];
      char   BranchNumb[4];
      char   ApplDD[3];
      char   ApplMM[3];
      char   SeqNumber[10];
      char   ApplSurname[36];
      char   ApplInitials[5];
      char   ApplBirthDate[9];
      char   Currency[4];
      char   ForeignAmount[14];
      char   RandAmount[14];
      char   MicrCode[9];
      char   Category[3];
      char   SubCategory[2];
      char   DepartDate[9];
      char   CatDetails[29];
      char   FromDate[7];
      char   ToDate[7];
      char   BeneSurname[36];
      char   BeneInitials[5];
      char   GeogArea[3];
      char   BeneStatus[2];
      char   FormDate[9];
      char   ExcApplRef[7];
      char   ExcReplyDate[9];
      char   AuthDealer[3];
      char   CrNonResAcc[2];
      char   ValueDate[9];
      char   Filler27[27];
      char   SubCat1[3];
      char   LandRes[3];
      char   CapInvest[31];
  };

   long ValidMsg(tCreateAE &aCreateAE);
   long AddFormA ( char *pRxBuffer ,char *AERef);
   long AddFormE ( char *pRxBuffer ,char *AERef);
   long RevFormAE( char *pRxBuffer ,char *AERef);
   long SendMCS(tResBank *aResBank);
   char* Padd( char* Field , int LengthOfField);
   void LoadData(tCreateAE &aCreateAE , tResBank &aResBank);
   short ReadIniFile(tString IniFile);
   void ShowAERec(tCreateAE *AERec);

   tString     LogFilename;
   tString     Program;
   tString     MCSName;
   tString     Service;
   int         NetNo;
   int         LSN;
   tMCS        *MCS;
   tLogFile    *LogFile;
};

#else // _SARB_

class tPassSARB
{
   public:
        tPassSARB(char *Socket, char *UserID);
        ~tPassSARB();
        long SendFormA(tCreateAE &aCreateAE , char *AERef);
        long SendFormE(tCreateAE &aCreateAE , char *AERef);
        long SARBTestMsg();
        long ReverseSARB(tReverseAE &aReverseAE);
    private:
        long SendSARB(long MsgType , tCreateAE &aCreateAE , char *AERef);
//#ifdef M_AIX
//        tRPCClient      *RPC;
//#endif
};

#endif // _SARB_

#endif // _PASSSARB_H_

/*****************************************************************************************************************/
/*    The calling module must fill the structure provided.                                                       */
/*    The structure is as follows:-                                                                              */
/*                                                                                                               */
/*      Form[5]            - FRMA or FRME;                                                                       */
/*     SendingBranch[4] - Branch number must be numeric and 0 < brnno < 1000;                                    */
/*     ApplSurname[36]     - Surname must not be spaces;                                                         */
/*     ApplInitials[5]     - If there is no initials must = spaces;                                              */
/*     ApplBirthdate[9] - numeric and > todays date   CCYYMMDD;                                                  */
/*     Currency[4]           - Valid currency code not spaces;                                                   */
/*     ForeignAmt[14]      - Numeric and if = 0 then currency must be ZAR;                                       */
/*     RandAmt[14]           - Numeric and > 0;                                                                  */
/*     Category[3]           - Numeric and > 0 Must not equal 99;                                                */
/*                      Categories for FORM A are:-                                                              */
/*                      2 , 4 , 11 , 14 , 17 , 29 , 30 , 36 , 37 , 39                                            */
/*                      Categories for FORM E are:-                                                              */
/*                      01 - Capital investment in CMA                                                           */
/*                      02 - Capital repatriation by CMA resident                                                */
/*                      03 - Charter receipts i.r.o. vessels , aircraft.                                         */
/*                      04 - Commission or brokerage                                                             */
/*                      05 - Copyrights , royalties and patent fees                                              */
/*                      06 - Diplomatic transfer                                                                 */
/*                      07 - Directors fees                                                                      */
/*                      08 - Gift                                                                                */
/*                      09 - Immigrants capital transfer                                                         */
/*                      10 - Assurance / Insurance claims                                                        */
/*                      11 - Assurance / Insurance surplus funds                                                 */
/*                      12 - Income e.g. dividends , interest ,  profits                                         */
/*                      13 - Legacy                                                                              */
/*                      14 - Loan to CMA resident                                                                */
/*                      15 - Maintenance                                                                         */
/*                      16 - Office management                                                                   */
/*                      17 - Pension                                                                             */
/*                      18 - Prizes and earnings of sportsmen                                                    */
/*                      19 - Professional services                                                               */
/*                      20 - Refunds or reversals                                                                */
/*                      21 - Religious or charitable transfer                                                    */
/*                      22 - Research and technical management fees                                              */
/*                      23 - Taxes                                                                               */
/*                      24 - Freight receipts                                                                    */
/*                      25 - Travel                                                                              */
/*                      26 - Travel agents receipts                                                              */
/*                      27 - Unused travel facilities of CMA resident                                            */
/*                      28 - Advertising                                                                         */
/*                      29 - Film rentals                                                                        */
/*                      30 - Subscriptions                                                                       */
/*                      31 - Export proceeds                                                                     */
/*                      32 - Other transactions.                                                                 */
/*                                                                                                               */
/*     SubCategory[2]      - Depending on the categories the following applies                                   */
/*                     there are no sub categories for FORM E. Leave as zeroes                                   */
/*                      For category 11 the sub category must be:-                                               */
/*                         1 - FOB   - Free on board                                                             */
/*                         2 - CaF   - Carriage and freight                                                      */
/*                         3 - CIF   - Carriage , insurance and freight.                                         */
/*                      For category 29 and 30  the sub category must be:-                                       */
/*                         1 - Other countries                                                                   */
/*                         2 - Neighboring countries.                                                            */
/*                      For category 36 the sub category must be:-                                               */
/*                         1 - Father in law or step father.                                                     */
/*                         2 - Brother in law or step brother.                                                   */
/*                         3 - Mother in law or step mother                                                      */
/*                         4 - Sister in law or step sister.                                                     */
/*                      For category 37 the sub category must be:-                                               */
/*                         1 - Single                                                                            */
/*                         2 - Accompanied by spouse.                                                            */
/*                      For category 39 there are no sub categories.                                             */
/*                                                                                                               */
/*     DepartureDate[9] - Only captured for FORM A - category 29 and 30 else = zeroes CCYYMMDD;                  */
/*     FromDate[9]          - Only captured for FORM A - category 36 and 37 else = zeroes CCYYMMDD;              */
/*     ToDate[9]            - Only captured for FORM A - category 36 and 37 else = zeroes CCYYMMDD;              */
/*     Details[29]           - Only captured for FORM A - category 39 else = spaces:                             */
/*     ApplRefNo[7]       - If categories 2 , 4 , 14 or 17 are captured for FORM A then                          */
/*                      the application reference number must > 0;                                               */
/*                      If the category is 14 for FORM E then must not = zero.                                   */
/*                      If a authorised dealer is captured then the application reference number must > 0        */
/*     ReplyDate[9]       - If the application reference number is > 0 then the reply date must be a valid date  */
/*                      The reply date > today's date.                                                           */
/*     BeneSurname[36]     - Must not = spaces;                                                                  */
/*     BeneInit[5]           - If there is no initials must = spaces;                                            */
/*     BeneStatus[2]      - Must be numeric and for FORM A only;                                                 */
/*                      1 - Non resident                                                                         */
/*                      2 - Emigrant                                                                             */
/*                      3 - C M A resident                                                                       */
/*                      4 - Temporary resident of C M A.                                                         */
/*                            FORM E = zeroes.                                                                   */
/*     GeogArea[3]           - Must be numeric. > 0 and < 18 codes are:                                          */
/*                      01 - Africa;                                                                             */
/*                      02 - Benelux;                                                                            */
/*                      03 - France;                                                                             */
/*                      04 - Italy;                                                                              */
/*                      05 - Germany                                                                             */
/*                      06 - Scandinavia                                                                         */
/*                      07 - Switzerland                                                                         */
/*                      08 - United Kingdom                                                                      */
/*                      09 - Other Europe                                                                        */
/*                      10 - Middle East                                                                         */
/*                      11 - Canada                                                                              */
/*                      12 - United States of America                                                            */
/*                      13 - Other Americas                                                                      */
/*                      14 - Japan                                                                               */
/*                      15 - Asia                                                                                */
/*                      16 - Australia                                                                           */
/*                      17 - Other                                                                               */
/*     FormDate[9]         - Date form was captured. Must be valid. CCYYMMDD;                                    */
/*     ValueDate[9]     - Date the rate is of value. Must be valid CCYYMMDD;                                     */
/*     AuthDealer[3]    - If the authorised dealer is = zero then there must be no applicaton                    */
/*                      reference number                                                                         */
/*                      The authorised dealer codes are as follows:-                                             */
/*                         08 - Nedcor bank limited                                                              */
/*                         02 - First National bank                                                              */
/*                         04 - Firstcorp merchant bank                                                          */
/*                         11 - Standard bank of Namibia                                                         */
/*                         12 - Standard merchant bank                                                           */
/*                         13 - Bank of Lisbon international                                                     */
/*                         14 - French bank of South Africa                                                      */
/*                         15 - South African bank of Athens                                                     */
/*                         16 - Standard bank                                                                    */
/*                         42 - ABSA bank limited                                                                */
/*                         18 - UAL merchant bank                                                                */
/*                         56 - Bank of Taiwan                                                                   */
/*                         38 - Boland bank limited                                                              */
/*                         52 - International bank of South Africa                                               */
/*                         39 - Investec bank limited                                                            */
/*                         31 - Rand Merchant bank                                                               */
/*                         33 - Societe Generale South Africa limited                                            */
/*                         37 - Standard bank of Bophuthatswana                                                  */
/*                         34 - Trust bank of Namibia limited                                                    */
/*                         40 - Nedtravel                                                                        */
/*                         23 - Rennies travel.                                                                  */
/*                                                                                                               */
/*     CrNonResAccInd[2]   - This is a Y or N   ;                                                                */
/*     MicrCode[9]         - This is validated as per the sortcode in the branch file in the CIF system;         */
/*                                                                                                               */
/*****************************************************************************************************************/
