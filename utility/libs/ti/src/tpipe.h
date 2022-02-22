#ifndef _TPIPE_H_
#define _TPIPE_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_TPIPE_H_);

#define errtypNone   '0'
#define errtypNp     '1'
#define errtypShm    '2'
#define errtypSem    '3'
#define errtypSocket '4'
#define errtypOther  '5'
#define errtypSQL    '6'
typedef char eErrTyp;

typedef struct
{
  char ReqCode;
  char ErrType;
  char Error[4];      /* ErrType + Error together form a complete error. */
} tPipeHeader, *pPipeHeader;

/*  other errors: values of Error when ErrType is errtypOther */
#define ERR_GENERAL        1
#define ERR_MALLOC         2
#define ERR_TIMEOUT        3
#define ERR_WRONGSERVER    4
#define ERR_BADNAME        5
#define ERR_NOHOST         6
#define ERR_HOSTTRYAGAIN   7
#define ERR_HOSTNORECOVERY 8
#define ERR_HOSTNOADDRESS  9
#define ERR_IGNORE        10

#endif
