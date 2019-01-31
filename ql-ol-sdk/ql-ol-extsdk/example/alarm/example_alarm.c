/*
*  alarm is a kind of simple timer, but the minimum time unit is one second.
*/
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void callback_onAlarm(int param);

int main(int argc,char *argv[])
{
	signal(SIGALRM, callback_onAlarm);
	printf("<start alarm>\n");
	alarm(2);

    while (1)
	{
	}
 
    return 0;
}

void callback_onAlarm(int param)
{
	static int ringCnt = 0;
	if ((++ringCnt) == 10)
	{
		alarm(0);
		printf("<stop looping alarm>\n");
		return;
	}
	printf("<alarm is ringning. start again>\n");
	alarm(2);
}
