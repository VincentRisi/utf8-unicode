#ifndef __clibinu_h__
#define __clibinu_h__
#include "cliapi.h"
#include "swapbytes.h"
#include "addlist.h"

inline char* dupChar(const char* data)
{
  return TJAlloc::dupChar(data);
}
 
inline void deleteChar(char *&data) 
{
  TJAlloc::deleteChar(data);
  data=0;
}

struct TJLine
{
  bool isVar;
  char* line;
  TJLine()
  {
    isVar = false;
    line = 0;
  }
  ~TJLine()
  {
    if (line != 0)
      deleteChar(line);
  }
};

struct TJField
{
  static const int
    TJ_BLOB         = 1
  , TJ_BOOLEAN      = 2
  , TJ_BYTE         = 3
  , TJ_CHAR         = 4
  , TJ_DATE         = 5
  , TJ_DATETIME     = 6
  , TJ_DOUBLE       = 7
  , TJ_DYNAMIC_     = 8
  , TJ_FLOAT        = 9
  , TJ_IDENTITY     = 10
  , TJ_INT          = 11
  , TJ_LONG         = 12
  , TJ_MONEY        = 13
  , TJ_SEQUENCE     = 14
  , TJ_SHORT        = 15
  , TJ_STATUS       = 16
  , TJ_TIME         = 17
  , TJ_TIMESTAMP    = 18
  , TJ_TLOB         = 19
  , TJ_USERSTAMP    = 20
  , TJ_ANSICHAR     = 21
  , TJ_UID          = 22
  , TJ_XML          = 23
  , TJ_BIGSEQUENCE  = 24
  , TJ_BIGIDENTITY  = 25
  ;
  static const int 
    IS_PRIMARY_KEY  = 0x0001
  , IS_SEQUENCE     = 0x0002
  , IS_NULL         = 0x0004
  , IS_IN           = 0x0008
  , IS_OUT          = 0x0010
  ;
  char *name;
  int type;
  int length;
  int precision;
  int scale;
  int offset;
  int size;
  int options;
  DATE_STRUCT *date;
  TIMESTAMP_STRUCT *timestamp;
  TIME_STRUCT *time;
  TJField() 
  {
    name = 0;
    date = 0;
    time = 0;
    timestamp = 0;
  }
  ~TJField()
  {
    if (name != 0) 
      deleteChar(name);
    if (date != 0)
      delete date;
    if (time != 0)
      delete time;
    if (timestamp != 0)
      delete timestamp;
  }
  bool test(int option)
  {
    return (options & option) == option;
  }
};

struct TJDynamic
{
  char *name;
  int length;
  int offset;
  TJDynamic(){name = 0;}
  ~TJDynamic()
  {
    if (name != 0) 
      deleteChar(name);
  }
};

struct TJRet
{
  char* head;
  char* output;
  char* tail;
  char* sequence;
  char* dropField;
  bool doesGeneratedKeys;
  TJRet(const char* table, const char* field=0);
  ~TJRet();
};

struct TJConn
{
  char database[256];
  char connectStr[256];
  TJConnector *connector;
  TJConn(const char* database, TJConnector *connector, const char* connectStr)
  {
    memset(this->database, 0, 256);
    strncpy(this->database, database, 255);
    this->connector = connector;
    memset(this->connectStr, 0, 256);
    strncpy(this->connectStr, connectStr, 255);
  }
};

typedef TAddList<TJConn, int32> ConnList;

struct TJProc
{
  static const int 
    IS_PROC         = 0x0001
  , IS_SPROC        = 0x0002
  , IS_DATA         = 0x0004
  , IS_IDL_CODE     = 0x0008
  , IS_SQL          = 0x0010
  , IS_SINGLE       = 0x0020
  , IS_ACTION       = 0x0040
  , IS_STD          = 0x0080
  , USE_STD         = 0x0100
  , EXTENDS_STD     = 0x0200
  , USE_KEY         = 0x0400
  , HAS_IMAGE       = 0x0800
  , IS_MULTIPLE_INP = 0x1000
  , IS_INSERT       = 0x2000
  , HAS_RETURNING   = 0x4000
  ;
  char *database;
  char *table;
  char *name;
  char *server;
  char *schema;
  char *user;
  char *password;
  int noRows; 
  int options;
  int noBinds;
  int noInps;
  int noOuts;
  int noDyns;
  int noLines;
  int recSize;
  TJLine* lines;
  int *binds;
  TJField* inps;
  TJField* outs;
  TJDynamic* dyns;
  TJConnector* conn;
  TJQuery* query;
  TJRet* ret;
  TJProc() 
  {
    database = 0;
    table = 0;
    name = 0;
    server = 0;
    schema = 0;
    user = 0;
    password = 0;
    lines = 0;
    inps = 0;
    outs = 0;
    dyns = 0;
    conn = 0;
    query = 0;
    ret = 0;
  }
  ~TJProc()
  { 
    if (database != 0) 
      deleteChar(database);
    if (table != 0) 
      deleteChar(table);
    if (name != 0) 
      deleteChar(name);
    if (server != 0) 
      deleteChar(server);
    if (schema != 0) 
      deleteChar(schema);
    if (user != 0) 
      deleteChar(user);
    if (password != 0) 
      deleteChar(password);
    if (lines != 0) 
      delete [] lines;
    if (inps != 0) 
      delete [] inps;
    if (outs !=0 ) 
      delete [] outs;
    if (dyns != 0) 
      delete [] dyns;
    if (query != 0)
      delete query;
    if (ret != 0)
      delete ret;
  }
  bool test(int option)
  {
    return (options & option) == option;
  }
  bool hasReturning()
  {
    return (options & HAS_RETURNING) != 0;
  }
  bool isInsert()
  {
    return (options & IS_INSERT) != 0;
  }
  void actionOnly(TJConnector *conn);
  void action(TJConnector *conn, int size, void *rec);
  void bulkaction(TJConnector *conn, int noRecs, void *recs);
  bool singleOnly(TJConnector *conn, int *outBuffSize, void *&outBuffRec);
  bool single(TJConnector *conn, int size, void *rec, int* outBuffSize, void *&outBuffRec);
  void multipleOnly(TJConnector *conn, int * outNo, int* outSize, void *&outRecs);
  void multiple(TJConnector *conn, int size, void *rec, int* outNo, int* outSize,  void *&recs);
  static void addconnect(const char* database, TJConnector *conn, const char* connectStr=0);
  static void setAutoSave(bool how=true) {autoSave = how;}
  static ConnList conns;
  static TJConnector *getConnect() 
  {
    if (conns.getCount() > 0) 
      return conns[0].connector;
    else
      return 0;
  }
protected:
  char* _getvar(TJLine &line, void *rec);
  TJConnector* _select(TJConnector *conn);
  void _setup(TJConnector *conn);
  void _setup(TJConnector *conn, void *rec);
  void _bind(TJQuery &query, void *rec);
  void _bindArray(TJQuery &query);
  void _define(TJQuery &query);
  bool _fetch(TJQuery &query, void *&rec);
  void _move(TJQuery &query, TJField& field, int recNo, int noRecs, int &noNull, void *&rec, void *&data);
  int  _noNulls();
  int  _load(void *&recs, void* rec=0);
private:
  static bool autoSave;
};

#endif