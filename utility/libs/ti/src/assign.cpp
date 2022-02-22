#include "ti.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "assign.h"

#ifdef M_STDLIB
using namespace std;
#endif

void Assign(char* aTarget, char* aSource, size_t aSize)
{
  // Store exactly aSize characters from aSource into aTarget
  // not null terminated. If the input size of aSource is
  // less than size then space pad the result.
  size_t n = strlen(aSource);
  if (n > aSize)
    throw(XAssign2(errAssignAssign, n, aSize));
  size_t Rest = aSize - n;
  memcpy(aTarget, aSource, n);
  if (Rest)
    memset(aTarget+n, ' ', Rest);
}

void Assign(char* aTarget, uint aSource, size_t aSize)
{
  // Store aSource into aTarget for exactly the length aSize
  // not null terminated, ensuring the value is left padded with
  // zeroes.
  char Work[13];
  size_t n = sprintf(Work, "%0*ld", aSize, aSource);
  if (n > aSize)
    throw(XAssign2(errAssignAssign, n, aSize));
  memcpy(aTarget, Work, aSize);
}

void Assign(char* aTarget, int aSource, size_t aSize)
{
  // Store aSource into aTarget for exactly the length aSize
  // not null terminated, ensuring the value is left padded with
  // zeroes.
  char Work[13];
  size_t n = sprintf(Work, "%0*d", aSize, aSource);
  if (n > aSize)
    throw(XAssign2(errAssignAssign, n, aSize));
  memcpy(aTarget, Work, aSize);
}

ushort Number(char *aSource, size_t aSize)
{
  char Work[13];
  if (aSize > sizeof(Work)-1)
    throw(XAssign2(errAssignNo, sizeof(Work)-1, aSize));
  strncpy(Work, aSource, aSize);
  Work[aSize] = 0;
  return atoi(Work);
}

#define xAssignName "xAssign"

xAssign::xAssign(const xAssign& aX)
: xCept(aX)
{}

xAssign::xAssign(const char *aFname, int aLine, eError aError, int aOne, int aTwo)
: xCept(aFname, aLine, xAssignName, aError)
, One(aOne)
, Two(aTwo)
{
  switch (aError)
  {
     case errAssignAssign: 
    osErr << "Assign assign error actual:"
          << aOne << " expecting:" << aTwo << ends;
        break;
     case errAssignNo:
    osErr << "Assign to number error max length allowed:"
          << aOne << " actual supplied:" << aTwo << ends;
        break;
  }
};

 
