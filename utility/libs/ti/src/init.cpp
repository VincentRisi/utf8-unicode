
// init.c
// function NewHandler - throws a xAlloc exception
// function Init()

#ifndef _INIT_H_
#define _INIT_H_

#include "ti.h"

#ifdef M_AIX
#include <unexpected.h>
#include <terminate.h>
#elif defined(__MSVC__) || defined(M_GNU)
	#include <exception>
#elif defined(__BORLANDC__)
#include <except.h>
#endif

void Terminate()
{
  cerr << "Program Terminated" << endl;
}

void Unexpected()
{
  cerr << "Unexpected exception" << endl;
  Terminate();
}

#ifdef M_AIX_OLD
#include <new.h>
static void NewHandler()
{
  throw xalloc();
}
#endif

void Init()
{
#ifdef M_AIX_OLD
  set_new_handler(NewHandler);
#endif
#ifndef M_W32
  set_terminate(Terminate);
  set_unexpected(Unexpected);
#endif
}

#endif

