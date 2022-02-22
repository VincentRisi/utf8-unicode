/***********************************************************************/
/*                                                                     */
/*  System : Treasury                                                  */
/*                                                                     */
/*  File   : Error.h                                                   */
/*                                                                     */
/*  Target :                                                           */
/*                                                                     */
/*  Author : S. Shemesh                                                */
/*                                                                     */
/*  Date   : Mon 25-Jul-1994                                           */
/*                                                                     */
/*  Change History :                                                   */
/*                                                                     */
/*  NB. Any change made here must be conveyed to Drafts and            */
/*      Transfers system.  The corresponding  change must be  made to  */
/*      DFTXFERS.BAS (ErrorMsg).                                       */
/*                                                                     */
/***********************************************************************/
#ifndef _ERROR_INCLUDE
#define _ERROR_INCLUDE "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_ERROR_INCLUDE);
#error PLEASE USE ERRBASE.H
//
//#define ERROR_OK                0
//#define SUCCESS                 ERROR_OK
//
//#define ERROR_INVALID_REQUEST   9999
//
//#define ERROR_DM_BASE           2000  // Diary sub-system: 2000..2099
//#define ERROR_ST_BASE           2100  // settlement base
//#define ERROR_DT_BASE           2200  // Drafts and transfers
//#define ERROR_SF_BASE           2300  // Store And forward
//#define ERROR_WT_BASE           2400  // Waste
//#define ERROR_ACB_BASE          2500  // ACB
//#define ERROR_AE_BASE           2600  // Form A and E For SARB
//#define ERROR_ODBC_BASE         2700  // ODBC/Access
//#define ERROR_SWIFT_BASE        2800  // Swift
//#define ERROR_LC_BASE           3000  // Letters of Credit
//#define ERROR_LU_BASE           3100  // Limits & Utilisations
//#define ERROR_RPC_BASE          3200  // RPC
//#define ERROR_CIF_BASE          3300  // CIF
//#define ERROR_SC_BASE           3500  // Security sub-system: 3500..3599
//#define ERROR_CC_BASE           3600  // Commission & Charges sub-system: 3600..3699
//#define ERROR_RS_BASE           3700  // Rates Server ( Leslie )
//
//#define ERROR_SQLAPI_BASE       8000  // SQLAPI
//#define ERROR_CLISOC_BASE       9000  // Client Sockets For Windows
//#define ERROR_CLISOCK_BASE     10000  // Windows Sockets defined in winsock.h
//#define ERROR_CLISOCK_BASE1    11000  // Windows Sockets defined in winsock.h
//
//// Diary Module errors
//#define ERROR_DM_INVALID_VERSION                       ERROR_DM_BASE
//#define ERROR_DM_DB_FAILURE                            ERROR_DM_BASE + 1
//#define ERROR_DM_INVALID_REMINDERDATE                  ERROR_DM_BASE + 2
//#define ERROR_DM_INVALID_EVENTTIME                     ERROR_DM_BASE + 3
//#define ERROR_DM_NOREPEAT                              ERROR_DM_BASE + 4
///* ie. Calculated EventDate > LastDate */
//#define ERROR_DM_REF_NOT_FOUND                         ERROR_DM_BASE + 5
//#define ERROR_DM_INVALID_ORIGINATING_SYSTEM            ERROR_DM_BASE + 6
//#define ERROR_DM_INVALID_REFNO                         ERROR_DM_BASE + 7
//#define ERROR_DM_INVALID_DIARYACTION                   ERROR_DM_BASE + 8
//#define ERROR_DM_NOMEMORY                              ERROR_DM_BASE + 9
//#define ERROR_DM_INVALID_QUERY                         ERROR_DM_BASE + 10
//#define ERROR_DM_ANY_WITH_INVALID_INDEX                ERROR_DM_BASE + 11
///* ie. if DayOfWeek == CN_DM_ANY then IndexIntoMonth = CN_DM_LAST or 1..31 */
//#define ERROR_DM_DAY_WITH_INVALID_INDEX                ERROR_DM_BASE + 12
///* ie. if DayOfWeek == a specific day then IndexIntoMonth = CN_DM_LAST or 1..5 */
//#define ERROR_DM_INVALID_FIRSTDATE                     ERROR_DM_BASE + 13
//#define ERROR_DM_INVALID_LASTDATE                      ERROR_DM_BASE + 14
//#define ERROR_DM_INVALID_BASEDATE                      ERROR_DM_BASE + 15
//#define ERROR_DM_INVALID_DATETIME                      ERROR_DM_BASE + 16
//#define ERROR_DM_INVALID_EVENT                         ERROR_DM_BASE + 17
//#define ERROR_DM_INI_FILE_ERROR                        ERROR_DM_BASE + 18
//#define ERROR_DM_INVALID_DATE                          ERROR_DM_BASE + 19
//#define ERROR_DM_SEND_ERROR                            ERROR_DM_BASE + 20
//#define ERROR_DM_GEN_REFNO_FAILURE                     ERROR_DM_BASE + 21
//
//// Error codes returned by Settlement module API's
//#define ERROR_ST_CORRUPT_DATA            (ERROR_ST_BASE+1)     // The block data is not valid
//#define ERROR_ST_REFERENCE               (ERROR_ST_BASE+2)     // Bad reference number
//#define ERROR_ST_NO_MEMORY               (ERROR_ST_BASE+3)     // Out of memory
//#define ERROR_ST_DB_ERROR                (ERROR_ST_BASE+4)     // Error accessing database
//#define ERROR_ST_DUPLICATE_REFERENCE     (ERROR_ST_BASE+5)     // The reference number has
//																					// already been used
//#define ERROR_ST_COMMS_FAILURE           (ERROR_ST_BASE+6)     // Unrecoverable network error
//#define ERROR_ST_BAD_REQUEST             (ERROR_ST_BASE+7)     // Unknown RPC request ID
//#define ERROR_ST_INCOMPLETE              (ERROR_ST_BASE+8)     // The transaction is incomplete
//#define ERROR_ST_RAND_EQUIV              (ERROR_ST_BASE+9)     // Rand equivalent error
//#define ERROR_ST_FAP_REJECT              (ERROR_ST_BASE+10)    // FAP rejected the message sent
//#define ERROR_ST_PROCESS                 (ERROR_ST_BASE+11)    // Problem during processing
//#define ERROR_ST_FAP_OFFLINE             (ERROR_ST_BASE+12)    // Couldn't sign on to FAP
//#define ERROR_ST_INVALID_TABLE_ID        (ERROR_ST_BASE+13)    // Invalid table ID supplied
//#define ERROR_ST_INVALID_WASTE_MSG       (ERROR_ST_BASE+14)    // Unknown waste message type
//
//// Error codes returned by Limits and Utilisations module API's
//#define ERROR_LU_REFNO                    ERROR_LU_BASE
//#define ERROR_LU_PRODUCT                 (ERROR_LU_BASE+2)
//#define ERROR_LU_EVENT                   (ERROR_LU_BASE+3)
//#define ERROR_LU_CURRENCY                (ERROR_LU_BASE+4)
//#define ERROR_LU_CLIENT                  (ERROR_LU_BASE+5)
//#define ERROR_LU_COMPANY                 (ERROR_LU_BASE+6)
//#define ERROR_LU_ALREADY_PROCESSED       (ERROR_LU_BASE+7)
//#define ERROR_LU_COMMS_FAILURE           (ERROR_LU_BASE+8)
//#define ERROR_LU_DB_ERROR                (ERROR_LU_BASE+9)
//#define ERROR_LU_NO_MEMORY               (ERROR_LU_BASE+10)
//
//
//
//// Error codes returned by RPC (Remote Procedure Call).
//#define ERROR_RPC_MOREDATA               (ERROR_RPC_BASE + 0)
//#define ERROR_RPC_NO_DATA_RETURNED       (ERROR_RPC_BASE + 1)
//#define ERROR_RPC_INVALID_HOST_HEADER    (ERROR_RPC_BASE + 2)
//#define ERROR_RPC_INVALID_CLIENT_HEADER  (ERROR_RPC_BASE + 3)
//#define ERROR_RPC_INVALID_HOST_NAME      (ERROR_RPC_BASE + 4)
//#define ERROR_RPC_INVALID_SOCKET_NAME    (ERROR_RPC_BASE + 5)
//#define ERROR_RPC_CLISERV                (ERROR_RPC_BASE + 6)
//#define ERROR_RPC_NO_MEMORY              (ERROR_RPC_BASE + 7)
//#define ERROR_RPC_TOO_LONG               (ERROR_RPC_BASE + 8)
//#define ERROR_RPC_INETADDR               (ERROR_RPC_BASE + 9)
//#define ERROR_RPC_NOTREADY               (ERROR_RPC_BASE + 10)
//#define ERROR_RPC_NODEKEYNOTSET          (ERROR_RPC_BASE + 11)
//#define ERROR_RPC_USERNOTSET             (ERROR_RPC_BASE + 12)
//#define ERROR_RPC_HOSTNAME               (ERROR_RPC_BASE + 13)
//#define ERROR_RPC_ENVIRONMENT_NOTSET     (ERROR_RPC_BASE + 14)
//
//// Error codes returned by CIF (Customer Information File).
//
//#define ERROR_CIF_OK                                  0
//#define ERROR_CIF_INVALID_REQUEST                     (ERROR_CIF_BASE + 1)
//#define ERROR_CIF_SQLAPI_EXCEPTION                    (ERROR_CIF_BASE + 2)
//#define ERROR_CIF_NO_SWIFT_ADDR                       (ERROR_CIF_BASE + 3)
//#define ERROR_CIF_NAME_NOT_FOUND                      (ERROR_CIF_BASE + 4)
//#define ERROR_CIF_INVALID_ACC_NO                      (ERROR_CIF_BASE + 5)
//#define ERROR_CIF_INVALID_CIF_NO                      (ERROR_CIF_BASE + 6)
//#define ERROR_CIF_INVALID_SWIFT_ADDR                  (ERROR_CIF_BASE + 7)
//#define ERROR_CIF_OUTPUT_BUFFER_TOO_SMALL             (ERROR_CIF_BASE + 8)
//
//// Error codes returned by Form A/E DLL.
//
//#define ERROR_AE_INI_FILE                  (ERROR_AE_BASE + 1)
//#define ERROR_AE_ACCESS_DATABASE           (ERROR_AE_BASE + 2)
//#define ERROR_AE_NOTEXIST                  (ERROR_AE_BASE + 3)
//
//// Swift servers errors
//#define ERROR_SWIFT_ROUTE                  (ERROR_SWIFT_BASE + 1)
//#define ERROR_SWIFT_REROUTE                (ERROR_SWIFT_BASE + 2)
//#define ERROR_SWIFT_MANDATORY_MISSING      (ERROR_SWIFT_BASE + 3)
//#define ERROR_SWIFT_INVALID_REQUEST        (ERROR_SWIFT_BASE + 4)
//#define ERROR_SWIFT_ORACLE                 (ERROR_SWIFT_BASE + 5)
//#define ERROR_SWIFT_NO_DATA_FOUND          (ERROR_SWIFT_BASE + 6)
//#define ERROR_SWIFT_DUPLICATE              (ERROR_SWIFT_BASE + 7)
//#define ERROR_SWIFT_NO_END_OF_MSG          (ERROR_SWIFT_BASE + 8)
//#define ERROR_SWIFT_TAG_NOT_FOUND          (ERROR_SWIFT_BASE + 9)
//#define ERROR_SWIFT_SWRD_ROUTINGBRANCH     (ERROR_SWIFT_BASE + 10)
//#define ERROR_SWIFT_SWRD_MSGNO             (ERROR_SWIFT_BASE + 11)
//#define ERROR_SWIFT_SWRD_OSNNO             (ERROR_SWIFT_BASE + 12)
//#define ERROR_SWIFT_SWRD_MSGTYPE           (ERROR_SWIFT_BASE + 13)
//#define ERROR_SWIFT_SWRD_CURRENCY          (ERROR_SWIFT_BASE + 14)
//#define ERROR_SWIFT_SWRD_AMOUNT            (ERROR_SWIFT_BASE + 15)
//#define ERROR_SWIFT_SWRD_TOBRANCH          (ERROR_SWIFT_BASE + 16)
//#define ERROR_SWIFT_SWRD_FORMAT            (ERROR_SWIFT_BASE + 17)
//#define ERROR_SWIFT_DUPLICATE_MSG          (ERROR_SWIFT_BASE + 18)
//#define ERROR_SWIFT_ST400_RESPONSE         (ERROR_SWIFT_BASE + 19)
//#define ERROR_SWIFT_ROUTEBACK              (ERROR_SWIFT_BASE + 21)
//
//// Commission & Charges error codes
//#define ERROR_CC_INVALID_VERSION                       ERROR_CC_BASE
//#define ERROR_CC_DB_FAILURE                            ERROR_CC_BASE + 1
//#define ERROR_CC_INVALID_ORIGINATING_SYSTEM            ERROR_CC_BASE + 2
//#define ERROR_CC_NOMEMORY                              ERROR_CC_BASE + 3
//#define ERROR_CC_INVALID_PROD_CODE                     ERROR_CC_BASE + 4
//#define ERROR_CC_INVALID_EVENT_CODE                    ERROR_CC_BASE + 5
//#define ERROR_CC_INVALID_CLIENT_NO                     ERROR_CC_BASE + 6
//#define ERROR_CC_INVALID_AMT                           ERROR_CC_BASE + 7
//#define ERROR_CC_INVALID_CURRENCY                      ERROR_CC_BASE + 8
//#define ERROR_CC_INVALID_BRANCH                        ERROR_CC_BASE + 9
//#define ERROR_CC_NOT_LOGGED_ON                         ERROR_CC_BASE + 10
//#define ERROR_CC_NO_DATA                               ERROR_CC_BASE + 11
//#define ERROR_CC_INIFILE                               ERROR_CC_BASE + 12
//
//// Error codes returned by Letters of Credit
//
//#define ERROR_LC_INVALID_REQUEST                       ERROR_LC_BASE
//#define ERROR_LC_ORACLE                                ERROR_LC_BASE + 1
//#define ERROR_LC_ST400_REJECT                          ERROR_LC_BASE + 2
//#define ERROR_LC_NO_DATA_FOUND                         ERROR_LC_BASE + 3
//#define ERROR_LC_INVALID_SEQNO                         ERROR_LC_BASE + 4
//
//// Error codes returned by Drafts and Transfers
//
//#define ERROR_DT_CORRUPT_DATA           ERROR_DT_BASE
//#define ERROR_DT_REFERENCE              ERROR_DT_BASE  + 1
//#define ERROR_DT_INVALID_REQUEST        ERROR_DT_BASE  + 2
//#define ERROR_DT_INI_FILE               ERROR_DT_BASE  + 3
//#define ERROR_DT_MEMORY                 ERROR_DT_BASE  + 4
//#define ERROR_DT_INVALID                ERROR_DT_BASE  + 5
//#define ERROR_DT_LOCKED                 ERROR_DT_BASE  + 6
//#define ERROR_DT_ORACLE                 ERROR_DT_BASE  + 7
//#define ERROR_DT_DB_ERROR               ERROR_DT_ORACLE
//#define ERROR_DT_INVALID_ACTION         ERROR_DT_BASE  + 8
//#define ERROR_DT_SF_INIT                ERROR_DT_BASE  + 9
//#define ERROR_DT_CTRL_REFERENCE         ERROR_DT_BASE  + 10
//#define ERROR_DT_NO_DATA_FOUND          ERROR_DT_BASE  + 11
//#define ERROR_DT_SF                     ERROR_DT_BASE  + 12
//#define ERROR_DT_NOT_READY_FOR_PROCESS  ERROR_DT_BASE  + 13
//#define ERROR_DT_INVALID_MSG_TYPE       ERROR_DT_BASE  + 14
//#define ERROR_DT_ST400_REJECT           ERROR_DT_BASE  + 15
//#define ERROR_DT_INVALID_SEQNO          ERROR_DT_BASE  + 16
//#define ERROR_DT_RAND_EQUIV             ERROR_DT_BASE  + 17
//#define ERROR_DT_NOT_LOCKED             ERROR_DT_BASE  + 18
//#define ERROR_DT_PAYMENT_EXISTS         ERROR_DT_BASE  + 19
//#define ERROR_DT_NO_PAYMENT             ERROR_DT_BASE  + 20
//#define ERROR_DT_NO_BENEFICIARY         ERROR_DT_BASE  + 21
//#define ERROR_DT_INVALID_STATUS         ERROR_DT_BASE  + 22
//#define ERROR_DT_INVALID_CCY            ERROR_DT_BASE  + 23
//#define ERROR_DT_BUSY_PROCESSING        ERROR_DT_BASE  + 24
//#define ERROR_DT_INVALID_RECIPIENT      ERROR_DT_BASE  + 25
//#define ERROR_DT_INVALID_PARAMETER      ERROR_DT_BASE  + 26
//#define ERROR_DT_INVALID_HANDLE         ERROR_DT_BASE  + 27
//#define ERROR_DT_SOCKET                 Erc
//
//// Security Error codes
//enum {
// ERROR_SC_USER_EXISTS = ERROR_SC_BASE,                      // 0
// ERROR_SC_ALREADY_RUNNING,                                  // 1
// ERROR_SC_APP_NOT_ALLOWED,                                  // 2
// ERROR_SC_ARRAY_OF_TABS_ILLEGAL,                            // 3
// ERROR_SC_CANNOT_CREATE_NEW_FILE,                           // 4
// ERROR_SC_CANNOT_DELETE_FILE,                               // 5
// ERROR_SC_CANNOT_GET_PROPERTY,                              // 6
// ERROR_SC_CANNOT_LOCK_MEMORY,                               // 7
// ERROR_SC_CANNOT_RENAME_FILE,                               // 8
// ERROR_SC_CANNOT_SEEK_IN_FILE,                              // 9
// ERROR_SC_CANNOT_WRITE_TO_FILE,                             // 10
// ERROR_SC_CREATE_DIALOG_FAILED,                             // 11
// ERROR_SC_DIALOG_NOT_ALLOWED,                               // 12
// ERROR_SC_DISABLE_CONTROL,                                  // 13
// ERROR_SC_FLAG_ALREADY_SET,                                 // 14
// ERROR_SC_HIDE_CONTROL,                                     // 15
// ERROR_SC_INITIAL_LOGON_OFFLINE,                            // 16
// ERROR_SC_INVALID,                                          // 17
// ERROR_SC_INVALID_DATAFILE,                                 // 18
// ERROR_SC_INVALID_PASSWORD,                                 // 19
// ERROR_SC_INVALID_PROFILE,                                  // 20
// ERROR_SC_LOCAL_LOGON_EXPIRED,                              // 21
// ERROR_SC_LOCAL_LOGON_NOT_FOUND,                            // 22
// ERROR_SC_LOGGED_ON_LOCAL,                                  // 23
// ERROR_SC_NOT_LOGGED_ON,                                    // 24
// ERROR_SC_NO_MEMORY,                                        // 25
// ERROR_SC_PASSWD_AND_USERID_MUST_DIFFER,                    // 26
// ERROR_SC_PASSWD_ENTRIES_DIFFER,                            // 27
// ERROR_SC_PASSWD_EXPIRED,                                   // 28
// ERROR_SC_PASSWD_RECYCLED,                                  // 29
// ERROR_SC_PASSWD_REPETITVE,                                 // 30
// ERROR_SC_PASSWD_SAME,                                      // 31
// ERROR_SC_PASSWD_SOON_TOO_EXPIRE,                           // 32
// ERROR_SC_PASSWD_TOO_SHORT,                                 // 33
// ERROR_SC_PROFILES_DIFFER,                                  // 34
// ERROR_SC_PROGRAM_NOT_FOUND,                                // 35
// ERROR_SC_REGISTER_CLASS_FAILED,                            // 36
// ERROR_SC_REQUEST_DENIED,                                   // 37
// ERROR_SC_WORKSTATION_EXISTS,                               // 38
// ERROR_SC_WORKSTATION_NOT_REGISTERED,                       // 39
// ERROR_SC_WSLOGON_NOT_RUNNING,                              // 40
// ERROR_SC_PASSWD_CHANGE_TOO_EARLY,                          // 41
// ERROR_SC_NO_HOST_IN_INIFILE,                               // 42
// ERROR_SC_NO_SERVICE_IN_INIFILE,                            // 43
// ERROR_SC_REGISTER_WORKSTATION_RQD,                         // 44
// ERROR_SC_VLOGIN_RQD,                                       // 45
// ERROR_SC_CHANGE_PASSWORD_OFFLINE,                          // 46
// ERROR_SC_TOO_MANY_LIMITS,                                  // 47
// ERROR_SC_INVALID_LIMITID,                                  // 48
// ERROR_SC_CTLTYPE_NOT_FOUND,                                // 49
// ERROR_SC_UNKNOWN_PROFILE_VERSION,                          // 50
// ERROR_SC_KEYFILE,                                          // 51
// ERROR_SC_LIMIT_NOT_FOUND,                                  // 52
// ERROR_SC_NOT_TABCTL,                                       // 53
// ERROR_SC_PASSWD_REPETITIVE,                                // 54
// ERROR_SC_NO_APPS,                                          // 55
// ERROR_SC_LOGGED_ON_ELSEWHERE,                              // 56
// ERROR_SC_INVALID_RPC_VERSION,                              // 57
// ERROR_SC_INVALID_WORKSTATION,                              // 58
// ERROR_SC_INVALID_USER,                                     // 59
// ERROR_SC_INVALID_LOGON                                     // 60
//};
//
//// Rates Server Errors ( Leslie )
//#define ERROR_RS_COMMS_FAILURE    ERROR_RS_BASE
//#define ERROR_RS_DB_ERROR         ERROR_RS_BASE + 1
//#define ERROR_RS_INVALID_REQUEST  ERROR_RS_BASE + 2
//#define ERROR_RS_FAP_FILE         ERROR_RS_BASE + 3
//#define ERROR_RS_RATECODE         ERROR_RS_BASE + 4
//#define ERROR_RS_MEMORY           ERROR_RS_BASE + 5
//#define ERROR_RS_CCYCODE          ERROR_RS_BASE + 6
//
//// ERROR CODES FOR WASTE                          (Vernon)
//#define VALID_WT_SUCCESSFUL                 (ERROR_OK)
//#define  ERROR_WT_INVALID_RPC_HEADER         (ERROR_WT_BASE + 1)
//#define  ERROR_WT_ACCOUNT_NOT_NUMERIC        (ERROR_WT_BASE + 2)
//#define  ERROR_WT_AMOUNT_NOT_NUMERIC         (ERROR_WT_BASE + 3)
//#define  ERROR_WT_CODE_NOT_NUMERIC           (ERROR_WT_BASE + 4)
//#define  ERROR_WT_INVALID_DATE               (ERROR_WT_BASE + 5)
//#define  ERROR_WT_INVALID_TIME               (ERROR_WT_BASE + 6)
//#define  ERROR_WT_BRANCH                     (ERROR_WT_BASE + 7)
//#define  ERROR_WT_TERMINAL                   (ERROR_WT_BASE + 8)
//#define ERROR_WT_INVALID_MSG_TYPE           (ERROR_WT_BASE + 9)
//#define ERROR_WT_NO_SWIFT_CONTRA            (ERROR_WT_BASE + 10)
//#define ERROR_WT_READING_INI_FILE           (ERROR_WT_BASE + 11)
//#define ERROR_WT_CREATING_SOCKET            (ERROR_WT_BASE + 12)
//#define ERROR_WT_SERVER_ERROR               (ERROR_WT_BASE + 13)
//#define ERROR_WT_UNSPECIFIED_EXCEPTION      (ERROR_WT_BASE + 14)
//#define ERROR_WT_INVALID_SOCKET_IO          (ERROR_WT_BASE + 15)
//#define ERROR_WT_INVALID_LENGTH             (ERROR_WT_BASE + 16)
//#define ERROR_WT_SOCKET_TIMEOUT             (ERROR_WT_BASE + 17)
//#define ERROR_WT_TRANS_CODE_INVALID     (ERROR_WT_BASE + 18)
//#define ERROR_WT_SQLAPI_CIF_ERROR           (ERROR_WT_BASE + 19)
//#define  ERROR_WT_DAY_OUT_OF_RANGE           (ERROR_WT_BASE + 20)
//#define ERROR_WT_MONTH_OUT_OF_RANGE         (ERROR_WT_BASE + 21)
//#define ERROR_WT_CENTUARY_OUT_OF_RANGE      (ERROR_WT_BASE + 22)
//#define ERROR_WT_DATE_ERROR                   (ERROR_WT_BASE + 23)
//#define ERROR_WT_SQLAPI_DATE_ERROR          (ERROR_WT_BASE + 24)
//#define ERROR_WT_HOUR_OUT_OF_RANGE          (ERROR_WT_BASE + 25)
//#define ERROR_WT_MINUTE_OUT_OF_RANGE        (ERROR_WT_BASE + 26)
//#define ERROR_WT_SECOND_OUT_OF_RANGE        (ERROR_WT_BASE + 27)
//#define ERROR_WT_ONLINE_DOWN                (ERROR_WT_BASE + 28)
//#define ERROR_WT_INVALID_CRCR_CODE          (ERROR_WT_BASE + 29)
//#define ERROR_WT_ONLINE_FILE_UNAVAILABLE    (ERROR_WT_BASE + 30)
//#define ERROR_WT_ACCOUNT_NOT_ON_FILE        (ERROR_WT_BASE + 31)
//#define ERROR_WT_UNIDENTIFIED_DCRWAC_STATUS (ERROR_WT_BASE + 32)
//#define ERROR_WT_UNIDENTIFIED_ERROR         (ERROR_WT_BASE + 33)
//#define ERROR_WT_BRANCH_NOT_NUMERIC         (ERROR_WT_BASE + 34)
//#define ERROR_WT_SEQ_NO_NOT_NUMERIC         (ERROR_WT_BASE + 35)
//#define ERROR_WT_YEAR_OUT_OF_RANGE          (ERROR_WT_BASE + 36)
//#define ERROR_WT_TIME_NOT_NUMERIC           (ERROR_WT_BASE + 37)
//#define  ERROR_WT_INVALID_REQUEST            (ERROR_INVALID_REQUEST)
//
//// ERROR CODES FOR FORMS A AND E            (Vernon)
//
//#define VALID_AE_SUCCESSFUL                 (ERROR_OK)
//#define  ERROR_AE_INVALID_RPC_HEADER         (ERROR_AE_BASE + 1)
//#define ERROR_AE_INVALID_MSG_TYPE           (ERROR_AE_BASE + 2)
//#define ERROR_AE_UNSPECIFIED_EXCEPTION      (ERROR_AE_BASE + 3)
//#define ERROR_AE_INVALID_LENGTH             (ERROR_AE_BASE + 4)
//#define ERROR_AE_INVALID_FORM               (ERROR_AE_BASE + 5)
//#define  ERROR_AE_INVALID_SUB_CATEGORY       (ERROR_AE_BASE + 6)
//#define  ERROR_AE_INVALID_DEPART_DATE        (ERROR_AE_BASE + 7)
//#define  ERROR_AE_INVALID_FROM_DATE          (ERROR_AE_BASE + 8)
//#define  ERROR_AE_INVALID_TO_DATE            (ERROR_AE_BASE + 9)
//#define  ERROR_AE_INVALID_BENEFIC_STATUS     (ERROR_AE_BASE + 10)
//#define  ERROR_AE_INVALID_ACCOUNT_IND        (ERROR_AE_BASE + 11)
//#define  ERROR_AE_BRANCH_NOT_NUMERIC         (ERROR_AE_BASE + 12)
//#define  ERROR_AE_INVALID_BIRTH_DATE         (ERROR_AE_BASE + 13)
//#define  ERROR_AE_INVALID_CURRENCY           (ERROR_AE_BASE + 14)
//#define  ERROR_AE_FOREIGN_AMOUNT_NOT_NUMERIC (ERROR_AE_BASE + 15)
//#define  ERROR_AE_RAND_AMOUNT_NOT_NUMERIC    (ERROR_AE_BASE + 16)
//#define  ERROR_AE_CATEGORY_INVALID           (ERROR_AE_BASE + 17)
//#define  ERROR_AE_APPL_REF_NO_INVALID        (ERROR_AE_BASE + 18)
//#define  ERROR_AE_INVALID_REPLY_DATE         (ERROR_AE_BASE + 19)
//#define  ERROR_AE_INVALID_GEOG_AREA          (ERROR_AE_BASE + 20)
//#define  ERROR_AE_INVALID_FORM_DATE          (ERROR_AE_BASE + 21)
//#define  ERROR_AE_INVALID_VALUE_DATE         (ERROR_AE_BASE + 22)
//#define  ERROR_AE_NO_DEALER                  (ERROR_AE_BASE + 23)
//#define  ERROR_AE_INVALID_DEALER             (ERROR_AE_BASE + 24)
//#define  ERROR_AE_INVALID_MICR_NO            (ERROR_AE_BASE + 25)
//#define  ERROR_AE_SQLAPI_CIF_ERROR           (ERROR_AE_BASE + 26)
//#define ERROR_AE_EXCON_AUTH_REQ             (ERROR_AE_BASE + 27)
//#define ERROR_AE_RULE_BASE_INCORRECT        (ERROR_AE_BASE + 28)
//#define ERROR_AE_INVALID_FOREX_IND          (ERROR_AE_BASE + 29)
//#define ERROR_AE_INVALID_TO_OR_FROM_DATE    (ERROR_AE_BASE + 30)
//#define ERROR_AE_INVALID_CR_NON_RES_ACC     (ERROR_AE_BASE + 31)
//#define ERROR_AE_REF_NO_DOES_NOT_EXIST      (ERROR_AE_BASE + 32)
//#define ERROR_AE_TRAN_ALREADY_CANCELLED     (ERROR_AE_BASE + 33)
//#define ERROR_AE_LINE_DOWN                  (ERROR_AE_BASE + 34)
//#define ERROR_AE_ONLINE_FIELD_NOT_NUMERIC   (ERROR_AE_BASE + 35)
//#define ERROR_AE_ONLINE_FIELD_SPACES        (ERROR_AE_BASE + 36)
//#define ERROR_AE_UNIDENTIFIED_ERROR         (ERROR_AE_BASE + 37)
//#define ERROR_AE_SQLAPI_CCY_ERROR           (ERROR_AE_BASE + 38)
//#define ERROR_AE_EMPTY_SURNAME              (ERROR_AE_BASE + 39)
//#define ERROR_AE_FOREIGN_AMOUNT_INVALID     (ERROR_AE_BASE + 40)
//#define ERROR_AE_NO_DETAILS                 (ERROR_AE_BASE + 41)
//#define ERROR_AE_EMPTY_BEN_SURNAME          (ERROR_AE_BASE + 42)
//#define ERROR_AE_AUTH_DEALER_NOT_NUMERIC    (ERROR_AE_BASE + 43)
//#define ERROR_AE_INVALID_SUB_CATCAT1        (ERROR_AE_BASE + 44)
//#define ERROR_AE_INVALID_SOFT_VERSION       (ERROR_AE_BASE + 45)
//#define ERROR_AE_INVALID_COUNTRY_RESIDENCE  (ERROR_AE_BASE + 46)
//#define ERROR_AE_EMPTY_ENTITY               (ERROR_AE_BASE + 47)
//#define ERROR_AE_BRANCH_DOES_NOT_EXIST      (ERROR_AE_BASE + 48)
//#define ERROR_AE_BRANCHNO_ZEROES            (ERROR_AE_BASE + 49)
//#define ERROR_AE_NO_REFNO                   (ERROR_AE_BASE + 50)
//#define  ERROR_AE_INVALID_REQUEST            (ERROR_INVALID_REQUEST)
//
#endif

