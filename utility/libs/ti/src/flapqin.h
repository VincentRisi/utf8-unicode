#ifndef __FLAPD_H
#define __FLAPD_H "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(__FLAPD_H);

#define flapdefSign 0xBEEF
#define flinkSign   0xFEED

typedef enum eFieldType eFieldType;
typedef struct tQueryCB tQueryCB, *pQueryCB;
typedef struct tFieldDef tFieldDef, *pFieldDef;

typedef struct tQuery  tQuery,  *pQuery;
typedef struct tBuffer tBuffer, *pBuffer;
typedef int (*fptr)(const void*, const void*);

enum eFieldType
{
  ftypeChar     = 'C',
  ftypeFloat    = 'F',
  ftypeInt      = 'I',
  ftypeSmallint = 'S',
  ftypeTinyint  = 'T'
};

struct tQueryCB
{
  pchar Name;
  pchar Server;
  pchar Schema;
  pchar Table;
  ushort Size;
  pchar SQLStmt;
  ushort Length;
  ushort NoFields;
  pFieldDef Fields;
};

struct tFieldDef
{
  pchar Name;
  eFieldType Type;
  ushort Length;
  ushort Offset;
};

struct tQuery
{
  pchar  Name, SQLStmt;
  ushort Size;
  ushort ServerNo;
  ushort SchemaNo;
  ushort TableNo;
  ushort BufferNo;
};

struct tBuffer
{
  ushort Length;
  ushort NoFields;
  pFieldDef Fields;
};

bool InitQuery(char* Filename);
pQueryCB GetQuery(char* Query);
void FreeQuery(pQueryCB QueryCB);
void DoneQuery(void);

#endif

