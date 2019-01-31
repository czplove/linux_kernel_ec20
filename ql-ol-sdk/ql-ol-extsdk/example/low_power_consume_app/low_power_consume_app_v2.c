#include <ql_oe.h>
#include "ql_qcmap_client_api.h"

#define GPIO_NOTIFY "/dev/wakeup_out"
#define IRQ_EDGE "/proc/irq_edge"

static volatile boolean deamon_flag = TRUE;
static boolean autoconnect = TRUE;
static int wakelock_fd;
static int wakelock_flag = 0;
static int notify_fd;
static int sockfd = 0;

/*********************** SIGTERM signal handler *************************/
static void ql_sighandler(int signal)
{
    switch(signal)
    {
    case SIGTERM:
    case SIGINT:
        if(wakelock_flag == 1)
        {
            Ql_SLP_WakeLock_Unlock(wakelock_fd);
            Ql_SLP_WakeLock_Destroy(wakelock_fd);
        }
        exit(1);
        break;

    default:
        printf("Receiver unexpected signal %s\n",signal);
        break;
    }
}

/************************** gpio irq *****************************/
/* gpio cb handler */
void gpio_cb_handler(int signal)
{
	char edge[8] = {0};
	int edge_fd;

	printf("gpio irq: Have caught signal %d from driver\n", signal);

	if(wakelock_flag != 1)
	{
            if (Ql_SLP_WakeLock_Lock(wakelock_fd) != 0)
       	    {
            	Ql_SLP_WakeLock_Destroy(wakelock_fd);
                printf("Lock wakelock failed! \n");
                return;
            }
            wakelock_flag = 1;
	}

	edge_fd = open(IRQ_EDGE, O_RDONLY | O_NONBLOCK);
	if(edge_fd < 0)
	{
		perror("open");
		return;
	}
	

	if(read(edge_fd, edge, sizeof(edge)) < 0)
	{
		perror("read");
		return;
	}
	printf("edge: %s\n", edge);

	if(strncmp(edge, "falling", 7) == 0)
	{
		printf("do something before enter sleep mode\n");
		printf("module will enter sleep mode\n");
		if(wakelock_flag == 1)
    		{
    			wakelock_flag = 0;
	   		if (Ql_SLP_WakeLock_Unlock(wakelock_fd) != 0)
	   		{
	   			Ql_SLP_WakeLock_Destroy(wakelock_fd);
	   			close(edge_fd);
	   			printf("Unlock wakelock failed!\n");
	   			return;
	   		}
		}
		
	}
	else if(strncmp(edge, "rising", 6) == 0)
	{
		if(wakelock_flag != 1)
	    	{
			if (Ql_SLP_WakeLock_Lock(wakelock_fd) != 0)
			{
				Ql_SLP_WakeLock_Destroy(wakelock_fd);
				close(edge_fd);
				printf("Lock wakelock failed!\n");
				return;
			}
			wakelock_flag = 1;
		}
		ioctl(notify_fd, 1);	//pull up notify mcu
		printf("pull up notify mcu\n");
	}
	close(edge_fd);
}  


/* register gpio cb handler */
int register_gpio_cb()
{
	int ret, Oflags;

	signal(SIGIO, gpio_cb_handler);
	notify_fd = open(GPIO_NOTIFY, O_RDWR);
	if (notify_fd < 0)
	{
		perror("open");
		goto err;
	}
	ret = fcntl(notify_fd, F_SETOWN, getpid());
	if(ret < 0)
	{
		perror("fcntl");
		goto err;
	}

	Oflags = fcntl(notify_fd, F_GETFL);
	if(Oflags < 0)
	{
		perror("fcntl");
		goto err;
	}
	ret = fcntl(notify_fd, F_SETFL, Oflags | FASYNC);
	if(ret < 0)
	{
		perror("fcntl");
		goto err;
	}
        printf("register gpio irq callback ok\n");
	return 0;
err:
	return -1;
}

/*************************** Main ***********************************/
int main(int argc, char **argv)
{
    signal(SIGTERM, ql_sighandler);
    signal(SIGINT, ql_sighandler);

    if(daemon(1, 1) < 0)   
    {  
        perror("daemon");  
        exit(1);  
    }

    if(register_gpio_cb() < 0)
        kill(getpid(), SIGTERM); 

    wakelock_fd = Ql_SLP_WakeLock_Create("low_power_consume", sizeof("low_power_consume"));
    if(wakelock_flag != 1)
    {
        if (Ql_SLP_WakeLock_Lock(wakelock_fd) != 0)
        {
            Ql_SLP_WakeLock_Destroy(wakelock_fd);
            printf("Lock wakelock failed! \n");
            return;
        }
        wakelock_flag = 1;
    }

    Ql_Autosleep_Enable(1);

    while (deamon_flag) {
        sleep(1);
    }

    return 0;
}
