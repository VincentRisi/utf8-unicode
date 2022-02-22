#ifndef _REPORT_H_
#define _REPORT_H_ 

#include "tstring.h"
#include "list.hpp"

CLASSDEF(Report);
class tReport
{
  FILE *PSRpt;
  tString RptName;
  pString Headers;
  int PageSize;
  int PageCount;
  int LineCount;
  int NoHeaders;
public:
  tReport(char *aRptName, int aPageSize = 66);
  ~tReport();
  void Print(int aLines, const char *aFmt, ...);
  void PrintHeaders(int aPrintSubHeaders=1);
  void BuildHeader(tString aText, tString StripDate);
  void AddHeader(tString aHeader);
  void AddHeader(const char *aFmt, ...);
  void ModeHeader(int aHederNo, tString aHeader);
  void FormFeed() {LineCount = PageSize;}
};

#endif

