#ifndef _TSTREAMS_H_
#define _TSTREAMS_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_TSTREAMS_H_);

#include "ti.h"
#include "xcept.h"
#include "tbuffer.h"

static pchar StreamErrors[] =
{ "OK ???"
, "File Open Error"
, "File Buffer Error"
};

XCLASSDEF(StreamCept);
class xStreamCept : public xCept
{
public:
  enum eError
  { eOk
  , eFileOpenError
  , eFileBufferError
  };
  xStreamCept(pchar aFname, int aLine, eError aError)
  : xCept(aFname, aLine, "xStreamCept", aError)
  {
    osErr << "TStream: " << StreamErrors[aError] <<  endl;
    osErr <<  ends;
  }
  xStreamCept(const xStreamCept& aX)
  : xCept(aX) {}
};
#define XStreamCept(err) xStreamCept(__FILE__, __LINE__, xStreamCept::err)

/**
 * Simple wrapper for File Stream.
 * -------------------------------
 * I like these as the file is opened on construction and closed on deconstruction
 * which means I dont have to worry about it, when there are exceptions thrown.
 * The resources are cleaned up automagically.
 */
class TStream
{
  FILE* file;
public:
  TStream(pchar FileName, pchar Mode, long BufferSize=8192)
  {
    file = fopen(FileName, Mode);
    if (file == 0)
      throw XStreamCept(eFileOpenError);
    if (setvbuf(file, 0, _IOFBF, BufferSize) != 0)
      throw XStreamCept(eFileBufferError);
  }
  ~TStream()
  {
    if (file != 0)
      fclose(file);
  }
  operator FILE*() {return file;}
};

#endif
