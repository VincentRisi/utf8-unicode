#include "versions.h"
#define _VERSION_NO "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
COMPILED_VERSION(_VERSION_NO);

/*=======================================================================
=                                                                       =
= Author       : N.Rademeyer                                            =
= File         : mqseries.cpp                                           =
= Date Written : 19970912                                               =
= Description  : Class for accessing MQSeries queues.                   =
= --------------------------------------------------------------------- =
=                           CHANGE HISTORY                              =
= --------------------------------------------------------------------- =
=   DATE  | BY |   VER   |               DESCRIPTION                    =
= --------+----+---------+--------------------------------------------- =
= 19970912| NR | 1.00.00 | Created                                      =
=======================================================================*/
#include <string.h>
#include "mqseries.h"
#include "errbase.h"


#include "ti.h"


// Local Macro used in a couple of procs
#define SETUP_QUEUE \
   if ( QPtr == 0 )\
      if ( MQQueueName )\
      {\
         QPtr = GetQueueByName(MQQueueName);\
         if ( QPtr == 0 )\
            throw XMQConnect(errMQQName);\
         CurQueue = QPtr;\
      }\
      else\
         QPtr = CurQueue;\
   else\
      CurQueue = QPtr;\
   if ( QPtr == 0 )\
      throw XMQConnect(errMQQueue);

//---------------------------------------------------------------------------
// Constructor connects to MQueue Manager. NB '' is the default Queue Manager
//---------------------------------------------------------------------------
tMQConnect::tMQConnect(char *iMQManagerName)
{
   CurQueue = 0;
   QueueHead = 0;
   Buffer = 0;
   DataLen = 0;
   MQManCon = 0;
   QManagerConnect(iMQManagerName);
}

//---------------------------------------------------------------------------
// Destructor disconnects from Queue Manager
//---------------------------------------------------------------------------
tMQConnect::~tMQConnect()
{
   QManagerDisconnect();
}


//---------------------------------------------------------------------------
// Connects to MQueue Manager. NB '' is the default Queue Manager
// Returns : ERROR_OK if successfull
//           ERROR_MQ_CONNECT if unsucessfull
//---------------------------------------------------------------------------
int tMQConnect::QManagerConnect(char *iMQManagerName)
{
   strncpy(MQManagerName,iMQManagerName,sizeof(MQManagerName));
   MQCONN(MQManagerName,               // Queue manager
          &MQManCon,                   // Connection handle
          &CompCode,                   // Completion code
          &ReasonCode);                // Reason code

   if ( CompCode == MQCC_FAILED )
      throw XrMQConnect(errMQConnect);

   ///////////////////////////////////////////////////////////////////////////
   // Find out Queues max message length
   ///////////////////////////////////////////////////////////////////////////
   MQOD     odI = {MQOD_DEFAULT};   // Object Descriptor for INQUIRE
   MQHOBJ   Hinq;                   // handle for MQINQ
   MQLONG   Options;
   MQLONG   Selection;              // Selection options (what do we want to enquire)
   MQLONG   IAV;                    // Integer Attrib Values (results)

   strncpy(odI.ObjectQMgrName,iMQManagerName,sizeof(MQManagerName));

   odI.ObjectType=MQOT_Q_MGR;
   Options = MQOO_INQUIRE + MQOO_FAIL_IF_QUIESCING;
   MQOPEN(MQManCon,
        &odI,                    // Inquire object descriptor
        Options,
        &Hinq,                   // object handle
        &CompCode,               // MQOPEN completion code
        &ReasonCode);            // reason code
   if (ReasonCode != MQRC_NONE)
   {
      QManagerDisconnect();
      throw XrMQConnect(errMQConnect);
   }

   Selection = MQIA_MAX_MSG_LENGTH;
   MQINQ(MQManCon,         // connection handle
         Hinq,             // object handle
         1L,               // Selector count
         &Selection,       // Selector array
         1L,               // integer attribute count
         &IAV,             // integer attribute array
         0,                // character attribute count
         0,                // character attribute array
         &CompCode,        // completion code
         &ReasonCode);     // reason code
   if (CompCode != MQCC_OK)
   {
      QManagerDisconnect();
      throw XrMQConnect(errMQConnect);
   }
   MQMaxDataLen=IAV;

   return ERROR_OK;
}


//---------------------------------------------------------------------------
// Disconnects from connected MQueue Manager.
//---------------------------------------------------------------------------
int tMQConnect::QManagerDisconnect()
{
   if ( MQManCon == 0 ) // Not connected
      return ERROR_OK;

   // First close all open Queues (and free the memory allocated to them)
   CurQueue = QueueHead;
   while ( CurQueue )
   {
      try {
         QClose();               // This closes and moves on the CurQueue pointer
      } catch(xMQConnect &x) {}  // Not interested in errors
   }

   // Disconnect Queue Manager
   MQDISC(&MQManCon,                     // connection handle
            &CompCode,                   // completion code
            &ReasonCode);                // reason code

   if (ReasonCode != MQRC_NONE )
      throw XrMQConnect(errMQDisconnect);
   return ERROR_OK;
}


//---------------------------------------------------------------------------
// Opens the named Queue and sets the current queue info
// Returns pointer to the new queue element
//---------------------------------------------------------------------------
tMQQueue * tMQConnect::QOpen(char *MQQueueName,MQLONG Options,
                      char *AlternateBuffer,ulong AlternateBufferLen)
{
MQOD     MQODTemplate = {MQOD_DEFAULT};


   // Allocate memory for new Queue
   tMQQueue *TempQueue=new tMQQueue;
   if ( TempQueue == 0 )
      throw XMQConnect(errMQMemAlloc);
   TempQueue->Next = 0;

   // Allocate buffer (or use given buffer)
   if ( AlternateBuffer == 0 )
   {
      // Allocate buffer to size of MAX message reported by Queue Manager
      TempQueue->Buffer = new char[MQMaxDataLen];
      if ( TempQueue->Buffer == 0 )
      {
         delete TempQueue;
         throw XMQConnect(errMQMemAlloc);
      }
      TempQueue->BufferLen = MQMaxDataLen;
      TempQueue->UsingAlternateBuffer = FALSE;
   }
   else
   {
      // Use buffer provided
      TempQueue->Buffer=AlternateBuffer;
      TempQueue->BufferLen=AlternateBufferLen;
      TempQueue->UsingAlternateBuffer = TRUE;
   }
   TempQueue->DataLen=0;


   // Setup queue info
   memcpy((void *)&TempQueue->QObjDescriptor,(void *)&MQODTemplate,sizeof(TempQueue->QObjDescriptor));
   strncpy(TempQueue->QObjDescriptor.ObjectName,MQQueueName,sizeof(TempQueue->QObjDescriptor.ObjectName));
   strncpy(TempQueue->Name,MQQueueName,sizeof(TempQueue->Name));
   TempQueue->Options = Options;

   // Open Queue
   MQOPEN(MQManCon,                    // connection handle
          &TempQueue->QObjDescriptor,  // object descriptor for queue
          Options,                     // open options
          &TempQueue->QObjHandle,      // object handle
          &CompCode,                   // completion code
          &ReasonCode);                // Reason code
   if ( ReasonCode != MQRC_NONE || CompCode == MQCC_FAILED )
   {
      delete TempQueue;
      throw XrMQConnect(errMQOpen);
   }

   // Link new Queue element into linked list
   tMQQueue *QueuePtr,*PrevQueuePtr=0;
   QueuePtr = QueueHead;
   while ( QueuePtr )
   {
      PrevQueuePtr=QueuePtr;
      QueuePtr = QueuePtr->Next;
   }

   if ( PrevQueuePtr )
      PrevQueuePtr->Next = TempQueue;
   else
      QueueHead = TempQueue;

   CurQueue = TempQueue;
   Buffer=CurQueue->Buffer;
   DataLen=CurQueue->DataLen;

   return CurQueue;
}

//---------------------------------------------------------------------------
// Closes the named Queue,Current Queue, Or Queue referenced by queue ptr
// Returns the current (or new current queue pointer)
//---------------------------------------------------------------------------
tMQQueue * tMQConnect::QClose(tMQQueue *QPtr,char *MQQueueName)
{
MQLONG Options=0;                      // No close options

   SETUP_QUEUE                         // Setup Queue specified to be used and default CurQueue

   MQCLOSE(MQManCon,                   // connection handle
         &QPtr->QObjHandle,            // object handle
         Options,
         &CompCode,                    // completion code
         &ReasonCode);                 // reason code
   if (ReasonCode != MQRC_NONE)
      throw XrMQConnect(errMQClose);

   if ( QPtr == CurQueue )
      CurQueue = CurQueue->Next;

   // Free the linked list member and remove it from the list
   if ( QPtr == QueueHead )
      QueueHead = QueueHead->Next;
   else
   {
      tMQQueue *TempQueuePtr=QueueHead,*PrevQueuePtr=0;

      while ( TempQueuePtr->Next != QPtr )
         TempQueuePtr = TempQueuePtr->Next;
      TempQueuePtr->Next=QPtr->Next;
   }
   if ( QPtr->UsingAlternateBuffer == FALSE )
      delete QPtr->Buffer;

   delete QPtr;

   if ( CurQueue )
   {
      Buffer=CurQueue->Buffer;
      DataLen=CurQueue->DataLen;
   }
   return CurQueue;
}


//---------------------------------------------------------------------------
// Reads from the current, or named Queue
// Returns a pointer to the read Queue
//---------------------------------------------------------------------------
tMQQueue * tMQConnect::Read(MQLONG ReadWaitInterval,MQLONG Options,char *MQQueueName)
{
   return Read(ReadWaitInterval,Options,0,MQQueueName);
}

tMQQueue * tMQConnect::Read(MQLONG ReadWaitInterval,MQLONG Options,tMQQueue *QPtr,char *MQQueueName)
{
MQMD    MsgDscrptrTemplate = {MQMD_DEFAULT};    // Message Descriptor
MQGMO   MsgOptionsTemplate = {MQGMO_DEFAULT};   // get message options

   SETUP_QUEUE                         // Setup Queue specified to be used and default CurQueue
   Buffer=CurQueue->Buffer;

   // Setup message information
   memcpy((void *)&MsgDscrptr,(void *)&MsgDscrptrTemplate,sizeof(MsgDscrptr));
   memcpy((void *)&GetMsgOptions,(void *)&MsgOptionsTemplate,sizeof(GetMsgOptions));
   memcpy(MsgDscrptr.MsgId, MQMI_NONE, sizeof(MsgDscrptr.MsgId));
   memcpy(MsgDscrptr.CorrelId, MQCI_NONE, sizeof(MsgDscrptr.CorrelId));
   GetMsgOptions.Options = Options;

   memset(QPtr->Buffer,0,QPtr->BufferLen);

   GetMsgOptions.WaitInterval = ReadWaitInterval;
   MQGET(MQManCon,               // connection handle
         QPtr->QObjHandle,       // object handle
         &MsgDscrptr,            // message descriptor
         &GetMsgOptions,         // get message options
         QPtr->BufferLen,        // buffer length
         QPtr->Buffer,           // message buffer
         &QPtr->DataLen,         // message length
         &CompCode,              // completion code
         &ReasonCode);           // reason code
   if (ReasonCode != MQRC_NONE)
      throw XrMQConnect(errMQRead);
   if ( QPtr->DataLen < QPtr->BufferLen )
      QPtr->Buffer[QPtr->DataLen] = '\0';  // Add null terminator
   memcpy(MsgId,MsgDscrptr.MsgId,sizeof(MsgId));
   memcpy(CorrelId,MsgDscrptr.CorrelId,sizeof(CorrelId));

   DataLen = CurQueue->DataLen;
   return CurQueue;
}

//---------------------------------------------------------------------------
// Writes to open Queue
//---------------------------------------------------------------------------
tMQQueue * tMQConnect::Write(MQLONG Options) // M
{
   return Write(0,0,0,Options,0);
}
tMQQueue * tMQConnect::Write(tMQQueue *QPtr,MQLONG Options) // M,O
{
   return Write(0,0,QPtr,Options,0);
}
tMQQueue * tMQConnect::Write(char *MQQueueName,MQLONG Options) // M,O
{
   return Write(0,0,0,Options,MQQueueName);
}

tMQQueue * tMQConnect::Write(char *SrcData,ulong SrcDataLen,char *MQQueueName,MQLONG Options) // M,M,M,O
{
   return Write(SrcData,SrcDataLen,0,Options,MQQueueName);
}
tMQQueue * tMQConnect::Write(char *SrcData,ulong SrcDataLen,tMQQueue *QPtr,MQLONG Options,char *MQQueueName)
{
MQMD    MsgDscrptrTemplate = {MQMD_DEFAULT};       // Message Descriptor
MQPMO   MsgOptionsTemplate = {MQPMO_DEFAULT};   /* put message options           */

   SETUP_QUEUE                         // Setup Queue specified to be used and default CurQueue

   if ( SrcData == 0 )
      SrcData=QPtr->Buffer;
   if ( SrcDataLen == 0 )
      SrcDataLen=QPtr->DataLen;
   Buffer=CurQueue->Buffer;

   // Setup message information
   memcpy((void *)&MsgDscrptr,(void *)&MsgDscrptrTemplate,sizeof(MsgDscrptr));
   memcpy((void *)&PutMsgOptions,(void *)&MsgOptionsTemplate,sizeof(PutMsgOptions));
   memcpy(MsgDscrptr.Format,MQFMT_NONE,(size_t)MQ_FORMAT_LENGTH);  // Char array format
   //memcpy(MsgDscrptr.Format,MQFMT_STRING,(size_t)MQ_FORMAT_LENGTH);  // Char string format
   //memcpy(MsgDscrptr.Format,MQFMT_NONE_ARRAY,(size_t)MQ_FORMAT_LENGTH);  // Char array format
   memcpy(MsgDscrptr.MsgId, MQMI_NONE, sizeof(MsgDscrptr.MsgId));
   memcpy(MsgDscrptr.CorrelId, MQCI_NONE, sizeof(MsgDscrptr.CorrelId));

   PutMsgOptions.Options=Options;

   // Write message
   MQPUT(MQManCon,               // connection handle
         QPtr->QObjHandle,       // object handle
         &MsgDscrptr,            // message descriptor
         &PutMsgOptions,            // default options (datagram)
         SrcDataLen,             // buffer length
         SrcData,                // message buffer
         &CompCode,              // completion code
         &ReasonCode);           // reason code

   if (ReasonCode != MQRC_NONE)
      throw XrMQConnect(errMQWrite);

   // I don't know if the following 2 lines are usefull on write
   memcpy(MsgId,MsgDscrptr.MsgId,sizeof(MsgId));
   memcpy(CorrelId,MsgDscrptr.CorrelId,sizeof(CorrelId));

   return CurQueue;
}


//---------------------------------------------------------------------------
// Rollback the current unit of work
//---------------------------------------------------------------------------
int tMQConnect::Rollback()
{
   MQBACK(MQManCon,               // connection handle
         &CompCode,              // completion code
         &ReasonCode);           // reason code
   if (ReasonCode != MQRC_NONE)
      throw XrMQConnect(errMQRollback);
   return ERROR_OK;
}


//---------------------------------------------------------------------------
// Commit the current unit of work
//---------------------------------------------------------------------------
int tMQConnect::Commit()
{
   MQCMIT(MQManCon,              // connection handle
         &CompCode,              // completion code
         &ReasonCode);           // reason code
   if (ReasonCode != MQRC_NONE)
      throw XrMQConnect(errMQCommit);
   return ERROR_OK;
}

//---------------------------------------------------------------------------
// Gets the Queue pointer, from the queue linked list, by QueueName
//---------------------------------------------------------------------------
tMQQueue * tMQConnect::GetQueueByName(char *MQQueueName)
{
tMQQueue *TempQueue;

   TempQueue=QueueHead;
   while ( TempQueue )
   {
      if ( strcmp(TempQueue->Name,MQQueueName) == 0 )
         break;
      TempQueue = TempQueue->Next;
   }
   return TempQueue;
}



//---------------------------------------------------------------------------
// Exception class methods.
//---------------------------------------------------------------------------
xMQConnect::xMQConnect(pchar aFname, int aLine, eError aErrorNo,MQLONG aCompCode,MQLONG aReasonCode) :
     xCept(aFname, aLine, "xMQConnect", aErrorNo)
{
static char *eErrMsg[] = {
         "Connect error",
         "Disconnect error",
         "Open error",
         "Close error",
         "Read error",
         "Write error",
         "Rollback error",
         "Commit error",
         "Memory allocation error",
         "Invalid queue name",
         "Invalid queue",
         "Unknown error"
      };
   if ( aErrorNo > errMQUnknown )
      aErrorNo = errMQUnknown;

   ReasonCode = aReasonCode;
   CompCode = aCompCode;

   ErrorNumber = aErrorNo;
   eErrorMessage= eErrMsg[aErrorNo];
   osErr << eErrMsg[aErrorNo] << "(" << CompCode << "," << ReasonCode << ")" << std::endl;
}
