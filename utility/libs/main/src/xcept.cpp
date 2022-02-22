#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "ti.h"
#include "xcept.h"
#include "xstring.h"

#ifdef M_STDLIB
using namespace std;
#endif

xCept::xCept(const char       *aFname,
             const int         aLine,
             const char       *aName,
             const int         aError,
             const eSeverity   aSeverity)
      :line(aLine)
,error(aError)
,severity(aSeverity)
,osErr("")
{
   osErr.clear();
   errorStr[0] = 0;
   __buffer[0] = 0;
   name[0] = 0;
   memset(errorStr, 0, sizeof(errorStr));
   strncpyz(fname, aFname, sizeof(fname) - 1);
   strncpyz(name,  aName,  sizeof(name) - 1);
}

xCept::xCept(const xCept& x)
      :line(x.line)
,error(x.error)
,severity(x.severity)
,osErr(x.osErr.str())
{
   osErr.clear();
   __buffer[0] = 0;
   name[0] = 0;
   memset(errorStr, 0, sizeof(errorStr));;
   strcpy(fname,    x.fname);
   strcpy(name,     x.name);
}

void xCept::Display(std::ostream& os, const eMode mode)
{
   if (mode == modeVerbose)
   {
      if (severity == sevFatal)
         os << "Fatal Error ";
      else
         os << "Error ";
      os << name << " in "<< fname << "(" << line << ") " << error << ": ";
   }
   os << ErrorStr() << std::endl << ends;
}


/*------------------------------------------------------------------------------
   xBase Constructor
------------------------------------------------------------------------------*/
xBase::xBase(const char *file, const int line, const char *fmt, ...)
      :xCept(file, line, "xBase", -1)
{
   va_list ap;
   char    jotter[1024];
   va_start(ap, fmt);
   vsnprintf(jotter, sizeof(jotter) - 1, fmt, ap);
   va_end(ap);
   osErr << jotter << std::endl << ends;
}



