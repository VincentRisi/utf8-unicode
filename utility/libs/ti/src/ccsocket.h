#ifndef _CCSOCKET_H_
#define _CCSOCKET_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_CCSOCKET_H_);

#include "obsocket.h"

#include "ti.h"

struct tCCSockBuffer
{
   unsigned long BytesToRead;
   unsigned long BytesRead;
   unsigned long BytesToWrite;
   unsigned long BytesWritten;
   char* p;
};
typedef struct tCCSockBuffer* pCCSockBuffer;

class tCCSockClient
{
protected:
   pSockCB     fSockCB;
   ulong       fTimeout;
   ushort      fRetries;
   int         fError;
   char        fErrorStr[256];
    ostrstream  fErrMsg;
   bool           fIsOpen;       // Open status

   // Default constructor
   tCCSockClient() : fErrMsg(fErrorStr, sizeof(fErrorStr)) {};

   static int ParseName(char* name, char sep, char* names[]);
public:

   // This form of the constructor will parse the Name and pull out the
   // Host and Service. This is present to cater for the older interfaces
   // like cliserv and cs. the form of this name is :socket:<host>:<service>
   tCCSockClient(pchar aName);

   // This is the preferred form of constructor.
   // aHost is the name of the server providing the service, defined either in
   // the hosts file (/etc/hosts or \winnt\system32\drivers\etc\hosts) or
   // resolved by a domain name server.
   // aService is a service name defined in the services file // (/etc/services
   // or \winnt\system32\drivers\etc\services). This must resolve to the same
   // number on both the local and remote machines.
   tCCSockClient(pchar aHost, pchar aService);

   // Standard destructor
   ~tCCSockClient();

   int ShowOnError( ostream& os); // return 0 if no error

   // Open the socket for the client.
   void  Open();

   // Close the socket.
   void Close();

   // ReadLength is used to read an unsigned long as data and convert it
   // to host from network byte order. This is the counterpart for
   // WriteLength;
   ulong  ReadLength();

   // Read data into aBuffer for the exact length aLength.
   // This is used directly after ReadLength;
   void   Read(void * aBuffer, ulong aLength);

   // Read data into aBuffer for the length aToRead throwing away
   // any excess and return the actual no read in aRead.
   // This is arguably a bad form and should be avoided.
   void   Read(void * aBuffer, ulong aToRead, ulong &aRead);

   // Old form read that uses a tBuffer construct for the lengths and data.
   // Will resolve into new form.
   void   Read(pCCSockBuffer aBuffer);

   // Read data into aBuffer up to the length aLength
   // returns the length read.
   ulong  ReadStream(void * aBuffer, ulong aLength);

   // WriteLength writes an unsigned long as data after converting it
   // to network byte order.
   void   WriteLength(ulong aLength);

   // Write data for exactly the length of aLength. This is normally
   // used after WriteLength, but can also be used to write stream
   // data.
   void   Write(void * aBuffer, ulong aLength);

   // Old form write that uses a tBuffer construct for the lengths and
   // data. Will resolve into the new form.
   void   Write(pCCSockBuffer aBuffer);

   // WaitRead will not read data, but will wait until there is data
   // to read or timeout. Returns true if there is data or returns
   // false if it times out.
   bool   WaitRead(ulong aTimeout);

   // Will try to read data into for for specified length. Timeout and retries
   // are specified. Returns true if reads OK or false if fails.
   bool ReadAll(void * aBuffer, ulong aLength, ulong aTimeout, ushort aRetries);

   // WaitWrite will not write data, but will wait until the
   // counterpart is ready to read or timeout. Returns true if the
   // counterpart is ready or returns false if it times out.
   bool   WaitWrite(ulong aTimeout);

   // This only really makes sense from a client point of view
   // Will do an old style write followed by a read.
   void Transact(pCCSockBuffer bufOut, pCCSockBuffer bufIn); // WR

   // This only really makes sense from a client point of view
   // Will do an open, old style write followed by a read, then a close.
   void Call(pCCSockBuffer bufOut, pCCSockBuffer bufIn);     // OWRC

   // Sets the timeout parameter is millisecs
   void   SetTimeout(ulong aTimeout) { fTimeout = aTimeout; }

   // gets the timeout value
   ulong  GetTimeout() { return fTimeout; }

   // Sets the number of times to retry
   void   SetRetries(ushort aRetries) { fRetries = aRetries; }

   // Gets the number of retries
   ushort GetRetries() { return fRetries; }

   // Get the last error
   int LastError( void ) { return fError; }

   // Open status
   bool IsOpen( void ) { return fIsOpen; }

};
typedef tCCSockClient*  pCCSockClient;
typedef tCCSockClient&  rCCSockClient;
typedef tCCSockClient*& rpCCSockClient;

class tCCSockServer : public tCCSockClient
{
   // This form of constructor is illegal now.
   tCCSockServer() {}
public:
   // This form of constructor expects a service defined in a services file.
   // see above. It will also honour the old form name :socket:<host>:<service>
   // and extract the service.
   tCCSockServer(pchar aService);

   // Open the socket for the server.
   void Open();

   // Waits until a client is ready to connect or times out.
   // Does not do the Open. Returns true if a client is
   // present else false if times out.
   bool WaitConnect(ulong aTimeout);
};
typedef tCCSockServer*  pCCSockServer;
typedef tCCSockServer&  rCCSockServer;
typedef tCCSockServer*& rpCCSockServer;

#endif
