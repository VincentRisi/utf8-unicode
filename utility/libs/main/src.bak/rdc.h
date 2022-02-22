#ifndef _RDC_H_
#define _RDC_H_

#include <stddef.h>

size_t RdcCompress(unsigned char *aInBuff, size_t aInBuffLen, unsigned char *aOutBuff);

size_t RdcDecompress(unsigned char * aInBuff, size_t aInbuffLen, unsigned char *aOutBuff);

#endif
