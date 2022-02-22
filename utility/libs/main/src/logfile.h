//
// Class for logging messages to a specified file
//
#ifndef __LOGFILE_H_
#define __LOGFILE_H_

#include "ti.h"
#include <stdio.h>
#ifndef _WINDOWS
#include "xcept.h"
#endif
#if defined(M_AIX) || defined(M_GNU)
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>
#endif
//#include "tbuffer.h"

enum eLevel {eLogDebug, eLogInfo, eLogWarning, eLogError};

class xLogFileErr
: public xCept
{
public:
   xLogFileErr(const char *aFname, int aLine, int aError, const char *aMsg1=0, const char *aMsg2=0, const char *aMsg3=0);
   xLogFileErr(const xLogFileErr& aX)
   : xCept(aX)
   {}
};

#define XLogFile1(err, msg1) \
     xLogFileErr(__FILE__, __LINE__, err, msg1)
#define XLogFile2(err, msg1, msg2) \
     xLogFileErr(__FILE__, __LINE__, err, msg1, msg2)
#define XLogFile3(err, msg1, msg2, msg3) \
     xLogFileErr(__FILE__, __LINE__, err, msg1, msg2, msg3)

#define LOGFILE_BUFFER_LENGTH 32*1024

class tLogFile
{
public:
  static bool defRotate;
  static bool defDisplay;
  static int  defMaxFileSize;
  static tLogFile *__logFile__;
  static tLogFile *singleton(const char *FileName, eLevel DoLevel=eLogDebug, int aMaxFileSize = defMaxFileSize, bool aDisplay = defDisplay, bool aRotate = defRotate);
  static eLevel level(std::string logLevel)
  {
    if (logLevel.length() > 0)
    {
      if (logLevel[0] == 'E' || logLevel[0] == 'e') return eLogError;
      if (logLevel[0] == 'W' || logLevel[0] == 'w') return eLogWarning;
      if (logLevel[0] == 'I' || logLevel[0] == 'i') return eLogInfo;
    }
    return eLogDebug;
  }
  std::string CustomString;
  void SetCustomString(std::string aCustomString) {CustomString = aCustomString;}
  tLogFile();
  tLogFile(const char *FileName, eLevel DoLevel=eLogDebug, int aMaxFileSize = defMaxFileSize, bool aDisplay = defDisplay, bool aRotate = defRotate);
  ~tLogFile();
  void Switch(const char *FileName);
  void Switch(const char *aFileName, eLevel aDoLevel, int aMaxFileSize, bool aDisplay = defDisplay, bool aRotate = defRotate);
  void Log(const char *Msg, eLevel MsgLevel=eLogDebug);
  void Log(xCept &x, eLevel MsgLevel=eLogError);
  void lprintf(eLevel MsgLevel, const char *Fmt, ...);
  void Progress(const char *Fmt, ...);
  void SetMaxFileSize(int aMaxFileSize) { maxFileSize = aMaxFileSize; }
  void LimitSize(FILE *fLog);
  void LogLevel (const char *StrLogLevel);
  void SetLogLevel(eLevel level) {doLevel = level; }
  void SetHistoryExtension(const char *ext) { historyExtention = ext; }
  void SetDisplay(bool value) { Display = value; }
  std::string FileName;
  eLevel doLevel;
  bool Display;
  int  PrevDate;
protected:
  std::string historyExtention;
  FILE *fLog;
  int  maxFileSize;
  char buffer[LOGFILE_BUFFER_LENGTH+1];
  void Open(const char *fileName, const char *mode="a");
  void LogToFile(const char *msg, eLevel msgLevel);
  void GetDatedFilename(std::string &filename);
  void SwitchOnDate();
#if defined(M_AIX) || defined(M_GNU)
  int  sem;
#endif
  bool Rotate;
};

class SingletonLogFile
{
public :
  static void Initialize(const char *fileName = 0, eLevel doLevel = eLogDebug, bool display = tLogFile::defDisplay) 
  {
    const char* usefilename = fileName;
    if (usefilename == 0) 
    {
      if (tLogFile::__logFile__ == 0) 
      {
        XLogFile1(0, "Can not initialize - No logfile previously constructed");
      }
      usefilename = tLogFile::__logFile__->FileName.c_str();
    }
    tLogFile::singleton(usefilename, doLevel, 0);
    tLogFile::__logFile__->SetDisplay(display);
  }
  static void Assign(tLogFile *logFile);
  static void Destroy(void);

  static void Initialize(const char *fileName, const char* doLevel, bool display = tLogFile::defDisplay) 
  {
    Initialize(fileName, eLogDebug, display);
    tLogFile::__logFile__->LogLevel(doLevel);
  }

};

#define SLOG  tLogFile::__logFile__
#define SAVECUSTOMSTR(logfile, str) logfile->CustomString=str

#endif
