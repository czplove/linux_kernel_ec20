#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>

#define CLOCKID (0)	/*CLOCK_REALTIME, a postix timer*/

void timer_thread(union sigval v)
{
	struct timespec time = {0,0};

	clock_gettime(CLOCK_REALTIME, &time);
	printf("tick=%d.%d\n", (int)time.tv_sec, (int)time.tv_nsec);
	printf("timer_thread function! %d\n", v.sival_int);
}


int main(int argc, char *argv[])
{
	timer_t timer_id;
	struct sigevent evp;
	int ret;
	
	memset(&evp, 0, sizeof(struct sigevent));
	evp.sigev_value.sival_int = 111;		// it can be passed into callback
	evp.sigev_notify = SIGEV_THREAD;		// The thread notifying way -- dispatch a new thread.
	evp.sigev_notify_function = timer_thread;	// The thread address

	if(timer_create(CLOCKID, &evp, &timer_id) == -1)
	{
		perror("fail to timer_create");
		exit(-1);
	}
	
	struct itimerspec it;
	it.it_interval.tv_sec = 5;	//periodic or not, assign it_interval to it_value once the it_value time expiry
	it.it_interval.tv_nsec = 0;
	it.it_value.tv_sec = 5;
	it.it_value.tv_nsec = 0;
	
	if(timer_settime(timer_id, 0, &it, NULL) == -1)
	{
		perror("fail to timer_settime");
		goto delete_posix_timer;
	}

	pause();

	return 0;

delete_posix_timer:
	if(timer_delete(timer_id) == -1)	
	{
		if(errno == EINVAL)
		{
			printf("this posix timer is invalid\n");
		}
		return -1;
	}
	printf("this posix timer has been deleted\n");
	return 0;
}
