
/*
 *  Quectel iic ctrl interface
 *
 *  file name: quectel_iic_interface.h
 *
 *  Design by: jun.wu 
 *
 *  Date: 2015-11-06 
 *  
 */
#ifndef QUECTEL_IIC_INTERFACE
#define QUECTEL_IIC_INTERFACE

struct q_iic_parameters{
	uint8_t  mode;
	uint8_t  address;
	uint8_t  offset;
	uint8_t  byte_num;
	uint8_t  value[2];
};

int quec_iic_read(struct q_iic_parameters* iic_cmd);
int quec_iic_write(struct q_iic_parameters* iic_cmd);

#endif
