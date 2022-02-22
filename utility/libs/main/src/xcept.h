#ifndef _XCEPT_H_
#define _XCEPT_H_

#include "machine.h"
using namespace std;
#include <string.h>
#include <sstream>

class xCept
{

public:
  enum eSeverity
  {
    sevFatal,
    sevNonFatal
  };

  enum eMode
  {
    modeVerbose,
    modeUser
  };
  xCept(const xCept &x);
  virtual ~xCept() {}
  const char *FileName() const { return fname; }
  int Line() const { return line; }
  const char *Name() const { return name; }
  int Error() const { return error; }
  eSeverity Severity() const { return severity; }
  //pchar ErrorStr()     {return errorStr;}
  char *ErrorStr()
  {
    memset(__buffer, 0, sizeof(__buffer));
    strncpy(__buffer, osErr.str().c_str(), sizeof(__buffer) - 1);
    int len = (int)strlen(__buffer);
    strncat(__buffer, errorStr, sizeof(__buffer) - len - 1);
    __buffer[sizeof(__buffer) - 1] = 0;
    return __buffer;
  }
  void Display(std::ostream &os, const eMode mode = modeVerbose);

protected:
  enum
  {
    ErrorStrLen = 4096,
    NameLen = 20,
    FileLen = 256
  };

  char name[NameLen];  // name of exception class
  int error;           // error no. within exception class
  char fname[FileLen]; // file name
  int line;            // line number
  eSeverity severity;
  char errorStr[ErrorStrLen]; // error string - contains all relevant info

  xCept(const char *aFname,
        const int aLine,
        const char *aName,
        const int aError,
        const eSeverity aSeverity = sevNonFatal);
  std::stringstream osErr;

private:
  char __buffer[ErrorStrLen];
};

class xBase : public xCept
{
public:
  xBase(const char *file, const int line, const char *fmt, ...);
  xBase(const xBase &x)
      : xCept(x) {}
};

/*-----------------------------------
   xDBException
-----------------------------------*/
class xDBException : public xCept
{
public:
  xDBException(const char *file, const int line, const char *typeOf, int errorNo, char *error)
      : xCept(file, line, typeOf, errorNo)
  {
    osErr << "Database Error: " << std::endl
          << error << std::endl
          << std::ends;
  }
  xDBException(const xDBException &copy)
      : xCept(copy)
  {
  }
};

class xIdlException : public xCept
{
public:
  xIdlException(const char *file, const int line, const char *typeOf, int errorNo, const char *error)
      : xCept(file, line, typeOf, errorNo)
  {
    osErr << "RPC IDL Error: " << std::endl
          << error << std::endl
          << std::ends;
  }
  xIdlException(const xIdlException &copy)
      : xCept(copy)
  {
  }
};

/*-----------------------------------
   Marcos for backwards compatibility
-----------------------------------*/
#define DisplayVerbose(x, os)                              \
  do                                                       \
  {                                                        \
    os << "Caught exception at "                           \
       << __FILE__ << "(" << __LINE__ << ")" << std::endl; \
    x.Display(os);                                         \
  } while (0)

#define UndefineDisplayVerbose(x, os)                      \
  do                                                       \
  {                                                        \
    os << "Caught Undefine exception at "                  \
       << __FILE__ << "(" << __LINE__ << ")" << std::endl; \
  } while (0)

#define DisplayUser(x, os)          \
  do                                \
  {                                 \
    x.Display(os, xCept::modeUser); \
  } while (0)

// logging to syslog on Unix

#ifdef M_AIX

#define LogVerbose(x, severity, logstream)                   \
  do                                                         \
  {                                                          \
    logstream << severity << "Caught exception at "          \
              << __FILE__ << "(" << __LINE__ << ")" << '\n'; \
    x.Display(logstream);                                    \
  } while (0)

#define LogUser(x, severity, logstream)    \
  do                                       \
  {                                        \
    logstream << severity;                 \
    x.Display(logstream, xCept::modeUser); \
  } while (0)

#else

#define LogVerbose(x, severity, logstream)
#define LogUser(x, severity, logstream)

#endif

#endif
