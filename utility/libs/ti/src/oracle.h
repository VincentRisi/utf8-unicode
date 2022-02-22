#ifndef __ORACLE_H
#define __ORACLE_H "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(__ORACLE_H);

#define HDALEN 256
#define ORAERRLEN 80
#define NO_MORE_DATA_ERR 4

// oracle external types
enum
{
  ORACLE_INT = 3,
  ORACLE_FLOAT,
  ORACLE_CHAR
};

// lda structure
typedef struct
{
  short          V2_rc;        // V2 return code
  unsigned char  fill1[10];    // filler
  unsigned short rc;           // V7 return code
  unsigned char  fill2[19];    // filler
  unsigned long  ose;          // os dependent error code
  unsigned char  chk;          // check byte
  unsigned char  sysparm[26];  // system area
} LDADEF;

// cursor structure - note: we have added a few fields at the end of this
typedef struct
{
  short          V2_rc;        // V2 return code
  short          ft;           // sql function type
  unsigned long  rpc;          // rows processed count
  short          peo;          // parse error offset
  unsigned char  fc;           // OCI function code
  unsigned char  fill1;        // filler
  unsigned short rc;           // V7 return code
  unsigned char  wrn;          // warning flags
  unsigned char  flg;          // error flags
  unsigned long  cn;           // cursor number
  unsigned char  rid[13];      // rowid internal structure
  unsigned long  ose;          // os dependent error code
  unsigned char  chk;          // check byte
  unsigned char  sysparm[26];  // system area

  // internal bb&d use
  pQueryCB query;              // pointer to query from binary file
  bool     first;              // first row has not been read
} CSRDEF;

// prototypes
// obndrn
// obndrv
// obreak
int ocan(CSRDEF *cursor);
int oclose(CSRDEF *cursor);
// ocof
int ocom(LDADEF *lda);
// ocon
int odefin(CSRDEF *cursor, int position, void* buffer, int buf_len,
  int f_type, int scale, short *ind_p, char *fmt, int fmt_len, int fmt_typ,
  short* ret_len, short* ret_code);
#define ODEFIN(cursor, pos, buf, len, type, ind_p, ret_len, ret_code) \
  odefin(cursor, pos, buf, len, type, -1, ind_p, 0, -1, -1, ret_len, ret_code)
#define ODEFIN0(cursor, pos, buf, len, type) \
  ODEFIN(cursor, pos, buf, len, type, 0, 0, 0)
// odsc
int oerhms(LDADEF *lda, short rcode, char* msg_buf, int buf_len);
int oermsg(short rcode, char* msg_buf);
int oexec(CSRDEF *cursor);
// oexn
// ofen
int ofetch(CSRDEF *cursor);
int ologof(LDADEF *lda);
int olon(LDADEF *lda, char* user_id, int user_id_len, char* password,
  int passwd_len, int audit_flag);
#define OLON(lda, user_id, password) \
  olon(lda, user_id, -1, password, -1, -1)
// oname
int oopen(CSRDEF *cursor, LDADEF *lda, char* dbn, int dbn_len, int area_size,
  char* user_id, int user_id_len);
#define OOPEN(cursor, lda) oopen(cursor, lda, 0, -1, -1, 0, -1)
// oopt
int orlon(LDADEF *lda, char* hda, char* user_id, int user_id_len,
  char* passwd, int passwd_len, int audit_flag);
#define ORLON(lda, hda, user_id) orlon(lda, hda, user_id, -1, 0, -1, -1)
int orol(LDADEF *lda);
int osql3(CSRDEF *cursor, char* statement, int statement_len);
#define OSQL3(cursor, statement) osql3(cursor, statement, -1)
// sqlda

#endif

