#ifndef _BYTEBUFFER_H_
#define _BYTEBUFFER_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_BYTEBUFFER_H_);

/*
 *  The purpose for this class is to supply a memory allocation mechanism
 *  that will deallocate when going out of scope. It is similar to
 *  tbuffer.h (which is used for zero terminated vectors of scalars)
 */

#include "ti.h"
#include "xcept.h"
#ifdef size
  #undef size
#endif

static pchar ByteBufferErrors[] =
{ "OK ???"
  , "Memory Allocation Failed"
};

XCLASSDEF(ByteBufferCept);
class xByteBufferCept : public xCept
{
public:
  enum eError
  {
    eOk
    , eMemoryError
  };
  xByteBufferCept(pchar aFname, int aLine, eError aError)
  : xCept(aFname, aLine, "xBufferCept", aError)
  {
    osErr << "ByteBuffer: " << ByteBufferErrors[aError] <<  endl;
    osErr <<  ends;
  }
  xByteBufferCept(const xByteBufferCept& aX)
  : xCept(aX)
  {
  }
};

#define XByteBufferCept(err) xByteBufferCept(__FILE__, __LINE__, xByteBufferCept::err)

struct ByteBuffer
{
  unsigned char* data; // data buffer
  size_t capacity;     // current capacity
  size_t length;       // current length used
  ByteBuffer(size_t insize=512)
  : capacity(insize)
  , length(0)
  {
    if (capacity <= 0) capacity = 512;
    data = (unsigned char *)calloc(capacity, sizeof(unsigned char));
    if (data == 0)
      throw XByteBufferCept(eMemoryError);
  }
  ~ByteBuffer()
  {
    if (data)
      free(data);
  }
  operator unsigned char*()
  {
    return data;
  }
  virtual void clear()
  {
    memset(data, 0, capacity);
    length = 0;
  }
  virtual void resize(size_t newCapacity)
  {
    if (newCapacity > capacity)
      grow(newCapacity - capacity);
  }
  virtual void sizeto(size_t newCapacity)
  {
    capacity = newCapacity;
    data = (unsigned char *)realloc(data, capacity);
    if (capacity != 0 && data == 0)
      throw XByteBufferCept(eMemoryError);
    if (length > capacity)
      length = capacity;
  }
  virtual void grow(size_t additional)
  {
    size_t newCapacity = capacity + additional;
    data = (unsigned char *)realloc(data, newCapacity);
    if (data == 0)
      throw XByteBufferCept(eMemoryError);
    unsigned char *b = data + capacity;
    capacity = newCapacity;
    memset(b, 0, additional*sizeof(unsigned char));
  }
  virtual void set(void* inputData, size_t inputLength)
  {
    clear();
    append(inputData, inputLength);
  }
  virtual void append(void* inputData, size_t inputLength)
  {
    resize(length + inputLength);
    memcpy(data+length, inputData, inputLength);
    length = length + inputLength;
  }
};

#endif

