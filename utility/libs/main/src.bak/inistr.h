#ifndef _TINISTR_H_
#define _TINISTR_H_
/****************************************************************************/
/*                                                                          */
/*  System : NRW                                                            */
/*                                                                          */
/*  File   : inistr.h                                                       */
/*                                                                          */
/*  Description : class for loading of parameters from an 'ini' file        */
/*                                                                          */
/*  Author : S. Shemesh                                                     */
/*                                                                          */
/*  Date   : Fri 3 Nov 2000                                                 */
/*                                                                          */
/*  $Author: vince $                                                        */
/*                                                                          */
/*  $Revision: 92 $                                                        */
/*                                                                          */
/*  $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $                                            */
/*                                                                          */
/*  Change History :                                                        */
/*                                                                          */
/*
*   $Log: IniStr.h,v $
*   Revision 412.1  2004/11/18 10:26:44  vince
*   no message
*
*   Revision 409.1  2004/07/20 09:14:10  vince
*   no message
*
*   Revision 407.1  2004/06/16 09:41:26  vince
*   no message
*
*   Revision 405.1  2004/04/16 12:32:46  vince
*   no message
*
*   Revision 404.1  2004/03/27 09:40:19  vince
*   no message
*
*   Revision 403.1  2004/02/27 12:09:54  vince
*   no message
*
*   Revision 401.1  2003/12/02 14:50:02  vince
*   no message
*
*   Revision 310.1  2003/11/10 10:04:31  vince
*   no message
*
*   Revision 3.10  2003/11/07 13:54:08  vince
*   no message
*
*   Revision 1.1.1.1  2003/11/04 14:09:43  vince
*   no message
*
*   Revision 1.3  2003/09/19 08:54:25  vince
*   no message
*
*   Revision 1.10  2003/09/18 16:52:07  vince
*   no message
*
*   Revision 1.9  2003/04/24 14:52:13  vince
*   no message
*
*   Revision 1.8  2002/04/09 13:40:58  simon
*   hrow exception when there is no value
*
*   Revision 1.7  2002/04/04 08:55:31  vince
*   no message
*
*   Revision 1.6  2002/03/14 08:05:51  simon
*   no message
*
*   Revision 1.5  2002/03/14 08:02:43  simon
*   no message
*
*   Revision 1.4  2002/03/13 15:00:22  simon
*   add cvs info
*
*****************************************************************************/
#include "machine.h"
#include <stdio.h>

#include "xcept.h"
#include "tstring.h"
#include "tbuffer.h"


const int MEM_SIZE = 10240;

//--------------------------------------------------

class tINISTR
{
public:
  tINISTR(char * aInifile);                     // constructor
  ~tINISTR();                                   // destructor

  bool QueryValueDefault(tString aKeyword, tString & aValue, tString aSection);

  // following functions throw exceptions if keyword is not found

  void QueryValue(pchar aKeyword,      // get a value back for keyword
                  tString & aValue,    // and section - value as string
                  pchar aDefault = 0,  // default value if not found
                  pchar aSection = 0);

  void QueryValue(pchar aKeyword,      // get a value back for keyword
                  pchar & aValue,      // and section - value as string
                  pchar aDefault = 0,  // default value if not found
                  pchar aSection = 0);

  void QueryValue(pchar aKeyword,      // get a value back for keyword
                  int & aValue,        // and section - value as int
                  int * aDefault = 0,    // default value if not found
                  pchar aSection = 0);

  void QueryValue(pchar aKeyword,      // get a value back for keyword
                  long & aValue,       // and section - value as long
                  long * aDefault = 0,       // default value if not found
                  pchar aSection = 0);

  void QueryValue(pchar aKeyword,      // get a value back for keyword
                  double & aValue,     // and section - value as double
                  double * aDefault = 0,     // default value if not found
                  pchar  aSection = 0);

  void QueryValue(pchar aKeyword,      // get a value back for keyword
                  bool & aValue,       // and section - value as bool
                  pchar  aTrueValue,    // true value else false
                  bool * aDefault = 0,       // default value if not found
                  pchar  aSection = 0);

  void SetSection(pchar aSection);             // set the section
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

