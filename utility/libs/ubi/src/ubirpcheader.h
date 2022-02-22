#ifndef __UBIRPC_HEADER_H_
#define __UBIRPC_HEADER_H_
#include "padgen.h"
#include "ubixcept.h"

/**
 * UbiRPCHeader
 *
 * All fields are aligned to 8
 *
 */

struct UbiRPCHeader
{
  char    RPCVersion[16];                                //   0 -  15
  char    HostId[16];                                    //  16 -  31
  char    WsId[16];                                      //  32 -  47
  char    UsrId[16];                                     //  48 -  63
  int32   RpcReqID;                IDL2_INT32_PAD(0);    //  64 -  67 -  71
  int32   ReturnCode;              IDL2_INT32_PAD(1);    //  72 -  75 -  79
  int32   Msize;                   IDL2_INT32_PAD(2);    //  80 -  83 -  87
  int32   Esize;                   IDL2_INT32_PAD(3);    //  88 -  91 -  95
  int32   PrevRpcReqID;            IDL2_INT32_PAD(4);    //  96 -  99 - 103
  int32   PrevTimeSlot;            IDL2_INT32_PAD(5);    // 104 - 107 - 111
  int32   PrevReturnCode;          IDL2_INT32_PAD(6);    // 112 - 115 - 119
  int32   PrevBytesSent;           IDL2_INT32_PAD(7);    // 120 - 123 - 127
  int32   PrevBytesReceived;       IDL2_INT32_PAD(8);    // 128 - 131 - 135
  int32   PrevActualBytesReceived; IDL2_INT32_PAD(9);    // 136 - 139 - 143
  int32   PrevActualBytesSent;     IDL2_INT32_PAD(10);   // 144 - 147 - 151
  int32   PrevProcessId;           IDL2_INT32_PAD(11);   // 152 - 155 - 159
  double  PrevCallDuration;                              // 160 - 167
  double  PrevProcessDuration;                           // 168 - 175
                                   IDL2_CHAR_PAD(12,80); // 176 - 255

};

#endif
