#include "errbase.h"
#include "machine.h"
#include "xstring.h"

#if defined(M_W32) || defined(M_NT)
#include "windows.h"
#include <winsock.h>
#include "clisock.h"
#elif defined(_WINDOWS) || defined(_WIN32)
#include <winsock.h>
#include "clisock.h"
#endif

typedef struct tagErrMsg
{
   int ErrorNo;
   char Text[81];
} ERRMSG;

static ERRMSG _ErrMsg[] =
{
     { ERROR_OK                               , "Operation successful" }

// Security errors
   , { ERROR_SC_USER_EXISTS                   , "SECURITY: User already exists" }
   , { ERROR_SC_ALREADY_RUNNING               , "SECURITY: WSLogon is already running" }
   , { ERROR_SC_APP_NOT_ALLOWED               , "SECURITY: You are not authorised to run this application" }
   , { ERROR_SC_ARRAY_OF_TABS_ILLEGAL         , "SECURITY: An array of Tab controls is not supported" }
   , { ERROR_SC_CANNOT_CREATE_NEW_FILE        , "SECURITY: Cannot create file" }
   , { ERROR_SC_CANNOT_DELETE_FILE            , "SECURITY: Cannot delete file" }
   , { ERROR_SC_CANNOT_GET_PROPERTY           , "SECURITY: Cannot get property" }
   , { ERROR_SC_CANNOT_LOCK_MEMORY            , "SECURITY: Cannot lock memory" }
   , { ERROR_SC_CANNOT_RENAME_FILE            , "SECURITY: Cannot rename file" }
   , { ERROR_SC_CANNOT_SEEK_IN_FILE           , "SECURITY: Cannot seek within file" }
   , { ERROR_SC_CANNOT_WRITE_TO_FILE          , "SECURITY: Cannot write to file" }
   , { ERROR_SC_CREATE_DIALOG_FAILED          , "SECURITY: Could not create dialog" }
   , { ERROR_SC_DIALOG_NOT_ALLOWED            , "SECURITY: You are not authorised to run this dialog/form" }
   , { ERROR_SC_DISABLE_CONTROL               , "SECURITY: Cannot disable control" }
   , { ERROR_SC_FLAG_ALREADY_SET              , "SECURITY: Flag already set" }
   , { ERROR_SC_HIDE_CONTROL                  , "SECURITY: Cannot hide control" }
   , { ERROR_SC_INITIAL_LOGON_OFFLINE         , "SECURITY: You are not currently online - Please try again later" }
   , { ERROR_SC_INVALID                       , "SECURITY: Security violation" }
   , { ERROR_SC_INVALID_DATAFILE              , "SECURITY: Invalid security file" }
   , { ERROR_SC_INVALID_PASSWORD              , "SECURITY: Invalid password" }
   , { ERROR_SC_INVALID_PROFILE               , "SECURITY: Security profile invalid" }
   , { ERROR_SC_LOCAL_LOGON_EXPIRED           , "SECURITY: Your local logon has expired, and you are currently offline" }
   , { ERROR_SC_LOCAL_LOGON_NOT_FOUND         , "SECURITY: Your logon needs to be validated by the host" }
   , { ERROR_SC_LOGGED_ON_LOCAL               , "SECURITY: Local logon successful" }
   , { ERROR_SC_NOT_LOGGED_ON                 , "SECURITY: Your logon was not successful" }
   , { ERROR_SC_NO_MEMORY                     , "SECURITY: Insufficient memory" }
   , { ERROR_SC_PASSWD_AND_USERID_MUST_DIFFER , "SECURITY: Your Password and UserID may not be the same" }
   , { ERROR_SC_PASSWD_ENTRIES_DIFFER         , "SECURITY: Your first and second entry of your password were different" }
   , { ERROR_SC_PASSWD_EXPIRED                , "SECURITY: Your password has expired" }
   , { ERROR_SC_PASSWD_RECYCLED               , "SECURITY: This password has been used before. Please select another one." }
   , { ERROR_SC_PASSWD_SAME                   , "SECURITY: Your password cannot be the same as your old password" }
   , { ERROR_SC_PASSWD_SOON_TOO_EXPIRE        , "SECURITY: Your password is going to expire please change it now." }
   , { ERROR_SC_PASSWD_TOO_SHORT              , "SECURITY: Your password is too short. Please select another one." }
   , { ERROR_SC_PROFILES_DIFFER               , "SECURITY: Your profile has changed" }
   , { ERROR_SC_PROGRAM_NOT_FOUND             , "SECURITY: Program not found in your profile" }
   , { ERROR_SC_REGISTER_CLASS_FAILED         , "SECURITY: Unable to register window class" }
   , { ERROR_SC_REQUEST_DENIED                , "SECURITY: Request denied" }
   , { ERROR_SC_WORKSTATION_EXISTS            , "SECURITY: Workstation already exists" }
   , { ERROR_SC_WORKSTATION_NOT_REGISTERED    , "SECURITY: Workstation not registered on host" }
   , { ERROR_SC_WSLOGON_NOT_RUNNING           , "SECURITY: WSLogon.exe is not running" }
   , { ERROR_SC_PASSWD_CHANGE_TOO_EARLY       , "SECURITY: It is too soon to change your password" }
   , { ERROR_SC_NO_HOST_IN_INIFILE            , "SECURITY: WSLogon.ini requires a 'Host=' entry in the [SecAdm] section" }
   , { ERROR_SC_NO_SERVICE_IN_INIFILE         , "SECURITY: WSLogon.ini requires a 'Service=' entry in the [SecAdm] section" }
   , { ERROR_SC_REGISTER_WORKSTATION_RQD      , "SECURITY: Register workstation required" }
   , { ERROR_SC_VLOGIN_RQD                    , "SECURITY: Initial login required" }
   , { ERROR_SC_CHANGE_PASSWORD_OFFLINE       , "SECURITY: You are not currently online - Please try again later" }
   , { ERROR_SC_TOO_MANY_LIMITS               , "SECURITY: Too many limits defined in profile" }
   , { ERROR_SC_INVALID_LIMITID               , "SECURITY: Limit ID invalid" }
   , { ERROR_SC_CTLTYPE_NOT_FOUND             , "SECURITY: Control type not found in control type table" }
   , { ERROR_SC_UNKNOWN_PROFILE_VERSION       , "SECURITY: Profile rcvd from host requires a newer version of WSLogon" }
   , { ERROR_SC_KEYFILE                       , "SECURITY: Error reading from/writing to the key file" }
   , { ERROR_SC_LIMIT_NOT_FOUND               , "SECURITY: LimitID not found" }
   , { ERROR_SC_NOT_TABCTL                    , "SECURITY: Control is not of type TabCtl" }
   , { ERROR_SC_PASSWD_REPETITIVE             , "SECURITY: Password contains repeating elements - Please try another" }
   , { ERROR_SC_NO_APPS                       , "SECURITY: You have no applications to run" }
   , { ERROR_SC_LOGGED_ON_ELSEWHERE           , "SECURITY: A more recent Logon on another Workstation" }
   , { ERROR_SC_INVALID_RPC_VERSION           , "SECURITY: Invalid RPC Version" }
   , { ERROR_SC_INVALID_WORKSTATION           , "SECURITY: Invalid Workstation" }
   , { ERROR_SC_INVALID_USER                  , "SECURITY: Invalid User" }
   , { ERROR_SC_INVALID_LOGON                 , "SECURITY: Invalid Logon" }
   , { ERROR_SC_TOO_MANY_ATTEMPTS             , "SECURITY: Too many attempts" }
   , { ERROR_SC_MASQUERADES_AS_NEW_USER       , "SECURITY: Existing user is acting like a new user" }
   , { ERROR_SC_USE_SMART                     , "SECURITY: User must use a smartcard" }
   , { ERROR_SC_SMART_USE                     , "SECURITY: User must use a smartcard" }

// RPC errors
   , { ERROR_RPC_MOREDATA                     , "NEDRPC: More data to follow" }
   , { ERROR_RPC_NO_DATA_RETURNED             , "NEDRPC: No data returned by host" }
   , { ERROR_RPC_INVALID_HOST_HEADER          , "NEDRPC: Invalid RPC Header sent by host" }
   , { ERROR_RPC_INVALID_CLIENT_HEADER        , "NEDRPC: Invalid RPC Header sent by client" }
   , { ERROR_RPC_INVALID_HOST_NAME            , "NEDRPC: HostName invalid or host down" }
   , { ERROR_RPC_INVALID_SOCKET_NAME          , "NEDRPC: SocketName not specified or invalid" }
   , { ERROR_RPC_CLISERV                      , "NEDRPC: ClientServer Error" }
   , { ERROR_RPC_NO_MEMORY                    , "NEDRPC: Insufficient memory" }
   , { ERROR_RPC_TOO_LONG                     , "NEDRPC: Message to long" }
   , { ERROR_RPC_INETADDR                     , "NEDRPC: Unable to Get Internet address for device" }
   , { ERROR_RPC_NOTREADY                     , "NEDRPC: RPC object is notready use Setup() method" }
   , { ERROR_RPC_NODEKEYNOTSET                , "NEDRPC: NODEKEY not registered for this device" }
   , { ERROR_RPC_USERNOTSET                   , "NEDRPC: User not registered for this device" }
   , { ERROR_RPC_HOSTNAME                     , "NEDRPC: Unable to Get HostName for device" }
   , { ERROR_RPC_ENVIRONMENT_NOTSET           , "NEDRPC: Environment variable must be set" }

// Winsock errors
#if defined(_WINDOWS) || defined(_WIN32)
   , { WSAENAMETOOLONG                        , "WINSOCK: Name too long" }
   , { WSANOTINITIALISED                      , "WINSOCK: Not initialized" }
   , { WSASYSNOTREADY                         , "WINSOCK: System not ready" }
   , { WSAVERNOTSUPPORTED                     , "WINSOCK: Version is not supported" }
   , { WSAESHUTDOWN                           , "WINSOCK: Can't send after socket shutdown" }
   , { WSAEINTR                               , "WINSOCK: Interrupted system call" }
   , { WSAHOST_NOT_FOUND                      , "WINSOCK: Host not found" }
   , { WSATRY_AGAIN                           , "WINSOCK: Try again" }
   , { WSANO_RECOVERY                         , "WINSOCK: Non-recoverable error" }
   , { WSANO_DATA                             , "WINSOCK: No data record available" }
   , { WSAEBADF                               , "WINSOCK: Bad file number" }
   , { WSAEWOULDBLOCK                         , "WINSOCK: Operation would block" }
   , { WSAEINPROGRESS                         , "WINSOCK: Operation now in progress" }
   , { WSAEALREADY                            , "WINSOCK: Operation already in progress" }
   , { WSAEFAULT                              , "WINSOCK: Bad address" }
   , { WSAEDESTADDRREQ                        , "WINSOCK: Destination address required" }
   , { WSAEMSGSIZE                            , "WINSOCK: Message too long" }
   , { WSAEPFNOSUPPORT                        , "WINSOCK: Protocol family not supported" }
   , { WSAENOTEMPTY                           , "WINSOCK: Directory not empty" }
   , { WSAEPROCLIM                            , "WINSOCK: EPROCLIM returned" }
   , { WSAEUSERS                              , "WINSOCK: EUSERS returned" }
   , { WSAEDQUOT                              , "WINSOCK: Disk quota exceeded" }
   , { WSAESTALE                              , "WINSOCK: ESTALE returned" }
   , { WSAEINVAL                              , "WINSOCK: Invalid argument" }
   , { WSAEMFILE                              , "WINSOCK: Too many open files" }
   , { WSAEACCES                              , "WINSOCK: Access denied" }
   , { WSAELOOP                               , "WINSOCK: Too many levels of symbolic links" }
   , { WSAEREMOTE                             , "WINSOCK: The object is remote" }
   , { WSAENOTSOCK                            , "WINSOCK: Socket operation on non-socket" }
   , { WSAEADDRNOTAVAIL                       , "WINSOCK: Can't assign requested address" }
   , { WSAEADDRINUSE                          , "WINSOCK: Address already in use" }
   , { WSAEAFNOSUPPORT                        , "WINSOCK: Address family not supported by protocol family" }
   , { WSAESOCKTNOSUPPORT                     , "WINSOCK: Socket type not supported" }
   , { WSAEPROTONOSUPPORT                     , "WINSOCK: Protocol not supported" }
   , { WSAENOBUFS                             , "WINSOCK: No buffer space is supported" }
   , { WSAETIMEDOUT                           , "WINSOCK: Connection timed out" }
   , { WSAEISCONN                             , "WINSOCK: Socket is already connected" }
   , { WSAENOTCONN                            , "WINSOCK: Socket is not connected" }
   , { WSAENOPROTOOPT                         , "WINSOCK: Bad protocol option" }
   , { WSAECONNRESET                          , "WINSOCK: Connection reset by peer" }
   , { WSAECONNABORTED                        , "WINSOCK: Software caused connection abort" }
   , { WSAENETDOWN                            , "WINSOCK: Network is down" }
   , { WSAENETRESET                           , "WINSOCK: Network was reset" }
   , { WSAECONNREFUSED                        , "WINSOCK: Connection refused" }
   , { WSAEHOSTDOWN                           , "WINSOCK: Host is down" }
   , { WSAEHOSTUNREACH                        , "WINSOCK: Host is unreachable" }
   , { WSAEPROTOTYPE                          , "WINSOCK: Protocol is wrong type for socket" }
   , { WSAEOPNOTSUPP                          , "WINSOCK: Operation not supported on socket" }
   , { WSAENETUNREACH                         , "WINSOCK: ICMP network unreachable" }
   , { WSAETOOMANYREFS                        , "WINSOCK: Too many references" }

// CliSock errors
   , { ERROR_CLISOC_BASE + OURMSGTOSHORT      , "CLISOCK: Message to send must be > 0 long" }
   , { ERROR_CLISOC_BASE + OURVERINCOMPAT     , "CLISOCK: WinSock version incompatible" }
   , { ERROR_CLISOC_BASE + OURDATAWRITE       , "CLISOCK: Data written < Expected Data written" }
   , { ERROR_CLISOC_BASE + OURDATAREAD        , "CLISOCK: Data read < Expected Data read" }
   , { ERROR_CLISOC_BASE + OURMEMALLOC        , "CLISOCK: Memory alloc failed" }
   , { ERROR_CLISOC_BASE + OURPERMISSION      , "CLISOCK: Permissions invalid on host" }
   , { ERROR_CLISOC_BASE + OURSPACE           , "CLISOCK: No disk space on host" }
   , { ERROR_CLISOC_BASE + OURINVALIDSOCK     , "CLISOCK: Socket is not connected" }
   , { ERROR_CLISOC_BASE + OURFILENOTFOUND    , "CLISOCK: Data file not found" }
#endif

// Diary module errors
   , { ERROR_DM_INVALID_VERSION               , "DIARY: Invalid version" }
   , { ERROR_DM_DB_FAILURE                    , "DIARY: Database failure" }
   , { ERROR_DM_INVALID_REMINDERDATE          , "DIARY: Invalid ReminderDate" }
   , { ERROR_DM_INVALID_EVENTTIME             , "DIARY: Invalid EventTime" }
   , { ERROR_DM_NOREPEAT                      , "DIARY: No more repeats, message not generated" }
   , { ERROR_DM_REF_NOT_FOUND                 , "DIARY: Diary Event not found" }
   , { ERROR_DM_INVALID_ORIGINATING_SYSTEM    , "DIARY: Invalid Originating System" }
   , { ERROR_DM_INVALID_REFNO                 , "DIARY: Invalid Diary Reference Number" }
   , { ERROR_DM_INVALID_DIARYACTION           , "DIARY: Invalid DiaryAction" }
   , { ERROR_DM_NOMEMORY                      , "DIARY: Insufficient memory" }
   , { ERROR_DM_ANY_WITH_INVALID_INDEX        , "DIARY: If DayOfWeek = ANY, IndexIntoMonth must be LAST or 1..31" }
   , { ERROR_DM_DAY_WITH_INVALID_INDEX        , "DIARY: If DayOfWeek <> ANY, IndexIntoMonth must be LAST or 1..5" }
   , { ERROR_DM_INVALID_FIRSTDATE             , "DIARY: Invalid FirstDate" }
   , { ERROR_DM_INVALID_LASTDATE              , "DIARY: Invalid LastDate" }
   , { ERROR_DM_INVALID_BASEDATE              , "DIARY: Invalid BaseDate" }
   , { ERROR_DM_INVALID_DATETIME              , "DIARY: Invalid DateTime" }
   , { ERROR_DM_INVALID_EVENT                 , "DIARY: Invalid Event Pointer" }
   , { ERROR_DM_INI_FILE_ERROR                , "DIARY: Error reading ini file" }
   , { ERROR_DM_INVALID_DATE                  , "DIARY: Date Error" }
   , { ERROR_DM_SEND_ERROR                    , "DIARY: Error sending diary event" }
   , { ERROR_DM_GEN_REFNO_FAILURE             , "DIARY: Error generating reference number" }

   // Commission & Charges Errors
   , { ERROR_CC_INVALID_VERSION               , "CHARGES: Invalid version" }
   , { ERROR_CC_DB_FAILURE                    , "CHARGES: Database failure" }
   , { ERROR_CC_INVALID_ORIGINATING_SYSTEM    , "CHARGES: Invalid Originating System" }
   , { ERROR_CC_NOMEMORY                      , "CHARGES: Insufficient memory" }
   , { ERROR_CC_INVALID_PROD_CODE             , "CHARGES: Invalid Product Code" }
   , { ERROR_CC_INVALID_EVENT_CODE            , "CHARGES: Invalid Event Code" }
   , { ERROR_CC_INVALID_CLIENT_NO             , "CHARGES: Invalid Client Number" }
   , { ERROR_CC_INVALID_AMT                   , "CHARGES: Invalid Principal Amount" }
   , { ERROR_CC_INVALID_CURRENCY              , "CHARGES: Invalid Currency Code" }
   , { ERROR_CC_INVALID_BRANCH                , "CHARGES: Invalid Branch Code" }
   , { ERROR_CC_NOT_LOGGED_ON                 , "CHARGES: Server was not logged-on to database" }
   , { ERROR_CC_NO_DATA                       , "CHARGES: No data recieved from host (ccCalcCharges unsuccessful)" }
   , { ERROR_CC_INIFILE                       , "CHARGES: Charges.ini file missing or invalid" }

// Drafts and Transfers Errors
   , { ERROR_DT_CORRUPT_DATA                  , "D&T: Corrupt Data" }
   , { ERROR_DT_REFERENCE                     , "D&T: Invalid Reference Number" }
   , { ERROR_DT_INVALID_REQUEST               , "D&T: Invalid Request" }
   , { ERROR_DT_INI_FILE                      , "D&T: INI File Error" }
   , { ERROR_DT_MEMORY                        , "D&T: Out of Memory" }
   , { ERROR_DT_INVALID                       , "D&T: Invalid Request" }
   , { ERROR_DT_LOCKED                        , "D&T: The record is locked" }
   , { ERROR_DT_DB_ERROR                      , "D&T: Database Error" }
   , { ERROR_DT_INVALID_ACTION                , "D&T: Invalid Action" }
   , { ERROR_DT_SF_INIT                       , "D&T: S&F initialization Failure" }
   , { ERROR_DT_CTRL_REFERENCE                , "D&T: Control Reference Error" }
   , { ERROR_DT_NO_DATA_FOUND                 , "D&T: No more data" }
   , { ERROR_DT_SF                            , "D&T: S&F Error" }
   , { ERROR_DT_NOT_READY_FOR_PROCESS         , "D&T: Transaction not ready for processing" }
   , { ERROR_DT_INVALID_MSG_TYPE              , "D&T: Invalid Message Type" }
   , { ERROR_DT_ST400_REJECT                  , "D&T: Rejection by ST400" }
   , { ERROR_DT_INVALID_SEQNO                 , "D&T: Invalid Sequence Number" }
   , { ERROR_DT_RAND_EQUIV                    , "D&T: Error calculating rand equivalent" }
   , { ERROR_DT_NOT_LOCKED                    , "D&T: Record is not locked" }
   , { ERROR_DT_PAYMENT_EXISTS                , "D&T: A Payment record already exists" }
   , { ERROR_DT_NO_PAYMENT                    , "D&T: There is no existsing payment record" }
   , { ERROR_DT_NO_BENEFICIARY                , "D&T: No beneficiary exists for this refence number" }
   , { ERROR_DT_INVALID_STATUS                , "D&T: Invalid Status" }
   , { ERROR_DT_INVALID_CCY                   , "D&T: Invalid Currency" }
   , { ERROR_DT_BUSY_PROCESSING               , "D&T: Busy Processing" }
   , { ERROR_DT_INVALID_RECIPIENT             , "D&T: Invalid Recipient" }
   , { ERROR_DT_INVALID_PARAMETER             , "D&T: Invalid Parameter" }
   , { ERROR_DT_INVALID_HANDLE                , "D&T: Invalid Handle" }

// Settlement Errors
   , { ERROR_ST_CORRUPT_DATA                  , "Settlement: Corrupt Data" }
   , { ERROR_ST_REFERENCE                     , "Settlement: Invalid Reference Number" }
   , { ERROR_ST_NO_MEMORY                     , "Settlement: Out of Memory" }
   , { ERROR_ST_DB_ERROR                      , "Settlement: Database Error" }
   , { ERROR_ST_DUPLICATE_REFERENCE           , "Settlement: Duplicate Reference Number" }
   , { ERROR_ST_COMMS_FAILURE                 , "Settlement: Communications Failure" }
   , { ERROR_ST_BAD_REQUEST                   , "Settlement: Invalid Request" }
   , { ERROR_ST_INCOMPLETE                    , "Settlement: Transaction Incomplete" }
   , { ERROR_ST_RAND_EQUIV                    , "Settlement: Rand Equivalent Error" }
   , { ERROR_ST_FAP_REJECT                    , "Settlement: Rejected by FAP" }
   , { ERROR_ST_PROCESS                       , "Settlement: Processing Error" }

#if 0
// SQLAPI Errors
   , { SqlApiMemErr                           , "SQLAPI: Out of memory" }
   , { SqlApiBinFileErr                       , "SQLAPI: Error with bin file" }
   , { SqlApiConnectErr                       , "SQLAPI: Could not connect" }
   , { SqlApiNoQueryErr                       , "SQLAPI: Query not found" }
   , { SqlApiOpenErr                          , "SQLAPI: Error opening" }
   , { SqlApiParseErr                         , "SQLAPI: Error parsing" }
   , { SqlApiBindErr                          , "SQLAPI: Error binding" }
   , { SqlApiDefineErr                        , "SQLAPI: Error defining" }
   , { SqlApiExecErr                          , "SQLAPI: Error executing" }
   , { SqlApiFetchErr                         , "SQLAPI: Error fetching" }
   , { SqlApiCancelErr                        , "SQLAPI: Error cancelling" }
   , { SqlApiCloseErr                         , "SQLAPI: Error closing" }
   , { SqlApiCommitErr                        , "SQLAPI: Error Committing" }
   , { SqlApiRollbackErr                      , "SQLAPI: Error rolling back" }
   , { SqlApiNoMoreRows                       , "SQLAPI: No more rows" }
   , { SqlApiCBErr                            , "SQLAPI: CB error" }
   , { SqlApiCursorErr                        , "SQLAPI: Cursor error" }
   , { SqlApiStateErr                         , "SQLAPI: State error" }
   , { SqlApiAllocEnvErr                      , "SQLAPI: Error allocating environment" }
   , { SqlApiAllocConnectErr                  , "SQLAPI: Error allocating connection" }
   , { SqlApiUnsupportedErr                   , "SQLAPI: Unsupported" }
   , { SqlApiNoFldname                        , "SQLAPI: No field name" }
   , { SqlApiKeyFileErr                       , "SQLAPI: Key file error" }
#endif

// Rates Server
   , { ERROR_RS_COMMS_FAILURE                 , "RATES : Communication Failure" }
   , { ERROR_RS_DB_ERROR                      , "RATES : Database Error" }
   , { ERROR_RS_INVALID_REQUEST               , "RATES : Invalid Request" }
   , { ERROR_RS_FAP_FILE                      , "RATES : Invalid FAP filename" }
   , { ERROR_RS_RATECODE                      , "RATES : Invalid rate code" }
   , { ERROR_RS_MEMORY                        , "RATES : Error allocating memory" }
   , { ERROR_RS_CCYCODE                       , "RATES : Unknown currency code" }
};

void GetErrorMessage(int rc, char *pBuffer, int cbBuffer)
{
   if (!pBuffer || !cbBuffer)
      return;

   char* errmsg = "Unknown error";
   for (int i=0; i < (sizeof(_ErrMsg)/sizeof(ERRMSG)); i++)
      if (_ErrMsg[i].ErrorNo == rc)
      {
         errmsg = _ErrMsg[i].Text;
         break;
      }

   strncpyz(pBuffer, errmsg, cbBuffer - 1);
}

