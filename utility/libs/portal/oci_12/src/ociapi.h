#ifndef ociapiH
#define ociapiH

#include "ocimachine.h"
#include "swapbytes.h"

struct TJDate
{
  char* date;
  TJDate(char* aDate)
  {
    date = aDate;
  }
};

struct TJTime
{
  char* time;
  TJTime(char* aTime)
  {
    time = aTime;
  }
};

struct TJDateTime
{
  char* dateTime;
  TJDateTime(char* aDateTime)
  {
    dateTime = aDateTime;
  }
};

struct TJOCIDate
{
  unsigned char cc, yy, mm, dd, hh, mi, ss;
  unsigned char filler;
};

template <int32 size>
class TJBlob
{
  unsigned char _data[size + 4];
public:
  int32 len() { return *(int32*)_data; }
  unsigned char* data() { return _data + 4; }
  void setData(int32 len, unsigned char* data)
  {
    memset(_data, 0, size + 4);
    *(int32*)_data = len;
    memcpy(_data + 4, data, len);
  }
  unsigned char* getData() { return _data; }
  void Swaps()
  {
#ifdef swapbytesH
    int32 _len = *(int32*)_data;
    SwapBytes(_len);
#endif
  }
};

#if 0
template <int32 size>
class TJUnicode
{
  unsigned char _data[size + 4];
public:
  int16 len() { return *(int16*)_data - 4; }
  unsigned char* BOM() { return _data + 2; }
  unsigned char* data() { return _data + 4; }
  void setData(int16 len, unsigned char* data, unsigned char *BOM = 0)
  {
    // verify len <= size
    memset(_data, 0, size + 4);
    *(int16*)_data = len + 4;
    if (BOM == 0)
    {
      _data[2] = 0xFE;
      _data[3] = 0xFF;
    }
    else
    {
      _data[2] = BOM[0];
      _data[3] = BOM[1];
    }
    memcpy(_data + 4, data, len - 4);
  }
  unsigned char* getData() { return _data; }
  void Swaps()
  {
#ifdef swapbytesH
    int32 _len = *(int32*)_data;
    SwapBytes(_len);
#endif
  }
};
#endif

struct TJConnector
{
  int32       signature;
  int32        result;
  text       msgbuf[2048];
  int32        error;
  sb4        ociErrCode;
  char* user;
  char* server;
  time_t     serverTime;
  time_t     systemTime;
  OCIEnv* ociEnv;
  OCIError* ociError;
  OCISvcCtx* ociSvcCtx;
  int32        LoggedOn;
  TJConnector()
  {
    memset(this, 0, sizeof(*this));
    signature = 0xFACE0FF0;
  }
  ~TJConnector()
  {
    if (ociSvcCtx)
      Logoff();
  }
  void Logon(const char* aUser, const char* aPassword, const char* aServer);
  void Logoff();
  void Commit();
  void Rollback();
  void TimeStamp(char* timeStamp);
  void Error(char* Msg, int32 MsgLen);
  void ThrowOnError(char* file = 0, int32 line = 0);
};

struct TJQuery
{
  int32         signature;
  int32         noBinds;
  int32         noDefines;
  int32         noRows;
  int32         sizeRow;
  int32         noIndicators;
  int32         rowsIndex;
  int32         rowsDone;
  int32         rowsRead;
  int32         rowsWritten;
  int32         doCommit;
  int32         isOpen;
  int32         error;
  char* command;
  char* data;
  int16* indicators;
  char        file[65];
  int32         line;
  void FileAndLine(char* aFile, int32 aLine)
  {
    strncpy(file, aFile, sizeof(file) - 1);
    line = aLine;
  }
  TJConnector& conn;
  OCIBind** ociBinds;
  OCIDefine** ociDefines;
  OCIStmt* ociStmt;
  TJQuery(TJConnector& aConnector) : conn(aConnector)
  {
    signature = 0xFACE0FF1;
    noBinds = 0;
    noDefines = 0;
    noRows = 0;
    sizeRow = 0;
    noIndicators = 0;
    rowsIndex = 0;
    rowsDone = 0;
    rowsRead = 0;
    rowsWritten = 0;
    doCommit = 0;
    isOpen = 0;
    error = 0;
    command = 0;
    data = 0;
    indicators = 0;
    ociBinds = 0;
    ociDefines = 0;
    ociStmt = 0;
    file[sizeof(file) - 1] = 0; // just in case
  }
  ~TJQuery();
  void  SetCommit(int32 commit)
  {
    doCommit = commit;
  }
  void  Open(const char* query, int32 noBinds = 0, int32 noDefines = 0, int32 noRows = 0, int32 sizeRow = 0);
  void  OpenArray(const char* query, int32 noBinds, int32 noRows, int32 sizeRow);

  void  Bind(const char* name, int32 bindNo, char* data, int32 datalen, int16* null = 0, int32 ansichar = 0);
  void  BindUtf8(const char* name, int32 bindNo, unsigned char* data, int32 datalen, int16* null = 0);
  void  BindUnicode(const char* name, int32 bindNo, unsigned char* data, int32 size, int16* null = 0);
  void  BindBlob(const char* name, int32 bindNo, unsigned char* data, int32 dataSize);
  void  Bind(const char* name, int32 bindNo, int16& data, int16* null = 0);
  void  Bind(const char* name, int32 bindNo, int32& data, int16* null = 0);
  void  Bind(const char* name, int32 bindNo, int64& data, int16* null = 0);
  void  Bind(const char* name, int32 bindNo, double& data, int16* null = 0);
  void  Bind(const char* name, int32 bindNo, TJOCIDate& data, int16* null = 0);

  void  BindArray(const char* name, int32 bindNo, char* data, int32 datalen, int32 ansichar = 0);
  void  BindArray(const char* name, int32 bindNo, int32* data);
  void  BindArray(const char* name, int32 bindNo, int64* data);
  void  BindArray(const char* name, int32 bindNo, int16* data);
  void  BindArray(const char* name, int32 bindNo, double* data);
  void  BindArray(const char* name, int32 bindNo, TJOCIDate* data);

  void  Define(int32 defineNo, char* data, int32 dataLen, int32 ansichar = 0);
  void  DefineUtf8(int32 defineNo, unsigned char* data, int32 dataLen);
  void  DefineUnicode(int32 defineNo, unsigned char* data, int32 dataLen);
  void  Define(int32 defineNo, int16* data);
  void  Define(int32 defineNo, int32* data);
  void  Define(int32 defineNo, int64* data);
  void  Define(int32 defineNo, double* data);
  void  Define(int32 defineNo, TJOCIDate* data);
  void  DefineBlob(int32 defineNo, char* data, int32 dataSize);

  void  Exec();
  bool  Fetch();
  void  Deliver(int32 doit);
  void  GetNoNull(char* into, char* from, int32 dataLen);
  void  Get(char* into, char* from, int32 dataLen);
  void  GetUtf8(unsigned char* into, unsigned char* from, int32 dataLen);
  void  GetUnicode(unsigned char* into, unsigned char* from, int32 dataLen);
  void  GetBlob(unsigned char* into, unsigned char* from, int32 dataLen);
  void  Get(int16& into, char* from);
  void  Get(int32& into, char* from);
  void  Get(int64& into, char* from);
  void  Get(double& into, char* from);
  void  Get(TJDate into, char* from);
  void  Get(TJTime into, char* from);
  void  Get(TJDateTime into, char* from);
  void  GetNull(int16& into, int32 defineNo);
  void  Put(char* into, char* from, int32 dataLen);
  void  Put(char* into, unsigned char* from, int32 dataLen);
  void  Put(char* into, int16& from);
  void  Put(char* into, int32& from);
  void  Put(char* into, int64& from);
  void  Put(char* into, double& from);
  void  Put(char* into, TJDate from);
  void  Put(char* into, TJTime from);
  void  Put(char* into, TJDateTime from);
  void  PutNull(int16& from, int32 bindNo);
  void  Close();
  void  Clear();
  virtual int32& Sequence(int32& sequence, char* sequencer);
  virtual int64& Sequence(int64& sequence, char* sequencer);
  virtual char* UserStamp(char* userStamp);
  virtual TJOCIDate& Date(TJOCIDate& ocidate, char* date);
  virtual TJOCIDate& Time(TJOCIDate& ocidate, char* time);
  virtual TJOCIDate& DateTime(TJOCIDate& ocidate, char* dateTime);
  virtual TJOCIDate& TimeStamp(TJOCIDate& ocidate, char* timeStamp);
};

class TJAlloc
{
  static bool Failed;
public:
  static bool NoMemory()
  {
    return Failed;
  }
  static char* newChar(int32 size)
  {
    char* result = new char[size];
    Failed = (result == 0);
    if (result != 0)
      memset(result, 0, size);
    else
    {
      char work[256];
      snprintf(work, sizeof(work), "TJAlloc::newChar(%d) Memory allocation Error", size);
    }
    return result;
  }
  static int32* newInt(int32 size)
  {
    int32* result = new int32[size];
    Failed = (result == 0);
    if (result != 0)
      memset(result, 0, size);
    else
    {
      char work[256];
      snprintf(work, sizeof(work), "TJAlloc::newInt(%d) Memory allocation Error", size);
    }
    return result;
  }
  static char* dupChar(const char* value)
  {
    char* result = new char[strlen(value) + 1];
    if (result != 0)
      strcpy(result, value);
    return result;
  }
  static void deleteChar(char*& data)
  {
    if (data != 0)
    {
      delete[] data;
      data = 0;
    }
  }
  static void deleteInt(int32*& data)
  {
    if (data != 0)
    {
      delete[] data;
      data = 0;
    }
  }
};

struct TJCppRet
{
  char* head;
  char* output;
  char* sequence;
  char* tail;
  bool useField;
  bool useSequencer;
  bool hasTrigger;
  TJCppRet()
  {
    head = "";
    output = "";
    sequence = "";
    tail = "";
    useField = true;
    useSequencer = false;
    hasTrigger = false;
  }
  size_t TJCppRet::setup(const char* serverName, const char* tableName, const char* procName, const char* fieldName, bool hasReturning);
  const char* checkUse(const char* fieldName)
  {
    if (useField)
      return fieldName;
    return "";
  }
};

JP_EXTERNAL(int) JP_Error(TJConnector& conn, char* Msg, int32 MsgLen);
struct JP_XML_FORMAT
{
  char result[64];
  JP_XML_FORMAT(int32 value)
  {
    sprintf(result, "%d", value);
  }
  JP_XML_FORMAT(int64 value)
  {
#if defined M_W32
    sprintf(result, "%lld", value);
#else
    sprintf(result, "%ld", value);
#endif
  }
  JP_XML_FORMAT(double value)
  {
    sprintf(result, "%0.15g", value);
  }
};

#endif

