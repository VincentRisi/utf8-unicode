#ifndef _PUTTY3SHELL_H_
#define _PUTTY3SHELL_H_
#include "machine.h"
using namespace std;

#ifdef _DEBUG
#undef _DEBUG
#endif
#include "Python.h"
#include "tbuffer.h"
#include "logfile.h"

class Putty3Shell
{
    wchar_t *program;
public:
    Putty3Shell(tLogFile* logFile, const char* name, const char* configfile, const char* prefix);
    ~Putty3Shell();
    static char* modulePrefix;
};

void putty3Runner(string methodStr, string inputStr, string &outputStr);
void PyErrorHandler(TBChar &ErrorBuf);

#endif