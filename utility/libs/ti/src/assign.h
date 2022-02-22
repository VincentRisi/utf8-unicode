#ifndef _ASSIGN_H_
#define _ASSIGN_H_

void Assign(char* aTarget, char* aSource, size_t aSize);
void Assign(char* aTarget, uint aSource, size_t aSize);
void Assign(char* aTarget, int aSource, size_t aSize);
ushort Number(char *aSource, size_t aSize);

XCLASSDEF(Assign);
class xAssign : public xCept
{
  int One, Two;
public:
  enum eError { errAssignInit = 1
              , errAssignRead
              , errAssignWrite
              , errAssignAssign
              , errAssignNo
              , errAssignSize
              };
  xAssign(const char * aFname, int aLine, eError aError, int aOne=0, int aTwo=0);
  xAssign(const xAssign& aX);
  int Primary()   { return One; }
  int Secondary() { return Two; }
};


#define XAssign2(err, a, b) xAssign(__FILE__, __LINE__, xAssign::err, (a), (b))

#endif
