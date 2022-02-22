#ifndef _OCIDFN__H_
#define _OCIDFN__H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_OCIDFN__H_);

typedef struct cda_def
{
  short          v2_rc;        /*  V2 return code */
  unsigned short ft;           /*  SQL function type */
  unsigned long  rpc;          /*  rows processed count */
  unsigned short peo;          /*  parse error offset */
  unsigned char  fc;           /*  OCI function code */
  unsigned char  fill1;
  unsigned short rc;           /*  V7 return code */
  unsigned char  wrn;          /*  warning flags */
  unsigned char  flg;
  unsigned int   d0;           /*  2bytes vs 4bytes or ?? */
  struct
  {
    struct
    {
      unsigned long  d1;
      unsigned short d2;
      unsigned char  d3;
    } rd;
    unsigned   long  d4;
    unsigned   short d5;
  } rid;
  unsigned int  ose;          /*  os dependent error code */
  unsigned char sysparm[27];  /*  system area */
  unsigned char extra[4];
} cda_def, lda_def;

#endif

