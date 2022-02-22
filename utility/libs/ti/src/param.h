#ifndef _PARAM_H_
#define _PARAM_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_PARAM_H_);

#define MAXLINE 256

enum
{
  errNO_MORE_SECTIONS = 1,
  errNO_MORE_KEYWORDS,
  errBAD_LINE
};

/*  open the parameter file */
int ParamInitLoad(char* fname);

/*  close the parameter file */
void ParamTerminateLoad(void);

/*  get next section in the file */
int ParamGetNextSection(              /*  non-zero if no more sections */
  char* section,                      /*  returns next section */
  size_t sectionLen);                 /*  sizeof of buffer including null */

/*  get next keyword/value in section */
int ParamGetNextKeyword(              /*  non-zero if at end of section */
  char* keyword,                      /*  returns next section */
  size_t keywordLen,                  /*  sizeof of buffer including null */
  char* value,                        /*  returns next value */
  size_t valueLen);                   /*  sizeof of buffer including null */

#endif

