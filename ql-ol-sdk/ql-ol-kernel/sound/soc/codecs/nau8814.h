/*
 * nau8814.h  --  NAU8814 Soc Audio driver
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef _NAU8814_H
#define _NAU8814_H

/* NAU8814 register space */

#define NAU8814_RESET		0x0
#define NAU8814_POWER1		0x1
#define NAU8814_POWER2		0x2
#define NAU8814_POWER3		0x3
#define NAU8814_IFACE		0x4
#define NAU8814_COMP		0x5
#define NAU8814_CLOCK		0x6
#define NAU8814_ADD			0x7
#define NAU8814_GPIO		0x8
#define NAU8814_DAC			0xA
#define NAU8814_DACVOL		0xB
#define NAU8814_ADC			0xE
#define NAU8814_ADCVOL		0xF
#define NAU8814_EQ1			0x12
#define NAU8814_EQ2			0x13
#define NAU8814_EQ3			0x14
#define NAU8814_EQ4			0x15
#define NAU8814_EQ5			0x16
#define NAU8814_DACLIM1		0x18
#define NAU8814_DACLIM2		0x19
#define NAU8814_NOTCH1		0x1B
#define NAU8814_NOTCH2		0x1C
#define NAU8814_NOTCH3		0x1D
#define NAU8814_NOTCH4		0x1E
#define NAU8814_ALC1		0x20
#define NAU8814_ALC2		0x21
#define NAU8814_ALC3		0x22
#define NAU8814_NGATE		0x23
#define NAU8814_PLLN		0x24
#define NAU8814_PLLK1		0x25
#define NAU8814_PLLK2		0x26
#define NAU8814_PLLK3		0x27
#define NAU8814_ATTEN		0x28
#define NAU8814_INPUT		0x2C
#define NAU8814_INPPGA		0x2D
#define NAU8814_ADCBOOST	0x2F
#define NAU8814_OUTPUT		0x31
#define NAU8814_SPKMIX		0x32
#define NAU8814_SPKVOL		0x36
#define NAU8814_MONOMIX		0x38

#define NAU8814_POWER4		0x3A
#define NAU8814_TIMERSLOT	0x3B
#define NAU8814_ADCOUTDRIVER	0x3C

#define NAU8814_CACHEREGNUM 	60

/* Clock divider Id's */
#define NAU8814_OPCLKDIV		0
#define NAU8814_MCLKDIV		1
#define NAU8814_ADCCLK		2
#define NAU8814_DACCLK		3
#define NAU8814_BCLKDIV		4

/* DAC clock dividers */
#define NAU8814_DACCLK_F2	(1 << 3)
#define NAU8814_DACCLK_F4	(0 << 3)

/* ADC clock dividers */
#define NAU8814_ADCCLK_F2	(1 << 3)
#define NAU8814_ADCCLK_F4	(0 << 3)

/* PLL Out dividers */
#define NAU8814_OPCLKDIV_1	(0 << 4)
#define NAU8814_OPCLKDIV_2	(1 << 4)
#define NAU8814_OPCLKDIV_3	(2 << 4)
#define NAU8814_OPCLKDIV_4	(3 << 4)

/* BCLK clock dividers */
#define NAU8814_BCLKDIV_1	(0 << 2)
#define NAU8814_BCLKDIV_2	(1 << 2)
#define NAU8814_BCLKDIV_4	(2 << 2)
#define NAU8814_BCLKDIV_8	(3 << 2)
#define NAU8814_BCLKDIV_16	(4 << 2)
#define NAU8814_BCLKDIV_32	(5 << 2)

/* MCLK clock dividers */
#define NAU8814_MCLKDIV_1	(0 << 5)
#define NAU8814_MCLKDIV_1_5	(1 << 5)
#define NAU8814_MCLKDIV_2	(2 << 5)
#define NAU8814_MCLKDIV_3	(3 << 5)
#define NAU8814_MCLKDIV_4	(4 << 5)
#define NAU8814_MCLKDIV_6	(5 << 5)
#define NAU8814_MCLKDIV_8	(6 << 5)
#define NAU8814_MCLKDIV_12	(7 << 5)
//add by grady, 2017-06-21
enum {
	RT8814_AIF1,
	RT8814_AIF2,
	RT8814_AIF3,
	RT8814_AIFS,
};
struct rt8814_priv {
	struct snd_soc_codec *codec;
	struct delayed_work patch_work;

	int aif_pu;
	int sysclk;
	int sysclk_src;
	int lrck[RT8814_AIFS];
	int bclk[RT8814_AIFS];
	int master[RT8814_AIFS];

	int pll_src;
	int pll_in;
	int pll_out;

	int dmic_en;
	int dsp_sw; /* expected parameter setting */
	bool dsp_play_pass;
	bool dsp_rec_pass;
	struct regmap *regmap;
};

struct nau8814_setup_data {
	int spi;
	int i2c_bus;
	unsigned short i2c_address;
};

enum nau8814_sysclk_src {
	NAU8814_PLL,
	NAU8814_MCLK
};

#define NAU8814_DEV_NAME    "nau8814-codec"

#endif
