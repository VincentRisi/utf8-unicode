#include "versions.h"
#define _VERSION_NO "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
COMPILED_VERSION(_VERSION_NO);

#include "ti.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "assign.h"
#include "mcs.h"

tMCS::tMCS(pchar aProgram, pchar aMCSName, ushort aNetNo, ushort aLSN)
  : Program(aProgram)
  , MCSName(aMCSName)
  , NetNo(aNetNo)
  , LSN(aLSN)
  , HeaderType(CONVERSN_HDR)
  , MsgType(NWDATAMSG)
{}

tMCS::tMCS(pchar aProgram, pchar aMCSName, ushort aNetNo, ushort aLSN,
   pchar aMsgType, pchar aOrig_Term, pchar aOrig_Corp)
  : Program(aProgram)
  , MCSName(aMCSName)
  , NetNo(aNetNo)
  , LSN(aLSN)
  , HeaderType(CONVERSN_HDR)
  , MsgType(aMsgType)
  , Orig_Term(aOrig_Term)
  , Orig_Corp(aOrig_Corp)
{}

tMCS::tMCS(pchar aProgram, pchar aMCSName, pchar aNetNo, pchar aLSN)
  : Program(aProgram)
  , MCSName(aMCSName)
  , NetNo(Number(aNetNo,sizeof(buf[0].hdr.dst_netno)))
  , LSN(Number(aLSN,sizeof(buf[0].hdr.dst_lsn)))
  , HeaderType(CONVERSN_HDR)
  , MsgType(NWDATAMSG)
{}

void tMCS::Init()
{
  boj_area boj;
  Assign(boj.progid, Program, sizeof(boj.progid));
  ZINITMSG(&boj);
  if (memcmp(boj.status, C_OK, 8) != 0)
    throw(XMCS2(errMCSInit, Number(boj.status, sizeof(boj.status)),
                            Number(boj.xstatus, sizeof(boj.xstatus))));
}

void tMCS::Read(pchar aData, short aDataLen, int aFlag, int aSleep)
{
  if (aDataLen > sizeof(buf[0].data))
    throw(XMCS2(errMCSSize, aDataLen, sizeof(buf[0].data)));
  Assign(buf[0].ipchdr.ipc_peer, Program, sizeof(buf[0].ipchdr.ipc_peer));
  Assign(buf[0].ipchdr.ipc_source, "", sizeof(buf[0].ipchdr.ipc_source));
  Assign(buf[0].ipchdr.ipc_subq, "", sizeof(buf[0].ipchdr.ipc_subq));
  buf[0].ipchdr.ipc_handle = 0;
  buf[0].ipchdr.ipc_flags = aFlag;
  buf[0].ipchdr.ipc_sleep = aSleep;
  buf[0].ipchdr.ipc_maxcpu = 0;
  buf[0].ipchdr.ipc_checksum = 0;
  buf[0].ipchdr.ipc_qdepth = 0;
  buf[0].ipchdr.ipc_timeinq = 0;
  short result = ZGETMSGQ(&buf[0]);
  if (result < 0)
    throw(XMCS1(errMCSRead, result));
  memcpy(aData, buf[0].data, aDataLen);
}

ushort tMCS::ReadLength()
{
  return Number(buf[0].hdr.message_size, sizeof(buf[0].hdr.message_size));
}

void tMCS::SetTerm(pchar aCorp, pchar aTerm)
{
 Assign(buf[1].hdr.dst_corp, aCorp, sizeof(buf[1].hdr.dst_corp));
 Assign(buf[1].hdr.dst_term, aTerm, sizeof(buf[1].hdr.dst_term));
}

void tMCS::Write(pchar aData, short aDataLen, int aLSN, int aNetNo, int aFlag, int aSleep )
{
  memset(&buf[1].hdr, '0', sizeof(buf[1].hdr));
  Assign(buf[1].hdr.header_type, HeaderType, sizeof(buf[1].hdr.header_type));
  Assign(buf[1].hdr.msg_type, MsgType, sizeof(buf[1].hdr.msg_type));
  Assign(buf[1].hdr.dst_netno, NetNo, aNetNo);
  Assign(buf[1].hdr.dst_lsn, LSN, aLSN);
  Assign(buf[1].hdr.message_size, aDataLen+1, sizeof(buf[1].hdr.message_size));

  // The Orig Term and Orig Corp is used when the sending program wants
  // to appear to the receiving program as a terminal
  if ((strlen(Orig_Term) == 5) && (strlen(Orig_Corp) == 5))
  {
    Assign(buf[1].hdr.src_corp, Orig_Corp, sizeof(buf[1].hdr.origin_term));
    //cout << "Src Corp [" << buf[1].hdr.src_corp << "]" << endl << flush;

    Assign(buf[1].hdr.src_term, Orig_Term, sizeof(buf[1].hdr.origin_term));
    //cout << "Src Term [" << buf[1].hdr.src_term << "]" << endl << flush;
  }

  memset(&buf[1].ipchdr, ' ', sizeof(buf[1].ipchdr));
  Assign(buf[1].ipchdr.ipc_peer, MCSName, sizeof(buf[1].ipchdr.ipc_peer));
  Assign(buf[1].ipchdr.ipc_source, Program, sizeof(buf[1].ipchdr.ipc_source));
  buf[1].ipchdr.ipc_handle = 0;
  buf[1].ipchdr.ipc_flags = aFlag;
  buf[1].ipchdr.ipc_sleep = aSleep;
  buf[1].ipchdr.ipc_maxcpu = 0;
  buf[1].ipchdr.ipc_checksum = 0;
  buf[1].ipchdr.ipc_qdepth = 0;
  buf[1].ipchdr.ipc_timeinq = 0;
  buf[1].ipchdr.ipc_length = aDataLen + sizeof(ZDSEND_MESSAGE_HEADER) + 1;
  memcpy(buf[1].data, aData, aDataLen);

  short result = ZPUTMSGQ(&buf[1]);
  if (result < 0)
    throw(XMCS1(errMCSWrite, result));
}

void tMCS::Write(pchar aData, short aDataLen, int aFlag, int aSleep)
{
  memset(&buf[1].hdr, '0', sizeof(buf[1].hdr));
  Assign(buf[1].hdr.header_type, HeaderType, sizeof(buf[1].hdr.header_type));
  Assign(buf[1].hdr.msg_type, MsgType, sizeof(buf[1].hdr.msg_type));
  Assign(buf[1].hdr.dst_netno, NetNo, sizeof(buf[1].hdr.dst_netno));
  Assign(buf[1].hdr.dst_lsn, LSN, sizeof(buf[1].hdr.dst_lsn));
  Assign(buf[1].hdr.message_size, aDataLen+1, sizeof(buf[1].hdr.message_size));

  // The Orig Term and Orig Corp is used when the sending program wants
  // to appear to the receiving program as a terminal
  if ((strlen(Orig_Term) == 5) && (strlen(Orig_Corp) == 5))
  {
    Assign(buf[1].hdr.src_corp, Orig_Corp, sizeof(buf[1].hdr.origin_term));
    //cout << "Src Corp [" << buf[1].hdr.src_corp << "]" << endl << flush;

    Assign(buf[1].hdr.src_term, Orig_Term, sizeof(buf[1].hdr.origin_term));
    //cout << "Src Term [" << buf[1].hdr.src_term << "]" << endl << flush;
  }

  memset(&buf[1].ipchdr, ' ', sizeof(buf[1].ipchdr));
  Assign(buf[1].ipchdr.ipc_peer, MCSName, sizeof(buf[1].ipchdr.ipc_peer));
  Assign(buf[1].ipchdr.ipc_source, Program, sizeof(buf[1].ipchdr.ipc_source));
  buf[1].ipchdr.ipc_handle = 0;
  buf[1].ipchdr.ipc_flags = aFlag;
  buf[1].ipchdr.ipc_sleep = aSleep;
  buf[1].ipchdr.ipc_maxcpu = 0;
  buf[1].ipchdr.ipc_checksum = 0;
  buf[1].ipchdr.ipc_qdepth = 0;
  buf[1].ipchdr.ipc_timeinq = 0;
  buf[1].ipchdr.ipc_length = aDataLen + sizeof(ZDSEND_MESSAGE_HEADER) + 1;
  memcpy(buf[1].data, aData, aDataLen);

  short result = ZPUTMSGQ(&buf[1]);
  if (result < 0)
    throw(XMCS1(errMCSWrite, result));
}

#define macMove(a,b) memcpy(a, b, sizeof(a))
void tMCS::Echo(pchar aData, short aDataLen, int aFlag, int aSleep)
{
  buf[1] = buf[0];
  macMove(buf[1].hdr.src_netno, buf[0].hdr.dst_netno);
  macMove(buf[1].hdr.src_lsn,   buf[0].hdr.dst_lsn);
  macMove(buf[1].hdr.src_dev,   buf[0].hdr.dst_dev);
  macMove(buf[1].hdr.src_corp,  buf[0].hdr.dst_corp);
  macMove(buf[1].hdr.src_term,  buf[0].hdr.dst_term);
  macMove(buf[1].hdr.dst_netno, buf[0].hdr.src_netno);
  macMove(buf[1].hdr.dst_lsn,   buf[0].hdr.src_lsn);
  macMove(buf[1].hdr.dst_dev,   buf[0].hdr.src_dev);
  macMove(buf[1].hdr.dst_corp,  buf[0].hdr.src_corp);
  macMove(buf[1].hdr.dst_term,  buf[0].hdr.src_term);
  Assign(buf[1].hdr.message_size, aDataLen+1, sizeof(buf[1].hdr.message_size));
  memset(&buf[1].ipchdr, ' ', sizeof(buf[1].ipchdr));
  Assign(buf[1].ipchdr.ipc_peer, MCSName, sizeof(buf[1].ipchdr.ipc_peer));
  Assign(buf[1].ipchdr.ipc_source, Program, sizeof(buf[1].ipchdr.ipc_source));
  buf[1].ipchdr.ipc_handle = 0;
  buf[1].ipchdr.ipc_flags = aFlag;
  buf[1].ipchdr.ipc_sleep = aSleep;
  buf[1].ipchdr.ipc_maxcpu = 0;
  buf[1].ipchdr.ipc_checksum = 0;
  buf[1].ipchdr.ipc_qdepth = 0;
  buf[1].ipchdr.ipc_timeinq = 0;
  buf[1].ipchdr.ipc_length = aDataLen + sizeof(ZDSEND_MESSAGE_HEADER) + 1;
  memcpy(buf[1].data, aData, aDataLen);
  short result = ZPUTMSGQ(&buf[1]);
  if (result < 0)
    throw(XMCS1(errMCSWrite, result));
}
#undef macMove

#define xMCSName "xMCS"

xMCS::xMCS(const xMCS& aX)
: xCept(aX)
{}

xMCS::xMCS(pchar aFname, int aLine, eError aError, int aOne, int aTwo)
: xCept(aFname, aLine, xMCSName, aError)
, One(aOne)
, Two(aTwo)
{
  switch (aError)
  {
  case errMCSInit:
    osErr << "MCS initialisation error ("
          << aOne << ":" << aTwo << ")" << ends;
    break;
  case errMCSRead:
    osErr << "MCS read error(" << aOne << ")" << ends;
    break;
  case errMCSWrite:
    osErr << "MCS write error(" << aOne << ")" << ends;
    break;
  case errMCSSize:
    osErr << "MCS size of data request larger than buffer, request:"
          << aOne << " actual:" << aTwo << ends;
    break;
  }
};
