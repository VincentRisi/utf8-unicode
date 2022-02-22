#include "orchdefendpoint_snips.h"
#include "channeltransaction_snips.h"
#include "message_snips.h"
#include "moggy_snips.h"
#include "orchdef_snips.h"
#include "orchinst_snips.h"
#include "routehistory_snips.h"
#include "singletrans_snips.h"
#include "attachmenttype_snips.h"
#include "diarytype_snips.h"
#include "gatetype_snips.h"
#include "interfacetype_snips.h"
#include "notetype_snips.h"
#include "status_snips.h"
#include "taskparametersconfig_snips.h"
#include "tasktype_snips.h"
#include "xcept.h"

template <size_t N>
inline char* zzcopy(char(&target)[N], const char* source) 
{ 
    int n = N-1; 
    target[n] = 0; 
    return strncpy(target, source, n); 
}

void addMessage(TJConnector &conn)
{
    try
    {
        DMessage rec;
        zzcopy(rec.SourceSysId, "Hoolah hoop");
        zzcopy(rec.Reference, "Some Reference");
        rec.ControlFlag = 1;
        zzcopy(rec.SourceNodeId, "ABIG BOY");
        zzcopy(rec.NodeId, "Bigger Ears");
        zzcopy(rec.ReplyNodeId, "REPROBATED");
        zzcopy(rec.NodeType, "A");
        rec.MessageLen = 20;
        rec.MessageLenStored = 20;
        zzcopy(rec.MessageData, "A little twenty");
        zzcopy(rec.MetaData, "RITA METER MADE");
        rec.MessageType = 1;
        rec.Priority = 2;
        rec.ParentMessageId = 61552019;
        rec.OrchInstId = 123;
        rec.OrchInstIdIsNull = 0;
        zzcopy(rec.DateCreated, "20211021000000");
        zzcopy(rec.ExternalReference, "EXTREF");
        zzcopy(rec.TranGUID, "BAABAADEADCAF");
        zzcopy(rec.USId, "MOLLY");
        zzcopy(rec.TMStamp, "20210101010101");
        rec.ArchFlag = 1;
        MessageInsert(&conn, &rec);
        conn.Commit();
        bool result = MessageSelectOne(&conn, &rec);
        if (result == true)
        {
            printf("\nId:%lld\n"
                   "ParentMessageId:%lld\n"
                   "OrchInstId:%d\n"
                , rec.Id
                , rec.ParentMessageId
                , rec.OrchInstId
            );
        }
    }
    catch (TOdbcApiException& ex)
    {
        printf("%s\n", ex.ErrorStr());
        conn.Rollback();
    }
    catch (...)
    {
        //printf("%s\n", oe.ErrorDesc);
        conn.Rollback();
    }
}

void addMoggy(TJConnector& conn)
{
    try
    {
        DMoggy rec;
        zzcopy(rec.Descr, "This is a bit of data");
        MoggyInsert(&conn, &rec);
        conn.Commit();
        bool result = MoggySelectOne(&conn, &rec);
        if (result == true)
        {
            printf("\nId:%lld\n"
                   "Descr:%s\n"
                , rec.Id
                , rec.Descr
            );
        }
    }
    catch (TOdbcApiException& ex)
    {
        printf("%s\n", ex.ErrorStr());
        conn.Rollback();
    }
    catch (...)
    {
        conn.Rollback();
    }
}

void listAttachments(TJConnector& conn)
{
    int32 noOf=0;
    OAttachmentType* outRecs=0;
    AttachmentTypeSelectAll(&conn, &noOf, outRecs);
    for (int32 i=0; i < noOf; i++)
        printf("%d %s\n", outRecs[i].AttachmentTypeNo, outRecs[i].Descr);
}

void listNotes(TJConnector& conn)
{
    try
    {
        int32 noOf = 0;
        ONoteType* outRecs = 0;
        NoteTypeSelectAll(&conn, &noOf, outRecs);
        for (int32 i = 0; i < noOf; i++)
            printf("%d %s\n", outRecs[i].NoteTypeNo, outRecs[i].Descr);
        DNoteType rec;
        rec.NoteTypeNo = 6;
        bool result = NoteTypeSelectOne(&conn, &rec);
        printf("%d %s\n", rec.NoteTypeNo, rec.Descr);
    }
    catch (TOdbcApiException ex)
    {
        printf("%d - %s\n", ex.Error(), ex.ErrorStr());
    }
    catch (...)
    {
    }
}

const char* connstring = "Dsn=%s; Uid=%s; Pwd=%s;";
enum {  ARG_DRIVER = 1, ARG_USERID, ARG_PASSWORD };
; int main(int argc, char* argv[])
{
  TJConnector conn;
  try
  {
      char work[1024];
      snprintf(work, sizeof(work), connstring, argv[ARG_DRIVER], argv[ARG_USERID], argv[ARG_PASSWORD]);
      printf("%s\n", work); fflush(stdout);
      conn.Logon(work);
      listAttachments(conn);
      listNotes(conn);
      addMessage(conn);
      addMoggy(conn);
      conn.Logoff();
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
  return 0;
}

#if 0
Should be normal ODBC

But you need an Oracle driver

odbc - general is just ODBC ... still requires a DB specific ODBC driver installed

i.e.with Postgres I use this->VANGUARD_CONNECTIONSTRING = DRIVER = { PostgreSQL Unicode }; SERVER = localhost, 5432; DATABASE = Vanguard; UID = postgres; PWD = devpassword;
For something like MySQL I use{ MySQL ODBC 8.0 Unicode Driver }and {MySQL ODBC 8.0 ANSI Driver}
SQL Server I use ODBC Driver 17 for SQL Server
#endif
