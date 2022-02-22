/****************************************************************************/
/*                                                                          */
/*  System : putty                                                          */
/*                                                                          */
/*  File   : mymq.cpp                                                       */
/*                                                                          */
/*  Target : mqout.exe                                                      */
/*                                                                          */
/*  Description :                                                           */
/*                                                                          */
/*  Author : S. Shemesh                                                     */
/*                                                                          */
/*  $Revision: 353 $                                                        */
/*                                                                          */
/*  $Date: 2011-10-19 11:18:45 +0200 (Wed, 19 Oct 2011) $                                            */
/*                                                                          */
/*  Change History :
*
*   $Log: mymq.Cpp,v $
*   Revision 1.4  2002/04/05 13:40:42  simon
*   add Heder file version macros
*
*   Revision 1.3  2002/04/05 08:33:06  simon
*   no message
*
*   Revision 1.2  2002/03/25 11:23:05  simon
*   no message
*
*/
/****************************************************************************/
#define VERSION_NO "$Revision: 353 $ $Date: 2011-10-19 11:18:45 +0200 (Wed, 19 Oct 2011) $"
#include "versions.h"
COMPILED_VERSION(VERSION_NO);
#include "dumyvers.h"
#include "ti.h"

#include <stdio.h>
#include <string.h>

#include "date.h"

#include "mqclient.h"
#include "errbase.h"


//#include <strstream.h>

using namespace std;

//---------------------------------------------------------------------------
// Constructor connects to MQueue Manager. NB '' is the default Queue Manager
//---------------------------------------------------------------------------
tMQSAMGR::tMQSAMGR(char *iMQManagerName) {
   QManagerConnect(iMQManagerName);
}

//---------------------------------------------------------------------------
// Constructor connects to MQueue Manager. NB '' is the default Queue Manager
//---------------------------------------------------------------------------
tMQSAMGR::tMQSAMGR(char *iMQServerName, int iMQServerPort, char *iMQChannelName, char *iMQManagerName, MQLONG iMaxMsgLength) {
   QManagerConnect(iMQServerName, iMQServerPort, iMQChannelName, iMQManagerName, iMaxMsgLength);
}


//---------------------------------------------------------------------------
// Connect Local
//---------------------------------------------------------------------------

void tMQSAMGR::QManagerConnect(char *iMQManagerName) {

   MQManCon   = 0;
   memset(MQManagerName, 0, sizeof(MQManagerName));
   strncpy(MQManagerName,iMQManagerName,sizeof(MQManagerName));

   MQCONN(MQManagerName,               // Queue manager
          &MQManCon,                   // Connection handle
          &CompCode,                   // Completion code
          &ReasonCode);                // Reason code

   if ( CompCode == MQCC_FAILED ) {
      throw XMQ("MQ Connect");
   }

   MQDataLen=0;

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
   if (ReasonCode != MQRC_NONE) {
      QManagerDisconnect();
      throw XMQ("MQ Connect");
   }

   Selection = MQIA_MAX_MSG_LENGTH;
   MQINQ(MQManCon,         // connection handle
         Hinq,             // object handle
         1L,               // Selector count
         &Selection,       // Selector array
         1L,               // integer attribute count
         &IAV,              // integer attribute array
         0,                // character attribute count
         0,                // character attribute array
         &CompCode,        // completion code
         &ReasonCode);     // reason code
   if (CompCode != MQCC_OK) {
      QManagerDisconnect();
      throw XMQ("MQConnect");
   }
   MQDataLen=IAV;
}

//---------------------------------------------------------------------------
// Connect via channel
//---------------------------------------------------------------------------

void tMQSAMGR::QManagerConnect(char *iMQServerName, int iMQServerPort, char *iMQChannelName, char *iMQManagerName, MQLONG iMaxMsgLength) {

   MQManCon = 0;
   memset(MQServerName, 0, sizeof(MQServerName));
   strncpy(MQServerName, iMQServerName, sizeof(MQServerName));

   MQServerPort = iMQServerPort;

   memset(MQChannelName, 0, sizeof(MQChannelName));
   strncpy(MQChannelName, iMQChannelName, sizeof(MQChannelName));

   memset(MQManagerName, 0, sizeof(MQManagerName));
   strncpy(MQManagerName, iMQManagerName, sizeof(MQManagerName));

   MQCNO    connect_options = { MQCNO_DEFAULT }; /* MQCONNX options    */
   MQCD     clientConn = { MQCD_CLIENT_CONN_DEFAULT };

   /* Client connection channel     */
   /* definition                    */
   strcpy(clientConn.ConnectionName, MQServerName);//, MQ_CONN_NAME_LENGTH);
   strcpy(clientConn.ChannelName, MQChannelName);
   clientConn.MaxMsgLength = iMaxMsgLength;
   connect_options.ClientConnPtr = &clientConn;
   connect_options.Version = MQCNO_VERSION_2;

   MQCONNX(MQManagerName,               // Queue manager
      &connect_options,
      &MQManCon,                   // Connection handle
      &CompCode,                   // Completion code
      &ReasonCode);                // Reason code

   if (CompCode == MQCC_FAILED)
   {
      throw XMQ("MQ Connect");
   }

   MQDataLen = 0;

   ///////////////////////////////////////////////////////////////////////////
   // Find out Queues max message length
   ///////////////////////////////////////////////////////////////////////////
   MQOD     odI = { MQOD_DEFAULT };   // Object Descriptor for INQUIRE
   MQHOBJ   Hinq;                   // handle for MQINQ
   MQLONG   Options;
   MQLONG   Selection;              // Selection options (what do we want to enquire)
   MQLONG   IAV;                    // Integer Attrib Values (results)

   strncpy(odI.ObjectQMgrName, iMQManagerName, sizeof(MQManagerName));

   odI.ObjectType = MQOT_Q_MGR;
   Options = MQOO_INQUIRE + MQOO_FAIL_IF_QUIESCING;
   MQOPEN(MQManCon,
      &odI,                    // Inquire object descriptor
      Options,
      &Hinq,                   // object handle
      &CompCode,               // MQOPEN completion code
      &ReasonCode);            // reason code
   if (ReasonCode != MQRC_NONE) {
      QManagerDisconnect();
      throw XMQ("MQ Connect");
   }

   Selection = MQIA_MAX_MSG_LENGTH;
   MQINQ(MQManCon,         // connection handle
      Hinq,             // object handle
      1L,               // Selector count
      &Selection,       // Selector array
      1L,               // integer attribute count
      &IAV,              // integer attribute array
      0,                // character attribute count
      0,                // character attribute array
      &CompCode,        // completion code
      &ReasonCode);     // reason code
   if (CompCode != MQCC_OK) {
      QManagerDisconnect();
      throw XMQ("MQConnect");
   }
   MQDataLen = IAV;
}

//---------------------------------------------------------------------------
// Disconnects from connected MQueue Manager.
//---------------------------------------------------------------------------
int tMQSAMGR::QManagerDisconnect() {

   MQLONG   DiscCompCode;                           // local Completion code
   MQLONG   DiscReasonCode;                         // local Reason code

// Disconnect Queue Manager
   MQDISC(&MQManCon,                   // connection handle
          &DiscCompCode,                   // completion code
          &DiscReasonCode);                // reason code

   if (ReasonCode != MQRC_NONE ) {
       CompCode   = DiscCompCode;
       ReasonCode = DiscReasonCode;
      throw XMQ("MQDisconnect");
   }
   return ERROR_OK;
}

tMQSAMGR::~tMQSAMGR() {

      MQDISC(&MQManCon,                   // connection handle
             &CompCode,                   // completion code
             &ReasonCode);                // reason code

      if (ReasonCode != MQRC_NONE ) {
         printf("MQDisconnect MGR(%s) Failed CompCode(%ld) ReasonCode(%ld) %s \n", MQManagerName, CompCode, ReasonCode, xMQ::ResoneCodeText(ReasonCode));
      }
}


tMQ::tMQ(tMQSAMGR & aMqsaMgr)
: MqsaMgr(aMqsaMgr)
{
//   MQData = 0;
   MQDataLen = 0;
//   MQSupliedBuffer = false;

//   MQDataLen = aMqsaMgr.MQDataLen;
   MQManCon = aMqsaMgr.MQManCon;
   strcpy(MQManagerName, aMqsaMgr.MQManagerName);

   QObjHandle = 0;
   memset(MQQueueName, 0, sizeof(MQQueueName));

   MQMD    MsgDscrptrTemplate = {MQMD_DEFAULT};       // Message Descriptor
   MQPMO   MsgOptionsTemplate = {MQPMO_DEFAULT};   /* put message options           */
   memcpy((void *)&MsgDscrptr,(void *)&MsgDscrptrTemplate,sizeof(MsgDscrptr));
   memcpy((void *)&PutOptions,(void *)&MsgOptionsTemplate,sizeof(PutOptions));

   memset(tmp1, 0, sizeof(tmp1));
   memset(tmp2, 0, sizeof(tmp2));
}


//---------------------------------------------------------------------------
// Destructor disconnects from Queue Manager
//---------------------------------------------------------------------------
tMQ::~tMQ() {

MQLONG Options=0;                      // No close options

//   if (MQSupliedBuffer == false && MQData != 0 ) {
//      delete MQData;
//   }

   if (QObjHandle) {
      if(strlen(MQQueueName) != 0) {
         MQCLOSE(MQManCon,                   // connection handle
                 &QObjHandle,                // object handle
                 Options,
                 &CompCode,                  // completion code
                 &ReasonCode);               // reason code

         if (ReasonCode != MQRC_NONE ) {
             printf("MQClose MGR(%s) Q(%s)  Failed CompCode(%ld) ReasonCode(%ld) %s \n", MQManagerName, MQQueueName, CompCode, ReasonCode, xMQ::ResoneCodeText(ReasonCode)); 
             }
      }
   }
}

//---------------------------------------------------------------------------
// Opens the named Queue
//---------------------------------------------------------------------------
int tMQ::QOpen(char *aQueueName, MQLONG Options) {

MQOD     MQODTemplate = {MQOD_DEFAULT};

//   if (Buffer && BufferLen) {
//      MQData = Buffer;
//      MQDataLen = BufferLen;
//      MQSupliedBuffer = true;
//   }
//   else {
//      MQSupliedBuffer = false;
//      MQData = new char[MQDataLen];
//   }

   strncpy(MQQueueName, aQueueName, sizeof(MQQueueName) - 1);

   memcpy((void *)&QObjDescriptor,(void *)&MQODTemplate,sizeof(QObjDescriptor));
   strncpy(QObjDescriptor.ObjectName, MQQueueName, sizeof(QObjDescriptor.ObjectName) - 1);

   MQOPEN(MQManCon,              // connection handle
          &QObjDescriptor,       // object descriptor for queue
          Options,               // open options
          &QObjHandle,           // object handle
          &CompCode,             // completion code
          &ReasonCode);          // Reason code
   if ( ReasonCode != MQRC_NONE || CompCode == MQCC_FAILED ) {
      throw XMQ("MQ Open");
   }

   return ERROR_OK;
}

//---------------------------------------------------------------------------
// Closes the named Queue
//---------------------------------------------------------------------------
void tMQ::QClose() {

MQLONG Options=0;                      // No close options

   if (QObjHandle == 0) {
      return;
   }

   MQCLOSE(MQManCon,                   // connection handle
           &QObjHandle,                // object handle
           Options,
           &CompCode,                  // completion code
           &ReasonCode);               // reason code

   if (ReasonCode != MQRC_NONE) {
      throw XMQ("MQ Close");
   }
}


//---------------------------------------------------------------------------
// Reads from the open Queue
//---------------------------------------------------------------------------
long tMQ::Read(pchar Buffer, long Bufferlen, MQLONG ReadWaitInterval, MQLONG Options)
{
MQMD    MsgDscrptrTemplate = {MQMD_DEFAULT};    // Message Descriptor
MQGMO   MsgOptionsTemplate = {MQGMO_DEFAULT};     // get message options

   memcpy((void *)&MsgDscrptr,(void *)&MsgDscrptrTemplate,sizeof(MsgDscrptr));
   memcpy((void *)&GetOptions,(void *)&MsgOptionsTemplate,sizeof(GetOptions));

   memcpy(MsgDscrptr.MsgId, MQMI_NONE, sizeof(MsgDscrptr.MsgId));
   memcpy(MsgDscrptr.CorrelId, MQCI_NONE, sizeof(MsgDscrptr.CorrelId));


//   if (MQData) {
//      memset(MQData,0,MQDataLen);
//   }
   GetOptions.Options |= MQGMO_ACCEPT_TRUNCATED_MSG;
   GetOptions.Options |= MQGMO_CONVERT;
   GetOptions.Options |= Options; // BROWSE_FIRST BROWSE_NEXT
   GetOptions.WaitInterval = ReadWaitInterval;

   MQGET(MQManCon,               // connection handle
         QObjHandle,             // object handle
         &MsgDscrptr,            // message descriptor
         &GetOptions,            // get message options
         Bufferlen,              // buffer length
         Buffer,                 // message buffer
         &MQMessageLen,          // message length
         &CompCode,              // completion code
         &ReasonCode);           // reason code
   if (ReasonCode != MQRC_NONE) {
      throw XMQ("MQ Get");
   }
//   MQData[MQMessageLen] = '\0';  // Add null terminator
   if (MQMessageLen < Bufferlen) {
      Buffer[MQMessageLen] = '\0';  // Add null terminator
   }
   return MQMessageLen;
}

void tMQ::DefMd(void) {
   MQMD MsgDscrptrTemplate = {MQMD_DEFAULT};       // Message Descriptor
   MsgDscrptrTemplate.Version = MQMD_VERSION_2;
   memcpy((void *)&MsgDscrptr,(void *)&MsgDscrptrTemplate,sizeof(MsgDscrptr));
}

//---------------------------------------------------------------------------
// Writes to open Queue
//---------------------------------------------------------------------------
int tMQ::Write(void *SrcData, MQLONG SrcDataLen, MQLONG Options)
{
//MQMD    MsgDscrptrTemplate = {MQMD_DEFAULT};       // Message Descriptor
MQPMO   MsgOptionsTemplate = {MQPMO_DEFAULT};   /* put message options           */

   MsgOptionsTemplate.Version = MQGMO_CURRENT_VERSION;
   MsgOptionsTemplate.Version = MQGMO_VERSION_2;
   memcpy((void *)&PutOptions,(void *)&MsgOptionsTemplate,sizeof(PutOptions));
   memcpy(MsgDscrptr.Format, MQFMT_STRING, sizeof(MsgDscrptr.Format));  // Char string format
   PutOptions.Options = Options;

   MQPUT(MQManCon,               // connection handle
         QObjHandle,             // object handle
         &MsgDscrptr,            // message descriptor
         &PutOptions,            // default options (datagram)
         SrcDataLen,             // message buffer
         SrcData,                // buffer length
         &CompCode,              // completion code
         &ReasonCode);           // reason code
   if (ReasonCode != MQRC_NONE) {
      throw XMQ("MQ Put");
   }
   return ERROR_OK;
}


int tMQ::Reply(MQBYTE24 CorrelId, MQBYTE24 MsgId, MQLONG Feedback) {

//MQMD    MsgDscrptrTemplate = {MQMD_DEFAULT};       // Message Descriptor
MQPMO   MsgOptionsTemplate = {MQPMO_DEFAULT};   /* put message options           */


//   memcpy((void *)&MsgDscrptr,(void *)&MsgDscrptrTemplate,sizeof(MsgDscrptr));
   memcpy((void *)&PutOptions,(void *)&MsgOptionsTemplate,sizeof(PutOptions));

   memcpy(MsgDscrptr.Format,MQFMT_STRING,(size_t)MQ_FORMAT_LENGTH);  // Char string format
   MsgDscrptr.MsgType = MQMT_REPORT;

   memset(MsgDscrptr.ReplyToQ   , 0, sizeof(MsgDscrptr.ReplyToQ   ));
   memset(MsgDscrptr.ReplyToQMgr, 0, sizeof(MsgDscrptr.ReplyToQMgr));

   MsgDscrptr.Feedback = Feedback;

   memcpy(MsgDscrptr.CorrelId, CorrelId, sizeof(MsgDscrptr.CorrelId));
   memcpy(MsgDscrptr.MsgId   , MsgId   , sizeof(MsgDscrptr.MsgId   ));

   LocalTimeToOracle(MsgDscrptr.PutDate, 15);
//   MsgDscrptr.PutTime

   MQPUT(MQManCon,               // connection handle
         QObjHandle,             // object handle
         &MsgDscrptr,            // message descriptor
         &PutOptions,            // default options (datagram)
         0,                      // message buffer
         0,                      // buffer length
         &CompCode,              // completion code
         &ReasonCode);           // reason code
   if (ReasonCode != MQRC_NONE) {
      throw XMQ("MQ Put Reply");
   }
   return ERROR_OK;
}


//---------------------------------------------------------------------------
// Rollback the current unit of work
//---------------------------------------------------------------------------
int tMQ::Rollback() {

   MQBACK(MQManCon,               // connection handle
          &CompCode,              // completion code
          &ReasonCode);           // reason code

   if (ReasonCode != MQRC_NONE) {
      throw XMQ("MQ Rollback");
   }
   return ERROR_OK;
}


//---------------------------------------------------------------------------
// Commit the current unit of work
//---------------------------------------------------------------------------
int tMQ::Commit() {

   MQCMIT(MQManCon,              // connection handle
          &CompCode,              // completion code
          &ReasonCode);           // reason code
   if (ReasonCode != MQRC_NONE) {
      throw XMQ("MQ Commit");
   }
   return ERROR_OK;
}

//----------------------------------------------------------------------------
// MQReader
//----------------------------------------------------------------------------

tMQReader::tMQReader(int bufferSizeKB)
{
   m_pMQMgr       = NULL;
   m_pMQ          = NULL;
   m_firstTime    = true;

   m_Buffer.sizeto(bufferSizeKB * 1024);
}

tMQReader::~tMQReader()
{
   if (m_pMQ != NULL)
      delete m_pMQ;
   if (m_pMQMgr != NULL)
      delete m_pMQMgr;
}


void tMQReader::Connect(pchar manager, pchar queue)
{
   SLOG->lprintf(eLogInfo, "tMQReader::Connect() - Connecting to MQ Manager [%s] Queue[%s]..", manager, queue);

   m_pMQMgr = new tMQSAMGR(manager);
   m_pMQ = new tMQ(*m_pMQMgr);
   m_pMQ->QOpen(queue, MQ_DEFAULT_BROWSE_OPTIONS | MQ_DEFAULT_READ_OPTIONS);

   SLOG->lprintf(eLogInfo, "tMQReader::Connect() - Connection done.");
}


long tMQReader::BrowseNextMessage(pchar &msg, int readTimeOut)
{
   SLOG->lprintf(eLogDebug, "tMQReader::BrowseNextMessage()");

   long len = 0;

   try
   {
      while (1)
      {
         len = 0;
         memset(m_Buffer, 0, sizeof(m_Buffer));

         try
         {
            if (m_firstTime)
            {
               m_firstTime = false;
               //Read will throw an exception if it times out
               SLOG->lprintf(eLogDebug, "Calling MQ Read with MQGMO_BROWSE_FIRST...");
               len = m_pMQ->Read(m_Buffer.data, (long)m_Buffer.bytelen, readTimeOut, MQGMO_WAIT | MQGMO_BROWSE_FIRST | MQGMO_LOCK);
               SLOG->lprintf(eLogDebug, "Returned from MQ Read:[%ld]", len);
            }
            else
            {
               //Read will throw an exception if it times out
               SLOG->lprintf(eLogDebug, "Calling MQ Read with MQGMO_BROWSE_NEXT...");
               len = m_pMQ->Read(m_Buffer.data, (long)m_Buffer.bytelen, readTimeOut, MQGMO_WAIT | MQGMO_BROWSE_NEXT | MQGMO_LOCK);
               SLOG->lprintf(eLogDebug, "Returned from MQ Read:[%d]", len);
            }
         }
         catch (xMQ e)
         {
            //If we get a MQRC_FORMAT_ERROR, it means that the MQ could not convert
            //the MQ message to string format. This could occur if the message was actually
            //for/from the mainframe, so we ignore the message and continue.
            if (e.ReasonCode == MQRC_FORMAT_ERROR && e.CompCode == MQCC_WARNING)
            {
               //Check that it is our message...
               string correlationID = string((char*)m_pMQ->MsgDscrptr.CorrelId);
               string tmp = "MQ: Unknown Message Format on Queue for correlationID ";
               tmp += correlationID;
               tmp += ". Ignoring message...";
               SLOG->Log(tmp.c_str(), eLogWarning);
               continue;
            }
            //If we get a Truncated message error, it means the message on the queue is larger than
            //we can read, so log the error and continue
            else if (e.ReasonCode == MQRC_TRUNCATED_MSG_ACCEPTED)
            {
               char tmp[512];
               sprintf(tmp, "Truncated Message Accepted! Message size = %d, but buffer is only %d. Ignoring message... (CompCode = %d)", len, m_Buffer.bytelen, e.CompCode);
               SLOG->Log(tmp, eLogError);
               continue;
            }
            else
               throw;
         }

         memcpy(m_CorrelationID, m_pMQ->MsgDscrptr.CorrelId, sizeof(MQBYTE24));
         memcpy(m_MessageID, m_pMQ->MsgDscrptr.MsgId, sizeof(MQBYTE24));

         if (len)
         {
            //Hack to be able to use the .used member
            SLOG->lprintf(eLogDebug, "Message received successfully with length [%ld]", len);
            m_Buffer.used = len;
            m_MessageLength = len;

            msg = m_Buffer.data;

            return m_MessageLength;
         }
         else
         {
            //No processing is required - so just remove the message from the queue
            SLOG->lprintf(eLogDebug, "Zero Length message received - removing from queue");

            SLOG->lprintf(eLogDebug, "Calling MQ Read with MQGMO_MSG_UNDER_CURSOR...");
            m_pMQ->Read(m_Buffer.data, (long)m_Buffer.bytelen, readTimeOut, MQGMO_MSG_UNDER_CURSOR);
            SLOG->lprintf(eLogDebug, "Returned from MQ Read:[%d]", len);
         }

         //This section deals with MQ responses, like COD, COA etc.
         switch (m_pMQ->MsgDscrptr.MsgType)
         {
         case MQMT_REPORT:
         case MQMT_REPLY:
            if (m_pMQ->MsgDscrptr.Feedback == MQFB_COD)
            {
               // This is a confirmation of delivery

               continue;
            }
            break;
         case MQMT_DATAGRAM:
            //case MQMT_SYSTEM_FIRST :
         case MQMT_REQUEST:
         case MQMT_SYSTEM_LAST:
         case MQMT_APPL_FIRST:
         case MQMT_APPL_LAST:
         default:
            SLOG->lprintf(eLogError, "Expecting Report MsgType and received(%d)", m_pMQ->MsgDscrptr.MsgType);
            break;
         }
      }
   }
   catch (xMQ &x)
   {
      if (x.ReasonCode == MQRC_NO_MSG_AVAILABLE)
      {
         SLOG->lprintf(eLogDebug, "Timeout reached and no message on queue..");
         return NO_MSG_AVAILABLE;
      }
      SLOG->Log(x);
      throw;
   }

}

void tMQReader::RemoveMessage(int readTimeOut)
{
   SLOG->lprintf(eLogDebug, "tMQReader::RemoveMessage()");

   long len = 0;

   // Now that the response has been handled, we can remove the message from the MQ
   SLOG->lprintf(eLogDebug, "Calling MQ Read with MQGMO_MSG_UNDER_CURSOR...");
   len = m_pMQ->Read(m_Buffer.data, (long)m_Buffer.bytelen, readTimeOut, MQGMO_MSG_UNDER_CURSOR);
   SLOG->lprintf(eLogDebug, "Returned from MQ Read:[%d]", len);

   SLOG->lprintf(eLogDebug, "Message removed.");
}


//----------------------------------------------------------------------------
// MQWriter
//----------------------------------------------------------------------------

tMQWriter::tMQWriter(string applicationID)
{
   m_pMQMgr        = NULL;
   m_pMQ           = NULL;
   m_ApplicationID = applicationID;
   m_replyManager  = "";
   m_replyQueue    = "";
}

tMQWriter::~tMQWriter()
{
   if (m_pMQ != NULL)
      delete m_pMQ;
   if (m_pMQMgr != NULL)
      delete m_pMQMgr;
}

void tMQWriter::Connect(pchar manager, pchar queue, pchar replyManager, pchar replyQueue)
{
   if (replyManager == NULL)
   {
      SLOG->lprintf(eLogInfo, "tMQWriter::Connect() - Connecting to MQ Manager [%s] Queue[%s]", manager, queue);
   }
   else
   {
      SLOG->lprintf(eLogInfo, "tMQWriter::Connect() - Connecting to MQ Manager [%s] Queue[%s] ReplyManager[%s] ReplyQueue[%s]", manager, queue, replyManager, replyQueue);
   }

   m_pMQMgr = new tMQSAMGR(manager);
   m_pMQ = new tMQ(*m_pMQMgr);
   m_pMQ->QOpen(queue, MQ_DEFAULT_WRITE_OPTIONS);

   if (replyManager != NULL)
      m_replyManager = replyManager;

   if (replyQueue != NULL)
      m_replyQueue   = replyQueue;

   SLOG->lprintf(eLogInfo, "tMQWriter::Connect() - Connection done.");
}

void tMQWriter::WriteMessage(pchar message, long messageLength, MQBYTE24 correlationID, bool useCOD)
{
   SLOG->lprintf(eLogDebug, "tMQWriter::WriteMessage() - Writing message with length [%ld]", messageLength);

   try
   {
      m_pMQ->DefMd();
      memcpy(m_pMQ->MsgDscrptr.Format, MQFMT_STRING, (size_t)MQ_FORMAT_LENGTH);
      strcpy(m_pMQ->MsgDscrptr.PutApplName, m_ApplicationID.c_str());

      memcpy(m_pMQ->MsgDscrptr.CorrelId, MQCI_NONE, sizeof(m_pMQ->MsgDscrptr.CorrelId));
      if (correlationID[0] != 0)
      {
         memcpy(m_pMQ->MsgDscrptr.CorrelId, correlationID, sizeof(m_pMQ->MsgDscrptr.CorrelId));
         SLOG->lprintf(eLogInfo, "Correlation ID set to: %s", (char*)m_pMQ->MsgDscrptr.CorrelId);
      }

      memcpy(m_pMQ->MsgDscrptr.MsgId, MQCI_NONE, sizeof(m_pMQ->MsgDscrptr.MsgId));
      strncpy(m_pMQ->MsgDscrptr.ReplyToQ, MQCI_NONE, sizeof(m_pMQ->MsgDscrptr.ReplyToQ));
      strncpy(m_pMQ->MsgDscrptr.ReplyToQMgr, MQCI_NONE, sizeof(m_pMQ->MsgDscrptr.ReplyToQMgr));

      if (!m_replyManager.empty())
      {
         strncpy(m_pMQ->MsgDscrptr.ReplyToQ, m_replyQueue.c_str(), sizeof(m_pMQ->MsgDscrptr.ReplyToQ) - 1);
         strncpy(m_pMQ->MsgDscrptr.ReplyToQMgr, m_replyManager.c_str(), sizeof(m_pMQ->MsgDscrptr.ReplyToQMgr) - 1);
      }

      m_pMQ->MsgDscrptr.Report = MQRO_NONE;
      m_pMQ->MsgDscrptr.Report |= MQRO_PASS_CORREL_ID;
      m_pMQ->MsgDscrptr.Report |= MQRO_PASS_MSG_ID;
      m_pMQ->MsgDscrptr.Report |= MQRO_PAN;
      m_pMQ->MsgDscrptr.Report |= MQRO_NAN;

      if (useCOD)
      {
         m_pMQ->MsgDscrptr.Report |= MQRO_COD;
      }

      //WriteTrace("Befor Write", Message, MQ.MsgDscrptr);

      m_pMQ->Write((void *)message, (ulong)messageLength, MQPMO_NEW_MSG_ID);
      m_pMQ->Commit();

      SLOG->lprintf(eLogDebug, "tMQWriter::WriteMessage() - Message written to queue.");
   }
   catch (rxMQ x) {
      throw x;
   }
   catch (rxCept x) {
      throw x;
   }
}

//--------------------------------------------------------------------------------------------------

xMQ::xMQ(pchar aFname, int aLine,
                       pchar WhatFailed, MQLONG aCompCode, MQLONG aReasonCode,
                       pchar aMsg1, pchar aMsg2)
: xCept(aFname, aLine, "xMQ", 0)
{

   ReasonCode = aReasonCode;
   CompCode = aCompCode;

   osErr << WhatFailed << " CompCode(" << CompCode << ") ReasonCode(" << ReasonCode << ")" << endl;

   osErr << "1) " << ResoneCodeText(ReasonCode) << endl;

   if (aMsg1) {
      osErr << "2) " << aMsg1 << endl;
   }
   if (aMsg2) {
      osErr << "3) " << aMsg2 << endl;
   }
   osErr << ends;
}

static pchar MQResonCodeText[] = {
 "None"                        // 0L
,"Alias Base Q Type Error"     // 2001L
,"Already Connected"           // 2002L
,"Backed Out"                  // 2003L
,"Buffer Error"                // 2004L
,"Buffer Length Error"         // 2005L
,"Char Attr Length Error"      // 2006L
,"Char Attrs Error"            // 2007L
,"Char Attrs Too Short"        // 2008L
,"Connection Broken"           // 2009L
,"Data Length Error"           // 2010L
,"Dynamic Q Name Error"        // 2011L
,"Environment Error"           // 2012L
,"Expiry Error"                // 2013L
,"Feedback Error"              // 2014L
,"Not Assigned"                // 2015L
,"Get Inhibited"               // 2016L
,"Handle Not Available"        // 2017L
,"Hconn Error"                 // 2018L
,"Hobj Error"                  // 2019L
,"Inhibit Value Error"         // 2020L
,"Int Attr Count Error"        // 2021L
,"Int Attr Count Too Small"    // 2022L
,"Int Attrs Array Error"       // 2023L
,"Syncpoint Limit Reached"     // 2024L
,"Max Conns Limit Reached"     // 2025L
,"Md Error"                    // 2026L
,"Missing Reply To Q"          // 2027L
,"Not Assigned"                // 2028L
,"Msg Type Error"              // 2029L
,"Msg Too Big For Q"           // 2030L
,"Msg Too Big For Q Mgr"       // 2031L
,"Not Assigned"                // 2032L
,"No Msg Available"            // 2033L
,"No Msg Under Cursor"         // 2034L
,"Not Authorized"              // 2035L
,"Not Open For Browse"         // 2036L
,"Not Open For Input"          // 2037L
,"Not Open For Inquire"        // 2038L
,"Not Open For Output"         // 2039L
,"Not Open For Set"            // 2040L
,"Object Changed"              // 2041L
,"Object In Use"               // 2042L
,"Object Type Error"           // 2043L
,"Od Error"                    // 2044L
,"Option Not Valid For Type"   // 2045L
,"Options Error"               // 2046L
,"Persistence Error"           // 2047L
,"Persistent Not Allowed"      // 2048L
,"Priority Exceeds Maximum"    // 2049L
,"Priority Error"              // 2050L
,"Put Inhibited"               // 2051L
,"Q Deleted"                   // 2052L
,"Q Full"                      // 2053L
,"Not Assigned"                // 2054L
,"Q Not Empty"                 // 2055L
,"Q Space Not Available"       // 2056L
,"Q Type Error"                // 2057L
,"Q Mgr Name Error"            // 2058L
,"Q Mgr Not Available"         // 2059L
,"Not Assigned"                // 2060L
,"Report Options Error"        // 2061L
,"Second Mark Not Allowed"     // 2062L
,"Security Error"              // 2063L
,"Not Assigned"                // 2064L
,"Selector Count Error"        // 2065L
,"Selector Limit Exceeded"     // 2066L
,"Selector Error"              // 2067L
,"Selector Not For Type"       // 2068L
,"Signal Outstanding"          // 2069L
,"Signal Request Accepted"     // 2070L
,"Storage Not Available"       // 2071L
,"Syncpoint Not Available"     // 2072L
,"Not Assigned"                // 2073L
,"Not Assigned"                // 2074L
,"Trigger Control Error"       // 2075L
,"Trigger Depth Error"         // 2076L
,"Trigger Msg Priority Err"    // 2077L
,"Trigger Type Error"          // 2078L
,"Truncated Msg Accepted"      // 2079L
,"Truncated Msg Failed"        // 2080L
,"Not Assigned"                // 2081L
,"Unknown Alias Base Q"        // 2082L
,"Not Assigned"                // 2083L
,"Not Assigned"                // 2084L
,"Unknown Object Name"         // 2085L
,"Unknown Object Q Mgr"        // 2086L
,"Unknown Remote Q Mgr"        // 2087L
,"Not Assigned"                // 2088L
,"Not Assigned"                // 2089L
,"Wait Interval Error"         // 2090L
,"Xmit Q Type Error"           // 2091L
,"Xmit Q Usage Error"          // 2092L
,"Not Open For Pass All"       // 2093L
,"Not Open For Pass Ident"     // 2094L
,"Not Open For Set All"        // 2095L
,"Not Open For Set Ident"      // 2096L
,"Context Handle Error"        // 2097L
,"Context Not Available"       // 2098L
,"Signal1 Error"               // 2099L
,"Object Already Exists"       // 2100L
,"Object Damaged"              // 2101L
,"Resource Problem"            // 2102L
,"Another Q Mgr Connected"     // 2103L
,"Unknown Report Option"       // 2104L
,"Storage Class Error"         // 2105L
,"Cod_not Valid For Xcf Q"     // 2106L
,"Xwait Canceled"              // 2107L
,"Xwait Error"                 // 2108L
,"Suppressed By Exit"          // 2109L
,"Format Error"                // 2110L
,"Source Ccsid Error"          // 2111L
,"Source Integer Enc Error"    // 2112L
,"Source Decimal Enc Error"    // 2113L
,"Source Float Enc Error"      // 2114L
,"Target Ccsid Error"          // 2115L
,"Target Integer Enc Error"    // 2116L
,"Target Decimal Enc Error"    // 2117L
,"Target Float Enc Error"      // 2118L
,"Not Converted"               // 2119L
,"Converted Msg Too Big"       // 2120L
,"No External Participants "   // 2121L
,"Participant Not Available"   // 2122L
,"Outcome Mixed"               // 2123L
,"Outcome Pending"             // 2124L
,"Bridge Started"              // 2125L
,"Bridge Stopped"              // 2126L
,"Adapter Storage Shortage"    // 2127L
,"Uow In Progress"             // 2128L
,"Adapter Conn Load Error"     // 2129L
,"Adapter Serv Load Error"     // 2130L
,"Adapter Defs Error"          // 2131L
,"Adapter Defs Load Error"     // 2132L
,"Adapter_conv_load_error"     // 2133L
,"Bo_error"                    // 2134L
,"Dh_error"                    // 2135L
,"Multiple_reasons"            // 2136L
,"Open_failed"                 // 2137L
,"Adapter Disc Load Error"     // 2138L
,"Not Assigned"                // 2139L
,"Cics Wait Failed"            // 2140L
,"Header Error"                // 2141L
,"Source Length Error"         // 2142L
,"Source Length Error"         // 2143L
,"Target Length Error"         // 2144L
,"Source Buffer Error"         // 2145L
,"Target Buffer Error"         // 2146L
,"Not Assigned"                // 2147L
,"Iih Error"                   // 2148L
,"Pcf Error"                   // 2149L
,"Dbcs Error"                  // 2150L
,"Truncated"                   // 2151L
,"Object Name Error      "     // 2152L
,"Object Q Mgr Name Error"     // 2153L
,"Recs Present Error     "     // 2154L
,"Object Records Error   "     // 2155L
,"Response Records Error "     // 2156L
,"Asid Mismatch"               // 2157L
,"Pmo Record Flags Error"      // 2158L
,"Put Msg Records Error "      // 2159L
,"Conn Id In Use"              // 2160L
,"Q Mgr Quiescing"             // 2161L
,"Q Mgr Stopping"              // 2162L
,"Duplicate Recov Coord"       // 2163L
,"Not Assigned"                // 2164L
,"Not Assigned"                // 2165L
,"Not Assigned"                // 2166L
,"Not Assigned"                // 2167L
,"Not Assigned"                // 2168L
,"Not Assigned"                // 2169L
,"Not Assigned"                // 2170L
,"Not Assigned"                // 2171L
,"Not Assigned"                // 2172L
,"Pmo Error"                   // 2173L
,"Not Assigned"                // 2174L
,"Not Assigned"                // 2175L
,"Not Assigned"                // 2176L
,"Not Assigned"                // 2177L
,"Not Assigned"                // 2178L
,"Not Assigned"                // 2179L
,"Not Assigned"                // 2180L
,"Not Assigned"                // 2181L
,"Api Exit Not Found"          // 2182L
,"Api Exit Load Error"         // 2183L
,"Remote Q Name Error"         // 2184L
,"Not Assigned"                // 2185L
,"Gmo Error"                   // 2186L
,"Not Assigned"                // 2187L
,"Not Assigned"                // 2188L
,"Not Assigned"                // 2189L
,"Not Assigned"                // 2190L
,"Tmc_error"                   // 2191L
,"Pageset Full"                // 2192L
,"Pageset Error"               // 2193L
,"Name Not Valid For Type"     // 2194L
,"Unexpected Error"            // 2195L
,"Unknown Xmit Q"              // 2196L
,"Unknown Def Xmit Q"          // 2197L
,"Def Xmit Q Type Error"       // 2198L
,"Def Xmit Q Usage Error"      // 2199L
,"Not Assigned"                // 2200L
,"Name In Use"                 // 2201L
,"Connection Quiescing"        // 2202L
,"Connection Stopping"         // 2203L
,"Adapter Not Available"       // 2204L
,"Not Assigned"                // 2205L
,"Msg Id Error"                // 2206L
,"Correl Id Error"             // 2207L
,"File System Error"           // 2208L
,"No Msg Locked"               // 2209L
,"Not Assigned"                // 2210L
,"Not Assigned"                // 2211L
,"Not Assigned"                // 2212L
,"Not Assigned"                // 2213L
,"Not Assigned"                // 2214L
,"Not Assigned"                // 2215L
,"File Not Audited"            // 2216L
,"Connection Not Authorized"   // 2217L
,"Msg Too Big For Channel"     // 2218L
,"Call In Progress"            // 2219L
,"Rmh_error"                   // 2220L
,"Not Assigned"                // 2221L
,"Q Mgr Active"                // 2222L
,"Q Mgr Not Active"            // 2223L
,"Q Depth High"                // 2224L
,"Q Depth Low"                 // 2225L
,"Q Service Interval High"     // 2226L
,"Q Service Interval Ok"       // 2227L
,"Not Assigned"                // 2228L
,"Not Assigned"                // 2229L
,"Not Assigned"                // 2230L
,"Not Assigned"                // 2231L
,"Not Assigned"                // 2232L
,"Channel Auto Def Ok"         // 2233L
,"Channel Auto Def Error"      // 2234L
,"Cfh Error"                   // 2235L
,"Cfil Error"                  // 2236L
,"Cfin Error"                  // 2237L
,"Cfsl Error"                  // 2238L
,"Cfst Error"                  // 2239L
,"Notassigned"                 // 2240L
,"Incomplete Group"            // 2241L
,"Incomplete Msg"              // 2242L
,"Inconsistent Ccsids"         // 2243L
,"Inconsistent Encodings"      // 2244L
,"Inconsistent Uow"            // 2245L
,"Invalid Msg Under Cursor"    // 2246L
,"Match Options Error"         // 2247L
,"Mde Error"                   // 2248L
,"Msg Flags Error"             // 2249L
,"Msg Seq Number Error"        // 2250L
,"Offset Error"                // 2251L
,"Original Length Error"       // 2252L
,"Segment Length Zero"         // 2253L
,"Notassigned"                 // 2254L
,"Uow Not Available"           // 2255L
,"Wrong Gmo Version"           // 2256L
,"Wrong Md Version"            // 2257L
,"Group Id Error"              // 2258L
,"Inconsistent Browse"         // 2259L
,"Xqh Error"                   // 2260L
,"Src Env Error"               // 2261L
,"Src Name Error"              // 2262L
,"Dest Env Error"              // 2263L
,"Dest Name Error"             // 2264L
,"Tm Error"                    // 2265L
,"Not Assigned"                // 2266L
,"Not Assigned"                // 2267L
,"Not Assigned"                // 2268L
,"Not Assigned"                // 2269L
,"Not Assigned"                // 2270L
,"Not Assigned"                // 2271L
,"Not Assigned"                // 2272L
,"Not Assigned"                // 2273L
,"Not Assigned"                // 2274L
,"Not Assigned"                // 2275L
,"Not Assigned"                // 2276L
,"Not Assigned"                // 2277L
,"Not Assigned"                // 2278L
,"Not Assigned"                // 2279L
,"Hconfig Error"               // 2280L
,"Function Error"              // 2281L
,"Channel Started"             // 2282L
,"Channel Stopped"             // 2283L
,"Channel Conv Error"          // 2284L
,"Service Not Available"       // 2285L
,"Initialization Failed"       // 2286L
,"Termination Failed"          // 2287L
,"Unknown Q Name"              // 2288L
,"Service Error"               // 2289L
,"Q Already Exists"            // 2290L
,"User Id Not Available"       // 2291L
,"Unknown Entity"              // 2292L
,"Unknown Auth Entity"         // 2293L
,"Unknown Ref Object"          // 2294L
,"Channel Activated"           // 2295L
,"Channel Not Activated"       // 2296L
};

pchar xMQ::ResoneCodeText (long aResoneCode) {

   if (aResoneCode < MQRC_ALIAS_BASE_Q_TYPE_ERROR || aResoneCode > MQRC_CHANNEL_NOT_ACTIVATED) {
      return MQResonCodeText[0];
   }
   return MQResonCodeText[aResoneCode - MQRC_ALIAS_BASE_Q_TYPE_ERROR + 1];
}
