#ifndef __DDS_H__
#define __DDS_H__ "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(__DDS_H__);

#include "ddcom.h"

const int BLOCKSIZE  =512;

class dds {

   private :

        void SendSync( tConnect & aConnect );
        void SendQueryCount( tConnect & aConnect );
        void SendTableRevisions(tConnect & aConnect );
        void SendDataBaseCreateScript(tConnect & aConnect );
        void GenerateSqlStmts( ofstream & ofs, tConnect & aConnect,
			      tTableRevisions & aTable);
        void HostToClientOrder( pSqlQuery SqlQuery, char * pData );
        int GenerateInsert (pSqlQuery SqlQuery , char * pData,
                             char * aTableName, char * aStmtBuf, int BufSize);

        struct MessageHdr ReplyHdr,      // dds message
                          CallHdr;

        tSockServer  *SockSrv;
	char         m_pid[9];

   public :
        dds( char * aInifile , int aSockDescriptor );

};
#endif
