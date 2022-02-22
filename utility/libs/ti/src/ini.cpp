
//FileName: ini.c
//Platform: aix
//Description: class for loading of parameters from an 'ini' file
//SubSystem: infrastructure
//Author: P. Searle
//Date Created: August 1994
//
// CHANGES :
//     SS  15 jan 1995 -  check that inifile actually is a regular file i.e
//                        change execeptions so that keyword and section
//                        are displayed for missing keywords etc
//
//     M.Barnard 30 Dec 97 - Allowed size of data in ini file to
//                           be 500 char not just 80 char

#include "ini.h"
#if !defined(M_AIX) && !defined(M_GNU)
#include <io.h>
#endif
#include <time.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifndef _S_IFMT
#define _S_IFMT S_IFMT
#endif
#ifndef _S_IFREG
#define _S_IFREG S_IFREG
#endif

#define xININame "xINI"

// exception class for tINI
xINI::xINI(pchar aFname, int aLine, eError aError,
    char* section, char* keyword) :
    xCept(aFname, aLine, xININame, aError)
{
    Construct(aError, section, keyword);
};

xINI::xINI(pchar aFname, int aLine, eError aError, tString aIniFile,
    char* section, char* keyword) :
    xCept(aFname, aLine, xININame, aError)
{
    Construct(aError, section, keyword);
    osErr << endl << "INI file - " << aIniFile;
}

void xINI::Construct(eError aError, char* section, char* keyword)
{
    switch (aError)
    {
    case errOpenFailed:
        osErr << "INI file " << section << " open failed" << ends;
        break;
    case errNoSection:
        osErr << "INI file no matching section " << section << ends;
        break;
    case errNoKeyword:
        osErr << "INI file no matching keyword " << keyword << " in section " << section << ends;
        break;
    case errStatFailed:
        osErr << "INI file " << section << " unable to get file statistics" << ends;
        break;
    case errStatNotRegular:
        osErr << "INI file must be a regular file" << ends;
        break;
    }
}

static char EMPTY_STRING[1];
// tINI class
tINI::tINI(char* aIniFile)
{
    hf = 0;
    pReadBuff = 0;
    BufferRetained = 0;
    NextByte = cbBuff = 0;
    inifile = EMPTY_STRING;
    section.SetCompMode(tString::cmIgnore);    // change compare mode
    iniok = InitLoad(aIniFile); // open file
}

tINI::~tINI()
{
    TerminateLoad();
}

void tINI::SetSection(tString aSection)
{
    tString nextSection = EMPTY_STRING;
    // set the section to given value
    section = aSection;
    // reset file pointer to begining
    // as SetSection cannot make
    // assumptions about file pos
    if (fseek(hf, SEEK_SET, 0))
    {
        // unable to set file pos back to start
        // close file and open again
        fclose(hf);
        hf = fopen(inifile, "r");
        if (!hf)
            throw xINI(__FILE__, __LINE__, xINI::errNoSection, (char*)aSection);
    }
    NextByte = 0; // Reset mem buffer so we start reading from file beginning
    cbBuff = 0;

    // check if section name is wrapped in brackets
    // if not wrap it up
    if ((section[(size_t)0] != '[') && (section[section.Length()] != ']'))
        section = tString("[") + aSection + tString("]");
    // get to the position in file for specified section
    section.SetCompMode(tString::cmIgnore);        // change compare mode
    nextSection.SetCompMode(tString::cmIgnore);    // change compare mode
    for (;;)
    {
        // get the next section from ini file
        if (!GetNextSection(nextSection))
            throw xINI(__FILE__, __LINE__, xINI::errNoSection, (char*)section);
        // if next section in ini file matches the section set then break
        if (section == nextSection)
            return;
    }
}

tString tINI::QueryNextSection(void)
{
    tString nextSection = EMPTY_STRING;
    GetNextSection(nextSection);
    return (nextSection);
}

tString tINI::QueryNextKeyword(tString& aKeyword)
{
    tString nextKeyword = EMPTY_STRING;
    tString nextValue = EMPTY_STRING;
    // initialise aKeyword to nothing
    aKeyword = EMPTY_STRING;
    // get next keyword
    for (;;)
    {
        if (GetNextKeyword(nextKeyword, nextValue))
        {
            aKeyword = nextKeyword;
            return nextValue;
        }
        else
            break;
    }
    // not found so return empty value
    return (nextValue);
}

void tINI::QueryValue(tString aKeyword, tString& aValue, tString aSection)
{
    tString aDefault = EMPTY_STRING;
    bool rc = QueryValueDefault(aKeyword, aValue, aDefault, aSection);
    if (!rc)
    {
        if (!aSection.Length())
            aSection = section;
        throw xINI(__FILE__, __LINE__, xINI::errNoKeyword, (char*)aSection, (char*)aKeyword);
    }
}

void tINI::QueryValue(tString aKeyword, int& aValue, tString aSection)
{
    tString Value = EMPTY_STRING;
    // use QueryValue for a tString
    QueryValue(aKeyword, Value, aSection);
    // if value was found
    if (Value.Length())
        // convert to short
        aValue = atoi((const char*)Value);
    else
        aValue = (int)0;
}

void tINI::QueryValue(tString aKeyword, long& aValue, tString aSection)
{
    tString Value = EMPTY_STRING;
    // use QueryValue for a tString
    QueryValue(aKeyword, Value, aSection);
    // if value was found
    if (Value.Length())
        // convert to short
        aValue = atol((const char*)Value);
    else
        aValue = (long)0;
}

void tINI::QueryValue(tString aKeyword, double& aValue, tString aSection)
{
    tString Value = EMPTY_STRING;
    // use QueryValue for a tString
    QueryValue(aKeyword, Value, aSection);
    // if value was found
    if (Value.Length())
        // convert to short
        aValue = atof((const char*)Value);
    else
        aValue = (double)0.0;
}

void tINI::GetEnv(tString& aValue)
{
    if (aValue.Length() == 0)
        return;
    TBChar work1, work2, work3;
    work1.set(aValue.c_str());
    char* p = work1.data, * q, * r;
    while (p[0] && (q = strstr(p, "$(")) != 0)
    {
        r = strstr(q, ")");
        int n = r - q - 2;
        if (r == 0 || n <= 0)
            break;
        if (p != q)
            work2.append(p, q - p);
        work3.set(q + 2, n);
        char* e = getenv(work3);
        if (e)
            work2.append(e);
        p = r + 1;
    }
    if (p == work1.data)
        return;
    work2.append(p);
    aValue = work2.data;
}

bool tINI::QueryValueDefault(tString aKeyword, tString& aValue,
    tString aDefault, tString aSection)
{
    tString nextSection = EMPTY_STRING;
    tString nextKeyword = EMPTY_STRING;
    tString nextValue = EMPTY_STRING;
    // if section was not specfified then default to current
    if (!aSection.Length())
        nextSection = section;
    else
        // set to section specified
        nextSection = aSection;
    // start at the top of the section again
    SetSection(nextSection);
    aKeyword.SetCompMode(tString::cmIgnore);     // change compare mode
    nextValue.SetCompMode(tString::cmIgnore);    // change compare mode
    // get to keyword and value
    for (;;)
    {
        if (GetNextKeyword(nextKeyword, nextValue))
        {
            if (aKeyword == nextKeyword)
            {
                // matched keyword
                aValue = nextValue;
                GetEnv(aValue);
                return true;
            }
        }
        else // end of ini section keyword not matched
            break;
    }
    // set default
    aValue = aDefault;
    GetEnv(aValue);
    return false;
}

int tINI::ReadData(FILE* aHf, char* aData)
{
    if ((size_t)NextByte < cbBuff)
    {
        aData[0] = pReadBuff[NextByte];
        NextByte++;
        return (1);
    }
    NextByte = 0;
    cbBuff = 0;
    cbBuff = fread(pReadBuff, 1, MEM_SIZE, aHf);
    if (!cbBuff)
        return (0);
    aData[0] = pReadBuff[NextByte];
    NextByte++;
    return (1);
}

int tINI::ReadConfig(FILE* aHf, char* aData)
{
    short i = 0;
    char  Jotter[500];
    if (BufferRetained)
    {
        strcpy(aData, RetainBuffer);
        BufferRetained = 0;
        return (1);
    }
    for (;;)
    {
        if (!ReadData(aHf, &Jotter[i])) break;
        if (Jotter[i] == 0x0A)
        {
            if (i < 2)
            {
                i = 0;
                continue;
            }
            Jotter[i] = 0x00;
            strcpy(aData, Jotter);
            return (1);
        }
        i++;
        if (i > 500)
            return(1);
    }
    return (0);
}

void tINI::Retain(char* aData)
{
    BufferRetained = 1;
    strcpy(RetainBuffer, aData);
}

short tINI::InitLoad(char* aFileName)
{
    struct stat buf;
    // S Sena Check that path refers to a regular text file 15/1/95
    if (stat(aFileName, &buf) < 0)
        throw XINIFILE(errStatFailed, aFileName);
    else if ((buf.st_mode & _S_IFMT) != _S_IFREG)
        throw XINIFILE(errStatNotRegular, aFileName);
    hf = fopen(aFileName, "r");
    if (!hf)
        throw XINIFILE(errOpenFailed, aFileName);
    /* set our private file name to the param */
    inifile = aFileName;
    pReadBuff = (char*)malloc(MEM_SIZE);
    if (!pReadBuff)
        return 1;
    memset(pReadBuff, 0, MEM_SIZE);
    cbBuff = 0;
    NextByte = 0;
    return 0;
}

void tINI::TerminateLoad(void)
{
    if (hf)
        fclose(hf);
    if (pReadBuff)
        free(pReadBuff);
}

int tINI::GetNextSection(tString& aSection)
{
    char data[500];
    for (;;)
    {
        if (!ReadConfig(hf, data))
            return (0);
        if (data[0] != '[')
            continue;
        // We have found the next Section. Return
        // the section details.
        aSection = data;
        return (1);
    }
}

int tINI::GetNextKeyword(tString& aKeyword, tString& aParam)
{
    char  data[500];
    char  keyword[500];
    char  param[500];
    short i = 0, bound = 0;
    for (;;)
    {
        if (!ReadConfig(hf, data))
            return (0);
        if (data[0] == '[')
        {
            // We have hit the next section.
            // retain the current buffer for
            // the next read.
            Retain(data);
            return (0);
        }
        if (data[0] != '{')
            continue;
        // We have found the next keyword. Return
        // the keyword and the parameter.
        bound = -1;
        for (i = 1; i < (short)strlen(data); i++)
        {
            if (data[i] != '}')
                continue;
            bound = i;
            break;
        }
        if (bound == -1)
        {
            // Invalid Syntax, ignore the line
            continue;
        }
        // Extract the Keyword
        memcpy(keyword, data, bound + 1);
        keyword[bound + 1] = 0x00;
        // Extract the Parameter
        memcpy(param, &data[bound + 1], strlen(data) - (bound + 1));
        param[strlen(data) - (bound + 1)] = 0x00;
        aKeyword = keyword;
        aParam = param;
        return (1);
    }
}

/**************************************************************************/
/*                                                                        */
/* Saving Procedures.                                                     */
/*                                                                        */
/**************************************************************************/

int tINI::InitSave(char* aFileName)
{
    hf = fopen(aFileName, "w");
    if (!hf)
        // stuffed
        return (0);
    return (1);
}

void tINI::WriteKeyword(char* aKeyword, char* aValue)
{
    char  newLine[5];
    newLine[0] = 0x0D;
    newLine[1] = 0x0A;
    newLine[2] = 0x00;
    fwrite(aKeyword, strlen(aKeyword), 1, hf);
    fwrite(aValue, strlen(aValue), 1, hf);
    fwrite(newLine, strlen(newLine), 1, hf);
}

void tINI::WriteSection(char* aSection)
{
    char  newLine[5];
    newLine[0] = 0x0D;
    newLine[1] = 0x0A;
    newLine[2] = 0x00;
    fwrite(newLine, strlen(newLine), 1, hf);
    fwrite(aSection, strlen(aSection), 1, hf);
    fwrite(newLine, strlen(newLine), 1, hf);
    fwrite(newLine, strlen(newLine), 1, hf);
}

void tINI::TerminateSave(void)
{
    fclose(hf);
}


