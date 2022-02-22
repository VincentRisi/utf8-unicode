#ifndef SFAPI_H_
#define SFAPI_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(SFAPI_H_);

// FileName: sfapi.h
// Description: header file for C interface to S&F functions in windows dll
// SubSystem: infrastructure
// Author: P. Searle, b fortune, j pickup
// Date Created: Nov 1994

#include "clisock.h"

#ifdef __cplusplus
extern "C" {
#endif

// -------------------------------------------------------------------
// functions used by store and forward SERVER to init and stop service
// and to transmit queued data and check queue depths
// -------------------------------------------------------------------

// Return DLL version info in Info up to MaxLen chars
void FAR PASCAL _export sfGetVersionInfo(char *Info, short MaxLen);

// starts the store and forward service - only called by store and forward
// server application
short FAR PASCAL _export sfStartup(
  char* aQPath,                // path for persistent queue files
  short aQElementSize,         // maximum size of queue items
  short aQCntApplKeys);        // maximum number of sub queues allowed

// stops store and forward service - only called by the store and forward
// server application
short FAR PASCAL _export sfShutdown();

// for the dll to process a timer event
// checks on the TransmitQueue - head of each sub queue - for items to
// transmit to the host. Attempts to transmit each queue item found and
// if succesfull dequeues from transmit queue and enqueues to transmit
// status queue - only called by the store and forward server application
short FAR PASCAL _export sfProcessTimer();

// returns count of queued items on each of the three queues - only
// called by the store and forward server application
short FAR PASCAL _export sfQueryStats(
  short* aQ1,                  // out: count of items in TX queue
  short* aQ2,                  // out: count of items in TXS queue
  short* aQ3);                 // out: count of items in RX queue

// ------------------
// connection methods
// ------------------

// connects a calling program to the store and forward service
// the 'Handle' returned is used in all subsequent operations
// with the store and forward service to identify this caller
short FAR PASCAL _export sfConnect(
   short* aHandle,             // out: Handle - magic cookie for connection
   char*  aHost,               // Server name in 'hosts' file - can be 0
   char*  aService,            // Service in 'services' file - can be 0
   char*  aApplName);          // application name

// disconnect from the store and forward service. All clients calling
// sfConnect must call sfDisconnect - if not memory leaks result
//*** should add mechanism to detect a user closing another way
short FAR PASCAL _export sfDisconnect(
  short  aHandle);             // 'Handle' from a prior call to sfConnect

// ----------------
// RX queue methods
// ----------------

// add item to RX (receive) queue - only called by the store and forward
// server application
short FAR PASCAL _export sfInsRxMsg(
   //short aHandle,              // 'Handle' from a prior call to sfConnect
   char* aAppName,             // Name of the application inserting the msg
   char* aData,                // Data to be added to RX queue as the msg
   short aLen);                // Length of aData

// get next item from RX (receive) queue
short FAR PASCAL _export sfReadRxMsg(
   //short  aHandle,             // 'Handle' from a prior call to sfConnect
   char*  aAppName,            // Application from which we require a msg
   char*  aData,               // out: data
   short* aLen,                // inout: Length of aData
   short* aItemId);            // out: a unique id for this queued item.

// delete next item from RX queue - validate aItemId matches the next item
short FAR PASCAL _export sfDelRxMsg(
   //short aHandle,              // 'Handle' from a prior call to sfConnect
   char*  aAppName,            // App which sent msg we wish to delete
   short aItemId);             // a unique id for this queued item.

// --------------------
// TX/TXS queue methods
// --------------------

// queue a message for transmission
short FAR PASCAL _export sfInsTxMsg(
   short     aHandle,          // 'Handle' from a prior call to sfConnect
   long      aReqCode,         // rpc request code
   DATABLOCK aData,            // data to be queued
   DATALEN   aDataSize,        // Length of aData
   short*    aItemId);         // out: a unique id for this queued item.

// query status of an item queued in TX queue. Item may now be in either
// the TX or TXS queue
short FAR PASCAL _export sfQueryTxMsg(
  short      aHandle,          // 'Handle' from a prior call to sfConnect
  short*     isSent,           // out: 0 - not sent, other - sent
  DATABLOCK* aData,            // out: response from the server, if sent
                               // must be freed when done with sfFree(aData)
  DATALEN*   aDataSize,        // out: Length of aData.
  short*     socketErr,        // out: Winsock error if not sent
                               // use sfSockErrMsg to get error string
  short*     retries,          // out: no of retries, if not sent
  short      aItemId);         // ItemId to query on specified queue.

// free block of memory allocated in sfQueryQueue
void FAR PASCAL _export sfFree(
  DATABLOCK aData);            // data block

// lookup error code and return error string
short FAR PASCAL _export sfSockErrMsg(
  short socketErr,             // socket error returned from Winsock
  char* msg,                   // out: error string
  short len);                  // length of msg buffer

// lookup error code and return error string
short FAR PASCAL _export sfErrMsg(
  short sfErr,                 // error return from function in this module
  char* msg,                   // out: error string
  short len);                  // length of msg buffer

// delete an item from TX or TXS queue - whichever it belongs to
short FAR PASCAL _export sfDelTxMsg(
  short aHandle,               // 'Handle' from a prior call to sfConnect
  short aItemId);              // ItemId to query on specified queue. This

// display the last tpqueue error in a message box
// (used for debugging)
short FAR PASCAL _export sfShowError(void);

#ifdef __cplusplus
}
#endif



