/*
 * SPI testing utility (using spidev driver)
 * for example: insmod /usr/lib/modules/3.18.48/kernel/drivers/spi/spidev.ko busnum=6 chipselect=0
 */

#include <stdint.h>
#include <stdio.h>
#include <linux/spi/spidev.h>
#include "ql_spi.h"

#define device   "/dev/spidev6.0"

int main(int argc, char *argv[])
{
	int fd;
	int i;
	uint8_t  writebuf[1024];
	uint8_t  readbuf[1024];

	fd = Ql_SPI_Init(device,SPIMODE0,8,S_19_2M);

	for(i = 0 ;i < 1024;i++)
		writebuf[i] = i%256;

	Ql_SPI_Write_Read(fd,writebuf,readbuf,1024);

	for (i = 0; i<1024; i++) {
		if (!(i % 32))
		puts("");
		printf("%.2X ", readbuf[i]);
	}
	puts("");
	
	return  0 ;
}
