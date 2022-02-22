// Filename      : txssem.h
// Description   :
//     Semaphore with two locking states exclusive or sharemode
//     a process executing an exclusive locks out all other users
//     a process sharemode lock only temoraryilg locks out writers
//     many sharemode locsk may exist
// Subsystem     :  security
// Author        :  S M Sena  $AUTHOR$
// Date Created  :  1 Nov 1994
// $LOG$

#ifndef _TSEM2_H_
#define _TSEM2_H_ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(_TSEM2_H_);

#ifdef  DEBUG_ON
  #define debug(X)  X
#else
  #define debug(X)
#endif

class tSem2 {

     public :

	enum {Create=1};
	tSem2( long aKey,  int aMode = 0 );   // ctor
	~tSem2();
        void AquireEXCL();	          // ask for an (writer) exclusive lock
	void AquireSHR();                 // ask for a (reader) share lock
	void Release();                   // Release lock
  void SetTerseOutput() { m_stream_mode = 0 ;}
  void SetVerboseOutput() { m_stream_mode = 1 ;}

	friend  std::ostream & operator << (  std::ostream & os, tSem2 & sem );

     private :

	static sembuf QR[2],
		      AX[4],   // Aquire exclusive
		      RX[1],   // Release Exclusive
	     	      AS[3],   // Aquire share
		      RS[1];   // Release share

	enum  lockstate { None, Exclusive, Share };

        lockstate m_state;  // indicates the locks that this object has
	bool      m_owner;
	key_t     m_semid;
	int       m_stream_mode;

};



class xSem2:public xCept
{
private:
  int unixErrorNo;                      // error number from system call
public:
  xSem2(char *aFile, const int aLine, int aErrno, char *aInfo = 0);
};

#define XSem2(errno,mtext) xSem2(__FILE__,__LINE__,errno,mtext)

#endif
