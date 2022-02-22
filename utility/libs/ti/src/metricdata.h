#ifndef __METRICDATA_H__
#define __METRICDATA_H__
#include "machine.h"

struct TMetricData
{
  char   HostIP[15];
  char   ClientIP[15];
  int32  ServerPort;
  int32  ProcessId;
  int32  RequestId;
  int32  ReturnCode;
  int32  BytesSent;
  int32  BytesReceived;
  int32  ActualBytesSent;
  int32  ActualBytesReceived;
  int32  TimeSlot;     // Use time(0) function from time.h
  double ServerProcessDuration;
  double ClientTotalDuration;
  char   Comments[512];
  TMetricData(char  *HostIP,
              char  *ClientIP,
              int32  ServerPort,
              int32  ProcessId,
              int32  RequestId,
              int32  ReturnCode,
              int32  BytesSent,
              int32  BytesReceived,
              int32  ActualBytesSent,
              int32  ActualBytesReceived,
              int32  TimeSlot,
              double ServerProcessDuration,
              double ClientTotalDuration,
              char  *Comments
             )
   {
     strncpy(this->HostIP, HostIP, sizeof(this->HostIP)-1);
     strncpy(this->ClientIP, ClientIP, sizeof(this->ClientIP)-1);
     this->ServerPort = ServerPort;
     this->ProcessId = ProcessId;
     this->RequestId = RequestId;
     this->ReturnCode = ReturnCode;
     this->BytesSent = BytesSent;
     this->BytesReceived = BytesReceived;
     this->ActualBytesSent = ActualBytesSent;
     this->ActualBytesReceived = ActualBytesReceived;
     this->TimeSlot = TimeSlot;
     this->ServerProcessDuration = ServerProcessDuration;
     this->ClientTotalDuration = ClientTotalDuration;
     strncpy(this->Comments, Comments, sizeof(this->Comments)-1);
   }
   TMetricData()
   {
     memset(this, 0, sizeof(*this));
   }
};

#if defined(M_AIX)
#include "aqueue.h"
typedef TQueue<TMetricData> TMetricQueue;
#elif defined(M_GNU)
#include "aqueue.h"
typedef TQueue<TMetricData> TMetricQueue;
#else
#include "xstring.h"
#include "blackboard.h"
typedef TBlackBoard<TMetricData> TMetricQueue;
#endif

#endif

