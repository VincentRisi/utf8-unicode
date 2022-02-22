#ifndef _OCIAPR__H_
#define _OCIAPR__H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_OCIAPR__H_);

#ifdef __cplusplus
extern "C" {
#endif

#if defined(OCI_UPPER) || defined(ORA71DLL) || defined(ORA72DLL)
#define obndrn OBNDRN
#define obndrv OBNDRV
#define obreak OBREAK
#define ocan   OCAN
#define oclose OCLOSE
#define ocof   OCOF
#define ocom   OCOM
#define ocon   OCON
#define odefin ODEFIN
#define odsc   ODSC
#define odescr ODESCR
#define oerhms OERHMS
#define oermsg OERMSG
#define oexec  OEXEC
#define oexfet OEXFET
#define oexn   OEXN
#define ofen   OFEN
#define ofetch OFETCH
#define oflng  OFLNG
#define ologof OLOGOF
#define olon   OLON
#define oname  ONAME
#define oopen  OOPEN
#define oopt   OOPT
#define oparse OPARSE
#define orlon  ORLON
#define orol   OROL
#define sqlld2 SQLLD2
#define sqllda SQLLDA
#endif

int  APC obndrn(cda_def *cursor, int sqlvn,
                void *progv, int progvl, int ftype, int scale,
                short *indp, char *fmt, int fmtl, int fmtt);
int  APC obndrv(cda_def *cursor, char *sqlvar, int sqlvl,
                void *progv, int progvl, int ftype, int scale,
                short *indp, char *fmt, int fmtl, int fmtt);
int  APC obreak(lda_def *lda);
int  APC ocan  (cda_def *cursor);
int  APC oclose(cda_def *cursor);
int  APC ocof  (lda_def *lda);
int  APC ocom  (lda_def *lda);
int  APC ocon  (lda_def *lda);
int  APC odefin(cda_def *cursor, int pos, void* buf,
                int bufl, int ftype, int scale, short *indp,
                char *fmt, int fmtl, int fmtt,
                unsigned short* rlen,
                unsigned short* rcode);
int  APC odsc  (cda_def *cursor, int pos, short *dbsize,
                short *fsize, short *rcode, short *dtype,
                char *buf, short *bufl, short *dsize);
int  APC odescr(cda_def *cursor, int pos, long *dbsize,
                short *dbtype, char *cbuf, long *cbufl,
                long *dsize,
                short *prec, short *scale, short *nullok);
int  APC oerhms(lda_def *lda, short rcode, char *buf, int bufl);
int  APC oermsg(short rcode, char* msg_buf);
int  APC oexec (cda_def *cursor);
int  APC oexfet(cda_def *cursor, unsigned long nrows,
                int cancel, int exact);
int  APC oexn  (cda_def *cursor, int iters, int rowoff);
int  APC ofen  (cda_def *cursor, int nrows);
int  APC ofetch(cda_def *cursor);
int  APC oflng (cda_def *cursor, int pos, void *buf,
                long bufl, int dtype, unsigned long *retl,
                long offset);
int  APC ologof(lda_def *lda);
int  APC olon  (lda_def *lda, char* uid, int uidl,
                char* pswd, int pswdl, int audit);
int  APC oopen (cda_def *cursor, lda_def *lda,
                char* dbn, int dbnl, int arsize,
                char* uid, int uidl);
int  APC oopt  (cda_def *cursor, int rbopt, int waitopt);
int  APC oname (cda_def *cursor, int pos, char *tbuf, short *tbufl,
                char* buf, short *bufl);
int  APC oparse(cda_def *cursor, char *sqlstm, long sqll,
                int defflg, unsigned long lngflg);
int  APC orlon (lda_def *lda, char* hda, char* uid, int uidl,
                char* pswd, int pswdl, int audit);
int  APC orol  (lda_def *lda);
void APC sqlld2(lda_def *lda, char *cname, long *cnamel);
void APC sqllda(lda_def *lda);
#if defined(M_W32) || defined(M_AIX)
/* OCI Environment Modes for opinit call */
#define OCI_EV_DEF 0                  /* default single-threaded environment */
#define OCI_EV_TSF 1                              /* thread-safe environment */
/* OCI Logon Modes for olog call */
#define OCI_LM_DEF 0                                        /* default login */
#define OCI_LM_NBL 1                                   /* non-blocking logon */
int  opinit   (int mode);
int  APC olog  (lda_def *lda, char* hda, char* uid, int uidl,
                char* pswd, int pswdl, char* conn, int connl, int mode);
#endif
#ifdef __cplusplus
}
#endif

#endif

