#ifndef _CODEPAGE_H_
#define _CODEPAGE_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_CODEPAGE_H_);
/*
Copyright (c) 1998, 1999 Thai Open Source Software Center Ltd
See the file copying.txt for copying permission.
*/

int codepageMap(int cp, int *map);
int codepageConvert(int cp, const char *p);

#endif
