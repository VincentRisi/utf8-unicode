#include "machine.h"
#include <stdlib.h>
#if defined(M_W32)
#include <windows.h>
#include <winsock2.h>
#endif
#include <time.h>
#include "ubiclient.h"
#include "cbsocket.h"
#include "xstring.h"
#include "rdc.h"
#include "md5.h"
#include "metrics.h"
#include "swapbytes.h"

#if defined(M_W32)
// This ensures a WSACleanup
struct autoWSA
{
  WSADATA wsa;
  autoWSA()   {WSAStartup(DESIRED_WINSOCK_VERSION, &wsa);}
 ~autoWSA()   {WSACleanup();}
  int32 Errno() {return WSAGetLastError();}
};
#endif

// This ensures that an allocation get deallocated
struct autoChar
{
  unsigned char *buffer;
  autoChar()        {buffer = 0;}
  void alloc(int32 size) {buffer = new unsigned char[size];}
 ~autoChar()        {if (buffer) delete [] buffer;}
};

// This ensures that an open socket gets closed
struct autoSockOpen
{
  tCBSockClient &Sock;
  autoSockOpen(tCBSockClient &aSock)
  : Sock(aSock)     {Sock.Open();}
 ~autoSockOpen()    {Sock.Close();}
};

UbiClient::UbiClient(char *HostName, char *SocketName, int32 Timeout)
{
#if defined(M_W32)
  autoWSA wsa;
  struct hostent FAR *ph;
#else
  struct hostent *ph;
#endif
  memset(&mHeader, 0, sizeof(UbiRPCHeader));
  strcpy(mHeader.RPCVersion, "BEEBEEDEE");
  mTimeout = Timeout;
  if (HostName == 0)
    throw XUbiRPCErr(errUbiNoHost);
  if (SocketName == 0)
    throw XUbiRPCErr(errUbiNoService);
  strcpy(mHostName, HostName);
  strcpy(mSocketName, SocketName);
  GetInetAddr(mHostName, mHeader.HostId);
  ph = gethostbyname(mHostName);
  if (!ph)
#if defined(M_W32)
    throw XUbiRPCErr1(errUbiHostInvalid, wsa.Errno(), mHostName);
#else
  throw XUbiRPCErr1(errUbiHostInvalid, 1111, mHostName);
#endif
  int rc;
  char WSName[32];
  rc = gethostname(WSName, sizeof(WSName));
  if (rc != 0)
    throw XUbiRPCErr(errUbiNoHost);
  GetInetAddr(WSName, mHeader.WsId);
}

UbiClient::~UbiClient()
{
  if (mRxBuffer)
    free(mRxBuffer);
}

#if defined(M_W32)
#include <mmsystem.h>
double Now()
{
  int64 now = (int64)timeGetTime();
  return (double)now / 1000;
}
#endif

#if defined (M_AIX) || defined (M_GNU)
static struct timeval stp;

double Now()
{
  if (stp.tv_sec == 0) // must be zero to start with by decree
    gettimeofday(&stp, 0);
  struct timeval tp;
  int rc=gettimeofday (&tp, 0);
  time_t a = tp.tv_sec-stp.tv_sec;
  time_t b = tp.tv_usec-stp.tv_usec;
  while (b < 0) {a-=1; b+=1000000;}
  return (double)(a*1000000+b) / 1000000.0;
}
#endif


void UbiClient::Call(int32 ReqID, void *TxBuffer, int32 TxSize)
{
  int32 rc;
  int32 DataLen;
  TMetrics M;
  double starts = Now();
  time_t nowInSecs = time(0);
  struct tm *x = localtime(&nowInSecs);
  int32 TimeSlot = x->tm_hour*60*60 + x->tm_min*60 + x->tm_sec;
  mErrSize = 0;
  mErrBuffer = 0;
  mRxBuffer = 0;
  mRxSize = 0;
  mHeader.RpcReqID = ReqID;
  SwapBytes(mHeader.RpcReqID);
  mHeader.Msize = TxSize;
  SwapBytes(mHeader.Msize);
  tCBSockClient Sock(mHostName, mSocketName);
  Sock.SetTimeout(mTimeout);
  autoSockOpen openSock(Sock);
  int32 SizeSent, ActualSizeSent;
  size_t compressed = 0;
  size_t oneLen = sizeof(UbiRPCHeader)+4;
  ActualSizeSent = oneLen;
  autoChar out;
  if (TxSize && TxBuffer)
  {
     ActualSizeSent += (TxSize+4);
    if (TxSize > 1452 - sizeof(UbiRPCHeader))
    {
      out.alloc(TxSize);
      compressed = RdcCompress((puchar) TxBuffer, (size_t) TxSize, out.buffer);
    }
    if (compressed != 0)
      oneLen += compressed;
    else
      oneLen += TxSize;
    oneLen += 4;
  }
  autoChar theOne;
  theOne.alloc(oneLen);
  char *p = (char*)theOne.buffer;
  *(int32*)p = sizeof(UbiRPCHeader);
  SwapBytes(*(int32*)p);
  p += 4;
  *(UbiRPCHeader*)p = mHeader;
  M.Sent(p, sizeof(mHeader));
  p += sizeof(mHeader);
  if (compressed != 0)
  {
    *(int32*)p = compressed;
    SwapBytes(*(int32*)p);
    p += 4;
    memcpy(p, out.buffer, compressed);
    M.Sent(p, compressed);
  }
  else
  {
    *(int32*)p = TxSize;
    SwapBytes(*(int32*)p);
    p += 4;
    memcpy(p, TxBuffer, TxSize);
    M.Sent(p, TxSize);
  }
  Sock.Write((void*)theOne.buffer, (unsigned int)oneLen);
  SizeSent = oneLen;
  DataLen = Sock.ReadLength();
  if (DataLen != (int32)sizeof(UbiRPCHeader))
    throw XUbiRPCErr(errUbiInvResponse);
  UbiRPCHeader HostHeader;
  memset(&HostHeader, 0, sizeof(HostHeader));
  Sock.Read(&HostHeader, (unsigned int)DataLen);
  M.Received((char*)&HostHeader, DataLen);
  int32 SizeReceived = 4 + DataLen;
  int32 ActualSizeReceived = SizeReceived;
  mHeader.ReturnCode = ntohl(HostHeader.ReturnCode);
  mHeader.Msize = ntohl(HostHeader.Msize);
  if (mHeader.Msize > 0)
  {
    ActualSizeReceived += (mHeader.Msize+4);
    DataLen = Sock.ReadLength();
    if (DataLen <= 0)
      throw XUbiRPCErr(errUbiNoDataRead);
    mRxBuffer = new char [mHeader.Msize];
    try
    {
      autoChar in;
      in.alloc(DataLen);
      Sock.Read(in.buffer, (unsigned int)DataLen);
      M.Received((char*)in.buffer, DataLen);
      SizeReceived += 4 + DataLen;
      if (mHeader.Msize > DataLen)
        mRxSize = RdcDecompress(in.buffer, DataLen, (puchar)mRxBuffer);
      else
      {
        mRxSize = DataLen;
        memcpy(mRxBuffer, in.buffer, DataLen);
      }
    }
    catch (...)
    {
      delete [] mRxBuffer;
      throw;
    }
  }
  mHeader.Esize = ntohl(HostHeader.Esize);
  if (mHeader.Esize > 0)
  {
    ActualSizeReceived += (mHeader.Esize+4);
    DataLen = Sock.ReadLength();
    if (DataLen <= 0)
      throw XUbiRPCErr(errUbiDataReadError);
    mErrBuffer = new char [mHeader.Esize];
    try
    {
      autoChar in;
      in.alloc(DataLen);
      Sock.Read(in.buffer, (unsigned int)DataLen);
      M.Received((char*)in.buffer, DataLen);
      SizeReceived += 4 + DataLen;
      if (mHeader.Esize > DataLen)
        mErrSize = RdcDecompress(in.buffer, DataLen, (puchar)mErrBuffer);
      else
      {
        mErrSize = DataLen;
        memcpy(mErrBuffer, in.buffer, DataLen);
      }
    }
    catch (...)
    {
      delete [] mErrBuffer;
      throw;
    }
  }
  double CallDuration = Now() - starts;
  double ProcessDuration;
  double HostCallDuration;
  memcpy(&ProcessDuration, &HostHeader.PrevProcessDuration, 8);
  memcpy(&HostCallDuration, &HostHeader.PrevCallDuration, 8);
  SwapBytes(ProcessDuration); // swap_eight is destructive
  SwapBytes(HostCallDuration); // swap_eight is destructive
  M.Record(HostHeader.HostId,
           HostHeader.WsId,
           Sock.PortNo(),
           ntohl(HostHeader.PrevProcessId),
           TimeSlot,
           ReqID,                        // Request Id
           ntohl(HostHeader.ReturnCode),
           SizeSent,
           SizeReceived,
           ActualSizeSent,
           ActualSizeReceived,
           ProcessDuration,
           HostCallDuration,
           CallDuration);
  mHeader.PrevRpcReqID = htonl(ReqID);
  mHeader.PrevTimeSlot = htonl(TimeSlot);
  mHeader.PrevReturnCode = HostHeader.ReturnCode;
  mHeader.PrevBytesSent = htonl(SizeSent);
  mHeader.PrevBytesReceived = htonl(SizeReceived);
  mHeader.PrevActualBytesSent = htonl(ActualSizeSent);
  mHeader.PrevActualBytesReceived = htonl(ActualSizeReceived);
  mHeader.PrevProcessId = HostHeader.PrevProcessId;
  memcpy(&mHeader.PrevCallDuration, &CallDuration, 8);
  SwapBytes(mHeader.PrevCallDuration);
  memcpy(&mHeader.PrevProcessDuration, &HostHeader.PrevProcessDuration, 8);
}

void UbiClient::GetInetAddr(char *HostName, char *InetAddr)
{
#if defined(M_W32)
  struct hostent FAR *ph;
#else
  struct hostent *ph;
#endif
  strcpy(InetAddr, "xxx.xxx.xxx.xxx");
  ph = gethostbyname(HostName);
  if (!ph)
    throw XUbiRPCErr(errUbiIPAddrUnavailable);
  strcpy(InetAddr, (char *) inet_ntoa(*(struct in_addr *) (ph -> h_addr)));
}



