#include <stdio.h>
#include <time.h>

static int show_time = 0;

void addto(struct tm *stm, char* a)
{
    int no = atoi(a + 2);
    if (no == 0)
        return;
    switch (a[0])
    {
    case 'd':
        stm->tm_mday += no;
        break;
    case 'm':
        stm->tm_mon += no;
        break;
    case 'y':
        stm->tm_year += no;
        break;
    case 'H':
        stm->tm_hour += no;
        break;
    case 'M':
        stm->tm_min += no;
        break;
    case 'S':
        stm->tm_sec += no;
        break;
    }
}

void subtract(struct tm *stm, char* a)
{
    int no = atoi(a + 2);
    if (no == 0)
        return;
    switch (a[0])
    {
    case 'd':
        stm->tm_mday -= no;
        break;
    case 'm':
        stm->tm_mon -= no;
        break;
    case 'y':
        stm->tm_year -= no;
        break;
    case 'H':
        stm->tm_hour -= no;
        break;
    case 'M':
        stm->tm_min -= no;
        break;
    case 'S':
        stm->tm_sec -= no;
        break;
    }
}

int main(int argc, char* argv[])
{
    time_t now;
    now = time(0);
    struct tm *stm;
    stm = localtime(&now);
    int i;
    for (i = 1; i < argc; i++)
    {
        char* a = argv[i];
        switch (a[0])
        {
        case 'H':
        case 'M':
        case 'S':
            show_time = 1;
            if (a[1] == 0)
                break;
        case 'd':
        case 'm':
        case 'y':
            if (a[1] == '-')
                subtract(stm, a);
            else if (a[1] == '+')
                addto(stm, a);
            else
            {
                fprintf(stderr, "%s - the second char must be - or + for d, y, m; H, M, S cause time to be displayed\n", a);
                exit(1);
            }
            break;
        default:
            fprintf(stderr, "%s - does not start with d, m, y or H, M, S\n", a);
            exit(1);
        }
    }
    char work[256];
    mktime(stm);
    strftime(work, sizeof(work), show_time == 1 ? "%Y%m%d%H%M%S" : "%Y%m%d", stm);
    printf("%s\n", work);
    return 0;
}
