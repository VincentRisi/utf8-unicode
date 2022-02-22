#ifndef _NSW_FMT_H_
#define _NSW_FMT_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_NSW_FMT_H_);

typedef struct {
	char tag[2];
	char option[5];
	char subtag;
	char part[3];// for each "occurs"
	char description[50];
	char value[81];
}tUntagged, *pUntagged;
#endif
