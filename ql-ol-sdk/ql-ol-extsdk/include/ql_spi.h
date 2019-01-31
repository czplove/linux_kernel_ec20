#ifndef __QL_SPI_H__
#define __QL_SPI_H__

typedef enum
{
	SPIMODE0  =   SPI_MODE_0,
	SPIMODE1  =   SPI_MODE_1,
	SPIMODE2  =   SPI_MODE_2,
	SPIMODE3  =   SPI_MODE_3,
}SPI_MODE;

/**************************************************************************

SPI default support frequency

**************************************************************************/
typedef enum
{
	S_960K  	=   960000,
	S_4_8M 		=   4800000,
	S_9_6M  	=   9600000,
	S_16M  		=   16000000,
	S_19_2M 	=   19200000,
	S_25M  		=   25000000,
	S_50M  		=   50000000,
}SPI_SPEED;

/**************************************************************************
dev_name:need to insmod spidev.ko or quec_spi_chn.ko, The corresponding device can be found in the "/dev/" directory.
mode:the default is mode0
	CPOL:when spi work in idles,the level of sclk, 1:HIGH,0:LOW
	CHPA:sample on the first few edges of sclk     0:the first edge 1:the second edge
bits:bits of word, Supports 4 to 32 bits per word of transfer
speed:the max spi speed
return:
	spi_fd when successfully
	-1 when failed
**************************************************************************/
int Ql_SPI_Init(char *dev_name,SPI_MODE mode,uint8_t bits,SPI_SPEED speed);


/**************************************************************************
function: write and read spi data
para:
	fd:spi file description
	write_buf: the pointer to write data
	read_buf: the pointer to read data
	len:the length of data
return:
	0 when successfully
	negative value when failed
**************************************************************************/
int Ql_SPI_Write_Read(int fd,uint8_t* write_buf,uint8_t* read_buf,uint32_t len);

/**************************************************************************
close spi device fd
**************************************************************************/
int Ql_SPI_DeInit(int fd);

#endif
