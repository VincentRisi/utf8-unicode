#ifndef _LOG_H_
#define _LOG_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_LOG_H_);

#include "machine.h"

int  OpenLogFile(char *LogName, char *AppName);
void RecordError(int Log, char* AppName, short ErrorCode, char* Desc, ...);
void RecordData(int Log, char *AppName, char* Desc, ...);
void CloseLogFile(int Log, char *AppName);

int  OpenJournalFile(char *Name);
void JournalNote(int Journal, char* Desc, ... );
void Journal(int Journal, char *Ref, void* Data, ushort DataSize);
void CloseJournalFile(char *Name, int Journal);

#endif

