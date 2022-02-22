#include "versions.h"

#define _VERSION_NO "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
COMPILED_VERSION(_VERSION_NO);

// tstring.c
// Author: Barry Fortune
// Date: 1 May 1994
// class xString -- string exception class
// class tString -- string class

#include <ctype.h>

#include "ti.h"
#include "tstring.h"

//-------------------------------------------------------------
//  xString class
//-------------------------------------------------------------

#define xStringName "xString"

xString::xString(char * aFname, int aLine, eError aError) :
  xCept(aFname, aLine, xStringName, aError)
{
  switch (aError)
  {
  case errOverflow:
    //sprintf(errorMsg, "String exceeded character limit");
    osErr << "String exceeded character limit";
    break;
  }
};

//-------------------------------------------------------------
//  tString class
//-------------------------------------------------------------

// string comaparison method
int tString::Compare(const tString& str) const
{
  // handle special cases where one or both strings are empty
  if (p == 0)
    return (str.p == 0) ? 0 : -1;
  if (str.p == 0)
    return 1;

  return (compMode == cmSensitive) ? strcmp(p, str) : stricmp(p, str);
}

// default constructor
tString::tString() :
  compMode(cmSensitive)
{
  len = siz = 0;
  p = 0;
}

// copy constructor
tString::tString(const tString& str) :
  compMode(str.compMode),
  len(str.len),
  siz(str.siz),
  p(str.p)
{
  // note: we assume str is a valid string, so don't check for overflow

  if (p != 0)
  {
    p = new char[siz];
    memcpy(p, str.p, len + 1);
  }
}

// constuctor from cstring
tString::tString(const char * cstr) :
  compMode(cmSensitive)
{
  if ((cstr == 0) || (*cstr == 0))
  {
    len = 0;
    siz = 0;
    p = 0;
  }
  else
  {
    len = strlen(cstr);
    siz = CalcSize(len);
    if (siz > maxStrlen)
      throw XString(errOverflow);

    p = new char[siz];
    memcpy(p, cstr, len + 1);
  }
}

// constuctor from nstring
tString::tString(const char * anstr, size_t asize)
: compMode(cmSensitive)
, len(asize)
{
  siz = CalcSize(len+1);
  if (siz > maxStrlen)
    throw XString(errOverflow);
  p = new char[siz];
  memcpy(p, anstr, len);
  p[len]=0;
}

// single-char constructor
tString::tString(char c) :
  compMode(cmSensitive)
{
  len = 1;
  siz = CalcSize(1);
  p = new char[siz];
  *p = c;
  p[1] = 0;
}

// multi-char constructor
tString::tString(size_t count, char fill) :
  compMode(cmSensitive)
{
  if (count == 0)
  {
    len = siz = 0;
    p = 0;
    return;
  }

  len = count;
  siz = CalcSize(count);
  if (siz > maxStrlen)
    throw XString(errOverflow);

  p = new char[siz];
  memset(p, fill, count);
  p[count] = 0;
}

tString::~tString()
{
  if (p != 0)
    delete [] p;
  p = 0;
}

tString& tString::operator=(const tString& str)
{
  // note: we assume str is a valid string, so don't check for overflow

  if (p == str.p)                 // handle case of s = s;
    return *this;

  len = str.len;
  siz = str.siz;
  compMode = str.compMode;

  if (p != 0)
    delete [] p;
  p = 0;

  if (siz)
  {
    p = new char[siz];
    memcpy(p, str.p, len + 1);
  }

  return *this;
}

tString& tString::operator=(const char * cstr)
{
  if (p != 0)
    delete [] p;
  p = 0;
  compMode = cmSensitive;
  if ((cstr == 0) || (*cstr == 0))
  {
    len = 0;
    siz = 0;
  }
  else
  {
    len = strlen(cstr);
    siz = CalcSize(len);
    if (siz > maxStrlen)
      throw XString(errOverflow);

    p = new char[siz];
    memcpy(p, cstr, len + 1);
  }
  return *this;
}

tString tString::AsUpper() const
{
  tString temp = *this;
  temp.compMode = compMode;

  if (temp.p != 0)
    strupr(temp.p);

  return temp;
}

tString tString::AsLower() const
{
  tString temp = *this;
  temp.compMode = compMode;

  if (temp.p != 0)
    strlwr(temp.p);

  return temp;
}

tString::tString(const tString& str1, const tString& str2) :
  compMode(str1.compMode)
{
  size_t totalLen = str1.len + str2.len;
  if (totalLen == 0)
  {
    len = 0;
    siz = CalcSize(len);
    p = 0;
    return;
  }

  size_t size = CalcSize(totalLen);
  if (size > tString::maxStrlen)
    throw XString(errOverflow);

  len = 0;
  siz = size;
  p = new char[siz];

  if (str1.p != 0)
  {
    memcpy(p, str1.p, str1.len);
    len = str1.len;
  }

  if (str2.p != 0)
  {
    memcpy(p + len, str2.p, str2.len);
    len += str2.len;
  }

  p[len] = 0;
}

tString::tString(const tString& str, const char ch) :
  compMode(str.compMode)
{
  size_t totalLen = str.len + 1;

  size_t size = CalcSize(totalLen);
  if (size > tString::maxStrlen)
    throw XString(errOverflow);

  len = size_t(totalLen);
  siz = size_t(size);
  p = new char[siz];

  if (str.p != 0 && str.len != 0)
    memcpy(p, str.p, len);

  p[str.len]  = ch;
  p[len] = 0;
}

/*  -------------------------------------------------------------- */
#ifdef TSTRING_TST

int main()
{
  Init();

  // upper/lower case, index ----------
  tString x("\"this is a testABCD123abcd!@#$%^&*();'{}[]\\|~`\"");
  std::cout << "x           = <" << x << ">" << std::endl;

  std::cout << "x.AsLower() = <" << x.AsLower() << ">" << std::endl;

  x.ToUpper();
  std::cout << "x.ToUpper() : <" << x << ">" << std::endl;

  std::cout << "x[1] = '" << x[1] << "'" << std::endl;

  // empty string, single character, repeated character
  tString y;
  tString single('1');
  tString z(4, 'z');
  std::cout << "y = <" << y << ">, single = <" << single
       << ">, z = <" << z << ">" << std::endl;

  // conversion, length
  std::cout << "char *(z) = <" << char *(z) << ">" << std::endl;
  std::cout << "z.Length() = " << z.Length() << std::endl;
  std::cout << std::endl;

  // concat operators -----------------
  tString w("Test");
  std::cout << "w = <" << w << ">" << std::endl;

  std::cout << "w + 'a' = <" << w + 'a' << ">" << std::endl;

  std::cout << "w + w + w = <" << w + w + w << ">" << std::endl;

  y = w;
  y += 'a';
  y += w;
  std::cout << "w + 'a' + w + w = <" << y + w << ">" << std::endl;

  std::cout << std::endl;

  // comparisons ----------------------
  tString t("abc"), u("abc"), v("xyz");
  std::cout << "t = <" << t << ">";
  std::cout << ", u = <" << u << ">";
  std::cout << ", v = <" << v << ">" << std::endl;

  std::cout <<   "t == u: " << (t == u);
  std::cout << "   t <= u: " << (t <= u);
  std::cout << "   t <  u: " << (t <  u);
  std::cout << "   t >  u: " << (t >  u);
  std::cout << "   t != u: " << (t != u);
  std::cout << "   t >= u: " << (t >= u) << std::endl;

  std::cout <<   "u == v: " << (u == v);
  std::cout << "   u <= v: " << (u <= v);
  std::cout << "   u <  v: " << (u <  v);
  std::cout << "   u >  v: " << (u >  v);
  std::cout << "   u != v: " << (u != v);
  std::cout << "   u >= v: " << (u >= v) << std::endl;

  std::cout << std::endl;

  // test exception mechanism - cause overflow in constructor
  try
  {
    tString(4000);
  }
  catch(xString& x)
  {
    DisplayUser(x, std::cerr);
  }

  // test exception mechanism - cause overflow in concat operation
  tString overflow("a");
  int i;
  try
  {
    tString temp;
    for (i = 0; i < 100; i++)
    {
      temp = overflow;
      overflow += temp;
    }
    std::cout << "No exception. i = " << i << std::endl;
  }
  catch(xString& x)
  {
    DisplayVerbose(x, std::cerr);
    std::cerr << "i = " << i << std::endl;
  }
  return 0;
}

#endif

