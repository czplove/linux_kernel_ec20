/*
 * SPI testing utility
 * for example: insmod /usr/lib/modules/3.18.48/kernel/drivers/spi/quec_spi_chn.ko speed_hz=19200000 gpiomodemready=25
 * gpiomcuready=10
 */

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/select.h>

#define device   "/dev/spi6_0_0"

static void * read_thread(void *fd);

int main(int argc, char *argv[])
{
	int fd = -1;
	pthread_t pid = -1;
	int ret;
	char write_buf[] = "hello,I am a six line spi test process!";
	fd = open(device, O_RDWR | O_NONBLOCK);
	if(fd < 0)
	{
		printf("error: %s\n", strerror(errno));
		exit(1);
	}
	ret = pthread_create(&pid, NULL, (void *)read_thread, (void*)&fd);
	if(ret != 0)
	{
		printf("error occur: %s\n", strerror(errno));
		exit(1);
	}

	while(fd > 0)
	{
		write(fd, write_buf, sizeof(write_buf));
		sleep(2);
	}
	
	pthread_join(pid, NULL);	//non-execution in this example

	return 0;
}

static void * read_thread(void *fd)
{
	int spi_fd = *(int *)fd;
	fd_set fdset;
	char read_buf[25];
	int ret;
	int nread;
	
	while(1)
	{
		FD_ZERO(&fdset);
		FD_SET(spi_fd, &fdset);
		ret = select(spi_fd + 1, &fdset, NULL, NULL, NULL);
		if(ret == -1)
		{
			printf("error occur: %s\n", strerror(errno));
			continue;
		}
		else if(FD_ISSET(spi_fd, &fdset))
		{
			do
			{
				memset(read_buf, 0, sizeof(read_buf));
				nread = read(spi_fd, read_buf, 25);
				if(nread < 0)
				{	
					printf("read error: %s\n", strerror(errno));
				}
				printf("read %d bytes %s\n", nread, read_buf);
			}while(nread == 25);
		}
		else
		{
			printf("exception occur\n");
			continue;
		}
	}
}
