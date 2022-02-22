// sfcommon.h
// barry fortune
// aug 94
// struct tSfReceive
// struct tSfRet

#ifndef _SFCOMMON_H_
#define _SFCOMMON_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_SFCOMMON_H_);

#ifdef M_AIX
#include <netinet/in.h>
#endif

// for illegal struct hack
#define MAX_HACK 10000
typedef char DATA[MAX_HACK];

#define APPCODELEN 4
typedef char APPCODE[APPCODELEN]; // application code - *not* null terminated

#define SERVICELEN 9
typedef char SERVICE[SERVICELEN + 1]; // service in services file

enum eDirection { hton, ntoh }; // host to/from network order
#define CONVERT_SHORT(direction, field) field =       \
  ((direction == hton) ? htons(field) : ntohs(field))

typedef long tAlign;

// allocates a new type* pointer aligned on the type 'tAlign' for at least
// length bytes -- the pointer must be freed with delete[]
#define HACK_PTR(type, length) (type*) \
  (new tAlign[(length + sizeof(tAlign) - 1) / sizeof(tAlign)])

// same as above with length increased by offsetof(type, field)
#define HACK_PTR_OFFSET(type, length, field) \
  HACK_PTR(type, length + offsetof(type, field))

typedef struct tSfReceive // sends data to next s&f
{
  APPCODE  appcode;       // application code
  short    msgLength;     // network byte order - length of data
  DATA     data;          // only appears if msgLength > 0

  // convert to/from network byte order
  void convert(eDirection direction) { CONVERT_SHORT(direction, msgLength); }
} *pSfReceive;

typedef struct tSfRet
{
  short    rc;           // return code from s&f - network byte ordered

  // convert to/from network byte order
  void convert(eDirection direction) { CONVERT_SHORT(direction, rc); }
} *pSfRet;

#endif

