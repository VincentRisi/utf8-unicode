#include "logfile.h"
#include "metrics.h"
// -=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// THIS IS A NULL IMPLEMENTATION - if you want Metrics
// then implement a new version and include it before
// the library that this is in. viz. libidl2.lib
// -=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

TMetrics::TMetrics() {}

void TMetrics::Record(char  *HostIP,
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
                      double ClientTotalDuration) {
}

void TMetrics::Sent(char *data, long length) 
{
}

void TMetrics::Received(char *data, long length) 
{
}

#if defined(M_AIX) || defined(M_GNU)

double TimeOfCall() {

   static struct timeval stp;

   if ( stp.tv_sec == 0 ) // must be zero to start with by decree
      gettimeofday(&stp, 0);
   struct timeval tp;
   int rc=gettimeofday (&tp, 0);
   double a = tp.tv_sec-stp.tv_sec;
   long b = tp.tv_usec-stp.tv_usec;
   /*
   * The while could be an if. it's emulating a subtraction with carry
   */
   while ( b < 0 ) {
      a-=1; b+=1000000;
   }
   return a + b / 1000000.0;
}

double ComputeRunningAverage(tProcess *pProcess) {

   int i, len;
   double zDuration = 0;
   double zBetween  = 0;

   len = pProcess->rrBeeps.GetLength();
   if ( len < 2 ) {
      return 0;
   }

   for ( i = 1; i < len; i++ ) {
      zDuration += pProcess->rrBeeps[i]->Duration;
      zBetween  += pProcess->rrBeeps[i]->TimeOfCall - pProcess->rrBeeps[i-1]->TimeOfCall;
   }

   if ( i > 1 && zBetween > 0 ) {
      pProcess->RunningAverage = zDuration / zBetween * 100;
   }
   return pProcess->RunningAverage;
}

double ComputeAverage(tLogFile *monitor, tProcessLinks &Process, int IntervalTime) {

   int i, len;
   double zRunningPercentage = 0;
   tProcess *pProcess;
   int count = 0;
   int Intervalcount = 0;
   double IntervalTimeSecond = 0;
   double zDuration = 0;
   double Average = 0;
   eLevel Loglevel;
   len = Process.len();
   if (len == 1) return 0.0; // vince with the restarting child logic this becomes a possibility
   for ( i = 1; i < len; i++ ) {
      pProcess = Process.find(i);
      zRunningPercentage += pProcess->ComputeRunningPercentage(monitor, IntervalTime);
      Average = 0;
      if ( pProcess->Intervalcount > 0 ) {
         Average = pProcess->zDuration / pProcess->Intervalcount;
      }
      if ( pProcess->Intervalcount ) {
         Loglevel = eLogInfo;
      }
      else {
         Loglevel = eLogDebug;
      }
      monitor->lprintf(Loglevel,
                    "pid %9d "
                    "count %4d "
                    "No of %4d "
                    "Time %3.0f "
                    "Total %9.3f "
                    "Average %9.6f "
                    "Percent %5.2f"
                    ,pProcess->pid
                    ,pProcess->count
                    ,pProcess->Intervalcount
                    ,pProcess->IntervalTimeSecond
                    ,pProcess->zDuration
                    ,Average
                    ,pProcess->RunningPercentage
                   );
      count         += pProcess->count;
      Intervalcount += pProcess->Intervalcount;
      zDuration     += pProcess->zDuration;
   }
   double ProcAverage = zRunningPercentage / (len-1);
   if ( Intervalcount ) {
      Loglevel = eLogInfo;
   }
   else {
      Loglevel = eLogDebug;
   }
   monitor->lprintf(Loglevel,
                 "cnt %9d "
                 "count %4d "
                 "No of %4d "
                 "         "
                 "Total %9.3f "
                 "                  "
                 "Percent %5.2f"
                 //"\n"
                 ,len - 1
                 ,count
                 ,Intervalcount
                 ,zDuration
                 ,ProcAverage
                );
   return ProcAverage;
}

void ProcessClear(tProcessLinks &Process) 
{
   tProcess *pProcess;
   int len = Process.len();
   for ( int i = 1; i < len; i++ ) {
      pProcess = Process.find(i);
   }
   Process.clear();
}

/*
*  remove the process stats from the list
*/
bool RemoveProcesFromList(tProcessLinks &Process, pid_t pid) 
{
   tProcess* process;
   int len = Process.len();
   bool found = false;
   for (int i = 1; i < len; i++) 
   {
      process = Process.find(i);
      if (process->pid == pid)
      {
         Process.zap(i);
         found = true;
         break;
      }
   }
   return found;
}


#if defined(M_AIX) || defined(M_GNU)
void Beepsend(tBeep & ProcBeep, tBeepQueue *BeepQueue) 
{
   bool stop = true;
   bool retry = false;
   try 
   {
      if ( BeepQueue && BeepQueue->isOpend ) {
         while ( stop == true ) {
            try {
               BeepQueue->insert(ProcBeep);
               stop = false;
               return;
            }
            catch ( xCept &x ) {
               MyDisplayVerbose(x);
               if ( retry ) {
                  return;
               }
               BeepQueue->reOpen();
               retry = true;
               continue;
            }
            catch ( ... ) {
               MyUndefinedDisplay();
               if ( retry ) {
                  return;
               }
               BeepQueue->reOpen();
               retry = true;
               continue;
            }
         }
      }
   }
   catch ( xCept &x ) {
      MyDisplayVerbose(x);
   }
   catch ( ... ) {
      MyUndefinedDisplay();
   }
}
#endif

#endif
