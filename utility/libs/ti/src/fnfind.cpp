
#include "machine.h"

#if defined(M_OS2) || defined(M_DOS) || defined(M_W32)
#include <io.h>
#endif

#if defined(M_AIX)
#include <unistd.h>
#include <sys/access.h>
#endif

#include <string.h>
#include <stdlib.h>
#include "xdir.h"

char *FNameFind(char *File, char *Env)
{
  static char Path[DIRMAX_PATH];
  char Dir[DIRMAX_DIR];
  char Name[DIRMAX_NAME];
  char Ext[DIRMAX_EXT];
  char *EnvData, *p1, *p2;
  int size;

  // if File exists as is, return it
  if (access(File, 0) == 0)
  {
    strcpy(Path, File);
    return Path;
  }

  FNameSplit(File, Dir, Name, Ext);
  if (strlen(Dir))
  {
    strcpy(Path,"");
    return Path;
  }

  EnvData = getenv(Env);
  p1 = EnvData;
  while (p1)
  {
    p2 = strchr(p1, PATH_DELIM);
    if (p2)
    {
      size = p2 - p1;
      strncpy(Dir, p1, size);
      Dir[size]=0;
      p2++;
    }
    else
      strcpy(Dir, p1);
    p1 = p2;
    FNameMerge(Path, Dir, Name, Ext);

    // if file exists on path for environment variable Env, return it
    if (access(Path, 0) == 0)
      return Path;
  }
  strcpy(Path, "");
  return Path;
}



