// Filename       :  lstream.h
// Author         :  S M Sena
// Description    :  class tLogstream: wrapper for UNIX syslog API
// Date Created   :  29-Aug-1994
// Subsystem      :  General

#ifndef __LOGSTREAM__
#define __LOGSTREAM__ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(__LOGSTREAM__);

#include "ti.h"
#include <syslog.h>

//#include <string.h>

CLASSDEF(Logstream);

class tLogstream
{
public:
  tLogstream(char* Facility,  std::ostream *localtrace = 0);
  ~tLogstream();

  // operator << overloads
  tLogstream & operator<< (char);
  tLogstream & operator<< (signed char);
  tLogstream & operator<< (unsigned char);

  tLogstream & operator<< (short);
  tLogstream & operator<< (unsigned short);

  tLogstream & operator<< (int);
  tLogstream & operator<< (unsigned int);

  tLogstream & operator<< (long);
  tLogstream & operator<< (unsigned long);

  tLogstream & operator<< (float);
  tLogstream & operator<< (double);
  tLogstream & operator<< (long double);

  tLogstream & operator<< (const unsigned char *);
  tLogstream & operator<< (const signed char *);
  tLogstream & operator<< (const char *);

  tLogstream & operator<< (const void *);

  tLogstream & operator<< (tLogstream &);

  // manipulators
  friend tLogstream & endl(tLogstream &);
  friend tLogstream & flush(tLogstream &);  // flush stream
  friend tLogstream & debug(tLogstream &);  // set error message priority
  friend tLogstream & alert(tLogstream &);  // set error message priority
  friend tLogstream & crit(tLogstream &);   // set error message priority
  friend tLogstream & emerg(tLogstream &);  // set error message priority
  friend tLogstream & err(tLogstream &);    // set error message priority
  friend tLogstream & info(tLogstream &);   // set error message priority
  friend tLogstream & notice(tLogstream &); // set error message priority
  friend tLogstream & warn(tLogstream &);   // set error message priority
  

private:
  int       m_priority;  // error serverity level
  char      *m_facility; // facility name i.e. server name
#ifdef M_STDLIB
  std::stringstream *m_pstr;
  std::ostream   *m_pofs;     // output file stream for local debug
#else
  strstream *m_pstr;     // string stream for message buffer
  ostream   *m_pofs;     // output file stream for local debug
#endif
};

inline tLogstream::~tLogstream()
{
  closelog();
  delete[] m_facility;
  delete m_pstr;
}

// operator<< overloads --------------------------
#ifdef M_GNU
#define DEFINE_OP(type)                           \
inline tLogstream & tLogstream::operator<< (type s) \
{ *m_pstr << s; return *this; }
#else
  #define DEFINE_OP(type)                           \
  inline tLogstream & tLogstream::operator<< (type s) \
  { *m_pstr << s; return *this; }
#endif
DEFINE_OP(char)
DEFINE_OP(signed char)
DEFINE_OP(unsigned char)
DEFINE_OP(short)
DEFINE_OP(unsigned short)
DEFINE_OP(int)
DEFINE_OP(unsigned int)
DEFINE_OP(long)
DEFINE_OP(unsigned long)
DEFINE_OP(float)
DEFINE_OP(double)
DEFINE_OP(long double)
DEFINE_OP(const unsigned char*)
DEFINE_OP(const signed char*)
DEFINE_OP(const char*)
DEFINE_OP(const void*)


inline tLogstream & tLogstream::operator<< (tLogstream &f)
{
  return *this;
}


// priority manipulators ------------

#define DEFINE_PRIORITY(manipulator, priority) \
inline tLogstream & manipulator(tLogstream & s)    \
{ s.m_priority = priority; return s; }

DEFINE_PRIORITY(debug,  LOG_DEBUG)
DEFINE_PRIORITY(alert,  LOG_ALERT)
DEFINE_PRIORITY(crit,   LOG_CRIT)
DEFINE_PRIORITY(emerg,  LOG_EMERG)
DEFINE_PRIORITY(err,    LOG_ERR)
DEFINE_PRIORITY(info,   LOG_INFO)
DEFINE_PRIORITY(notice, LOG_NOTICE)
DEFINE_PRIORITY(warn,   LOG_WARNING)

#endif

