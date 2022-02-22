#ifndef _WINTOOLS_H_
#define _WINTOOLS_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_WINTOOLS_H_);

#ifdef __cplusplus
extern "C" {
#endif

#define RGB_LWHITE    RGB(255,255,255)
#define RGB_DARKGREY  RGB(128,128,128)
#define RGB_LIGHTGREY RGB(192,192,192)

#define SPLURB DebugOut("   FILE=%s LINE=%d",__FILE__,__LINE__);

BOOL RegisterPopUpClass ( HINSTANCE hInst );
void __cdecl PopUp ( const char _far *Format, ... );
void __cdecl DebugOut ( const char _far *Format, ... );
void __cdecl DebugLog ( const char *Format, ... );

// Debug macros

#define DUMP_PATH "c:\\ti\\"
#define PROF_RCV  "c:\\ti\\profile.rcv"
#define DUMP_FILE DUMP_PATH "dump.log"
#define PROF_FILE DUMP_PATH "profile.rcv"
#define PROF_SRV  DUMP_PATH "prof_srv.log"
#define PROF_SHM  DUMP_PATH "prof_shm.log"
#define PROF_DSK  DUMP_PATH "prof_dsk.log"

#define DUMP(s)                                            \
{                                                          \
   FILE* f = fopen(DUMP_FILE, "a");                        \
   if (f){                                                 \
     fprintf(f, "<%s>\n", s);                              \
     fclose(f);                                            \
   }                                                       \
}

#define DUMPCHAR(c)                                        \
{                                                          \
   FILE* f = fopen(DUMP_FILE, "a");                        \
   if (f){                                                 \
     fprintf(f, "%c", c);                                  \
     if (c==0) fprintf(f, "%c", '\n');                     \
     fclose(f);                                            \
   }                                                       \
}

#define DUMPPTR(p)                                         \
{                                                          \
   FILE* f = fopen(DUMP_FILE, "a");                        \
   if (f){                                                 \
     fprintf(f, "<%p>\n", p);                              \
     fclose(f);                                            \
   }                                                       \
}

#define DUMPDBL(val)                                       \
{                                                          \
   FILE* f = fopen(DUMP_FILE, "a");                        \
   if (f){                                                 \
     char buf[64];                                         \
     fprintf(f, "<%s>\n", _gcvt(val, 6, buf));             \
     fclose(f);                                            \
   }                                                       \
}

#define SHOWLINE                                           \
{                                                          \
   FILE* f = fopen(DUMP_FILE, "a");                        \
   if (f){                                                 \
     fprintf(f, "%8.8s %4d\n", __FILE__, __LINE__);        \
     fclose(f);                                            \
   }                                                       \
}

WORD NumberOfChildWindows ( HINSTANCE hInst, HWND hWnd );
BOOL SubClass ( HINSTANCE hInst, HWND hWnd, FARPROC _far *pNewWndProc, FARPROC _far *pOldWndProc, FARPROC SubClassWndProc );
char _far *_GetStr ( char _far * _far *pPtr );
int _GetInt ( char _far *_far *pPtr );
void MakeBotRightRelativeToTopLeft ( RECT _far *pRect );
void PaintWindowBox     ( HDC hDC, RECT Rect );
void PaintWindowDent    ( HDC hDC, RECT Rect, short Width, COLORREF TopLeftColour, COLORREF BottomRightColour );
void PaintWindowIndent  ( HDC hDC, RECT Rect );
void PaintWindowOutdent ( HDC hDC, RECT Rect );
void PaintWindowGroove  ( HDC hDC, RECT Rect, int Height, int Width, COLORREF DarkColour, COLORREF LightColour );
void PaintWindowBitmap  ( HINSTANCE hInst, HWND hWnd, HDC hDC, char _far *ResourceName, BOOL Stretch );

// Dates have the format CCYYMMDD
int DateDifferenceInDays ( char _far *OldDate, char _far *NewDate );
char _far *CurrentDate ( char _far *Date );

void CentreWindowOnScreen ( HWND hWnd );

#ifdef __cplusplus
}
#endif

#endif

