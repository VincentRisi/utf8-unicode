#include "machine.h"
#include <windows.h>
#include <stdlib.h>
#include <memory.h>
#include "sqlhnd.h"

tSqlHandle::tSqlHandle(short DataSize,short MaxHandles,long aHandleBase)
{
    int     i;

    ElemSize=DataSize;
    ArraySize=MaxHandles;
    ArraySize = 10;
    HandleBase=aHandleBase;

    Data=(void *) new char[ElemSize*ArraySize];
    Entry=new tEntry[ArraySize];

    for (i=0;i<ArraySize;i++)
    {
        Entry[i].Used=0;
        Entry[i].DataPtr=(void *) (((char *) Data) + (i*ElemSize));
        Entry[i].Caller=NULL;
    }
}

tSqlHandle::~tSqlHandle()
{
    delete [] Data;
    delete [] Entry;
}

HSQL tSqlHandle::Create(void *Element)
{
    HSQL    Hnd;
    if ((Hnd=FindFirstFree())==-1)
        return NULL;

    memcpy(Entry[Hnd].DataPtr,Element,ElemSize);
    Entry[Hnd].Used=1;
    Entry[Hnd].Caller=GetCurrentTask();

    return Hnd + HandleBase;
}

void *tSqlHandle::Use(HSQL Handle)
{
    char    MsgBuff[255];

    Handle-=HandleBase;

    if ((Handle<0) || (Handle>=ArraySize)) {
        wsprintf(MsgBuff,"Use: Invalid Handle, normalized = %ld",Handle);
        MessageBox(0,MsgBuff,"SQLODBC.DLL",MB_ICONSTOP);
        return NULL;
        }

    if (Entry[Handle].Used==0) {
        wsprintf(MsgBuff,"Use: Handle is not currently in use : %ld",Handle);
        MessageBox(0,MsgBuff,"SQLODBC.DLL",MB_ICONSTOP);
        return NULL;
        }

    if (Entry[Handle].Caller!=GetCurrentTask()) {
        wsprintf(MsgBuff,"Use: Caller not handle owner. \nHandle=%ld, Owner=%04x, You=%04x",
           Handle, Entry[Handle].Caller, GetCurrentTask());
        MessageBox(0,MsgBuff,"SQLODBC.DLL",MB_ICONSTOP);
        return NULL;
        }

    return Entry[Handle].DataPtr;
}

void tSqlHandle::Release(HSQL Handle)
{
    char    MsgBuff[255];

    Handle-=HandleBase;

    if ((Handle<0) || (Handle>=ArraySize)) {
        wsprintf(MsgBuff,"Release: Invalid Handle, normalized = %ld",Handle);
        MessageBox(0,MsgBuff,"SQLODBC.DLL",MB_ICONSTOP);
        return;
        }

    if (Entry[Handle].Used==0) {
        wsprintf(MsgBuff,"Release: Handle is not currently in use : %ld",Handle);
        MessageBox(0,MsgBuff,"SQLODBC.DLL",MB_ICONSTOP);
        return;
        }

    if (Entry[Handle].Caller!=GetCurrentTask()) {
        wsprintf(MsgBuff,"Release: Caller not handle owner. \nHandle=%ld, Owner=%04x, You=%04x",
           Handle, Entry[Handle].Caller, GetCurrentTask());
        MessageBox(0,MsgBuff,"SQLODBC.DLL",MB_ICONSTOP);
        return;
        }

    Entry[Handle].Used=0;
    Entry[Handle].Caller=NULL;
}

HSQL tSqlHandle::FindFirstFree(void)
{
    HSQL        i;

    for (i=0;(i<ArraySize) && (Entry[i].Used);i++);

    if (Entry[i].Used)
        return -1;
    else
        return i;
}

int tSqlHandle::NoInUse()
{
   int      Num=0;

   for (int i=0;i<ArraySize;i++)
      if (Entry[i].Used)
         Num++;

   return Num;
}


