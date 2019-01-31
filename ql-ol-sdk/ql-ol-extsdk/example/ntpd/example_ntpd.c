#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <locale.h>
#include <time.h>
#include <sys/time.h>
#include"ql_ntp.h"

static int show_time()
{
    time_t t;

    t = time(NULL);
    printf("Current time:%s", ctime(&t));

    return 0;
}

int main(int argc, char *argv[])
{
    int ret = -1;
    int sync_times = -1;
    struct timeval set_tv;

    /*Set system time:2028/06/01 17:25:35 CST*/
    set_tv.tv_sec = 1843464335;
    set_tv.tv_usec = 0;

    /*Periodically update system time with 1 minute cycle*/
    if(RES_NTP_OK != ql_start_ntpd(1)) {
	printf("Start ntpd update system time Failed\n");
	return -1;
    }

    while(1) {
        printf("Show Current system Time:\n");
        if(0 != show_time())
        {
            printf("Show Current system Time Failed\n");
            return -1;
        }
        printf("Change system Time with: 2028/06/01 17:25:35 CST\n");
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
	printf("\n");
        sleep(120);
    }

    return 1;
}

