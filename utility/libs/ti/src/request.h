#ifndef _REQUEST_INCLUDE
#define _REQUEST_INCLUDE

/***********************************************************************/
/*  System : Treasury                                                  */
/*  File   : Request.h                                                 */
/*  Target :                                                           */
/*  Author : S. Shemesh                                                */
/*  Date   : Mon 25-Jul-1994                                           */
/*  Change History :                                                   */
/***********************************************************************/

#define REQ_DM_BASE          2000 // Diary sub-system
#define REQ_ST_BASE          2100 // settlement base request
#define REQ_DT_BASE          2200 // server base request
#define REQ_SF_BASE          2300 // Store And forward
#define REQ_WT_BASE          2400 // Waste
#define REQ_ACB_BASE         2500 // ACB
#define REQ_AE_BASE          2600 // Form A and E For SARB
#define REQ_SWIFT_BASE       2800 // Swift
#define REQ_RESEND_BASE      2900 // Swift Resend
#define REQ_LC_BASE          3000 // Letters of credit base request
#define REQ_LU_BASE          3100 // Limits and utilisations base request
#define REQ_CIF_BASE         3200 // Customer Information base request
#define REQ_SC_BASE          3300 // Security sub-system: 3300..3399
#define REQ_CC_BASE          3400 // Commission & Charges sub-system: 3400..3499
#define REQ_RS_BASE          3500 // Rates Server ( Leslie )
#define REQ_NGS_BASE         3900 // Generic SWIFT (Adriana)
#define REQ_NTDS_BASE        4000 // NTDS ( Matthew )
#define REQ_GENCS_BASE       4100 // Generic Server (Geoff)
#define REQ_VB_BASE          5000 // VB   ( Margie )
#define REQ_ITBOS_BASE       5500 // Intergrated Treasury Back Office System (Matthew)
#define REQ_CTS_BASE         6000 // Cost to Sqaure on NTDS   ( Margie )
#define REQ_BP_BASE          7000 // Bulk Payments (NR)
#define REQ_DTSWIFT_BASE     8000 //SWIFT FORMATTER
#define REQ_STP_BASE         9000 //STP FORMATTER
#define REQ_TIBOS_REAL_BASE  10000  // Tibos Real Time Engine

// Diary module request codes
#define REQ_DM_CREATE_SIMPLE_EVENT        (REQ_DM_BASE    )
#define REQ_DM_CREATE_REPEAT_EVENT        (REQ_DM_BASE + 1)
#define REQ_DM_CREATE_MONTHLY_EVENT       (REQ_DM_BASE + 2)
#define REQ_DM_DELETE_EVENT               (REQ_DM_BASE + 3)
#define REQ_DM_QUERY_EVENTS               (REQ_DM_BASE + 4)
#define REQ_DM_VIEW_EVENT                 (REQ_DM_BASE + 5)
#define REQ_DM_EVENT                      (REQ_DM_BASE + 6)

// Settlement module request id's
#define REQ_ST_CHECK_ALIVE                REQ_ST_BASE
#define REQ_ST_BUILD_TX                   (REQ_ST_BASE+1)   // stBuildTX API
#define REQ_ST_DISCARD                    (REQ_ST_BASE+2)   // stDiscard API
#define REQ_ST_PASS_TX                    (REQ_ST_BASE+3)   // stPassTX  API
#define REQ_ST_PROCESS_TX                 (REQ_ST_BASE+4)   // stProcessTX API
#define REQ_ST_RAND_EQUIV                 (REQ_ST_BASE+5)   // Rand equivalent API
#define REQ_ST_FAP_ENTRY_QUERY            (REQ_ST_BASE+6)
#define REQ_ST_WASTE_ENTRY_QUERY          (REQ_ST_BASE+7)
#define REQ_ST_CHANGE_REFNO               (REQ_ST_BASE+8)
#define REQ_ST_CONVERT_TO_RANDS           (REQ_ST_BASE+9)
#define REQ_ST_ZAR_SPOT                   (REQ_ST_BASE+10)
#define REQ_ST_SARB_IND                   (REQ_ST_BASE+11)
#define REQ_ST_FORN_CASH_MAN_QUERY       (REQ_ST_BASE+12)

// Limits and Utilisations module request id's
#define REQ_LU_REQUESTLIMITAPPROVAL       REQ_LU_BASE
#define REQ_LU_FORCELIMITAPPROVAL         (REQ_LU_BASE+1)
#define REQ_LU_MOVEPENDINGTOACTUAL        (REQ_LU_BASE+2)
#define REQ_LU_CHANGEUTILISATION          (REQ_LU_BASE+3)
#define REQ_LU_CANCELPENDING              (REQ_LU_BASE+4)
#define REQ_LU_SHUTDOWN                   (REQ_LU_BASE+5)
#define REQ_LU_QUERYLIMITREFUSAL          (REQ_LU_BASE+6)

// Swift Request IDs
#define REQ_SWIFT_CHECK_ALIVE    REQ_SWIFT_BASE
#define REQ_SWIFT_STATS          REQ_SWIFT_BASE + 1   // WS Request for Stats Info
#define REQ_SWIFT_REROUTE        REQ_SWIFT_BASE + 2   // Ws Request to reroute message
#define REQ_SWIFT_ROUTE          REQ_SWIFT_BASE + 3   // server Request to route message
#define REQ_SWIFT_SEND           REQ_SWIFT_BASE + 4   // client Request to send message
#define REQ_SWIFT_CLOSE          REQ_SWIFT_BASE + 5   // Close ST400INP If ST400OUT Terminate
#define REQ_SWIFT_RESEND         REQ_SWIFT_BASE + 6   // Force resend of previously sent message
#define REQ_SWIFT_ACTIVE         REQ_SWIFT_BASE + 7   // Check if ST400INP is active
#define REQ_SWIFT_ST400          REQ_SWIFT_BASE + 8   //
#define REQ_SWIFT_ACKNAK         REQ_SWIFT_BASE + 9   //

// Swift Resend Request IDs
#define REQ_RESEND_CHECK_ALIVE    REQ_RESEND_BASE
#define REQ_RESEND_UPDSWIFT       REQ_RESEND_BASE + 1   // WS Request for Stats Info
#define REQ_RESEND_QUERY          REQ_RESEND_BASE + 2   // Ws Request to reroute message
#define REQ_RESEND_COMMSTAT       REQ_RESEND_BASE + 3   // Ws Request to reroute message
#define REQ_RESEND_GETISN         REQ_RESEND_BASE + 4   // Ws Request to reroute message
#define REQ_RESEND_UPDAUDIT       REQ_RESEND_BASE + 5   // Ws Request to reroute message
#define REQ_RESEND_HISTORY        REQ_RESEND_BASE + 6   // Ws Request to reroute message
#define REQ_RESEND_RESEND         REQ_RESEND_BASE + 7   // Ws Request to reroute message
#define REQ_RESEND_GETINFO        REQ_RESEND_BASE + 8   // Ws Request to reroute message
#define REQ_RESEND_COUNTHIST      REQ_RESEND_BASE + 9   // Ws Request to reroute message
#define REQ_RESEND_GETSWIFT       REQ_RESEND_BASE + 10  // Ws Request to reroute message
#define REQ_RESEND_LASTQUERY      REQ_RESEND_BASE + 12  // Ws Request to reroute message

// CIF Request IDs
#define REQ_CIF_READ_BY_IP              (REQ_CIF_BASE + 1)
#define REQ_CIF_READ_BY_ACCOUNT         (REQ_CIF_BASE + 2)
#define REQ_CIF_READ_BY_SWIFT           (REQ_CIF_BASE + 3)
#define REQ_CIF_GET_INFO                (REQ_CIF_BASE + 4)
#define REQ_CIF_GET_NAMES               (REQ_CIF_BASE + 5)
#define REQ_CIF_GET_ADDRESS             (REQ_CIF_BASE + 6)
#define REQ_CIF_GET_ID                  (REQ_CIF_BASE + 7)
#define REQ_CIF_GET_ACC                 (REQ_CIF_BASE + 8)
#define REQ_CIF_GET_ADDTYPE             (REQ_CIF_BASE + 9)
#define REQ_CIF_GET_ACCTYPE             (REQ_CIF_BASE + 10)
#define REQ_CIF_GET_IDTYPE              (REQ_CIF_BASE + 11)
#define REQ_CIF_GET_NOSTS               (REQ_CIF_BASE + 12)
#define REQ_CIF_GET_SWIFT               (REQ_CIF_BASE + 13)
#define REQ_CIF_TOTAL_NAMES             (REQ_CIF_BASE + 14)
#define REQ_CIF_GET_ACCOUNT             (REQ_CIF_BASE + 15)
#define REQ_CIF_GET_DRAFTS              (REQ_CIF_BASE + 16)
#define REQ_CIF_VALID_IND               (REQ_CIF_BASE + 17)
#define REQ_CIF_GET_VOSTRO              (REQ_CIF_BASE + 18)
#define REQ_CIF_GET_ZAPS                (REQ_CIF_BASE + 19)
#define REQ_CIF_GET_ACC_STATUS          (REQ_CIF_BASE + 20)
#define REQ_CIF_CAT_NAMES               (REQ_CIF_BASE + 21)
#define REQ_CIF_UCAT_NAMES              (REQ_CIF_BASE + 22)
#define REQ_IP_CAT_LIST                 (REQ_CIF_BASE + 23)
#define REQ_TIBOS_STMNT_ADDR            (REQ_CIF_BASE + 24)
#define REQ_SOMECIF_BY_IP               (REQ_CIF_BASE + 25)
#define REQ_CFMSCIF_GET_NAMES           (REQ_CIF_BASE + 26)

// General messages
#define REQ_SC_LOGIN                        REQ_SC_BASE
#define REQ_SC_VLOGIN                      (REQ_SC_BASE+ 1)
#define REQ_SC_LOGOUT                      (REQ_SC_BASE+ 2)
#define REQ_SC_REGISTER_WORKSTATION        (REQ_SC_BASE+ 3)
#define REQ_SC_USER_CHANGE_PASSWD          (REQ_SC_BASE+ 4)
// Supervisor messages - unrestricted
#define REQ_SC_REFRESH_CTLTYPES            (REQ_SC_BASE+ 5)
#define REQ_SC_ALIVE                       (REQ_SC_BASE+ 6)
#define REQ_SC_LOG_QUERY                   (REQ_SC_BASE+ 7)
// Supervisor messages - restricted
#define REQ_SC_WORKSTATION_RESET           (REQ_SC_BASE+ 8)
#define REQ_SC_WORKSTATION_SELECTONE       (REQ_SC_BASE+ 9)
#define REQ_SC_WORKSTATION_INSERT          (REQ_SC_BASE+10)
#define REQ_SC_WORKSTATION_DELETE          (REQ_SC_BASE+11)
#define REQ_SC_WORKSTATION_UPDATE          (REQ_SC_BASE+12)
#define REQ_SC_USER_RESET                  (REQ_SC_BASE+13)
#define REQ_SC_USER_SELECTONE              (REQ_SC_BASE+14)
#define REQ_SC_USER_INSERT                 (REQ_SC_BASE+15)
#define REQ_SC_USER_DELETE                 (REQ_SC_BASE+16)
#define REQ_SC_USER_UPDATE                 (REQ_SC_BASE+17)
#define REQ_SC_REQUESTS_ENABLE             (REQ_SC_BASE+19)
#define REQ_SC_REQUESTS_DISABLE            (REQ_SC_BASE+21)
#define REQ_SC_LOGINS_ENABLE               (REQ_SC_BASE+22)
#define REQ_SC_LOGINS_DISABLE              (REQ_SC_BASE+23)
#define REQ_SC_SHUTDOWN                    (REQ_SC_BASE+24)
#define REQ_SC_RPCSTATUS                   (REQ_SC_BASE+25)
// AIX Servers as Client masseges
#define REQ_SC_VALIDATERQ                  (REQ_SC_BASE+26)
// SmartCard messages
#define REQ_SC_SMARTCARD_SELECTONE         (REQ_SC_BASE+27)
#define REQ_SC_SMARTCARD_INSERT            (REQ_SC_BASE+28)
#define REQ_SC_SMARTCARD_DELETE            (REQ_SC_BASE+29)
#define REQ_SC_MAC_CHANGE                  (REQ_SC_BASE+30)
#define REQ_SC_LOGIN_SMARTCARD             (REQ_SC_BASE+31)
#define REQ_SC_VLOGIN_SMARTCARD            (REQ_SC_BASE+32)
#define REQ_SC_TRUSTED_LOGIN               (REQ_SC_BASE+33)
// AIX Servers as Trusted Client masseges
#define REQ_SC_TRUSTED_VALIDATERQ          (REQ_SC_BASE+34)
// Monitor messages
#define REQ_SC_SPY_LOG                     (REQ_SC_BASE+40)
#define REQ_SC_SPY_USERS                   (REQ_SC_BASE+41)
#define REQ_SC_SPY_WORKSTATIONS            (REQ_SC_BASE+42)
#define REQ_SC_SPY_STATS                   (REQ_SC_BASE+43)
// Profile Service Request
#define REQ_SC_USER_PROFILE                (REQ_SC_BASE+51)
#define REQ_SC_PROFILE_CTLTYPES            (REQ_SC_BASE+52)
// Datetime request
#define REQ_SC_GET_DATETIME                (REQ_SC_BASE+60)
#define REQ_SC_VALIDATE_USER_PASSWORD      (REQ_SC_BASE+70)
// Castor and Pollux
#define REQ_SC_RECONNECT                   (REQ_SC_BASE+255)


#define REQ_DT_SHUTDOWN             REQ_DT_BASE
#define REQ_DT_DISCARD              REQ_DT_BASE + 1
#define REQ_DT_GET                  REQ_DT_BASE + 2
#define REQ_DT_QUERY                REQ_DT_BASE + 3
#define REQ_DT_REROUTE              REQ_DT_BASE + 4
#define REQ_DT_SUBMIT               REQ_DT_BASE + 5
#define REQ_DT_UNLOCK               REQ_DT_BASE + 6
#define REQ_DT_SF                   REQ_DT_BASE + 7
#define REQ_DT_DIARY                REQ_DT_BASE + 8
#define REQ_DT_ISRECLOCKED          REQ_DT_BASE + 9
#define REQ_DT_PRINTQUERY           REQ_DT_BASE + 10
#define REQ_DT_PAYMENTS             REQ_DT_BASE + 11
#define REQ_DT_REPRINT              REQ_DT_BASE + 12
#define REQ_DT_CLIENTQUERY          REQ_DT_BASE + 13
#define REQ_DT_UPDATEPAYMENTS       REQ_DT_BASE + 14
#define REQ_DT_LOCK                 REQ_DT_BASE + 15
#define REQ_DT_PROCESS              REQ_DT_BASE + 16
#define REQ_DT_UPDATESTATUS         REQ_DT_BASE + 17
#define REQ_DT_CALCRANDEQUIV        REQ_DT_BASE + 18
#define REQ_DT_SUBMITMT             REQ_DT_BASE + 19
#define REQ_DT_GETMT                REQ_DT_BASE + 20
#define REQ_DT_QUERYLOCKED          REQ_DT_BASE + 21
#define REQ_DT_GETSETTLEFAILURES    REQ_DT_BASE + 22
#define REQ_DT_GETSWIFTFAILURES     REQ_DT_BASE + 23
#define REQ_DT_SWIFTRESPONSEQUERY   REQ_DT_BASE + 24
#define REQ_DT_UPDAPPLSTAT          REQ_DT_BASE + 26
#define REQ_DT_ZARSPOT              REQ_DT_BASE + 27
#define REQ_DT_ISNNUM               REQ_DT_BASE + 28
#define REQ_DT_GETMT100             REQ_DT_BASE + 29
#define REQ_DT_SWIFTREPORT          REQ_DT_BASE + 30
#define REQ_DT_CHECK_ALIVE          REQ_DT_BASE + 31
#define REQ_DT_TLXPRINT             REQ_DT_BASE + 32
#define REQ_DT_TELEXQUERY           REQ_DT_BASE + 33
#define REQ_DT_GETSIXDIGIT          REQ_DT_BASE + 34
#define REQ_DT_GET_SENDER_ADDR      REQ_DT_BASE + 35
#define REQ_DT_GET_SWIFT_MSG        REQ_DT_BASE + 36
#define REQ_DT_GET_CLIENT_INSTR     REQ_DT_BASE + 37
#define REQ_DT_INSERT_CLIENT_INSTR  REQ_DT_BASE + 38
#define REQ_DT_UPD_CLIENT_INSTR     REQ_DT_BASE + 39
#define REQ_DT_QUERYLOCKEDNEWITTS   REQ_DT_BASE + 40
#define REQ_DT_GET_LOCK_DETAILS     REQ_DT_BASE + 41
#define REQ_DT_QUERYLOCKEDBYCLIENTNAME REQ_DT_BASE + 42
#define REQ_DT_CHANGE_WORKTEAM      REQ_DT_BASE + 43
#define REQ_DT_QUERYLOCKEDFORREROUTES  REQ_DT_BASE + 44
#define REQ_DT_INT_ROUTE            REQ_DT_BASE + 45
#define REQ_DT_ZAPS_CHARGES_MAX     REQ_DT_BASE + 46
#define REQ_DT_LASTQUERY            REQ_DT_BASE + 47  // NB Always last!

// Rates Server Requests            ( Leslie )
#define REQ_RS_FAPSEND              REQ_RS_BASE
#define REQ_RS_GETRATES             REQ_RS_BASE + 1
#define REQ_RS_EURORATES            REQ_RS_BASE + 2
#define REQ_RS_EUROCALC             REQ_RS_BASE + 3
#define REQ_RS_EUROLIST             REQ_RS_BASE + 4
#define REQ_RS_GETCROSSRATE         REQ_RS_BASE + 5
#define REQ_RS_LEGCALC              REQ_RS_BASE + 6
#define REQ_RS_DBL_EUROCALC         REQ_RS_BASE + 7
#define REQ_RS_SPOT_MAX             REQ_RS_BASE + 8


// Generic Swift
#define REQ_NGS_QRY_MSG_TYPE           (REQ_NGS_BASE+ 1)
#define REQ_NGS_QRY_VERSION            (REQ_NGS_BASE+ 2)
#define REQ_NGS_QRY_SCREEN_CONTROL     (REQ_NGS_BASE+ 3)
#define REQ_NGS_QRY_SCREEN_LISTS       (REQ_NGS_BASE+ 4)
#define REQ_NGS_SUBMIT_MSG             (REQ_NGS_BASE+ 5)
#define REQ_NGS_GET_MSG                (REQ_NGS_BASE+ 6)
#define REQ_NGS_LIST_MSG               (REQ_NGS_BASE+ 7)
#define REQ_NGS_GET_MSG_EVENTS         (REQ_NGS_BASE+ 8)
#define REQ_NGS_VALIDATE_MSG           (REQ_NGS_BASE+ 9)
#define REQ_NGS_SWIFT_SEND             (REQ_NGS_BASE+ 10)
#define REQ_NGS_SWIFT_EDIT_RESEND      (REQ_NGS_BASE+ 11)
#define REQ_NGS_SWIFT_RESEND           (REQ_NGS_BASE+ 12)
#define REQ_NGS_SWIFT_CHECK_ALIVE      (REQ_NGS_BASE+ 13)
#define REQ_NGS_SHUTDOWN               (REQ_NGS_BASE+ 14)
#define REQ_NGS_CAPTURE_MSG            (REQ_NGS_BASE+ 15)
#define REQ_NGS_PROCESS                (REQ_NGS_BASE+ 16)
#define REQ_NGS_GET_CCY                (REQ_NGS_BASE+ 17)
#define REQ_NGS_VALIDATE_ADDRESS       (REQ_NGS_BASE+ 18)
#define REQ_NGS_VIEW_OUTGOING_MSG      (REQ_NGS_BASE+ 19)
#define REQ_NGS_VIEW_INCOMING_MSG      (REQ_NGS_BASE+ 20)
#define REQ_NGS_UPD_STATUS             (REQ_NGS_BASE+ 21)
#define REQ_NGS_LOCK                   (REQ_NGS_BASE+ 22)
#define REQ_NGS_UNLOCK                 (REQ_NGS_BASE+ 23)
#define REQ_NGS_QUERY_LOCKED           (REQ_NGS_BASE+ 24)
#define REQ_NGS_VERIFY                 (REQ_NGS_BASE+ 25)
#define REQ_NGS_MODIF_SWADDRESS        (REQ_NGS_BASE+ 26)
#define REQ_NGS_QUERY_SWIFT_INFO       (REQ_NGS_BASE+ 27)
#define REQ_NGS_CHG_INCOM_STATUS       (REQ_NGS_BASE+ 28)
#define REQ_NGS_QUERY_CONTROL_INFO     (REQ_NGS_BASE+ 29)
#define REQ_NGS_QUERY_EOD_INFO         (REQ_NGS_BASE+ 30)
#define REQ_NGS_QUERY_CURR_STATUS      (REQ_NGS_BASE+ 31)
#define REQ_NGS_GET_NO                 (REQ_NGS_BASE+ 32)
#define REQ_NGS_MODIF_VALDATE          (REQ_NGS_BASE+ 33)
#define REQ_NGS_MSG_TYPES_PER_ROLE     (REQ_NGS_BASE+ 34)

// REQUEST CODES FOR WASTE          ( Vernon )
#define REQ_WT_NORM_MSG             (REQ_WT_BASE + 1)
#define REQ_WT_SWIFT_MSG            (REQ_WT_BASE + 2)
#define REQ_WT_BATCH_MSG            (REQ_WT_BASE + 3)
#define REQ_WT_END_BATCH            (REQ_WT_BASE + 4)
#define REQ_WT_TEST_MSG             (REQ_WT_BASE + 5)
#define REQ_WT_ENQ_MSG              (REQ_WT_BASE + 6)
#define REQ_WT_NST_MSG              (REQ_WT_BASE + 7)
#define REQ_WT_STP_MSG              (REQ_WT_BASE + 8)

// REQUEST CODES FOR FORMS A AND E  (Vernon)
#define REQ_AE_FORM_A               (REQ_AE_BASE + 1)
#define REQ_AE_FORM_E               (REQ_AE_BASE + 2)
#define REQ_AE_REV_FORM_AE          (REQ_AE_BASE + 3)
#define REQ_AE_CHECK_ALIVE          (REQ_AE_BASE + 4)


// NTDS TREASURY INTERFACE TO FAP
#define  REQ_TIBS_SEND              (REQ_NTDS_BASE + 1)
#define  REQ_TIBS_OK                (REQ_NTDS_BASE + 2)
#define  REQ_TIBS_ERR               (REQ_NTDS_BASE + 3)

// VB INTERFACE NTDS TREASURY
#define  REQ_VB_SEND                (REQ_VB_BASE + 1)
#define  REQ_VB_OK                  (REQ_VB_BASE + 2)
#define  REQ_VB_ERR                 (REQ_VB_BASE + 3)

// INTERNATIONAL TREASURY COMMMON BACK OFFICE SYSTEM
#define  REQ_ITBOS_GENQ_MSG         (REQ_ITBOS_BASE + 1)
#define  REQ_ITBOS_GENQ_MSG_OK      (REQ_ITBOS_BASE + 2)
#define  REQ_ITBOS_GENQ_MSG_ERR     (REQ_ITBOS_BASE + 3)


// CTS INTERFACE NTDS TREASURY
#define  REQ_CTS_CALC                (REQ_CTS_BASE + 1)
#define  REQ_CTS_OK                  (REQ_CTS_BASE + 2)
#define  REQ_CTS_ERR                 (REQ_CTS_BASE + 3)

// DTSwift Request IDs
#define REQ_DTSWIFT_CHECK_ALIVE    REQ_DTSWIFT_BASE
#define REQ_DTSWIFT_REROUTE        REQ_DTSWIFT_BASE + 2   //
#define REQ_DTSWIFT_ROUTE          REQ_DTSWIFT_BASE + 3   //
#define REQ_DTSWIFT_SEND           REQ_DTSWIFT_BASE + 4   //
#define REQ_DTSWIFT_RESEND         REQ_DTSWIFT_BASE + 6   //
#define REQ_DTSWIFT_SHUTDOWN       REQ_DTSWIFT_BASE + 7   //
#define REQ_DTSWIFT_EDIT_RESEND    REQ_DTSWIFT_BASE + 8   //
#define REQ_DTSWIFT_REJECTS        REQ_DTSWIFT_BASE + 9   // Query Refno's of rejected swift messages
#define REQ_DTSWIFT_QUERY          REQ_DTSWIFT_BASE + 10  // Query info on a swift message
#define REQ_DTSWIFT_NEXT_REFNO     REQ_DTSWIFT_BASE + 11  // Get next RefNo for internal reroute

// REQUEST CODES FOR BULK PAYMENTS SERVER
#define REQ_BP_SHUTDOWN                   REQ_BP_BASE
#define REQ_BP_GET_BATCH_STAT             (REQ_BP_BASE + 1)
#define REQ_BP_GET_OUT_BATCH              (REQ_BP_BASE + 2)
#define REQ_BP_UPD_BATCH_STAT             (REQ_BP_BASE + 3)
#define REQ_BP_GET_SETTLE_FAILS           (REQ_BP_BASE + 4)
#define REQ_BP_GET_SWIFT_FAILS            (REQ_BP_BASE + 5)
#define REQ_BP_SAVE_STRAIGHT_SWIFT        (REQ_BP_BASE + 6)
#define REQ_BP_SAVE_THIRD_CCY_SWIFT       (REQ_BP_BASE + 7)
#define REQ_BP_SAVE_SETTLE_RULE           (REQ_BP_BASE + 8)
#define REQ_BP_UPD_STRAIGHT_SWIFT_STAT    (REQ_BP_BASE + 9)
#define REQ_BP_UPD_THIRD_SWIFT_STAT       (REQ_BP_BASE + 10)
#define REQ_BP_UPD_SWIFT_LOCK             (REQ_BP_BASE + 11)
#define REQ_BP_UPD_FEC_LOCK               (REQ_BP_BASE + 12)
#define REQ_BP_RM_SAVE_VALDATE            (REQ_BP_BASE + 13)
#define REQ_BP_RM_LIST_VALDATE            (REQ_BP_BASE + 14)
#define REQ_BP_RM_SAVE_CORRBANK           (REQ_BP_BASE + 15)
#define REQ_BP_RM_LIST_CORRBANK           (REQ_BP_BASE + 16)
#define REQ_BP_RM_SAVE_BENBANK            (REQ_BP_BASE + 17)
#define REQ_BP_RM_LIST_BENBANK            (REQ_BP_BASE + 18)
#define REQ_BP_LASTQUERY                  (REQ_BP_BASE + 19)
#define REQ_BP_UPD_FEC_STAT               (REQ_BP_BASE + 20)




// REQUEST CODES FOR TIBOS REAL TIME ENGINE
#define  REQ_TRANSACTION             (REQ_TIBOS_REAL_BASE + 1)
#define  REQ_ENQUIRY                 (REQ_TIBOS_REAL_BASE + 2)
#define  REQ_STORE_AND_FORWARD       (REQ_TIBOS_REAL_BASE + 3)
#define  REQ_SEQUENCE_NO             (REQ_TIBOS_REAL_BASE + 4)
#define  REQ_TRAN_STATUS             (REQ_TIBOS_REAL_BASE + 5)

// REQUEST CODES FOR THE GENERIC SERVER (EXPORT LC'S)
#define  REQ_GENCS_LONG_MESSAGE      (REQ_GENCS_BASE + 1)
#define  REQ_GENCS_API_REQUEST       (REQ_GENCS_BASE + 2)
#define  REQ_GENCS_TEST              (REQ_GENCS_BASE + 3)
#define  REQ_GENCS_GETLONGPACKET     (REQ_GENCS_BASE + 4)
#define  REQ_GENCS_DELETEFILE        (REQ_GENCS_BASE + 5)
#define  REQ_GENCS_GETBINFILE        (REQ_GENCS_BASE + 6)
#define  REQ_GENCS_SQLPLUS           (REQ_GENCS_BASE + 7)
#define  REQ_GENCS_INST_QRY          (REQ_GENCS_BASE + 8) 
#define  REQ_GENCS_INST_PROC         (REQ_GENCS_BASE + 9) 

#define  FUN_GENCS_EOP            1
#define  FUN_GENCS_GETLONGPACKET  2
#define  FUN_GENCS_DELETEFILE     3
#define  FUN_GENCS_GETBINFILE     4

// REQUEST CODES FOR STP

#define  REQ_STP_NEWREC           REQ_STP_BASE

#endif

