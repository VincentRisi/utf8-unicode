#ifndef _TINISTR_H_
#define _TINISTR_H_ 

// ***********************************************************************
//                                                                        
//   System : NRW                                                         
//                                                                        
//   File   : inistr.h                                                    
//                                                                        
//   Description : class for loading of parameters from an 'ini' file     
//                                                                        
//   Author : S. Shemesh                                                  
//                                                                        
//   Date   : Fri 3 Nov 2000                                              
//                                                                        
// ***********************************************************************
#include "ti.h"
#include <stdio.h>

#include "xcept.h"
#include "tstring.h"
#include "tbuffer.h"

const int MEM_SIZE = 10240;

//--------------------------------------------------
CLASSDEF(INISTR);
class tINISTR
{
public:
  tINISTR(char * aInifile);                     // constructor
  ~tINISTR();                                   // destructor
  bool QueryValueDefault(tString aKeyword, tString & aValue, tString aSection);
  // following functions throw exceptions if keyword is not found
  void QueryValue(pchar aKeyword,   // get a value back for keyword
                  tString & aValue,    // and section - value as string
    pchar aDefault = 0,             // default value if not found
    pchar aSection = 0);            
  void QueryValue(pchar aKeyword,   // get a value back for keyword
    pchar &aValue,                  // and section - value as string
    pchar aDefault = 0,             // default value if not found
    pchar aSection = 0);            
  void QueryValue(pchar aKeyword,   // get a value back for keyword
                  int & aValue,        // and section - value as int
                  int * aDefault = 0,    // default value if not found
    pchar aSection = 0);            
  void QueryValue(pchar aKeyword,   // get a value back for keyword
                  long & aValue,       // and section - value as long
                  long * aDefault = 0,       // default value if not found
    pchar aSection = 0);            
  void QueryValue(pchar aKeyword,   // get a value back for keyword
                  double & aValue,     // and section - value as double
                  double * aDefault = 0,     // default value if not found
    pchar  aSection = 0);           
  void QueryValue(pchar aKeyword,   // get a value back for keyword
                  bool & aValue,       // and section - value as bool
    pchar aTrueValue,               // true value else false
                  bool * aDefault = 0,       // default value if not found
    pchar aSection = 0);
  void SetSection(pchar aSection);                // set the section
  tString  QueryNextKeyword(tString & aKeyword);  // get next keyword / value
  tString QueryNextSection(void);                // get next section
private:
  tINISTR(); // default constructor not used - causes link error if used
  tString  section;
  tString  keyword;
  short   iniok;
  char    inifile[500];
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
XCLASSDEF(INISTR);
class xINISTR : public xCept
{
public:
  enum eError { errOpenFailed = 1,
                errNoSection = 2,
                errNoKeyword = 3,
                errStatFailed =4,
                errStatNotRegular =5,
                errNoValue = 6
               };
  xINISTR(pchar aFname, int aLine, int aError, pchar aMsg);
  xINISTR(pchar aFname, int aLine, eError aError, pchar aIniFile, pchar aSection = NULL, char * aKeyword = NULL);
  xINISTR(const xINISTR& x);
private:
  void Construct(eError aError,char *aSection=NULL, char *aKeyword=NULL, char *aFileName=NULL);
  char   m_keyword[50],
         m_section[50];
};

inline xINISTR::xINISTR(const xINISTR& x) : xCept(x) {}

#if defined(M_GNU)
		#define XINISTR(err, Msg) \
  xINISTR(__FILE__, __LINE__, xINISTR::err, Msg)
		#define XINISTRF(err, Section, Keyword) \
		  xINISTR(__FILE__, __LINE__, xINISTR::err, inifile, (char *)Section, (char *)Keyword)
		#define XINISTRS(err, Section) \
		  xINISTR(__FILE__, __LINE__, xINISTR::err, inifile, (char *)Section)
		#define XINISTRFILE(err, file) \
		  xINISTR(__FILE__, __LINE__, xINISTR::err, file)
#else
    #define XINISTR(err, Msg) \
      xINISTR(__FILE__, __LINE__, xINISTR::##err, Msg)
    #define XINISTRF(err, Section, Keyword) \
      xINISTR(__FILE__, __LINE__, xINISTR::##err, inifile, (char *)Section, (char *)Keyword)
    #define XINISTRS(err, Section) \
      xINISTR(__FILE__, __LINE__, xINISTR::##err, inifile, (char *)Section)
    #define XINISTRFILE(err, file) \
      xINISTR(__FILE__, __LINE__, xINISTR::##err, file)
#endif  
#endif

