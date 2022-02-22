#ifndef _SQLTOK_H_
#define _SQLTOK_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_SQLTOK_H_);

char * SqlToken(char * Source, char * Token, int TokenSize);
// Returns point in source after token. Null indicates completed
// Fills Token with the token conforming to SQL and additionally
// :vars (placeholders) and $vars (dynamic substitutions)

#endif
