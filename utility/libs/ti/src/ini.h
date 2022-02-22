// FileName: ini.h
// Platform: aix
// Description: class for loading of parameters from an 'ini' file
// SubSystem: infrastructure
// Author: P. Searle
// Date Created: August 1994
// class tINI
// class xINI

#ifndef _TINI_H_
#define _TINI_H_ 

#include "ti.h"
#include "xcept.h"
#include "tbuffer.h"

#include <stdio.h>

const int MEM_SIZE = 10240;

//--------------------------------------------------
class tINI
{
public:
  tINI(char * aInifile);                     // constructor
  ~tINI();                                   // destructor
  // following functions throw exceptions if keyword is not found
  void QueryValue(tString aKeyword,       // get a value back for keyword
                  tString & aValue,       // and section - value as string
                  tString aSection = "");
  void QueryValue(tString aKeyword,       // get a value back for keyword
                  int & aValue,           // and section - value as int
                  tString aSection = "");
  void QueryValue(tString aKeyword,       // get a value back for keyword
                  long & aValue,          // and section - value as long
                  tString aSection = "");
  void QueryValue(tString aKeyword,       // get a value back for keyword
                  double & aValue,        // and section - value as double
                  tString aSection = "");
  // same as QueryValue(tString aKeyword, tString aValue, tString aSection)
  // but if not found no exception is thrown, and aValue is set to aDefault
  bool QueryValueDefault(                 // true iff value found
                  tString aKeyword,       // get a value back for keyword
                  tString & aValue,       // and section - value as string
                  tString aDefault,       // default value if not found
                  tString aSection = "");
  void SetSection(tString aSection);             // set the section
  tString QueryNextKeyword(tString & aKeyword);  // get next keyword / value
  tString QueryNextSection(void);                // get next section
private:
  tINI(); // default constructor not used - causes link error if used
  tString section;
  tString keyword;
  short   iniok;
  tString inifile;
  FILE    *hf;
  int     BufferRetained;
  char    RetainBuffer[500];
  char    *pReadBuff;
  size_t  cbBuff;
  short   NextByte;
  void  Retain(char * aData);
  short InitLoad(char * aFileName);
  int   ReadData(FILE * aHf, char * aData);
  int   ReadConfig(FILE *aHf, char * aData);
  int   GetNextSection(tString & aSection);
  int   GetNextKeyword(tString & aKeyword, tString & aParam);
  void  TerminateLoad(void);
  int  InitSave(char * aFileName);
  void WriteKeyword(char * aKeyword, char * aValue);
  void WriteSection(char * aSection);
  void TerminateSave(void);
  void GetEnv(tString &aValue);
};

//--------------------------------------------------
class xINI : public xCept
{
public:
  enum eError { errOpenFailed = 1,
                errNoSection = 2,
                errNoKeyword = 3,
                errStatFailed =4,
                errStatNotRegular =5
               };
  xINI(const pchar aFname, int aLine, eError aError,
       char * aSection = NULL, char * aKeyword = NULL);
  xINI(const pchar aFname, int aLine, eError aError, tString aIniFile,
       char * aSection = NULL, char * aKeyword = NULL);
  xINI(const xINI& x);
private:
  void Construct(eError aError,char *aSection=NULL, char *aKeyword=NULL);
//  char m_keyword[50],  m_section[50];
};

inline xINI::xINI(const xINI& x)
: xCept(x)
{}

#define XINI(err) xINI(__FILE__, __LINE__, xINI::err)
#define XINIFILE(err, file) xINI(__FILE__, __LINE__, xINI::err, file)

#endif

