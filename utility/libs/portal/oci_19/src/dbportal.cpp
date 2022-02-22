#include "dbportal.h"

static char *DBErrors[] =
{
    "OK ???"
    , "Logon"
    , "Logoff"
    , "RunProc"
    , "Commit"
    , "Rollback"
    , "Open"
    , "Close"
    , "Exec"
    , "Fetch"
    , "FetchMany"
    , "Cancel"
    , "SetNull"
    , "TestNull"
    , "Cipher"
    , "Ping"
};

using namespace std;

xDBError::xDBError(char *aFname, int aLine, eError aError, int aNo, int aDBNo,
                   char *aMsg, char *aMsg1, char *aMsg2)
    : xCept(aFname, aLine, xDBErrorName, aError)
    , No(aNo)
    , DBNo(aDBNo)
{
    osErr << "Database (" << DBNo << ") " << DBErrors[aError] << " error" << std::endl;
    osErr << "1) " << aNo << ": " << GetSqlError(aNo) << std::endl;
    if (aMsg)  osErr << "2) " << aMsg << std::endl;
    if (aMsg1) osErr << "3) " << aMsg1 << std::endl;
    if (aMsg2) osErr << "4) " << aMsg2 << std::endl;
    osErr << ends;
}

tDBConnect::tDBConnect()
{
    BarCommitFile = 0;
    SqlCB = 0;
}

tDBConnect::~tDBConnect()
{
    if (SqlCB)
        Logoff();
}

void tDBConnect::Logon(char *aBinFile, char *aLogonString)
{
    if (SqlCB)
        Logoff();
    int RC = SqlLogon(&SqlCB, aBinFile, aLogonString);
    if (RC)
    {
        int s = 0;
        for (size_t i = 0; i < strlen(aLogonString); i++)
        {
            if (aLogonString[i] == '@')
                s = 0;
            else if (aLogonString[i] == '/')
                s = 1;
            else if (s == 1)
                aLogonString[i] = '*';
        }
        struct Msg
        {
            char *text;
            Msg(char *m)
            {
                text = strdup(m);
            }
            ~Msg()
            {
                free(text);
                text = 0;
            }
        } msg(ErrorMsg());
        int ErrCode = SqlCB->ociErrCode;
        free(SqlCB);
        SqlCB = 0;
        throw XDBError2(errDBLogon, RC, ErrCode, msg.text, aBinFile, aLogonString);
    }
}

char *tDBConnect::Cipher(char *Input, int InpLen,
                         char *Output, int OutLen,
                         int Seed)
{
    SqlOneWayCipher(Input, InpLen, Output, OutLen, Seed);
    return Output;
}

void tDBConnect::Logoff()
{
    int RC = 0;
    if (SqlCB)
        RC = SqlLogoff(SqlCB);
    if (RC)
        throw XDBError(errDBLogoff, RC, SqlCB->ociErrCode, ErrorMsg());
    SqlCB = 0;
}

char *tDBConnect::ErrorMsg()
{
    if (SqlCB && SqlCB->ociErrCode)
        return SqlCBError(SqlCB);
    return 0;
}

void tDBConnect::RunProc(char *aQuery, void *aData)
{
    int RC = SqlRunProc(SqlCB, aQuery, aData);
    if (RC)
        throw XDBError1(errDBRunProc, RC, SqlCB->ociErrCode, ErrorMsg(), aQuery);
}

bool tDBConnect::ReadOne(char *aQuery, void *aData)
{
    int RC = SqlRunProc(SqlCB, aQuery, aData);
    if (RC)
    {
        if (SqlCB->ociErrCode == OCI_NO_DATA || SqlCB->ociErrCode == 1403)
            return false;
        throw XDBError1(errDBRunProc, RC, SqlCB->ociErrCode, ErrorMsg(), aQuery);
    }
    return true;
}

void tDBConnect::Commit(void)
{
    int RC = SqlCommit(SqlCB);
    if (RC)
        throw XDBError(errDBCommit, RC, SqlCB->ociErrCode, ErrorMsg());
}

void tDBConnect::Rollback(void)
{
    int RC = SqlRollback(SqlCB);
    if (RC)
        throw XDBError(errDBRollback, RC, SqlCB->ociErrCode, ErrorMsg());
}

void tDBConnect::Ping(void)
{
    int RC = SqlPing(SqlCB);
    if (RC)
        throw XDBError(errDBPing, RC, SqlCB->ociErrCode, ErrorMsg());
}

bool tDBConnect::Connected(void)
{
    bool RC = SqlCBConnected(SqlCB);
    return RC;
}

tDBQuery::tDBQuery(tDBConnect &aDBConnect, char *aQuery, void *aData)
{
    int RC = SqlOpen(aDBConnect.SqlCB, &SqlCursor, aQuery, aData);
    if (RC)
        throw XDBError2(errDBOpen, RC, SqlCursor->ociErrCode, ErrorMsg(), aQuery, SqlCursor->Command);
}

tDBQuery::~tDBQuery()
{
    if (SqlCursor)
        Close();
}

void tDBQuery::Close()
{
    int RC = SqlClose(SqlCursor);
    if (RC)
        throw XDBError1(errDBClose, RC, SqlCursor->ociErrCode, ErrorMsg(), SqlCursor->Command);
    SqlCursor = 0;
}

void tDBQuery::Exec()
{
    int RC = SqlExec(SqlCursor);
    if (RC)
        throw XDBError1(errDBExec, RC, SqlCursor->ociErrCode, ErrorMsg(), SqlCursor->Command);
}

bool tDBQuery::Fetch()
{
    int RC = SqlFetch(SqlCursor);
    if (RC)
    {
        if (SqlEOD(SqlCursor))
            return false;
        throw XDBError1(errDBFetch, RC, SqlCursor->ociErrCode, ErrorMsg(), SqlCursor->Command);
    }
    return true;
}

char *tDBQuery::ErrorMsg()
{
    if (SqlCursor->ociErrCode)
        return SqlCursorError(SqlCursor);
    return 0;
}

void tDBQuery::Cancel()
{
    int RC = SqlCancel(SqlCursor);
    if (RC)
        throw XDBError1(errDBCancel, RC, SqlCursor->ociErrCode, ErrorMsg(), SqlCursor->Command);
}

bool tDBQuery::FetchMany(unsigned short *aNo, void *aData)
{
    int RC = SqlFetchMany(SqlCursor, aNo, aData);
    if (RC)
    {
        if (SqlEOD(SqlCursor))
            return false;
        throw XDBError1(errDBFetchMany, RC, SqlCursor->ociErrCode, ErrorMsg(), SqlCursor->Command);
    }
    return true;
}

void tDBQuery::SetNull(char *aColumn, bool aHow)
{
    int RC = SqlSetNull(SqlCursor, aColumn, aHow);
    if (RC)
        throw XDBError1(errDBSetNull, RC, SqlCursor->ociErrCode, ErrorMsg(), SqlCursor->Command);
}

bool tDBQuery::TestNull(char *aColumn)
{
    bool aHow;
    int RC = SqlTestNull(SqlCursor, aColumn, &aHow);
    if (RC)
        throw XDBError1(errDBTestNull, RC, SqlCursor->ociErrCode, ErrorMsg(), SqlCursor->Command);
    return aHow;
}

tHugeCHAR::tHugeCHAR()
{
    length = 0;
    data = 0;
}

tHugeCHAR::~tHugeCHAR()
{
    if (data)
    {
        delete [] data;
        data = 0;
    }
    length = 0;
}

tHugeCHAR &tHugeCHAR::operator=(const tHugeCHAR &inClob)
{
    if (data == inClob.data)
        return *this;
    length = inClob.length;
    if (data)
        delete [] data;
    data = 0;
    if (length > 0)
    {
        data = new char[length];
        memcpy(data, inClob.data, length - 1);
        data[length - 1] = 0;
    }
    return *this;
}

tHugeCHAR &tHugeCHAR::operator=(const char *inData)
{
    if (data)
        delete [] data;
    data = 0;
    if (inData == 0 || inData[0] == 0)
        length = 0;
    else
    {
        length = strlen(inData) + 1;
        data = new char[length];
        memcpy(data, inData, length - 1);
        data[length - 1] = 0;
    }
    return *this;
}

