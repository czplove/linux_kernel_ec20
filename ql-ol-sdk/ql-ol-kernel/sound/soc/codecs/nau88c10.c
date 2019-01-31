/*
 * nau88c10.c  --  NAU88c10 ALSA Soc Audio driver
 *
 * Copyright 2006 Wolfson Microelectronics PLC.
 *
 * Author: Liam Girdwood <lrg@slimlogic.co.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/pm.h>
#include <linux/i2c.h>
#include <linux/platform_device.h>
#include <linux/spi/spi.h>
#include <linux/slab.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>
#include <sound/initval.h>

#include "nau88c10.h"

/*
 * nau88c10 register cache
 * We can't read the NAU88c10 register space when we are
 * using 2 wire for device control, so we cache them instead.
 */
static const u16 nau88c10_reg[NAU88C10_CACHEREGNUM] = {
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0050, 0x0000, 0x0140, 0x0000,
	0x0000, 0x0000, 0x0000, 0x00ff,
	0x0000, 0x0000, 0x0100, 0x00ff,
	0x0000, 0x0000, 0x012c, 0x002c,
	0x002c, 0x002c, 0x002c, 0x0000,
	0x0032, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0038, 0x000b, 0x0032, 0x0000,
	0x0008, 0x000c, 0x0093, 0x00e9,
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0003, 0x0010, 0x0000, 0x0000,
	0x0000, 0x0002, 0x0001, 0x0000,
	0x0000, 0x0000, 0x0039, 0x0000,
	0x0001,
};

#define NAU88c10_POWER1_BIASEN  0x08
#define NAU88c10_POWER1_BUFIOEN 0x10

static u16 init_reg[][2] = {	
	{NAU88c10_POWER1,   			0x011d},
	{NAU88c10_POWER2,   			0x0015},
	{NAU88c10_POWER3,   			0x00ED},
	{NAU88c10_IFACE,    			0x0118},
	{NAU88c10_CLOCK1,    			0x000C},
	{NAU88c10_DAC,	 			0x0008},
	{NAU88c10_ADC,				0x0108},
	{NAU88c10_INPUT,			0x0003},
	{NAU88c10_INPPGA,			0x0010},
	{NAU88c10_SPKMIX,			0x0001},
	{NAU88c10_MONOMIX,			0x0001},
	{NAU88c10_ADCBOOST,		0x0000},
};

#define SET_CODEC_REG_INIT_NUM  ARRAY_SIZE(init_reg)

/*
 * read nau88c10 register cache
 */
static inline unsigned int nau88c10_read_reg_cache(struct snd_soc_codec  *codec,
  unsigned int reg)
{
	u16 *cache = codec->reg_cache;

	if (reg == NAU88c10_RESET)
	{
		return 0;
	}
	if (reg >= NAU88C10_CACHEREGNUM)
	{
		return -1;
	}
	return cache[reg];
}

/*
 * write to the NAU88c10 register space
 */
static int nau88c10_write(struct snd_soc_codec  *codec, unsigned int reg,
  unsigned int value)
{
	u8 data[2];
	int ret = 0;

	/* data is
	*   D15..D9 NAU88c10 register offset
	*   D8...D0 register data
	*/
	data[0] = (reg << 1) | ((value >> 8) & 0x0001);
	data[1] = value & 0x00ff;

	ret = snd_soc_write(codec, data[0], data[1]);

	printk(KERN_DEBUG "func: %s, reg: 0x%08x, value: 0x%08x, ret: %d\n", __FUNCTION__, reg, value, ret);
}

/*
 * write to the NAU88c10 register space
 */
static int nau88c10_read(struct snd_soc_codec  *codec, unsigned int reg)
{
	int ret = 0;

	reg = (reg << 1);
	ret = snd_soc_read(codec, reg);

	printk(KERN_DEBUG "func: %s, reg: 0x%08x, ret: %d\n", __FUNCTION__, reg, ret);
	if(ret >= 0)
	{
		return ret;
	}
	else
	{
		return 0;
	}
}

/* codec private data */
struct nau88c10_priv {
	struct snd_soc_codec *codec;
	unsigned int f_pllout;
	unsigned int f_mclk;
	unsigned int f_256fs;
	unsigned int f_opclk;
	int mclk_idx;
	enum nau88c10_sysclk_src sysclk;
	u16 reg_cache[NAU88C10_CACHEREGNUM];
	
	struct regmap *regmap;
};


#define nau88c10_reset(c)	   nau88c10_write(c, NAU88c10_RESET, 0)

//2010.8.18 snd_soc_put_volsw_with_update
int update(struct snd_kcontrol *kcontrol, struct snd_ctl_elem_value *ucontrol)
{
	struct soc_mixer_control *mc =
		(struct soc_mixer_control *)kcontrol->private_value;
	struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);
	unsigned int reg = mc->reg;
	unsigned int shift = mc->shift;
	unsigned int rshift = mc->rshift;
	int max = mc->max;
	unsigned int mask = (1 << fls(max)) - 1;
	unsigned int invert = mc->invert;
	unsigned int val, val2, val_mask;
//snd_soc_update_bits_locked declare
	int change;
	unsigned int old, new;
//snd_soc_update_bits_locked declare end //

    printk(KERN_DEBUG "+%s\n", __FUNCTION__);

	val = (ucontrol->value.integer.value[0] & mask);
	if (invert)
		val = max - val;
	val_mask = mask << shift;
	val = val << shift;
	if (shift != rshift) {
		val2 = (ucontrol->value.integer.value[1] & mask);
		if (invert)
			val2 = max - val2;
		val_mask |= mask << rshift;
		val |= val2 << rshift;
	}
//return snd_soc_update_bits_locked(codec, reg, val_mask, val);
	old = nau88c10_read(codec, reg);
	new = (old & ~mask) | val;
	change = old != new;
  new |= 0x100; //add update bit for  b,c,2d,2e,34,35,36,37
	if (change)
		nau88c10_write(codec, reg, new);
	
    printk(KERN_DEBUG "-%s\n", __FUNCTION__);

	return change;
}

static const char *nau88c10_companding[] = { "Off", "NC", "u-law", "A-law" };
static const char *nau88c10_deemp[] = { "None", "32kHz", "44.1kHz", "48kHz" };
static const char *nau88c10_alc[] = { "ALC", "Limiter" };

static const struct soc_enum nau88c10_enum[] = {
	SOC_ENUM_SINGLE(NAU88c10_COMP, 1, 4, nau88c10_companding), /* adc */
	SOC_ENUM_SINGLE(NAU88c10_COMP, 3, 4, nau88c10_companding), /* dac */
	SOC_ENUM_SINGLE(NAU88c10_DAC,  4, 4, nau88c10_deemp),
	SOC_ENUM_SINGLE(NAU88c10_ALC3,  8, 2, nau88c10_alc),
};

static const struct snd_kcontrol_new nau88c10_snd_controls[] = {

SOC_SINGLE("Digital Loopback Switch", NAU88c10_COMP, 0, 1, 0),

SOC_ENUM("DAC Companding", nau88c10_enum[1]),
SOC_ENUM("ADC Companding", nau88c10_enum[0]),

SOC_ENUM("Playback De-emphasis", nau88c10_enum[2]),
SOC_SINGLE("DAC Inversion Switch", NAU88c10_DAC, 0, 1, 0),

SOC_SINGLE("Master Playback Volume", NAU88c10_DACVOL, 0, 127, 0),

SOC_SINGLE("High Pass Filter Switch", NAU88c10_ADC, 8, 1, 0),
SOC_SINGLE("High Pass Cut Off", NAU88c10_ADC, 4, 7, 0),
SOC_SINGLE("ADC Inversion Switch", NAU88c10_COMP, 0, 1, 0),

SOC_SINGLE("Capture Volume", NAU88c10_ADCVOL,  0, 127, 0),

SOC_SINGLE("DAC Playback Limiter Switch", NAU88c10_DACLIM1,  8, 1, 0),
SOC_SINGLE("DAC Playback Limiter Decay", NAU88c10_DACLIM1,  4, 15, 0),
SOC_SINGLE("DAC Playback Limiter Attack", NAU88c10_DACLIM1,  0, 15, 0),

SOC_SINGLE("DAC Playback Limiter Threshold", NAU88c10_DACLIM2,  4, 7, 0),
SOC_SINGLE("DAC Playback Limiter Boost", NAU88c10_DACLIM2,  0, 15, 0),

SOC_SINGLE("ALC Enable Switch", NAU88c10_ALC1,  8, 1, 0),
SOC_SINGLE("ALC Capture Max Gain", NAU88c10_ALC1,  3, 7, 0),
SOC_SINGLE("ALC Capture Min Gain", NAU88c10_ALC1,  0, 7, 0),

SOC_SINGLE("ALC Capture ZC Switch", NAU88c10_ALC2,  8, 1, 0),
SOC_SINGLE("ALC Capture Hold", NAU88c10_ALC2,  4, 7, 0),
SOC_SINGLE("ALC Capture Target", NAU88c10_ALC2,  0, 15, 0),

SOC_ENUM("ALC Capture Mode", nau88c10_enum[3]),
SOC_SINGLE("ALC Capture Decay", NAU88c10_ALC3,  4, 15, 0),
SOC_SINGLE("ALC Capture Attack", NAU88c10_ALC3,  0, 15, 0),

SOC_SINGLE("ALC Capture Noise Gate Switch", NAU88c10_NGATE,  3, 1, 0),
SOC_SINGLE("ALC Capture Noise Gate Threshold", NAU88c10_NGATE,  0, 7, 0),

SOC_SINGLE("Capture PGA ZC Switch", NAU88c10_INPPGA,  7, 1, 0),
SOC_SINGLE("Capture PGA Volume", NAU88c10_INPPGA,  0, 63, 0),

SOC_SINGLE("Speaker Playback ZC Switch", NAU88c10_SPKVOL,  7, 1, 0),
SOC_SINGLE("Speaker Playback Switch", NAU88c10_SPKVOL,  6, 1, 1),
SOC_SINGLE("Speaker Playback Volume", NAU88c10_SPKVOL,  0, 63, 0),
SOC_SINGLE("Speaker Boost", NAU88c10_OUTPUT, 2, 1, 0),

SOC_SINGLE("Capture Boost(+20dB)", NAU88c10_ADCBOOST,  8, 1, 0),
SOC_SINGLE("Mono Playback Switch", NAU88c10_MONOMIX, 6, 1, 1),
};

/* Speaker Output Mixer */
static const struct snd_kcontrol_new nau88c10_speaker_mixer_controls[] = {
SOC_DAPM_SINGLE("Line Bypass Switch", NAU88c10_SPKMIX, 1, 1, 0),
SOC_DAPM_SINGLE("Aux Playback Switch", NAU88c10_SPKMIX, 5, 1, 0),
SOC_DAPM_SINGLE("PCM Playback Switch", NAU88c10_SPKMIX, 0, 1, 0),
};

/* Mono Output Mixer */
static const struct snd_kcontrol_new nau88c10_mono_mixer_controls[] = {
SOC_DAPM_SINGLE("Line Bypass Switch", NAU88c10_MONOMIX, 1, 1, 0),
SOC_DAPM_SINGLE("Aux Playback Switch", NAU88c10_MONOMIX, 2, 1, 0),
SOC_DAPM_SINGLE("PCM Playback Switch", NAU88c10_MONOMIX, 0, 1, 0),
};

static const struct snd_kcontrol_new nau88c10_boost_controls[] = {
SOC_DAPM_SINGLE("Mic PGA Switch", NAU88c10_INPPGA,  6, 1, 1),
SOC_DAPM_SINGLE("Aux Volume", NAU88c10_ADCBOOST, 0, 7, 0),
SOC_DAPM_SINGLE("Mic Volume", NAU88c10_ADCBOOST, 4, 7, 0),
};

static const struct snd_kcontrol_new nau88c10_micpga_controls[] = {
SOC_DAPM_SINGLE("MICP Switch", NAU88c10_INPUT, 0, 1, 0),
SOC_DAPM_SINGLE("MICN Switch", NAU88c10_INPUT, 1, 1, 0),
SOC_DAPM_SINGLE("AUX Switch", NAU88c10_INPUT, 2, 1, 0),
};

static const struct snd_soc_dapm_widget nau88c10_dapm_widgets[] = {
SND_SOC_DAPM_MIXER("Speaker Mixer", NAU88c10_POWER3, 2, 0,
	&nau88c10_speaker_mixer_controls[0],
	ARRAY_SIZE(nau88c10_speaker_mixer_controls)),
SND_SOC_DAPM_MIXER("Mono Mixer", NAU88c10_POWER3, 3, 0,
	&nau88c10_mono_mixer_controls[0],
	ARRAY_SIZE(nau88c10_mono_mixer_controls)),
SND_SOC_DAPM_DAC("DAC", "HiFi Playback", NAU88c10_POWER3, 0, 0),
SND_SOC_DAPM_ADC("ADC", "HiFi Capture", NAU88c10_POWER2, 0, 0),
SND_SOC_DAPM_PGA("Aux Input", NAU88c10_POWER1, 6, 0, NULL, 0),
SND_SOC_DAPM_PGA("SpkN Out", NAU88c10_POWER3, 5, 0, NULL, 0),
SND_SOC_DAPM_PGA("SpkP Out", NAU88c10_POWER3, 6, 0, NULL, 0),
SND_SOC_DAPM_PGA("Mono Out", NAU88c10_POWER3, 7, 0, NULL, 0),

SND_SOC_DAPM_MIXER("Mic PGA", NAU88c10_POWER2, 2, 0,
		   &nau88c10_micpga_controls[0],
		   ARRAY_SIZE(nau88c10_micpga_controls)),
SND_SOC_DAPM_MIXER("Boost Mixer", NAU88c10_POWER2, 4, 0,
	&nau88c10_boost_controls[0],
	ARRAY_SIZE(nau88c10_boost_controls)),

SND_SOC_DAPM_MICBIAS("Mic Bias", NAU88c10_POWER1, 4, 0),

SND_SOC_DAPM_INPUT("MICN"),
SND_SOC_DAPM_INPUT("MICP"),
SND_SOC_DAPM_INPUT("AUX"),
SND_SOC_DAPM_OUTPUT("MONOOUT"),
SND_SOC_DAPM_OUTPUT("SPKOUTP"),
SND_SOC_DAPM_OUTPUT("SPKOUTN"),
};

static const struct snd_soc_dapm_route nau88c10_audio_map[] = {
	/* Mono output mixer */
	{"Mono Mixer", "PCM Playback Switch", "DAC"},
	{"Mono Mixer", "Aux Playback Switch", "Aux Input"},
	{"Mono Mixer", "Line Bypass Switch", "Boost Mixer"},

	/* Speaker output mixer */
	{"Speaker Mixer", "PCM Playback Switch", "DAC"},
	{"Speaker Mixer", "Aux Playback Switch", "Aux Input"},
	{"Speaker Mixer", "Line Bypass Switch", "Boost Mixer"},

	/* Outputs */
	{"Mono Out", NULL, "Mono Mixer"},
	{"MONOOUT", NULL, "Mono Out"},
	{"SpkN Out", NULL, "Speaker Mixer"},
	{"SpkP Out", NULL, "Speaker Mixer"},
	{"SPKOUTN", NULL, "SpkN Out"},
	{"SPKOUTP", NULL, "SpkP Out"},

	/* Microphone PGA */
	{"Mic PGA", "MICN Switch", "MICN"},
	{"Mic PGA", "MICP Switch", "MICP"},
	{ "Mic PGA", "AUX Switch", "Aux Input" },

	/* Boost Mixer */
	{"Boost Mixer", "Mic PGA Switch", "Mic PGA"},
	{"Boost Mixer", "Mic Volume", "MICP"},
	{"Boost Mixer", "Aux Volume", "Aux Input"},

	{"ADC", NULL, "Boost Mixer"},
};

static int nau88c10_add_widgets(struct snd_soc_codec *codec)
{
	struct snd_soc_dapm_context *dapm = &codec->dapm;
    printk(KERN_DEBUG "+%s\n", __FUNCTION__);

	snd_soc_dapm_new_controls(dapm, nau88c10_dapm_widgets,
				  ARRAY_SIZE(nau88c10_dapm_widgets));
	snd_soc_dapm_add_routes(dapm, nau88c10_audio_map, ARRAY_SIZE(nau88c10_audio_map));
    printk(KERN_DEBUG "-%s\n", __FUNCTION__);

	return 0;
}

struct pll_ {
	unsigned int pre_div:4; /* prescale - 1 */
	unsigned int n:4;
	unsigned int k;
};

static struct pll_ pll_div;

/* The size in bits of the pll divide multiplied by 10
 * to allow rounding later */
#define FIXED_PLL_SIZE ((1 << 24) * 10)

static void pll_factors(unsigned int target, unsigned int source)
{
	unsigned long long Kpart;
	unsigned int K, Ndiv, Nmod;
	
    printk(KERN_DEBUG "+%s\n", __FUNCTION__);
	Ndiv = target / source;
	if (Ndiv < 6) {
		source >>= 1;
		pll_div.pre_div = 1;
		Ndiv = target / source;
	} else
		pll_div.pre_div = 0;

	if ((Ndiv < 6) || (Ndiv > 12))
		printk(KERN_WARNING
			"NAU88c10 N value %u outwith recommended range!d\n", Ndiv);

	pll_div.n = Ndiv;
	Nmod = target % source;
	Kpart = FIXED_PLL_SIZE * (long long)Nmod;

	do_div(Kpart, source);

	K = Kpart & 0xFFFFFFFF;

	/* Check if we need to round */
	if ((K % 10) >= 5)
		K += 5;

	/* Move down to proper range now rounding is done */
	K /= 10;

	pll_div.k = K;
    printk(KERN_DEBUG "-%s\n", __FUNCTION__);
}

static int nau88c10_set_dai_pll(struct snd_soc_dai *codec_dai, int pll_id,
		int source, unsigned int freq_in, unsigned int freq_out)
{
	struct snd_soc_codec *codec = codec_dai->codec;
	u16 reg;
	
    printk(KERN_DEBUG "+%s\n", __FUNCTION__);

	//scott.hu comment out this function.
	if(pll_id == 0)
	{
		printk(KERN_ERR "%s: returned.\n", __func__);
		return 0;
	}
	
	if (freq_in == 0 || freq_out == 0) {
		/* Clock CODEC directly from MCLK */
		reg = nau88c10_read(codec, NAU88c10_CLOCK1);
		nau88c10_write(codec, NAU88c10_CLOCK1, reg & 0x0ff);

		/* Turn off PLL */
		reg = nau88c10_read(codec, NAU88c10_POWER1);
		nau88c10_write(codec, NAU88c10_POWER1, reg & 0x1df);
		return 0;
	}

	pll_factors(freq_out*4, freq_in);

	printk(KERN_ERR "%s: pre_div=%d,n=%d,k=%d.\n", 
		__func__, pll_div.pre_div, pll_div.n, pll_div.k);
	
	nau88c10_write(codec, NAU88c10_PLLN, (pll_div.pre_div << 4) | pll_div.n);
	nau88c10_write(codec, NAU88c10_PLLK1, pll_div.k >> 18);
	nau88c10_write(codec, NAU88c10_PLLK2, (pll_div.k >> 9) & 0x1ff);
	nau88c10_write(codec, NAU88c10_PLLK3, pll_div.k & 0x1ff);
	reg = nau88c10_read(codec, NAU88c10_POWER1);
	nau88c10_write(codec, NAU88c10_POWER1, reg | 0x020);

	/* Run CODEC from PLL instead of MCLK */
	reg = nau88c10_read(codec, NAU88c10_CLOCK1);
	nau88c10_write(codec, NAU88c10_CLOCK1, reg | 0x100);
    printk(KERN_DEBUG "-%s\n", __FUNCTION__);

	return 0;
}

/*
 * Configure NAU88c10 clock dividers.
 */
static int nau88c10_set_dai_clkdiv(struct snd_soc_dai *codec_dai,
		int div_id, int div)
{
	struct snd_soc_codec *codec = codec_dai->codec;
	u16 reg;
	
    printk(KERN_DEBUG "+%s\n", __FUNCTION__);
	switch (div_id) {
	case NAU88c10_OPCLKDIV:
	//	reg = nau88c10_read(codec, NAU88c10_GPIO) & 0x1cf;
	//	nau88c10_write(codec, NAU88c10_GPIO, reg | div);
		break;
	case NAU88c10_MCLKDIV:
		reg = nau88c10_read(codec, NAU88c10_CLOCK1) & 0x11f;
		nau88c10_write(codec, NAU88c10_CLOCK1, reg | div);
		break;
	case NAU88c10_ADCCLK:
		reg = nau88c10_read(codec, NAU88c10_ADC) & 0x1f7;
		nau88c10_write(codec, NAU88c10_ADC, reg | div);
		break;
	case NAU88c10_DACCLK:
		reg = nau88c10_read(codec, NAU88c10_DAC) & 0x1f7;
		nau88c10_write(codec, NAU88c10_DAC, reg | div);
		break;
	case NAU88c10_BCLKDIV:
		reg = nau88c10_read(codec, NAU88c10_CLOCK1) & 0x1e3;
		nau88c10_write(codec, NAU88c10_CLOCK1, reg | div);
		break;
	default:
		return -EINVAL;
	}
    printk(KERN_DEBUG "-%s\n", __FUNCTION__);

	return 0;
}

static int nau88c10_set_dai_fmt(struct snd_soc_dai *codec_dai,
		unsigned int fmt)
{
	struct snd_soc_codec *codec = codec_dai->codec;
	u16 iface = 0;
	u16 clk = nau88c10_read(codec, NAU88c10_CLOCK1) & 0x1fe;
    printk(KERN_DEBUG "+%s\n", __FUNCTION__);

	/* set master/slave audio interface */
	switch (fmt & SND_SOC_DAIFMT_MASTER_MASK) {
	case SND_SOC_DAIFMT_CBM_CFM:
		clk |= 0x0001;
		break;
	case SND_SOC_DAIFMT_CBS_CFS:
		break;
	default:
		return -EINVAL;
	}
	
	/* interface format */
	switch (fmt & SND_SOC_DAIFMT_FORMAT_MASK) {
	case SND_SOC_DAIFMT_I2S:
		iface |= 0x0010;
		break;
	case SND_SOC_DAIFMT_RIGHT_J:
		break;
	case SND_SOC_DAIFMT_LEFT_J:
		iface |= 0x0008;
		break;
	case SND_SOC_DAIFMT_DSP_A:
		iface |= 0x00018;
		break;
	default:
		return -EINVAL;
	}

	/* clock inversion */
	switch (fmt & SND_SOC_DAIFMT_INV_MASK) {
	case SND_SOC_DAIFMT_NB_NF:
		break;
	case SND_SOC_DAIFMT_IB_IF:
		iface |= 0x0180;
		break;
	case SND_SOC_DAIFMT_IB_NF:
		iface |= 0x0100;
		break;
	case SND_SOC_DAIFMT_NB_IF:
		iface |= 0x0080;
		break;
	default:
		return -EINVAL;
	}
	
	nau88c10_write(codec, NAU88c10_IFACE, iface);
	nau88c10_write(codec, NAU88c10_CLOCK1, clk);
	
    printk(KERN_DEBUG "-%s\n", __FUNCTION__);
	return 0;
}

static int nau88c10_pcm_hw_params(struct snd_pcm_substream *substream,
				struct snd_pcm_hw_params *params,
				struct snd_soc_dai *dai)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_codec *codec = rtd->codec;
	u16 iface = nau88c10_read(codec, NAU88c10_IFACE) & 0x19f;
	u16 adn = nau88c10_read(codec, NAU88c10_CLOCK2) & 0x1f1;
	
    printk(KERN_DEBUG "+%s\n", __FUNCTION__);

	/* bit size */
	switch (params_format(params)) {
	case SNDRV_PCM_FORMAT_S16_LE:
		break;
	case SNDRV_PCM_FORMAT_S20_3LE:
		iface |= 0x0020;
		break;
	case SNDRV_PCM_FORMAT_S24_LE:
		iface |= 0x0040;
		break;
	case SNDRV_PCM_FORMAT_S32_LE:
		iface |= 0x0060;
		break;
	}

	/* filter coefficient */
	switch (params_rate(params)) {
	case 8000:
		adn |= 0x5 << 1;
		break;
	case 11025:
		adn |= 0x4 << 1;
		break;
	case 16000:
		adn |= 0x3 << 1;
		break;
	case 22050:
		adn |= 0x2 << 1;
		break;
	case 32000:
		adn |= 0x1 << 1;
		break;
	case 44100:
	case 48000:
		break;
	}
	
	//nau88c10_write(codec, NAU88c10_IFACE, iface);
	nau88c10_write(codec, NAU88c10_CLOCK2, adn);
	
    printk(KERN_DEBUG "-%s\n", __FUNCTION__);
	return 0;
}

static int nau88c10_mute(struct snd_soc_dai *dai, int mute)
{
	struct snd_soc_codec *codec = dai->codec;
	u16 mute_reg = nau88c10_read(codec, NAU88c10_DAC) & 0xffbf;
	
    printk(KERN_DEBUG "+%s\n", __FUNCTION__);

	if (mute)
	{
		 printk(KERN_DEBUG "+%s do nothing\n", __FUNCTION__);
		//nau88c10_write(codec, NAU88c10_DAC, mute_reg | 0x40);
	}
	else
		nau88c10_write(codec, NAU88c10_DAC, mute_reg);
	
    printk(KERN_DEBUG "-%s\n", __FUNCTION__);
	return 0;
}

/* liam need to make this lower power with dapm */
static int nau88c10_set_bias_level(struct snd_soc_codec *codec,
	enum snd_soc_bias_level level)
{
	u16 power1 = nau88c10_read(codec, NAU88c10_POWER1) & ~0x3;
	
    printk(KERN_DEBUG "+%s\n", __FUNCTION__);

	switch (level) {
	  case SND_SOC_BIAS_ON:
	  case SND_SOC_BIAS_PREPARE:
	  	//power1 |= 0x1;  /* VMID 50k */
	  	power1 = 0x11d; //scott.hu
	  	nau88c10_write(codec, NAU88c10_POWER1, power1);
  		break;

	  case SND_SOC_BIAS_STANDBY:
		  power1 |= NAU88c10_POWER1_BIASEN | NAU88c10_POWER1_BUFIOEN;

		  if (codec->dapm.bias_level == SND_SOC_BIAS_OFF) {
			  /* Initial cap charge at VMID 5k */
			  nau88c10_write(codec, NAU88c10_POWER1, power1 | 0x3);
			  mdelay(100);
		  }

		  power1 |= 0x2;  /* VMID 500k */
		  nau88c10_write(codec, NAU88c10_POWER1, power1);
		  break;

	  case SND_SOC_BIAS_OFF:
		  nau88c10_write(codec, NAU88c10_POWER1, 0);
		  nau88c10_write(codec, NAU88c10_POWER2, 0);
		  nau88c10_write(codec, NAU88c10_POWER3, 0);
		  break;
	}

	codec->dapm.bias_level = level;
    printk(KERN_DEBUG "-%s\n", __FUNCTION__);
	return 0;
}

static int nau88c10_reg_init(struct snd_soc_codec *codec)
{
	int i;
	int ret;
	
	printk("+%s\n",__func__);
	
	printk("%s, SET_CODEC_REG_INIT_NUM: %d \n",__func__, SET_CODEC_REG_INIT_NUM);
    for(i = 0; i < SET_CODEC_REG_INIT_NUM; i++)
    {
		ret = nau88c10_write(codec,init_reg[i][0],init_reg[i][1]);
    }
	
	printk("-%s\n",__func__);
	return 0;
}

#define NAU88c10_RATES (SNDRV_PCM_RATE_8000 | SNDRV_PCM_RATE_11025 |\
		SNDRV_PCM_RATE_16000 | SNDRV_PCM_RATE_22050 |\
		SNDRV_PCM_RATE_44100 | SNDRV_PCM_RATE_48000)

#define NAU88c10_FORMATS (SNDRV_PCM_FMTBIT_S16_LE | SNDRV_PCM_FMTBIT_S20_3LE |\
	SNDRV_PCM_FMTBIT_S24_LE | SNDRV_PCM_FMTBIT_S32_LE)
	
static int nau88c10_volatile_register(
	struct snd_soc_codec *codec, unsigned int reg)
{
	return 0;
}

static int nau88c10_readable_register(
	struct snd_soc_codec *codec, unsigned int reg)
{
	return 1;
}

static struct regmap *nau88c10_get_regmap(struct device *dev)
{
	struct nau88c10_priv *nau88c10 = dev_get_drvdata(dev);

	return nau88c10->regmap;
}

static int nau88c10_suspend(struct snd_soc_codec *codec, pm_message_t state)
{
    printk(KERN_DEBUG "+%s\n", __FUNCTION__);
    nau88c10_write(codec, NAU88c10_POWER1, 0x0000);
    nau88c10_write(codec, NAU88c10_POWER2, 0x0000);
    nau88c10_write(codec, NAU88c10_POWER3, 0x0000);
    printk(KERN_DEBUG "-%s\n", __FUNCTION__);
	return 0;
}

static int nau88c10_resume(struct snd_soc_codec *codec)
{
    	printk(KERN_DEBUG "+%s\n", __FUNCTION__);
	nau88c10_reg_init(codec);
    	printk(KERN_DEBUG "-%s\n", __FUNCTION__);
	return 0;
}

static int nau88c10_probe(struct snd_soc_codec *codec)
{
	struct nau88c10_priv *nau88c10 = snd_soc_codec_get_drvdata(codec);
	int ret = 0;

	printk("+%s\n",__func__);

	nau88c10_reset(codec);

	/* power on device */
	nau88c10_set_bias_level(codec, SND_SOC_BIAS_STANDBY);

	nau88c10_reg_init(codec);

	codec->dapm.bias_level = SND_SOC_BIAS_STANDBY;
	nau88c10->codec = codec;
	
	printk("-%s\n",__func__);
	return 0;
}

/* power down chip */
static int nau88c10_remove(struct snd_soc_codec *codec)
{
	struct nau88c10_priv *nau88c10 = snd_soc_codec_get_drvdata(codec);

	nau88c10_set_bias_level(codec, SND_SOC_BIAS_OFF);
	kfree(nau88c10);
	return 0;
}

static const struct regmap_config nau88c10_regmap = {
	.reg_bits = 8,
	.val_bits = 8,
	.reg_stride = 1,

	.max_register = NAU88C10_CACHEREGNUM << 1,
	.cache_type = REGCACHE_RBTREE,
};

static struct snd_soc_codec_driver soc_codec_dev_nau88c10 = {
	.probe = nau88c10_probe,
	.remove = nau88c10_remove,
	.suspend = nau88c10_suspend,
	.resume = nau88c10_resume,
	.set_bias_level = nau88c10_set_bias_level,
	.reg_cache_size = ARRAY_SIZE(nau88c10_reg),
	.reg_word_size = sizeof(u16),
	.reg_cache_default = nau88c10_reg,
	.volatile_register = nau88c10_volatile_register,
	.readable_register = nau88c10_readable_register,
	.reg_cache_step = 1,	
	.get_regmap = nau88c10_get_regmap,
	.controls = nau88c10_snd_controls,
	.num_controls = ARRAY_SIZE(nau88c10_snd_controls),
	.dapm_widgets = nau88c10_dapm_widgets,
	.num_dapm_widgets = ARRAY_SIZE(nau88c10_dapm_widgets),
	.dapm_routes = nau88c10_audio_map,
	.num_dapm_routes = ARRAY_SIZE(nau88c10_audio_map),
};

static struct snd_soc_dai_ops nau88c10_dai_ops = {
	.hw_params		= nau88c10_pcm_hw_params,
	.digital_mute 	= nau88c10_mute,
	.set_fmt		= nau88c10_set_dai_fmt,
	.set_clkdiv		= nau88c10_set_dai_clkdiv,
	.set_pll		= nau88c10_set_dai_pll,
};

static struct snd_soc_dai_driver nau88c10_dai[] = {
	{
		.name = "nau88c10-aif1",
		.playback = {
			.stream_name = "Playback",
			.channels_min = 2,
			.channels_max = 2,
			.rates = NAU88c10_RATES,
			.formats = NAU88c10_FORMATS,},
		.capture = {
			.stream_name = "Capture",
			.channels_min = 2,
			.channels_max = 2,
			.rates = NAU88c10_RATES,
			.formats = NAU88c10_FORMATS,},
		.ops = &nau88c10_dai_ops,
		.symmetric_rates = 1,
	},
};

//achang.zhang-2018/05/11:Add for codec compatible dynamically (start)
static unsigned int nau8810_reg_read(struct device *dev,
			   unsigned int reg)
{
	struct i2c_client *i2c = to_i2c_client(dev);
	struct i2c_msg xfer[2];
	int ret;
	unsigned int tmp = reg, val = 0;

	tmp = (tmp << 1);
	xfer[0].addr = i2c->addr;
	xfer[0].flags = 0;
	xfer[0].len = 1;
	xfer[0].buf = (void *)&tmp;

	xfer[1].addr = i2c->addr;
	xfer[1].flags = I2C_M_RD;
	xfer[1].len = 2;
	xfer[1].buf = (void *)&val;

	ret = i2c_transfer(i2c->adapter, xfer, 2);
	val = cpu_to_be16(val);
	if (ret == 2)
		return val;
	else if (ret < 0)
		return ret;
	else
		return -EIO;
}
extern void quec_set_codec_info(const char* codec_name, const char* dai_name);
//achang.zhang-2018/05/11:Add for codec compatible dynamically (end)
static int nau88c10_i2c_probe(struct i2c_client *i2c,
				      const struct i2c_device_id *id)
{
	struct nau88c10_priv *nau88c10;
	int ret;
	unsigned int val = 0;
	
	printk("+%s\n",__func__);

	nau88c10 = kzalloc(sizeof(struct nau88c10_priv), GFP_KERNEL);
	if (nau88c10 == NULL)
		return -ENOMEM;

	nau88c10->regmap = devm_regmap_init_i2c(i2c, &nau88c10_regmap);
	if (IS_ERR(nau88c10->regmap)) {
		ret = PTR_ERR(nau88c10->regmap);
		dev_err(&i2c->dev, "Failed to initialise I/O: %d\n", ret);
		return ret;
	}
	
	//achang.zhang-2018/05/11:Add for codec compatible dynamically (start)
	val = nau8810_reg_read(&i2c->dev, 0x3F);
	if (val == 0x1A) {
		quec_set_codec_info("nau88c10-codec.2-001a", "nau88c10-aif1");	
	} else {
		dev_err(&i2c->dev, "Failed to read device ID\n");
		kfree(nau88c10);
		return -EINVAL;
	}
	//achang.zhang-2018/05/11:Add for codec compatible dynamically (end)

	i2c_set_clientdata(i2c, nau88c10);

	ret =  snd_soc_register_codec(&i2c->dev,
			&soc_codec_dev_nau88c10, &nau88c10_dai, ARRAY_SIZE(nau88c10_dai));
	if (ret < 0)
		kfree(nau88c10);
	
	printk("-%s\n",__func__);
	return ret;
}

static int nau88c10_i2c_remove(struct i2c_client *client)
{
	snd_soc_unregister_codec(&client->dev);
	return 0;
}

static const struct of_device_id nau88c10_of_match[] = {
	{ .compatible = "quec,quec-nau8814-i2c", },
	{ }
};
MODULE_DEVICE_TABLE(of, nau88c10_of_match);

static const struct i2c_device_id nau88c10_i2c_id[] = {
	{ NAU88C10_DEV_NAME, 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, nau88c10_i2c_id);

static struct i2c_driver nau88c10_i2c_driver = {
	.driver = {
		.name = NAU88C10_DEV_NAME,
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(nau88c10_of_match),
	},
	.probe =    nau88c10_i2c_probe,
	.remove =   nau88c10_i2c_remove,
	.id_table = nau88c10_i2c_id,
};

static int __init nau88c10_modinit(void)
{
	printk("enter %s\n",__func__);
	return i2c_add_driver(&nau88c10_i2c_driver);
}
module_init(nau88c10_modinit);

static void __exit nau88c10_exit(void)
{
	i2c_del_driver(&nau88c10_i2c_driver);
}
module_exit(nau88c10_exit);

MODULE_DESCRIPTION("ASoC NAU88c10 driver");
MODULE_AUTHOR("Liam Girdwood");
MODULE_LICENSE("GPL");


