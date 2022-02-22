#include "xcept.h"
#include "tbuffer.h"
#include "moggy2_snips.h"

template <size_t N>
inline char* zzcopy(char(&target)[N], const char* source)
{
  int n = N - 1;
  target[n] = 0;
  return strncpy(target, source, n);
}

int addMoggy2(TJConnector& conn)
{
  unsigned char data[888];
  for (int i = 0; i < sizeof(data); i++)
    data[i] = (i + 17) * 19 % 255;
  try
  {
    DMoggy2 rec;
    zzcopy(rec.Descr, "PETER EATS TOFFEE");
    rec.ProcessData.setData(sizeof(data), data);
    rec.DataSize = rec.ProcessData.len();
    zzcopy(rec.TmStamp, "00010101000000");
    Moggy2Insert(&conn, &rec);
    conn.Commit();
    return rec.Id;
  }
  catch (TOciApiException& oe)
  {
    printf("%s\n", oe.ErrorDesc);
    conn.Rollback();
  }
  catch (xCept& ex)
  {
    printf("%s\n", ex.ErrorStr());
    conn.Rollback();
  }
  catch (...)
  {
    conn.Rollback();
  }
}

char* dump(TBChar& buff, unsigned char* data, int len)
{
  char hexLine[81];
  buff.clear();
  buff.append("\n");
  for (int i = 0; i < len; i++)
  {
    char work[256];
    snprintf(work, sizeof(work), "%02x", data[i]);
    buff.append(work);
    if (i % 16 == 15)
      buff.append(" ");
    if (i % 32 == 31)
      buff.append("\n");
  }
  return buff.data;
}

void readMoggy(TJConnector& conn, int id)
{
  try
  {
    TBChar buff;
    DMoggy2 rec;
    rec.Clear();
    rec.Id = id;
    bool hasIt = Moggy2SelectOne(&conn, &rec);
    if (hasIt == true)
    {
      printf("Id: %d\n"
        "Descr: %s\n"
        "ProcessData:\n"
        "  len: %d\n"
        "  data: %s\n"
        "DataSize: %d\n"
        "TmStamp: %s\n"
        , rec.Id
        , rec.Descr
        , rec.ProcessData.len()
        , dump(buff, rec.ProcessData.data(), rec.ProcessData.len())
        , rec.DataSize
        , rec.TmStamp
      );
    }
  }
  catch (TOciApiException& oe)
  {
    printf("%s\n", oe.ErrorDesc);
  }
  catch (xCept& ex)
  {
    printf("%s\n", ex.ErrorStr());
  }
  catch (...)
  {
  }
}

int main(int argc, char* argv[])
{
  TJConnector conn;
  conn.Logon("workflow", "oracle", "lv01");
  int id = addMoggy2(conn);
  readMoggy(conn, id);
  conn.Logoff();
  return 0;
}