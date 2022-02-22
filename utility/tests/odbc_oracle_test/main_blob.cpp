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
  char data[888];
  for (int i = 0; i < sizeof(data); i++)
    data[i] = (i + 17) * 19 % 255;
  try
  {
    DMoggy2 rec;
    zzcopy(rec.Descr, "SPEAKER EKES COFFEE");
    rec.ProcessData.len = sizeof(data);
    memcpy(rec.ProcessData.data, data, sizeof(data));
    rec.DataSize = rec.ProcessData.len;
    zzcopy(rec.TmStamp, "00010101000000");
    Moggy2Insert(&conn, &rec);
    conn.Commit();
    return rec.Id;
  }
  catch (TOdbcApiException& ex)
  {
    printf("%d - %s\n", ex.Error(), ex.ErrorStr());
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
  TBChar buff;
  DMoggy2 rec;
  try
  {
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
        , rec.ProcessData.len
        , dump(buff, rec.ProcessData.data, rec.ProcessData.len)
        , rec.DataSize
        , rec.TmStamp
      );
    }
  }
  catch (TOdbcApiException& ex)
  {
    printf("%d - %s\n", ex.Error(), ex.ErrorStr());
    conn.Rollback();
  }
  catch (xCept& ex)
  {
    printf("%s\n", ex.ErrorStr());
    conn.Rollback();
  }
  catch (...)
  {
  }
}

const char* connstring = "Dsn=%s; Uid=%s; Pwd=%s;";
enum { ARG_DRIVER = 1, ARG_USERID, ARG_PASSWORD };
int main(int argc, char* argv[])
{
  TJConnector conn;
  try
  {
    char work[1024];
    snprintf(work, sizeof(work), connstring, argv[ARG_DRIVER], argv[ARG_USERID], argv[ARG_PASSWORD]);
    printf("%s\n", work); fflush(stdout);
    conn.Logon(work);
    int id = addMoggy2(conn);
    readMoggy(conn, id);
    conn.Logoff();
    return 0;
  }
  catch (TOdbcApiException ex)
  {
    printf("%d - %s\n", ex.Error(), ex.ErrorStr());
    return -1;
  }
  catch (...)
  {
    return 111;
  }
}