#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <locale.h>
#include <time.h>
#include <sys/time.h>
#include"ql_time.h"

#define SECONDS_IN_TROPICAL_YEAR (365.24219 * 24 * 60 * 60)

static int show_time()
{
    time_t t;
    struct tm *gmp, *locp;
    struct tm gm, loc;
    struct timeval tv;

    t = time(NULL);
    printf("Seconds since the Epoch (1 Jan 1970): %d", (long) t);
    printf(" (about %6.3f years)\n", t / SECONDS_IN_TROPICAL_YEAR);

    if (gettimeofday(&tv, NULL) == -1)
        return -1;
    printf("  gettimeofday() returned %ld secs, %ld microsecs\n",
            (long) tv.tv_sec, (long) tv.tv_usec);

    gmp = gmtime(&t);
    if (gmp == NULL)
        return -1;

    gm = *gmp;          /* Save local copy, since *gmp may be modified
                           by asctime() or gmtime() */
    printf("Broken down by gmtime():\n");
    printf("  year=%d mon=%d mday=%d hour=%d min=%d sec=%d ", gm.tm_year,
            gm.tm_mon, gm.tm_mday, gm.tm_hour, gm.tm_min, gm.tm_sec);
    printf("wday=%d yday=%d isdst=%d\n", gm.tm_wday, gm.tm_yday, gm.tm_isdst);

    locp = localtime(&t);
    if (locp == NULL)
        return -1;

    loc = *locp;        /* Save local copy */

    printf("Broken down by localtime():\n");
    printf("  year=%d mon=%d mday=%d hour=%d min=%d sec=%d ",
            loc.tm_year, loc.tm_mon, loc.tm_mday,
            loc.tm_hour, loc.tm_min, loc.tm_sec);
    printf("wday=%d yday=%d isdst=%d\n",
            loc.tm_wday, loc.tm_yday, loc.tm_isdst);

    printf("asctime() formats the gmtime() value as: %s", asctime(&gm));
    printf("ctime() formats the time() value as:     %s", ctime(&t));

    printf("mktime() of gmtime() value:    %ld secs\n", (long) mktime(&gm));
    printf("mktime() of localtime() value: %ld secs\n", (long) mktime(&loc));

    return 0;

}

int main(int argc, char *argv[])
{
    struct timeval set_tv;

    /*Set system time:2028/06/01 17:25:35 CST*/
    set_tv.tv_sec = 1843464335;
    set_tv.tv_usec = 0;

    printf("Show Current system Time:\n");
    if(0 != show_time())
    {
        printf("Show Current system Time Failed\n");
        return -1;
    }
    
    printf("\n\nChange system Time with: 2028/06/01 17:25:35 CST\n");
    if(0 != ql_settimeofday(&set_tv))
    {
        printf("Changed system Time Failed\n");
    }
    
    printf("Show Changed system Time:\n");
    if(0 != show_time())
    {
        printf("Show Changed system Time Failed\n");
        return -1;
    }

    return 1;
}

