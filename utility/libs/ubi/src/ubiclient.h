#ifndef _UBICLIENT_H_
#define _UBICLIENT_H_

#include "ubirpcclient.h"

class UbiClient : public UbiRPCClient
{
public:
  UbiClient();
  UbiClient(char *host, char *service, int32 timeout=150000);
  ~UbiClient();
  void Call(int32 ReqID, void *TxBuffer, int32 TxSize);
private:
  char mHostName[256];
  char mSocketName[256];
  void GetInetAddr(char *host, char *inetAddr);
};

#endif



