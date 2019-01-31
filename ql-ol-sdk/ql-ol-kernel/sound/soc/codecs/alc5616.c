/*
 * rt5616.c  --  RT5616 ALSA SoC audio codec driver
 *
 * Copyright 2011 Realtek Semiconductor Corp.
 * Author: Johnny Hsu <johnnyhsu@realtek.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/pm.h>
#include <linux/i2c.h>
#include <linux/platform_device.h>
#include <linux/spi/spi.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>
#include <sound/soc-dapm.h>
#include <sound/initval.h>
#include <sound/tlv.h>

#include "alc5616.h"

#define POWER_ON_MICBIAS1
#define RT5616_REG_RW 0 /* for debug */

struct rt5616_init_reg {
	u8 reg;
	u16 val;
};

static struct rt5616_init_reg init_list[] = {
	{ RT5616_D_MISC, 		0x0011 } ,
	{ RT5616_PWR_DIG1, 		0x9806 } ,
	{ RT5616_PWR_DIG2, 		0x8800 } ,
	{ RT5616_PWR_ANLG1, 	0xF8FE } ,
	{ RT5616_PWR_ANLG2, 	0x0A00 } ,
	{ RT5616_PWR_MIXER, 	0xCC00 } ,
	{ RT5616_PWR_VOL, 		0x3F00 } ,

	{ RT5616_PRIV_INDEX, 	0x003D } ,
	{ RT5616_PRIV_DATA, 	0x3600 } ,
	{ RT5616_I2S1_SDP, 		0x8082 } ,
	{ RT5616_ADDA_CLK1, 	0x0000 } , // achang-20180524, improve DAC SNR.
	{ RT5616_ADDA_CLK2, 	0x0C00 } ,
	{ RT5616_GLB_CLK, 		0x5000 } ,
	{ RT5616_PLL_CTRL1, 	0x0E1C } ,
	{ RT5616_PLL_CTRL2, 	0x0800 } ,

	{ RT5616_HP_VOL, 		0x1010 } ,
	{ RT5616_LOUT_CTRL1, 	0x0808 } ,
	{ RT5616_LOUT_CTRL2, 	0x8000 } ,

	{ RT5616_IN1_IN2, 		0x04C0 } ,
	{ RT5616_INL1_INR1_VOL, 0x0808 } ,
	{ RT5616_DAC1_DIG_VOL,  0xAFAF } ,
	{ RT5616_ADC_DIG_VOL, 	0x5c5c } ,
	{ RT5616_ADC_BST_VOL, 	0x0000 } ,
	{ RT5616_STO1_ADC_MIXER,0x3820 } ,
	{ RT5616_AD_DA_MIXER, 	0x8080 } ,
	{ RT5616_STO_DAC_MIXER, 0x1250 } ,

	{ RT5616_REC_L1_MIXER, 	0x0000 } ,
	{ RT5616_REC_L2_MIXER, 	0x004F } ,
	{ RT5616_REC_R1_MIXER, 	0x2000 } ,
	{ RT5616_REC_R2_MIXER, 	0x004F } ,

	{ RT5616_HPO_MIXER, 	0x4000 } ,
	{ RT5616_OUT_L1_MIXER,  0x0000 } ,
	{ RT5616_OUT_L2_MIXER,  0x0000 } ,
	{ RT5616_OUT_L3_MIXER,  0x0278 } ,
	{ RT5616_OUT_R1_MIXER,  0x0000 } ,
	{ RT5616_OUT_R2_MIXER,  0x0000 } ,
	{ RT5616_OUT_R3_MIXER,  0x0278 } ,
	{ RT5616_LOUT_MIXER, 	0xC800 } , 

	{ RT5616_DEPOP_M1, 		0x8019 } ,
	{ RT5616_DEPOP_M2, 		0x3100 } ,
	{ RT5616_MICBIAS, 		0x2C00 } ,
	{ RT5616_A_JD_CTL1, 	0x0200 } ,

	{ RT5616_EQ_CTRL1, 		0x2080 } ,
	{ RT5616_EQ_CTRL2, 		0x0000 } ,

    // 2017.10.24 eddy.qin add
	{ RT5616_CHARGE_PUMP,   0x0e00 },
    // end eddy.qin
};

#define RT5616_INIT_REG_LEN ARRAY_SIZE(init_list)
static int alc_val = 0;
static int alc_pwrctr = 0;

static int rt5616_reg_init(struct snd_soc_codec *codec)
{
	int i;

	dev_err(codec->dev, "func: %s, init codec \n", __func__);
	for (i = 0; i < RT5616_INIT_REG_LEN; i++)
		snd_soc_write(codec, init_list[i].reg, init_list[i].val);

	return 0;
}

static int rt5616_index_sync(struct snd_soc_codec *codec)
{
	int i;

	dev_err(codec->dev, "func: %s, sync register\n", __func__);
	for (i = 0; i < RT5616_INIT_REG_LEN; i++)
		if (RT5616_PRIV_INDEX == init_list[i].reg ||
			RT5616_PRIV_DATA == init_list[i].reg)
			snd_soc_write(codec, init_list[i].reg,
					init_list[i].val);
	return 0;
}

static const u16 rt5616_reg[RT5616_DEVICE_ID + 1] = {
	[RT5616_RESET] = 0x0020,
	[RT5616_HP_VOL] = 0xc8c8,
	[RT5616_LOUT_CTRL1] = 0xc8c8,
	[RT5616_INL1_INR1_VOL] = 0x0808,
	[RT5616_DAC1_DIG_VOL] = 0xafaf,
	[RT5616_ADC_DIG_VOL] = 0x2f2f,
	[RT5616_STO1_ADC_MIXER] = 0x7860,
	[RT5616_AD_DA_MIXER] = 0x8080,
	[RT5616_STO_DAC_MIXER] = 0x5252,
	[RT5616_REC_L2_MIXER] = 0x006f,
	[RT5616_REC_R2_MIXER] = 0x006f,
	[RT5616_HPO_MIXER] = 0x6000,
	[RT5616_OUT_L3_MIXER] = 0x0279,
 	[RT5616_OUT_R3_MIXER] = 0x0279,
	[RT5616_LOUT_MIXER] = 0xf000,
	[RT5616_PWR_ANLG1] = 0x00c0,
	[RT5616_I2S1_SDP] = 0x8000,
	[RT5616_ADDA_CLK1] = 0x1104,
	[RT5616_ADDA_CLK2] = 0x0c00,
	[RT5616_HP_OVCD] = 0x0600,
	[RT5616_DEPOP_M1] = 0x0004,
	[RT5616_DEPOP_M2] = 0x1100,
	[RT5616_MICBIAS] = 0x2000,
	[RT5616_A_JD_CTL1] = 0x0200,
	[RT5616_EQ_CTRL1] = 0x2080,
	[RT5616_ALC_1] = 0x2206,
	[RT5616_ALC_2] = 0x1f00,
	[RT5616_GPIO_CTRL1] = 0x0400,
	[RT5616_BASE_BACK] = 0x0013,
	[RT5616_MP3_PLUS1] = 0x0680,
	[RT5616_MP3_PLUS2] = 0x1c17,
	[RT5616_ADJ_HPF_CTRL1] = 0xb320,
	[RT5616_SV_ZCD1] = 0x0809,
	[RT5616_D_MISC] = 0x0010,
	[RT5616_VENDOR_ID] = 0x10ec,
	[RT5616_DEVICE_ID] = 0x6281,
};

static int rt5616_reset(struct snd_soc_codec *codec)
{
	dev_err(codec->dev, "func: %s, reset codec \n", __func__);
	return snd_soc_write(codec, RT5616_RESET, 0);
}

/**
 * rt5616_index_write - Write private register.
 * @codec: SoC audio codec device.
 * @reg: Private register index.
 * @value: Private register Data.
 *
 * Modify private register for advanced setting. It can be written through
 * private index (0x6a) and data (0x6c) register.
 *
 * Returns 0 for success or negative error code.
 */
static int rt5616_index_write(struct snd_soc_codec *codec,
		unsigned int reg, unsigned int value)
{
	int ret;

	dev_err(codec->dev, "func: %s, write pri register, reg: 0x%x, value: 0x%x\n", __func__, reg, value);
	ret = snd_soc_write(codec, RT5616_PRIV_INDEX, reg);
	if (ret < 0) {
		dev_err(codec->dev, "Failed to set private addr: %d\n", ret);
		goto err;
	}
	ret = snd_soc_write(codec, RT5616_PRIV_DATA, value);
	if (ret < 0) {
		dev_err(codec->dev, "Failed to set private value: %d\n", ret);
		goto err;
	}
	return 0;

err:
	return ret;
}

/**
 * rt5616_index_read - Read private register.
 * @codec: SoC audio codec device.
 * @reg: Private register index.
 *
 * Read advanced setting from private register. It can be read through
 * private index (0x6a) and data (0x6c) register.
 *
 * Returns private register value or negative error code.
 */
static unsigned int rt5616_index_read(
	struct snd_soc_codec *codec, unsigned int reg)
{
	int ret;

	dev_err(codec->dev, "func: %s, write pri register, reg: 0x%x\n", __func__, reg);
	ret = snd_soc_write(codec, RT5616_PRIV_INDEX, reg);
	if (ret < 0) {
		dev_err(codec->dev, "Failed to set private addr: %d\n", ret);
		return ret;
	}

	return snd_soc_read(codec, RT5616_PRIV_DATA);
}

/**
 * rt5616_index_update_bits - update private register bits
 * @codec: audio codec
 * @reg: Private register index.
 * @mask: register mask
 * @value: new value
 *
 * Writes new register value.
 *
 * Returns 1 for change, 0 for no change, or negative error code.
 */
static int rt5616_index_update_bits(struct snd_soc_codec *codec,
	unsigned int reg, unsigned int mask, unsigned int value)
{
	unsigned int old, new;
	int change, ret;

	ret = rt5616_index_read(codec, reg);
	if (ret < 0) {
		dev_err(codec->dev, "Failed to read private reg: %d\n", ret);
		goto err;
	}

	old = ret;
	new = (old & ~mask) | (value & mask);
	change = old != new;
	if (change) {
		ret = rt5616_index_write(codec, reg, new);
		if (ret < 0) {
			dev_err(codec->dev,
				"Failed to write private reg: %d\n", ret);
			goto err;
		}
	}
	return change;

err:
	return ret;
}

static int rt5616_volatile_register(
	struct snd_soc_codec *codec, unsigned int reg)
{
	switch (reg) {
	case RT5616_RESET:
	case RT5616_PRIV_DATA:
	case RT5616_EQ_CTRL1:
	case RT5616_ALC_1:
	case RT5616_IRQ_CTRL2:
	case RT5616_INT_IRQ_ST:
	case RT5616_PGM_REG_ARR1:
	case RT5616_PGM_REG_ARR3:
	case RT5616_VENDOR_ID:
	case RT5616_DEVICE_ID:
		return 1;
	default:
		return 0;
	}
}

static int rt5616_readable_register(
	struct snd_soc_codec *codec, unsigned int reg)
{
	switch (reg) {
	case RT5616_RESET:
	case RT5616_VERSION_ID:
	case RT5616_VENDOR_ID:
	case RT5616_DEVICE_ID:
	case RT5616_HP_VOL:
	case RT5616_LOUT_CTRL1:
	case RT5616_LOUT_CTRL2:
	case RT5616_IN1_IN2:
	case RT5616_INL1_INR1_VOL:
	case RT5616_DAC1_DIG_VOL:
	case RT5616_ADC_DIG_VOL:
	case RT5616_ADC_BST_VOL:
	case RT5616_STO1_ADC_MIXER:
	case RT5616_AD_DA_MIXER:
	case RT5616_STO_DAC_MIXER:
	case RT5616_REC_L1_MIXER:
	case RT5616_REC_L2_MIXER:
	case RT5616_REC_R1_MIXER:
	case RT5616_REC_R2_MIXER:
	case RT5616_HPO_MIXER:
	case RT5616_OUT_L1_MIXER:
	case RT5616_OUT_L2_MIXER:
	case RT5616_OUT_L3_MIXER:
	case RT5616_OUT_R1_MIXER:
	case RT5616_OUT_R2_MIXER:
	case RT5616_OUT_R3_MIXER:
	case RT5616_LOUT_MIXER:
	case RT5616_PWR_DIG1:
	case RT5616_PWR_DIG2:
	case RT5616_PWR_ANLG1:
	case RT5616_PWR_ANLG2:
	case RT5616_PWR_MIXER:
	case RT5616_PWR_VOL:
	case RT5616_PRIV_INDEX:
	case RT5616_PRIV_DATA:
	case RT5616_I2S1_SDP:
	case RT5616_ADDA_CLK1:
	case RT5616_ADDA_CLK2:
	case RT5616_GLB_CLK:
	case RT5616_PLL_CTRL1:
	case RT5616_PLL_CTRL2:
	case RT5616_HP_OVCD:
	case RT5616_DEPOP_M1:
	case RT5616_DEPOP_M2:
	case RT5616_DEPOP_M3:
	case RT5616_CHARGE_PUMP:
	case RT5616_PV_DET_SPK_G:
	case RT5616_MICBIAS:
	case RT5616_A_JD_CTL1:
	case RT5616_A_JD_CTL2:
	case RT5616_EQ_CTRL1:
	case RT5616_EQ_CTRL2:
	case RT5616_WIND_FILTER	:
	case RT5616_ALC_1:
	case RT5616_ALC_2:
	case RT5616_ALC_3:
	case RT5616_SVOL_ZC:
	case RT5616_JD_CTRL1:
	case RT5616_JD_CTRL2:
	case RT5616_IRQ_CTRL1:
	case RT5616_IRQ_CTRL2:
	case RT5616_INT_IRQ_ST:
	case RT5616_GPIO_CTRL1:
	case RT5616_GPIO_CTRL2:
	case RT5616_GPIO_CTRL3:
	case RT5616_PGM_REG_ARR1:
	case RT5616_PGM_REG_ARR2:
	case RT5616_PGM_REG_ARR3:
	case RT5616_PGM_REG_ARR4:
	case RT5616_PGM_REG_ARR5:
	case RT5616_SCB_FUNC:
	case RT5616_SCB_CTRL:
	case RT5616_BASE_BACK:
	case RT5616_MP3_PLUS1:
	case RT5616_MP3_PLUS2:
	case RT5616_ADJ_HPF_CTRL1:
	case RT5616_ADJ_HPF_CTRL2:
	case RT5616_HP_CALIB_AMP_DET:
	case RT5616_HP_CALIB2:
	case RT5616_SV_ZCD1:
	case RT5616_SV_ZCD2:
	case RT5616_D_MISC:
	case RT5616_DUMMY2:
	case RT5616_DUMMY3:
		return 1;
	default:
		return 0;
	}
}

/**
 * rt5616_headset_detect - Detect headset.
 * @codec: SoC audio codec device.
 * @jack_insert: Jack insert or not.
 *
 * Detect whether is headset or not when jack inserted.
 *
 * Returns detect status.
 */
 /*
int rt5616_headset_detect(struct snd_soc_codec *codec, int jack_insert)
{
	int jack_type;
	int sclk_src;

	if(jack_insert) {
		if (SND_SOC_BIAS_OFF == codec->dapm.bias_level) {
			snd_soc_write(codec, RT5616_PWR_ANLG1, 0x2004);
			snd_soc_write(codec, RT5616_MICBIAS, 0x3830);
			snd_soc_write(codec, RT5616_DUMMY1 , 0x3701);
		}
		sclk_src = snd_soc_read(codec, RT5616_GLB_CLK) &
			RT5616_SCLK_SRC_MASK;
		snd_soc_update_bits(codec, RT5616_GLB_CLK,
			RT5616_SCLK_SRC_MASK, 0x3 << RT5616_SCLK_SRC_SFT);
		snd_soc_update_bits(codec, RT5616_PWR_ANLG1,
			RT5616_PWR_LDO, RT5616_PWR_LDO);
		snd_soc_update_bits(codec, RT5616_PWR_ANLG2,
			RT5616_PWR_MB1, RT5616_PWR_MB1);
		snd_soc_update_bits(codec, RT5616_MICBIAS,
			RT5616_MIC1_OVCD_MASK | RT5616_MIC1_OVTH_MASK |
			RT5616_PWR_CLK25M_MASK | RT5616_PWR_MB_MASK,
			RT5616_MIC1_OVCD_EN | RT5616_MIC1_OVTH_600UA |
			RT5616_PWR_MB_PU | RT5616_PWR_CLK25M_PU);
		snd_soc_update_bits(codec, RT5616_DUMMY1,
			0x1, 0x1);
		msleep(100);
		if (snd_soc_read(codec, RT5616_IRQ_CTRL2) & 0x8)
			jack_type = RT5616_HEADPHO_DET;
		else
			jack_type = RT5616_HEADSET_DET;
		snd_soc_update_bits(codec, RT5616_IRQ_CTRL2,
			RT5616_MB1_OC_CLR, 0);
		snd_soc_update_bits(codec, RT5616_GLB_CLK,
			RT5616_SCLK_SRC_MASK, sclk_src);
	} else {
		snd_soc_update_bits(codec, RT5616_MICBIAS,
			RT5616_MIC1_OVCD_MASK,
			RT5616_MIC1_OVCD_DIS);
		
		jack_type = RT5616_NO_JACK;
	}

	return jack_type;
}
EXPORT_SYMBOL(rt5616_headset_detect);
*/

static const DECLARE_TLV_DB_SCALE(out_vol_tlv, -4650, 150, 0);
static const DECLARE_TLV_DB_SCALE(dac_vol_tlv, -65625, 375, 0);
static const DECLARE_TLV_DB_SCALE(in_vol_tlv, -3450, 150, 0);
static const DECLARE_TLV_DB_SCALE(adc_vol_tlv, -17625, 375, 0);
static const DECLARE_TLV_DB_SCALE(adc_bst_tlv, 0, 1200, 0);

/* {0, +20, +24, +30, +35, +40, +44, +50, +52} dB */
static unsigned int bst_tlv[] = {
	TLV_DB_RANGE_HEAD(7),
	0, 0, TLV_DB_SCALE_ITEM(0, 0, 0),
	1, 1, TLV_DB_SCALE_ITEM(2000, 0, 0),
	2, 2, TLV_DB_SCALE_ITEM(2400, 0, 0),
	3, 5, TLV_DB_SCALE_ITEM(3000, 500, 0),
	6, 6, TLV_DB_SCALE_ITEM(4400, 0, 0),
	7, 7, TLV_DB_SCALE_ITEM(5000, 0, 0),
	8, 8, TLV_DB_SCALE_ITEM(5200, 0, 0),
};

/* IN1/IN2 Input Type */
static const char *rt5616_input_mode[] = {
	"Single ended", "Differential"};

static const SOC_ENUM_SINGLE_DECL(
	rt5616_in1_mode_enum, RT5616_IN1_IN2,
	RT5616_IN_SFT1, rt5616_input_mode);

static const SOC_ENUM_SINGLE_DECL(
	rt5616_in2_mode_enum, RT5616_IN1_IN2,
	RT5616_IN_SFT2, rt5616_input_mode);

#ifdef RT5616_REG_RW
#define REGVAL_MAX 0xffff
static unsigned int regctl_addr;
static int rt5616_regctl_info(struct snd_kcontrol *kcontrol,
			struct snd_ctl_elem_info *uinfo)
{
	uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;
	uinfo->count = 2;
	uinfo->value.integer.min = 0;
	uinfo->value.integer.max = REGVAL_MAX;
	return 0;
}

static int rt5616_regctl_get(struct snd_kcontrol *kcontrol,
			struct snd_ctl_elem_value *ucontrol)
{
	struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);
	ucontrol->value.integer.value[0] = regctl_addr;
	ucontrol->value.integer.value[1] = snd_soc_read(codec, regctl_addr);
	return 0;
}

static int rt5616_regctl_put(struct snd_kcontrol *kcontrol,
			struct snd_ctl_elem_value *ucontrol)
{
	struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);
	regctl_addr = ucontrol->value.integer.value[0];
	if(ucontrol->value.integer.value[1] <= REGVAL_MAX)
		snd_soc_write(codec, regctl_addr, ucontrol->value.integer.value[1]);
	return 0;
}
#endif

static int rt5616_vol_rescale_get(struct snd_kcontrol *kcontrol,
		struct snd_ctl_elem_value *ucontrol)
{
	struct soc_mixer_control *mc =
		(struct soc_mixer_control *)kcontrol->private_value;
	struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);
	unsigned int val = snd_soc_read(codec, mc->reg);

	ucontrol->value.integer.value[0] = RT5616_VOL_RSCL_MAX -
		((val & RT5616_L_VOL_MASK) >> mc->shift);
	ucontrol->value.integer.value[1] = RT5616_VOL_RSCL_MAX -
		(val & RT5616_R_VOL_MASK);

	return 0;
}

static int rt5616_vol_rescale_put(struct snd_kcontrol *kcontrol,
		struct snd_ctl_elem_value *ucontrol)
{
	struct soc_mixer_control *mc =
		(struct soc_mixer_control *)kcontrol->private_value;
	struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);
	unsigned int val, val2;

	val = RT5616_VOL_RSCL_MAX - ucontrol->value.integer.value[0];
	val2 = RT5616_VOL_RSCL_MAX - ucontrol->value.integer.value[1];
	return snd_soc_update_bits(codec, mc->reg, RT5616_L_VOL_MASK |
			RT5616_R_VOL_MASK, val << mc->shift | val2);
}

static const struct snd_kcontrol_new rt5616_snd_controls[] = {
	/* Headphone Output Volume */
	SOC_DOUBLE("HP Playback Switch", RT5616_HP_VOL,
		RT5616_L_MUTE_SFT, RT5616_R_MUTE_SFT, 1, 1),
    // 2017.10.24 eddy.qin changed
	//SOC_DOUBLE("HP Channel Switch", RT5616_HP_VOL,
	//	RT5616_VOL_L_SFT, RT5616_VOL_R_SFT, 1, 1),
	// end eddy.qin
	SOC_DOUBLE_EXT_TLV("HP Playback Volume", RT5616_HP_VOL,
		RT5616_L_VOL_SFT, RT5616_R_VOL_SFT, RT5616_VOL_RSCL_RANGE, 0,
		rt5616_vol_rescale_get, rt5616_vol_rescale_put, out_vol_tlv),
	/* OUTPUT Control */
	SOC_DOUBLE("OUT Playback Switch", RT5616_LOUT_CTRL1,
		RT5616_L_MUTE_SFT, RT5616_R_MUTE_SFT, 1, 1),
	SOC_DOUBLE("OUT Channel Switch", RT5616_LOUT_CTRL1,
		RT5616_VOL_L_SFT, RT5616_VOL_R_SFT, 1, 1),
	SOC_DOUBLE_TLV("OUT Playback Volume", RT5616_LOUT_CTRL1,
		RT5616_L_VOL_SFT, RT5616_R_VOL_SFT, 39, 1, out_vol_tlv),

	/* DAC Digital Volume */
	SOC_DOUBLE_TLV("DAC1 Playback Volume", RT5616_DAC1_DIG_VOL,
			RT5616_L_VOL_SFT, RT5616_R_VOL_SFT,
			175, 0, dac_vol_tlv),
	/* IN1/IN2 Control */
	SOC_ENUM("IN1 Mode Control",  rt5616_in1_mode_enum),
	SOC_SINGLE_TLV("IN1 Boost", RT5616_IN1_IN2,
		RT5616_BST_SFT1, 8, 0, bst_tlv),
	SOC_ENUM("IN2 Mode Control", rt5616_in2_mode_enum),
	SOC_SINGLE_TLV("IN2 Boost", RT5616_IN1_IN2,
		RT5616_BST_SFT2, 8, 0, bst_tlv),
	/* INL/INR Volume Control */
	SOC_DOUBLE_TLV("IN Capture Volume", RT5616_INL1_INR1_VOL,
			RT5616_INL_VOL_SFT, RT5616_INR_VOL_SFT,
			31, 1, in_vol_tlv),
	/* ADC Digital Volume Control */
	SOC_DOUBLE("ADC Capture Switch", RT5616_ADC_DIG_VOL,
		RT5616_L_MUTE_SFT, RT5616_R_MUTE_SFT, 1, 1),
	SOC_DOUBLE_TLV("ADC Capture Volume", RT5616_ADC_DIG_VOL,
			RT5616_L_VOL_SFT, RT5616_R_VOL_SFT,
			127, 0, adc_vol_tlv),

	/* ADC Boost Volume Control */
	SOC_DOUBLE_TLV("ADC Boost Gain", RT5616_ADC_BST_VOL,
			RT5616_ADC_L_BST_SFT, RT5616_ADC_R_BST_SFT,
			3, 0, adc_bst_tlv),

#ifdef RT5616_REG_RW
	{
		.iface = SNDRV_CTL_ELEM_IFACE_MIXER,
		.name = "Register Control",
		.info = rt5616_regctl_info,
		.get = rt5616_regctl_get,
		.put = rt5616_regctl_put,
	},
#endif
};

static int check_sysclk1_source(struct snd_soc_dapm_widget *source,
			 struct snd_soc_dapm_widget *sink)
{
	unsigned int val;

	val = snd_soc_read(source->codec, RT5616_GLB_CLK);
	val &= RT5616_SCLK_SRC_MASK;
	if (val == RT5616_SCLK_SRC_PLL1)
		return 1;
	else
		return 0;
}

/* Digital Mixer */
static const struct snd_kcontrol_new rt5616_sto1_adc_l_mix[] = {
	SOC_DAPM_SINGLE("ADC1 Switch", RT5616_STO1_ADC_MIXER,
			RT5616_M_STO1_ADC_L1_SFT, 1, 1),
};

static const struct snd_kcontrol_new rt5616_sto1_adc_r_mix[] = {
	SOC_DAPM_SINGLE("ADC1 Switch", RT5616_STO1_ADC_MIXER,
			RT5616_M_STO1_ADC_R1_SFT, 1, 1),
};

static const struct snd_kcontrol_new rt5616_dac_l_mix[] = {
	SOC_DAPM_SINGLE("Stereo ADC Switch", RT5616_AD_DA_MIXER,
			RT5616_M_ADCMIX_L_SFT, 1, 1),
	SOC_DAPM_SINGLE("INF1 Switch", RT5616_AD_DA_MIXER,
			RT5616_M_IF1_DAC_L_SFT, 1, 1),
};

static const struct snd_kcontrol_new rt5616_dac_r_mix[] = {
	SOC_DAPM_SINGLE("Stereo ADC Switch", RT5616_AD_DA_MIXER,
			RT5616_M_ADCMIX_R_SFT, 1, 1),
	SOC_DAPM_SINGLE("INF1 Switch", RT5616_AD_DA_MIXER,
			RT5616_M_IF1_DAC_R_SFT, 1, 1),
};

static const struct snd_kcontrol_new rt5616_sto_dac_l_mix[] = {
	SOC_DAPM_SINGLE("DAC L1 Switch", RT5616_STO_DAC_MIXER,
			RT5616_M_DAC_L1_MIXL_SFT, 1, 1),
	SOC_DAPM_SINGLE("DAC R1 Switch", RT5616_STO_DAC_MIXER,
			RT5616_M_DAC_R1_MIXL_SFT, 1, 1),
};

static const struct snd_kcontrol_new rt5616_sto_dac_r_mix[] = {
	SOC_DAPM_SINGLE("DAC R1 Switch", RT5616_STO_DAC_MIXER,
			RT5616_M_DAC_R1_MIXR_SFT, 1, 1),
	SOC_DAPM_SINGLE("DAC L1 Switch", RT5616_STO_DAC_MIXER,
			RT5616_M_DAC_L1_MIXR_SFT, 1, 1),
};

/* Analog Input Mixer */
static const struct snd_kcontrol_new rt5616_rec_l_mix[] = {
	SOC_DAPM_SINGLE("INL1 Switch", RT5616_REC_L2_MIXER,
			RT5616_M_IN1_L_RM_L_SFT, 1, 1),
	SOC_DAPM_SINGLE("BST2 Switch", RT5616_REC_L2_MIXER,
			RT5616_M_BST2_RM_L_SFT, 1, 1),
	SOC_DAPM_SINGLE("BST1 Switch", RT5616_REC_L2_MIXER,
			RT5616_M_BST1_RM_L_SFT, 1, 1),
};

static const struct snd_kcontrol_new rt5616_rec_r_mix[] = {
	SOC_DAPM_SINGLE("INR1 Switch", RT5616_REC_R2_MIXER,
			RT5616_M_IN1_R_RM_R_SFT, 1, 1),
	SOC_DAPM_SINGLE("BST2 Switch", RT5616_REC_R2_MIXER,
			RT5616_M_BST2_RM_R_SFT, 1, 1),
	SOC_DAPM_SINGLE("BST1 Switch", RT5616_REC_R2_MIXER,
			RT5616_M_BST1_RM_R_SFT, 1, 1),
};

/* Analog Output Mixer */

static const struct snd_kcontrol_new rt5616_out_l_mix[] = {
	SOC_DAPM_SINGLE("BST1 Switch", RT5616_OUT_L3_MIXER,
			RT5616_M_BST1_OM_L_SFT, 1, 1),
	SOC_DAPM_SINGLE("BST2 Switch", RT5616_OUT_L3_MIXER,
			RT5616_M_BST2_OM_L_SFT, 1, 1),
	SOC_DAPM_SINGLE("INL1 Switch", RT5616_OUT_L3_MIXER,
			RT5616_M_IN1_L_OM_L_SFT, 1, 1),
	SOC_DAPM_SINGLE("REC MIXL Switch", RT5616_OUT_L3_MIXER,
			RT5616_M_RM_L_OM_L_SFT, 1, 1),
	SOC_DAPM_SINGLE("DAC L1 Switch", RT5616_OUT_L3_MIXER,
			RT5616_M_DAC_L1_OM_L_SFT, 1, 1),
};

static const struct snd_kcontrol_new rt5616_out_r_mix[] = {
	SOC_DAPM_SINGLE("BST2 Switch", RT5616_OUT_R3_MIXER,
			RT5616_M_BST2_OM_R_SFT, 1, 1),
	SOC_DAPM_SINGLE("BST1 Switch", RT5616_OUT_R3_MIXER,
			RT5616_M_BST1_OM_R_SFT, 1, 1),
	SOC_DAPM_SINGLE("INR1 Switch", RT5616_OUT_R3_MIXER,
			RT5616_M_IN1_R_OM_R_SFT, 1, 1),
	SOC_DAPM_SINGLE("REC MIXR Switch", RT5616_OUT_R3_MIXER,
			RT5616_M_RM_R_OM_R_SFT, 1, 1),
	SOC_DAPM_SINGLE("DAC R1 Switch", RT5616_OUT_R3_MIXER,
			RT5616_M_DAC_R1_OM_R_SFT, 1, 1),
};

static const struct snd_kcontrol_new rt5616_hpo_mix[] = {
	SOC_DAPM_SINGLE("DAC1 Switch", RT5616_HPO_MIXER,
			RT5616_M_DAC1_HM_SFT, 1, 1),
	SOC_DAPM_SINGLE("HPVOL Switch", RT5616_HPO_MIXER,
			RT5616_M_HPVOL_HM_SFT, 1, 1),
};

static const struct snd_kcontrol_new rt5616_lout_mix[] = {
	SOC_DAPM_SINGLE("DAC L1 Switch", RT5616_LOUT_MIXER,
			RT5616_M_DAC_L1_LM_SFT, 1, 1),
	SOC_DAPM_SINGLE("DAC R1 Switch", RT5616_LOUT_MIXER,
			RT5616_M_DAC_R1_LM_SFT, 1, 1),
	SOC_DAPM_SINGLE("OUTVOL L Switch", RT5616_LOUT_MIXER,
			RT5616_M_OV_L_LM_SFT, 1, 1),
	SOC_DAPM_SINGLE("OUTVOL R Switch", RT5616_LOUT_MIXER,
			RT5616_M_OV_R_LM_SFT, 1, 1),
};

static int rt5616_adc_event(struct snd_soc_dapm_widget *w,
	struct snd_kcontrol *kcontrol, int event)
{
    // 2017.10.24 eddy.qin changed
#if 0
	struct snd_soc_codec *codec = w->codec;

	switch (event) {
	case SND_SOC_DAPM_POST_PMU:
		snd_soc_update_bits(codec, RT5616_ADC_DIG_VOL, RT5616_L_MUTE | RT5616_R_MUTE, 0);
		break;

	case SND_SOC_DAPM_POST_PMD:
		snd_soc_update_bits(codec, RT5616_ADC_DIG_VOL, RT5616_L_MUTE | RT5616_R_MUTE, RT5616_L_MUTE | RT5616_R_MUTE);
		break;

	default:
		return 0;
	}
#endif
    // end eddy.qin

	return 0;
}

void hp_amp_power(struct snd_soc_codec *codec, int on)
{
	static int hp_amp_power_count;

	if(on) {
		if(hp_amp_power_count <= 0) {
			/* depop parameters */
			snd_soc_update_bits(codec, RT5616_DEPOP_M2,
				RT5616_DEPOP_MASK, RT5616_DEPOP_MAN);
            // 2017.10.24 eddy.qin comment
			//snd_soc_update_bits(codec, RT5616_DEPOP_M1,
			//	RT5616_HP_CP_MASK | RT5616_HP_SG_MASK | RT5616_HP_CB_MASK,
			//	RT5616_HP_CP_PU | RT5616_HP_SG_DIS | RT5616_HP_CB_PU);
			// end eddy.qin
			rt5616_index_write(codec, RT5616_HP_DCC_INT1, 0x9f00);
			/* headphone amp power on */
			snd_soc_update_bits(codec, RT5616_PWR_ANLG1,
				RT5616_PWR_FV1 | RT5616_PWR_FV2 , 0);
			snd_soc_update_bits(codec, RT5616_PWR_VOL,
				RT5616_PWR_HV_L | RT5616_PWR_HV_R,
				RT5616_PWR_HV_L | RT5616_PWR_HV_R);
			//if(0 == alc_val)
			{
				snd_soc_update_bits(codec, RT5616_PWR_ANLG1,
					RT5616_PWR_HP_L | RT5616_PWR_HP_R | RT5616_PWR_HA | RT5616_PWR_LM,
					RT5616_PWR_HP_L | RT5616_PWR_HP_R | RT5616_PWR_HA | RT5616_PWR_LM);
			}
			msleep(50);
			snd_soc_update_bits(codec, RT5616_PWR_ANLG1,
				RT5616_PWR_FV1 | RT5616_PWR_FV2,
				RT5616_PWR_FV1 | RT5616_PWR_FV2);

            // 2017.10.24 eddy.qin removed
			//snd_soc_update_bits(codec, RT5616_CHARGE_PUMP,
			//	RT5616_PM_HP_MASK, RT5616_PM_HP_HV);
			// end eddy.qin
			rt5616_index_update_bits(codec, RT5616_CHOP_DAC_ADC, 0x0200, 0x0200);
            // 2017.10.24 eddy.qin comment
			//snd_soc_update_bits(codec, RT5616_DEPOP_M1,
			//	RT5616_HP_CO_MASK | RT5616_HP_SG_MASK,
			//	RT5616_HP_CO_EN | RT5616_HP_SG_EN);
			// end eddy.qin
		}
		hp_amp_power_count++;
	} else {
		hp_amp_power_count--;
		if(hp_amp_power_count <= 0) {
			rt5616_index_update_bits(codec, RT5616_CHOP_DAC_ADC, 0x0200, 0x0);
            // 2017.10.24 eddy.qin comment
			//snd_soc_update_bits(codec, RT5616_DEPOP_M1,
			//	RT5616_HP_SG_MASK | RT5616_HP_L_SMT_MASK |
			//	RT5616_HP_R_SMT_MASK, RT5616_HP_SG_DIS |
			//	RT5616_HP_L_SMT_DIS | RT5616_HP_R_SMT_DIS);
			/* headphone amp power down */
			//snd_soc_update_bits(codec, RT5616_DEPOP_M1,
			//	RT5616_SMT_TRIG_MASK | RT5616_HP_CD_PD_MASK |
			//	RT5616_HP_CO_MASK | RT5616_HP_CP_MASK |
			//	RT5616_HP_SG_MASK | RT5616_HP_CB_MASK,
			//	RT5616_SMT_TRIG_DIS | RT5616_HP_CD_PD_EN |
			//	RT5616_HP_CO_DIS | RT5616_HP_CP_PD |
			//	RT5616_HP_SG_EN | RT5616_HP_CB_PD);
			// end eddy.qin
			//if(0 == alc_val)
			{
				snd_soc_update_bits(codec, RT5616_PWR_ANLG1,
				RT5616_PWR_HP_L | RT5616_PWR_HP_R | RT5616_PWR_HA | RT5616_PWR_LM,
				0);
			}
		}
	}
}

static void rt5616_pmu_depop(struct snd_soc_codec *codec)
{
	hp_amp_power(codec, 1);

	/* headphone unmute sequence */
	snd_soc_update_bits(codec, RT5616_DEPOP_M3,
		RT5616_CP_FQ1_MASK | RT5616_CP_FQ2_MASK | RT5616_CP_FQ3_MASK,
		(RT5616_CP_FQ_192_KHZ << RT5616_CP_FQ1_SFT) |
		(RT5616_CP_FQ_12_KHZ << RT5616_CP_FQ2_SFT) |
		(RT5616_CP_FQ_192_KHZ << RT5616_CP_FQ3_SFT));
	rt5616_index_write(codec, RT5616_MAMP_INT_REG2, 0xfc00);
    // 2017.10.24 eddy.qin comment
	//snd_soc_update_bits(codec, RT5616_DEPOP_M1,
	//	RT5616_SMT_TRIG_MASK, RT5616_SMT_TRIG_EN);
	//snd_soc_update_bits(codec, RT5616_DEPOP_M1,
	//	RT5616_RSTN_MASK, RT5616_RSTN_EN);
	//snd_soc_update_bits(codec, RT5616_DEPOP_M1,
	//	RT5616_RSTN_MASK | RT5616_HP_L_SMT_MASK | RT5616_HP_R_SMT_MASK,
	//	RT5616_RSTN_DIS | RT5616_HP_L_SMT_EN | RT5616_HP_R_SMT_EN);
	//snd_soc_update_bits(codec, RT5616_HP_VOL,
	//	RT5616_L_MUTE | RT5616_R_MUTE, 0);
	msleep(100);
	//snd_soc_update_bits(codec, RT5616_DEPOP_M1,
	//	RT5616_HP_SG_MASK | RT5616_HP_L_SMT_MASK |
	//	RT5616_HP_R_SMT_MASK, RT5616_HP_SG_DIS |
	//	RT5616_HP_L_SMT_DIS | RT5616_HP_R_SMT_DIS);
	// end eddy.qin
	msleep(20);	
	snd_soc_update_bits(codec, RT5616_HP_CALIB_AMP_DET,
		RT5616_HPD_PS_MASK, RT5616_HPD_PS_EN);
}

static void rt5616_pmd_depop(struct snd_soc_codec *codec)
{
	/* headphone mute sequence */
	snd_soc_update_bits(codec, RT5616_DEPOP_M3,
		RT5616_CP_FQ1_MASK | RT5616_CP_FQ2_MASK | RT5616_CP_FQ3_MASK,
		(RT5616_CP_FQ_96_KHZ << RT5616_CP_FQ1_SFT) |
		(RT5616_CP_FQ_12_KHZ << RT5616_CP_FQ2_SFT) |
		(RT5616_CP_FQ_96_KHZ << RT5616_CP_FQ3_SFT));
	rt5616_index_write(codec, RT5616_MAMP_INT_REG2, 0xfc00);
    // 2017.10.24 eddy.qin comment
	//snd_soc_update_bits(codec, RT5616_DEPOP_M1,
	//	RT5616_HP_SG_MASK, RT5616_HP_SG_EN);
	//snd_soc_update_bits(codec, RT5616_DEPOP_M1,
	//	RT5616_RSTP_MASK, RT5616_RSTP_EN);
	//snd_soc_update_bits(codec, RT5616_DEPOP_M1,
	//	RT5616_RSTP_MASK | RT5616_HP_L_SMT_MASK |
	//	RT5616_HP_R_SMT_MASK, RT5616_RSTP_DIS |
	//	RT5616_HP_L_SMT_EN | RT5616_HP_R_SMT_EN);
	snd_soc_update_bits(codec, RT5616_HP_CALIB_AMP_DET,
		RT5616_HPD_PS_MASK, RT5616_HPD_PS_DIS);
	msleep(90);
	//snd_soc_update_bits(codec, RT5616_HP_VOL,
	//	RT5616_L_MUTE | RT5616_R_MUTE, RT5616_L_MUTE | RT5616_R_MUTE);
	// end eddy.qin
	msleep(30);

	hp_amp_power(codec, 0);

}

static ssize_t alc5616_cfg_pow_ctr_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	int cnt = 0;
 
	cnt += sprintf(buf, "%d\n", alc_pwrctr);

    if (cnt >= PAGE_SIZE)
		cnt = PAGE_SIZE - 1;
    return cnt;
}

static ssize_t alc5616_cfg_pow_ctr_store(struct device *dev, struct device_attribute *attr, char *buf, size_t count)
{
	int mode;

	sscanf(buf, "%d", &mode);
	
	if(mode != 0 && mode != 1)
	{
		pr_err("the para is wrong, %d \n", mode);
	}
	{
		alc_pwrctr = mode;	
	}
	return count;
}

static DEVICE_ATTR(alc5616_pow_ctr_cfg, 0644, alc5616_cfg_pow_ctr_show, alc5616_cfg_pow_ctr_store);

static ssize_t alc5616_cfg_pow_save_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	int cnt = 0;
 
	cnt += sprintf(buf, "%d\n", alc_val);

    if (cnt >= PAGE_SIZE)
		cnt = PAGE_SIZE - 1;

    return cnt;
}

static ssize_t alc5616_cfg_pow_save_store(struct device *dev, struct device_attribute *attr, char *buf, size_t count)
{
	int mode;

	sscanf(buf, "%d", &mode);
	
	if(mode != 0 && mode != 1)
	{
		pr_err("the para is wrong, %d \n", mode);	
	}
	{
		alc_val = mode;	
	}

	return count;
}

static DEVICE_ATTR(alc5616_pow_save_cfg, 0644, alc5616_cfg_pow_save_show, alc5616_cfg_pow_save_store);

static int rt5616_hp_event(struct snd_soc_dapm_widget *w, 
	struct snd_kcontrol *kcontrol, int event)
{
	struct snd_soc_codec *codec = w->codec;

	switch (event) {
	case SND_SOC_DAPM_POST_PMU:
		rt5616_pmu_depop(codec);
		//rt5616_index_write(codec, RT5616_LOUT_CTRL1, 0x0808);
		break;

	case SND_SOC_DAPM_PRE_PMD:
		rt5616_pmd_depop(codec);
		//rt5616_index_write(codec, RT5616_LOUT_CTRL1, 0xC8C8);
		break;

	default:
		return 0;
	}

	return 0;
}

static int rt5616_lout_event(struct snd_soc_dapm_widget *w, 
	struct snd_kcontrol *kcontrol, int event)
{
	struct snd_soc_codec *codec = w->codec;

	switch (event) {
	case SND_SOC_DAPM_POST_PMU:
		hp_amp_power(codec,1);
        // 2017.10.24 eddy.qin comment
		//snd_soc_update_bits(codec, RT5616_LOUT_CTRL1,
		//	RT5616_L_MUTE | RT5616_R_MUTE, 0);
		// end eddy.qin
		break;

	case SND_SOC_DAPM_PRE_PMD:
        // 2017.10.24 eddy.qin comment
		//snd_soc_update_bits(codec, RT5616_LOUT_CTRL1,
		//	RT5616_L_MUTE | RT5616_R_MUTE,
		//	RT5616_L_MUTE | RT5616_R_MUTE);
		// end eddy.qin
		hp_amp_power(codec,0);
		break;

	default:
		return 0;
	}

	return 0;
}

static int rt5616_bst1_event(struct snd_soc_dapm_widget *w, 
	struct snd_kcontrol *kcontrol, int event)
{
	struct snd_soc_codec *codec = w->codec;
	
	switch (event) {
	case SND_SOC_DAPM_POST_PMU:
		snd_soc_update_bits(codec, RT5616_PWR_ANLG2,
			RT5616_PWR_BST1_OP2, RT5616_PWR_BST1_OP2);
		break;

	case SND_SOC_DAPM_PRE_PMD:
		snd_soc_update_bits(codec, RT5616_PWR_ANLG2,
			RT5616_PWR_BST1_OP2, 0);
		break;

	default:
		return 0;
	}

	return 0;
}

static int rt5616_bst2_event(struct snd_soc_dapm_widget *w, 
	struct snd_kcontrol *kcontrol, int event)
{
	struct snd_soc_codec *codec = w->codec;
	
	switch (event) {
	case SND_SOC_DAPM_POST_PMU:
		snd_soc_update_bits(codec, RT5616_PWR_ANLG2,
			RT5616_PWR_BST2_OP2, RT5616_PWR_BST2_OP2);
		break;

	case SND_SOC_DAPM_PRE_PMD:
		snd_soc_update_bits(codec, RT5616_PWR_ANLG2,
			RT5616_PWR_BST2_OP2, 0);
		break;

	default:
		return 0;
	}

	return 0;
}

static const struct snd_soc_dapm_widget rt5616_dapm_widgets[] = {
	SND_SOC_DAPM_SUPPLY("PLL1", RT5616_PWR_ANLG2,
			RT5616_PWR_PLL_BIT, 0, NULL, 0),
	/* Input Side */
	/* micbias */
	SND_SOC_DAPM_SUPPLY("LDO", RT5616_PWR_ANLG1,
			RT5616_PWR_LDO_BIT, 0, NULL, 0),
#ifdef POWER_ON_MICBIAS1
	SND_SOC_DAPM_SUPPLY("micbias1", RT5616_PWR_ANLG2,
			RT5616_PWR_MB1_BIT, 0, NULL, 0),
#else
	SND_SOC_DAPM_MICBIAS("micbias1", RT5616_PWR_ANLG2,
			RT5616_PWR_MB1_BIT, 0),
#endif
	/* Input Lines */
	SND_SOC_DAPM_INPUT("MIC1"),
	SND_SOC_DAPM_INPUT("MIC2"),


	SND_SOC_DAPM_INPUT("IN1P"),
	SND_SOC_DAPM_INPUT("IN2P"),
	SND_SOC_DAPM_INPUT("IN2N"),

	/* Boost */
	SND_SOC_DAPM_PGA_E("BST1", RT5616_PWR_ANLG2,
		RT5616_PWR_BST1_BIT, 0, NULL, 0, rt5616_bst1_event,
		SND_SOC_DAPM_PRE_PMD | SND_SOC_DAPM_POST_PMU),
	SND_SOC_DAPM_PGA_E("BST2", RT5616_PWR_ANLG2,
		RT5616_PWR_BST2_BIT, 0, NULL, 0, rt5616_bst2_event,
		SND_SOC_DAPM_PRE_PMD | SND_SOC_DAPM_POST_PMU),
	/* Input Volume */
	SND_SOC_DAPM_PGA("INL1 VOL", RT5616_PWR_VOL,
		RT5616_PWR_IN1_L_BIT, 0, NULL, 0),
	SND_SOC_DAPM_PGA("INR1 VOL", RT5616_PWR_VOL,
		RT5616_PWR_IN1_R_BIT, 0, NULL, 0),
	SND_SOC_DAPM_PGA("INL2 VOL", RT5616_PWR_VOL,
		RT5616_PWR_IN2_L_BIT, 0, NULL, 0),
	SND_SOC_DAPM_PGA("INR2 VOL", RT5616_PWR_VOL,
		RT5616_PWR_IN2_R_BIT, 0, NULL, 0),

	/* REC Mixer */
	SND_SOC_DAPM_MIXER("RECMIXL", RT5616_PWR_MIXER, RT5616_PWR_RM_L_BIT, 0,
			rt5616_rec_l_mix, ARRAY_SIZE(rt5616_rec_l_mix)),
	SND_SOC_DAPM_MIXER("RECMIXR", RT5616_PWR_MIXER, RT5616_PWR_RM_R_BIT, 0,
			rt5616_rec_r_mix, ARRAY_SIZE(rt5616_rec_r_mix)),
	/* ADCs */
	SND_SOC_DAPM_ADC_E("ADC L", NULL, RT5616_PWR_DIG1,
		RT5616_PWR_ADC_L_BIT, 0, rt5616_adc_event,
		SND_SOC_DAPM_POST_PMD | SND_SOC_DAPM_POST_PMU),
	SND_SOC_DAPM_ADC_E("ADC R", NULL, RT5616_PWR_DIG1,
		RT5616_PWR_ADC_R_BIT, 0, rt5616_adc_event,
		SND_SOC_DAPM_POST_PMD | SND_SOC_DAPM_POST_PMU),

	/* ADC Mixer */
	SND_SOC_DAPM_SUPPLY("stereo1 filter", RT5616_PWR_DIG2,
		RT5616_PWR_ADC_STO1_F_BIT, 0, NULL, 0),
	SND_SOC_DAPM_MIXER("Stereo1 ADC MIXL", SND_SOC_NOPM, 0, 0,
		rt5616_sto1_adc_l_mix, ARRAY_SIZE(rt5616_sto1_adc_l_mix)),
	SND_SOC_DAPM_MIXER("Stereo1 ADC MIXR", SND_SOC_NOPM, 0, 0,
		rt5616_sto1_adc_r_mix, ARRAY_SIZE(rt5616_sto1_adc_r_mix)),

	/* Digital Interface */
	SND_SOC_DAPM_SUPPLY("I2S1", RT5616_PWR_DIG1,
		RT5616_PWR_I2S1_BIT, 0, NULL, 0),
	SND_SOC_DAPM_PGA("IF1 DAC", SND_SOC_NOPM, 0, 0, NULL, 0),
	SND_SOC_DAPM_PGA("IF1 DAC1 L", SND_SOC_NOPM, 0, 0, NULL, 0),
	SND_SOC_DAPM_PGA("IF1 DAC1 R", SND_SOC_NOPM, 0, 0, NULL, 0),
	SND_SOC_DAPM_PGA("IF1 ADC1", SND_SOC_NOPM, 0, 0, NULL, 0),

	/* Digital Interface Select */

	/* Audio Interface */
	SND_SOC_DAPM_AIF_IN("AIF1RX", "AIF1 Playback", 0, SND_SOC_NOPM, 0, 0),
	SND_SOC_DAPM_AIF_OUT("AIF1TX", "AIF1 Capture", 0, SND_SOC_NOPM, 0, 0),

	/* Audio DSP */
	SND_SOC_DAPM_PGA("Audio DSP", SND_SOC_NOPM, 0, 0, NULL, 0),

	/* Output Side */
	/* DAC mixer before sound effect  */
	SND_SOC_DAPM_MIXER("DAC MIXL", SND_SOC_NOPM, 0, 0,
		rt5616_dac_l_mix, ARRAY_SIZE(rt5616_dac_l_mix)),
	SND_SOC_DAPM_MIXER("DAC MIXR", SND_SOC_NOPM, 0, 0,
		rt5616_dac_r_mix, ARRAY_SIZE(rt5616_dac_r_mix)),

	SND_SOC_DAPM_SUPPLY("Stero1 DAC Power", RT5616_PWR_DIG2,
			RT5616_PWR_DAC_STO1_F_BIT, 0, NULL, 0),

	/* DAC Mixer */
	SND_SOC_DAPM_MIXER("Stereo DAC MIXL", SND_SOC_NOPM, 0, 0,
		rt5616_sto_dac_l_mix, ARRAY_SIZE(rt5616_sto_dac_l_mix)),
	SND_SOC_DAPM_MIXER("Stereo DAC MIXR", SND_SOC_NOPM, 0, 0,
		rt5616_sto_dac_r_mix, ARRAY_SIZE(rt5616_sto_dac_r_mix)),

	/* DACs */
	SND_SOC_DAPM_DAC("DAC L1", NULL, RT5616_PWR_DIG1,
			RT5616_PWR_DAC_L1_BIT, 0),
	SND_SOC_DAPM_DAC("DAC R1", NULL, RT5616_PWR_DIG1,
			RT5616_PWR_DAC_R1_BIT, 0),
	/* OUT Mixer */
	SND_SOC_DAPM_MIXER("OUT MIXL", RT5616_PWR_MIXER, RT5616_PWR_OM_L_BIT,
		0, rt5616_out_l_mix, ARRAY_SIZE(rt5616_out_l_mix)),
	SND_SOC_DAPM_MIXER("OUT MIXR", RT5616_PWR_MIXER, RT5616_PWR_OM_R_BIT,
		0, rt5616_out_r_mix, ARRAY_SIZE(rt5616_out_r_mix)),
	/* Ouput Volume */
	SND_SOC_DAPM_PGA("OUTVOL L", RT5616_PWR_VOL,
		RT5616_PWR_OV_L_BIT, 0, NULL, 0),
	SND_SOC_DAPM_PGA("OUTVOL R", RT5616_PWR_VOL,
		RT5616_PWR_OV_R_BIT, 0, NULL, 0),
	SND_SOC_DAPM_PGA("HPOVOL L", RT5616_PWR_VOL,
		RT5616_PWR_HV_L_BIT, 0, NULL, 0),
	SND_SOC_DAPM_PGA("HPOVOL R", RT5616_PWR_VOL,
		RT5616_PWR_HV_R_BIT, 0, NULL, 0),
	SND_SOC_DAPM_PGA("DAC 1", SND_SOC_NOPM,
		0, 0, NULL, 0),
	SND_SOC_DAPM_PGA("DAC 2", SND_SOC_NOPM, 
		0, 0, NULL, 0),
	SND_SOC_DAPM_PGA("HPOVOL", SND_SOC_NOPM, 
		0, 0, NULL, 0),
	SND_SOC_DAPM_PGA("INL1", RT5616_PWR_VOL,
		RT5616_PWR_IN1_L_BIT, 0, NULL, 0),
	SND_SOC_DAPM_PGA("INR1", RT5616_PWR_VOL,
		RT5616_PWR_IN1_R_BIT, 0, NULL, 0),
	SND_SOC_DAPM_PGA("INL2", RT5616_PWR_VOL,
		RT5616_PWR_IN2_L_BIT, 0, NULL, 0),
	SND_SOC_DAPM_PGA("INR2", RT5616_PWR_VOL,
		RT5616_PWR_IN2_R_BIT, 0, NULL, 0),
	/* HPO/LOUT/Mono Mixer */
	SND_SOC_DAPM_MIXER("HPO MIX", SND_SOC_NOPM, 0, 0,
		rt5616_hpo_mix, ARRAY_SIZE(rt5616_hpo_mix)),
	SND_SOC_DAPM_MIXER("LOUT MIX", RT5616_PWR_ANLG1, RT5616_PWR_LM_BIT, 0,
		rt5616_lout_mix, ARRAY_SIZE(rt5616_lout_mix)),

	SND_SOC_DAPM_PGA_S("HP amp", 1, SND_SOC_NOPM, 0, 0,
		rt5616_hp_event, SND_SOC_DAPM_PRE_PMD | SND_SOC_DAPM_POST_PMU),
	SND_SOC_DAPM_PGA_S("LOUT amp", 1, SND_SOC_NOPM, 0, 0,
		rt5616_lout_event, SND_SOC_DAPM_PRE_PMD | SND_SOC_DAPM_POST_PMU),

	/* Output Lines */
	SND_SOC_DAPM_OUTPUT("HPOL"),
	SND_SOC_DAPM_OUTPUT("HPOR"),
	SND_SOC_DAPM_OUTPUT("LOUTL"),
	SND_SOC_DAPM_OUTPUT("LOUTR"),
};

static const struct snd_soc_dapm_route rt5616_dapm_routes[] = {
//	{ OUT, VIA, IN},
	{"IN1P", NULL, "LDO"},
	{"IN2P", NULL, "LDO"},

	{"IN1P", NULL, "MIC1"},
	{"IN2P", NULL, "MIC2"},
	{"IN2N", NULL, "MIC2"},

	{"BST1", NULL, "IN1P"},
	{"BST2", NULL, "IN2P"},
	{"BST2", NULL, "IN2N"},
#ifdef POWER_ON_MICBIAS1
	{"BST1", NULL, "micbias1"},
	{"BST2", NULL, "micbias1"},
#endif

	{"INL1 VOL", NULL, "IN2P"},
	{"INR1 VOL", NULL, "IN2N"},
	
	{"RECMIXL", "INL1 Switch", "INL1 VOL"},
	{"RECMIXL", "BST2 Switch", "BST2"},
	{"RECMIXL", "BST1 Switch", "BST1"},

	{"RECMIXR", "INR1 Switch", "INR1 VOL"},
	{"RECMIXR", "BST2 Switch", "BST2"},
	{"RECMIXR", "BST1 Switch", "BST1"},

	{"ADC L", NULL, "RECMIXL"},
	{"ADC R", NULL, "RECMIXR"},

	{"Stereo1 ADC MIXL", "ADC1 Switch", "ADC L"},
	{"Stereo1 ADC MIXL", NULL, "stereo1 filter"},
	{"stereo1 filter", NULL, "PLL1", check_sysclk1_source},

	{"Stereo1 ADC MIXR", "ADC1 Switch", "ADC R"},
	{"Stereo1 ADC MIXR", NULL, "stereo1 filter"},
	{"stereo1 filter", NULL, "PLL1", check_sysclk1_source},

	{"IF1 ADC1", NULL, "Stereo1 ADC MIXL"},
	{"IF1 ADC1", NULL, "Stereo1 ADC MIXR"},
	{"IF1 ADC1", NULL, "I2S1"},

	{"AIF1TX", NULL, "IF1 ADC1"},

	{"IF1 DAC", NULL, "AIF1RX"},
	{"IF1 DAC", NULL, "I2S1"},

	{"IF1 DAC1 L", NULL, "IF1 DAC"},
	{"IF1 DAC1 R", NULL, "IF1 DAC"},
	
	{"DAC MIXL", "Stereo ADC Switch", "Stereo1 ADC MIXL"},
	{"DAC MIXL", "INF1 Switch", "IF1 DAC1 L"},
	{"DAC MIXR", "Stereo ADC Switch", "Stereo1 ADC MIXR"},
	{"DAC MIXR", "INF1 Switch", "IF1 DAC1 R"},

	{"Audio DSP", NULL, "DAC MIXL"},
	{"Audio DSP", NULL, "DAC MIXR"},

	{"Stereo DAC MIXL", "DAC L1 Switch", "Audio DSP"},
	{"Stereo DAC MIXL", "DAC R1 Switch", "DAC MIXR"},
	{"Stereo DAC MIXL", NULL, "Stero1 DAC Power"},
	{"Stereo DAC MIXR", "DAC R1 Switch", "Audio DSP"},
	{"Stereo DAC MIXR", "DAC L1 Switch", "DAC MIXL"},
	{"Stereo DAC MIXR", NULL, "Stero1 DAC Power"},

	{"DAC L1", NULL, "Stereo DAC MIXL"},
	{"DAC L1", NULL, "PLL1", check_sysclk1_source},
	{"DAC R1", NULL, "Stereo DAC MIXR"},
	{"DAC R1", NULL, "PLL1", check_sysclk1_source},

	{"OUT MIXL", "BST1 Switch", "BST1"},
	{"OUT MIXL", "BST2 Switch", "BST2"},
	{"OUT MIXL", "INL1 Switch", "INL1 VOL"},
	{"OUT MIXL", "REC MIXL Switch", "RECMIXL"},
	{"OUT MIXL", "DAC L1 Switch", "DAC L1"},

	{"OUT MIXR", "BST2 Switch", "BST2"},
	{"OUT MIXR", "BST1 Switch", "BST1"},
	{"OUT MIXR", "INR1 Switch", "INR1 VOL"},
	{"OUT MIXR", "REC MIXR Switch", "RECMIXR"},
	{"OUT MIXR", "DAC R1 Switch", "DAC R1"},

	{"HPOVOL L", NULL, "OUT MIXL"},
	{"HPOVOL R", NULL, "OUT MIXR"},
	{"OUTVOL L", NULL, "OUT MIXL"},
	{"OUTVOL R", NULL, "OUT MIXR"},

	{"DAC 1", NULL, "DAC L1"},
	{"DAC 1", NULL, "DAC R1"},
	{"HPOVOL", NULL, "HPOVOL L"},
	{"HPOVOL", NULL, "HPOVOL R"},
	{"HPO MIX", "DAC1 Switch", "DAC 1"},
	{"HPO MIX", "HPVOL Switch", "HPOVOL"},

	{"LOUT MIX", "DAC L1 Switch", "DAC L1"},
	{"LOUT MIX", "DAC R1 Switch", "DAC R1"},
	{"LOUT MIX", "OUTVOL L Switch", "OUTVOL L"},
	{"LOUT MIX", "OUTVOL R Switch", "OUTVOL R"},
	
	{"HP amp", NULL, "HPO MIX"},
	{"HPOL", NULL, "HP amp"},
	{"HPOR", NULL, "HP amp"},

	{"LOUT amp", NULL, "LOUT MIX"},
	{"LOUTL", NULL, "LOUT amp"},
	{"LOUTR", NULL, "LOUT amp"},

};

static int get_clk_info(int sclk, int rate)
{
	int i, pd[] = {1, 2, 3, 4, 6, 8, 12, 16};

	pr_err("sclk: %d, rate: %d\n", sclk, rate);
	if (sclk <= 0 || rate <= 0)
		return -EINVAL;

	rate = rate << 8;
	for (i = 0; i < ARRAY_SIZE(pd); i++)
		if (sclk == rate * pd[i])
			return i;
	return -EINVAL;
}

static int rt5616_hw_params(struct snd_pcm_substream *substream,
	struct snd_pcm_hw_params *params, struct snd_soc_dai *dai)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_codec *codec = rtd->codec;
	struct rt5616_priv *rt5616 = snd_soc_codec_get_drvdata(codec);
	unsigned int val_len = 0, val_clk, mask_clk;
	int pre_div, bclk_ms, frame_size;

    rt5616->sysclk = 2048000;
	rt5616->lrck[dai->id] = params_rate(params);

#ifdef CONFIG_QUECTEL_PCM16K_SUPPORT	//for pcm 16k , pll value is diff
	if(params_rate(params) == 16000)
	{
		rt5616->sysclk = 4096000;
	}
	else
#endif
	{
		rt5616->sysclk = 2048000;
	}

	pre_div = get_clk_info(rt5616->sysclk, rt5616->lrck[dai->id]);

	if (pre_div < 0) {
		dev_err(codec->dev, "Unsupported clock setting\n");
		return -EINVAL;
	}
	frame_size = snd_soc_params_to_frame_size(params);
	if (frame_size < 0) {
		dev_err(codec->dev, "Unsupported frame size: %d\n", frame_size);
		return -EINVAL;
	}
	bclk_ms = frame_size > 32 ? 1 : 0;
	rt5616->bclk[dai->id] = rt5616->lrck[dai->id] * (32 << bclk_ms);

	dev_dbg(dai->dev, "bclk is %dHz and lrck is %dHz\n",
		rt5616->bclk[dai->id], rt5616->lrck[dai->id]);
	dev_dbg(dai->dev, "bclk_ms is %d and pre_div is %d for iis %d\n",
				bclk_ms, pre_div, dai->id);

	switch (params_format(params)) {
	case SNDRV_PCM_FORMAT_S16_LE:
		break;
	case SNDRV_PCM_FORMAT_S20_3LE:
		val_len |= RT5616_I2S_DL_20;
		break;
	case SNDRV_PCM_FORMAT_S24_LE:
		val_len |= RT5616_I2S_DL_24;
		break;
	case SNDRV_PCM_FORMAT_S8:
		val_len |= RT5616_I2S_DL_8;
		break;
	default:
		return -EINVAL;
	}

	mask_clk = RT5616_I2S_PD1_MASK;
	val_clk = pre_div << RT5616_I2S_PD1_SFT;
	snd_soc_update_bits(codec, RT5616_I2S1_SDP,
		RT5616_I2S_DL_MASK, val_len);
	snd_soc_update_bits(codec, RT5616_ADDA_CLK1, mask_clk, val_clk);

	return 0;
}

static int rt5616_prepare(struct snd_pcm_substream *substream,
				struct snd_soc_dai *dai)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_codec *codec = rtd->codec;
	struct rt5616_priv *rt5616 = snd_soc_codec_get_drvdata(codec);

	rt5616->aif_pu = dai->id;
	return 0;
}

static int rt5616_set_dai_fmt(struct snd_soc_dai *dai, unsigned int fmt)
{
	struct snd_soc_codec *codec = dai->codec;
	struct rt5616_priv *rt5616 = snd_soc_codec_get_drvdata(codec);
	unsigned int reg_val = 0;

	switch (fmt & SND_SOC_DAIFMT_MASTER_MASK) {
	case SND_SOC_DAIFMT_CBM_CFM:
		rt5616->master[dai->id] = 1;
		break;
	case SND_SOC_DAIFMT_CBS_CFS:
		reg_val |= RT5616_I2S_MS_S;
		rt5616->master[dai->id] = 0;
		break;
	default:
		return -EINVAL;
	}

	switch (fmt & SND_SOC_DAIFMT_INV_MASK) {
	case SND_SOC_DAIFMT_NB_NF:
		break;
	case SND_SOC_DAIFMT_IB_NF:
		reg_val |= RT5616_I2S_BP_INV;
		break;
	default:
		return -EINVAL;
	}

	switch (fmt & SND_SOC_DAIFMT_FORMAT_MASK) {
	case SND_SOC_DAIFMT_I2S:
		break;
	case SND_SOC_DAIFMT_LEFT_J:
		reg_val |= RT5616_I2S_DF_LEFT;
		break;
	case SND_SOC_DAIFMT_DSP_A:
		reg_val |= RT5616_I2S_DF_PCM_A;
		break;
	case SND_SOC_DAIFMT_DSP_B:
		reg_val |= RT5616_I2S_DF_PCM_B;
		break;
	default:
		return -EINVAL;
	}

	snd_soc_update_bits(codec, RT5616_I2S1_SDP,
		RT5616_I2S_MS_MASK | RT5616_I2S_BP_MASK |
		RT5616_I2S_DF_MASK, reg_val);


	return 0;
}

static int rt5616_set_dai_sysclk(struct snd_soc_dai *dai,
		int clk_id, unsigned int freq, int dir)
{
	struct snd_soc_codec *codec = dai->codec;
	struct rt5616_priv *rt5616 = snd_soc_codec_get_drvdata(codec);
	unsigned int reg_val = 0;

	if (freq == rt5616->sysclk && clk_id == rt5616->sysclk_src)
		return 0;

	switch (clk_id) {
	case RT5616_SCLK_S_MCLK:
		reg_val |= RT5616_SCLK_SRC_MCLK;
		break;
	case RT5616_SCLK_S_PLL1:
		reg_val |= RT5616_SCLK_SRC_PLL1;
		break;
	default:
		dev_err(codec->dev, "Invalid clock id (%d)\n", clk_id);
		return -EINVAL;
	}
	snd_soc_update_bits(codec, RT5616_GLB_CLK,
		RT5616_SCLK_SRC_MASK, reg_val);
	rt5616->sysclk = freq;
	rt5616->sysclk_src = clk_id;

	dev_dbg(dai->dev, "Sysclk is %dHz and clock id is %d\n", freq, clk_id);

	return 0;
}

/**
 * rt5616_pll_calc - Calcualte PLL M/N/K code.
 * @freq_in: external clock provided to codec.
 * @freq_out: target clock which codec works on.
 * @pll_code: Pointer to structure with M, N, K and bypass flag.
 *
 * Calcualte M/N/K code to configure PLL for codec. And K is assigned to 2
 * which make calculation more efficiently.
 *
 * Returns 0 for success or negative error code.
 */
static int rt5616_pll_calc(const unsigned int freq_in,
	const unsigned int freq_out, struct rt5616_pll_code *pll_code)
{
	int max_n = RT5616_PLL_N_MAX, max_m = RT5616_PLL_M_MAX;
	int n = 0, m, red, n_t, m_t, in_t, out_t, red_t = abs(freq_out - freq_in);
	bool bypass = false;

	if (RT5616_PLL_INP_MAX < freq_in || RT5616_PLL_INP_MIN > freq_in)
		return -EINVAL;

	for (n_t = 0; n_t <= max_n; n_t++) {
		in_t = (freq_in >> 1) + (freq_in >> 2) * n_t;
		if (in_t < 0)
			continue;
		if (in_t == freq_out) {
			bypass = true;
			n = n_t;
			goto code_find;
		}
		for (m_t = 0; m_t <= max_m; m_t++) {
			out_t = in_t / (m_t + 2);
			red = abs(out_t - freq_out);
			if (red < red_t) {
				n = n_t;
				m = m_t;
				if (red == 0)
					goto code_find;
				red_t = red;
			}
		}
	}
	pr_debug("Only get approximation about PLL\n");

code_find:

	pll_code->m_bp = bypass;
	pll_code->m_code = m;
	pll_code->n_code = n;
	pll_code->k_code = 2;
	return 0;
}

static int rt5616_set_dai_pll(struct snd_soc_dai *dai, int pll_id, int source,
			unsigned int freq_in, unsigned int freq_out)
{
	struct snd_soc_codec *codec = dai->codec;
	struct rt5616_priv *rt5616 = snd_soc_codec_get_drvdata(codec);
	struct rt5616_pll_code pll_code;
	int ret;

	if (source == rt5616->pll_src && freq_in == rt5616->pll_in &&
	    freq_out == rt5616->pll_out)
		return 0;

	if (!freq_in || !freq_out) {
		dev_dbg(codec->dev, "PLL disabled\n");

		rt5616->pll_in = 0;
		rt5616->pll_out = 0;
		snd_soc_update_bits(codec, RT5616_GLB_CLK,
			RT5616_SCLK_SRC_MASK, RT5616_SCLK_SRC_MCLK);
		return 0;
	}

	switch (source) {
	case RT5616_PLL1_S_MCLK:
		snd_soc_update_bits(codec, RT5616_GLB_CLK,
			RT5616_PLL1_SRC_MASK, RT5616_PLL1_SRC_MCLK);
		break;
	case RT5616_PLL1_S_BCLK1:
	case RT5616_PLL1_S_BCLK2:

		snd_soc_update_bits(codec, RT5616_GLB_CLK,
			RT5616_PLL1_SRC_MASK, RT5616_PLL1_SRC_BCLK1);


		break;
	default:
		dev_err(codec->dev, "Unknown PLL source %d\n", source);
		return -EINVAL;
	}

	ret = rt5616_pll_calc(freq_in, freq_out, &pll_code);
	if (ret < 0) {
		dev_err(codec->dev, "Unsupport input clock %d\n", freq_in);
		return ret;
	}

	dev_dbg(codec->dev, "bypass=%d m=%d n=%d k=2\n", pll_code.m_bp,
		(pll_code.m_bp ? 0 : pll_code.m_code), pll_code.n_code);

	snd_soc_write(codec, RT5616_PLL_CTRL1,
		pll_code.n_code << RT5616_PLL_N_SFT | pll_code.k_code);
	snd_soc_write(codec, RT5616_PLL_CTRL2,
		(pll_code.m_bp ? 0 : pll_code.m_code) << RT5616_PLL_M_SFT |
		pll_code.m_bp << RT5616_PLL_M_BP_SFT);

	rt5616->pll_in = freq_in;
	rt5616->pll_out = freq_out;
	rt5616->pll_src = source;

	return 0;
}


typedef enum {
	CODEC_CONFIG_DL_GAIN,
	CODEC_CONFIG_UL_GAIN,
	CODEC_CONFIG_MAX,
} codec_config_type;

static int s_gain_val[CODEC_CONFIG_MAX] = {79, 73};

static bool update_alc5616_dl_gain(struct snd_soc_codec *codec, int val)
{
	int bits_val;

	if (val < 0 || val > 100)
		return false;

	s_gain_val[CODEC_CONFIG_DL_GAIN] = val;

	bits_val = val * 0xAF / 100;
	snd_soc_update_bits(codec, 0x19, (0xFF)|(0xFF<<8), (bits_val)|(bits_val<<8));

        bits_val = (100 - val) * 0x27 / 100;
        snd_soc_update_bits(codec, 0x02, 0x3F|(0x3F<<8), bits_val|(bits_val<<8));
        snd_soc_update_bits(codec, 0x03, 0x3F|(0x3F<<8), bits_val|(bits_val<<8));

	return true;
}

static bool update_alc5616_ul_gain(struct snd_soc_codec *codec, int val)
{
	int bits_val;

	if (val < 0 || val > 100)
                return false;

        s_gain_val[CODEC_CONFIG_UL_GAIN] = val;

	bits_val = val * 0x7F / 100;
        snd_soc_update_bits(codec, 0x1C, 0x7F|(0x7F<<8), bits_val|(bits_val<<8));
   
	return true;
}

static bool update_codec_config(codec_config_type type, struct snd_soc_codec *codec, int val)
{
	bool ret = false;
 
	switch (type)
	{
	case CODEC_CONFIG_DL_GAIN:
		ret = update_alc5616_dl_gain(codec, val);
		break;

        case CODEC_CONFIG_UL_GAIN:
                ret = update_alc5616_ul_gain(codec, val);
                break;

	default:
		break;
	}

	return ret;
}

static bool hex_str_to_num(char* buf, size_t count, u32 *val)
{
	int i = 0;
	unsigned long tmp = 0;
	
	for (i=0; i<count; i++)
	{
		if (buf[i] == 0 || buf[i] == '\r' || buf[i] == '\n')
			break;

		if (buf[i] == 'x' || buf[i] == 'X' || buf[i] == ' ' || buf[i] == '\t')
			continue;

		if ((buf[i]>='0') && (buf[i]<='9'))
			tmp = tmp*16 + buf[i] - '0';
		else if ((buf[i]>='a') && (buf[i]<='f'))
                        tmp = tmp*16 + buf[i] - 'a' + 0xA;
                else if ((buf[i]>='A') && (buf[i]<='F'))
                        tmp = tmp*16 + buf[i] - 'A' + 0xA;
		else
			return false;
	}

	*val = tmp;
	return true;
}

static ssize_t alc5616_cfg_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	int cnt = 0;
 
	cnt += sprintf(buf, "0x%x,0x%x\n", CODEC_CONFIG_DL_GAIN, s_gain_val[CODEC_CONFIG_DL_GAIN]);
        cnt += sprintf(buf+cnt, "0x%x,0x%x\n", CODEC_CONFIG_UL_GAIN, s_gain_val[CODEC_CONFIG_UL_GAIN]);

        if (cnt >= PAGE_SIZE)
                cnt = PAGE_SIZE - 1;

        return cnt;
}

static ssize_t alc5616_cfg_store(struct device *dev, struct device_attribute *attr, char *buf, size_t count)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct rt5616_priv *rt5616 = i2c_get_clientdata(client);
	struct snd_soc_codec *codec = rt5616->codec;

	char* type_str;
	char* val_str; 
	char* b = strim(buf);
	int type, val;

	if (!b)
		return -EINVAL;

	type_str = strsep(&b, ",");
	if (!type_str || !b)
		return -EINVAL;
	if (!hex_str_to_num(type_str, strlen(type_str), &type))
		return -EINVAL;

        val_str = strsep(&b, ",");
	if (!val_str)
		return -EINVAL;
        if (!hex_str_to_num(val_str, strlen(val_str), &val))
                return -EINVAL;

	if (type < CODEC_CONFIG_DL_GAIN || type >= CODEC_CONFIG_MAX)
		return -EINVAL;

	if (!update_codec_config((codec_config_type)type,codec, val))
		return -EINVAL;

	return count;
}

static DEVICE_ATTR(alc5616_cfg, 0644, alc5616_cfg_show, alc5616_cfg_store);

/**
 * rt5616_index_show - Dump private registers.
 * @dev: codec device.
 * @attr: device attribute.
 * @buf: buffer for display.
 *
 * To show non-zero values of all private registers.
 *
 * Returns buffer length.
 */
static ssize_t rt5616_index_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct rt5616_priv *rt5616 = i2c_get_clientdata(client);
	struct snd_soc_codec *codec = rt5616->codec;
	unsigned int val;
	int cnt = 0, i;

	cnt += sprintf(buf, "RT5616 index register\n");
	for (i = 0; i < 0xb4; i++) {
		if (cnt + RT5616_REG_DISP_LEN >= PAGE_SIZE)
			break;
		val = rt5616_index_read(codec, i);
		if (!val)
			continue;
		cnt += snprintf(buf + cnt, RT5616_REG_DISP_LEN,
				"%02x: %04x\n", i, val);
	}

	if (cnt >= PAGE_SIZE)
		cnt = PAGE_SIZE - 1;

	return cnt;
}
static DEVICE_ATTR(index_reg, 0444, rt5616_index_show, NULL);

static int rt5616_set_bias_level(struct snd_soc_codec *codec,
			enum snd_soc_bias_level level)
{
	switch (level) {
	case SND_SOC_BIAS_ON:
		//add by scott.hu
		snd_soc_update_bits(codec, RT5616_PWR_ANLG2,
				RT5616_PWR_MB1, RT5616_PWR_MB1);
		break;

	case SND_SOC_BIAS_PREPARE:
		break;

	case SND_SOC_BIAS_STANDBY:		
		if (SND_SOC_BIAS_OFF == codec->dapm.bias_level) {
			snd_soc_update_bits(codec, RT5616_PWR_ANLG1,
				RT5616_PWR_VREF1 | RT5616_PWR_MB |
				RT5616_PWR_BG | RT5616_PWR_VREF2,
				RT5616_PWR_VREF1 | RT5616_PWR_MB |
				RT5616_PWR_BG | RT5616_PWR_VREF2);
			msleep(10);
			snd_soc_update_bits(codec, RT5616_PWR_ANLG1,
				RT5616_PWR_FV1 | RT5616_PWR_FV2,
				RT5616_PWR_FV1 | RT5616_PWR_FV2);
			//codec->cache_only = false;
			// 2017.10.24 eddy.qin removed
			//codec->cache_sync = 1;
			//snd_soc_cache_sync(codec);
			// end eddy.qin
			rt5616_index_sync(codec);
			snd_soc_write(codec, RT5616_D_MISC, 0x0011);
			
			//add by scott.hu
			snd_soc_update_bits(codec, RT5616_PWR_ANLG2,
					RT5616_PWR_MB1, 0);
		}
		break;

	case SND_SOC_BIAS_OFF:
		snd_soc_write(codec, RT5616_D_MISC, 0x0010);
		snd_soc_write(codec, RT5616_PWR_DIG1, 0x0000);
		snd_soc_write(codec, RT5616_PWR_DIG2, 0x0000);
		snd_soc_write(codec, RT5616_PWR_VOL, 0x0000);
		snd_soc_write(codec, RT5616_PWR_MIXER, 0x0000);
        // 2017.10.24 eddy.qin changed
        snd_soc_update_bits(codec, RT5616_PWR_ANLG1, 0xB8D0, 0);
        snd_soc_update_bits(codec, RT5616_PWR_ANLG2, 0xCA06, 0);
        // end eddy.qin
		break;

	default:
		break;
	}
	codec->dapm.bias_level = level;

	return 0;
}

static struct regmap *rt5616_get_regmap(struct device *dev)
{
	struct rt5616_priv *rt5616 = dev_get_drvdata(dev);

	return rt5616->regmap;
}

static int rt5616_probe(struct snd_soc_codec *codec)
{
	struct rt5616_priv *rt5616 = snd_soc_codec_get_drvdata(codec);
	int ret;
	printk("enter %s\n",__func__);

	rt5616_reset(codec);
	snd_soc_update_bits(codec, RT5616_PWR_ANLG1,
		RT5616_PWR_VREF1 | RT5616_PWR_MB |
		RT5616_PWR_BG | RT5616_PWR_VREF2,
		RT5616_PWR_VREF1 | RT5616_PWR_MB |
		RT5616_PWR_BG | RT5616_PWR_VREF2);
	msleep(10);
	snd_soc_update_bits(codec, RT5616_PWR_ANLG1,
		RT5616_PWR_FV1 | RT5616_PWR_FV2,
		RT5616_PWR_FV1 | RT5616_PWR_FV2);

	rt5616_reg_init(codec);
	snd_soc_update_bits(codec, RT5616_PWR_ANLG1,
		RT5616_PWR_LDO_DVO_MASK, RT5616_PWR_LDO_DVO_1_2V);

	codec->dapm.bias_level = SND_SOC_BIAS_STANDBY;
	rt5616->codec = codec;

	ret = device_create_file(codec->dev, &dev_attr_index_reg);
	if (ret != 0) {
		dev_err(codec->dev,
			"Failed to create index_reg sysfs files: %d\n", ret);
		return ret;
	}

    ret = device_create_file(codec->dev, &dev_attr_alc5616_cfg);
    if (ret != 0) {
        dev_err(codec->dev,
        "Failed to create index_reg sysfs files: %d\n", ret);
        return ret;
    }

    ret = device_create_file(codec->dev, &dev_attr_alc5616_pow_save_cfg);
    if (ret != 0) {
        dev_err(codec->dev,
        "Failed to create index_reg sysfs files: %d\n", ret);
        return ret;
    }

    ret = device_create_file(codec->dev, &dev_attr_alc5616_pow_ctr_cfg);
    if (ret != 0) {
        dev_err(codec->dev,
        "Failed to create index_reg sysfs files: %d\n", ret);
        return ret;
    }

	return 0;
}

static int rt5616_remove(struct snd_soc_codec *codec)
{
	rt5616_set_bias_level(codec, SND_SOC_BIAS_OFF);
	return 0;
}

#ifdef CONFIG_PM
static int rt5616_suspend(struct snd_soc_codec *codec, pm_message_t state)
{
	//rt5616_set_bias_level(codec, SND_SOC_BIAS_OFF);
	return 0;
}

static int rt5616_resume(struct snd_soc_codec *codec)
{
	//rt5616_set_bias_level(codec, SND_SOC_BIAS_STANDBY);
	return 0;
}
#else
#define rt5616_suspend NULL
#define rt5616_resume NULL
#endif

#define RT5616_STEREO_RATES SNDRV_PCM_RATE_8000_96000
#define RT5616_FORMATS (SNDRV_PCM_FMTBIT_S16_LE | SNDRV_PCM_FMTBIT_S20_3LE | \
			SNDRV_PCM_FMTBIT_S24_LE | SNDRV_PCM_FMTBIT_S8)

struct snd_soc_dai_ops rt5616_aif_dai_ops = {
	.hw_params = rt5616_hw_params,
	.prepare = rt5616_prepare,
	.set_fmt = rt5616_set_dai_fmt,
	.set_sysclk = rt5616_set_dai_sysclk,
	.set_pll = rt5616_set_dai_pll,
};

struct snd_soc_dai_driver rt5616_dai[] = {
	{
		.name = "rt5616-aif1",
		.id = RT5616_AIF1,
		.playback = {
			.stream_name = "AIF1 Playback",
			.channels_min = 1,
			.channels_max = 2,
			.rates = RT5616_STEREO_RATES,
			.formats = RT5616_FORMATS,
		},
		.capture = {
			.stream_name = "AIF1 Capture",
			.channels_min = 1,
			.channels_max = 2,
			.rates = RT5616_STEREO_RATES,
			.formats = RT5616_FORMATS,
		},
		.ops = &rt5616_aif_dai_ops,
	},
};

static struct snd_soc_codec_driver soc_codec_dev_rt5616 = {
	.probe = rt5616_probe,
	.remove = rt5616_remove,
	.suspend = rt5616_suspend,
	.resume = rt5616_resume,
	.set_bias_level = rt5616_set_bias_level,
	.reg_cache_size = RT5616_DEVICE_ID + 1,
	.reg_word_size = sizeof(u16),
	.reg_cache_default = rt5616_reg,
	.volatile_register = rt5616_volatile_register,
	.readable_register = rt5616_readable_register,
	.reg_cache_step = 1,
	.get_regmap = rt5616_get_regmap,
    
    .controls = rt5616_snd_controls,
    .num_controls = ARRAY_SIZE(rt5616_snd_controls),
    .dapm_widgets = rt5616_dapm_widgets,
    .num_dapm_widgets = ARRAY_SIZE(rt5616_dapm_widgets),
    .dapm_routes = rt5616_dapm_routes,
    .num_dapm_routes = ARRAY_SIZE(rt5616_dapm_routes),
};

static const struct regmap_config rt5616_regmap = {
	.reg_bits = 8,
	.val_bits = 16,
	.reg_stride = 1,

	.max_register = RT5616_VENDOR_ID,
	// 2017.10.24 eddy.qin changed
	//.cache_type = REGCACHE_RBTREE,
	.cache_type = REGCACHE_NONE,
	// end eddy.qin
};

extern void quec_set_codec_info(const char* codec_name, const char* dai_name);
static int rt5616_i2c_probe(struct i2c_client *i2c,
		    const struct i2c_device_id *id)
{
	struct rt5616_priv *rt5616;
	int ret;
	unsigned int val;
	printk("enter %s\n",__func__);

	rt5616 = kzalloc(sizeof(struct rt5616_priv), GFP_KERNEL);
	if (NULL == rt5616)
		return -ENOMEM;
	
	rt5616->regmap = devm_regmap_init_i2c(i2c, &rt5616_regmap);
	if (IS_ERR(rt5616->regmap)) {
		ret = PTR_ERR(rt5616->regmap);
		dev_err(&i2c->dev, "Failed to initialise I/O: %d\n", ret);
		return ret;
	}
	//achang.zhang-2018/05/11:Add for codec compatible dynamically (start)
	regcache_cache_bypass(rt5616->regmap, true);
	ret = regmap_read(rt5616->regmap, RT5616_VENDOR_ID, &val);
	if((ret < 0) || (val != 0x10EC)) {
		dev_err(&i2c->dev, "Failed to read device ID, %x != 0x10EC\n", val);
		kfree(rt5616);
		return -EINVAL;
	} else {
		quec_set_codec_info("alc5616-codec.2-001b", "rt5616-aif1");	
	}
	regcache_cache_bypass(rt5616->regmap, false);
	//achang.zhang-2018/05/11:Add for codec compatible dynamically (end)

	i2c_set_clientdata(i2c, rt5616);

	ret = snd_soc_register_codec(&i2c->dev, &soc_codec_dev_rt5616,
			rt5616_dai, ARRAY_SIZE(rt5616_dai));
	if (ret < 0)
		kfree(rt5616);

	return ret;
}

static int rt5616_i2c_remove(struct i2c_client *i2c)
{
	snd_soc_unregister_codec(&i2c->dev);
	kfree(i2c_get_clientdata(i2c));
	return 0;
}

static void rt5616_i2c_shutdown(struct i2c_client *client)
{
	struct rt5616_priv *rt5616 = i2c_get_clientdata(client);
	struct snd_soc_codec *codec = rt5616->codec;

	if (codec != NULL)
		rt5616_set_bias_level(codec, SND_SOC_BIAS_OFF);
}

static const struct of_device_id alc5616_of_match[] = {
	{ .compatible = "quec,quec-alc5616-i2c", },
	{ }
};
MODULE_DEVICE_TABLE(of, alc5616_of_match);

static const struct i2c_device_id rt5616_i2c_id[] = {
	{ ALC5616_DEV_NAME, 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, rt5616_i2c_id);

struct i2c_driver rt5616_i2c_driver = {
	.driver = {
		.name = ALC5616_DEV_NAME,
		.owner = THIS_MODULE,	
		.of_match_table = of_match_ptr(alc5616_of_match),
	},
	.probe = rt5616_i2c_probe,
	.remove   = rt5616_i2c_remove,
	.shutdown = rt5616_i2c_shutdown,
	.id_table = rt5616_i2c_id,
};

static int __init rt5616_modinit(void)
{
	printk("enter %s\n",__func__);
	return i2c_add_driver(&rt5616_i2c_driver);
}
module_init(rt5616_modinit);

static void __exit rt5616_modexit(void)
{
	i2c_del_driver(&rt5616_i2c_driver);
}
module_exit(rt5616_modexit);

MODULE_DESCRIPTION("ASoC RT5616 driver");
MODULE_AUTHOR("Johnny Hsu <johnnyhsu@realtek.com>");
MODULE_LICENSE("GPL");
