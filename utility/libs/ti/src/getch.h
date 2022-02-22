#ifndef _GETCH_H_
#define _GETCH_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_GETCH_H_);

#if defined(M_OS2) || defined(M_DOS)
#include <conio.h>
#endif

/*  call at start to initialise tty properly for single character input */
int initch(void);

/*  call at end to reset the tty */
int endch(void);

/*  single character non-buffered console read/write routines */
int inchar(void);
int outchar(int c);

#endif

