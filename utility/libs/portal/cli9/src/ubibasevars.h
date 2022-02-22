#ifndef __UBI_BASE_VARS_H_
#define __UBI_BASE_VARS_H_

#include "machine.h"
#include "logfile.h"
#include "cliapi.h"

class UbiBaseVars
{
public:
  tLogFile* logFile;
  TJConnector* connect;
  UbiBaseVars() {logFile = 0; connect = 0;}
  virtual ~UbiBaseVars() {}
};

// extern "C" 
// #ifdef OS_DECLSPEC
// OS_DECLSPEC 
// #endif
// UbiBaseVars* UbiBaseLogon(const char *connectString, const char* logFileName);
// extern "C" 
// #ifdef OS_DECLSPEC
// OS_DECLSPEC 
// #endif
// void UbiBaseLogoff(UbiBaseVars* baseVar);
// extern "C" 
// #ifdef OS_DECLSPEC
// OS_DECLSPEC 
// #endif
// void UbiBaseCommit();
// #ifdef OS_DECLSPEC
// OS_DECLSPEC 
// #endif
// void UbiBaseRollback();

#endif
