#ifndef _ASCIFY_H_
#define _ASCIFY_H_

#ifdef __cplusplus
#define extC extern "C"
#else
#define extC
#endif

extC uint APPLAPI In3Out4(puchar input, //--------------------------------------
   uint inlen, // Converts Binary Data to Ascii encoded
   puchar output, // every 3 bytes to 4 bytes
   uint outlen); //--------------------------------------

extC uint APPLAPI In4Out3(puchar input, //--------------------------------------
   uint inlen, // Converts Ascii encoded to Binary Data
   puchar output, // every 4 bytes to 3 bytes
   uint outlen); //--------------------------------------

extC uint APPLAPI Encode12To8(puchar input, // Expects an 12 DIGIT String
   puchar output); // Returns an 8 DIGIT String

extC uint APPLAPI Decode8To12(puchar input, // Expects an 8 DIGIT String
   puchar output); // Returns an 12 DIGIT String

#undef extC

#endif

