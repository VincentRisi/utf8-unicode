#ifndef __METRICS__H__
#define __METRICS__H__
#include "machine.h"

#include <math.h>

class TMetrics {
public:
   TMetrics();
   virtual void Record(char  *HostIP,
                       char  *ClientIP,
                       long   ServerPort,
                       long   ProcessId,
                       long   TimeSlot,
                       long   ReqId,
                       long   ReturnCode,
                       long   BytesSent,
                       long   BytesReceived,
                       long   ActualBytesSent,
                       long   ActualBytesReceived,
                       double ServerProcessDuration,
                       double ServerTotalDuration,
                       double ClientTotalDuration);
   virtual void Sent(char *data, long length);
   virtual void Received(char *data, long length);
};


/*
*   Child Process Beep Class
*/

#if defined(M_AIX) || defined(M_GNU)
#include <sys/time.h>
#include "logfile.h"
#include "tllist.h"
#include "squeue.h"

#ifndef MyDisplayVerbose
#define MyDisplayVerbose(x) \
SLOG->lprintf(eLogError, \
"Caught exception at %s(%d) \n"\
"Name %s In %s(%d) Error(%d) : %s" ,\
__FILE__, __LINE__,\
x.Name(), x.FileName(), x.Line(), x.Error(), x.ErrorStr());
#endif

#ifndef MyUndefinedDisplay
#define MyUndefinedDisplay() \
SLOG->lprintf(eLogError,"Caught Un-Defined Exception at %s(%d)", __FILE__, __LINE__)
#endif

double TimeOfCall();

struct tBeep {
   enum beep_t { BEEP_START, BEEP_END, BEEP_EXTRA };
   beep_t   type;
   pid_t    ProcessId;
   double   TimeOfCall;
   double   Duration;
   double   ReadTime;
   double   ServiceTime;
   long     BytesSent;
   long     BytesReceived;
   int      Requestid;
   char     wsid[16];

   tBeep (void) {
      memset(this, 0, sizeof(*this));
   }

   void print(tLogFile* monitor, eLevel Loglevel) {
      monitor->lprintf(Loglevel,
                    "Pid(%d) "
                    "Wsid(%s) "
                    "Req(%d) "
                    "Read(%0.6f)+"
                    "Process(%0.6f)+"
                    "Write(%0.6f)="
                    "Duration(%0.6f) "
                    "Sent(%ld) "
                    "Recv(%ld)"
                    ,this->ProcessId
                    ,this->wsid
                    ,this->Requestid
                    ,this->ReadTime
                    ,this->ServiceTime
                    ,this->Duration - (this->ServiceTime + this->ReadTime)
                    ,this->Duration
                    ,this->BytesSent
                    ,this->BytesReceived
                   );
   }
};
typedef tBeep *pBeep;

typedef TSQueue<tBeep> tBeepQueue;

typedef tLList<tBeep> tProcessBeepLinks;


class tRoundRobin {
public:
   static const int size = 1000;
   tRoundRobin() {
      clear();
   }

   void print(void) {
      printf("Top(%d) Len(%d)\n", top, len);
   }

   void printbeep(void) {
      for ( int i=0; i<this->len;i++ ) {
         printf("beep(%d)\n", (*this)[i]->ProcessId);
      }
   }

   void append(tBeep &Beep) {

      this->Beeps[top] = Beep;
      if ( len < this->size ) {
         len++;
      }
      top = ++top % this->size;
   }

   void zaptail(void) {
      if ( len > 0) {
         len--;
         if ( len <= 0 ) {
            Reset();
         }
      }
   }

   void clear(void) {
      this->top    = 0;
      this->len    = 0;
      memset(this->Beeps, 0, sizeof(this->Beeps));
   }

   void Reset(void) {
      this->top    = 0;
      this->len    = 0;
   }

   tBeep *getbeep(int i) {

      if (i > this->size || i < 0) {
         printf("invalid Getbeep(%d)\n", i);
         return 0;
      }

      int index = (top - len + i + this->size) % this->size;
      if (index > this->size || index < 0) {
         printf("top(%d) len(%d) index(%d)", top, len, index);
         return 0;
      }
      return &Beeps[index];
   }

   tBeep * operator[](int i) {
      if (i > this->size || i < 0) {
         return 0;
      }
      int index = (top - len + i + this->size) % this->size;
      if (index > this->size || index < 0) {
         printf("top(%d) len(%d) index(%d)", top, len, index);
         return 0;
      }
      return(&Beeps[index]);
   }

   int GetLength(void) {
      return len;
   }

protected:
   tBeep Beeps[size];
   int top;
   int len;
};


/*
* Parent Class beep collector and stats
*/

class tProcess {
public:
   tProcess(pid_t pid) :
   pid (pid) {
      count = 0;
      avg = sqrs = stddev = min = max = RunningAverage = StartIntervalTime = 0;
      time(&timeStamp);
   };
   ~tProcess() {
   }
   pid_t pid;
   time_t timeStamp;
   tRoundRobin rrBeeps;
   /*
   * server statistics
   */
   int    count;
   double avg;
   double sqrs;
   double stddev;
   double min;
   double max;
   double StartIntervalTime;
   int    Intervalcount;
   double IntervalTimeSecond;
   double zDuration;
   double RunningPercentage;
   double RunningAverage;
   double ServiceDenayTime;
   void RunningStats(double plus) 
   {
      double total = avg*count+plus;
      count++;
      avg = total / count;
      if ( plus != 0 && (min == 0 || plus < min) )
         min = plus;
      if ( plus != 0 && (max == 0 || plus > max) )
         max = plus;
      sqrs += (plus*plus);
      double var = avg*avg -(2*avg*total - sqrs) / count;
      stddev = var <= 0 ? 0 : sqrt(var);
   }

   double ComputeRunningPercentage(tLogFile* monitor, int IntervalTime) 
   {
      int i, len;
      zDuration = 0;
      RunningPercentage = 0;
      Intervalcount = 0;
      /*
      * if no activities
      */
      len = this->rrBeeps.GetLength();
      if ( len == 0 ) {
         IntervalTimeSecond = 0;
         return 0;
      }
      double TimeNow = TimeOfCall();
      IntervalTimeSecond = IntervalTime * 60;
      double TimeBack = TimeNow - IntervalTimeSecond;
      if ( TimeBack < 0 ) {
         TimeBack = 0;
      }
      StartIntervalTime = TimeNow;
      tBeep *Beep;
      /*
      *  remove all beep that older then out interval time
      */
      for ( ; rrBeeps.GetLength(); ) {

         Beep = this->rrBeeps.getbeep(0);
         if (Beep == 0) {
            break;
         }

         if ( (Beep->TimeOfCall + Beep->Duration) < TimeBack ) {
            this->rrBeeps.zaptail();
            continue;
         }
         break;
      }

      bool first = false;
      double starttime;
      double Duration;

      for ( i = 0; i < len; i++) {
         Beep = this->rrBeeps.getbeep(i);

         if ( (Beep->TimeOfCall + Beep->Duration) < TimeBack ) {
            continue;
         }

         Duration = Beep->Duration;
         if ( first == false ) {
            if ( Beep->TimeOfCall < TimeBack ) {
               Duration -= TimeBack - Beep->TimeOfCall;
            }
            else {
               IntervalTimeSecond =  TimeNow - Beep->TimeOfCall;
               if (IntervalTimeSecond < 0) {
                  printf("IntervalTimeSecond(%f), Beep->TimeOfCall(%f)\n", IntervalTimeSecond, Beep->TimeOfCall);
               }
            }
            first = true;
         }

         if (Duration < 0 || Duration > 300) {
            Beep->print(monitor, eLogError);
         }

         zDuration += Duration;
         Intervalcount++;
      }

      if ( zDuration > 0 ) {
         RunningPercentage = (zDuration / IntervalTimeSecond) * 100;
      }
      return RunningPercentage;
   }


//   double getAge(void) {
//      return difftime(time(NULL),timeStamp);
//   }
   int operator ==(tProcess &a) {
      return a.pid == pid;
   }
};
typedef tLList<tProcess> tProcessLinks;

double ComputeRunningAverage(tProcess *pProcess);

double ComputeAverage(tLogFile* monitor, tProcessLinks &Process, int IntervalTime);

bool RemoveProcesFromList(tProcessLinks &Process, pid_t pid);

void ProcessClear(tProcessLinks &Process);

void Beepsend(tBeep & ProcBeep, tBeepQueue *BeepQueue);


#endif

#endif
