#include "machine.h"

#include <stdio.h>
#include "xstring.h"
#include <stdlib.h>
#include <ctype.h>

#include "sqldef.h"

#include "global.h"
#include "xdir.h"
#include "tbuffer.h"

static FILE *PyDBApiFile = 0;

static char g_ext[512];
static char g_fname[512];

static const char* useName(TYYTable &table)
{
    if (table.Alias)
        return table.Alias;
    return table.Name;
}

static const char* useName(TYYField &field)
{
    if (field.Alias)
        return field.Alias;
    return field.Name;
}

static void makePyDBApiName(PYYTable Table)
{
    char newpath[512];
    char dir[512];
    FNameSplit(Table->InputFileName, dir, g_fname, g_ext);
    char pname[512];
    memset(pname, 0, sizeof(pname));
    if (Table->noFields == 0)
    {
        strncpy(pname, g_fname, sizeof(pname));
        const char* uname = useName(*Table);
        for (int i = 0; i < strlen(uname); i++)
        {
            if (pname[i] != uname[i])
            {
                if (uname[i] == toupper(pname[i]))
                {
                    pname[i] = uname[i];
                    continue;
                }
                break;
            }
        }
    }
    else
        strncpy(pname, useName(*Table), sizeof(pname));
    int pos = strlen(pname);
    int left = sizeof(pname) - pos -1;
    strncpy(pname+pos, "DBApi", left);
    if (strlen(Table->PyDBApiDir))
        FNameMerge(newpath, Table->PyDBApiDir, pname, strlwr(Table->PyDBApiExt));
    else
        FNameMerge(newpath, dir, pname, strlwr(Table->PyDBApiExt));
    Table->PyDBApiFileName = strdup(newpath);
}

static bool isString(TYYField &field)
{
    if (field.Type == ftypeChar
        || field.Type == ftypeNChar
        || field.Type == ftypeBinary
        || field.Type == ftypeTimeStamp
        || field.Type == ftypeUserStamp
        || field.Type == ftypeStatus
        || field.Type == ftypeMoney
        || field.Type == ftypeDynamic
        || field.Type == ftypeDate
        || field.Type == ftypeDateTime)
        return true;
    return false;
}

static const char* constName(char* name)
{
    return strupr(name);
}

static void generateConsts(TYYTable &table)
{
    int i, f;
    bool hasImport = false;
    for (i = 0; i < table.noFields; i++)
    {
        TYYField &field = table.Fields[i];
        if (field.noConsts > 0)
        {
            if (hasImport == false)
            {
                hasImport = true;
                fprintf(PyDBApiFile, "from enum import Enum\n\n");
            }
            fprintf(PyDBApiFile, "class %s%s(Enum):\n", useName(table), useName(field));
            for (f = 0; f < field.noConsts; f++)
            {
                TYYConst &constant = field.Consts[f];
                char work[256];
                strncpy(work, constant.Name, sizeof(work));
                fprintf(PyDBApiFile, "    %s = %d\n", constName(work), constant.Value);
            }
            fprintf(PyDBApiFile, "    def __str__(self):\n");
            if (isString(field) == true)
                fprintf(PyDBApiFile, "        return self.name\n\n");
            else
                fprintf(PyDBApiFile, "        return str(self.value)\n\n");
        }
    }
}

static FILE* openFile(const char *FileFlag, const char *FileName, const char *Mode = "wt")
{
    char Work[512];
    sprintf(Work, "%-13.13s: %s\n", FileFlag, FileName);
    yyerror(Work);
    FILE* File = fopen(FileName, Mode);
    if (File == 0)
        yyerror("Cannot open %s : %s", FileFlag, FileName);
    else
        setvbuf(File, 0, _IOFBF, 32768);
    return File;
}

static void generateSlots(int noFields, PYYField fields)
{
    fprintf(PyDBApiFile, "    __slots__ = [");
    for (int i = 0; i < noFields; i++)
    {
        char tail = i+1 < noFields ? ',' : ']';
        TYYField &field = fields[i];
        fprintf(PyDBApiFile, "%s'%s'%c\n", i > 0 ? "        " : "", useName(field), tail);
    }
}

static void generateInit(int noFields, PYYField fields)
{
    fprintf(PyDBApiFile, "    def __init__(self):\n");
    for (int i = 0; i < noFields; i++)
    {
        TYYField &field = fields[i];
        fprintf(PyDBApiFile, "        self.%s = ''\n", useName(field));
    }
}

static void generateStdD(TYYTable &table)
{
    fprintf(PyDBApiFile, "class D%s(object):\n", useName(table));
    fprintf(PyDBApiFile, "    def _make(self):\n");
    fprintf(PyDBApiFile, "        return D%s()\n", useName(table));
    if (table.noFields > 0)
    {
        generateSlots(table.noFields, table.Fields);
        generateInit(table.noFields, table.Fields);
        fprintf(PyDBApiFile, "    def _fields(self):\n");
        fprintf(PyDBApiFile, "        return D%s.__slots__\n", useName(table));
    }
    fprintf(PyDBApiFile, "\n");
}

static void generateProcD(TYYTable &table, TYYProc &proc)
{
    char name[256];
    snprintf(name, sizeof(name), "%s%s", useName(table), proc.Name);
    fprintf(PyDBApiFile, "class D%s(object):\n", name);
    fprintf(PyDBApiFile, "    def _make(self):\n");
    fprintf(PyDBApiFile, "        return D%s()\n", name);
    if (proc.noFields > 0)
    {
        generateSlots(proc.noFields, proc.Fields);
        generateInit(proc.noFields, proc.Fields);
        fprintf(PyDBApiFile, "    def _fields(self):\n");
        fprintf(PyDBApiFile, "         return D%s.__slots__\n", name);
    }
    fprintf(PyDBApiFile, "\n");
}

static int numBinds(TYYProc &proc)
{
    int result = 0;
    for (int i = 0; i < proc.noFields; i++)
    {
        TYYField &field = proc.Fields[i];
        if (field.isBound != 0)
            result++;
    }
    return result;
}

static int numInputs(TYYProc &proc)
{
    int result = 0;
    for (int i = 0; i < proc.noFields; i++)
    {
        TYYField &field = proc.Fields[i];
        if (field.isInput != 0)
            result++;
    }
    return result;
}

static int numDynamics(TYYProc &proc)
{
    int result = 0;
    for (int i = 0; i < proc.noFields; i++)
    {
        TYYField &field = proc.Fields[i];
        if (field.Type == ftypeDynamic)
            result++;
    }
    return result;
}

static char* useLine(TYYLine &line, int i, int &p)
{
    int n = strspn(line.p, " \t");
    if (i == 0)
        p = n;
    return n >= p ? line.p + p : line.p;
}

static char* checkLine(TYYLine &line, int i, int &p, char* work, int worksize)
{
    char* r = useLine(line, i, p);
    work[0] = 0;
    while (true)
    {
        char* c = strchr(r, '$');
        if (c == 0)
        {
            strcat(work, r);
            return work;
        }
        strncat(work, r, c - r);
        c++;
        strcat(work, "{self.");
        int n = strspn(c, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_");
        strncat(work, c, n);
        strcat(work, "}");
        r = c + n;
    }
}

static void generateProcYaml(TYYProc &proc)
{
    fprintf(PyDBApiFile, "    _proc = '''\n");
    fprintf(PyDBApiFile, "Length: %d\n", proc.Length);
    fprintf(PyDBApiFile, "ABSize: %d\n", proc.ABSize);
    fprintf(PyDBApiFile, "noFields: %d\n", proc.noFields);
    fprintf(PyDBApiFile, "noDynamics: %d\n", numDynamics(proc));
    fprintf(PyDBApiFile, "noOutputs: %d\n", proc.noOutputs);
    if (proc.noOutputs > 0)
    {
        fprintf(PyDBApiFile, "Defines: [");
        for (int i = 0, no = 0; i < proc.noFields; i++)
        {
            TYYField &field = proc.Fields[i];
            if (field.isOutput)
            {
                fprintf(PyDBApiFile, "%s%d", no == 0 ? "" : ", ", i);
                no++;
            }
        }
        fprintf(PyDBApiFile, "]\n");
    }
    int noBinds = numBinds(proc);
    fprintf(PyDBApiFile, "noBound: %d\n", noBinds);
    if (noBinds > 0)
    {
        fprintf(PyDBApiFile, "Binds: [");
        for (int i = 0; i < proc.noFields; i++)
        {
            TYYField &field = proc.Fields[i];
            if (field.isBound != 0)
            {
                fprintf(PyDBApiFile, "%s%d", i == 0 ? "" : ", ", proc.Binds[i]);
            }
        }
        fprintf(PyDBApiFile, "]\n");
    }
    fprintf(PyDBApiFile, "noWorkFields: %d\n", proc.noWorkFields);
    fprintf(PyDBApiFile, "noCursors: %d\n", proc.noCursors);
    fprintf(PyDBApiFile, "noWiths: %d\n", proc.noWiths);
    fprintf(PyDBApiFile, "noLines: %d\n", proc.noLines);
    if (proc.noLines > 0)
    {
        fprintf(PyDBApiFile, "Lines: |\n");
        for (int i = 0, p = 0; i < proc.noLines; i++)
        {
            TYYLine &line = proc.Lines[i];
            fprintf(PyDBApiFile, "  %s", useLine(line, i, p));
        }
    }
    fprintf(PyDBApiFile, "noUsers: %d\n", proc.noUsers);
    fprintf(PyDBApiFile, "noProjects: %d\n", proc.noProjects);
    fprintf(PyDBApiFile, "isSql: %d\n", proc.isSql);
    fprintf(PyDBApiFile, "isSingle: %d\n", proc.isSingle);
    fprintf(PyDBApiFile, "isData: %d\n", proc.isData);
    fprintf(PyDBApiFile, "useStd: %d\n", proc.useStd);
    fprintf(PyDBApiFile, "useKey: %d\n", proc.useKey);
    fprintf(PyDBApiFile, "isStd: %d\n", proc.isStd);
    fprintf(PyDBApiFile, "isManyQuery: %d\n", proc.isManyQuery);
    fprintf(PyDBApiFile, "isSProc: %d\n", proc.isSProc);
    fprintf(PyDBApiFile, "hasImage: %d\n", proc.hasImage);
    fprintf(PyDBApiFile, "hasLOB: %d\n", proc.hasLOB);
    fprintf(PyDBApiFile, "hasHugeCHAR: %d\n", proc.hasHugeCHAR);
    fprintf(PyDBApiFile, "hasXMLTYPE: %d\n", proc.hasXMLTYPE);
    fprintf(PyDBApiFile, "extProc: %d\n", proc.extProc);
    fprintf(PyDBApiFile, "'''\n");
}

static void generateSqlCommand(TYYProc &proc)
{
    int noDynamics = numDynamics(proc);
    fprintf(PyDBApiFile, "    def execute(self, connect):\n");
    fprintf(PyDBApiFile, "        _command = %s'''\\\n", noDynamics ? "f" : "");
    char work[512];
    for (int i = 0, p = 0; i < proc.noLines; i++)
    {
        TYYLine &line = proc.Lines[i];
        fprintf(PyDBApiFile, "%s", checkLine(line, i, p, work, sizeof(work)));
    }
    fprintf(PyDBApiFile, "'''\n");
}

static void generateExecuteWithBinds(TYYProc &proc)
{
    int noBinds = numBinds(proc);
    if (noBinds != 0)
    {
        fprintf(PyDBApiFile, "        cursor.execute(_command,\n");
        for (int i = 0, n = 0; i < proc.noFields; i++)
        {
            TYYField &field = proc.Fields[i];
            if (field.isBound)
            {
                n++;
                fprintf(PyDBApiFile, "            %s=self.%s%s\n"
                    , field.Name
                    , useName(field)
                    , n < noBinds ? "," : ")");
            }
        }
    }
    else
        fprintf(PyDBApiFile, "    def execute(self, connect):\n");
}

static void generateCommand(TYYProc &proc)
{
    generateSqlCommand(proc);
    fprintf(PyDBApiFile, "        cursor = connect.cursor()\n");
    fprintf(PyDBApiFile, "        cursor.execute(_command)\n");
}

static void generateAction(TYYProc &proc)
{
    generateSqlCommand(proc);
    fprintf(PyDBApiFile, "        cursor = connect.cursor()\n");
    generateExecuteWithBinds(proc);
}

static void generateCopyInput(TYYProc &proc)
{
    fprintf(PyDBApiFile, "    def _copy_input(self, record):\n");
    for (int i = 0; i < proc.noFields; i++)
    {
        TYYField &field = proc.Fields[i];
        if (field.isInput == 1|| field.Type == ftypeDynamic)
            fprintf(PyDBApiFile, "        record.%s = self.%s\n", useName(field), useName(field));
    }
}

static void generateGetOutput(TYYProc &proc)
{
    fprintf(PyDBApiFile, "    def _get_output(self, record, _result):\n");
    for (int i = 0, n = 0; i < proc.noFields; i++)
    {
        TYYField &field = proc.Fields[i];
        if (field.isOutput)
        {
            fprintf(PyDBApiFile, "        record.%s = _result[%d]\n",useName(field), n);
            n++;
        }
    }
}

static void generateSingle(TYYProc &proc)
{
    int no = numInputs(proc);
    if (no > 0)
        generateCopyInput(proc);
    generateGetOutput(proc);
    generateSqlCommand(proc);
    fprintf(PyDBApiFile, "        cursor = connect.cursor()\n");
    if (no > 0)
        generateExecuteWithBinds(proc);
    else
        fprintf(PyDBApiFile, "        cursor.execute(_command)\n");
    fprintf(PyDBApiFile, "        result = cursor.fetchone()\n");
    fprintf(PyDBApiFile, "        if result == None:\n");
    fprintf(PyDBApiFile, "            return None\n");
    fprintf(PyDBApiFile, "        record = self._make()\n");
    if (no > 0)
        fprintf(PyDBApiFile, "        self._copy_input(record)\n");
    fprintf(PyDBApiFile, "        self._get_output(record, result)\n");
    fprintf(PyDBApiFile, "        return record\n");
}

static void generateMultiple(TYYProc &proc)
{
    int no = numInputs(proc);
    if (no > 0)
        generateCopyInput(proc);
    generateGetOutput(proc);
    generateSqlCommand(proc);
    fprintf(PyDBApiFile, "        cursor = connect.cursor()\n");
    if (no > 0)
        generateExecuteWithBinds(proc);
    else
        fprintf(PyDBApiFile, "        cursor.execute(_command)\n");
    fprintf(PyDBApiFile, "        records = []\n");
    fprintf(PyDBApiFile, "        for row in cursor:\n");
    fprintf(PyDBApiFile, "            record = self._make()\n");
    if (no > 0)
        fprintf(PyDBApiFile, "            self._copy_input(record)\n");
     fprintf(PyDBApiFile, "            self._get_output(record, row)\n");
     fprintf(PyDBApiFile, "            records.append(record)\n");
     fprintf(PyDBApiFile, "        return records\n");
}

static void generateCode(TYYTable &table)
{
    for (int p = 0; p < table.noProcs; p++)
    {
        TYYProc &proc = table.Procs[p];
        if (proc.isData != 0)
            continue;
        TBChar parent, current;
        current.set(useName(table));
        current.append(proc.Name);
        if (proc.noFields == 0)
        {
            parent.set("object");
        }
        else if (proc.useStd)
        {
            parent.set("D");
            parent.append(useName(table));
        }
        else
        {
            parent.set("D");
            parent.append(useName(table));
            parent.append(proc.Name);
        }
        fprintf(PyDBApiFile, "class %s(%s):\n", current.data, parent.data);
        if (proc.noFields - numDynamics(proc) == 0)
            generateCommand(proc);
        else if (proc.noOutputs == 0)
            generateAction(proc);
        else if (proc.isSingle == 1)
            generateSingle(proc);
        else
            generateMultiple(proc);
        generateProcYaml(proc);
        fprintf(PyDBApiFile, "\n");
    }
}

void GeneratePyDBApi(PYYTable Table)
{
    makePyDBApiName(Table);
    PyDBApiFile = openFile("PyDBApi File", Table->PyDBApiFileName);
    if (PyDBApiFile == 0)
        return;
    fprintf(PyDBApiFile, "# This code was generated, do not modify it, modify it at source and regenerate it.\n");
    fprintf(PyDBApiFile, "# %s %s\n\n", g_fname, g_ext);
    fprintf(PyDBApiFile, "import dbapi_util\n\n");
    generateConsts(*Table);
    if (Table->noFields > 0)
        generateStdD(*Table);
    for (int p = 0; p < Table->noProcs; p++)
    {
        TYYProc &proc = Table->Procs[p];
        if (proc.isData != 0 || proc.noFields == 0)
            continue;
        if (proc.useStd)
            continue;
        generateProcD(*Table, proc);
    }
    generateCode(*Table);
}
