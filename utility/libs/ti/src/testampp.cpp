//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("testAmpP.res");
USEFORM("TestAmpUnit.cpp", Form1);
USELIB("..\lib\Bcb5\libti.lib");
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
  try
  {
     Application->Initialize();
     Application->CreateForm(__classid(TForm1), &Form1);
     Application->Run();
  }
  catch (Exception &exception)
  {
     Application->ShowException(&exception);
  }
  return 0;
}
//---------------------------------------------------------------------------
