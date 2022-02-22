/****************************************************************************/
/*                                                                          */
/*  System : NRW                                                            */
/*                                                                          */
/*  File   : inistr.Cpp                                                       */
/*                                                                          */
/*  Description : class for loading of parameters from an 'ini' file        */
/*                                                                          */
/*  Author : S. Shemesh                                                     */
/*                                                                          */
/*  Date   : Fri 3 Nov 2000                                                */
/*                                                                          */
/*  Version : 1.0.0                                                         */
/*                                                                          */
/*  Change History :                                                        */
/*                                                                          */
/****************************************************************************/
#include "inistr.h"

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

#define xINISTRName "xINISTR"

xINISTR::xINISTR(pchar aFname, int aLine, int aError, pchar aMsg)
: xCept(aFname, aLine, xINISTRName, aError) {
   osErr << aFname << "(" << aLine << ") ";
   if ( aError )
      osErr << "Erc(" << aError << ") ";
   if ( aMsg )
      osErr << aMsg << endl;
   osErr << ends;
}

// exception class for tINI
xINISTR::xINISTR(pchar aFname, int aLine, eError aError, pchar aIniFile, char * section, char * keyword) :
xCept(aFname, aLine, xINISTRName, aError) {
   Construct(aError, section, keyword, aIniFile);
//   osErr << endl << "INI file - " << aIniFile;
};

void xINISTR::Construct(eError aError, char * section, char * keyword, char * aIniFile) {

   switch ( aError ) {
      case errOpenFailed:
         //sprintf(errorStr, "Open INI file (%s) Faild", aIniFile);
				osErr << "Open INI file (" << aIniFile << ") Failed";
         break;
      case errNoSection:
         //sprintf(errorStr, "no matching section %s", section);
					osErr << "no matching section " << section;
         break;
      case errNoKeyword:
         //sprintf(errorStr, "no matching keyword %s in section %s", keyword, section);
				osErr << "no matching keyword " << keyword << " in section " << section;
         break;
      case errStatFailed:
         //sprintf(errorStr, "unable to get file statistics for %s", aIniFile);
				 osErr << "unable to get file statistics for " << aIniFile;
         break;
      case errStatNotRegular:
         //sprintf(errorStr, "INI file %s must be a regular file", aIniFile);
				 osErr << "INI file %s must be a regular file";
         break;
      case errNoValue:
         //sprintf(errorStr, "   No Value for keyword %s in section %s", keyword, section);
				 osErr << "   No Value for keyword " << keyword << " in section " << section;
         break;
   }
}

// tINI class
tINISTR::tINISTR(char * aIniFile) {
   hf = 0;
   pReadBuff = 0;
   BufferRetained = 0;
   NextByte = cbBuff = 0;
   strcpy(inifile, aIniFile);
   iniok = InitLoad(aIniFile); // open file
}

tINISTR::~tINISTR() {
   TerminateLoad();
}

void tINISTR::SetSection(pchar aSection) {
   tString nextSection = "";

   /* set the section to given value */
   section = aSection;

   /* reset file pointer to begining */
   /* as SetSection cannot make      */
   /* assumptions about file pos     */

   if ( fseek(hf, SEEK_SET, 0) ) {
      /* unable to set file pos back to start */
      /* close file and open again            */

      fclose(hf);
      hf = fopen(inifile, "r");
      if ( !hf ) {
         section = tString("Fail To Open Ini File ") + tString(inifile);
         throw XINISTR (errOpenFailed, (char *)section);
      }
   }

   /* check if section name is wrapped in brackets */
   /* if not wrap it up                            */

   if ( (section[(size_t)0] != '[') && (section[section.Length()] != ']') ) {
      section = tString("[") + aSection + tString("]");
   }

   /* get to the position in file for specified section */
   section.SetCompMode(tString::cmIgnore);        // change compare mode
   nextSection.SetCompMode(tString::cmIgnore);    // change compare mode
   for ( ;; ) {
      /* get the next section from ini file */
      if ( !GetNextSection(nextSection) )
         throw XINISTRS(errNoSection, section);

      /* if next section in ini file matches the section set then break */
      if ( section == nextSection ) return;
   }
}

tString tINISTR::QueryNextSection(void) {
   tString nextSection = "";

   GetNextSection(nextSection);
   return(nextSection);
}

tString tINISTR::QueryNextKeyword(tString & aKeyword) {
   tString nextKeyword = "";
   tString nextValue = "";

   /* initialise aKeyword to nothing */
   aKeyword = "";

   /* get next keyword */
   for ( ;; ) {
      if ( GetNextKeyword(nextKeyword, nextValue) ) {
         aKeyword = nextKeyword;
         return nextValue;
      }
      else
         break;
   }
   /* not found so return empty value */
   return(nextValue);
}

void tINISTR::QueryValue(pchar aKeyword, tString & aValue, pchar aDefault, pchar aSection) {

   bool rc = QueryValueDefault(aKeyword, aValue, aSection);
   if ( !rc ) {
      if ( aDefault ) {
         aValue = aDefault;
      }
      throw XINISTRF(errNoKeyword, section, aKeyword);
   }
}


void tINISTR::QueryValue(pchar aKeyword, pchar & aValue, pchar aDefault, pchar aSection) {

   tString Value = aValue;
   bool rc = QueryValueDefault(aKeyword, Value, aSection);
   if ( !rc ) {
      if ( aDefault == 0) {
         throw XINISTRF(errNoKeyword, section, aKeyword);
      }
      else {
         aValue = strdup(aDefault);
         return;
      }
   }

   if (Value && Value.Length() > 0) {
      aValue = strdup((char *)Value);
   }
   else {
      if (Value.Length() == 0) {
         throw XINISTRF(errNoValue, section, aKeyword);
      }
   }
}

void tINISTR::QueryValue(pchar aKeyword, int & aValue, int * aDefault, pchar aSection) {

   tString Value = "";

   /* use QueryValue for a tString */
   bool rc = QueryValueDefault(aKeyword, Value, aSection);

   /* if value was found */
   if ( rc ) {
      /* convert to short */
      pchar tmp = (char * ) Value;
      aValue = atoi( tmp );
   }
   else {
      if ( aDefault ) {
         aValue = *aDefault;
      }
      else {
         throw XINISTRF(errNoKeyword, section, aKeyword);
      }
   }
}

void tINISTR::QueryValue(pchar aKeyword, long & aValue, long * aDefault, pchar aSection) {

   tString Value = "";
   /* use QueryValue for a tString */
   bool rc = QueryValueDefault(aKeyword, Value, aSection);

   /* if value was found */
   if ( rc ) {
      /* convert to short */
      aValue = atol( (const char * ) Value);
   }
   else {
      if ( aDefault ) {
         aValue = *aDefault;
      }
      else {
         throw XINISTRF(errNoKeyword, section, aKeyword);
      }
   }
}

void tINISTR::QueryValue(pchar aKeyword, double & aValue, double * aDefault, pchar aSection) {

   tString Value = "";
   /* use QueryValue for a tString */
   bool rc = QueryValueDefault(aKeyword, Value, aSection);

   /* if value was found */
   if ( rc ) {
      /* convert to short */
      aValue = atof( (const char * ) Value);
   }
   else {
      if ( aDefault ) {
         aValue = *aDefault;
      }
      else {
         throw XINISTRF(errNoKeyword, section, aKeyword);
      }
   }
}

void tINISTR::QueryValue(pchar aKeyword, bool & aValue, pchar aTrueValue, bool * aDefault, pchar aSection) {

   tString Value = "";
   tString TrueValue = aTrueValue;
   Value.SetCompMode(tString::cmIgnore);        // change compare mode
   TrueValue.SetCompMode(tString::cmIgnore);    // change compare mode

   bool rc = QueryValueDefault(aKeyword, Value, aSection);
   if (rc == 0) {
      if ( aDefault ) {
         aValue = *aDefault;
      }
      else {
         throw XINISTRF(errNoKeyword, section, aKeyword);
      }
      return;
   }

   if ( Value == TrueValue ) {
      aValue = true;
   }
   else {
      aValue = false;
   }
}

void tINISTR::GetEnv(tString &aValue)
{
  TBChar work1, work2, work3;
  work1.set(aValue);
  char *p=work1.data, *q, *r;
  while (p[0] && (q = strstr(p, "$(")) != 0)
  {
    r = strstr(q, ")");
    int n = r-q-2;
    if (r == 0 || n <= 0)
      break;
    if (p != q)
      work2.append(p, q-p);
    work3.set(q+2, n);
    char *e = getenv(work3);
    if (e)
      work2.append(e);
    p = r+1;
  }
  if (p == work1.data)
    return;
  work2.append(p);
  aValue = work2.data;
}

bool tINISTR::QueryValueDefault(tString aKeyword, tString & aValue, tString aSection) {

   tString nextSection = "";
   tString nextKeyword = "";
   tString nextValue = "";

   // if section was not specfified then default to current
   if ( !aSection.Length() ) {
      nextSection = section;
   }
   else {
      /* set to section specified */
      nextSection = aSection;
   }

   // start at the top of the section again
   SetSection(nextSection);

   aKeyword.SetCompMode(tString::cmIgnore);     // change compare mode
   nextValue.SetCompMode(tString::cmIgnore);    // change compare mode

   // get to keyword and value
   for ( ;; ) {
      if ( GetNextKeyword(nextKeyword, nextValue) ) {
         if ( aKeyword == nextKeyword ) {
            /* matched keyword */
            aValue = nextValue;
            if (aValue.Length() == 0) {
               return true;
            }
            GetEnv(aValue);
            return true;
         }
      }
      else // end of ini section keyword not matched
         break;
   }
//   aValue = aDefault;
//   GetEnv(aValue);
   return false;
}

int tINISTR::ReadData(FILE *aHf, char * aData) {
   if ( (size_t)NextByte < cbBuff ) {
      aData[0] = pReadBuff[NextByte];
      NextByte++;
      return(1);
   }

   NextByte = 0;
   cbBuff = 0;

   cbBuff = fread(pReadBuff, 1, MEM_SIZE, aHf);
   if ( !cbBuff ) {
      return(0);
   }

   aData[0] = pReadBuff[NextByte];
   NextByte++;
   return(1);

}

int tINISTR::ReadConfig(FILE *aHf, char * aData) {
   short i = 0;
   char  Jotter[500];

   if ( BufferRetained ) {
      strcpy(aData, RetainBuffer);
      BufferRetained = 0;
      return(1);
   }

   for ( ;; ) {
      if ( !ReadData(aHf, &Jotter[i]) ) break;
      if ( Jotter[i]==0x0A ) {
         if ( i<2 ) {
            i=0;
            continue;
         }

         Jotter[i]=0x00;
         strcpy(aData, Jotter);
         return(1);
      }

      i++;
//      if(i > 80) i = 0;     // M.Barnard 30 Dec 97 See Comment
      if ( i > 500 ) return(1);
   }
   return(0);
}

void tINISTR::Retain(char * aData) {
   BufferRetained = 1;
   strcpy(RetainBuffer, aData);
}

short tINISTR::InitLoad(char * aFileName) {
   struct stat buf;

   // S Sena Check that path refers to a regular text file 15/1/95
   if ( stat ( aFileName, &buf ) < 0 )
      throw XINISTRFILE(errStatFailed, aFileName);
   else
      if ( (buf.st_mode & _S_IFMT) != _S_IFREG )
      throw XINISTRFILE(errStatNotRegular, aFileName);

   hf = fopen(aFileName, "r");
   if ( !hf ) {
      throw XINISTRFILE(errOpenFailed, aFileName);
   }

   /* set our private file name to the param */
   strcpy(inifile,aFileName);

   pReadBuff = (char *) malloc(MEM_SIZE);
   if ( !pReadBuff ) return 1;
   memset(pReadBuff, 0, MEM_SIZE);
   cbBuff   = 0;
   NextByte = 0;

   return 0;
}

void tINISTR::TerminateLoad(void) {
   if ( hf ) fclose(hf);
   if ( pReadBuff ) free(pReadBuff);
}


int tINISTR::GetNextSection(tString & aSection) {
   char data[500];

   for ( ;; ) {
      if ( !ReadConfig(hf, data) ) {
         return(0);
      }

      if ( data[0] != '[' ) continue;

      /*                                        */
      /* We have found the next Section. Return */
      /* the section details.                   */
      /*                                        */

      aSection = data;
      return(1);
   }

}

int tINISTR::GetNextKeyword(tString & aKeyword, tString & aParam) {

   char  data[500];
   char  keyword[500];
   char  param[500];
   short i = 0, bound = 0;

   for ( ;; ) {
      if ( !ReadConfig(hf, data) ) return(0);

      if ( data[0] == '[' ) {

         /*                                 */
         /* We have hit the next section.   */
         /* retain the current buffer for   */
         /* the next read.                  */
         /*                                 */

         Retain(data);
         return(0);
      }

      if ( data[0] != '{' ) continue;

      /*                                        */
      /* We have found the next keyword. Return */
      /* the keyword and the parameter.         */
      /*                                        */

      bound = -1;
      for ( i=1; i< (short) strlen(data); i++ ) {

         if ( data[i] != '}' ) continue;
         bound = i;
         break;

      }

      if ( bound == -1 ) {

         /*                                    */
         /* Invalid Syntax, ignore the line    */
         /*                                    */

         continue;
      }

      /*                                       */
      /* Extract the Keyword                   */
      /*                                       */

      memcpy(keyword, data, bound+1);
      keyword[bound+1] = 0x00;

      /*                                       */
      /* Extract the Parameter                 */
      /*                                       */

      memcpy(param, &data[bound+1], strlen(data)-(bound+1));
      param[strlen(data)-(bound+1)] = 0x00;

      aKeyword = keyword;
      aParam = param;

      return(1);
   }
}

/**************************************************************************/
/*                                                                        */
/* Saving Procedures.                                                     */
/*                                                                        */
/**************************************************************************/

int tINISTR::InitSave(char * aFileName) {
   hf = fopen(aFileName, "w");
   if ( !hf ) {
      /* stuffed */
      return(0);
   }
   return(1);
}

void tINISTR::WriteKeyword(char * aKeyword, char * aValue) {
   char  newLine[5];

   newLine[0] = 0x0D;
   newLine[1] = 0x0A;
   newLine[2] = 0x00;

   fwrite( aKeyword, strlen(aKeyword), 1, hf);
   fwrite( aValue, strlen(aValue), 1, hf);
   fwrite( newLine, strlen(newLine), 1, hf);

}

void tINISTR::WriteSection(char * aSection) {
   char  newLine[5];

   newLine[0] = 0x0D;
   newLine[1] = 0x0A;
   newLine[2] = 0x00;

   fwrite(newLine, strlen(newLine), 1, hf);
   fwrite(aSection, strlen(aSection), 1, hf);
   fwrite(newLine, strlen(newLine), 1, hf);
   fwrite(newLine, strlen(newLine), 1, hf);

}

void tINISTR::TerminateSave(void) {
   fclose(hf);
}


