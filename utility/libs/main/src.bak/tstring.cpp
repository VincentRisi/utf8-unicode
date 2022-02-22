// tstring.c
// Author: Barry Fortune
// Date: 1 May 1994
// class xString -- string exception class
// class tString -- string class

#include <ctype.h>

#include "machine.h"
#include "tstring.h"

//-------------------------------------------------------------
//  xString class
//-------------------------------------------------------------

#define xStringName "xString"

xString::xString(pchar aFname, int aLine, eError aError) :
  xCept(aFname, aLine, xStringName, aError)
{
  switch (aError)
  {
  case errOverflow:
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
tString::tString(const pchar cstr) :
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
tString::tString(const pchar anstr, size_t asize)
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

tString& tString::operator=(const pchar cstr)
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
  cout << "x           = <" << x << ">" << endl;

  cout << "x.AsLower() = <" << x.AsLower() << ">" << endl;

  x.ToUpper();
  cout << "x.ToUpper() : <" << x << ">" << endl;

  cout << "x[1] = '" << x[1] << "'" << endl;

  // empty string, single character, repeated character
  tString y;
  tString single('1');
  tString z(4, 'z');
  cout << "y = <" << y << ">, single = <" << single
       << ">, z = <" << z << ">" << endl;

  // conversion, length
  cout << "pchar(z) = <" << pchar(z) << ">" << endl;
  cout << "z.Length() = " << z.Length() << endl;
  cout << endl;

  // concat operators -----------------
  tString w("Test");
  cout << "w = <" << w << ">" << endl;

  cout << "w + 'a' = <" << w + 'a' << ">" << endl;

  cout << "w + w + w = <" << w + w + w << ">" << endl;

  y = w;
  y += 'a';
  y += w;
  cout << "w + 'a' + w + w = <" << y + w << ">" << endl;

  cout << endl;

  // comparisons ----------------------
  tString t("abc"), u("abc"), v("xyz");
  cout << "t = <" << t << ">";
  cout << ", u = <" << u << ">";
  cout << ", v = <" << v << ">" << endl;

  cout <<   "t == u: " << (t == u);
  cout << "   t <= u: " << (t <= u);
  cout << "   t <  u: " << (t <  u);
  cout << "   t >  u: " << (t >  u);
  cout << "   t != u: " << (t != u);
  cout << "   t >= u: " << (t >= u) << endl;

  cout <<   "u == v: " << (u == v);
  cout << "   u <= v: " << (u <= v);
  cout << "   u <  v: " << (u <  v);
  cout << "   u >  v: " << (u >  v);
  cout << "   u != v: " << (u != v);
  cout << "   u >= v: " << (u >= v) << endl;

  cout << endl;

  // test exception mechanism - cause overflow in constructor
  try
  {
    tString(4000);
  }
  catch(xString& x)
  {
    DisplayUser(x, cerr);
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
    cout << "No exception. i = " << i << endl;
  }
  catch(xString& x)
  {
    DisplayVerbose(x, cerr);
    cerr << "i = " << i << endl;
  }
  return 0;
}

#endif

