#include "clibinu.h"

static bool _useNull(TJField &field);

TJRet::TJRet(const char* table, const char* field)
{
  const char* SEQUENCE = "nextval for %sseq,";
  int size = strlen(SEQUENCE)+strlen(table)+1;
  sequence = TJAlloc::newChar(size);
  snprintf(sequence, size, SEQUENCE, table);
  if (field != 0)
  {
    const char* SELECT = "select %s from new table (";
    size = strlen(SELECT)+strlen(field)+1;
    head = TJAlloc::newChar(size);
    snprintf(head, size, SELECT, field);
    tail = ")";
  }
  else
  {
    head = "";
    tail = "";
  }
  output = "";
  dropField = "";
  doesGeneratedKeys = false;
}

TJRet::~TJRet()
{
  if (strlen(sequence) > 0)
    TJAlloc::deleteChar(sequence);
  if (strlen(head) > 0)
    TJAlloc::deleteChar(head);
}

static char* _rtrim(char *str)
{
  int n=strlen(str);
  for (int i=n-1; i >= 0; i--)
  {
    if (str[i] == ' ') 
      str[i] = 0;
    else
      break;
  }
  return str;
}

char* TJProc::_getvar(TJLine &line, void *rec)
{
  char* var = "";
  if (line.isVar == true)
  {
    char* varname;
    if (strncmp(line.line, "_ret.", 5) == 0)
    {
      if (ret == 0) // if was not setup above
        ret = new TJRet(table);
      varname = _rtrim(line.line+5);
      if (strcmp(varname, "head") == 0)
        var = ret->head;
      else if (strcmp(varname, "tail") == 0)
        var = ret->tail;
      else if (strcmp(varname, "sequence") == 0)
        var = ret->sequence;
      else if (strcmp(varname, "output") == 0)
        var = ret->output;
      else if (strcmp(varname, "dropField") == 0)
        var = ret->dropField;
    }
    else
    {
      varname = _rtrim(line.line);
      for (int j=0; j<noDyns; j++)
      {
        TJDynamic &dyn = dyns[j];
        if (strcmp(varname, dyn.name) == 0)
        {
          var = (char*)rec + dyn.offset;
          break;
        }
      }
    }
  }
  else
    var = line.line;
  return var;
}

TJConnector* TJProc::_select(TJConnector *conn)
{
  try
  {
    for (int i=0; i<conns.getCount(); i++)
    {
      if (strcmp(conns[i].database, database) == 0)
        return conns[i].connector;
    }
    if (conn == 0 && server != 0 && schema != 0 && user != 0 && password != 0)
    {
      conn = TJConnector::Coordinated();
      conn->Logon(user, password, server, schema);
      char connectStr[256];
      snprintf(connectStr, sizeof(connectStr), "%s:%s:%s:%s", server, schema, user, password);
      addconnect(database, conn, connectStr);
      return conn;
    }
    if (conn == 0)
      throw TCliApiException(999, "", "", "TJProc: No logon info present", __FILE__, __LINE__);
    return conn;
  }
  catch (xCept x)
  {
    printf("%s\n", x.ErrorStr());
    throw;
  }
}

void TJProc::_setup(TJConnector *conn)
{
  query = new TJQuery(*_select(conn));
  int cmdLen = 1;
  for (int i=0; i<noLines; i++)
  {
    TJLine &line = lines[i];
    cmdLen += strlen(line.line);
  }
  query->command = TJAlloc::newChar(cmdLen);
  for (int i=0; i<noLines; i++)
  {
    TJLine &line = lines[i];
    strcat(query->command, line.line);
  }
}

void TJProc::_setup(TJConnector *conn, void *rec)
{
  query = new TJQuery(*_select(conn));
  if ((options & IS_INSERT) && (options & HAS_RETURNING))
  {
    for (int i=0; i<noOuts; i++)
    {
      TJField &field = outs[i];
      if ((field.options & TJField::IS_SEQUENCE) && (field.options & TJField::IS_PRIMARY_KEY))
      {
        ret = new TJRet(table, field.name);
        break;
      }
    }
  }
  else if ((options & IS_INSERT) && (options & IS_MULTIPLE_INP))
  {
    ret = new TJRet(table);
  }
  int cmdLen = 1;
  for (int i=0; i<noLines; i++)
  {
    TJLine &line = lines[i];
    char* var = _getvar(line, rec);
    cmdLen += strlen(var);
  }
  query->command = TJAlloc::newChar(cmdLen);
  for (int i=0; i<noLines; i++)
  {
    TJLine &line = lines[i];
    char* var = _getvar(line, rec);
    strcat(query->command, var);
  }
}

int TJProc::_noNulls()
{
  int noNulls = 0;
  for (int i=0; i<noInps; i++)
  {
    TJField& field = inps[i];
    if ((field.options & TJField::IS_NULL) != 0 /* && _useNull(field) == true */)
      noNulls++;
  }
  return noNulls;
}

void TJProc::_bind(TJQuery &query, void *rec)
{
  char* p = (char*)rec;
  for (int i=0; i<noBinds; i++)
  {
    TJField& field = inps[binds[i]];
    switch (field.type)
    {
    case TJField::TJ_BOOLEAN: 
    case TJField::TJ_BYTE: 
    case TJField::TJ_SHORT: 
    case TJField::TJ_STATUS: 
      {
        int16 &val = *(int16*)(p+field.offset);
        SwapBytes(val);
        query.Bind(i, val, SQL_PARAM_INPUT);
        break;
      }
    case TJField::TJ_IDENTITY: 
    case TJField::TJ_INT: 
    case TJField::TJ_SEQUENCE: 
      {
        int32 &val = *(int32*)(p+field.offset);
        SwapBytes(val);
        query.Bind(i, val, SQL_PARAM_INPUT);
        break;
      }
    case TJField::TJ_BIGIDENTITY: 
    case TJField::TJ_BIGSEQUENCE: 
    case TJField::TJ_LONG: 
      {
        int64 &val = *(int64*)(p+field.offset);
        SwapBytes(val);
        query.Bind(i, val, SQL_PARAM_INPUT);
        break;
      }
    case TJField::TJ_FLOAT: 
    case TJField::TJ_DOUBLE: 
      if (field.precision <= 15)
      {
        double &val = *(double*)(p+field.offset);
        SwapBytes(val);
        query.Bind(i, val, field.precision, field.scale, SQL_PARAM_INPUT);
        break;
      }
      else
      {
        char* cval = p + field.offset;
        query.Bind(i, cval, field.precision, field.scale, SQL_PARAM_INPUT);
        break;
      }
    case TJField::TJ_MONEY: 
      {
        char* cval = p + field.offset;
        query.Bind(i, cval, 18, 2, SQL_PARAM_INPUT);
        break;
      }
    case TJField::TJ_BLOB: 
      {
        int32 &val = *(int32*)(p+field.offset);
        SwapBytes(val);
        char *cval = p + field.offset;
        int32 size = field.size-sizeof(int32);
        query.BindBlob(i, cval, size, SQL_PARAM_INPUT);
        break;
      }
    case TJField::TJ_ANSICHAR: 
    case TJField::TJ_CHAR: 
      {
        int type = field.type == TJField::TJ_CHAR ? 0 : 1;
        int null = (field.options & TJField::IS_NULL) != 0 ? 1 : 0;
        char* cval = p + field.offset;
        query.Bind(i, cval, field.length, SQL_PARAM_INPUT, type, null);
        break;
      }
    case TJField::TJ_DATE: 
      {
        char* dval = p + field.offset;
        int16* null = 0;
        if ((field.options & TJField::IS_NULL) != 0)
          null = (int16*)(p + field.offset + field.size);
        field.date = new DATE_STRUCT();
        query.Bind(i,  query.Date(*field.date, dval), SQL_PARAM_INPUT , null);
        break;
      }
    case TJField::TJ_TIMESTAMP: 
      {
        char* dval = p + field.offset;
        int16* null = 0;
        field.timestamp = new TIMESTAMP_STRUCT();
        query.Bind(i,  query.TimeStamp(*field.timestamp, dval), SQL_PARAM_INPUT , null);
        break;
      }
    case TJField::TJ_DATETIME: 
      {
        char* dval = p + field.offset;
        int16* null = 0;
        if ((field.options & TJField::IS_NULL) != 0)
          null = (int16*)(p + field.offset + field.size);
        field.timestamp = new TIMESTAMP_STRUCT();
        query.Bind(i,  query.DateTime(*field.timestamp, dval), SQL_PARAM_INPUT , null);
        break;
      }
    case TJField::TJ_TIME: 
      {
        char* dval = p + field.offset;
        int16* null = 0;
        if ((field.options & TJField::IS_NULL) != 0)
          null = (int16*)(p + field.offset + field.size);
        field.time = new TIME_STRUCT();
        query.Bind(i,  query.Time(*field.time, dval), SQL_PARAM_INPUT , null);
        break;
      }
    case TJField::TJ_TLOB: 
    case TJField::TJ_XML: 
      {
        char* cval = p + field.offset;
        query.Bind(i, cval, field.length, SQL_PARAM_INPUT, 0, 0);
        break;
      }
    case TJField::TJ_USERSTAMP: 
    case TJField::TJ_DYNAMIC_: 
    case TJField::TJ_UID: 
      break;
    }
  }
}

void TJProc::_define(TJQuery &query)
{
  int noInArray = noRows <= 0 ? 1 : noRows;
  for (int i=0; i<noOuts; i++)
  {
    TJField& field = outs[i];
    int offset = field.offset * noInArray;
    char *data = query.data + offset;
    switch (field.type)
    {
    case TJField::TJ_BOOLEAN: 
    case TJField::TJ_BYTE: 
    case TJField::TJ_SHORT: 
    case TJField::TJ_STATUS: 
      {
        int16* val = (int16*) data;
        query.Define(i, val);
        break;
      }
    case TJField::TJ_IDENTITY: 
    case TJField::TJ_INT: 
    case TJField::TJ_SEQUENCE: 
      {
        int32* val = (int32*) data;
        query.Define(i, val);
        break;
      }
    case TJField::TJ_BIGIDENTITY: 
    case TJField::TJ_BIGSEQUENCE: 
    case TJField::TJ_LONG: 
      {
        int64* val = (int64*) data;
        query.Define(i, val);
        break;
      }
    case TJField::TJ_FLOAT: 
    case TJField::TJ_DOUBLE: 
      if (field.precision <= 15)
      {
        double* val = (double*) data;
        query.Define(i, val);
        break;
      }
      else
      {
        char* val = data;
        query.Define(i, val, field.precision+3);
        break;
      }
    case TJField::TJ_MONEY: 
      {
        char* val = data;
        query.Define(i, val, 21);
        break;
      }
    case TJField::TJ_BLOB: 
      {
        char* val = data;
        query.DefineBlob(i, val, field.length);
        break;
      }
    case TJField::TJ_ANSICHAR: 
    case TJField::TJ_CHAR: 
      {
        char* val = data;
        query.Define(i, val, field.length+1);
        break;
      }
    case TJField::TJ_DATE: 
      {
        DATE_STRUCT* val = (DATE_STRUCT*) data;
        query.Define(i, val);
        break;
      }
    case TJField::TJ_DATETIME: 
    case TJField::TJ_TIMESTAMP: 
      {
        TIMESTAMP_STRUCT* val = (TIMESTAMP_STRUCT*) data;
        query.Define(i, val);
        break;
      }
    case TJField::TJ_TIME: 
      {
        TIME_STRUCT* val = (TIME_STRUCT*) data;
        query.Define(i, val);
        break;
      }
    case TJField::TJ_TLOB: 
    case TJField::TJ_XML: 
      {
        char* val = data;
        query.Define(i, val, field.length+1);
        break;
      }
    case TJField::TJ_USERSTAMP: 
    case TJField::TJ_DYNAMIC_: 
    case TJField::TJ_UID: 
      break;
    }
  }
}

static bool _useNull(TJField &field)
{
  switch (field.type)
  {
  case TJField::TJ_BLOB: 
  case TJField::TJ_ANSICHAR: 
  case TJField::TJ_CHAR: 
  case TJField::TJ_TLOB: 
  case TJField::TJ_XML: 
    return false;
  }
  return true;
}

bool TJProc::_fetch(TJQuery &query, void *&rec)
{
  bool result = query.Fetch();
  if (result == false)
    return result;
  int noInArray = noRows <= 0 ? 1 : noRows;
  char* fld = (char*) rec;
  for (int i=0; i<noOuts; i++)
  {
    TJField& field = outs[i];
    int data_offset = field.offset * noInArray;
    char* data = query.data + data_offset;
    switch (field.type)
    {
    case TJField::TJ_BOOLEAN: 
    case TJField::TJ_BYTE: 
    case TJField::TJ_SHORT: 
    case TJField::TJ_STATUS: 
      {
        int16 &val = *(int16*)(fld+field.offset);
        query.Get(val, data);
        SwapBytes(val);
        break;
      }
    case TJField::TJ_IDENTITY: 
    case TJField::TJ_INT: 
    case TJField::TJ_SEQUENCE: 
      {
        int32 &val = *(int32*)(fld+field.offset);
        query.Get(val, data);
        SwapBytes(val);
        break;
      }
    case TJField::TJ_BIGIDENTITY: 
    case TJField::TJ_BIGSEQUENCE: 
    case TJField::TJ_LONG: 
      {
        int64 &val = *(int64*)(fld+field.offset);
        query.Get(val, data);
        SwapBytes(val);
        break;
      }
    case TJField::TJ_FLOAT: 
    case TJField::TJ_DOUBLE: 
      if (field.precision <= 15)
      {
        double &val = *(double*)(fld+field.offset);
        query.Get(val, data);
        SwapBytes(val);
        break;
      }
      else
      {
        char *cval = (fld+field.offset);
        query.Get(cval, data, field.precision+3);
        break;
      }
    case TJField::TJ_MONEY: 
      {
        char *cval = (fld+field.offset);
        query.Get(cval, data, 21);
        break;
      }
    case TJField::TJ_BLOB: 
      {
        int32 val = *(int32*)(fld+field.offset);
        unsigned char *uval = (unsigned char*)(fld + field.offset);
        int32 size = field.size-sizeof(int32);
        query.Get(val, uval, data, size);
        SwapBytes(val);
        break;
      }
    case TJField::TJ_ANSICHAR: 
    case TJField::TJ_CHAR: 
      {
        char *cval = (fld+field.offset);
        query.Get(cval, data, field.length+1);
        break;
      }
    case TJField::TJ_DATE: 
      {
        char *dval = (fld+field.offset);
        query.Get(TJDate(dval), data); 
        break;
      }
    case TJField::TJ_DATETIME: 
    case TJField::TJ_TIMESTAMP: 
      {
        char *dval = (fld+field.offset);
        query.Get(TJDateTime(dval), data); 
        break;
      }
    case TJField::TJ_TIME: 
      {
        char *dval = (fld+field.offset);
        query.Get(TJTime(dval), data); 
        break;
      }
    case TJField::TJ_TLOB: 
    case TJField::TJ_XML: 
      {
        char *dval = (fld+field.offset);
        query.Get(dval, data, field.length+1);
        break;
      }
    case TJField::TJ_USERSTAMP: 
    case TJField::TJ_DYNAMIC_: 
    case TJField::TJ_UID: 
      break;
    }
    if ((field.options & TJField::IS_NULL) != 0)
    {
      if (_useNull(field) == true)
      {
        int16* null = (int16*)(fld+field.offset+field.size);
        query.GetNull(*null, i);
        SwapBytes(null);
      }
    }
  }
  return result;
}

template <class T>
inline void _swap(T &result, char* data)
{
  if (mustDoSwap == true)
    SwapBytes(data, (int) sizeof(T));
  result = *(T*)data;
}

void TJProc::_move(TJQuery &query, TJField& field, int recNo, int noRecs, int &nullNo, void *&rec, void *&data)
{
  if ((field.options & TJField::IS_NULL) != 0)
  {
    SQLINTEGER* out = &query.indicators[nullNo*noRecs];
    nullNo++;
    if (_useNull(field) == true)
    {
      int16 val; 
      _swap(val, (char*)rec+field.offset+field.size);
      out[recNo] = (SQLINTEGER)val;
    }
    else
    {
      char* inp = (char*)rec+field.offset;
      int16 val = strlen(inp) == 0 ? JP_NULL : JP_NOT_NULL;
      out[recNo] = (SQLINTEGER)val;
    }
  }
  switch (field.type)
  {
  case TJField::TJ_BOOLEAN: 
  case TJField::TJ_BYTE: 
  case TJField::TJ_SHORT: 
  case TJField::TJ_STATUS: 
    {
      int16* arr = (int16*)data;
      int16 val; 
      _swap(val, (char*)rec+field.offset);
      arr[recNo] = val;
      break;
    }
  case TJField::TJ_IDENTITY: 
  case TJField::TJ_INT: 
  case TJField::TJ_SEQUENCE: 
    {
      int32* arr = (int32*)data;
      int32 val; 
      _swap(val, (char*)rec+field.offset);
      arr[recNo] = val;
      break;
    }
  case TJField::TJ_BIGIDENTITY: 
  case TJField::TJ_BIGSEQUENCE: 
  case TJField::TJ_LONG: 
    {
      int64* arr = (int64*)data;
      int64 val; 
      _swap(val, (char*)rec+field.offset);
      arr[recNo] = val;
      break;
    }
  case TJField::TJ_FLOAT: 
  case TJField::TJ_DOUBLE: 
    if (field.precision <= 15)
    {
      double* arr = (double*)data;
      double val; 
      _swap(val, (char*)rec+field.offset);
      arr[recNo] = val;
      break;
    }
    else
    {
      int length = field.precision+3;
      char* inp = (char*)rec+field.offset;
      char* out = (char*)data + (recNo*length);
      memcpy(out, inp, length);
      break;
    }
  case TJField::TJ_MONEY: 
    {
      int length = 21;
      char* inp = (char*)rec+field.offset;
      char* out = (char*)data + (recNo*length);
      memcpy(out, inp, length-1);
      break;
    }
  case TJField::TJ_BLOB: 
    {
      int length = field.length;
      char* inp = (char*)rec+field.offset;
      char* out = (char*)data + (recNo*length);
      memcpy(out, inp, length);
      break;
    }
  case TJField::TJ_ANSICHAR: 
  case TJField::TJ_CHAR: 
    {
      int length = field.length+1;
      char* inp = (char*)rec+field.offset;
      char* out = (char*)data + (recNo*length);
      memcpy(out, inp, length);
      break;
    }
  case TJField::TJ_DATE: 
    {
      char* inp = (char*)rec+field.offset;
      DATE_STRUCT* out = (DATE_STRUCT*)data;
      query.Date(out[recNo], inp);
      break;
    }
  case TJField::TJ_DATETIME: 
    {
      char* inp = (char*)rec+field.offset;
      TIMESTAMP_STRUCT* out = (TIMESTAMP_STRUCT*)data;
      query.DateTime(out[recNo], inp);
      break;
    }
  case TJField::TJ_TIMESTAMP: 
    {
      char* inp = (char*)rec+field.offset;
      TIMESTAMP_STRUCT* out = (TIMESTAMP_STRUCT*)data;
      query.TimeStamp(out[recNo], inp);
      break;
    }
  case TJField::TJ_TIME: 
    {
      char* inp = (char*)rec+field.offset;
      TIME_STRUCT* out = (TIME_STRUCT*)data;
      query.Time(out[recNo], inp);
      break;
    }
  case TJField::TJ_TLOB: 
  case TJField::TJ_XML: 
    {
      int length = field.length+1;
      char* inp = (char*)rec+field.offset;
      char* out = (char*)data + (recNo*length);
      memcpy(out, inp, length);
      break;
    }
  case TJField::TJ_USERSTAMP: 
  case TJField::TJ_DYNAMIC_: 
  case TJField::TJ_UID: 
    break;
  }
}

void  TJProc::_bindArray(TJQuery &query)
{
  for (int bind = 0; bind < noBinds; bind++)
  {
    int inp = binds[bind];
    TJField& field = inps[inp];
    int noRows = query.noRows;
    char* data = query.data + (field.offset*noRows);
    SQLINTEGER *null = 0;
    int noNull = 0;
    if ((field.options & TJField::IS_NULL) != 0)
    {
      if (_useNull(field) == true)
        null = &query.indicators[noNull*noRows];
      noNull++;
    }
    switch (field.type)
    {
    case TJField::TJ_BOOLEAN: 
    case TJField::TJ_BYTE: 
    case TJField::TJ_SHORT: 
    case TJField::TJ_STATUS: 
      {
        int16* arr = (int16*)data;
        query.BindInt16Array(bind, arr, null);
        break;
      }
    case TJField::TJ_IDENTITY: 
    case TJField::TJ_INT: 
    case TJField::TJ_SEQUENCE: 
      {
        int32* arr = (int32*)data;
        query.BindInt32Array(bind, arr, null);
        break;
      }
    case TJField::TJ_BIGIDENTITY: 
    case TJField::TJ_BIGSEQUENCE: 
    case TJField::TJ_LONG: 
      {
        int64* arr = (int64*)data;
        query.BindInt64Array(bind, arr, null);
        break;
      }
    case TJField::TJ_FLOAT: 
    case TJField::TJ_DOUBLE: 
      if (field.precision <= 15)
      {
        double* arr = (double*)data;
        query.BindDoubleArray(bind, arr, field.precision, field.scale, null);
        break;
      }
      else
      {
        query.BindMoneyArray(bind, data, field.precision, field.scale, null);
        break;
      }
    case TJField::TJ_MONEY: 
      {
        query.BindMoneyArray(bind, data, 18, 2, null);
        break;
      }
    case TJField::TJ_BLOB: 
      {
        int32 size = field.length-sizeof(int32);
        query.BindBlob(bind, data, size, SQL_PARAM_INPUT);
        break;
      }
    case TJField::TJ_ANSICHAR: 
    case TJField::TJ_CHAR: 
      {
        query.BindCharArray(bind, data, field.length+1, null);
        break;
      }
    case TJField::TJ_DATE: 
      {
        DATE_STRUCT* out = (DATE_STRUCT*)data;
        query.BindDateArray(bind, out, null);
        break;
      }
    case TJField::TJ_DATETIME: 
      {
        TIMESTAMP_STRUCT* out = (TIMESTAMP_STRUCT*)data;
        query.BindDateTimeArray(bind, out, null);
        break;
      }
    case TJField::TJ_TIMESTAMP: 
      {
        TIMESTAMP_STRUCT* out = (TIMESTAMP_STRUCT*)data;
        query.BindDateTimeArray(bind, out, null);
        break;
      }
    case TJField::TJ_TIME: 
      {
        TIME_STRUCT* out = (TIME_STRUCT*)data;
        query.BindTimeArray(bind, out, null);
        break;
      }
    case TJField::TJ_TLOB: 
    case TJField::TJ_XML: 
      {
        query.BindCharArray(bind, data, field.length, null);
        break;
      }
    case TJField::TJ_USERSTAMP: 
    case TJField::TJ_DYNAMIC_: 
    case TJField::TJ_UID: 
      break;
    }
  }
}

struct autoBuff
{
  void *buff;
  autoBuff(void *inBuff, int size)
  {
    if (size > 0)
    {
      buff = new char [size];
      if (inBuff != 0)
        memcpy(buff, inBuff, size);
      else
        memset(buff, 0, size);
    }
    else
      buff = 0;
  }
  ~autoBuff()
  {
    if (buff != 0)
      delete [] buff;
  }
};

bool     TJProc::autoSave;
ConnList TJProc::conns;

void TJProc::addconnect(const char* database, TJConnector *connector, const char* connectStr)
{
  TJConn conn(database, connector, connectStr);
  conns.add(conn);
}

void TJProc::actionOnly(TJConnector *conn)
{
  try
  {
    _setup(conn);
    query->Open(query->command);
    query->Exec();
    if (autoSave == true)
      conn->Commit();
  }
  catch (...)
  {
    if (autoSave == true)
      conn->Rollback();
    throw;
  }
}

void TJProc::action(TJConnector *conn, int size, void *rec)
{
  try
  {
    _setup(conn, rec);
    query->Open(query->command, noBinds);
    autoBuff inp(rec, recSize);
    _bind(*query, inp.buff);
    query->Exec();
    if (autoSave == true)
      conn->Commit();
  }
  catch (...)
  {
    if (autoSave == true)
      conn->Rollback();
    throw;
  }
}

void TJProc::bulkaction(TJConnector *conn, int noRecs, void *recs)
{
  try
  {
    _setup(conn, recs);
    query->OpenArray(query->command, noBinds, _noNulls(), noRecs, recSize);
    memset(query->data, 0, noRecs*recSize);
    for (int recNo=0; recNo<noRecs; recNo++)
    {
      void *inRec = (char*)recs+(recSize*recNo);
      void *outData = query->data;
      int noNull = 0;
      for (int j=0; j<noInps; j++) 
      {
        TJField& field = inps[j];
        outData = (char*)query->data + field.offset*noRecs;
        _move(*query, field, recNo, noRecs, noNull, inRec, outData);
      }
    }
    _bindArray(*query);
    query->Exec();
    if (autoSave == true)
      conn->Commit();
  }
  catch (...)
  {
    if (autoSave == true)
      conn->Rollback();
    throw;
  }
}

bool TJProc::singleOnly(TJConnector *conn, int* outSize, void *&outRec)
{
  try
  {
    _setup(conn);
    query->Open(query->command, 0, noOuts, 1, recSize);
    _define(*query);
    query->Exec();
    *outSize = recSize;
    outRec = calloc(1, recSize);
    bool result = _fetch(*query, outRec);
    if (autoSave == true)
      conn->Commit();
    return result;
  }
  catch (...)
  {
    if (autoSave == true)
      conn->Rollback();
    throw;
  }
}

bool TJProc::single(TJConnector *conn, int size, void *rec, int* outSize, void *&outRec)
{
  try
  {
    _setup(conn, rec);
    query->Open(query->command, noBinds, noOuts, 1, recSize);
    autoBuff inp(rec, recSize);
    if (noBinds > 0)
      _bind(*query, inp.buff);
    _define(*query);
    query->Exec();
    *outSize = recSize;
    outRec = calloc(1, recSize);
    memcpy(outRec, rec, recSize); // copy in the inputs
    bool result = _fetch(*query, outRec); // fetch the outputs
    if (autoSave == true)
      conn->Commit();
    return result;
  }
  catch (...)
  {
    if (autoSave == true)
      conn->Rollback();
    throw;
  }
}

void TJProc::multipleOnly(TJConnector *conn, int* outNo, int* outSize, void *&outRecs)
{
  try
  {
    _setup(conn);
    *outSize = recSize;
    query->Open(query->command, 0, noOuts, noRows, recSize);
    _define(*query);
    query->Exec();
    if (autoSave == true)
      conn->Commit();
    *outNo = _load(outRecs);
    *outSize = *outNo * recSize;
  }
  catch (...)
  {
    if (autoSave == true)
      conn->Rollback();
    throw;
  }
}

void TJProc::multiple(TJConnector *conn, int size, void *rec, int* outNo, int* outSize, void *&outRecs)
{
  try
  {
    _setup(conn, rec);
    query->Open(query->command, noBinds, noOuts, noRows, recSize);
    autoBuff inp(rec, recSize);
    _bind(*query, inp.buff);
    _define(*query);
    query->Exec();
    if (autoSave == true)
      conn->Commit();
    *outNo = _load(outRecs, rec);
    *outSize = *outNo * recSize;
  }
  catch (...)
  {
    if (autoSave == true)
      conn->Rollback();
    throw;
  }
}

static void* _index(void* recs, int index, int rowSize)
{
  char* p = (char *) recs;
  p += index * rowSize;
  return p;
}

int TJProc::_load(void*& recs, void* rec)
{
  int count = 0, index = 0;
  int delta = 100;
  autoBuff x(rec, recSize);
  while (_fetch(*query, (void*&)x.buff) == true)
  {
    if (index >= count)
    {
      count += delta;
      void *newrecs = realloc(recs, count*recSize);
      recs = newrecs;
    }
    memcpy(_index(recs, index, recSize), x.buff, recSize);
    index++;
  }
  return index;
}

