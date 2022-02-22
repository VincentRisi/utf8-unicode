#ifndef _genClnt_H_
#define _genClnt_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_genClnt_H_);
/********************************************************************************
 *
 *
 *	Nedbank Foreign Product System
 *
 *
 *				GENERIC CLIENT / DLL
 *
 *
 *	Design Team:	Pieter Joubert	-	SQL, System Design and team leader
 *					Geoff Bruce		-	C++
 *					Celso Meira		-	VB and C++
 *					Alvin Ward		-	VB and C++
 *					
 *	Start Date inherited from Export LC's 15 May 1997
 *
 *	The dll handles remote procedure calls (RPC) to the Generic servers
 *	This module defines the API Code
 *
 *******************************************************************************/
#include "machine.h"
#include "ti.h"
#ifndef _WINDOWS
#undef WINAPI
#define WINAPI /* */
#endif

extern "C" {

// Swap functions complements to authors of tools.h
#define Swap(a, b) (void) (((a) == (b) || ((a) ^= (b) ^= (a) ^= (b))))

#define Swap2(a)\
  do\
  {\
    char* p = (char*)a;\
    Swap(p[0], p[1]);\
  } while (0)

#define Swap4(a)\
  do\
  {\
    char* p = (char*)a;\
    Swap(p[0], p[3]);\
    Swap(p[1], p[2]);\
  } while (0)

#define Swap8(a)\
  do\
  {\
    char* p = (char*)a;\
    Swap(p[0], p[7]);\
    Swap(p[1], p[6]);\
    Swap(p[2], p[5]);\
    Swap(p[3], p[4]);\
  } while (0)


#ifdef _WINDOWS
int FAR PASCAL _WEP(int);
#endif
// API Functions

unsigned long
WINAPI genLongSwop(unsigned long *ul);

unsigned short
WINAPI genShortSwop(unsigned short *us);

long
WINAPI genTest(	
char *Host, char *Socket, char *syscall,  char *Parameter);

//********************************************************************
// genRPCexec is the simplest call of the generic server suite of API's
// Supply the host name, the socket to use and an "app" name and command
// line arguments if required. The server addressed by the host and socket
// names will then execute the command line and an error returned to
// indicate success or failure.
long
WINAPI genRPCexec(	
char *Host, char *Socket, char *syscall);

//********************************************************************
// genRPCexecP has all the features of genRPCexec plus that a null
// terminated string may be also passed to the app.
long
WINAPI genRPCexecP(	
char *Host, char *Socket, char *syscall,  char *Parameter);

//********************************************************************
// genRPCexecB has the features of genRPCexec but a block (structure)
// may be returned by the app. The target block must be large enough
// for the returned data.
long
WINAPI genRPCexecB(	
char *Host, char *Socket, char *syscall, char *Block);

//********************************************************************
// genRPCexecSP allows the parameter to be structured and contain nulls
// because the size is passed with the call
long
WINAPI genRPCexecSP(	
char *Host, char *Socket, char *syscall,  short *ParmSize,  char *Parameter);

//********************************************************************
// genRPCexecSB allows the app to return the size of the block. NB. The
// target block must still be large enough for the returned data.
long
WINAPI genRPCexecSB(	
char *Host, char *Socket, char *syscall,  long *blockSize, char *Block);

//********************************************************************
// genRPCexecPB combines the features of genRPCexecP and genRPCexecB.
// A null terminated string may be also passed to the app and a block
// of data returned.
long
WINAPI genRPCexecPB(	
char *Host, char *Socket, char *syscall,  char *Parameter, char *Block);

//********************************************************************
// genRPCexecSPSB combines the features of genRPCexecSP and genRPCexecSB.
// The parameter may be structured and contain nulls because the size is
// passed with the call and the size of the block is returned with the
// the block
long
WINAPI genRPCexecSPSB(	
char *Host, char *Socket, char *syscall,  short *ParmSize,   char *Parameter,
	  long *blockSize,  char *Block);

//********************************************************************
// genRPCexecSBmemB allows an genRPCexec type app call, and the app may
// return a block. In this case the block is not returned with the call,
// but the handle to the block is returned. The app may also return the
// record size, the number of records and the size of the block.
// This will allow the memory for the data to be allocated after the call.
// The genRPCFetchBlock, genRPCfetch or genRPCvFetch API's may be used to
// unpack the block. When all the data has been aquired the block must be
// deleted. genRPCfreeB may be used to do this.
long
WINAPI genRPCexecSBmemB(	
char *Host, char *Socket, char *syscall,long *RecordSize,
	long *Records,long *BlockSize, long *memBlock);

//********************************************************************
// genRPCFetchBlock allows the data saved at the handle returned by a
// "memBlock" variable to be unpacked as a block or as smaller blocks.
// The handle, the offset into the returned block and the maximum size
// of the target block to be returned is passed with the call
long
WINAPI genRPCFetchBlock(long *offset,long *maxSize, long *memBlock, char *Block);

//********************************************************************
// genRPCinstProc is a non-app API - the server executes the proc and
// returns the output. The Procedure to be run, the size of the portion
// from the beginning of the block which contains the input data, the
// size of the portion of the end of the block which will target the
// returned output data and the total combined row size needs to be
// passed to the API.
// The inSize measures the size of the input portion of the row
// The outSize measures the output portion of the row ie the tail end.
//
// |----------- inSize ----------|
// |------------------------------- rowSize ----------------|
//                          |--------------- outSize -------|
long
WINAPI genRPCinstProc(
char *Host, char *Socket, char *procName,  long *inSize,  long *outSize,
	long *recSize, char *Block);

#ifdef _WINDOWS

//********************************************************************
// genRPCsqlPlus  is a non-app API and will cause the server to execute
// sqlplus with a command file containing the data in the "Parameter"
// variable. The app is effectively sqlplus. The output is
// spooled by the server and the contents of the spool file put into a
// block and the handle to this block ("memBlock") is returned,
// The "RecordSize" parameter contains the row size and "Records", the number
// of lines in the output. genRPCvFetch would be used to upack the data.
long
WINAPI genRPCsqlPlus(
char *Host, char *Socket, char *userDBname,  short *ParmSize,   char *Parameter,
	long *RecordSize, long *Records, long *memBlock);

//********************************************************************
// genRPCexecSPSRmemB is combination of genRPCexecSPSB and genRPCexecSBmemB.
// and allows an genRPCexecSP type app call, and the app may
// return a block. In this case the block is not returned with the call,
// but the handle to the block is returned. The app may also return the
// record size, the number of records and the size of the block.
// This will allow the memory for the data to be allocated after the call.
// The genRPCFetchBlock, genRPCfetch or genRPCvFetch API's may be used to
// unpack the block. When all the data has been aquired the block must be
// deleted. genRPCfreeB may be used to do this.
long
WINAPI genRPCexecSPSRmemB(	
char *Host, char *Socket, char *syscall,  short *ParmSize,   char *Parameter,
	long *RecordSize, long *Records, long *memBlock);
	
long
WINAPI genRPCexecRmemB(	
char *Host, char *Socket, char *syscall,  short *Records, long *memBlock);

//********************************************************************
// genRPCFetch allows the data saved at the handle returned by a
// "memBlock" variable to be unpacked as a block of fixed length records,
// one record at a time. (Each call returns one record) The record required
// is passed in the "Records" argument and the handle to the returned block
// are passed with the call.
long
WINAPI genRPCfetch(long *Records, long *memBlock, char *Block);

//********************************************************************
// genRPCvFetch allows the data saved at the handle returned by a
// "memBlock" variable to be unpacked as a block of variable length records,
// one record at a time. (Each call returns one record) The record required
// is passed in the "Records" argument and the handle to the returned block
// are passed with the call. The block is structured as a block of text and
// the records are delimited with the newline character (ASCII 10)
long
WINAPI genRPCvFetch(long *Records, long *memBlock, char *Block);

//********************************************************************
// genRPCfreeB may be used to free the memory used to save the "memBlock"
// handle
long
WINAPI genRPCfreeB(long *memBlock);

//********************************************************************
// genRPCinstQuery is a non-app API - the server executes the proc and
// returns the output. The Query to be executed, the size of the portion
// from the beginning of the block which contains the input data, the
// size of the portion of the end of the block which will target the
// returned output data and the total combined row size needs to be
// passed to the API.
// The inSize measures the size of the input portion of the row
// The outSize measures the output portion of the row ie the tail end.
//
// |----------- inSize ----------|
// |------------------------------- rowSize ----------------|
//                          |--------------- outSize -------|
// The block is structured as multiple rows of outSize and the handle to
// the block,  the record size, the number of records and the size of the
// block is returned with the call.
// This will allow the memory for the data to be allocated after the call.
// The genRPCFetchBlock, genRPCfetch or genRPCvFetch API's may be used to
// unpack the block. When all the data has been aquired the block must be
// deleted. genRPCfreeB may be used to do this.
long
WINAPI genRPCinstQuery(
char *Host, char *Socket, char *procName,  long *inSize,  long *outSize,  char *inBlock,
	long *recSize, long *Records, long *memBlock);

#else

long
WINAPI genRPCsqlPlus(
char *Host, char *Socket, char *userDBname,  short *ParmSize,   char *Parameter,
	long *RecordSize, long *Records, char **memBlock);

long
WINAPI genRPCexecSPSRmemB(	
char *Host, char *Socket, char *syscall,  short *ParmSize,   char *Parameter,
	long *RecordSize, long *Records, char **memBlock);

long
WINAPI genRPCexecRmemB(	
char *Host, char *Socket, char *syscall,  long *Records, char **memBlock);

long
WINAPI genRPCfetch(long *Records, char *memBlock, char *Block);

long
WINAPI genRPCvFetch(long *Records, char *memBlock, char *Block);

long
WINAPI genRPCfreeB(char *memBlock);

long
WINAPI genRPCinstQuery(
char *Host, char *Socket, char *procName,  long *inSize,  long *outSize,  char *inBlock,
	long *recSize, long *Records, char **memBlock);

#endif
}

void  OutputDebug(char *dbugString);

#endif
