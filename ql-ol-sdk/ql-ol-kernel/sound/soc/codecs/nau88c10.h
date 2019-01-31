/*
 * nau88c10.h  --  NAU88C10 Soc Audio driver
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef _NAU88C10_H
#define _NAU88C10_H

/* NAU88C10 register space */

#define NAU88c10_RESET		0x0
#define NAU88c10_POWER1		0x1
#define NAU88c10_POWER2		0x2
#define NAU88c10_POWER3		0x3
#define NAU88c10_IFACE		0x4
#define NAU88c10_COMP		0x5
#define NAU88c10_CLOCK1		0x6
#define NAU88c10_CLOCK2		0x7

#define NAU88c10_DAC			0xA
#define NAU88c10_DACVOL		0xB
#define NAU88c10_ADC			0xE
#define NAU88c10_ADCVOL		0xF
#define NAU88c10_EQ1			0x12
#define NAU88c10_EQ2			0x13
#define NAU88c10_EQ3			0x14
#define NAU88c10_EQ4			0x15
#define NAU88c10_EQ5			0x16
#define NAU88c10_DACLIM1		0x18
#define NAU88c10_DACLIM2		0x19
#define NAU88c10_NOTCH1		0x1B
#define NAU88c10_NOTCH2		0x1C
#define NAU88c10_NOTCH3		0x1D
#define NAU88c10_NOTCH4		0x1E
#define NAU88c10_ALC1		0x20
#define NAU88c10_ALC2		0x21
#define NAU88c10_ALC3		0x22
#define NAU88c10_NGATE		0x23
#define NAU88c10_PLLN		0x24
#define NAU88c10_PLLK1		0x25
#define NAU88c10_PLLK2		0x26
#define NAU88c10_PLLK3		0x27
#define NAU88c10_ATTEN		0x28
#define NAU88c10_INPUT		0x2C
#define NAU88c10_INPPGA		0x2D
#define NAU88c10_ADCBOOST	0x2F
#define NAU88c10_OUTPUT		0x31
#define NAU88c10_SPKMIX		0x32
#define NAU88c10_SPKVOL		0x36
#define NAU88c10_MONOMIX		0x38

#define NAU88c10_POWER4		0x3A
#define NAU88c10_TIMERSLOT	0x3B
#define NAU88c10_ADCOUTDRIVER	0x3C







#define NAU88C10_CACHEREGNUM 	60

/* Clock divider Id's */
#define NAU88c10_OPCLKDIV		0
#define NAU88c10_MCLKDIV		1
#define NAU88c10_ADCCLK		2
#define NAU88c10_DACCLK		3
#define NAU88c10_BCLKDIV		4

/* DAC clock dividers */
#define NAU88c10_DACCLK_F2	(1 << 3)
#define NAU88c10_DACCLK_F4	(0 << 3)

/* ADC clock dividers */
#define NAU88c10_ADCCLK_F2	(1 << 3)
#define NAU88c10_ADCCLK_F4	(0 << 3)

/* PLL Out dividers */
#define NAU88c10_OPCLKDIV_1	(0 << 4)
#define NAU88c10_OPCLKDIV_2	(1 << 4)
#define NAU88c10_OPCLKDIV_3	(2 << 4)
#define NAU88c10_OPCLKDIV_4	(3 << 4)

/* BCLK clock dividers */
#define NAU88c10_BCLKDIV_1	(0 << 2)
#define NAU88c10_BCLKDIV_2	(1 << 2)
#define NAU88c10_BCLKDIV_4	(2 << 2)
#define NAU88c10_BCLKDIV_8	(3 << 2)
#define NAU88c10_BCLKDIV_16	(4 << 2)
#define NAU88c10_BCLKDIV_32	(5 << 2)

/* MCLK clock dividers */
#define NAU88c10_MCLKDIV_1	(0 << 5)
#define NAU88c10_MCLKDIV_1_5	(1 << 5)
#define NAU88c10_MCLKDIV_2	(2 << 5)
#define NAU88c10_MCLKDIV_3	(3 << 5)
#define NAU88c10_MCLKDIV_4	(4 << 5)
#define NAU88c10_MCLKDIV_6	(5 << 5)
#define NAU88c10_MCLKDIV_8	(6 << 5)
#define NAU88c10_MCLKDIV_12	(7 << 5)

struct nau88c10_setup_data {
	int spi;
	int i2c_bus;
	unsigned short i2c_address;
};

enum nau88c10_sysclk_src {
	NAU88c10_PLL,
	NAU88c10_MCLK
};

#define NAU88C10_DEV_NAME    "nau88c10-codec"

#endif
