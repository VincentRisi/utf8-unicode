#include "machine.h"

#include <stdio.h>
#include "xstring.h"
#include <stdlib.h>

#include "sqlcons.h"

#include "sqldef.h"

#include "global.h"
#include "xdir.h"
#include "binio.h"

//static ushort sqldefSignOCI  = 0x0C1A;
static ushort sqldefSignOCI2 = 0x0C2A;
const char nines_pattern[] = "99999999999999999999999999999999999999";
const char zeroes_pattern[] = "00000000000000000000000000000000000000";

static char *UpperName;
static char *ControlName;

static FILE *SqlFile = 0;
static FILE *SqlTableFile = 0;
static FILE *SqlViewsFile = 0;
static FILE *SqlConFile = 0;
static FILE *SqlIdxFile = 0;
static FILE *SqlGrantFile = 0;
static FILE *SqlProcFile = 0;
static FILE *SqlSnapFile = 0;
static FILE *SqlAuditFile = 0;
static FILE *BinFile = 0;

static void GenerateSyn1(FILE * file, const char *Name, const char *FullName, const char *User);
static void GenerateSyn2(FILE * file, const char *Name, const char *FullName, const char *User, const char *ProcName, const char *Under);
static void GenerateSyn3(FILE * file, const char *Name, const char *DBName, const char *User);
static void GenerateSyn4(FILE * file, const char *Name, const char *FullName, const char *User);
static void GenerateSql(PYYTable Table);
static void GenerateTable(PYYTable Table);
static void GenerateAudit(PYYTable Table);
static void GenerateViews(PYYTable Table);
static void GenerateView(PYYTable Table, PYYView View);
static void GenerateSequence(PYYTable Table);
static void GenerateConstraints(PYYTable Table);
static void GenerateKeys(PYYTable Table);
static void GenerateKey(PYYTable Table, PYYKey Key, ushort *PkNo, ushort *UkNo);
static void GenerateNulls(PYYTable Table);
static void GenerateLinks(PYYTable Table);
static void GenerateLink(PYYTable Table, PYYLink Link, short FkNo);
static void GenerateIndexes(PYYTable Table);
static void GenerateIndex(PYYTable Table, PYYKey Key, pshort IxNo);
static void GenerateGrants(PYYTable Table);
static void GenerateGrant(PYYTable Table, PYYGrant Grant);
static void GenerateProcs(PYYTable Table);
static void GenerateStdProcs(PYYTable Table);
static void GenerateInsert(PYYTable Table);
static void GenerateSelectOne(PYYTable Table, bool ForUpd);

static void GenerateConnect(PYYTable Table, FILE* SqlFile);
static void GenerateExit(PYYTable Table, FILE* SqlFile);

static void GenerateDeleteOne(PYYTable Table);
static void GenerateUpdate(PYYTable Table);
static void GenerateUsrProcs(PYYTable Table);
static void GenerateUsrProc(PYYTable Table, PYYProc Proc);
static void GenerateDataLines(PYYProc Proc);

static void GenerateBinary(PYYTable Table);
static void GenerateBinTable(PYYTable Table);
static void GenerateBinProcs(PYYTable Table);
static void GenerateBinSelectAll(PYYTable Table, bool ForUpd);
static void GenerateBinProc(PYYTable Table, const char *ProcName,
    ushort noFields, PYYField Fields,
    ushort ABSize, const char *Code,
    PYYField ExtraField,
    uchar  testSeq);
static void GenerateBinSql(PYYTable Table, PYYProc Proc);

static char *CutUser(const char *aConnect);

static void SelectTypeFormat(PYYField field, ushort &first);
static void WhereTypeFormat(PYYField field, ushort &first);

static const char *ParmFieldType(PYYField Field);
static const char *ParmFieldTypeInsert(PYYField Field);
static const char *ParmFieldTypeUpdate(PYYField Field);
static const char *SqlFieldType(PYYField Field);
static ushort OCIFieldType(PYYField Field);
static short SQLCFieldType(PYYField Field);
static int ApproxFieldSize(PYYField Field);
static void MakeSQLNames(PYYTable Table);
inline void print(const char* name)
{
    printf("Generating %s\n", name);
}
void GenerateOCI(PYYTable Table)
{
    UpperName = strdup(Table->Name);
    if (strlen(Table->ControlDB))
    {
        ControlName = (char *)malloc(strlen(Table->Name) + strlen(Table->ControlDB) + 2);
        strcpy(ControlName, Table->ControlDB);
        strcat(ControlName, ".");
        strcat(ControlName, Table->Name);
    }
    else
        ControlName = strdup(Table->Name);
    strupr(UpperName);
    MakeSQLNames(Table);
    if (Table->TargetSQL)
    {
        print("GenerateSql"), GenerateSql(Table);
    }
    fflush(stdout);
    if (Table->TargetVB || Table->MarkedVB)
    {
        print("GenerateVB"), GenerateVB(Table);
    }
    fflush(stdout);
    if (Table->TargetVB5 || Table->MarkedVB5)
    {
        print("GenerateVB5"), GenerateVB5(Table);
    }
    fflush(stdout);
    if (Table->TargetVBCode3 || Table->MarkedVBCode3)
    {
        print("GenerateVBCode3"), GenerateVBCode3(Table);
    }
    fflush(stdout);
    if (Table->TargetVBNet7 || Table->MarkedVBNet7)
    {
        print("GenerateVBNet7"), GenerateVBNet7(Table);
    }
    fflush(stdout);
    if (Table->TargetCSNet7 || Table->MarkedCSNet7)
    {
        print("GenerateCSNet7"), GenerateCSNet7(Table);
    }
    fflush(stdout);
    if (Table->TargetCSAdoNet || Table->MarkedCSAdoNet)
    {
        print("GenerateCSADoNet"), GenerateCSAdoNet(Table);
    }
    fflush(stdout);
    if (Table->TargetCSRW || Table->MarkedCSRW)
    {
        print("GenerateCSRW"), GenerateCSRW(Table);
    }
    fflush(stdout);
    if (Table->TargetCSIDL2 || Table->MarkedCSIDL2)
    {
        print("GenerateCSIdl2"), GenerateCSIDL2(Table);
    }
    fflush(stdout);
    if (Table->TargetVBforIDL || Table->MarkedVBforIDL)
    {
        print("GenerateVBforIDL"), GenerateVBforIDL(Table);
    }
    fflush(stdout);
    if (Table->TargetVBforADOR || Table->MarkedVBforADOR)
    {
        print("GenerateVBforADOR"), GenerateVBforADOR(Table);
    }
    fflush(stdout);
    if (Table->TargetPAS || Table->MarkedPAS)
    {
        print("GeneratePAS"), GeneratePAS(Table);
    }
    fflush(stdout);
    if (Table->TargetPARMS)
    {
        print("GeneratePARMS"), GeneratePARMS(Table);
    }
    fflush(stdout);
    if (Table->TargetC || Table->MarkedC)
    {
        print("GenerateHeader"), GenerateHeader(Table);
    }
    fflush(stdout);
    if (Table->TargetIDL || Table->MarkedIDL)
    {
        print("GenerateIDL"), GenerateIDL(Table);
    }
    fflush(stdout);
    if (Table->TargetPython || Table->MarkedPython)
    {
        print("GeneratePython"), GeneratePython(Table);
    }
    fflush(stdout);
    if (Table->TargetPyDBApi)
    {
        print("GeneratePyDBApi"), GeneratePyDBApi(Table);
    }
    fflush(stdout);
    if (Table->TargetSO)
    {
        print("GenerateBinary"), GenerateBinary(Table);
    }
    fflush(stdout);
    if (Table->TargetARCHIVE)
    {
        print("GenerateVBCode3"), GenerateArchive(Table);
    }
    fflush(stdout);
    if (Table->TargetSwagger)
    {
        print("GenerateSwagger"), GenerateSwagger(Table);
    }
    fflush(stdout);
    if (Table->TargetOpenapi)
    {
        print("GenerateOpenapi"), GenerateOpenapi(Table);
    }
    fflush(stdout);
    if (Table->TargetJPORTAL)
    {
        print("GenerateJPORTAL"), GenerateJPORTAL(Table);
    }
    fflush(stdout);
    free(UpperName);
    free(ControlName);
}

static FILE* OpenFile(const char *FileFlag, const char *FileName, const char *Mode = "wt")
{
    char Work[512];
    sprintf(Work, "%-13.13s: %s\n", FileFlag, FileName);
    yyerror(Work);
    FILE* File = fopen(FileName, Mode);
    if (File == 0)
        yyerror("Cannot open %s : %s\n", FileFlag, FileName);
    else
        setvbuf(File, 0, _IOFBF, 32768);
    return File;
}

static void MakeSQLNames(PYYTable Table)
{
    char newpath[512];
    char dir[512];
    char ext[512];
    char fname[512];
    FNameSplit(Table->InputFileName, dir, fname, ext);
    if (Table->ShortName == 0)
        free(Table->ShortName);
    Table->ShortName = strdup(fname);
    if (Table->ShortName == 0)
        free(Table->ShortName);
    Table->ShortName = strdup(fname);
    if (strlen(Table->SqlDir))
        FNameMerge(newpath, Table->SqlDir, fname, Table->SqlExt);
    else
        FNameMerge(newpath, dir, fname, Table->SqlExt);
    Table->SqlFileName = strdup(newpath);
    if (strlen(Table->LogDir))
        FNameMerge(newpath, Table->LogDir, fname, Table->LogExt);
    else
        FNameMerge(newpath, dir, fname, Table->LogExt);
    Table->LogFileName = strdup(newpath);
    if (Table->OneSQLScript == 0)
    {
        if (strlen(Table->SqlDir))
            FNameMerge(newpath, Table->SqlDir, fname, Table->SqlConExt);
        else
            FNameMerge(newpath, dir, fname, Table->SqlConExt);
        Table->SqlConFileName = strdup(newpath);
        if (strlen(Table->SqlDir))
            FNameMerge(newpath, Table->SqlDir, fname, Table->SqlTableExt);
        else
            FNameMerge(newpath, dir, fname, Table->SqlTableExt);
        Table->SqlTableFileName = strdup(newpath);
        if (strlen(Table->SqlDir))
            FNameMerge(newpath, Table->SqlDir, fname, Table->SqlGrantExt);
        else
            FNameMerge(newpath, dir, fname, Table->SqlGrantExt);
        Table->SqlGrantFileName = strdup(newpath);
        if (strlen(Table->SqlDir))
            FNameMerge(newpath, Table->SqlDir, fname, Table->SqlProcExt);
        else
            FNameMerge(newpath, dir, fname, Table->SqlProcExt);
        Table->SqlProcFileName = strdup(newpath);
        if (strlen(Table->SqlDir))
            FNameMerge(newpath, Table->SqlDir, fname, Table->SqlViewsExt);
        else
            FNameMerge(newpath, dir, fname, Table->SqlViewsExt);
        Table->SqlViewsFileName = strdup(newpath);
        if (strlen(Table->SqlDir))
            FNameMerge(newpath, Table->SqlDir, fname, Table->SqlIdxExt);
        else
            FNameMerge(newpath, dir, fname, Table->SqlIdxExt);
        Table->SqlIdxFileName = strdup(newpath);
    }
    if (strlen(Table->SqlDir))
        FNameMerge(newpath, Table->SqlDir, fname, Table->SqlSnapExt);
    else
        FNameMerge(newpath, dir, fname, Table->SqlSnapExt);
    Table->SqlSnapFileName = strdup(newpath);
    if (strlen(Table->SqlDir))
        FNameMerge(newpath, Table->SqlDir, fname, Table->SqlAuditExt);
    else
        FNameMerge(newpath, dir, fname, Table->SqlAuditExt);
    Table->SqlAuditFileName = strdup(newpath);
}

static void GenerateConnect(PYYTable Table, FILE* SqlFile)
{
    if (Table->ConnReqd)
    {
        if (Table->Server[0] == '@')
            fprintf(SqlFile, "CONNECT %s/%s%s\n\n"
                "SET HEADING OFF\n"
                "SPOOL %s\n\n", Table->UserID, Table->PassWD,
                Table->Server, Table->LogFileName);
        else
            fprintf(SqlFile, "CONNECT %s/%s\n\n"
                "SET HEADING OFF\n"
                "SPOOL %s\n\n", Table->UserID, Table->PassWD, Table->LogFileName);
    }
    else
        fprintf(SqlFile, "SET HEADING OFF\n"
            "SPOOL %s.log\n\n", Table->LogFileName);
}

static void GenerateExit(PYYTable Table, FILE* SqlFile)
{
    if (Table->ExitReqd)
        fprintf(SqlFile, "\nSPOOL OFF"
            "\nEXIT;\n\n");
}

static void GenerateSql(PYYTable Table)
{
    SqlFile = OpenFile("SQL File", Table->SqlFileName);
    if (SqlFile == 0)
        return;
    if (strlen(Table->Name) > 0)
    {
        GenerateConnect(Table, SqlFile);
        fprintf(SqlFile, "select 'LAST RUN: '||to_char(sysdate, 'YYYY/MM/DD HH24:MI:SS') \"LASTRUN\" from dual;\n\n");
        char Work[512];
        sprintf(Work, "%-13.13s: %s\n", "Log File", Table->LogFileName);
        yyerror(Work);
        GenerateTable(Table);
        if (Table->hasAudit)
            GenerateAudit(Table);
        GenerateConstraints(Table);
        GenerateIndexes(Table);
        if (Table->noGrants)
            GenerateGrants(Table);
        GenerateViews(Table);
        GenerateProcs(Table);
        GenerateExit(Table, SqlFile);
    }
    fclose(SqlFile);
}

static void GenerateSyn1(FILE * file, const char *Name, const char *FullName, const char *User)
{
    fprintf(file, "DROP PUBLIC SYNONYM %s;\n"
        "CREATE PUBLIC SYNONYM %s for %s;\n"
        "GRANT SELECT, INSERT, DELETE, UPDATE "
        "on %s to %s;\n\n", Name,
        Name, FullName,
        FullName, User);
}

static void GenerateSyn2(FILE * file, const char *Name, const char *FullName, const char *User,
    const char *ProcName, const char *Under)
{
    fprintf(file, "DROP PUBLIC SYNONYM %s%s%s;\n"
        "CREATE PUBLIC SYNONYM %s%s%s for %s%s%s;\n"
        "GRANT EXECUTE on %s%s%s to %s;\n\n", Name, Under, ProcName,
        Name, Under, ProcName,
        FullName, Under, ProcName,
        FullName, Under, ProcName, User);
}

static void GenerateSyn3(FILE * file, const char *Name, const char *DBName, const char *User)
{
    fprintf(file, "DROP PUBLIC SYNONYM %s;\n"
        "CREATE PUBLIC SYNONYM %s for %s.%s;\n"
        "GRANT SELECT on %s.%s to %s;\n\n", Name, Name, DBName, Name, DBName, Name, User);
}

static void GenerateSyn4(FILE * file, const char *Name, const char *FullName, const char *User)
{
    fprintf(file, "DROP PUBLIC SYNONYM %sSeq;\n"
        "CREATE PUBLIC SYNONYM %sSeq for %sSeq;\n"
        "GRANT SELECT on %sSeq to %s;\n\n", Name,
        Name, FullName,
        FullName, User);
}

static void GenerateSyn5(FILE * file, const char *Name, const char *FullName, const char *User)
{
    fprintf(file, "DROP PUBLIC SYNONYM %sAudit;\n"
        "CREATE PUBLIC SYNONYM %sAudit for %sAudit;\n"
        "GRANT SELECT on %sAudit to %s;\n\n", Name,
        Name, FullName,
        FullName, User);
}

static void GenerateConstraints(PYYTable Table)
{
    if (Table->OneSQLScript == 0)
    {
        fprintf(SqlFile, "\n@%s\n", Table->SqlConFileName);
        SqlConFile = OpenFile("SQL File(Con)", Table->SqlConFileName);
        if (SqlConFile == 0)
            return;
        GenerateConnect(Table, SqlConFile);
    }
    else
        SqlConFile = SqlFile;
    GenerateLinks(Table);
    if (Table->OneSQLScript == 0)
    {
        GenerateExit(Table, SqlConFile);
        fclose(SqlConFile);
    }
}

static void GenerateNulls(PYYTable Table)
{
    ushort i, n, m;
    for (i = 0; i < Table->noFields; i++)
        if (!Table->Fields[i].isNull)
            break;
    if (i >= Table->noFields)
        return;
    fprintf(SqlTableFile, "ALTER TABLE %s\n"
        "MODIFY(", ControlName);
    for (i = n = 0, m = 1; i < Table->noFields; i++)
    {
        if (!Table->Fields[i].isNull)
            fprintf(SqlTableFile, "\n%s %s CONSTRAINT %s_NN%02d NOT NULL",
                n++ ? "," : " ",
                Table->Fields[i].Name,
                Table->Name,
                m++);
    }
    fprintf(SqlTableFile, "\n);\n");
}

static void GenerateProcs(PYYTable Table)
{
    if (Table->OneSQLScript == 0)
    {
        fprintf(SqlFile, "\n@%s\n", Table->SqlProcFileName);
        SqlProcFile = OpenFile("SQL File(Pro)", Table->SqlProcFileName);
        if (SqlProcFile == 0)
            return;
        GenerateConnect(Table, SqlProcFile);
    }
    else
        SqlProcFile = SqlFile;
    if (strlen(Table->UConnect) && strchr(Table->UConnect, '/'))
        fprintf(SqlProcFile, "connect %s\n\n", Table->UConnect);
    if (Table->Internal == 0)
        GenerateStdProcs(Table);
    GenerateUsrProcs(Table);
    if (Table->OneSQLScript == 0)
    {
        GenerateExit(Table, SqlProcFile);
        fclose(SqlProcFile);
    }
}

static void GenerateStdProcs(PYYTable Table)
{
    if (Table->hasSelectOne && Table->hasPrimaryKey)
        GenerateSelectOne(Table, false);
    if (Table->hasSelectOneUpd && Table->hasPrimaryKey)
        GenerateSelectOne(Table, true);
    if (Table->hasDeleteOne && Table->hasPrimaryKey)
        GenerateDeleteOne(Table);
    if (Table->hasUpdate && Table->hasPrimaryKey)
        GenerateUpdate(Table);
    if (Table->hasInsert)
        GenerateInsert(Table);
}

static char *ProcSuffix(PYYTable Table)
{
    static char result[64];
    if (strlen(Table->UConnect))
    {
        sprintf(result, "%s.%s", CutUser(Table->UConnect), Table->Name);
        return result;
    }
    return ControlName;
}

static void ExecuteGrant(PYYTable Table, const char *ProcName, const char *User)
{
    if (strlen(Table->UConnect) == 0)
        fprintf(SqlProcFile, "GRANT EXECUTE ON %s.%s%s%s TO %s;\n\n",
            strlen(Table->ControlDB) ? Table->ControlDB : Table->UserID,
            Table->Name,
            Table->UnderScore ? "_" : "",
            ProcName,
            User);
    else
        fprintf(SqlProcFile, "GRANT EXECUTE ON %s%s%s TO %s;\n\n",
            Table->Name,
            Table->UnderScore ? "_" : "",
            ProcName,
            User);
}

static void GenerateInsert(PYYTable Table)
{
    ushort i;
    if (Table->noFields == 0) return;
    fprintf(SqlProcFile, "CREATE OR REPLACE PROCEDURE %s%sInsert\n"
        "(\n", ProcSuffix(Table), Table->UnderScore ? "_" : "");
    for (i = 0; i < Table->noFields; i++)
        fprintf(SqlProcFile, "%c a%s%s %s\n",
            i == 0 ? ' ' : ',',
            Table->UnderScore ? "_" : "",
            Table->Fields[i].Name,
            ParmFieldTypeInsert(&Table->Fields[i]));
    fprintf(SqlProcFile, ") as\n"
        "BEGIN\n");//, Table->Name);
    for (i = 0; i < Table->noFields; i++)
    {
        if (Table->Fields[i].isRowSequence)
            fprintf(SqlProcFile, "  select RowSequence.NEXTVAL into a%s%s from dual;\n",
                Table->UnderScore ? "_" : "",
                Table->Fields[i].Name);
        else if (Table->Fields[i].isSequence)
            fprintf(SqlProcFile, "  select %sSeq.NEXTVAL into a%s%s from dual;\n", Table->Name,
                Table->UnderScore ? "_" : "",
                Table->Fields[i].Name);
        else if (Table->Fields[i].Type == ftypeTimeStamp)
            fprintf(SqlProcFile, "  a%s%s := to_char(SYSDATE, 'YYYYMMDDHH24MISS');\n",
                Table->UnderScore ? "_" : "",
                Table->Fields[i].Name);
        else if (Table->Fields[i].Type == ftypeUserStamp)
            fprintf(SqlProcFile, "  a%s%s := SUBSTR(USER, 1, 8);\n",
                Table->UnderScore ? "_" : "",
                Table->Fields[i].Name);
        else if (Table->Fields[i].Type == ftypeStatus)
            fprintf(SqlProcFile, "  a%s%s := 'I';\n",
                Table->UnderScore ? "_" : "",
                Table->Fields[i].Name);
    }
    fprintf(SqlProcFile, "  insert into %s\n"
        "  (\n", Table->Name);
    for (i = 0; i < Table->noFields; i++)
        fprintf(SqlProcFile, "  %c %s\n", i == 0 ? ' ' : ',', Table->Fields[i].Name);
    fprintf(SqlProcFile, "  ) values\n");//, Table->Name);
    for (i = 0; i < Table->noFields; i++)
        if (Table->Fields[i].Type == ftypeTimeStamp)
            fprintf(SqlProcFile, "  %c SYSDATE\n", i == 0 ? ' ' : ',');
        else
            fprintf(SqlProcFile,
                Table->Fields[i].Type == ftypeDate
                ? "  %c to_date(a%s%s,'YYYYMMDD')\n"
                : Table->Fields[i].Type == ftypeDateTime
                ? "  %c to_date(a%s%s,'YYYYMMDDHH24MISS')\n"
                : Table->Fields[i].Type == ftypeMoney
                ? "  %c to_number(a%s%s)\n"
                : "  %c a%s%s\n",
                i == 0 ? ' ' : ',',
                Table->UnderScore ? "_" : "",
                Table->Fields[i].Name);
    fprintf(SqlProcFile, "  );\n");
    fprintf(SqlProcFile, "END;\n/\nl\nshow errors\n\n");
    GenerateSyn2(SqlProcFile, Table->Name, ProcSuffix(Table), Table->UserID, "Insert", Table->UnderScore ? "_" : "");
    for (i = 0; i < Table->noInsertUsers; i++)
        ExecuteGrant(Table, "Insert", Table->InsertUsers[i].p);
    for (i = 0; i < Table->noExecuteUsers; i++)
        ExecuteGrant(Table, "Insert", Table->ExecuteUsers[i].p);
}

static void SelectTypeFormat(PYYField field, ushort &first)
{
    const char* format;
    const char* coding = "";
    switch (field->Type)
    {
    case ftypeTimeStamp:
        format = "  %c to_char(%s%s)\n";
        coding = ",'YYYYMMDDHH24MISS'";
        break;
    case ftypeNChar:
        if (field->isSequence)
        {
            coding = zeroes_pattern + sizeof(zeroes_pattern) - field->Length;
            format = "  %c to_char(%s,'FM%s')\n";
        }
        else
            format = "  %c to_char(%s%s)\n";
        break;
    case ftypeDate:
        format = "  %c to_char(%s%s)\n";
        coding = ",'YYYYMMDD'";
        break;
    case ftypeDateTime:
        format = "  %c to_char(%s%s)\n";
        coding = ",'YYYYMMDDHH24MISS'";
        break;
    case ftypeMoney:
        format = "  %c to_char(%s)\n";
        break;
    default:
        format = "  %c %s\n";
        break;
    }
    fprintf(SqlProcFile, format,
        first == 0 ? ' ' : ',',
        field->Name,
        coding);
    first = 1;
}

static void WhereTypeFormat(PYYField field, ushort &first)
{
    //
    const char* format;
    switch (field->Type)
    {
    case ftypeDate:
        format = "  %5s %s=to_date(a%s%s,'YYYYMMDD')\n";
        break;
    case ftypeDateTime:
        format = "  %5s %s=to_date(a%s%s,'YYYYMMDDHH24MISS')\n";
        break;
    case ftypeMoney:
        format = "  %5s %s=to_number(a%s%s)\n";
        break;
    default:
        format = "  %5s %s=a%s%s\n";
        break;
    }
    fprintf(SqlProcFile, format,
            first == 0 ? "where" : "and",
            field->Name,
            Table->UnderScore ? "_" : "",
            field->Name);
    first = 1;

}

static void GenerateSelectOne(PYYTable Table, bool ForUpd)
{
    ushort i;
    ushort first;
    if (Table->noFields == 0) return;
    fprintf(SqlProcFile, "CREATE OR REPLACE PROCEDURE %s%sSelectOne%s\n"
        "(\n",
        ProcSuffix(Table),
        Table->UnderScore ? "_" : "",
        ForUpd ? "Upd" : "");
    for (i = 0; i < Table->noFields; i++)
        fprintf(SqlProcFile, "%c a%s%s %s %s\n",
            i == 0 ? ' ' : ',',
            Table->UnderScore ? "_" : "",
            Table->Fields[i].Name,
            Table->Fields[i].isPrimaryKey ? "in" : "in out",
            ParmFieldType(&Table->Fields[i]));
    fprintf(SqlProcFile, ") as\n"
        "BEGIN\n"
        "  select\n");
    for (i = 0, first = 0; i < Table->noFields; i++)
        if (Table->Fields[i].isPrimaryKey == 0)
            SelectTypeFormat(&Table->Fields[i], first);
    fprintf(SqlProcFile, "  into\n");
    for (i = 0, first = 0; i < Table->noFields; i++)
        if (Table->Fields[i].isPrimaryKey == 0)
        {
            fprintf(SqlProcFile, "  %c a%s%s\n",
                first == 0 ? ' ' : ',',
                Table->UnderScore ? "_" : "",
                Table->Fields[i].Name);
            first = 1;
        }
    fprintf(SqlProcFile, "  from %s\n", Table->Name);
    for (i = 0, first = 0; i < Table->noFields; i++)
        if (Table->Fields[i].isPrimaryKey)
            WhereTypeFormat(&Table->Fields[i], first);
    if (ForUpd)
        fprintf(SqlProcFile, "  for update");
    fprintf(SqlProcFile, "  ;\n");
    fprintf(SqlProcFile, "END;\n/\nl\nshow errors\n\n");
    if (ForUpd)
        GenerateSyn2(SqlProcFile, Table->Name, ProcSuffix(Table), Table->UserID, "SelectOneUpd", Table->UnderScore ? "_" : "");
    else
        GenerateSyn2(SqlProcFile, Table->Name, ProcSuffix(Table), Table->UserID, "SelectOne", Table->UnderScore ? "_" : "");
    if (ForUpd)
    {
        for (i = 0; i < Table->noSelectOneUpdUsers; i++)
            ExecuteGrant(Table, "SelectOneUpd", Table->InsertUsers[i].p);
        for (i = 0; i < Table->noExecuteUsers; i++)
            ExecuteGrant(Table, "SelectOneUpd", Table->ExecuteUsers[i].p);
    }
    else
    {
        for (i = 0; i < Table->noSelectOneUsers; i++)
            ExecuteGrant(Table, "SelectOne", Table->InsertUsers[i].p);
        for (i = 0; i < Table->noExecuteUsers; i++)
            ExecuteGrant(Table, "SelectOne", Table->ExecuteUsers[i].p);
    }
}


static void GenerateDeleteOne(PYYTable Table)
{
    ushort i;
    ushort first;
    if (Table->noFields == 0) return;
    if (Table->noKeys == 0) return;
    fprintf(SqlProcFile, "CREATE OR REPLACE PROCEDURE %s%sDeleteOne\n"
        "(\n", ProcSuffix(Table), Table->UnderScore ? "_" : "");
    for (i = 0, first = 0; i < Table->Keys[0].noFields; i++)
    {
        fprintf(SqlProcFile, "%c a%s%s in %s\n",
            first == 0 ? ' ' : ',',
            Table->UnderScore ? "_" : "",
            Table->Keys[0].Fields[i].Name,
            ParmFieldType(&Table->Keys[0].Fields[i]));
        first = 1;
    }
    fprintf(SqlProcFile, ") as\n"
        "BEGIN\n"
        "  delete from %s\n", Table->Name);
    for (i = 0, first = 0; i < Table->Keys[0].noFields; i++)
    {
        fprintf(SqlProcFile,
            Table->Keys[0].Fields[i].Type == ftypeDate
            ? "  %5s %s=to_date(a%s%s, 'YYYYMMDD')\n"
            : Table->Keys[0].Fields[i].Type == ftypeDateTime
            ? "  %5s %s=to_date(a%s%s, 'YYYYMMDDHH24MISS')\n"
            : Table->Keys[0].Fields[i].Type == ftypeMoney
            ? "  %5s %s=to_number(a%s%s)\n"
            : "  %5s %s=a%s%s\n",
            first == 0 ? "where" : "and",
            Table->Keys[0].Fields[i].Name,
            Table->UnderScore ? "_" : "",
            Table->Keys[0].Fields[i].Name);
        first = 1;
    }
    fprintf(SqlProcFile, "  ;\n"
        "END;\n/\nl\nshow errors\n\n");
    GenerateSyn2(SqlProcFile, Table->Name, ProcSuffix(Table), Table->UserID, "DeleteOne", Table->UnderScore ? "_" : "");
    for (i = 0; i < Table->noDeleteOneUsers; i++)
        ExecuteGrant(Table, "DeleteOne", Table->InsertUsers[i].p);
    for (i = 0; i < Table->noExecuteUsers; i++)
        ExecuteGrant(Table, "DeleteOne", Table->ExecuteUsers[i].p);
}

static void GenerateUpdate(PYYTable Table)
{
    ushort i;
    ushort first;
    uchar hasTimeStamp = 0;

    // cannot have an update if no non-primary-key fields
    if (Table->noFields - Table->PrimaryKeyFields <= 0)
        return;

    fprintf(SqlProcFile, "CREATE OR REPLACE PROCEDURE %s%sUpdate\n"
        "(\n", ProcSuffix(Table), Table->UnderScore ? "_" : "");
    for (i = 0, first = 0; i < Table->noFields; i++)
    {
        if (Table->Fields[i].isSequence == 1
            && Table->Fields[i].isPrimaryKey == 0)
            continue;
        if (Table->Fields[i].isRowSequence == 1)
            continue;
        if (Table->Fields[i].Type == ftypeTimeStamp)
            hasTimeStamp = 1;
        fprintf(SqlProcFile, "%c a%s%s %s\n",
            first == 0 ? ' ' : ',',
            Table->UnderScore ? "_" : "",
            Table->Fields[i].Name,
            ParmFieldTypeUpdate(&Table->Fields[i]));
        first = 1;
    }
    fprintf(SqlProcFile, ") as\n"
        "wSYSDATE date;\n"
        "BEGIN\n");
    if (hasTimeStamp)
        fprintf(SqlProcFile, "  wSYSDATE := SYSDATE;\n");
    for (i = 0; i < Table->noFields; i++)
    {
        if (Table->Fields[i].Type == ftypeTimeStamp)
            fprintf(SqlProcFile, "  a%s%s := to_char(wSYSDATE, 'YYYYMMDDHH24MISS');\n",
                Table->UnderScore ? "_" : "",
                Table->Fields[i].Name);
        else if (Table->Fields[i].Type == ftypeUserStamp)
            fprintf(SqlProcFile, "  a%s%s := SUBSTR(USER, 1, 8);\n",
                Table->UnderScore ? "_" : "",
                Table->Fields[i].Name);
        else if (Table->Fields[i].Type == ftypeStatus)
            fprintf(SqlProcFile, "  a%s%s := 'U';\n",
                Table->UnderScore ? "_" : "",
                Table->Fields[i].Name);
    }
    fprintf(SqlProcFile, "  update %s\n"
        "  set\n", Table->Name);
    for (i = 0, first = 0; i < Table->noFields; i++)
        if (Table->Fields[i].isPrimaryKey == 0
            && Table->Fields[i].isSequence == 0
            && Table->Fields[i].isRowSequence == 0)
        {
            if (Table->Fields[i].Type == ftypeTimeStamp)
                fprintf(SqlProcFile, "  %c %s = wSYSDATE\n", first == 0 ? ' ' : ',',
                    Table->Fields[i].Name);
            else
                fprintf(SqlProcFile,
                    Table->Fields[i].Type == ftypeDate
                    ? "  %c %s = to_date(a%s%s,'YYYYMMDD')\n"
                    : Table->Fields[i].Type == ftypeDateTime
                    ? "  %c %s = to_date(a%s%s,'YYYYMMDDHH24MISS')\n"
                    : Table->Fields[i].Type == ftypeMoney
                    ? "  %c %s = to_number(a%s%s)\n"
                    : "  %c %s = a%s%s\n",
                    first == 0 ? ' ' : ',',
                    Table->Fields[i].Name,
                    Table->UnderScore ? "_" : "",
                    Table->Fields[i].Name);
            first = 1;
        }
    for (i = 0, first = 0; i < Table->noFields; i++)
        if (Table->Fields[i].isPrimaryKey)
        {
            fprintf(SqlProcFile,
                Table->Fields[i].Type == ftypeDate
                ? "  %5s %s = to_date(a%s%s,'YYYYMMDD')\n"
                : Table->Fields[i].Type == ftypeDateTime
                ? "  %5s %s = to_date(a%s%s,'YYYYMMDDHH24MISS')\n"
                : Table->Fields[i].Type == ftypeMoney
                ? "  %5s %s = to_number(a%s%s)\n"
                : "  %5s %s = a%s%s\n",
                first == 0 ? "where" : "and  ",
                Table->Fields[i].Name,
                Table->UnderScore ? "_" : "",
                Table->Fields[i].Name);
            first = 1;
        }
    fprintf(SqlProcFile, "  ;\n"
        "END;\n/\nl\nshow errors\n\n");
    GenerateSyn2(SqlProcFile, Table->Name, ProcSuffix(Table), Table->UserID, "Update", Table->UnderScore ? "_" : "");
    for (i = 0; i < Table->noUpdateUsers; i++)
        ExecuteGrant(Table, "Update", Table->InsertUsers[i].p);
    for (i = 0; i < Table->noExecuteUsers; i++)
        ExecuteGrant(Table, "Update", Table->ExecuteUsers[i].p);
}

static void GenerateUsrProcs(PYYTable Table)
{
    ushort i, j;
    for (i = 0; i < Table->noProcs; i++)
    {
        if (Table->Procs[i].isData)
        {
            GenerateDataLines(&Table->Procs[i]);
            continue;
        }
        else
            GenerateUsrProc(Table, &Table->Procs[i]);
        if (!Table->Procs[i].isSql)
        {
            for (j = 0; j < Table->Procs[i].noUsers; j++)
                ExecuteGrant(Table, Table->Procs[i].Name, Table->Procs[i].Users[j].p);
            for (j = 0; j < Table->noExecuteUsers; j++)
                ExecuteGrant(Table, Table->Procs[i].Name, Table->ExecuteUsers[j].p);
        }
    }
}

static void GenerateUsrProc(PYYTable Table, PYYProc Proc)
{
    ushort i, j;
    if (Proc->isSql)
        return;
    fprintf(SqlProcFile, Proc->noFields
        ? "CREATE OR REPLACE PROCEDURE %s%s%s\n"
        "(\n"
        : "CREATE OR REPLACE PROCEDURE %s%s%s\n"
        "as\n",
        ProcSuffix(Table),
        Table->UnderScore ? "_" : "",
        Proc->Name);
    for (i = 0; i < Proc->noFields; i++)
        fprintf(SqlProcFile, "%c a%s%s %s %s %s\n",
            i == 0 ? ' ' : ',',
            Table->UnderScore ? "_" : "",
            Proc->Fields[i].Name,
            Proc->Fields[i].isInput ? "IN" : "",
            Proc->Fields[i].isOutput ? "OUT" : "",
            ParmFieldType(&Proc->Fields[i])
        );
    if (Proc->noFields)
        fprintf(SqlProcFile, ") as\n");
    for (i = 0; i < Proc->noWorkFields; i++)
        fprintf(SqlProcFile, "%s %s;\n",
            Proc->WorkFields[i].Name,
            SqlFieldType(&Proc->WorkFields[i])
        );
    for (i = 0; i < Proc->noCursors; i++)
    {
        PYYCursor Cursor = &Proc->Cursors[i];
        for (j = 0; j < Cursor->noLines; j++)
        {
            fprintf(SqlProcFile, j == 0
                ? "CURSOR %s%s\n"
                : "  %s%s\n",
                Cursor->Lines[j].p,
                (j + 1) == Cursor->noLines ? ";" : ""
            );
        }
    }
    fprintf(SqlProcFile, "BEGIN\n");
    for (i = 0; i < Proc->noLines; i++)
    {
        fprintf(SqlProcFile, "%s", Proc->Lines[i].p);
        //if (Proc->Lines[i].isEOL)
        //  fprintf(SqlProcFile, "\n");
    }
    fprintf(SqlProcFile, "END;\n/\nl\nshow errors\n\n");
    GenerateSyn2(SqlProcFile, Table->Name, ProcSuffix(Table), Table->UserID, Proc->Name, Table->UnderScore ? "_" : "");
}

static void GenerateDataLines(PYYProc Proc)
{
    ushort i;
    for (i = 0; i < Proc->noLines; i++)
        fprintf(SqlProcFile, "%s\n", Proc->Lines[i].p);
}

static void GenerateViews(PYYTable Table)
{
    ushort i;
    if (Table->noViews == 0)
        return;
    if (Table->OneSQLScript == 0)
    {
        fprintf(SqlFile, "\n@%s\n", Table->SqlViewsFileName);
        SqlViewsFile = OpenFile("SQL File(Vws)", Table->SqlViewsFileName);
        if (SqlViewsFile == 0)
            return;
        GenerateConnect(Table, SqlViewsFile);
    }
    else
        SqlViewsFile = SqlFile;
    for (i = 0; i < Table->noViews; i++)
    {
        GenerateView(Table, &Table->Views[i]);
    }
    if (Table->OneSQLScript == 0)
    {
        GenerateExit(Table, SqlViewsFile);
        fclose(SqlViewsFile);
    }
}

static char *CutUser(const char *aConnect)
{
    static char result[64];
    int i;
    for (i = 0; i < 64 && aConnect[i] && aConnect[i] != '/'; i++)
        result[i] = aConnect[i];
    result[i] = 0;
    return result;
}

static void GenerateView(PYYTable Table, PYYView View)
{
    ushort i;
    fprintf(SqlViewsFile, "CREATE OR REPLACE FORCE VIEW %s%s%s as\n"
        "(\n",
        strlen(Table->ControlDB) ? Table->ControlDB : "",
        strlen(Table->ControlDB) ? "." : "",
        View->Name);
    for (i = 0; i < View->noLines; i++)
        fprintf(SqlViewsFile, "%s\n", View->Lines[i].p);
    fprintf(SqlViewsFile, ");\n"
        "l\n"
        "show errors\n");
    for (i = 0; i < View->noUsers; i++)
        fprintf(SqlViewsFile, "GRANT SELECT ON %s%s%s TO %s;\n\n",
            strlen(Table->ControlDB) ? Table->ControlDB : "",
            strlen(Table->ControlDB) ? "." : "",
            View->Name, View->Users[i].p);
    for (i = 0; i < Table->noSelectUsers; i++)
        fprintf(SqlViewsFile, "GRANT SELECT ON %s%s%s TO %s;\n\n",
            strlen(Table->ControlDB) ? Table->ControlDB : "",
            strlen(Table->ControlDB) ? "." : "",
            View->Name, Table->SelectUsers[i].p);
    if (strlen(Table->ControlDB))
        GenerateSyn3(SqlViewsFile, View->Name, Table->ControlDB,
            strlen(Table->UConnect) ? CutUser(Table->UConnect) : Table->UserID);
}

static void GenerateAudit(PYYTable Table)
{
    ushort i;
    PYYField Field;

    if (Table->noFields == 0) return;
    SqlAuditFile = OpenFile("SQL File(Tab)", Table->SqlAuditFileName);
    if (SqlAuditFile == 0)
        return;
    GenerateConnect(Table, SqlAuditFile);
    fprintf(SqlAuditFile, "SET ECHO ON\n\n");
    fprintf(SqlAuditFile, "SET SHOW ON\n\n");
    fprintf(SqlAuditFile, "SPOOL %8.8s.LOG\n\n", Table->Name);
    fprintf(SqlAuditFile, "REMARK AUDIT TABLE\n\n");
    fprintf(SqlAuditFile, "DROP TABLE %sAudit;\n\n", ControlName);
    fprintf(SqlAuditFile, "CREATE TABLE %sAudit\n"
        "("
        "\n  AuditChangeSeq number(12)"
        "\n ,AuditChangeAction char(2)", ControlName);
    for (i = 0; i < Table->noFields; i++)
    {
        Field = &Table->Fields[i];
        fprintf(SqlAuditFile, "\n ,%s %s", Field->Name, SqlFieldType(Field));
    }
    fprintf(SqlAuditFile, "\n ,AuditChangeUserID char(8)"
        "\n ,AuditChangeTmStamp date"
        "\n ,AuditChangeRowID rowid");
    fprintf(SqlAuditFile, "\n);\n\n");
    fprintf(SqlAuditFile, "DROP SEQUENCE %sAuditSeq;\n\n", ControlName);
    fprintf(SqlAuditFile, "CREATE SEQUENCE %sAuditSeq\n"
        "  MINVALUE 1\n"
        "  MAXVALUE 999999999999\n"
        "  CYCLE\n"
        "  ORDER;\n\n", ControlName);
    fprintf(SqlAuditFile, "DROP TRIGGER %sTrig;\n\n", ControlName);
    fprintf(SqlAuditFile, "CREATE TRIGGER %sTrig\n", ControlName);
    if (Table->hasAuditFull)
        fprintf(SqlAuditFile, "AFTER INSERT OR UPDATE OR DELETE ON %s FOR EACH ROW\n"
            "DECLARE\n"
            "  Oper1 char(2);\n"
            "  Oper2 char(2);\n"
            "BEGIN\n"
            "  IF INSERTING THEN\n"
            "    Oper2 := 'NA';\n"
            "  END IF;\n"
            "  IF UPDATING THEN\n"
            "    Oper1 := 'OC';\n"
            "    Oper2 := 'NC';\n"
            "  END IF;\n"
            "  IF DELETING THEN\n"
            "    Oper1 := 'OD';\n"
            "  END IF;\n", ControlName);
    else
        fprintf(SqlAuditFile, "AFTER UPDATE OR DELETE ON %s FOR EACH ROW\n"
            "DECLARE\n"
            "  Oper1 char(2);\n"
            "BEGIN\n"
            "  IF UPDATING THEN\n"
            "    Oper1 := 'OC';\n"
            "  END IF;\n"
            "  IF DELETING THEN\n"
            "    Oper1 := 'OD';\n"
            "  END IF;\n", ControlName);
    fprintf(SqlAuditFile, "  IF UPDATING OR DELETING THEN\n"
        "    INSERT INTO %sAudit VALUES\n"
        "    (\n", ControlName);
    fprintf(SqlAuditFile, "      %sAuditSeq.NEXTVAL\n"
        "    , Oper1\n", ControlName);
    for (i = 0; i < Table->noFields; i++)
    {
        Field = &Table->Fields[i];
        fprintf(SqlAuditFile, "    , :OLD.%s\n", Field->Name);
    }
    fprintf(SqlAuditFile, "    , USER\n"
        "    , SYSDATE\n");
    fprintf(SqlAuditFile, "    , :OLD.%s\n", "RowId");
    fprintf(SqlAuditFile, "    );\n"
        "  END IF;\n");
    if (Table->hasAuditFull)
    {
        fprintf(SqlAuditFile, "  IF UPDATING OR INSERTING THEN\n"
            "    INSERT INTO %sAudit VALUES\n"
            "    (\n", ControlName);
        fprintf(SqlAuditFile, "      %sAuditSeq.NEXTVAL\n"
            "    , Oper2\n", ControlName);
        for (i = 0; i < Table->noFields; i++)
        {
            Field = &Table->Fields[i];
            fprintf(SqlAuditFile, "    , :NEW.%s\n", Field->Name);
        }
        fprintf(SqlAuditFile, "    , USER\n"
            "    , SYSDATE\n");
        fprintf(SqlAuditFile, "    , :NEW.%s\n", "RowId");
        fprintf(SqlAuditFile, "    );\n"
            "  END IF;\n");
    }
    fprintf(SqlAuditFile, "END;\n"
        "/\n"
        "show errors\n\n");
    if (strlen(Table->ControlDB))
        GenerateSyn5(SqlAuditFile, Table->Name, ControlName,
            strlen(Table->UConnect) ? CutUser(Table->UConnect) : Table->UserID);
    fprintf(SqlAuditFile, "SPOOL OFF\n\n");
    GenerateExit(Table, SqlAuditFile);
    fclose(SqlAuditFile);
}

static void GenerateTable(PYYTable Table)
{
    char comma;
    ushort i;
    PYYField Field;
    if (Table->noFields == 0) return;
    if (Table->OneSQLScript == 0)
    {
        fprintf(SqlFile, "\n@%s\n", Table->SqlTableFileName);
        SqlTableFile = OpenFile("SQL File(Tab)", Table->SqlTableFileName);
        if (SqlTableFile == 0)
            return;
        GenerateConnect(Table, SqlTableFile);
    }
    else
        SqlTableFile = SqlFile;
    fprintf(SqlTableFile, "REMARK TABLE\n\n");
    fprintf(SqlTableFile, "DROP TABLE %s", ControlName);
    fprintf(SqlTableFile, " CASCADE CONSTRAINTS");
    fprintf(SqlTableFile, ";\n\n");
    fprintf(SqlTableFile, "CREATE TABLE %s\n"
        "(", ControlName);
    int TableSize = 0;
    for (i = 0, comma = ' '; i < Table->noFields; i++, comma = ',')
    {
        Field = &Table->Fields[i];
        TableSize += ApproxFieldSize(Field);
        fprintf(SqlTableFile, "\n %c%s %s", comma, Field->Name,
            SqlFieldType(Field));
    }
    fprintf(SqlTableFile, "\n) /* approx row size %d */", TableSize);
    if (Table->Free)
        fprintf(SqlTableFile, "\n  PCTFREE %u", Table->Free);
    if (Table->Used)
        fprintf(SqlTableFile, "\n  PCTUSED %u", Table->Used);
    if (Table->IniTrans)
        fprintf(SqlTableFile, "\n  INITRANS %u", Table->IniTrans);
    if (Table->MaxTrans)
        fprintf(SqlTableFile, "\n  MAXTRANS %u", Table->MaxTrans);
    if (Table->Space)
        fprintf(SqlTableFile, " TABLESPACE %s", Table->Space);
    if (Table->Initial || Table->StorageNext || Table->MinExtents
        || Table->MaxExtents || Table->Optimal || Table->FreeLists
        || Table->FreeListGroups)
    {
        fprintf(SqlTableFile, "\n  STORAGE\n  (");
        if (Table->Initial)
            fprintf(SqlTableFile, "\n    INITIAL %u%c", Table->Initial, Table->InitialSize);
        if (Table->StorageNext)
            fprintf(SqlTableFile, "\n    NEXT %u%c", Table->StorageNext, Table->NextSize);
        if (Table->MinExtents)
            fprintf(SqlTableFile, "\n    MINEXTENTS %u", Table->MinExtents);
        if (Table->MaxExtents)
            fprintf(SqlTableFile, "\n    MAXEXTENTS %u", Table->MaxExtents);
        if (Table->Increase)
            fprintf(SqlTableFile, "\n    PCTINCREASE %u", Table->Increase);
        if (Table->Optimal)
            fprintf(SqlTableFile, "\n    INITIAL %u%c", Table->Optimal, Table->OptimalSize);
        if (Table->FreeLists)
            fprintf(SqlTableFile, "\n    FREELISTS %u", Table->FreeLists);
        if (Table->FreeListGroups)
            fprintf(SqlTableFile, "\n    FREELIST GROUPS %u", Table->FreeListGroups);
        fprintf(SqlTableFile, "\n  )");
    }
    fprintf(SqlTableFile, ";\n\n");
    GenerateSequence(Table);
    if (strlen(Table->ControlDB))
        GenerateSyn1(SqlTableFile, Table->Name, ControlName,
            strlen(Table->UConnect) ? CutUser(Table->UConnect) : Table->UserID);
    fprintf(SqlTableFile, "REMARK CONTRAINTS\n\n");
    GenerateNulls(Table);
    GenerateKeys(Table);
    if (Table->OneSQLScript == 0)
    {
        GenerateExit(Table, SqlTableFile);
        fclose(SqlTableFile);
    }
}

static void GenerateKeys(PYYTable Table)
{
    ushort i, PkNo = 1, UkNo = 1;

    for (i = 0; i < Table->noKeys; i++)
        GenerateKey(Table, &Table->Keys[i], &PkNo, &UkNo);
}

static void GenerateKey(PYYTable Table, PYYKey Key, ushort *PkNo, ushort *UkNo)
{
    ushort i;
    char comma;

    if (!Key->isPrimary && !Key->isUnique)
        return;
    if (Key->isPrimary)
        fprintf(SqlTableFile, "REMARK PRIMARY KEY\n\n");
    else if (Key->isUnique)
        fprintf(SqlTableFile, "REMARK UNIQUE KEY\n\n");
    fprintf(SqlTableFile, "ALTER TABLE %s\n"
        "ADD(", ControlName);
    if (Key->isPrimary)
        fprintf(SqlTableFile, "\n  CONSTRAINT %s_PK%02d PRIMARY KEY"
            "\n  (", Table->Name, (*PkNo)++);
    else if (Key->isUnique)
        fprintf(SqlTableFile, "\n  CONSTRAINT %s_UK%02d UNIQUE"
            "\n  (", Table->Name, (*UkNo)++);
    for (i = 0, comma = ' '; i < Key->noFields; i++, comma = ',')
        fprintf(SqlTableFile, "\n   %c%s", comma, Key->Fields[i].Name);
    fprintf(SqlTableFile, "\n  )");
    if (Key->Space || Key->Free || Key->IniTrans || Key->MaxTrans
        || Key->Initial || Key->StorageNext || Key->Increase || Key->MinExtents
        || Key->MaxExtents || Key->Optimal || Key->FreeLists
        || Key->FreeListGroups)
    {
        fprintf(SqlTableFile, " USING INDEX");
        if (Key->Free)
            fprintf(SqlTableFile, "\n  PCTFREE %u", Key->Free);
        if (Key->IniTrans)
            fprintf(SqlTableFile, "\n  INITRANS %u", Key->IniTrans);
        if (Key->MaxTrans)
            fprintf(SqlTableFile, "\n  MAXTRANS %u", Key->MaxTrans);
        if (Key->Space)
            fprintf(SqlTableFile, " TABLESPACE %s", Key->Space);
        if (Key->Initial || Key->StorageNext || Key->MinExtents
            || Key->MaxExtents || Key->Optimal || Key->FreeLists
            || Key->FreeListGroups)
        {
            fprintf(SqlTableFile, "\n  STORAGE\n  (");
            if (Key->Initial)
                fprintf(SqlTableFile, "\n    INITIAL %u%c", Key->Initial, Key->InitialSize);
            if (Key->StorageNext)
                fprintf(SqlTableFile, "\n    NEXT %u%c", Key->StorageNext, Key->NextSize);
            if (Key->MinExtents)
                fprintf(SqlTableFile, "\n    MINEXTENTS %u", Key->MinExtents);
            if (Key->MaxExtents)
                fprintf(SqlTableFile, "\n    MAXEXTENTS %u", Key->MaxExtents);
            if (Key->Increase)
                fprintf(SqlTableFile, "\n    PCTINCREASE %u", Key->Increase);
            if (Key->Optimal)
                fprintf(SqlTableFile, "\n    INITIAL %u%c", Key->Optimal, Key->OptimalSize);
            if (Key->FreeLists)
                fprintf(SqlTableFile, "\n    FREELISTS %u", Key->FreeLists);
            if (Key->FreeListGroups)
                fprintf(SqlTableFile, "\n    FREELIST GROUPS %u", Key->FreeListGroups);
            fprintf(SqlTableFile, "\n  )");
        }
    }
    fprintf(SqlTableFile, "\n);\n");
}

static void GenerateLinks(PYYTable Table)
{
    ushort i;

    for (i = 0; i < Table->noLinks; i++)
        GenerateLink(Table, &Table->Links[i], i);
}

static void GenerateLink(PYYTable Table, PYYLink Link, short count)
{
    ushort i;
    char comma;

    fprintf(SqlTableFile, "REMARK FOREIGN KEY\n\n");
    fprintf(SqlConFile, "ALTER TABLE %s\n"
        "ADD(", ControlName);
    fprintf(SqlConFile, "\n  CONSTRAINT %s_FK%02d FOREIGN KEY"
        "\n  (", Table->Name, ++count);
    for (i = 0, comma = ' '; i < Link->noFields; i++, comma = ',')
        fprintf(SqlConFile, "\n   %c%s", comma, Link->Fields[i].Name);
    fprintf(SqlConFile, "\n  ) REFERENCES %s", Link->MasterName);
    if (Link->DeleteCascade)
        fprintf(SqlConFile, " ON DELETE CASCADE");
    fprintf(SqlConFile, "\n);\n");
}

static void GenerateIndexes(PYYTable Table)
{
    ushort i;
    short IxNo = 1;

    if (Table->OneSQLScript == 0)
    {
        fprintf(SqlFile, "\n@%s\n", Table->SqlIdxFileName);
        SqlIdxFile = OpenFile("SQL File(Idx)", Table->SqlIdxFileName);
        if (SqlIdxFile == 0)
            return;
        GenerateConnect(Table, SqlFile);
    }
    else SqlIdxFile = SqlFile;
    for (i = 0; i < Table->noKeys; i++)
        GenerateIndex(Table, &Table->Keys[i], &IxNo);
    if (Table->OneSQLScript == 0)
    {
        GenerateExit(Table, SqlFile);
        fclose(SqlIdxFile);
    }
}

static void GenerateIndex(PYYTable Table, PYYKey Key, pshort IxNo)
{
    ushort i;
    char comma;

    if ((Key->isPrimary || Key->isUnique))
        return;
    fprintf(SqlTableFile, "REMARK INDEX\n\n");
    fprintf(SqlIdxFile, "CREATE %s INDEX %s_IX%02d ON %s\n(",
        (Key->isPrimary || Key->isUnique) ? "UNIQUE" : "",
        Table->Name, (*IxNo)++, Table->Name);
    for (i = 0, comma = ' '; i < Key->noFields; i++, comma = ',')
        fprintf(SqlIdxFile, "\n %c%s", comma, Key->Fields[i].Name);
    fprintf(SqlIdxFile, "\n)");
    if (Key->Free)
        fprintf(SqlIdxFile, "\n  PCTFREE %u", Key->Free);
    if (Key->IniTrans)
        fprintf(SqlIdxFile, "\n  INITRANS %u", Key->IniTrans);
    if (Key->MaxTrans)
        fprintf(SqlIdxFile, "\n  MAXTRANS %u", Key->MaxTrans);
    if (Key->Space)
        fprintf(SqlIdxFile, " TABLESPACE %s", Key->Space);
    if (Key->Initial || Key->StorageNext || Key->MinExtents
        || Key->MaxExtents || Key->Optimal || Key->FreeLists
        || Key->FreeListGroups)
    {
        fprintf(SqlIdxFile, "\n  STORAGE\n  (");
        if (Key->Initial)
            fprintf(SqlIdxFile, "\n    INITIAL %u%c", Key->Initial, Key->InitialSize);
        if (Key->StorageNext)
            fprintf(SqlIdxFile, "\n    NEXT %u%c", Key->StorageNext, Key->NextSize);
        if (Key->MinExtents)
            fprintf(SqlIdxFile, "\n    MINEXTENTS %u", Key->MinExtents);
        if (Key->MaxExtents)
            fprintf(SqlIdxFile, "\n    MAXEXTENTS %u", Key->MaxExtents);
        if (Key->Increase)
            fprintf(SqlIdxFile, "\n    PCTINCREASE %u", Key->Increase);
        if (Key->Optimal)
            fprintf(SqlIdxFile, "\n    INITIAL %u%c", Key->Optimal, Key->OptimalSize);
        if (Key->FreeLists)
            fprintf(SqlIdxFile, "\n    FREELISTS %u", Key->FreeLists);
        if (Key->FreeListGroups)
            fprintf(SqlIdxFile, "\n    FREELIST GROUPS %u", Key->FreeListGroups);
        fprintf(SqlIdxFile, "\n  )");
    }
    fprintf(SqlIdxFile, ";\n\n");
}

static void GenerateSequence(PYYTable Table)
{
    if (Table->hasSequence)
    {
        int i;
        const char *maxvalue = nines_pattern + sizeof(nines_pattern) - 9;
        for (i = 0; i < Table->noFields; i++)
        {
            PYYField field = &Table->Fields[i];
            if (field->isSequence) 
            {
                if (field->Type == ftypeNChar)
                    maxvalue = nines_pattern + sizeof(nines_pattern) - field->Length;
                break;
            }
        }
        fprintf(SqlTableFile, "REMARK SEQUENCE\n\n");
        if (strlen(Table->ControlDB))
        {
            fprintf(SqlTableFile, "DROP SEQUENCE %sSeq;\n\n", ControlName);
            fprintf(SqlTableFile, "CREATE SEQUENCE %sSeq\n"
                "  MINVALUE 1\n"
                "  MAXVALUE %s\n"
                "  CYCLE\n"
                "  ORDER;\n\n", ControlName, maxvalue);
            GenerateSyn4(SqlTableFile, Table->Name, ControlName,
                strlen(Table->UConnect) ? CutUser(Table->UConnect) : Table->UserID);
        }
        else
        {
            fprintf(SqlTableFile, "DROP SEQUENCE %sSeq;\n\n", Table->Name);
            fprintf(SqlTableFile, "CREATE SEQUENCE %sSeq\n"
                "  MINVALUE 1\n"
                "  MAXVALUE %s\n"
                "  CYCLE\n"
                "  ORDER;\n\n", Table->Name, maxvalue);
        }
    }
}

static void GenerateGrants(PYYTable Table)
{
    ushort i;

    if (Table->OneSQLScript == 0)
    {
        fprintf(SqlFile, "\n@%s\n", Table->SqlGrantFileName);
        SqlGrantFile = OpenFile("SQL File(Gra)", Table->SqlGrantFileName);
        if (SqlGrantFile == 0)
            return;
        GenerateConnect(Table, SqlGrantFile);
    }
    else
        SqlGrantFile = SqlFile;
    for (i = 0; i < Table->noGrants; i++)
        GenerateGrant(Table, &Table->Grants[i]);
    if (Table->OneSQLScript == 0)
    {
        GenerateExit(Table, SqlGrantFile);
        fclose(SqlGrantFile);
    }
}

static void GenerateGrant(PYYTable Table, PYYGrant Grant)
{
    ushort i, j;
    PYYLine Perm, User;

    for (i = 0; i < Grant->noUsers; i++)
    {
        User = &Grant->Users[i];
        if (Table->hasSequence && Grant->noPerms > 0)
        {
            Perm = &Grant->Perms[0];
            fprintf(SqlGrantFile, "GRANT %s ON %s.%s TO %s;\n\n",
                Perm->p,
                strlen(Table->ControlDB) ? Table->ControlDB : Table->UserID,
                Table->Name,
                User->p);
        }
        for (j = 0; j < Grant->noPerms; j++)
        {
            Perm = &Grant->Perms[j];
            fprintf(SqlGrantFile, "GRANT %s ON %s.%s TO %s;\n\n",
                Perm->p,
                strlen(Table->ControlDB) ? Table->ControlDB : Table->UserID,
                Table->Name,
                User->p);
        }
    }
}

static ushort sqldefTailMark = 0xBEEF;

static void GenerateTailField(PYYField Field)
{
    char work[1]; work[0] = 0;
    putString(Field->Name, BinFile);
    if (Field->Alias)
        putString(Field->Alias, BinFile);
    else
        putString(work, BinFile);
    putUInt16(Field->Type, BinFile);
    putUInt16(Field->Length, BinFile);
    putUInt16(Field->Offset, BinFile);
    putUInt16(Field->Padding, BinFile);
    putUInt16(Field->Filler, BinFile);
    putUInt16(Field->Precision, BinFile);
    putUInt16(Field->Scale, BinFile);
    putUInt16(Field->isPrimaryKey, BinFile);
    putUInt16(Field->isNull, BinFile);
    putUInt16(Field->isInput, BinFile);
    putUInt16(Field->isBound, BinFile);
    putUInt16(Field->isOutput, BinFile);
    putUInt16(Field->isWork, BinFile);
    putUInt16(Field->isRowSequence, BinFile);
    putUInt16(Field->isSequence, BinFile);
}

static void GenerateTailTable(PYYTable Table)
{
    ushort i;
    putUInt16(Table->noFields, BinFile);
    for (i = 0; i < Table->noFields; i++)
        GenerateTailField(&Table->Fields[i]);
}

static void GenerateTailKey(PYYKey Key)
{
    ushort i;
    putString(Key->Name, BinFile);
    putUInt16(Key->isPrimary, BinFile);
    putUInt16(Key->isUnique, BinFile);
    putUInt16(Key->noFields, BinFile);
    for (i = 0; i < Key->noFields; i++)
        GenerateTailField(&Key->Fields[i]);
}

static void GenerateTailKeys(PYYTable Table)
{
    ushort i;
    putUInt16(Table->hasPrimaryKey, BinFile);
    putUInt16(Table->PrimaryKey, BinFile);
    putUInt16(Table->noKeys, BinFile);
    for (i = 0; i < Table->noKeys; i++)
        GenerateTailKey(&Table->Keys[i]);
}

char *TableToken(PYYTable Table)
{
    static char work[32];
    int i = 0, n;
    ulong l = Table->Token;
    while (l > 0 && i < 20)
    {
        n = l % 26;
        l = l / 26;
        work[i++] = (char)('A' + n);
        work[i] = 0;
    }
    return work;
}

void GenerateBinTail(PYYTable Table)
{
    //  char buf[512];
    char *p = FNameName(Table->InputFileName);
    putString(p, BinFile);
    free(p);
    putString(TableToken(Table), BinFile);
    putUInt16(sqldefTailMark, BinFile);
    GenerateTailTable(Table);
    GenerateTailKeys(Table);
}

static void MakeBinaryName(PYYTable Table)
{
    char newpath[512];
    char dir[512];
    char ext[512];
    char fname[512];
    FNameSplit(Table->InputFileName, dir, fname, ext);
    if (Table->ShortName == 0)
        free(Table->ShortName);
    Table->ShortName = strdup(fname);
    if (strlen(Table->SODir))
        FNameMerge(newpath, Table->SODir, fname, Table->SOExt);
    else
        FNameMerge(newpath, dir, fname, Table->SOExt);
    Table->BinFileName = strdup(newpath);
}

static void GenerateBinary(PYYTable Table)
{
    MakeBinaryName(Table);
    BinFile = OpenFile("Bin File", Table->BinFileName, "wb");
    if (BinFile == 0)
        return;
    GenerateBinTable(Table);
    fclose(BinFile);
}

static int _HasProject(int noProjects, PYYLine Projects, PYYTable Table)
{
    if (Table->UseUsedBy == 0)
        return 1;
    if (noProjects == 0)
        return 1;
    char *Name = Table->Name;
    int i;
    for (i = 0; i < noProjects; i++)
    {
        PYYLine Project = &Projects[i];
        if (stricmp(Project->p, Name) == 0)
            return 1;
        if (stricmp(Project->p, "ALL") == 0)
            return 1;
    }
    return 0;
}

static ushort _CountProcs(PYYTable Table)
{
    if (Table->UseUsedBy == 0)
        return (ushort)(Table->noProcs - Table->noDatas);
    ushort result = 0;
    int i;
    for (i = 0; i < Table->noProcs; i++)
    {
        PYYProc Proc = &Table->Procs[i];
        result += _HasProject(Proc->noProjects, Proc->Projects, Table);
    }
    return result;
}

static void GenerateBinTable(PYYTable Table)
{
    ushort Count = _CountProcs(Table);
    if (Table->Internal == 0)
    {
        if (Table->hasInsert)
            Count += _HasProject(Table->noInsertProjects, Table->InsertProjects, Table);
        if (Table->hasSelectAll)
            Count += _HasProject(Table->noSelectAllProjects, Table->SelectAllProjects, Table);
        if (Table->hasSelectAllUpd)
            Count += _HasProject(Table->noSelectAllUpdProjects, Table->SelectAllUpdProjects, Table);
        if (Table->hasDeleteAll)
            Count += _HasProject(Table->noDeleteAllProjects, Table->DeleteAllProjects, Table);
        if (Table->hasCount)
            Count += _HasProject(Table->noCountProjects, Table->CountProjects, Table);
        if (Table->hasSelectOne)
            Count += _HasProject(Table->noSelectOneProjects, Table->SelectOneProjects, Table);
        if (Table->hasSelectOneUpd)
            Count += _HasProject(Table->noSelectOneUpdProjects, Table->SelectOneUpdProjects, Table);
        if (Table->hasDeleteOne)
            Count += _HasProject(Table->noDeleteOneProjects, Table->DeleteOneProjects, Table);
        if (Table->hasUpdate)
            Count += _HasProject(Table->noUpdateProjects, Table->UpdateProjects, Table);
    }
    putUInt16(sqldefSignOCI2, BinFile);
    putString(Table->Server, BinFile);
    putString(Table->UserID, BinFile);
    putString(Table->Name, BinFile);
    putUInt16(Count, BinFile);
    GenerateBinProcs(Table);
    GenerateBinTail(Table);
}

static void GenerateBinProcs(PYYTable Table)
{
    ushort i;
    if (Table->hasCount && Table->Internal == 0
        && _HasProject(Table->noCountProjects, Table->CountProjects, Table) == 1)
        GenerateBinProc(Table,
            "Count",
            0, 0, 0,
            "BEGIN\n"
            " Select count(*) into :NoOfRecs from %s;"
            "END;",
            &Table->Count, 0);
    if (Table->hasDeleteAll && Table->Internal == 0
        && _HasProject(Table->noDeleteAllProjects, Table->DeleteAllProjects, Table) == 1)
        GenerateBinProc(Table,
            "DeleteAll",
            0, 0, 0,
            "Delete %s",
            0, 0);
    if (Table->hasInsert && Table->noFields && Table->Internal == 0
        && _HasProject(Table->noInsertProjects, Table->InsertProjects, Table) == 1)
        GenerateBinProc(Table,
            "Insert",
            Table->noFields,
            Table->Fields,
            Table->ABSize,
            0, 0, 0);
    if (Table->hasSelectAll && Table->noFields && Table->Internal == 0
        && _HasProject(Table->noSelectAllProjects, Table->SelectAllProjects, Table) == 1)
        GenerateBinSelectAll(Table, false);
    if (Table->hasSelectAllUpd && Table->noFields && Table->Internal == 0
        && _HasProject(Table->noSelectAllUpdProjects, Table->SelectAllUpdProjects, Table) == 1)
        GenerateBinSelectAll(Table, true);
    if (Table->hasSelectOne && Table->noFields && Table->Internal == 0
        && _HasProject(Table->noSelectOneProjects, Table->SelectOneProjects, Table) == 1)
        GenerateBinProc(Table,
            "SelectOne",
            Table->noFields,
            Table->Fields,
            Table->ABSize,
            0, 0, 0);
    if (Table->hasSelectOneUpd && Table->noFields && Table->Internal == 0
        && _HasProject(Table->noSelectOneUpdProjects, Table->SelectOneUpdProjects, Table) == 1)
        GenerateBinProc(Table,
            "SelectOneUpd",
            Table->noFields,
            Table->Fields,
            Table->ABSize,
            0, 0, 0);
    if (Table->hasDeleteOne && Table->hasPrimaryKey && Table->Internal == 0
        && _HasProject(Table->noDeleteOneProjects, Table->DeleteOneProjects, Table) == 1)
        GenerateBinProc(Table,
            "DeleteOne",
            Table->Keys[Table->PrimaryKey].noFields,
            Table->Keys[Table->PrimaryKey].Fields,
            Table->Keys[Table->PrimaryKey].ABSize,
            0, 0, 0);
    if (Table->hasUpdate && Table->noFields && Table->Internal == 0
        && _HasProject(Table->noUpdateProjects, Table->UpdateProjects, Table) == 1)
        GenerateBinProc(Table,
            "Update",
            Table->noFields,
            Table->Fields,
            Table->ABSize,
            0, 0, 1);
    for (i = 0; i < Table->noProcs; i++)
    {
        PYYProc Proc = &Table->Procs[i];
        if (Proc->isData)
            continue;
        if (_HasProject(Proc->noProjects, Proc->Projects, Table) == 0)
            continue;
        if (Proc->isSql)
            GenerateBinSql(Table, Proc);
        else
            GenerateBinProc(Table,
                Proc->Name,
                Proc->noFields,
                Proc->Fields,
                Proc->ABSize,
                0, 0, 0);
    }
}

static void GenerateBinProc(PYYTable Table, const char *ProcName,
    ushort noFields, PYYField Fields,
    ushort ABSize, const char *Code,
    PYYField ExtraField,
    uchar testSeq)
{
    char *TableName = Table->Name;
    char *AB;
    ushort i, first, no;
    ABSize += (ushort)512;
    AB = (char *)malloc(ABSize);
    strcpy(AB, TableName);
    if (Table->UnderScore)
        strcat(AB, "_");
    strcat(AB, ProcName);
    putString(AB, BinFile);
    if (testSeq)
        for (i = 0, no = 0; i < noFields; i++)
        {
            if (Fields[i].isSequence && Fields[i].isPrimaryKey == 0)
                continue;
            if (Fields[i].isRowSequence)
                continue;
            no++;
        }
    else
        no = noFields;
    if (ExtraField)
        putUInt16((ushort)(no + 1), BinFile);
    else
        putUInt16(no, BinFile);
    for (i = 0; i < noFields; i++)
    {
        if (testSeq && Fields[i].isSequence && Fields[i].isPrimaryKey == 0)
            continue;
        if (testSeq && Fields[i].isRowSequence)
            continue;
        sprintf(AB, ":%s", Fields[i].Name);
        putString(AB, BinFile);
        putInt16(SQLCFieldType(&Fields[i]), BinFile);
        putUInt16(OCIFieldType(&Fields[i]), BinFile);
        if (Fields[i].Type == ftypeImagePtr)
            putUInt32(0, BinFile);
        else
            putUInt32(Fields[i].Length, BinFile);
        putUInt32(Fields[i].Offset, BinFile);
        putUInt32(Fields[i].Precision, BinFile);
        putUInt32(Fields[i].Scale, BinFile);
        putUInt16(1, BinFile);
        putUInt16(0, BinFile);
    }
    if (ExtraField)
    {
        sprintf(AB, ":%s", ExtraField->Name);
        putString(AB, BinFile);
        putInt16(SQLCFieldType(ExtraField), BinFile);
        putUInt16(OCIFieldType(ExtraField), BinFile);
        putUInt32(ExtraField->Length, BinFile);
        putUInt32(ExtraField->Offset, BinFile);
        putUInt32(ExtraField->Precision, BinFile);
        putUInt32(ExtraField->Scale, BinFile);
        putUInt16(1, BinFile);
        putUInt16(0, BinFile);
    }
    if (noFields)
    {
        sprintf(AB, "BEGIN\n  %s%s%s",
            TableName,
            Table->UnderScore ? "_" : "",
            ProcName);
        if (Code)
        {
            strcat(AB, ".");
            strcat(AB, Code);
        }
        strcat(AB, "\n  (\n");
        if (ExtraField)
        {
            strcat(AB, "    :");
            strcat(AB, ExtraField->Name);
            strcat(AB, "\n");
        }
        for (i = first = 0; i < noFields; i++)
        {
            if (testSeq && Fields[i].isSequence && Fields[i].isPrimaryKey == 0)
                continue;
            if (testSeq && Fields[i].isRowSequence)
                continue;
            strcat(AB, (first == 0 && !ExtraField) ? "    :" : "  , :");
            strcat(AB, Fields[i].Name);
            strcat(AB, "\n");
            first = 1;
        }
        strcat(AB, "  );\n");
        strcat(AB, "END;");
    }
    else if (Code)
    {
        sprintf(AB, Code, TableName);
    }
    else
    {
        sprintf(AB, "BEGIN %s%s%s; END;",
            TableName,
            Table->UnderScore ? "_" : "",
            ProcName);
    }
    putUInt16((uint16)strlen(AB), BinFile);
    putExact(AB, BinFile);
    free(AB);
}

static ushort FieldFlag(TYYField Field)
{
    return (Field.isPrimaryKey ? 0x8000 : 0)
        | (Field.isNull ? 0x4000 : 0)
        | (Field.isInput ? 0x2000 : 0)
        | (Field.isBound ? 0x1000 : 0)
        | (Field.isOutput ? 0x0800 : 0)
        | (Field.isWork ? 0x0400 : 0)
        | (Field.isRowSequence ? 0x0200 : 0)
        | (Field.isSequence ? 0x0100 : 0)
        | (NullableField(&Field) ? 0x0010 : 0);
}

static void GenerateBinSelectAll(PYYTable Table, bool ForUpd)
{
    char *AB, *pAB;
    ushort i;
    PYYField Fields = Table->Fields;

    AB = (char *)calloc(strlen(Table->Name) + 20, 1);
    strcpy(AB, Table->Name);
    if (Table->UnderScore)
        strcat(AB, "_");
    strcat(AB, "SelectAll");
    if (ForUpd)
        strcat(AB, "Upd");
    putString(AB, BinFile);
    free(AB);
    putUInt16(((Table->noFields & 0x0FFF) | 0xA000), BinFile);
    AB = (char *)malloc(4096);
    for (i = 0; i < Table->noFields; i++)
    {
        sprintf(AB, ":%s", Fields[i].Name);
        putString(AB, BinFile);
        putInt16(SQLCFieldType(&Fields[i]), BinFile);
        putUInt16(OCIFieldType(&Fields[i]), BinFile);
        if (Fields[i].Type == ftypeImagePtr)
            putUInt32(0, BinFile);
        else
            putUInt32(Fields[i].Length, BinFile);
        putUInt32(Fields[i].Offset, BinFile);
        putUInt32(Fields[i].Precision, BinFile);
        putUInt32(Fields[i].Scale, BinFile);
        putUInt16(0, BinFile);
        putUInt16(1 | FieldFlag(Fields[i]), BinFile);
    }
    pAB = AB;
    pAB += sprintf(pAB, "select");
    for (i = 0; i < Table->noFields; i++)
        pAB += sprintf(pAB,
            Table->Fields[i].Type == ftypeDate
            ? "%s to_char(%s, 'YYYYMMDD')"
            : (Table->Fields[i].Type == ftypeTimeStamp
                || Table->Fields[i].Type == ftypeDateTime)
            ? "%s to_char(%s, 'YYYYMMDDHH24MISS')"
            : Table->Fields[i].Type == ftypeMoney
            ? "%s to_char(%s)"
            : "%s %s",
            i == 0
            ? ""
            : ",",
            Fields[i].Name);
    pAB += sprintf(pAB, " from %s", Table->Name);
    if (ForUpd)
        pAB += sprintf(pAB, " for update");
    putUInt16((uint16)strlen(AB), BinFile);
    putExact(AB, BinFile);
    free(AB);
}

static int _withSize(PYYTable Table, PYYProc Proc)
{
    int size = 0, i, j, k;
    if (Proc->noWiths == 0)
        return size;
    for (i = 0; i < Proc->noWiths; i++)
    {
        size += i ? 10 : 14;
        size += strlen(Proc->Withs[i].p) + 2;
        for (j = 0; j < Table->noWiths; j++)
        {
            if (stricmp(Table->Withs[j].Name, Proc->Withs[i].p) == 0)
            {
                for (k = 0; k < Table->Withs[j].noLines; k++)
                {
                    size += strlen(Table->Withs[j].Lines[k].p) + 2;
                }
            }
        }
    }
    return size;
}

static void _prepend(char* AB, PYYTable Table, PYYProc Proc)
{
    int i, j, k;
    bool first = true;
    for (i = 0; i < Proc->noWiths; i++)
    {
        for (j = 0; j < Table->noWiths; j++)
        {
            if (stricmp(Table->Withs[j].Name, Proc->Withs[i].p) == 0)
            {
                if (first)
                {
                    strcat(AB, "with ");
                    first = false;
                }
                else
                    strcat(AB, ", ");
                strcat(AB, Table->Withs[j].Name);
                strcat(AB, " as (");
                for (k = 0; k < Table->Withs[j].noLines; k++)
                {
                    strcat(AB, " ");
                    strcat(AB, strtrim(Table->Withs[j].Lines[k].p));
                }
                strcat(AB, ")");
            }
        }
    }

}

static void GenerateBinSql(PYYTable Table, PYYProc Proc)
{
    char *AB;
    ushort i, j, ABSize;
    PYYField Fields = Proc->Fields;

    AB = (char *)calloc(strlen(Table->Name) + strlen(Proc->Name) + 1, 1);
    strcpy(AB, Table->Name);
    if (Table->UnderScore)
        strcat(AB, "_");
    strcat(AB, Proc->Name);
    putString(AB, BinFile);
    free(AB);
    putUInt16(((Proc->noBinds & 0x0FFF)
        | (Proc->isSql ? 0x8000 : 0)
        | (Proc->isSingle ? 0x4000 : 0)
        | (Proc->isManyQuery ? 0x2000 : 0)
        | (Table->isNullEnabled ? 0x1000 : 0)), BinFile);
    AB = (char *)malloc(512);
    for (j = 0; j < Proc->noBinds; j++)
    {
        i = Proc->Binds[j];
        sprintf(AB, ":%s", Fields[i].Name);
        putString(AB, BinFile);
        putInt16(SQLCFieldType(&Fields[i]), BinFile);
        putUInt16(OCIFieldType(&Fields[i]), BinFile);
        if (Fields[i].Type == ftypeImagePtr)
            putUInt32(0, BinFile);
        else
            putUInt32(Fields[i].Length, BinFile);
        putUInt32(Fields[i].Offset, BinFile);
        putUInt32(Fields[i].Precision, BinFile);
        putUInt32(Fields[i].Scale, BinFile);
        if (Fields[i].Type == ftypeDynamic)
        {
            putUInt16(0, BinFile);
            putUInt16(0, BinFile);
        }
        else
        {
            putUInt16(j < Proc->noOutputs ? 0 : 1 | FieldFlag(Fields[i]), BinFile);
            putUInt16(j < Proc->noOutputs ? 1 | FieldFlag(Fields[i]) : 0, BinFile);
        }
    }
    free(AB);
    ABSize = 0;
    ABSize += _withSize(Table, Proc);
    for (i = 0; i < Proc->noLines; i++)
        ABSize += (ushort)strlen(Proc->Lines[i].p) + 2;
    AB = (char *)calloc(ABSize, 1);
    if (Proc->noWiths > 0)
        _prepend(AB, Table, Proc);
    for (i = 0; i < Proc->noLines; i++)
    {
        if (i) strcat(AB, " ");
        strcat(AB, strtrim(Proc->Lines[i].p));
    }
    for (i = (ushort)strlen(AB) - 1; AB[i] <= ' ' || AB[i] == ';'; i--)
    {
        AB[i] = 0;
    }
    if ((i > 4)
        && (i < ABSize)
        && (strnicmp(AB + (i - 2), "end", 3) == 0)
        && (strchr("abcdefghijklmnopqrstuvwxyz"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "0123456789_#$", AB[i - 3]) == 0))
        AB[++i] = ';';
    putUInt16((ushort)strlen(AB), BinFile);
    putExact(AB, BinFile);
    free(AB);
}

static const char *SqlFieldType(PYYField Field)
{
    static char work[512];
    switch (Field->Type)
    {
    case ftypeNChar:
        sprintf(work, "number(%u)", Field->Precision);
        return work;
    case ftypeChar:
    case ftypeUserStamp:
    case ftypeStatus:
        sprintf(work, "%s(%u)",
            (Table->BlankPadded || Table->CharZ)
            ? "char"
            : "varchar2",
            Field->Length - 1);
        return work;
    case ftypeBinary:
        sprintf(work, "raw(%u)", Field->Length - 1);
        return work;
    case ftypeDate:
        return "date";
    case ftypeDateTime:
        return "date";
    case ftypeTimeStamp:
        if (Field->Scale > 0)
        {
            sprintf(work, "timestamp(%u)", Field->Scale);
            return work;
        }
        return "date";
    case ftypeBoolean:
        return "number(1)";
    case ftypeTinyint:
        return "number(2)";
    case ftypeSmallint:
        return "number(4)";
    case ftypeInt:
        return "number(10)";
    case ftypeLong:
        return "number(18)";
    case ftypeFloat:
    case ftypeVarNum:
    case ftypeMoney:
        if (Field->Precision)
        {
            sprintf(work, "number(%u,%u)", Field->Precision, Field->Scale);
            return work;
        }
        return "number";
    case ftypeBLOB:
    case ftypeZLOB:
    case ftypeCLOB:
    case ftypeImage:
    case ftypeImagePtr:
        return "long raw";
    case ftypeHugeCHAR:
        return "clob";
    case ftypeXMLTYPE:
        return "xmltype";
    case ftypeException:
        return "exception";
    }
    return "junk9";
}

static const char *ParmFieldType(PYYField Field)
{
    switch (Field->Type)
    {
    case ftypeBinary:
        return "raw";
    case ftypeChar:
    case ftypeDateTime:
    case ftypeDate:
    case ftypeTimeStamp:
    case ftypeUserStamp:
    case ftypeStatus:
    case ftypeMoney:
        return Table->CharZ ? "char" : "varchar2";
    case ftypeNChar:
    case ftypeBoolean:
    case ftypeTinyint:
    case ftypeSmallint:
    case ftypeInt:
    case ftypeLong:
    case ftypeFloat:
    case ftypeVarNum:
        return "number";
    case ftypeBLOB:
    case ftypeZLOB:
    case ftypeCLOB:
    case ftypeImage:
    case ftypeImagePtr:
        return "long raw";
    case ftypeHugeCHAR:
        return "clob";
    case ftypeXMLTYPE:
        return "xmltype";
    }
    return "junk10";
}

static const char *ParmFieldTypeInsert(PYYField Field)
{
    switch (Field->Type)
    {
    case ftypeBinary:
        return "raw";
    case ftypeChar:
    case ftypeDate:
    case ftypeDateTime:
    case ftypeMoney:
        return Table->CharZ ? "char" : "varchar2";
    case ftypeTimeStamp:
    case ftypeUserStamp:
    case ftypeStatus:
        return Table->CharZ ? "char" : "varchar2";
    case ftypeNChar:
    case ftypeInt:
    case ftypeLong:
    case ftypeBoolean:
    case ftypeTinyint:
    case ftypeSmallint:
    case ftypeFloat:
    case ftypeVarNum:
        return "number";
    case ftypeBLOB:
    case ftypeZLOB:
    case ftypeCLOB:
    case ftypeImage:
    case ftypeImagePtr:
        return "long raw";
    case ftypeHugeCHAR:
        return "clob";
    case ftypeXMLTYPE:
        return "xmltype";
    }
    return "junk11";
}

static const char *ParmFieldTypeUpdate(PYYField Field)
{
    switch (Field->Type)
    {
    case ftypeBinary:
        return "raw";
    case ftypeChar:
    case ftypeDate:
    case ftypeDateTime:
    case ftypeMoney:
        return Table->CharZ ? "char" : "varchar2";
    case ftypeTimeStamp:
    case ftypeUserStamp:
    case ftypeStatus:
        return Table->CharZ ? "char" : "varchar2";
    case ftypeNChar:
    case ftypeInt:
    case ftypeLong:
    case ftypeBoolean:
    case ftypeTinyint:
    case ftypeSmallint:
    case ftypeFloat:
    case ftypeVarNum:
        return "number";
    case ftypeBLOB:
    case ftypeZLOB:
    case ftypeCLOB:
    case ftypeImage:
    case ftypeImagePtr:
        return "long raw";
    case ftypeHugeCHAR:
        return "clob";
    case ftypeXMLTYPE:
        return "xmltype";
    }
    return "junk12";
}

static ushort OCIFieldType(PYYField Field)
{
    switch (Field->Type)
    {
    case ftypeTimeStamp:
    case ftypeUserStamp:
    case ftypeStatus:
    case ftypeDate:
    case ftypeDateTime:
    case ftypeMoney:
    case ftypeChar:    return Table->BlankPadded
        ? SQL_OCI_CHARx
        : Table->CharZ
            ? SQL_OCI_CHARz
            : SQL_OCI_VARCHAR2;
    case ftypeNChar:   return SQL_OCI_NUMBER;
    case ftypeFloat:   return SQL_OCI_FLOAT;
    case ftypeVarNum:  return SQL_OCI_VARNUM;
    case ftypeSmallint:
    case ftypeBoolean:
    case ftypeTinyint: return SQL_OCI_NUMBER;
    case ftypeLong:    return SQL_OCI_NUMBER;
    case ftypeInt:     return SQL_OCI_NUMBER;
    case ftypeImage:   return SQL_OCI_IMAGE;
    case ftypeImagePtr:return SQL_OCI_IMAGE;
    case ftypeBLOB:    return SQL_OCI_BLIMAGE;
    case ftypeCLOB:    return SQL_OCI_CLIMAGE;
    case ftypeZLOB:    return SQL_OCI_BLIMAGE;
    case ftypeHugeCHAR:return SQL_OCI_HUGECHAR;
    case ftypeXMLTYPE: return SQL_OCI_XMLTYPE;
    case ftypeBinary:  return SQL_OCI_RAW;
    case ftypeDynamic: return '$';
    }
    return 0;
}

static short SQLCFieldType(PYYField Field)
{
    switch (Field->Type)
    {
    case ftypeChar:      return SQL_C_CHAR;
    case ftypeNChar:     return SQL_C_CHAR;
    case ftypeFloat:     return SQL_C_DOUBLE;
    case ftypeInt:       return SQL_C_LONG;
    case ftypeLong:      return SQL_C_LONG64;
    case ftypeSmallint:  return SQL_C_SHORT;
    case ftypeBoolean:   return SQL_C_TINYINT;
    case ftypeTinyint:   return SQL_C_TINYINT;
    case ftypeImage:     return SQL_C_BINARY;
    case ftypeImagePtr:  return SQL_C_BINARY;
    case ftypeBLOB:      return SQL_C_BLIMAGE;
    case ftypeCLOB:      return SQL_C_CLIMAGE;
    case ftypeZLOB:      return SQL_C_ZLIMAGE;
    case ftypeHugeCHAR:  return SQL_C_HUGECHAR;
    case ftypeXMLTYPE:   return SQL_C_XMLTYPE;
    case ftypeDate:      return SQL_C_CHAR;
    case ftypeDateTime:  return SQL_C_CHAR;
    case ftypeTimeStamp: return SQL_C_CHAR;
    case ftypeUserStamp: return SQL_C_CHAR;
    case ftypeStatus:    return SQL_C_CHAR;
    case ftypeMoney:     return SQL_C_CHAR;
    case ftypeBinary:    return SQL_C_BINARY;
    case ftypeDynamic:   return '$';
    }
    return 0;
}

static int ApproxFieldSize(PYYField Field)
{
    switch (Field->Type)
    {
    case ftypeBinary:
    case ftypeChar:
    case ftypeNChar:
    case ftypeUserStamp:
    case ftypeStatus:
        return Field->Length + 1;
    case ftypeTimeStamp:
    case ftypeDate:
    case ftypeDateTime:
        return 7;
    case ftypeTinyint:
        return 2;
    case ftypeBoolean:
        return 1;
    case ftypeSmallint:
        return 4;
    case ftypeInt:
        return 10;
    case ftypeLong:
        return 19;
    case ftypeFloat:
    case ftypeMoney:
    case ftypeVarNum:
        if (Field->Precision)
            return Field->Precision + 2;
        return 38;
    case ftypeImage:
        return Field->Length - 2;
    case ftypeImagePtr:
        return 32000;
    case ftypeBLOB:
    case ftypeCLOB:
    case ftypeXMLTYPE:
        return Field->Precision;
    case ftypeHugeCHAR:
        return 64000;
    case ftypeZLOB:
        return Field->Precision / 10;
    }
    return 38;
}


