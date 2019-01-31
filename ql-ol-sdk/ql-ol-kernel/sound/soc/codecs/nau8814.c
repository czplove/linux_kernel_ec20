/*
 * nau8814.c  --  NAU8814 ALSA Soc Audio driver
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

#include "nau8814.h"

/*
 * nau8814 register cache
 * We can't read the NAU8814 register space when we are
 * using 2 wire for device control, so we cache them instead.
 */
static const u16 nau8814_reg[NAU8814_CACHEREGNUM] = {
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

#define NAU8814_POWER1_BIASEN  0x08
#define NAU8814_POWER1_BUFIOEN 0x10

static u16 init_reg[][2] = {
        {NAU8814_POWER1,   		0x011d},
        {NAU8814_POWER2,   		0x0015},
        {NAU8814_POWER3,   		0x0075},
        {NAU8814_IFACE,    		0x0118},
        {NAU8814_CLOCK,    		0x0000},
        {NAU8814_ADD,      		0x000A},
        {NAU8814_DAC,	 		0x0008},
		//{NAU8814_DACVOL,		0x0040},
		{NAU8814_ADC,			0x0108},
		//{NAU8814_ADCVOL,		0x0040},
		{NAU8814_EQ1,			0x0000},
		{NAU8814_EQ2,			0x0000},
		{NAU8814_EQ3,			0x0000},
		{NAU8814_EQ4,			0x0000},
		{NAU8814_EQ5,			0x0000},
        {NAU8814_INPUT,			0x0003},
		{NAU8814_INPPGA,		0x0010},
		{NAU8814_SPKMIX,		0x0001},
		{NAU8814_MONOMIX,		0x0000},
		{NAU8814_ADCOUTDRIVER,	0x0000},
	//	{NAU8814_DACVOL,		0x00C5},
	//	{NAU8814_ADCVOL,		0x00B5},
};

#define SET_CODEC_REG_INIT_NUM  ARRAY_SIZE(init_reg)

/*
 * read nau8814 register cache
 */
static inline unsigned int nau8814_read_reg_cache(struct snd_soc_codec  *codec,
  unsigned int reg)
{
	u16 *cache = codec->reg_cache;

	if (reg == NAU8814_RESET)
	{
		return 0;
	}
	if (reg >= NAU8814_CACHEREGNUM)
	{
		return -1;
	}
	return cache[reg];
}

/*
 * write to the NAU8814 register space
 */
static int nau8814_write(struct snd_soc_codec  *codec, unsigned int reg,
  unsigned int value)
{
	u8 data[2];
	int ret = 0;

	/* data is
	*   D15..D9 NAU8814 register offset
	*   D8...D0 register data
	*/
	data[0] = (reg << 1) | ((value >> 8) & 0x0001);
	data[1] = value & 0x00ff;

	ret = snd_soc_write(codec, data[0], data[1]);

	printk(KERN_DEBUG "func: %s, reg: 0x%08x, value: 0x%08x, ret: %d\n", __FUNCTION__, reg, value, ret);
}

/*
 * write to the NAU8814 register space
 */
static int nau8814_read(struct snd_soc_codec  *codec, unsigned int reg)
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
struct nau8814_priv {
	struct snd_soc_codec *codec;
	unsigned int f_pllout;
	unsigned int f_mclk;
	unsigned int f_256fs;
	unsigned int f_opclk;
	int mclk_idx;
	enum nau8814_sysclk_src sysclk;
	u16 reg_cache[NAU8814_CACHEREGNUM];
	
	struct regmap *regmap;
};

#define NAU8814_POWER1_BIASEN  0x08
#define NAU8814_POWER1_BUFIOEN 0x10
#define nau8814_reset(c)	   nau8814_write(c, NAU8814_RESET, 0)

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
	old = nau8814_read(codec, reg);
	new = (old & ~mask) | val;
	change = old != new;
  new |= 0x100; //add update bit for  b,c,2d,2e,34,35,36,37
	if (change)
		nau8814_write(codec, reg, new);
	
    printk(KERN_DEBUG "-%s\n", __FUNCTION__);

	return change;
}

static const char *nau8814_companding[] = { "Off", "NC", "u-law", "A-law" };
static const char *nau8814_deemp[] = { "None", "32kHz", "44.1kHz", "48kHz" };
static const char *nau8814_alc[] = { "ALC", "Limiter" };

static const struct soc_enum nau8814_enum[] = {
	SOC_ENUM_SINGLE(NAU8814_COMP, 1, 4, nau8814_companding), /* adc */
	SOC_ENUM_SINGLE(NAU8814_COMP, 3, 4, nau8814_companding), /* dac */
	SOC_ENUM_SINGLE(NAU8814_DAC,  4, 4, nau8814_deemp),
	SOC_ENUM_SINGLE(NAU8814_ALC3,  8, 2, nau8814_alc),
};

static const struct snd_kcontrol_new nau8814_snd_controls[] = {

SOC_SINGLE("Digital Loopback Switch", NAU8814_COMP, 0, 1, 0),

SOC_ENUM("DAC Companding", nau8814_enum[1]),
SOC_ENUM("ADC Companding", nau8814_enum[0]),

SOC_ENUM("Playback De-emphasis", nau8814_enum[2]),
SOC_SINGLE("DAC Inversion Switch", NAU8814_DAC, 0, 1, 0),

SOC_SINGLE("Master Playback Volume", NAU8814_DACVOL, 0, 127, 0),

SOC_SINGLE("High Pass Filter Switch", NAU8814_ADC, 8, 1, 0),
SOC_SINGLE("High Pass Cut Off", NAU8814_ADC, 4, 7, 0),
SOC_SINGLE("ADC Inversion Switch", NAU8814_COMP, 0, 1, 0),

SOC_SINGLE("Capture Volume", NAU8814_ADCVOL,  0, 127, 0),

SOC_SINGLE("DAC Playback Limiter Switch", NAU8814_DACLIM1,  8, 1, 0),
SOC_SINGLE("DAC Playback Limiter Decay", NAU8814_DACLIM1,  4, 15, 0),
SOC_SINGLE("DAC Playback Limiter Attack", NAU8814_DACLIM1,  0, 15, 0),

SOC_SINGLE("DAC Playback Limiter Threshold", NAU8814_DACLIM2,  4, 7, 0),
SOC_SINGLE("DAC Playback Limiter Boost", NAU8814_DACLIM2,  0, 15, 0),

SOC_SINGLE("ALC Enable Switch", NAU8814_ALC1,  8, 1, 0),
SOC_SINGLE("ALC Capture Max Gain", NAU8814_ALC1,  3, 7, 0),
SOC_SINGLE("ALC Capture Min Gain", NAU8814_ALC1,  0, 7, 0),

SOC_SINGLE("ALC Capture ZC Switch", NAU8814_ALC2,  8, 1, 0),
SOC_SINGLE("ALC Capture Hold", NAU8814_ALC2,  4, 7, 0),
SOC_SINGLE("ALC Capture Target", NAU8814_ALC2,  0, 15, 0),

SOC_ENUM("ALC Capture Mode", nau8814_enum[3]),
SOC_SINGLE("ALC Capture Decay", NAU8814_ALC3,  4, 15, 0),
SOC_SINGLE("ALC Capture Attack", NAU8814_ALC3,  0, 15, 0),

SOC_SINGLE("ALC Capture Noise Gate Switch", NAU8814_NGATE,  3, 1, 0),
SOC_SINGLE("ALC Capture Noise Gate Threshold", NAU8814_NGATE,  0, 7, 0),

SOC_SINGLE("Capture PGA ZC Switch", NAU8814_INPPGA,  7, 1, 0),
SOC_SINGLE("Capture PGA Volume", NAU8814_INPPGA,  0, 63, 0),

SOC_SINGLE("Speaker Playback ZC Switch", NAU8814_SPKVOL,  7, 1, 0),
SOC_SINGLE("Speaker Playback Switch", NAU8814_SPKVOL,  6, 1, 1),
SOC_SINGLE("Speaker Playback Volume", NAU8814_SPKVOL,  0, 63, 0),
SOC_SINGLE("Speaker Boost", NAU8814_OUTPUT, 2, 1, 0),

SOC_SINGLE("Capture Boost(+20dB)", NAU8814_ADCBOOST,  8, 1, 0),
SOC_SINGLE("Mono Playback Switch", NAU8814_MONOMIX, 6, 1, 1),
};

/* Speaker Output Mixer */
static const struct snd_kcontrol_new nau8814_speaker_mixer_controls[] = {
SOC_DAPM_SINGLE("Line Bypass Switch", NAU8814_SPKMIX, 1, 1, 0),
SOC_DAPM_SINGLE("Aux Playback Switch", NAU8814_SPKMIX, 5, 1, 0),
SOC_DAPM_SINGLE("PCM Playback Switch", NAU8814_SPKMIX, 0, 1, 0),
};

/* Mono Output Mixer */
static const struct snd_kcontrol_new nau8814_mono_mixer_controls[] = {
SOC_DAPM_SINGLE("Line Bypass Switch", NAU8814_MONOMIX, 1, 1, 0),
SOC_DAPM_SINGLE("Aux Playback Switch", NAU8814_MONOMIX, 2, 1, 0),
SOC_DAPM_SINGLE("PCM Playback Switch", NAU8814_MONOMIX, 0, 1, 0),
};

static const struct snd_kcontrol_new nau8814_boost_controls[] = {
SOC_DAPM_SINGLE("Mic PGA Switch", NAU8814_INPPGA,  6, 1, 1),
SOC_DAPM_SINGLE("Aux Volume", NAU8814_ADCBOOST, 0, 7, 0),
SOC_DAPM_SINGLE("Mic Volume", NAU8814_ADCBOOST, 4, 7, 0),
};

static const struct snd_kcontrol_new nau8814_micpga_controls[] = {
SOC_DAPM_SINGLE("MICP Switch", NAU8814_INPUT, 0, 1, 0),
SOC_DAPM_SINGLE("MICN Switch", NAU8814_INPUT, 1, 1, 0),
SOC_DAPM_SINGLE("AUX Switch", NAU8814_INPUT, 2, 1, 0),
};

static const struct snd_soc_dapm_widget nau8814_dapm_widgets[] = {
SND_SOC_DAPM_MIXER("Speaker Mixer", NAU8814_POWER3, 2, 0,
	&nau8814_speaker_mixer_controls[0],
	ARRAY_SIZE(nau8814_speaker_mixer_controls)),
SND_SOC_DAPM_MIXER("Mono Mixer", NAU8814_POWER3, 3, 0,
	&nau8814_mono_mixer_controls[0],
	ARRAY_SIZE(nau8814_mono_mixer_controls)),
SND_SOC_DAPM_DAC("DAC", "HiFi Playback", NAU8814_POWER3, 0, 0),
SND_SOC_DAPM_ADC("ADC", "HiFi Capture", NAU8814_POWER2, 0, 0),
SND_SOC_DAPM_PGA("Aux Input", NAU8814_POWER1, 6, 0, NULL, 0),
SND_SOC_DAPM_PGA("SpkN Out", NAU8814_POWER3, 5, 0, NULL, 0),
SND_SOC_DAPM_PGA("SpkP Out", NAU8814_POWER3, 6, 0, NULL, 0),
SND_SOC_DAPM_PGA("Mono Out", NAU8814_POWER3, 7, 0, NULL, 0),

SND_SOC_DAPM_MIXER("Mic PGA", NAU8814_POWER2, 2, 0,
		   &nau8814_micpga_controls[0],
		   ARRAY_SIZE(nau8814_micpga_controls)),
SND_SOC_DAPM_MIXER("Boost Mixer", NAU8814_POWER2, 4, 0,
	&nau8814_boost_controls[0],
	ARRAY_SIZE(nau8814_boost_controls)),

SND_SOC_DAPM_MICBIAS("Mic Bias", NAU8814_POWER1, 4, 0),

SND_SOC_DAPM_INPUT("MICN"),
SND_SOC_DAPM_INPUT("MICP"),
SND_SOC_DAPM_INPUT("AUX"),
SND_SOC_DAPM_OUTPUT("MONOOUT"),
SND_SOC_DAPM_OUTPUT("SPKOUTP"),
SND_SOC_DAPM_OUTPUT("SPKOUTN"),
};

static const struct snd_soc_dapm_route nau8814_audio_map[] = {
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

static int nau8814_add_widgets(struct snd_soc_codec *codec)
{
	struct snd_soc_dapm_context *dapm = &codec->dapm;
    printk(KERN_DEBUG "+%s\n", __FUNCTION__);

	snd_soc_dapm_new_controls(dapm, nau8814_dapm_widgets,
				  ARRAY_SIZE(nau8814_dapm_widgets));
	snd_soc_dapm_add_routes(dapm, nau8814_audio_map, ARRAY_SIZE(nau8814_audio_map));
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
			"NAU8814 N value %u outwith recommended range!d\n", Ndiv);

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

static int nau8814_set_dai_pll(struct snd_soc_dai *codec_dai, int pll_id,
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
		reg = nau8814_read(codec, NAU8814_CLOCK);
		nau8814_write(codec, NAU8814_CLOCK, reg & 0x0ff);

		/* Turn off PLL */
		reg = nau8814_read(codec, NAU8814_POWER1);
		nau8814_write(codec, NAU8814_POWER1, reg & 0x1df);
		return 0;
	}

	pll_factors(freq_out*4, freq_in);

	printk(KERN_ERR "%s: pre_div=%d,n=%d,k=%d.\n", 
		__func__, pll_div.pre_div, pll_div.n, pll_div.k);
	
	nau8814_write(codec, NAU8814_PLLN, (pll_div.pre_div << 4) | pll_div.n);
	nau8814_write(codec, NAU8814_PLLK1, pll_div.k >> 18);
	nau8814_write(codec, NAU8814_PLLK2, (pll_div.k >> 9) & 0x1ff);
	nau8814_write(codec, NAU8814_PLLK3, pll_div.k & 0x1ff);
	reg = nau8814_read(codec, NAU8814_POWER1);
	nau8814_write(codec, NAU8814_POWER1, reg | 0x020);

	/* Run CODEC from PLL instead of MCLK */
	reg = nau8814_read(codec, NAU8814_CLOCK);
	nau8814_write(codec, NAU8814_CLOCK, reg | 0x100);
    printk(KERN_DEBUG "-%s\n", __FUNCTION__);

	return 0;
}

/*
 * Configure NAU8814 clock dividers.
 */
static int nau8814_set_dai_clkdiv(struct snd_soc_dai *codec_dai,
		int div_id, int div)
{
	struct snd_soc_codec *codec = codec_dai->codec;
	u16 reg;
	
    printk(KERN_DEBUG "+%s\n", __FUNCTION__);
	switch (div_id) {
	case NAU8814_OPCLKDIV:
		reg = nau8814_read(codec, NAU8814_GPIO) & 0x1cf;
		nau8814_write(codec, NAU8814_GPIO, reg | div);
		break;
	case NAU8814_MCLKDIV:
		reg = nau8814_read(codec, NAU8814_CLOCK) & 0x11f;
		nau8814_write(codec, NAU8814_CLOCK, reg | div);
		break;
	case NAU8814_ADCCLK:
		reg = nau8814_read(codec, NAU8814_ADC) & 0x1f7;
		nau8814_write(codec, NAU8814_ADC, reg | div);
		break;
	case NAU8814_DACCLK:
		reg = nau8814_read(codec, NAU8814_DAC) & 0x1f7;
		nau8814_write(codec, NAU8814_DAC, reg | div);
		break;
	case NAU8814_BCLKDIV:
		reg = nau8814_read(codec, NAU8814_CLOCK) & 0x1e3;
		nau8814_write(codec, NAU8814_CLOCK, reg | div);
		break;
	default:
		return -EINVAL;
	}
    printk(KERN_DEBUG "-%s\n", __FUNCTION__);

	return 0;
}

static int nau8814_set_dai_fmt(struct snd_soc_dai *codec_dai,
		unsigned int fmt)
{
	struct snd_soc_codec *codec = codec_dai->codec;
	u16 iface = 0;
	u16 clk = nau8814_read(codec, NAU8814_CLOCK) & 0x1fe;
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
	
	nau8814_write(codec, NAU8814_IFACE, iface);
	nau8814_write(codec, NAU8814_CLOCK, clk);
	
    printk(KERN_DEBUG "-%s\n", __FUNCTION__);
	return 0;
}

static int nau8814_pcm_hw_params(struct snd_pcm_substream *substream,
				struct snd_pcm_hw_params *params,
				struct snd_soc_dai *dai)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_codec *codec = rtd->codec;
	u16 iface = nau8814_read(codec, NAU8814_IFACE) & 0x19f;
	u16 adn = nau8814_read(codec, NAU8814_ADD) & 0x1f1;
	
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
	
	//nau8814_write(codec, NAU8814_IFACE, iface);
	nau8814_write(codec, NAU8814_ADD, adn);
	
    printk(KERN_DEBUG "-%s\n", __FUNCTION__);
	return 0;
}

static int nau8814_mute(struct snd_soc_dai *dai, int mute)
{
	struct snd_soc_codec *codec = dai->codec;
	u16 mute_reg = nau8814_read(codec, NAU8814_DAC) & 0xffbf;
	
    printk(KERN_DEBUG "+%s\n", __FUNCTION__);

	if (mute)
		nau8814_write(codec, NAU8814_DAC, mute_reg | 0x40);
	else
		nau8814_write(codec, NAU8814_DAC, mute_reg);
	
    printk(KERN_DEBUG "-%s\n", __FUNCTION__);
	return 0;
}

/* liam need to make this lower power with dapm */
static int nau8814_set_bias_level(struct snd_soc_codec *codec,
	enum snd_soc_bias_level level)
{
	u16 power1 = nau8814_read(codec, NAU8814_POWER1) & ~0x3;
	
    printk(KERN_DEBUG "+%s\n", __FUNCTION__);

	switch (level) {
	  case SND_SOC_BIAS_ON:
	  case SND_SOC_BIAS_PREPARE:
	  	//power1 |= 0x1;  /* VMID 50k */
	  	power1 = 0x11d; //scott.hu
	  	nau8814_write(codec, NAU8814_POWER1, power1);
  		break;

	  case SND_SOC_BIAS_STANDBY:
		  power1 |= NAU8814_POWER1_BIASEN | NAU8814_POWER1_BUFIOEN;

		  if (codec->dapm.bias_level == SND_SOC_BIAS_OFF) {
			  /* Initial cap charge at VMID 5k */
			  nau8814_write(codec, NAU8814_POWER1, power1 | 0x3);
			  mdelay(100);
		  }

		  power1 |= 0x2;  /* VMID 500k */
		  nau8814_write(codec, NAU8814_POWER1, power1);
		  break;

	  case SND_SOC_BIAS_OFF:
		  nau8814_write(codec, NAU8814_POWER1, 0);
		  nau8814_write(codec, NAU8814_POWER2, 0);
		  nau8814_write(codec, NAU8814_POWER3, 0);
		  break;
	}

	codec->dapm.bias_level = level;
    printk(KERN_DEBUG "-%s\n", __FUNCTION__);
	return 0;
}

//add by grady
typedef enum {
	CODEC_CONFIG_DL_GAIN,
	CODEC_CONFIG_MAX,
} codec_config_type;

static int s_gain_val[CODEC_CONFIG_MAX] = {79};

static bool update_nau8814_dl_gain(struct snd_soc_codec *codec, int val)
{
	int bits_val;

	if (val < 0 || val > 100)
		return false;

	s_gain_val[CODEC_CONFIG_DL_GAIN] = val;

	bits_val = val * 0xFF / 100;
	nau8814_write(codec, 0xB, bits_val);
//	snd_soc_update_bits(codec, 0xB, (0xFF)|(0xFF<<8), (bits_val)|(bits_val<<8));


	return true;
}

static bool update_codec_config(codec_config_type type, struct snd_soc_codec *codec, int val)
{
	bool ret = false;
 
	switch (type)
	{
	case CODEC_CONFIG_DL_GAIN:
		ret = update_nau8814_dl_gain(codec, val);
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

static ssize_t nau8814_cfg_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	int cnt = 0;
 
	cnt += sprintf(buf, "0x%x,0x%x\n", CODEC_CONFIG_DL_GAIN, s_gain_val[CODEC_CONFIG_DL_GAIN]);

        if (cnt >= PAGE_SIZE)
                cnt = PAGE_SIZE - 1;

        return cnt;
}

static ssize_t nau8814_cfg_store(struct device *dev, struct device_attribute *attr, char *buf, size_t count)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct rt8814_priv *rt8814 = i2c_get_clientdata(client);
	struct snd_soc_codec *codec = rt8814->codec;

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

static DEVICE_ATTR(nau8814_cfg, 0644, nau8814_cfg_show, nau8814_cfg_store);

static int nau8814_reg_init(struct snd_soc_codec *codec)
{
	int i;
	int ret;
	
	printk("+%s\n",__func__);
	
	printk("%s, SET_CODEC_REG_INIT_NUM: %d \n",__func__, SET_CODEC_REG_INIT_NUM);
    for(i = 0; i < SET_CODEC_REG_INIT_NUM; i++)
    {
		ret = nau8814_write(codec,init_reg[i][0],init_reg[i][1]);
    }
	
	printk("-%s\n",__func__);
	return 0;
}

#define NAU8814_RATES (SNDRV_PCM_RATE_8000 | SNDRV_PCM_RATE_11025 |\
		SNDRV_PCM_RATE_16000 | SNDRV_PCM_RATE_22050 |\
		SNDRV_PCM_RATE_44100 | SNDRV_PCM_RATE_48000)

#define NAU8814_FORMATS (SNDRV_PCM_FMTBIT_S16_LE | SNDRV_PCM_FMTBIT_S20_3LE |\
	SNDRV_PCM_FMTBIT_S24_LE | SNDRV_PCM_FMTBIT_S32_LE)
	
static int nau8814_volatile_register(
	struct snd_soc_codec *codec, unsigned int reg)
{
	return 0;
}

static int nau8814_readable_register(
	struct snd_soc_codec *codec, unsigned int reg)
{
	return 1;
}

static struct regmap *nau8814_get_regmap(struct device *dev)
{
	struct nau8814_priv *nau8814 = dev_get_drvdata(dev);

	return nau8814->regmap;
}

static int nau8814_suspend(struct snd_soc_codec *codec, pm_message_t state)
{
    printk(KERN_DEBUG "+%s\n", __FUNCTION__);
    nau8814_write(codec, NAU8814_POWER1, 0x0000);
    nau8814_write(codec, NAU8814_POWER2, 0x0000);
    nau8814_write(codec, NAU8814_POWER3, 0x0000);
    printk(KERN_DEBUG "-%s\n", __FUNCTION__);
	return 0;
}

static int nau8814_resume(struct snd_soc_codec *codec)
{
    	printk(KERN_DEBUG "+%s\n", __FUNCTION__);
	nau8814_reg_init(codec);
    	printk(KERN_DEBUG "-%s\n", __FUNCTION__);
	return 0;
}

static int nau8814_probe(struct snd_soc_codec *codec)
{
	struct nau8814_priv *nau8814 = snd_soc_codec_get_drvdata(codec);
	int ret = 0;

	printk("+%s\n",__func__);

	nau8814_reset(codec);

	/* power on device */
	nau8814_set_bias_level(codec, SND_SOC_BIAS_STANDBY);

	nau8814_reg_init(codec);

	codec->dapm.bias_level = SND_SOC_BIAS_STANDBY;
	nau8814->codec = codec;

	ret = device_create_file(codec->dev, &dev_attr_nau8814_cfg);
        if (ret != 0) {
        	dev_err(codec->dev,
       		"Failed to create index_reg sysfs files: %d\n", ret);
        	return ret;
   	 }
	

	printk("-%s\n",__func__);
	return 0;
}

/* power down chip */
static int nau8814_remove(struct snd_soc_codec *codec)
{
	struct nau8814_priv *nau8814 = snd_soc_codec_get_drvdata(codec);

	nau8814_set_bias_level(codec, SND_SOC_BIAS_OFF);
	kfree(nau8814);
	return 0;
}

static const struct regmap_config nau8814_regmap = {
	.reg_bits = 8,
	.val_bits = 8,
	.reg_stride = 1,

	.max_register = NAU8814_CACHEREGNUM << 1,
	.cache_type = REGCACHE_RBTREE,
};

static struct snd_soc_codec_driver soc_codec_dev_nau8814 = {
	.probe = nau8814_probe,
	.remove = nau8814_remove,
	.suspend = nau8814_suspend,
	.resume = nau8814_resume,
	.set_bias_level = nau8814_set_bias_level,
	.reg_cache_size = ARRAY_SIZE(nau8814_reg),
	.reg_word_size = sizeof(u16),
	.reg_cache_default = nau8814_reg,
	.volatile_register = nau8814_volatile_register,
	.readable_register = nau8814_readable_register,
	.reg_cache_step = 1,	
	.get_regmap = nau8814_get_regmap,
	.controls = nau8814_snd_controls,
	.num_controls = ARRAY_SIZE(nau8814_snd_controls),
	.dapm_widgets = nau8814_dapm_widgets,
	.num_dapm_widgets = ARRAY_SIZE(nau8814_dapm_widgets),
	.dapm_routes = nau8814_audio_map,
	.num_dapm_routes = ARRAY_SIZE(nau8814_audio_map),
};

static struct snd_soc_dai_ops nau8814_dai_ops = {
	.hw_params		= nau8814_pcm_hw_params,
	.digital_mute 	= nau8814_mute,
	.set_fmt		= nau8814_set_dai_fmt,
	.set_clkdiv		= nau8814_set_dai_clkdiv,
	.set_pll		= nau8814_set_dai_pll,
};

static struct snd_soc_dai_driver nau8814_dai[] = {
	{
		.name = "nau8814-aif1",
		.playback = {
			.stream_name = "Playback",
			.channels_min = 2,
			.channels_max = 2,
			.rates = NAU8814_RATES,
			.formats = NAU8814_FORMATS,},
		.capture = {
			.stream_name = "Capture",
			.channels_min = 2,
			.channels_max = 2,
			.rates = NAU8814_RATES,
			.formats = NAU8814_FORMATS,},
		.ops = &nau8814_dai_ops,
		.symmetric_rates = 1,
	},
};

static int nau8814_i2c_probe(struct i2c_client *i2c,
				      const struct i2c_device_id *id)
{
	struct nau8814_priv *nau8814;
	int ret;
	
	printk("+%s\n",__func__);

	nau8814 = kzalloc(sizeof(struct nau8814_priv), GFP_KERNEL);
	if (nau8814 == NULL)
		return -ENOMEM;

	nau8814->regmap = devm_regmap_init_i2c(i2c, &nau8814_regmap);
	if (IS_ERR(nau8814->regmap)) {
		ret = PTR_ERR(nau8814->regmap);
		dev_err(&i2c->dev, "Failed to initialise I/O: %d\n", ret);
		return ret;
	}
	
	i2c_set_clientdata(i2c, nau8814);

	ret =  snd_soc_register_codec(&i2c->dev,
			&soc_codec_dev_nau8814, &nau8814_dai, ARRAY_SIZE(nau8814_dai));
	if (ret < 0)
		kfree(nau8814);
	
	printk("-%s\n",__func__);
	return ret;
}

static int nau8814_i2c_remove(struct i2c_client *client)
{
	snd_soc_unregister_codec(&client->dev);
	return 0;
}

static const struct of_device_id nau8814_of_match[] = {
	{ .compatible = "quec,quec-nau8814-i2c", },
	{ }
};
MODULE_DEVICE_TABLE(of, nau8814_of_match);

static const struct i2c_device_id nau8814_i2c_id[] = {
	{ NAU8814_DEV_NAME, 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, nau8814_i2c_id);

static struct i2c_driver nau8814_i2c_driver = {
	.driver = {
		.name = NAU8814_DEV_NAME,
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(nau8814_of_match),
	},
	.probe =    nau8814_i2c_probe,
	.remove =   nau8814_i2c_remove,
	.id_table = nau8814_i2c_id,
};

static int __init nau8814_modinit(void)
{
	printk("enter %s\n",__func__);
	return i2c_add_driver(&nau8814_i2c_driver);
}
module_init(nau8814_modinit);

static void __exit nau8814_exit(void)
{
	i2c_del_driver(&nau8814_i2c_driver);
}
module_exit(nau8814_exit);

MODULE_DESCRIPTION("ASoC NAU8814 driver");
MODULE_AUTHOR("Liam Girdwood");
MODULE_LICENSE("GPL");
