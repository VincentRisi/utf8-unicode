#include "machine.h"
#include <string.h>
#include "ubidata.h"
#include "basemacros.h"
#include "ubixcept.h"

UbiData::UbiData()
{
  data = 0;
  dataLen = 0;
  returnCode = 0;
  reqID = 0;
}

UbiData::UbiData(const int32 size)
{
  data = 0;
  dataLen = 0;
  returnCode = 0;
  reqID = 0;
  Allocate(size);
}

UbiData::UbiData(unsigned char * aData, const int32 size)
{
  data = aData;
  dataLen = size;
  returnCode = 0;
  reqID = 0;
  Allocate(size);
}

UbiData::~UbiData()
{
  Clear();
}

void UbiData::Allocate(const int32 size)
{
  if (data) Clear();
  try
  {
  data = new unsigned char[size];
  memset(data, 0, size);
  dataLen = size;
  }
  catch (...)
  {
    throw TUbiException(TUbiException::MEMORY_ALLOCATION_FAILED, "UbiData::Allocate Memory allocation failed", __FILE__, __LINE__); 
  }
}

void UbiData::Use(unsigned char * aData, const int32 aDataLen, int32 aReturnCode, int32 aReqID)
{
  Clear();
  data = aData;
  dataLen = aDataLen;
  returnCode = aReturnCode;
  reqID = aReqID;
}

void UbiData::Clear()
{
  if (data) delete [] data; 
  data = 0;
  dataLen    = 0;
  returnCode = 0;
  reqID      = 0;
}
