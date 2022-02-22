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
        rec.ParentMessageId = 0;
        rec.ParentMessageIdIsNull = 1;
        rec.OrchInstId = 0;
        rec.OrchInstIdIsNull = 1;
        zzcopy(rec.DateCreated, "20211021000000");
        zzcopy(rec.ExternalReference, "EXTREF");
        zzcopy(rec.TranGUID, "BAABAADEADCAF");
        zzcopy(rec.USId, "MOLLY");
        zzcopy(rec.TMStamp, "20212223000000");
        rec.ArchFlag = 1;
        MessageInsert(&conn, &rec);
        conn.Commit();
    }
    catch (TOdbcApiException& oe)
    {
        printf("%s\n", oe.ErrorStr());
        conn.Rollback();
    }
    catch (xCept &ex)
    {
        printf("%s\n", ex.ErrorStr());
        conn.Rollback();
    }
    catch (...)
    {
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
    }
    catch (TOdbcApiException& oe)
    {
        printf("%s\n", oe.ErrorStr());
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
    int32 noOf = 0;
    ONoteType* outRecs = 0;
    NoteTypeSelectAll(&conn, &noOf, outRecs);
    for (int32 i = 0; i < noOf; i++)
        printf("%d %s\n", outRecs[i].NoteTypeNo, outRecs[i].Descr);
}

int main(int argc, char* argv[])
{
  TJConnector conn;
  conn.Logon("bbdmsg", "oracle", "lv01");
  addMoggy(conn);
  addMessage(conn);
  listAttachments(conn);
  listNotes(conn);
  conn.Logoff();
  return 0;
}