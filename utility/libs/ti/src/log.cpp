#include "versions.h"
#define _VERSION_NO "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
COMPILED_VERSION(_VERSION_NO);

#include "machine.h"


#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <fcntl.h>

//#ifndef M_W32
#include <io.h>
//#endif

#if defined(M_AIX) || defined(M_GNU)
#include <sys/socket.h>
#include <netinet/in.h>
#endif

#ifdef M_OS2
#define INCL_DOS
#define INCL_DOSERRORS
#define OS2
#endif
#include "osfuncs.h"

#include "log.h"
#include "cliserv.h"
#include "xstring.h"

#define NLOGS     20
#define LOGMAX  4096

#ifdef M_OS2
# define NL "\r\n"
#else
# define NL "\n"
#endif

typedef struct tLogFile
{
  char       *LogName;
  int         File;
  pClientThis Client;
  ushort      Usage;
} tLogFile, *pLogFile;

static tLogFile Logs[NLOGS];

int OpenLogFile(char *LogName, char *AppName)
{
  int i;

  for (i=0; i<NLOGS; i++)
    if (Logs[i].LogName && strieq(LogName, Logs[i].LogName))
      {
      Logs[i].Usage++;
      return i+1;
      }
  for (i=0; i<NLOGS; i++)
    if (!Logs[i].LogName)
       break;
  if (i>=NLOGS)
    return 0;
  Logs[i].Usage=1;
  Logs[i].LogName = strdup(LogName);
  if (*LogName == ':')
  {
    Logs[i].Client = ClientInit(LogName, LOGMAX, 0);
    Logs[i].File = 0;
    if (!Logs[i].Client)
      return 0;
  }
  else
  {
    Logs[i].File = open(LogName, O_WRONLY|O_CREAT|O_APPEND, 0664);
    if (!Logs[i].File)
       return 0;
  }
  RecordError(i+1, AppName, 0, "Opened");
  return i+1;
}

static void vRecordError(int Log, char *AppName, short ErrorCode,
                         char* Desc, va_list marker, int isData)
{
  static char *Jotter;
  static tBuffer Buffer = {LOGMAX};
  pLogFile LogP;
  int Count = 0;

  if (!Jotter)
  {
    Jotter = (char*) malloc(LOGMAX);
    if (!Jotter)
      return;
    Buffer.p = Jotter;
  }
  *Jotter = 0;

  LogP = &Logs[Log-1];

  if (!isData)
  {
    time_t secsNow;
    struct tm timeNow;

    time(&secsNow);
    timeNow = *localtime(&secsNow);
    Count += sprintf(Jotter, "%02d/%02d/%02d %02d:%02d:%02d ",
      timeNow.tm_year, timeNow.tm_mon + 1, timeNow.tm_mday,
      timeNow.tm_hour, timeNow.tm_min, timeNow.tm_sec);
    Count += sprintf(Jotter+Count, "%10.10s %3d ", AppName, ErrorCode);
  }
  Count += vsprintf(Jotter+Count, Desc, marker);
  if (!isData)
    {
    strcpy(Jotter+Count, NL);
    Count += strlen(NL);
    }
  if (LogP->File)
    write(LogP->File, Jotter, Count);
  else
    {
    Buffer.BytesToRead = Buffer.BytesToWrite = Count;
    if (LogP->Client)
      ClientCall(LogP->Client, &Buffer, &Buffer);
    }
}

void RecordError(int Log, char* AppName, short ErrorCode, char* Desc, ...)
{
  va_list marker;

  if (Log < 1 || Log > NLOGS)
    return;
  va_start(marker, Desc);
  vRecordError(Log, AppName, ErrorCode, Desc, marker, 0);
  va_end(marker);
}

void RecordData(int Log, char *AppName, char* Desc, ...)
{
  va_list marker;

  if (Log < 1 || Log > NLOGS)
    return;
  va_start(marker, Desc);
  vRecordError(Log, AppName, 0, Desc, marker, 1);
  va_end(marker);
}

void CloseLogFile(int Log, char *AppName)
{
  pLogFile LogP;

  if (Log < 1 || Log > NLOGS)
    return;
  LogP = &Logs[Log-1];
  if (LogP->Usage)
     LogP->Usage--;
  if (!LogP->Usage)
  {
    RecordError(Log, AppName, 0, "Closed");
    free(LogP->LogName);
    LogP->LogName = 0;
    if (LogP->File)
    {
      close(LogP->File);
      LogP->File = 0;
    }
  }
}

int OpenJournalFile(char *Name)
{
   int File = open(Name, O_WRONLY|O_CREAT|O_APPEND, 0664);
   if (File >= 0)
      JournalNote(File, "OPEN %s\n", Name);
   return File;
}

void JournalNote(int Journal, char* Desc, ... )
{
  static char *Jotter;
  va_list marker;
  ushort Count;
  time_t secsNow;
  struct tm timeNow;


  if (!Jotter)
  {
    Jotter = (char *) malloc(LOGMAX);
    if (!Jotter)
      return;
  }
  *Jotter = 0;
  va_start(marker, Desc);
  time(&secsNow);
  timeNow = *localtime(&secsNow);
  Count += sprintf(Jotter, "\n%02d/%02d/%02d %02d:%02d:%02d ",
     timeNow.tm_year, timeNow.tm_mon + 1, timeNow.tm_mday,
     timeNow.tm_hour, timeNow.tm_min, timeNow.tm_sec);
  Count += vsprintf(Jotter+Count, Desc, marker);
  write(Journal, Jotter, Count);
  va_end(marker);
}

void Journal(int Journal, char *Ref, void* Data, ushort DataSize)
{
  JournalNote(Journal, "%s\n", Ref);
  write(Journal, Data, DataSize);
}

void CloseJournalFile(char *Name, int Journal)
{
  JournalNote(Journal, "CLOSE %s\n", Name);
  close(Journal);
}
