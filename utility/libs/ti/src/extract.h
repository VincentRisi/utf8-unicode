/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿*/
/*³                   _____TREASURY_____                      ³*/
/*³             (C) Copyright NEDCOR Bank Ltd.                ³*/
/*ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´*/
/*³                                                           ³*/
/*³          File : extract.h                                 ³*/
/*³        Author : Jonnie Gilmore                            ³*/
/*³          Date : 11 November 1994                          ³*/
/*³   Description : Reverse engineer the data stored in the   ³*/
/*³                 .FRX file by FPTABxx.VBX                  ³*/
/*³               : Included in EXTRACT.DLL used by SECADM.EXE³*/
/*³                                                           ³*/
/*ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ*/

#ifndef _EXTRACT_H_
#define _EXTRACT_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_EXTRACT_H_);

#define MAXNAMELEN               40
#define MAXCLASSLEN              40
#define MAXCAPTIONLEN            128
#define MAXPAGES                 16
#define MAXCONTROLSPERPAGE       20
#define VERSION2                 "version 2.00"
#define VERSION4                 "version 4.00"
#define VERSION5                 "version 5.00"

#define ERR_VB_VERSION_NOT_FOUND           1
#define ERR_NOT_TEXT_FORMAT                2
#define ERR_NOT_VB_PROJECT                 3
#define ERR_FILE_OPEN_ERROR                4
#define ERR_UNEXPECTED_EOF                 5
#define ERR_INVALID_CONTROL                6
#define ERR_NO_MEMORY                      7
#define ERR_INVALID_TABCAPTION             8
#define ERR_CANNOT_FIND_PROJECT            9
#define ERR_NO_FORMS                       10
#define ERR_CANNOT_FIND_FORM               11
#define ERR_CANNOT_FIND_CONTROL            12
#define ERR_CANNOT_FIND_FORM_LOAD          13
#define ERR_CANNOT_FIND_VERSION            14
#define ERR_CANNOT_FIND_DESCRIPTION        15
#define ERR_CANNOT_FIND_RUNTIMEPATH        16

#define ERR_INVALID_FRX_CONTROL            30
#define ERR_INVALID_FRX_FILE               31
#define ERR_INVALID_FRX_HEADER             32
#define ERR_INVALID_FRX_PAGE               33

#pragma pack(1)
typedef struct tagFRXCNTRL10
{
   char Name[MAXNAMELEN+1];
} FRXCNTRL10;  // 50

typedef struct tagFRXPAGE10
{
   char       Title[MAXCAPTIONLEN+1];
   short      NumberOfControls;
   FRXCNTRL10 Cntrl [ MAXCONTROLSPERPAGE ];
} FRXPAGE10;  // 50*20 + 130 + 2 = 1132

typedef struct tagFRXTAB10
{
   short     Signature;
   short     NumberOfPages;
   FRXPAGE10 Page[MAXPAGES];
} FRXTAB10;   // 1132*16 + 10 = 17K
#pragma pack()

class tFRX
{
   private:
      FRXTAB10 *TabData;
      short ProcessControl ( FILE *File, short PageNo, short CntrlNo );
      short ProcessSettings ( FILE *File, int Which );
      void print ( char *str );
      void DebugOut ( const char *Format, ... );

   public:
      tFRX();
      ~tFRX();
      short   ReadData    ( char *Filename, long Pos );
      void    DisplayData ( void );
      FRXTAB10 *pTabData    ( void );
};

#ifdef _WINDOWS
#ifdef __cplusplus
extern "C" {
#endif

short __far __pascal E4NumberOfControlsInProject ( void );
short __far __pascal E4NumberOfFormsInProject    ( void );
short __far __pascal E4GetControlString          ( short WhichControl, char *PrintString, short *pIndent );
short __far __pascal E4OpenProject               ( char *ProjectFilename );
short __far __pascal E4CloseProject              ( void );

short __far __pascal E4GetAppDetails             ( char *Path, char *tName, double *pVersion, char *Description, int *pNumberOfForms );
short __far __pascal E4GetFormDetails            ( char *tName, char *Caption, int *pNumberOfForms );
short __far __pascal E4GetControlDetails         ( char *tName, int *pIndex, char *ControlType, char *Caption, char *ParentName, int *pParentIndex );
short __far __pascal E4GetErrorMessage           ( int ErrorCode, char *szErrorMessage1, char *szErrorMessage2 );

#ifdef __cplusplus
}
#endif

#endif // _WINDOWS

#endif

