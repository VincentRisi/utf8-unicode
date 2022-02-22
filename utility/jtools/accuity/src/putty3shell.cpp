#include "putty3shell.h"
using namespace std;

const char* initial =
"import sys, datetime, os, os.path\n"
"import INTRINSICS\n"
"from %s_IMPL import *\n"
"from HTTPSERVER_UTIL import *\n"
"config_file = r'%s'\n"
"rc, config = do_logons(config_file)\n"
"INTRINSICS.config = config\n"
"for path in sys.path:\n"
"  INTRINSICS.log_info (f'path: {path}')\n"
;

char* Putty3Shell::modulePrefix;
static tLogFile* logFile;

inline void getString(PyObject* item, string &gotten)
{
    PyObject * l1 = PyUnicode_AsLatin1String(item);
    gotten = PyBytes_AsString(l1);
    Py_DECREF(l1);
}

Putty3Shell::Putty3Shell(tLogFile* aLogFile, const char* name, const char* configfile, const char* prefix)
{
    logFile = aLogFile;
    modulePrefix = strdup(prefix);
    char work[8192];
    snprintf(work, sizeof(work), initial, prefix, configfile);
    logFile->lprintf(eLogDebug, "'initial'\n%s", work);
    program = Py_DecodeLocale(name, 0);
    Py_SetProgramName(program);
    Py_Initialize();
    PyRun_SimpleString(work);
}

Putty3Shell::~Putty3Shell()
{
    int rc = Py_FinalizeEx();
    PyMem_RawFree(program);
}

static void _traceback_getFilename(TBChar &ErrorBuf, PyObject *traceback)
{
  PyObject *tb_frame = PyObject_GetAttrString(traceback, "tb_frame");
  if (tb_frame) 
  {
    PyObject *f_code = PyObject_GetAttrString(tb_frame, "f_code");
    if (f_code) 
    {
      PyObject *co_filename = PyObject_GetAttrString(f_code, "co_filename" );
      ErrorBuf.append(PyBytes_AsString(co_filename));
      Py_DECREF(co_filename);
      ErrorBuf.append(" Function:");
      PyObject *co_name = PyObject_GetAttrString(f_code, "co_name" );
      ErrorBuf.append(PyBytes_AsString(co_name));
      Py_DECREF(co_name);
      Py_DECREF(f_code);
    }
    Py_DECREF(tb_frame);
  }
  PyObject *tb_lineno = PyObject_GetAttrString(traceback, "tb_lineno");
  if (tb_lineno)
  {
    char work[64];
    int lineno = (int) PyLong_AsLong(tb_lineno);
    sprintf(work, " Line:%d\n", lineno);
    ErrorBuf.append(work);
    Py_DECREF(tb_lineno);
  }
}

void PyErrorHandler(TBChar &ErrorBuf)
{
   PyObject *errobj, *errdata, *errtraceback, *pystring;
   PyErr_Fetch(&errobj, &errdata, &errtraceback);
   pystring = 0;
   if (errobj != 0 
   && (pystring = PyObject_Str(errobj)) != 0 
   && (PyBytes_Check(pystring)))
     ErrorBuf.set(PyBytes_AsString(pystring));
   else
     ErrorBuf.set("<unknown exception type>");
   ErrorBuf.append("\n");
   Py_XDECREF(errobj);
   Py_XDECREF(pystring);
   pystring = 0;
   if (errdata != 0 
   && (pystring = PyObject_Str(errdata)) != 0 
   && (PyBytes_Check(pystring)))
     ErrorBuf.append(PyBytes_AsString(pystring));
   else
     ErrorBuf.append("<unknown exception data>");
   ErrorBuf.append("\n");
   Py_XDECREF(errdata);         /* caller owns all 3 */
   Py_XDECREF(pystring);
   if (errtraceback != 0)
   {
     do 
     {
       pystring = PyObject_Str(errtraceback);
       if (pystring != 0 && PyBytes_Check(pystring) == true)
         _traceback_getFilename(ErrorBuf, errtraceback);
       Py_XDECREF(pystring);
       PyObject* wallah = PyObject_GetAttrString(errtraceback, "tb_next");
       Py_XDECREF(errtraceback);
       errtraceback = wallah;
     } while (errtraceback != Py_None);
   }
}

struct AutoModulePython
{
    PyObject *module;
    AutoModulePython(const char* name)
    {
        PyObject *pyName;
        pyName = PyUnicode_FromString(name);
        module = PyImport_Import(pyName);
        Py_DECREF(pyName);
    }
    ~AutoModulePython()
    {
        Py_XDECREF(module);
    }
};

void putty3Runner(string methodStr, string inputStr, string &outputStr)
{
    const char* method = methodStr.c_str();
    const char* input  = inputStr.c_str();
    outputStr = "";
    try
    {
        string moduleName = Putty3Shell::modulePrefix;
        moduleName += "_JSON";
        AutoModulePython AM(moduleName.c_str());
        if (AM.module != 0)
        {
            PyObject* pyMethod;
            PyObject* pyArgs;
            PyObject* pyOutput;
            pyMethod = PyObject_GetAttrString(AM.module, method);
            if (pyMethod && PyCallable_Check(pyMethod))
            {
                if (input[0] != 0)
                {
                    pyArgs = PyTuple_New(1);
                    PyObject* pyInput = PyUnicode_FromString(input);
                    PyTuple_SetItem(pyArgs, 0, pyInput);
                    pyOutput = PyObject_CallObject(pyMethod, pyArgs);
                    Py_DECREF(pyArgs);
                }
                else
                {
                    pyOutput = PyObject_CallObject(pyMethod, 0);
                }
                if (pyOutput != 0)
                {
                    getString(pyOutput, outputStr);
                    Py_DECREF(pyOutput);
                }
                Py_DECREF(pyMethod);
            }
        }
        else
        {
            TBChar ErrorBuf;
            PyErrorHandler(ErrorBuf);
            logFile->lprintf(eLogError, "%s\n", ErrorBuf.data);
        }
    }
    catch (...)
    {
        TBChar ErrorBuf;
        PyErrorHandler(ErrorBuf);
        logFile->lprintf(eLogError, "%s\n", ErrorBuf.data);
    }
}
