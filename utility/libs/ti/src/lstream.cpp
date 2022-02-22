#include "versions.h"
#define _VERSION_NO "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
COMPILED_VERSION(_VERSION_NO);

// Author         :  S M Sena
// Description    :  class tLogstream: stream wrapper for the UNIX syslog API
//                   this is not an extensive implementation
// Date Created   :  29-AUG-1994
// Subsystem      :  General Purpose
// Filename       :  logstream.c

#include "ti.h"

#if defined(M_AIX) || defined(M_GNU)
#include "lstream.h"
#include "xstring.h"

#ifdef M_STDLIB
using namespace std;
#endif

tLogstream::tLogstream(char* aFacility, ostream* aTrace)
{
   m_facility = new char[strlen(aFacility) + 1];   // facility only stored for Trace
   strcpy(m_facility, aFacility);
   m_priority=LOG_DEBUG;
#if defined(M_STDLIB)
   m_pstr = new stringstream;
#else
   m_pstr = new strstream;
#endif
   m_pofs = aTrace;
   openlog(aFacility, LOG_PID, LOG_LOCAL0);
}


tLogstream & endl(tLogstream & ls)
{
	*(ls.m_pstr) << endl;
	return ls;
}

tLogstream &flush(tLogstream &ls)
{
  *(ls.m_pstr) << ends;


  if (ls.m_pofs)
  {
    *(ls.m_pofs) << setw(8) << ls.m_facility << " " << setw(10);

    // priority to stream
    switch (ls.m_priority)
    {
    case LOG_DEBUG:
      *(ls.m_pofs) << "DEBUG";
     break;
    case LOG_ALERT:
      *(ls.m_pofs) <<  "ALERT";
     break;
    case LOG_CRIT:
      *(ls.m_pofs) <<  "CRIT";
     break;
    case LOG_EMERG:
      *(ls.m_pofs) <<  "EMERGENCY";
     break;
    case LOG_ERR:
      *(ls.m_pofs) <<  "ERROR";
     break;
    case LOG_INFO:
      *(ls.m_pofs) <<  "INFO";
     break;
    case LOG_NOTICE:
      *(ls.m_pofs) <<  "NOTE";
     break;
    case LOG_WARNING:
      *(ls.m_pofs) <<  "WARNING";
     break;
    }

    // text to stream
    *(ls.m_pofs) << " " << ls.m_pstr->str() << endl;


  }
  // stream to syslog function
#if defined(M_STDLIB)
  syslog(ls.m_priority, ls.m_pstr->str().c_str());
#else
  syslog(ls.m_priority, ls.m_pstr->str());
#endif
  delete ls.m_pstr;
#if defined(M_STDLIB)
  ls.m_pstr = new stringstream;
#else
  ls.m_pstr = new strstream;
#endif

  return ls;
}

#ifdef LSTREAM_TST

void main()
{
  tLogstream a("LogTest");
  a << warn <<   "string" << 'c' << " " << 45 << flush;
  a << notice  << 234.234 << flush;
  a << notice  << 234.234 << flush;

  ofstream trc("suri");
  tLogstream b("local", &trc);
  b << warn << "this should go to my local file " << 1 << flush;
  b << notice << "this should go to my local file " << 2 << flush;
  b << crit << "this should go to my local file " << 3 << flush;
  b << emerg << "this should go to my local file " << 4 << flush;
  b << alert << "this should go to my local file " << 5 << flush;
  b << err << "this should go to my local file "<< 6 << flush;
  b << debug << "this should go to my local file "<< 7 << flush;

  tLogstream c("local", &cout);
  c << debug << "this will go to the screen " << 22 << flush;
}

#endif

#endif
