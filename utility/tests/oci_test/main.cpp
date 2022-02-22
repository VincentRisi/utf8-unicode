#include "xcept.h"
#include "message_snips.h"
#include "moggy_snips.h"
#include "attachmenttype_snips.h"
#include "notetype_snips.h"
#include "processdataext_snips.h"
#include "processdatadef_snips.h"
#include "utf.hpp"

template <size_t N>
inline char* zzcopy(char(&target)[N], const char* source) 
{ 
  size_t n = N-1; 
  target[n] = 0; 
  return strncpy(target, source, n); 
}
template <size_t N>
inline unsigned char* zzcopy(unsigned char(&target)[N], const unsigned char* source)
{
  size_t n = N - 1;
  target[n] = 0;
  memcpy(target, source, n);
  return target;
}

template <class T>
struct HeapRec
{
  T* rec;
  HeapRec() { rec = new T(); memset(rec, 0, sizeof(rec)); }
  ~HeapRec() { delete rec; }
};

void get100(TJConnector& conn)
{
  DMessage inRec;
  struct autoRecs
  {
    OMessage* outRecs;
    int32 noOutRecs;
    autoRecs()
    {
      outRecs = 0;
      noOutRecs = 0;
    }
    ~autoRecs()
    {
      if (outRecs != 0)
      {
        free(outRecs);
        noOutRecs = 0;
      }
    }
  } res;
  try
  {
    zzcopy(inRec.NodeId, "Bigger Ears");
    MessageSelect100(&conn, &inRec, &res.noOutRecs, res.outRecs);
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
    catch (TOciApiException& oe)
    {
        printf("%s\n", oe.ErrorDesc);
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

#include <stdint.h>

/**
 * Encode a code point using UTF-8
 *
 * @author Ondřej Hruška <ondra@ondrovo.com>
 * @license MIT
 *
 * @param out - output buffer (min 5 characters), will be 0-terminated
 * @param utf - code point 0-0x10FFFF
 * @return number of bytes on success, 0 on failure (also produces U+FFFD, which uses 3 bytes)
 */
int utf8_encode(char* out, uint32_t utf)
{
  if (utf <= 0x7F) {
    // Plain ASCII
    out[0] = (char)utf;
    out[1] = 0;
    return 1;
  }
  else if (utf <= 0x07FF) {
    // 2-byte unicode
    out[0] = (char)(((utf >> 6) & 0x1F) | 0xC0);
    out[1] = (char)(((utf >> 0) & 0x3F) | 0x80);
    out[2] = 0;
    return 2;
  }
  else if (utf <= 0xFFFF) {
    // 3-byte unicode
    out[0] = (char)(((utf >> 12) & 0x0F) | 0xE0);
    out[1] = (char)(((utf >> 6) & 0x3F) | 0x80);
    out[2] = (char)(((utf >> 0) & 0x3F) | 0x80);
    out[3] = 0;
    return 3;
  }
  else if (utf <= 0x10FFFF) {
    // 4-byte unicode
    out[0] = (char)(((utf >> 18) & 0x07) | 0xF0);
    out[1] = (char)(((utf >> 12) & 0x3F) | 0x80);
    out[2] = (char)(((utf >> 6) & 0x3F) | 0x80);
    out[3] = (char)(((utf >> 0) & 0x3F) | 0x80);
    out[4] = 0;
    return 4;
  }
  else {
    // error - use replacement character
    out[0] = (char)0xEF;
    out[1] = (char)0xBF;
    out[2] = (char)0xBD;
    out[3] = 0;
    return 0;
  }
}

void utf8_encode_str(unsigned char* work, unsigned char* data)
{
  work[0] = 0;
  for (int i = 0; i < 1024 && data[i] != 0; i++)
  {
    uint32_t u = data[i];
    char ch[5];
    int size = utf8_encode(ch, u);
    strcat((char*)work, ch);
  }
}

#include "tbuffer.h"

char* dump(TBChar& buff, unsigned char* data, int len)
{
  char hexLine[81];
  buff.clear();
  buff.append("\n");
  int size;
  for (size = len; size > 0; size--)
  {
    if (data[size - 1] != 0 && data[size - 1] != ' ')
    {
      size--;
      break;
    }
  }
  char printable[64];
  memset(printable, 0, sizeof(printable));
  for (int i = 0; i < size + 1; i++)
  {
    char work[256];
    unsigned char ch = data[i];
    int n = i % 32;
    snprintf(work, sizeof(work), "%02x", ch);
    printable[n] = ch < ' ' ? '.' : ch;
    buff.append(work);
    if (i % 16 == 15)
      buff.append(" ");
    if (i % 32 == 31)
    {
      buff.append(" | ");
      buff.append(printable);
      buff.append("\n");
      memset(printable, 0, sizeof(printable));
    }
  }
  buff.append(" | ");
  buff.append(printable);
  return buff.data;
}

void addMoggy(TJConnector& conn)
{
  unsigned char* xyz = (unsigned char*)"author Ondrej Hruška ÇÜÑñïë 89012";
  printf("%s\n", xyz);
  unsigned char work[1024];
  memset(work, 0, sizeof(work));
  utf8_encode_str(work, xyz);
  printf("%s\n", work);
  HeapRec<DMoggy> heapRec;
  DMoggy& rec = *heapRec.rec;
  TBChar buff1, buff2;
  try
  {
    zzcopy(rec.Descr, work);
    printf("%s\n", (char*)rec.Descr);
    string descr = (char*)work;
    string fred = "\xFF\xFE"+utf::utf8To16(descr);
    memcpy(rec.Fred, fred.data(), fred.length());
    zzcopy(rec.JChar, "01234567890");
    zzcopy(rec.JNChar, "");
    zzcopy(rec.JCharBig, "biggy");
    zzcopy(rec.JNCharBig, "");
    rec.JByte = 123;
    rec.JNByteIsNull = JP_NULL;
    rec.JShort = 1234;
    rec.JNShortIsNull = JP_NULL;
    rec.JInt = 1234567890;
    rec.JNIntIsNull = JP_NULL;
    rec.JLong = 1234567890123456;
    rec.JNLongIsNull = JP_NULL;
    zzcopy(rec.JMoney, "12.7");
    zzcopy(rec.JNMoney, ""); rec.JNMoneyIsNull = JP_NULL;
    rec.JDouble = 12.3;
    rec.JNDoubleIsNull = JP_NULL;
    zzcopy(rec.JDate, "19490607");
    rec.JNDateIsNull = JP_NULL;
    zzcopy(rec.JTime, "120511");
    rec.JNTimeIsNull = JP_NULL;
    zzcopy(rec.JDateTime, "19490607120511");
    rec.JNDateTimeIsNull = JP_NULL;
    zzcopy(rec.USId, "Markell Proost");
    MoggyInsert(&conn, &rec);
    conn.Commit();
    bool result = MoggySelectOne(&conn, &rec);
    if (result == true)
    {
      printf("\nId:%lld\n"
        "Descr:%s\n"
        "Fred:%s\n"
        "JChar:%s\n"
        "JNChar:%s\n"
        "JCharBig:%s\n"
        "JNCharBig:%s\n"
        "JByte:%d\n"
        "JNByte:%d \n"
        "JShort:%d \n"
        "JNShort:%d\n"
        "JInt:%d\n"
        "JNInt:%d\n"
        "JLong:%lld\n"
        "JNLong:%lld\n"
        "JMoney:%s\n"
        "JNMoney:%s\n"
        "JDouble:%f\n"
        "JNDouble:%f\n"
        "JDate:%s\n"
        "JNDate:%s\n"
        "JTime:%s\n"
        "JNTime:%s\n"
        "JDateTime:%s\n"
        "JNDateTime:%s\n"
        "USId:%s\n"
        "TMStamp:%s\n"
        , rec.Id
        , dump(buff1, rec.Descr, sizeof(rec.Descr))
        , dump(buff2, rec.Fred,  sizeof(rec.Fred))
        , rec.JChar
        , rec.JNChar
        , rec.JCharBig
        , rec.JNCharBig
        , rec.JByte
        , rec.JNByte
        , rec.JShort
        , rec.JNShort
        , rec.JInt
        , rec.JNInt
        , rec.JLong
        , rec.JNLong
        , rec.JMoney
        , rec.JNMoney
        , rec.JDouble
        , rec.JNDouble
        , rec.JDate
        , rec.JNDate
        , rec.JTime
        , rec.JNTime
        , rec.JDateTime
        , rec.JNDateTime
        , rec.USId
        , rec.TMStamp
      );
    }
  }
  catch (TOciApiException& ex)
  {
    printf("%s\n", ex.ErrorDesc);
    conn.Rollback();
  }
  catch (...)
  {
    conn.Rollback();
  }
}

void addNoteType(TJConnector& conn)
{
  unsigned char work[1024];
  memset(work, 0, sizeof(work));
  utf8_encode_str(work, (unsigned char*)"Cuniform \x80\x9A\xA5\xA4\x8B\x89.");
  try
  {
    DNoteType rec;
    rec.NoteTypeNo = 101;
    zzcopy(rec.Descr, (char*)work);
    zzcopy(rec.UsrID, "OCI_TEST");
    NoteTypeInsert(&conn, &rec);
    conn.Commit();
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
        printf("%d %s\n", outRecs[i].NoteTypeNo, (unsigned char*)outRecs[i].Descr);
}

int main(int argc, char* argv[])
{
  TJConnector conn;
  conn.Logon("bbdmsg", "oracle", "lv01");
  //addNoteType(conn);
  //listNotes(conn);
  addMoggy(conn);
  //addMessage(conn);
  //listAttachments(conn);
  //get100(conn);
  conn.Logoff();
  return 0;
}