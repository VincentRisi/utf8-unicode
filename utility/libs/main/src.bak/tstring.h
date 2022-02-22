// tstring.h
// class xString
// class tString
//test
#ifndef _TSTRING_H_
#define _TSTRING_H_

#include "machine.h"
#include "xstring.h"
#include "xcept.h"

//-------------------------------------------------------------
//  xString class
//-------------------------------------------------------------

//CLASSDEF(String);
class xString : public xCept
{
public:
  enum eError { errOverflow = 1 };
  xString(pchar aFname, int aLine, eError aError);
  xString(const xString& x);

private:
  // following function not defined - error if used
  xString& operator=(const xString&);
};

inline xString::xString(const xString& x) : xCept(x) {}

#define XString(err) xString(__FILE__, __LINE__, xString::err)

//-------------------------------------------------------------
//  tString class
//-------------------------------------------------------------

//CLASSDEF(String);
class tString
{
public:
  enum CompMode { cmSensitive, cmIgnore };

  tString();                               // default constructor
  tString(const tString& str);             // copy constructor
  tString(const pchar cstr);               // cstring constructor
  tString(const pchar anstr, size_t asize);// fixed size constructor
  tString(char c);                         // single-char constructor
  tString(size_t count, char fill = ' ');  // multiple-char constructor
  ~tString();                              // destructor

  void SetCompMode(CompMode aCompMode);    // change compare mode
  size_t Length() const;                   // string length
  operator const pchar() const;            // conversion to const pchar op.
  tString& operator=(const tString& str);  // assignment operator
  tString& operator=(const pchar cstr);    // assignment operator
  char* c_str();
  char operator[](size_t pos) const;       // operator[]
  void ToUpper();                          // convert self to upper case
  void ToLower();                          // convert self to lower case
  tString AsUpper() const;                 // return copy as upper case
  tString AsLower() const;                 // return copy as lower case

  // concatenation operators
  friend tString operator+(const tString& str1, const tString& str2);
  void operator+=(const tString& str);
  friend tString operator+(const tString& str1, char ch);
  void operator+=(char ch);

  // comparison methods
  int operator< (const tString& str) const;
  int operator> (const tString& str) const;
  int operator<=(const tString& str) const;
  int operator>=(const tString& str) const;
  int operator==(const tString& str) const;
  int operator!=(const tString& str) const;

  // stream output
  friend ostream& operator<<(ostream& strm, const tString& str);

private:
  enum { allocIncr = 8, maxStrlen = 131072};
  static size_t CalcSize(size_t needed);  // calculate allocation size
  typedef unsigned long ulong;

  CompMode compMode;
  size_t siz; // allocated size
  size_t len; // current length
  pchar p;    // pointer to text
  int Compare(const tString& str) const;

  // special constructors to speed operator+ ops
  tString(const tString& str1, const tString& str2);
  tString(const tString& str1, const char ch);
};

// calculate the allocation size for a string
inline size_t tString::CalcSize(size_t needed)
{
  return ((needed + allocIncr) / allocIncr) * allocIncr;
}

inline void tString::SetCompMode(CompMode aCompMode)
{
  compMode = aCompMode;
}

inline size_t tString::Length() const
{
  return len;
}

inline tString::operator const pchar() const
{
  return p;
}

inline char tString::operator[](size_t pos) const
{
  return (pos >= len) ? char(0) : p[pos];
}

inline void tString::ToUpper()
{
  if (p != 0)
    strupr(p);
}

inline void tString::ToLower()
{
  if (p != 0)
    strlwr(p);
}

inline char* tString::c_str()
{
  return p;
}

inline tString operator+(const tString& str1, const tString& str2)
{
  return tString(str1, str2);
}

inline void tString::operator+=(const tString& str)
{
  *this = *this + str;
}

inline tString operator+(const tString& str, char ch)
{
  return tString(str, ch);
}

inline void tString::operator+=(char ch)
{
  *this = *this + ch;
}

// comparison methods ----------------------------------
inline int tString::operator<(const tString& str) const
{
  return (Compare(str) < 0);
}

inline int tString::operator>(const tString& str) const
{
  return (Compare(str) > 0);
}

inline int tString::operator<=(const tString& str) const
{
  return (Compare(str) <= 0);
}

inline int tString::operator>=(const tString& str) const
{
  return (Compare(str) >= 0);
}

inline int tString::operator==(const tString& str) const
{
  return (Compare(str) == 0);
}

inline int tString::operator!=(const tString& str) const
{
  return (Compare(str) != 0);
}


inline ostream& operator<<(ostream& os, const tString& str)
{
  os << str.p;
  return os;
}

#endif

