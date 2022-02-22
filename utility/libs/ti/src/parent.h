#ifndef _PARENT_H_
#define _PARENT_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_PARENT_H_);

// Filename      : parent.h
// Description   : AIX Multiple socket server parent spawn class
// Author        : Suri Sena, modified Vince Risi

#include "ti.h"
#include "tstring.h"
#include "vsocket.h"

XCLASSDEF(Parent);
CLASSDEF(Parent);

class xParent : public xCept
{
public:
  enum eError { errParentExec = 1
              , errParentFork
              };
  xParent(char * aFname, int aLine, eError aError);
  xParent(const xParent& aX);
};

#define XParent(err) xParent(__FILE__, __LINE__, xParent::err)

class tParent
{
  tString Service;                  // Service name found in /etc/services
  tString Server;                   // Name of server i.e. executable
  int     NoProcs;                  // No. of times to f**k()
  pSockServer SockServer;
public:
  tParent(char * aIniFile);
  tParent(tString aServer,
          tString aService,
          int aNProcs=1);
  void Run(char * aIniFile);
};

#endif

#if 0
// if inifile is used then use

[Socket Server]
===============
{Server}vtrans
{Service}ntapsoc1
{NoProcs}1

#endif

