
/******************************************************/
/*                                                    */
/*  System : dmRPC                                    */
/*  File   : dmRPC.H                                  */
/*  Author : Louis Van Dyk                            */
/*  Date   : Mon 25-Jul-1994                          */
/*                                                    */
/*  Change History :                                  */
/*                                                    */
/******************************************************/
#ifndef _DMRPC_H_

#define _DMRPC_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_DMRPC_H_);


/******************************************************/
/*  CONSTANTS                                         */
/******************************************************/

/* BASE FOR REQUEST CODES */
#define RQ_DM_BASE          2000

/* REQUEST CODES */
#define RQ_DM_CREATE_SIMPLE_EVENT                      RQ_DM_BASE
#define RQ_DM_CREATE_REPEAT_EVENT                      RQ_DM_BASE + 1
#define RQ_DM_CREATE_MONTHLY_EVENT                     RQ_DM_BASE + 2
#define RQ_DM_DELETE_EVENT                             RQ_DM_BASE + 3
#define RQ_DM_QUERY_EVENTS                             RQ_DM_BASE + 4
#define RQ_DM_VIEW_EVENT                               RQ_DM_BASE + 5
#define RQ_DM_EVENT                                    RQ_DM_BASE + 6

/* DIARY ACTION */

#define CN_DM_SIMPLEEV              1
#define CN_DM_MONTHLYEV             2
#define CN_DM_REPEATEV              3

/* WORK DAY ACTION */

#define CN_DM_NOTHING               0
#define CN_DM_ADVANCE_TO_NEXT       1
#define CN_DM_ADVANCE_TO_PREVIOUS   2

/* REPEAT TYPE */

#define CN_DM_DAILY             0
#define CN_DM_WEEKLY            1
#define CN_DM_MONTHLY           2

/* INDEX */

#define CN_DM_NONE          0
#define CN_DM_LAST          999

/* DAY OF WEEK */

#define CN_DM_ANY           0
#define CN_DM_MON           1
#define CN_DM_TUE           2
#define CN_DM_WED           3
#define CN_DM_THU           4
#define CN_DM_FRI           5
#define CN_DM_SAT           6
#define CN_DM_SUN           7

/* WEEKEND TYPE */

#define CN_DM_SATSUN            0  /*Both SAT & SUN - weekend*/
#define CN_DM_SUNONLY           1
#define CN_DM_NEITHERSATSUN     2


/******************************************************/
/*  TYPES                                             */
/******************************************************/
#pragma pack(4)
typedef struct dmEventTag
{
   short  DiaryAction;     /* SMR   CN_DM_SIMPLEEV, CN_DM_MONTHLYEV or CN_DM_REPEATEV  */
   short  WorkDayAction;   /* SMR   CN_DM_NOTHING, CN_DM_ADVANCE_TO_NEXT or CN_DM_ADVANCE_TO_PREVIOUS */
   short  WeekendType;     /* SMR   CN_DM_SATSUN, CN_DM_SUNONLY or CN_DM_NEITHERSATSUN */
   char   EventTime[7];    /* SMR   HHMMSS */
   char   ReminderDate[9]; /* S--   CCYYMMDD */
   short  DayOfWeek;       /* -M-   CN_DM_ANY or CN_DM_MON..CN_DM_SUN */
   short  IndexIntoMonth;  /* -M-   CN_DM_NONE or CN_DM_LAST */
   short  OffsetOnDate;    /* -M-   -n..0..n */
   short  RepeatType;      /* --R   CN_DM_DAILY, CN_DM_WEEKLY or CN_DM_MONTHLY */
   short  RepeatInterval;  /* --R   1..n */
   short  MaxReminders;    /* -MR   1..n */
   char   FirstDate[9];    /* -MR   CCYYMMDD */
   char   LastDate[9];     /* -MR   CCYYMMDD */
   char   BaseDate[9];     /* -MR   CCYYMMDD */
} dmEventType;

typedef struct dmDiaryRefTag
{
   char  DiaryRefNo[11];
} dmDiaryRefType;

typedef struct dmDiaryApRefTag
{
   char ApRef1[21];
   char ApRef2[21];
   char ApRef3[21];
} dmApRefType;

typedef struct dmDiaryBlobTag
{
   char             OrigSystem[5];
   dmEventType      DiaryEvent;
   dmApRefType      ApRef;
} dmDiaryBlobType;

typedef struct dmQueryEntryTag
{
   char        DiaryRef[11];
   char        DateTime[15];
   dmApRefType ApRef;
} dmQueryEntryType;

typedef struct dmTxQueryEventTag
{
   char  OrigSystem[5];
   char  DateTime[15];
   short nMaxEntries;
} dmTxQueryEventType;

typedef struct dmRxQueryEventTag
{
   short             nEntriesRet;
   short             NotUsed;
   /* dmQueryEntryType  Entries[nArrayEntries];  appended to this structure */
} dmRxQueryEventType;

typedef struct dmCreateEventTag
{
   dmDiaryBlobType DiaryBlob;
   long            cbBlob;
   /* char         Blob[cbBlob]; appended to this structure */
} dmCreateEventType;

typedef struct tGetBlobData
{
   ushort len;
   uchar data[20480];
} dmBlobData;

#pragma pack()

/******************************************************/
/*  FUNCTION PROTOTYPES                               */
/******************************************************/

#ifndef _WINDOWS
   #ifndef WINAPI
      #define WINAPI
   #endif
#endif /* _WINDOWS */

#ifdef __cplusplus
extern "C" {
#endif

// dmSetup MUST be called, before any other API's are called.
#ifdef _WINDOWS
long WINAPI dmSetup (char *Host, char *Socket);
#else
long WINAPI dmSetup (char *RPCUserID, char *Socket);
#endif

long WINAPI dmCreateSimpleEvent (  dmDiaryBlobType *pDiaryEvent,
                                   char *pBlob,
                                   short sBlob,
                                   char *DiaryRefRet);

long WINAPI dmCreateMonthlyEvent (  dmDiaryBlobType *pDiaryEvent,
                                    char *pBlob,
                                    short sBlob,
                                    char *DiaryRefRet);

long WINAPI dmCreateRepeatEvent (  dmDiaryBlobType *pDiaryEvent,
                                   char *pBlob,
                                   short sBlob,
                                   char *DiaryRefRet);

long WINAPI dmDeleteEvent( char* DiaryRefNo );

long WINAPI dmQueryEvents (  char *OrigSystem,
                             char *DateTime,
                             char *ArrayOfEntries,
                             short nMaxEntries,
                             short *pnEntriesRet );

long WINAPI dmViewEvent( char *DiaryRefNo,
                         dmDiaryBlobType *DiaryBlob );

#ifdef __cplusplus
}
#endif


#endif /* _DMRPC_H_ */
