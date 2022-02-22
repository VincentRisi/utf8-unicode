#include "versions.h"
#define _VERSION_NO "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
COMPILED_VERSION(_VERSION_NO);

#include "machine.h"

#ifdef M_AIX
#include <unistd.h>
#include <sys/types.h>
#include <termio.h>
#include <fcntl.h>
#endif
#include <stdio.h>
#include "getch.h"

#ifdef M_AIX
static int f;
static struct termio arg;      /*  to set/restore terminal's io info */
static int Eof;                /*  holds old value of eof character */
#endif

int initch()
{
#if defined(M_OS2) || defined(M_DOS) || defined(M_W32)
  return 0;
#else
  char* termname;

  termname = ctermid(0);           /*  get the terminal ttyname */
  f = open(termname, O_RDWR);      /*  open the device */
  ioctl(f, TCGETA, &arg);
  arg.c_lflag ^= (ECHO | ICANON); /*  turns off echo and stops translation */
                                  /*  and buffering */
  Eof = arg.c_cc[4];              /*  save old value */
  arg.c_cc[4] = 1;                /*  sets no. of chars to 1 */

  return ioctl(f, TCSETA, &arg);
#endif
}

int endch()
{
#ifdef M_AIX
  arg.c_lflag ^= (ECHO | ICANON); /*  turns on echo and stops translation */
  arg.c_cc[4] = Eof;              /*  restore old value */
  ioctl(f, TCSETA, &arg);         /*  restore terminal */
  close(f);
#endif

  return 0;
}

int inchar()
{
#if defined(M_OS2) || defined(M_DOS) || defined(M_W32)
  return getch();
#else
  char buf[1];

  read(f, buf, 1);
  return *buf;
#endif
}

int outchar(int c)
{
#if defined(M_OS2) || defined(M_DOS) || defined(M_W32)
  return putch(c);
#else
  char buf[1];

  *buf = (char)c;
  return write(f, buf, 1);
#endif
}

/*  -------------------------------------------------------------- */
#ifdef GETCH_TST

int main(void)
{
  int c;
  int i;

  initch();
  for (i = 0; i < 10; i++)
  {
    c = inchar();
    outchar(c);
    printf("\n%d> character %c(%d) pressed\n", i, c, c);
  }
  endch();
  return 0;
}
//
//  use ifstream x ("/dev/tty")
//
//char GetResponse ( ifstream & x, char *prompt, char *opts )
//{
//     char c;
//     bool First = true;
//     struct  sgttyb B;
//
//     if (ioctl(x.rdbuf()->fd(), TIOCGETP, &B ) < 0 )
//         perror ("GetResponse : ioctl");
//
//     B.sg_flags |= CBREAK;
//
//     if (ioctl(x.rdbuf()->fd(), TIOCSETP, &B ) < 0 )
//        perror("GetResponse : ioctl");
//
//     while (strchr(opts,c) == NULL)
//     {
//        if (!First)
//          cerr << "Invalid Response" << endl;
//        else
//           First=false;
//
//        cerr << prompt << " ";
//        x >> c;
//        cerr << endl;
//     }
//
//     B.sg_flags ^= CBREAK;
//
//     if (ioctl(x.rdbuf()->fd(), TIOCSETP, &B ) < 0 )
//        perror("GetResponse : ioctl");
//
//    return  c;
//}
//
#endif
