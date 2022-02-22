/*
Copyright (c) 1998, 1999 Thai Open Source Software Center Ltd
See the file copying.txt for copying permission.
*/
#ifndef _XMLFILE_H_
#define _XMLFILE_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_XMLFILE_H_);

#define XML_MAP_FILE 01
#define XML_EXTERNAL_ENTITIES 02

extern int XML_ProcessFile(XML_Parser parser,
			   const XML_Char *filename,
			   unsigned flags);

#endif
