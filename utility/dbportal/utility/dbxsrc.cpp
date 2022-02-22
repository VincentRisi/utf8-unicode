#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <sys/stat.h>
#include "addlist.h"

#define SIZE 64*1024
#define START_DEFAULT   "/main/nedcor/bbd/common"
#define OUTFILE_DEFAULT "filelist.txt"

TAddList<char*, int> sources;
TAddList<char*, int> fullResult;

int compare(char **A, char **B)
{
  return strcmp(*A, *B);
}

enum { NORMAL_STATE, START_STATE, OUTFILE_STATE, LISTFILE_STATE };

static char buffer[SIZE];
void process_file(FILE* filein, const char* start, bool checkExists)
{
  char fpath[512];
  size_t pos = 0;
  memset(fpath, 0, sizeof(fpath));
  while (true)
  {
    size_t size;
    size = fread(buffer, 1, SIZE, filein);
    if (size == 0) break;
    for (size_t i = 0; i < size; i++)
    {
      if ((buffer[i] <= ' ' || buffer[i] > 127 || strchr(":{}[]()%#*?", buffer[i]))
        || (pos < strlen(start) && pos < 255 && buffer[i] != start[pos]))
      {
        if (pos > strlen(start))
        {
          char* path = strdup(fpath);
          if (fullResult.search(&path) == -1)
            sources.add(path);
          else
            free(path);
        }
        memset(fpath, 0, sizeof(fpath));
        pos = 0;
        continue;
      }
      fpath[pos++] = buffer[i];
    }
  }
  sources.sort();
  memset(fpath, 0, sizeof(fpath));
#ifdef _MSC_VER
#  define S_ISDIR(type) ((type & _S_IFDIR) == _S_IFDIR)
#  define S_ISREG(type) ((type & _S_IFREG) == _S_IFREG) 
#endif
  for (int i = 0; i < sources.getCount(); i++)
  {
    if (strcmp(sources[i], fpath) != 0)
    {
      //strncpy(fpath, sources[i], sizeof(fpath) - 1);
      memcpy(fpath, sources[i], sizeof(fpath) - 1);
      struct stat path_stat;
      if (checkExists == true)
      {
        if (stat(fpath, &path_stat) != 0)
          continue;
        if (S_ISDIR(path_stat.st_mode) == false
          && (S_ISREG(path_stat.st_mode) == false))
          continue;
      }
      char* p = strrchr(fpath, '.');
      if (p == 0)
        continue;
      char* path = strdup(fpath);
      fullResult.add(path);
    }
    free(sources[i]);
  }
  fullResult.sort();
  sources.clear();
}

int main(int argc, char* argv[])
{
  sources.compare = compare;
  fullResult.compare = compare;
  const char* start = START_DEFAULT;
  const char* outfile = OUTFILE_DEFAULT;
  const char* listfile = "";
  int state = NORMAL_STATE;
  FILE* fileout = 0;
  FILE* listin = 0;
  bool done = false;
  bool checkExists = true;
  for (int argno = 1; argno < argc; argno++)
  {
    char* arg = argv[argno];
    if (strncmp(arg, "-c", 2) == 0)
    {
      if (strlen(arg) > 2)
        checkExists = arg[2] == '+' ? true : false;
      else
        checkExists = !checkExists;
      continue;
    }
    if (strncmp(arg, "-s", 2) == 0)
    {
      if (strlen(arg) > 2)
      {
        start = arg + 2;
        continue;
      }
      state = START_STATE;
      continue;
    }
    if (strncmp(arg, "-o", 2) == 0)
    {
      if (strlen(arg) > 2)
      {
        outfile = arg + 2;
        continue;
      }
      state = OUTFILE_STATE;
      continue;
    }
    if (strncmp(arg, "-f", 2) == 0)
    {
      if (strlen(arg) > 2)
      {
        listfile = arg + 2;
        continue;
      }
      state = LISTFILE_STATE;
      continue;
    }
    if (state == START_STATE)
    {
      start = arg;
      state = NORMAL_STATE;
      continue;
    }
    if (state == OUTFILE_STATE)
    {
      outfile = arg;
      state = NORMAL_STATE;
      continue;
    }
    if (state == LISTFILE_STATE)
    {
      listfile = arg;
      state = NORMAL_STATE;
      continue;
    }
    FILE* filein = fopen(arg, "rb");
    if (filein == 0)
    {
      fprintf(stderr, "%s file not opened - ignoring it.\n", arg);
      continue;
    }
    done = true;
    process_file(filein, start, checkExists);
    fclose(filein);
  }
  if (strlen(listfile) > 0)
  {
    listin = fopen(listfile, "rt");
    if (listin == 0)
    {
      fprintf(stderr, "%s file not opened - listfile ignored.\n", listfile);
    }
    char infile_name[1024];
    while (true)
    {
      if (listin == 0 || fgets(infile_name, sizeof(infile_name), listin) == 0)
        break;
      int n = strlen(infile_name);
      while (n > 0 && infile_name[n - 1] <= ' ')
        infile_name[--n] = 0;
      FILE* filein = fopen(infile_name, "rb");
      if (filein == 0)
      {
        fprintf(stderr, "%s file not opened - ignoring it.\n", infile_name);
        continue;
      }
      done = true;
      process_file(filein, start, checkExists);
      fclose(filein);
    }
  }
  if (done == true)
  {
    if (fileout == 0)
    {
      fileout = fopen(outfile, "wt");
      if (fileout == 0)
      {
        fprintf(stderr, "%s file not opened - using stdout.\n", outfile);
        fileout = stdout;
      }
    }
    for (int i = 0; i < fullResult.getCount(); i++)
    {
      fprintf(fileout, "%s\n", fullResult[i]);
      fflush(fileout);
      free(fullResult[i]);
    }
    fclose(fileout);
    fullResult.clear();
    return 0;
  }
  printf("usage:-\n"
    "%s [-c[+]] [-o outfile '%s'] [-s start '%s'] [-f listfile] file ...\n"
    "  -c toggle switch file existence check (default state on)\n"
    "  -c+ switch on file existence check\n",
    argv[0], OUTFILE_DEFAULT, START_DEFAULT
    );
  return 1;
}
