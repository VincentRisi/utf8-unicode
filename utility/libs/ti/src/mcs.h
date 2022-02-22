//#ifdef M_AIX
#ifndef _MCS_H_
#define _MCS_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_MCS_H_);

#include "ti.h"

extern "C" {
#define tl_sigset get_rid_of_diag1
#include "tpm_intf.h"         // Ipc routines, prototypes and structures
#include "zdcidsnd.h"         // MCS header and buffer structures
#include "zipiint1.h"         // Interface routines
#include "ndcirdef.h"         // for dst_corp values
}

CLASSDEF(MCS);
class tMCS
{
  tString Program;
  tString MCSName;
  tString Orig_Term;
  tString Orig_Corp;
  ushort  NetNo;
  ushort  LSN;
  char   *HeaderType;
  char   *MsgType;
  ZDSEND_CMESSAGE buf[2];
public:
  tMCS(pchar aProgram, pchar aMCSName, ushort aNetNo, ushort aLSN);
  tMCS(pchar aProgram, pchar aMCSName, ushort aNetNo, ushort aLSN,
      pchar MsgType, pchar aOrig_Term = "", pchar aOrig_Corp = "");
  tMCS(pchar aProgram, pchar aMCSName, pchar aNetNo, pchar aLSN);
  void Init();
  void Read(pchar aData, short aDataLen, int aFlag=IPC_RHEAD, int aSleep=0);
  void Write(pchar aData, short aDataLen, int aLSN, int aNetNo, int aFlag=IPC_WTAIL, int aSleep=0 );
  void Write(pchar aData, short aDataLen, int aFlag=IPC_WTAIL, int aSleep=0);
  void Echo(pchar aData, short aDataLen, int aFlag=IPC_WTAIL, int aSleep=0);
  ushort ReadLength();
  int  QDepth() {return buf[0].ipchdr.ipc_qdepth;}
  void * SourceHdrPtr() {return (void *) &buf[0].hdr;}
  long  SourceHdrSize() {return (long) sizeof(buf[0].hdr);}
  void SetTerm(pchar aCorp, pchar aTerm);
  void ChangeProgramName(pchar NewProgName) {Program=NewProgName;}
};

//---------------------------------------------------------
// aFlag  - IPC_NDELAY (no-delay: default is wait)
//          IPC_RHEAD|IPC_RTAIL|IPC_WHEAD|IPC_WTAIL|IPC_POS
//          IPC_CHKSUM (default is no-check)
//          IPC_HANDLE (use handle. default is no)
// aSleep - 0 = forever. Ignore if IPC_NDELAY is set
//---------------------------------------------------------

XCLASSDEF(MCS);
class xMCS : public xCept
{
  int One, Two;
public:
  enum eError { errMCSInit = 1
              , errMCSRead
              , errMCSWrite
              , errMCSSize
              };
  xMCS(pchar aFname, int aLine, eError aError, int aOne=0, int aTwo=0);
  xMCS(const xMCS& aX);
  int Primary()   { return One; }
  int Secondary() { return Two; }
};


#define XMCS(err) xMCS(__FILE__, __LINE__, xMCS::err)
#define XMCS1(err, a) xMCS(__FILE__, __LINE__, xMCS::err, (a))
#define XMCS2(err, a, b) xMCS(__FILE__, __LINE__, xMCS::err, (a), (b))


#endif

//#endif