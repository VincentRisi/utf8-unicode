/****************************************************************************/
/*                                                                          */
/*  System : NRW                                                            */
/*                                                                          */
/*  File   : Report.cpp                                                     */
/*                                                                          */
/*  Target : Archiver.exe                                                   */
/*                                                                          */
/*  Description : Aids The Report Printing Process                          */
/*                                                                          */
/*  Author : M. Papazian (Thanks Max for the non mention)                   */
/*                                                                          */
/*  Date   : Thu 01-Feb-1996                                                */
/*                                                                          */
/*  Version : 1.0.0                                                         */
/*                                                                          */
/*  Change History :                                                        */
/*                                                                          */
/****************************************************************************/
#include "ti.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "report.h"

tReport::tReport(char *aRptName, int aPageSize)
: RptName(aRptName)
, PageSize(aPageSize)
, LineCount(aPageSize)
, PageCount(0)
, Headers(0)
, NoHeaders(0)
{
  if ((PSRpt = fopen(aRptName, "w")) == 0)
    throw("Error : Cannot Open Report File");
}

tReport::~tReport()
{
  if (Headers)
  {
    fprintf(PSRpt, "\f");
    Print(1, "%57s", "*** E N D  O F  R E P O R T ***");
    fprintf(PSRpt, "\f");
    delete [] Headers;
  }
  if (fclose(PSRpt) != 0)
    throw("Error : On Report Close");
}

void tReport::Print(int aLines, const char *aFmt, ...)
{
  va_list  argp;
  if (LineCount > (PageSize - 5))
    PrintHeaders();
  va_start(argp, aFmt);
  if (vfprintf(PSRpt, aFmt, argp) == EOF)
    throw("Error : On Report Write");
  va_end(argp);

  LineCount += aLines;
  while (aLines--)
    fputs("\n", PSRpt);
}

void tReport::PrintHeaders(int aPrintSubHeaders)
{
  int i;
  if (PageCount != 0)
  {
     if (Headers == 0) {
        if (fprintf(PSRpt, "     Page : %d\n\n", ++PageCount) == EOF) {
           throw("Error : On Report Write");
        }
     }
     else
       if (fprintf(PSRpt, "\f%s     Page : %d\n\n", (pchar) Headers[0], ++PageCount) == EOF)
        throw("Error : On Report Write");
  }
  else
  {
     if (Headers == 0) {
        if (fprintf(PSRpt, "     Page : %d\n\n", ++PageCount) == EOF) {
           throw("Error : On Report Write");
        }
     }
    else if (fprintf(PSRpt, "%s     Page : %d\n\n", (pchar) Headers[0], ++PageCount) == EOF)
      throw("Error : On Report Write");
  }

  LineCount = 2;
  if (aPrintSubHeaders)
    for (i=1; i<NoHeaders; i++)
      Print(1, "%s", (pchar) Headers[i]);
}

void tReport::AddHeader(tString aHeader)
{
  Addlist(Headers, NoHeaders, aHeader, DELTA);
}

void tReport::AddHeader(const char *aFmt, ...)
{
char Jotter[2048];
int cb;

  va_list  argp;
  va_start(argp, aFmt);
  cb = vsprintf(Jotter, aFmt, argp);
  if (cb > sizeof(Jotter)) {
     throw("Error : Header To Long");
  }
  va_end(argp);
  tString aHeader = Jotter;
  Addlist(Headers, NoHeaders, aHeader, DELTA);
}

void tReport::ModeHeader(int aHederNo, tString aHeader)
{
tString EmptyHeader;

   if (aHederNo > NoHeaders) {
      int i;
      for (i = NoHeaders; i < aHederNo; i++) {
         Addlist(Headers, NoHeaders, EmptyHeader, DELTA);
      }
   }
   Headers[aHederNo] = aHeader;
}

