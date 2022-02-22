#ifndef _DDCOM_H_
#define _DDCOM_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_DDCOM_H_);

//
//  common declarations for Data Distribution Server and Client
//


//
// NOTE : an enum is not being used because its size differs on AIX and PC
// instead  use a const char which is the same size on both platforms plus
// there is no need to byte order
//

// Earliest time for revision
// 1 Jan 1970,  known as Universal Coordinate time for UNIX and DOS WINDOWS
// is the earliest valid time the data distribution system recognises as a valid
// date.  (UCTime)
// A date field of Zero is ment to mean NULL
//

#define UCTime "19700101000000"
#define SERVICE "dds"

const char
      DATA='1', // Data stream follows in block size N bytes    >> CLIENT
      UNKN='2', // Client is enquiring about an unknown table   >> CLIENT
      FAIL='3', // Unable to satisfy request next 80 bytes is error  >> CLIENT
      SYNC='4', // Send all rows for table T starting from rev R     << SERVER
      RQRY='5', // Send count of rows needed to get table T to rev R << SERVER
      TBRV='6', // Sendtable of all tablenames, and their currnent rev<< SERVER
      DWNL='7', // send sql statements to repopulate database  << SERVER
      STOP='8'; // End of conversation stop forked server            << SERVER

enum  { DML_INSERT = 'I', DML_UPDATE = 'U', DML_DELETE = 'D' };

//
//  ENSURE THAT STRUCTURE BOUNDARY FALLS ON 4 BYTE BOUNDARY ie divisible by 4
//  AND FIELDS ON NATURAL BOUNDARY
//
struct MessageHdr {
                    char   mtype;
                    char   tablename[54];
		            char   endian;
                    char   revision[15];
	     	        char   filler0[1];
	     	        union
	     	        {
                      long   msize;         // no of bytes that follow header
                      long   RevisionCount;
                    };
                  };                        // server overloads to Row Count
                                            // when responding a RQRY request

//
// This structure will be the same Getvers on all tables
// provided that sqldef doesn't change the structure it generates
//
typedef struct   {

  char     RowUpdated [15],
           filler[1];
  long     Count;
}tGetVerCount;


#endif

