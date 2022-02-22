#include "versions.h"
#define _VERSION_NO "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
COMPILED_VERSION(_VERSION_NO);

#include "machine.h"

//#include <ctype.h>
#include <string.h>

#include "sqltok.h"

#define sqltokALPHA "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define sqltokDIGIT "0123456789"
#define sqltokALPHANUM sqltokALPHA sqltokDIGIT
#define sqltokALPHANUMPLUS sqltokALPHANUM "_$#"
#define sqltokDIGITPLUS sqltokDIGIT ".+-eE"

inline int alpha(char ch)
{
  return (ch && strchr(sqltokALPHA, ch) != 0) ? 1 : 0;
}

inline int alphaNum(char ch)
{
  return (ch && strchr(sqltokALPHANUM, ch) != 0) ? 1 : 0;
}

inline int alphaNumPlus(char ch)
{
  return (ch && strchr(sqltokALPHANUMPLUS, ch) != 0) ? 1 : 0;
}

inline int digit(char ch)
{
  return (ch && strchr(sqltokDIGIT, ch) != 0) ? 1 : 0;
}

inline int digitPlus(char ch)
{
  return (ch && strchr(sqltokDIGITPLUS, ch) != 0) ? 1 : 0;
}

char * SqlToken(char * Source, char * Token, int TokenSize)
{
  int n;
  char * Result = Source+1;
  switch(Source[0])
  {
  case ' ':
  case '\t':
    while (Result[0] == ' ' || Result[0] == '\t')
      Result++;
    break;
  case ':':
  case '$':
    if (Result[0] == Source[0])
      break;
    if (alpha(Result[0]))
    {
      Result++;
      while (alphaNumPlus(Result[0]))
        Result++;
    }
    break;
  case '\'':
  case '\"':
    while (Result[0] && Result[0] != Source[0])
      Result++;
    if (Result[0] == Source[0])
      Result++;
    break;
  default:
    if (alpha(Source[0]))
    {
      while (alphaNumPlus(Result[0]))
        Result++;
      break;
    }
    if (digit(Source[0]))
    {
      while (digitPlus(Result[0]))
        Result++;
      break;
    }
    break;
  }
  n = Result - Source;
  if (n > TokenSize-1)
    n = TokenSize-1;
  strncpy(Token, Source, n);
  Token[n] = 0;
  return Result;
}
