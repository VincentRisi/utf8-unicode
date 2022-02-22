#ifndef __UBI_DATA_H_
#define __UBI_DATA_H_

/**
 * UbiData
 */
class UbiData
{
public:
  /**
  * UbiData Constructor - default
  */
  UbiData();
  /**
  * UbiData Constructor for a preallocated size
  */
  UbiData(const int32 size);
  /**
  * UbiData Constructor using existing data for size.
  * Destructor will free this data so be careful.
  * Also use 'new' for allocation of data.
  */
  UbiData(unsigned char * data, const int32 size);
  /**
  * UbiData Destructor : clears data
  */
  virtual ~UbiData();
  /**
  * Allocate new data - freeing up previous
  */
  void Allocate(const int32 size);
  /**
  * Use existing data : similar to constructor : will free up previous data
  * Destructor will free this data so be careful.
  * Also use 'new' for allocation of data.
  * Will clear return code and reqid, supply these as parameters if you
  * want to keep them.
  */
  void Use(unsigned char * data, const int32 size, int32 aReturnCode=0, int32 aReqID=0);
  /**
  * Clears the memory as well as the return code and id.
  */
  void Clear();
  unsigned char *data;
  int32 dataLen;
  int32 returnCode;
  int32 reqID;
};

#endif
