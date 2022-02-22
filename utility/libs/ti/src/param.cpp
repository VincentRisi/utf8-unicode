#include "versions.h"
#define _VERSION_NO "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
COMPILED_VERSION(_VERSION_NO);

#include "machine.h"

/*  This module provides the parameter file scanning routines. */
/*  Parameter files consist of sections enclosed in square brackets [], */
/*  keywords enclosed in braces {} and values following the keywords. */
/*  All lines not starting with tabs/spaces followed */
/*  by a '[' or '{' are treated as comments. */
/*  */
/*  For example: */
/*  */
/*    [Input] */
/*    ------- */
/*    {Pipe Name}\pipe\plsl.pip */
/*    {Date}20010101 */
/*  */
/*  The 1st line defines the section "[Input]" */
/*  The 2nd line is ignored */
/*  The 3rd line defines the keyword "{Pipe Name}" with value "\pipe\plsl.pip" */
/*  The 4rd line defines the keyword "{Date}" with value "20010101". Trailing */
/*  spaces are removed. */

#include <stdio.h>
#include <string.h>

#include "xstring.h"
#include "param.h"

FILE* ParmFile;
char  LastLine[MAXLINE];

int ParamInitLoad(char* fname)
{
  ParmFile = fopen(fname, "r");
  return ParmFile == 0;
}

void ParamTerminateLoad(void)
{
  fclose(ParmFile);
}

int ParamGetNextSection(char* section, size_t sectionLen)
{
  char *nl;

  if (*LastLine == '[')
  {
    strncpyz(section, LastLine, sectionLen - 1);
    *LastLine = ' ';
    return 0;
  }

  while (fgets(LastLine, MAXLINE, ParmFile))
  {
    /*  replace terminating newline with a space */
    if ((nl = strrchr(LastLine, '\n')) != 0)
      *nl = ' ';
    strtrim(LastLine);
    if (*LastLine == '[')
    {
      strncpyz(section, LastLine, sectionLen - 1);
      *LastLine = ' ';
      return 0;
    }
  }

  return errNO_MORE_SECTIONS;               /*  end of file */
}

int ParamGetNextKeyword(char* keyword, size_t keywordLen, char* value,
  size_t valueLen)
{
  char* rbrace;
  char *nl;

  while (fgets(LastLine, MAXLINE, ParmFile))
  {
    /*  replace terminating newline with a space */
    if ((nl = strrchr(LastLine, '\n')) != 0)
      *nl = ' ';
    strtrim(LastLine);
    if (*LastLine == '[')
      return errNO_MORE_KEYWORDS;
    if (*LastLine == '{')
    {
      rbrace = strchr(LastLine, '}');
      if (!rbrace)
        return errBAD_LINE;
      strncpyz(value, rbrace + 1, valueLen - 1);
      rbrace[1] = 0;
      strncpyz(keyword, LastLine, keywordLen - 1);
      return 0;
    }
  }

  return errNO_MORE_KEYWORDS;               /*  end of file */
}

/*  -------------------------------------------------------------- */
#ifdef PARAM_TST
#include <stdio.h>
#include <assert.h>

void writedata(void);

void main(void)
{
  int ret;
  char section[80];
  char keyword[80];
  char value[80];
  int rc;

  writedata();
#define testcnf "test.cnf"
  rc = ParamInitLoad(testcnf);
  assert(!rc);

  ret = ParamGetNextSection(section, 80);
#define sectabc "[abc]"
  assert(!ret && streq(section, sectabc));
  ret = ParamGetNextKeyword(keyword, 80, value, 80);
#define keywaaa "{aaa}"
#define val123  "123"
  assert(!ret && streq(keyword, keywaaa) && streq(value, val123));
  ret = ParamGetNextKeyword(keyword, 80, value, 80);
#define keywbbb "{bbb}"
#define val4567 "4567"
  assert(!ret && streq(keyword, keywbbb) && streq(value, val4567));
  ret = ParamGetNextKeyword(keyword, 80, value, 80);
  assert(ret == errNO_MORE_KEYWORDS);

  ret = ParamGetNextSection(section, 80);
#define sectdef "[def]"
  assert(!ret && streq(section, sectdef));
  ret = ParamGetNextKeyword(keyword, 80, value, 80);
#define keywccc "{ccc}"
#define valharry "harry"
  assert(!ret && streq(keyword, keywccc) && streq(value, valharry));
  ret = ParamGetNextKeyword(keyword, 80, value, 80);
  assert(ret == errNO_MORE_KEYWORDS);

  ret = ParamGetNextSection(section, 80);
  assert(!ret && streq(section, sectabc));
  ret = ParamGetNextKeyword(keyword, 80, value, 80);
  assert(ret == errNO_MORE_KEYWORDS);

  ret = ParamGetNextSection(section, 80);
#define sectwxy "[wxy]"
  assert(!ret && streq(section, sectwxy));
  ret = ParamGetNextKeyword(keyword, 80, value, 80);
  assert(ret == errBAD_LINE);
  ret = ParamGetNextKeyword(keyword, 80, value, 80);
#define keyw222 "{222}"
#define valnone ""
  assert(!ret && streq(keyword, keyw222) && streq(value, valnone));
  ret = ParamGetNextKeyword(keyword, 80, value, 80);
#define keyw333 "{333}"
#define valnoerr "no error here"
  assert(!ret && streq(keyword, keyw333) && streq(value, valnoerr));
  ret = ParamGetNextKeyword(keyword, 80, value, 80);
  assert(ret == errNO_MORE_KEYWORDS);

  ret = ParamGetNextSection(section, 80);
  assert(ret == errNO_MORE_SECTIONS);

  ParamTerminateLoad();
  printf("All OK folks!\n");
}

void writedata(void)
{
  FILE* f;

  f = fopen(testcnf, "w");
  fputs("\n",f);
  fputs("Self is a comment\n",f);
  fputs("[abc]\n",f);
  fputs("  {aaa}123\n",f);
  fputs("  {bbb}4567\n",f);
  fputs("[def]\n",f);
  fputs("  the following line has trailing blanks\n",f);
  fputs("  {ccc}harry         \n",f);
  fputs("[abc]\n",f);
  fputs("[wxy]\n",f);
  fputs("{111]error here\n",f);
  fputs("{222}\n",f);
  fputs("{333}no error here\n",f);
  fclose(f);
}

#endif

