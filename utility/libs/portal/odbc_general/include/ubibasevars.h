#ifndef __UBI_BASE_VARS_H_
#define __UBI_BASE_VARS_H_

#include "machine.h"
#include "logfile.h"
#include "odbcapi.h"

class UbiBaseVars
{
public:
  tLogFile* logFile;
  TJConnector* connect;
  UbiBaseVars() {logFile = 0; connect = 0;}
  virtual ~UbiBaseVars() {}
};

#endif
