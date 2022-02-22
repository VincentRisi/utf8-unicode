#ifndef __MQ_H__
#define __MQ_H__ "$Revision: 63 $ $Date: 2007-06-05 10:11:02 +0200 (Tue, 05 Jun 2007) $"
/****************************************************************************/
/*                                                                          */
/*  System : Putty                                                          */
/*                                                                          */
/*  File   : mymq.h                                                         */
/*                                                                          */
/*  Target : mqout.exe                                                      */
/*                                                                          */
/*  Description :                                                           */
/*                                                                          */
/*  Author : S. Shemesh                                                     */
/*                                                                          */
/*  Date Created   : 06-Jun-2001                                            */
/*                                                                          */
/*  $Date: 2007-06-05 10:11:02 +0200 (Tue, 05 Jun 2007) $                                            */
/*                                                                          */
/*  $Revision: 63 $                                                        */
/*                                                                          */
/*  $Author: simon $                                                        */
/*                                                                          */
/*  Change History :
*   $Log: mymq.h,v $
*   Revision 1.5  2002/04/05 13:40:42  simon
*   add Heder file version macros
*
*   Revision 1.4  2002/04/05 09:49:14  simon
*   add Heder file version macros
*
*   Revision 1.3  2002/03/25 11:23:05  simon
*   no message
*
*/
/****************************************************************************/
HEADER_VERSION(__MQ_H__);

#include "cmqc.h"               /* includes for MQI  */
#include <cmqxc.h>              /* For MQCD definition           */

#include "ti.h"
#include "tbuffer.h"
#include "logfile.h"

#include <string>

using namespace std;

#define MQ_DEFAULT_QMNGR         ""
#define MQ_DEFAULT_READ_OPTIONS  (MQOO_INPUT_AS_Q_DEF+MQOO_FAIL_IF_QUIESCING) // open queue for input but not if MQM stopping
#define MQ_DEFAULT_WRITE_OPTIONS (MQOO_OUTPUT+MQOO_FAIL_IF_QUIESCING) // open queue for output but not if MQM stopping
#define MQ_DEFAULT_GMO_OPTIONS   (MQGMO_WAIT + MQGMO_CONVERT)  // convert if necessary, wait for message
#define MQ_DEFAULT_BROWSE_OPTIONS (MQOO_BROWSE + MQOO_FAIL_IF_QUIESCING) // but not if MQM stopping

#define NO_MSG_AVAILABLE         -1

CLASSDEF(MQSAMGR);
class tMQSAMGR
{
   public:
      // Functions
   	tMQSAMGR(char *iMQManagerName);            // Constructor (Connects to Queue Manager)
      tMQSAMGR(char *iMQServerName, int iMQServerPort, char *iMQChannelName, char *iMQManagerName, MQLONG iMaxMsgLength);            // Constructor (Connects to Queue Manager)
      ~tMQSAMGR();                               // Destructor (Disconnect Queue Manager)

      void QManagerConnect(char *iMQManagerName);    // Connect local
      void QManagerConnect(char *iMQServerName, int iMQServerPort, char *iMQChannelName, char *iMQManagerName, MQLONG iMaxMsgLength);    // Connect via channel
      int QManagerDisconnect(void);

      // Variables

      MQLONG   MQDataLen;                          // Length Of data buffer
      MQLONG   CompCode;                           // Completion code
      MQLONG   ReasonCode;                         // Reason code

      char  MQServerName[1024];
      int   MQServerPort;
      char  MQChannelName[MQ_CHANNEL_NAME_LENGTH];
      char  MQManagerName[MQ_Q_MGR_NAME_LENGTH];   // Current Queue Manager Name

      MQHCONN  MQManCon;                           // Queue manager connection handle
};


CLASSDEF(MQ);
class tMQ
{
   public:
      // Functions
      tMQ(tMQSAMGR & aMqsaMgr);             // Constructor Pass Queue Manager)
      ~tMQ();                               // Destructor (Disconnect Queue Manager)
      void DefMd(void);

      int QOpen(char *MQQueueName,MQLONG Options); // Opens a Queue
      void QClose(void);                           // Closes a Queue
      int Commit();                                // Commit unit of work (UOM)
      int Rollback();                              // Rolls back the UOM

      /*
      // Reads from the Queue
      */
      long Read(pchar Buffer, long Bufferlen,MQLONG iReadWaitInterval=0, MQLONG Options=MQGMO_NONE);

      /*
      // Writes to the Queue
      */
      int Write(void *SrcData, MQLONG SrcDataLen, MQLONG Options = MQPMO_NONE);

      /*
      // Reply to the Message
      */
      int Reply(MQBYTE24 CorrelId, MQBYTE24 MsgId, MQLONG Feedback);

      // Variables
//      char     *MQData;                            // Data from/for Queue
//      bool     MQSupliedBuffer;                    // External Buffer Dont Free On Distruct
      MQLONG   MQDataLen;                          // Length Of data buffer
      MQLONG   MQMessageLen;                       // Length of Data in buffer
      MQLONG   CompCode;                           // Completion code
      MQLONG   ReasonCode;                         // Reason code
      MQLONG   ReadWaitInterval;                   // Interval to wait before quiting read (*1000 if in seconds)

      MQMD     MsgDscrptr;                         // Message Descriptor
      char     tmp1[1024];
//      char     tmp2[1024];
//      char     tmp3[1024];
//      char     tmp4[1024];

   private:
      char  MQManagerName[MQ_Q_MGR_NAME_LENGTH];   // Current Queue Manager Name
      char  MQQueueName[MQ_Q_NAME_LENGTH];         // Open Queue Name

      tMQSAMGR & MqsaMgr;

      MQHCONN  MQManCon;                           // Queue manager connection handle
      MQHOBJ   QObjHandle;                         // Object handle
      MQOD     QObjDescriptor;                     // Object Descriptor
      MQGMO    GetOptions;                         // Message Options
      MQGMO    PutOptions;                         // Message Options
      char     tmp2[1024];
};

CLASSDEF(MQReader);
class tMQReader
{
public:
   // Functions
   tMQReader(int bufferSizeKB);
   ~tMQReader();

   void Connect(pchar manager, pchar queue);
   long BrowseNextMessage(pchar &msg, int readTimeOut = 3000);
   void RemoveMessage(int readTimeOut = 3000);

private:
   tMQ                  *m_pMQ;
   tMQSAMGR             *m_pMQMgr;
   TBChar                m_Buffer;
   long                  m_MessageLength;
   MQBYTE24              m_CorrelationID;
   MQBYTE24              m_MessageID;
   bool                  m_firstTime;

};

CLASSDEF(MQWriter);
class tMQWriter
{
public:
   // Functions
   tMQWriter(string applicationID);
   ~tMQWriter();

   void Connect(pchar manager, pchar queue, pchar replyManager = NULL, pchar replyQueue = NULL);
   void WriteMessage(pchar message, long messageLength, MQBYTE24 correlationID, bool useCOD = false);

private:
   tMQ                  *m_pMQ;
   tMQSAMGR             *m_pMQMgr;
   TBChar                m_Buffer;
   MQBYTE24              m_CorrelationID;
   MQBYTE24              m_MessageID;
   string                m_ApplicationID;
   string                m_replyManager;
   string                m_replyQueue;
};

// own exception class - used to throw exceptions matching return error codes.
XCLASSDEF(MQ);
class xMQ : public xCept
{
public :
   xMQ(pchar aFname, int aLine, pchar WhatFailed,
       MQLONG aCompCode, MQLONG aReasonCode,
       pchar aMsg1 = 0, pchar aMsg2 = 0);
	xMQ(const xMQ& aX)
	:  xCept(aX)
     ,CompCode  (CompCode   = aX.CompCode  )
     ,ReasonCode(ReasonCode = aX.ReasonCode)
   {
   }

   static pchar ResoneCodeText (long aResoneCode);

   MQLONG   CompCode;                           // Completion code
   MQLONG   ReasonCode;                         // Reason code
private:
   long oError;
};

#define XMQ(WhatFailed) \
   xMQ(__FILE__, __LINE__, WhatFailed, CompCode, ReasonCode)
#define XMQ1(WhatFailed, msg1) \
   xMQ(__FILE__, __LINE__, WhatFailed, CompCode, ReasonCode, msg1)
#define XMQ2(WhatFailed, msg1, msg2) \
   xMQ(__FILE__, __LINE__, WhatFailed, CompCode, ReasonCode, msg1, msg2)


#endif // #define __MQ_H__
