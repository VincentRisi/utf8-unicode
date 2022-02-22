#include <vcl.h>
#pragma hdrstop

#include "TestAmpUnit.h"

#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;

__fastcall TForm1::TForm1(TComponent* Owner)
  : TForm(Owner)
{
}

#include "tbuffer.h"

void __fastcall TForm1::Button1Click(TObject *Sender)
{
  TBAmp fred;
  fred.ampappend("abcdefghij\xf0");
  Memo1->Lines->Add(fred.data);
}

