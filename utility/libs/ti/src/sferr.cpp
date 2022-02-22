#include "versions.h"
#define _VERSION_NO "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
COMPILED_VERSION(_VERSION_NO);

// FileName: sferr.c
// platform: aix + windows
// Description: function for errors from store and forward service
// SubSystem: infrastructure
// Author: P. Searle
// Date Created: September 1994

#include "sferr.h"

char *sfErrors[] =
{
  "SF: Ok"
, "SF: Service stopped"
, "SF: sfOverUtilized??"
, "SF: Unhandled cplusplus exception"
, "SF: Queue of item failed"
, "SF: Memory allocation problem"
, "SF: sfItsToLong??"
, "SF: No matching item found"
, "SF: Path for key file is invalid"
, "SF: Error on open of key file"
, "SF: Handle is not valid - connect not done"
, "SF: To many applications already connected"
, "SF: sfStatToSmall?"
, "SF: Item is in TX queue waiting to be transmitted"
, "SF: Error on open of overflow data file"
, "SF: Overflow data file already exists"
, "SF: Overflow data file has been removed"
, "SF: Specified Queue ID is not valid"
, "SF: Queue is in an invalid state - needs to be cleared"
, "SF: Unable to update queue item"
, "SF: Lock failed - Queue in use - repeat action"
, "SF: Item transmitted to host S&F OK"
, "SF: Transmission to host FAILED for item"
, "SF: No match on item in queue for specified item"
, "SF: sfOrderOut???"
, "SF: Overflow data file is corrupt"
, "SF: Construction of persistent queue class failed"
, "SF: sfTaskOversubscribed??"
, "SF: sfAppOverflow??"
, "SF: sfHostOverflow??"
, "SF: sfServiceOverflow??"
, "SF: Length of data in buffer not consistent with message length of CS"
, "SF: Delete of message in queue failed"
, "SF: Error unknown"
};

char * GetSFError(int errNo)
{
  if (errNo < sfOK || errNo > sfLastErr)
      return sfErrors[sfLastErr];
  return sfErrors[errNo];
}

