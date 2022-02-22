#include "versions.h"
#define _VERSION_NO "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
COMPILED_VERSION(_VERSION_NO);

#include <stdio.h>

int main(int argc, char **argv)
{
    int     i;

    for (i=1;i<(argc-1);i++) {
        printf("+%s &\n",argv[i]);
        }
    printf("+%s \n",argv[i]);

    return 0;
}
