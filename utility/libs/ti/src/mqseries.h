#ifndef __MQSERIES_H__
#define __MQSERIES_H__ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(__MQSERIES_H__);

#include "cmqc.h"               /* includes for MQI  */


#include "ti.h"

#define MQ_DEFAULT_QMNGR         ""
#define MQ_DEFAULT_READ_OPTIONS  (MQOO_INPUT_AS_Q_DEF+MQOO_FAIL_IF_QUIESCING) // open queue for input but not if MQM stopping
#define MQ_DEFAULT_WRITE_OPTIONS (MQOO_OUTPUT+MQOO_FAIL_IF_QUIESCING) // open queue for output but not if MQM stopping
#define MQ_DEFAULT_GMO_OPTIONS   (MQGMO_WAIT + MQGMO_CONVERT + MQGMO_SYNCPOINT)  // convert if necessary, wait for message, synchpoint control
#define MQ_DEFAULT_PMO_OPTIONS   (MQPMO_SYNCPOINT)  // Synchpoint control
#define MQ_DEFAULT_BROWSE_OPTIONS (MQOO_BROWSE + MQOO_FAIL_IF_QUIESCING) // but not if MQM stopping

typedef struct MQ_Queue_struct
{
   struct MQ_Queue_struct *Next;
   char     *Buffer;                            // Data from/for Queue
   MQLONG   BufferLen;                          // Length Of data buffer
   MQLONG   DataLen;
   ushort   UsingAlternateBuffer;               // TRUE if user specified buffer to use

   MQOD     QObjDescriptor;                     // Object Descriptor
   char     Name[MQ_Q_NAME_LENGTH];             // Queue Name
   MQHOBJ   QObjHandle;                         // Object handle
   MQLONG   Options;

} tMQQueue;





CLASSDEF(MQConnect);
class tMQConnect
{
   public:
      // Functions
      tMQConnect(){CurQueue = 0;QueueHead = 0;Buffer = 0;DataLen = 0;MQManCon = 0;}
      tMQConnect(char *iMQManagerName);          // Constructor (Connects to Queue Manager)
      ~tMQConnect();                               // Destructor (Disconnect Queue Manager)
      int QManagerConnect(char *iMQManagerName);   // Connects to Queue Manager
      int QManagerDisconnect();                    // Disconnects from Queue Manager


      tMQQueue * QOpen(char   *MQQueueName,        // Opens a Queue
                     MQLONG   Options,
                     char     *AlternateBuffer=0,
                     ulong    AlternateBufferLen=0);
      tMQQueue * QClose(tMQQueue *QPtr=0,          // Closes a Queue
                      char    *MQQueueName=0);

      tMQQueue * Read(MQLONG ReadWaitInterval,     // See below
                      MQLONG Options,
                      char *MQQueueName);
      tMQQueue * Read(MQLONG iReadWaitInterval=0, // Reads from the Queue
                      MQLONG         Options=MQ_DEFAULT_GMO_OPTIONS,
                      tMQQueue       *QPtr=0,
                      char           *MQQueueName=0);

      tMQQueue * Write(MQLONG Options);
      tMQQueue * Write(tMQQueue *QPtr,
                       MQLONG Options=MQ_DEFAULT_PMO_OPTIONS);
      tMQQueue * Write(char *MQQueueName,
                       MQLONG Options=MQ_DEFAULT_PMO_OPTIONS);
      tMQQueue * Write(char *SrcData,              // See below
                       ulong SrcDataLen,
                       char *MQQueueName,
                       MQLONG Options=MQ_DEFAULT_PMO_OPTIONS);
      tMQQueue * Write(char         *SrcData,      // Writes to the Queue
                       ulong        SrcDataLen,
                       tMQQueue      *QPtr=0,
                       MQLONG        Options=MQ_DEFAULT_PMO_OPTIONS,
                       char          *MQQueueName=0);

      int Commit();                                // Commit unit of work (UOM)
      int Rollback();                              // Rolls back the UOM

      tMQQueue * GetQueueByName(char *MQQueueName); // Returns Queue ll pointer for named queue

      // Variables
      MQLONG   MQMaxDataLen;                       // Maximum length for queues as reported from the Queue Manager
      MQLONG   CompCode;                           // Completion code
      MQLONG   ReasonCode;                         // Reason code
      MQLONG   ReadWaitInterval;                   // Interval to wait before quiting read (*1000 if in seconds)
      MQBYTE24  MsgId;                             // Message identifier
      MQBYTE24  CorrelId;                          // Correlation identifier

      // Queue information (linked list with 1 mandatory element
      tMQQueue *QueueHead,*CurQueue;
      char     *Buffer;
      long      DataLen;

   private:
      // Q Manager
      char     MQManagerName[MQ_Q_MGR_NAME_LENGTH];// Current Queue Manager Name
      MQHCONN  MQManCon;                           // Queue manager connection handle

      // Message specific
      MQMD     MsgDscrptr;                         // Message Descriptor
      MQGMO    GetMsgOptions;                         // Message Options
      MQPMO    PutMsgOptions;

};




// own exception class - used to throw exceptions matching return error codes.
XCLASSDEF(MQConnect);
class xMQConnect : public xCept
{
   public :
      enum eError {
         errMQConnect=0,
         errMQDisconnect,
         errMQOpen,
         errMQClose,
         errMQRead,
         errMQWrite,
         errMQRollback,
         errMQCommit,
         errMQMemAlloc,
         errMQQName,
         errMQQueue,
         errMQUnknown
      };

      char  *eErrorMessage;
      eError ErrorNumber;

      MQLONG   CompCode;                           // Completion code
      MQLONG   ReasonCode;                         // Reason code

      xMQConnect(pchar aFname, int aLine, eError eError,MQLONG aCompCode,MQLONG aReasonCode);
};

#define XrMQConnect(aErrorNo) xMQConnect(__FILE__, __LINE__, xMQConnect::aErrorNo,CompCode,ReasonCode)
#define XMQConnect(aErrorNo) xMQConnect(__FILE__, __LINE__, xMQConnect::aErrorNo,0,0)

#endif // #define __MQSERIES_H__
