#include "ubirpcclient.h"

static char* UbiRPCErrs[] =
  { "OK No Error"
  , "No Host specified for Server"
  , "No Socket Port specified for Server"
  , "Host as specified is invalid"
  , "Invalid response from Server"
  , "No Data Read from Socket"
  , "Invalid Message Signature"
  , "Data Read not of expected Size"
  , "Data Written not of expected Size"
  , "No WSLogon NodeID"
  , "No WSLogon UserID"
  , "No WSLogon Password"
  , "IPAddr is unavailable"
  , "SECPATH Environment variable not set"
  , "KEYFILE Error"
  , "NODEKEY NOT SET"
  , "USER NOT SET"
  , "Semaphore not clear"
};

xUbiRPCErr::xUbiRPCErr(char* aFname, int32 aLine, eError aError, int32 aNo, char* aMsg)
: xCept(aFname, aLine, xUbiRPCErrName, aError)
{
  osErr << "Socket " << UbiRPCErrs[aError] << ":" << aNo << endl;
  if (aMsg)
    osErr << "1) " << aMsg << endl;
  osErr << ends;
}

