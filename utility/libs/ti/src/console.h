#ifndef _CONSOLE_H_
#define _CONSOLE_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_CONSOLE_H_);

#ifdef M_DOS
#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */
#endif

void APPLAPI OpenDisplay(void);
void APPLAPI DisplayLine(char* aLine);
void APPLAPI ClearDisplay(void);
void APPLAPI CloseDisplay(void);

#ifdef M_DOS
#ifdef __cplusplus
}
#endif  /* __cplusplus */
#endif

#endif

