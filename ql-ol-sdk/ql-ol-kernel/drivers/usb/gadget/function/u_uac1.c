/*
 * u_uac1.c -- ALSA audio utilities for Gadget stack
 *
 * Copyright (c) 2012-2016, The Linux Foundation. All rights reserved.
 * Copyright (C) 2008 Bryan Wu <cooloney@kernel.org>
 * Copyright (C) 2008 Analog Devices, Inc
 *
 * Enter bugs at http://blackfin.uclinux.org/
 *
 * Licensed under the GPL-2 or later.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/ctype.h>
#include <linux/random.h>
#include <linux/syscalls.h>

#include "u_uac1.h"
#if 1 //add by carl.yin, export /dev/uac1 to userspace instead direct read/write alsa in kernel space
#ifdef pr_fmt
#undef pr_fmt
#endif
#define pr_fmt(fmt) "%s: " fmt, __func__

#ifdef QUECTEL_UAC_FEATURE
int quec_usb_audio_enable(void);
static int usb_snd_open = 0;
#endif

/*
 * This component encapsulates the ALSA devices for USB audio gadget
 */

/*-------------------------------------------------------------------------*/

/**
 * Some ALSA internal helper functions
 */
static int snd_interval_refine_set(struct snd_interval *i, unsigned int val)
{
	struct snd_interval t;
	t.empty = 0;
	t.min = t.max = val;
	t.openmin = t.openmax = 0;
	t.integer = 1;
	return snd_interval_refine(i, &t);
}

static int _snd_pcm_hw_param_set(struct snd_pcm_hw_params *params,
				 snd_pcm_hw_param_t var, unsigned int val,
				 int dir)
{
	int changed;
	if (hw_is_mask(var)) {
		struct snd_mask *m = hw_param_mask(params, var);
		if (val == 0 && dir < 0) {
			changed = -EINVAL;
			snd_mask_none(m);
		} else {
			if (dir > 0)
				val++;
			else if (dir < 0)
				val--;
			changed = snd_mask_refine_set(
					hw_param_mask(params, var), val);
		}
	} else if (hw_is_interval(var)) {
		struct snd_interval *i = hw_param_interval(params, var);
		if (val == 0 && dir < 0) {
			changed = -EINVAL;
			snd_interval_none(i);
		} else if (dir == 0)
			changed = snd_interval_refine_set(i, val);
		else {
			struct snd_interval t;
			t.openmin = 1;
			t.openmax = 1;
			t.empty = 0;
			t.integer = 0;
			if (dir < 0) {
				t.min = val - 1;
				t.max = val;
			} else {
				t.min = val;
				t.max = val+1;
			}
			changed = snd_interval_refine(i, &t);
		}
	} else
		return -EINVAL;
	if (changed) {
		params->cmask |= 1 << var;
		params->rmask |= 1 << var;
	}
	return changed;
}
/*-------------------------------------------------------------------------*/

static inline
struct snd_interval *param_to_interval(struct snd_pcm_hw_params *p, int n)
{
	return &(p->intervals[n - SNDRV_PCM_HW_PARAM_FIRST_INTERVAL]);
}

static int pcm_buffer_size(struct snd_pcm_hw_params *params)
{
	struct snd_interval *i =
		param_to_interval(params, SNDRV_PCM_HW_PARAM_BUFFER_BYTES);
	pr_debug("buffer_bytes = (%d,%d) omin=%d omax=%d int=%d empty=%d\n",
		i->min, i->max, i->openmin, i->openmax, i->integer, i->empty);
	return i->min;
}

static int pcm_period_size(struct snd_pcm_hw_params *params)
{
	struct snd_interval *i =
		param_to_interval(params, SNDRV_PCM_HW_PARAM_PERIOD_BYTES);
	return i->min;
}

/**
 * Set default hardware params
 */
static int playback_prepare_params(struct gaudio_snd_dev *snd)
{
	struct snd_pcm_substream *substream = snd->substream;
	struct snd_pcm_hw_params *params;
	struct snd_pcm_sw_params *swparams;
	unsigned long period_size;
	unsigned long buffer_size;
	snd_pcm_sframes_t result;
	struct gaudio *card = container_of(snd, struct gaudio, playback);
	struct f_uac1_opts *opts =
		container_of(card->func.fi, struct f_uac1_opts, func_inst);

       /*
	* SNDRV_PCM_ACCESS_RW_INTERLEAVED,
	* SNDRV_PCM_FORMAT_S16_LE
	* CHANNELS: 1
	* RATE: 16K default, user configurable
	*/
	snd->access = SNDRV_PCM_ACCESS_RW_INTERLEAVED;
	snd->format = SNDRV_PCM_FORMAT_S16_LE;
	snd->channels = 1;
	snd->rate = opts->sample_rate;

	params = kzalloc(sizeof(*params), GFP_KERNEL);
	if (!params)
		return -ENOMEM;

	_snd_pcm_hw_params_any(params);
	_snd_pcm_hw_param_set(params, SNDRV_PCM_HW_PARAM_ACCESS,
			snd->access, 0);
	_snd_pcm_hw_param_set(params, SNDRV_PCM_HW_PARAM_FORMAT,
			snd->format, 0);
	_snd_pcm_hw_param_set(params, SNDRV_PCM_HW_PARAM_CHANNELS,
			snd->channels, 0);
	_snd_pcm_hw_param_set(params, SNDRV_PCM_HW_PARAM_RATE,
			snd->rate, 0);

	result = snd_pcm_kernel_ioctl(substream, SNDRV_PCM_IOCTL_DROP, NULL);
	if (result < 0)
		ERROR(snd->card,
			"SNDRV_PCM_IOCTL_DROP failed: %d\n", (int)result);

	result = snd_pcm_kernel_ioctl(substream,
			SNDRV_PCM_IOCTL_HW_PARAMS, params);
	if (result < 0) {
		ERROR(snd->card,
			"SNDRV_PCM_IOCTL_HW_PARAMS failed: %d\n", (int)result);
		kfree(params);
		return result;
	}

	result = snd_pcm_kernel_ioctl(substream, SNDRV_PCM_IOCTL_PREPARE, NULL);
	if (result < 0)
		ERROR(snd->card,
			"Preparing sound card failed: %d\n", (int)result);

	/* Store the hardware parameters */
	snd->access = params_access(params);
	snd->format = params_format(params);
	snd->channels = params_channels(params);
	snd->rate = params_rate(params);

	/* Set SW params */
	swparams = kzalloc(sizeof(*swparams), GFP_KERNEL);
	if (!swparams) {
		pr_err("Failed to allocate sw params");
		kfree(params);
		return -ENOMEM;
	}

	buffer_size = pcm_buffer_size(params);
	period_size = pcm_period_size(params);
	swparams->avail_min = period_size/2;
	swparams->xfer_align = period_size/2;

	swparams->tstamp_mode = SNDRV_PCM_TSTAMP_NONE;
	swparams->period_step = 1;
	swparams->start_threshold = 1;
	swparams->stop_threshold = INT_MAX;
	swparams->silence_size = 0;
	swparams->silence_threshold = 0;

	result = snd_pcm_kernel_ioctl(substream,
				      SNDRV_PCM_IOCTL_SW_PARAMS, swparams);
	if (result < 0)
		pr_err("SNDRV_PCM_IOCTL_SW_PARAMS failed: %d\n", (int)result);
	kfree(swparams);

	kfree(params);

	pr_debug("playback params: access %x, format %x, channels %d, rate %d\n",
		snd->access, snd->format, snd->channels, snd->rate);

	return 0;
}

static int capture_prepare_params(struct gaudio_snd_dev *snd)
{
	struct snd_pcm_substream *substream = snd->substream;
	struct snd_pcm_runtime   *runtime = substream->runtime;
	struct snd_pcm_hw_params *params;
	struct snd_pcm_sw_params *swparams;
	unsigned long period_size;
	unsigned long buffer_size;
	snd_pcm_sframes_t result = 0;
	struct gaudio *card = container_of(snd, struct gaudio, capture);
	struct f_uac1_opts *opts =
		container_of(card->func.fi, struct f_uac1_opts, func_inst);

	/*
	 * SNDRV_PCM_ACCESS_RW_INTERLEAVED,
	 * SNDRV_PCM_FORMAT_S16_LE
	 * CHANNELS: 1
	 * RATE: 16K default, user configurable
	 */
	snd->access = SNDRV_PCM_ACCESS_RW_INTERLEAVED;
	snd->format = SNDRV_PCM_FORMAT_S16_LE;
	snd->channels = 1;
	snd->rate = opts->sample_rate;

	params = kzalloc(sizeof(*params), GFP_KERNEL);
	if (!params) {
		pr_err("Failed to allocate hw params");
		return -ENOMEM;
	}

	_snd_pcm_hw_params_any(params);
	_snd_pcm_hw_param_set(params, SNDRV_PCM_HW_PARAM_ACCESS,
			snd->access, 0);
	_snd_pcm_hw_param_set(params, SNDRV_PCM_HW_PARAM_FORMAT,
			snd->format, 0);
	_snd_pcm_hw_param_set(params, SNDRV_PCM_HW_PARAM_CHANNELS,
			snd->channels, 0);
	_snd_pcm_hw_param_set(params, SNDRV_PCM_HW_PARAM_RATE,
			snd->rate, 0);

	result = snd_pcm_kernel_ioctl(substream, SNDRV_PCM_IOCTL_DROP, NULL);
	if (result < 0)
		ERROR(snd->card,
			"SNDRV_PCM_IOCTL_DROP failed: %d\n", (int)result);

	result = snd_pcm_kernel_ioctl(substream,
			SNDRV_PCM_IOCTL_HW_PARAMS, params);
	if (result < 0) {
		ERROR(snd->card,
			"SNDRV_PCM_IOCTL_HW_PARAMS failed: %d\n", (int)result);
		kfree(params);
		return result;
	}

	result = snd_pcm_kernel_ioctl(substream, SNDRV_PCM_IOCTL_PREPARE,
					NULL);
	if (result < 0)
		ERROR(snd->card, "Preparing capture failed: %d\n", (int)result);

	/* Store the hardware parameters */
	snd->access = params_access(params);
	snd->format = params_format(params);
	snd->channels = params_channels(params);
	snd->rate = params_rate(params);

	runtime->frame_bits = snd_pcm_format_physical_width(runtime->format);

	swparams = kzalloc(sizeof(*swparams), GFP_KERNEL);
	if (!swparams) {
		pr_err("Failed to allocate sw params");
		kfree(params);
		return -ENOMEM;
	}

	buffer_size = pcm_buffer_size(params);
	period_size = pcm_period_size(params);
	swparams->avail_min = period_size/2;
	swparams->xfer_align = period_size/2;

	swparams->tstamp_mode = SNDRV_PCM_TSTAMP_NONE;
	swparams->period_step = 1;
	swparams->start_threshold = 1;
	swparams->stop_threshold = INT_MAX;
	swparams->silence_size = 0;
	swparams->silence_threshold = 0;

	result = snd_pcm_kernel_ioctl(substream,
			SNDRV_PCM_IOCTL_SW_PARAMS, swparams);
	if (result < 0)
		ERROR(snd->card,
			"SNDRV_PCM_IOCTL_SW_PARAMS failed: %d\n", (int)result);

	kfree(swparams);
	kfree(params);

	INFO(snd->card,
		"capture params: access %x, format %x, channels %d, rate %d\n",
		snd->access, snd->format, snd->channels, snd->rate);

	return result;
}

/**
 * Set default hardware params
 */
static int playback_default_hw_params(struct gaudio_snd_dev *snd)
{
	struct snd_pcm_hw_params *params;
	struct gaudio *card = container_of(snd, struct gaudio, playback);
	struct f_uac1_opts *opts =
		container_of(card->func.fi, struct f_uac1_opts, func_inst);

       /*
	* SNDRV_PCM_ACCESS_RW_INTERLEAVED,
	* SNDRV_PCM_FORMAT_S16_LE
	* CHANNELS: 1
	* RATE: 16K default, user configurable
	*/
	snd->access = SNDRV_PCM_ACCESS_RW_INTERLEAVED;
	snd->format = SNDRV_PCM_FORMAT_S16_LE;
	snd->channels = 1;
	snd->rate = opts->sample_rate;

	params = kzalloc(sizeof(*params), GFP_KERNEL);
	if (!params)
		return -ENOMEM;

	_snd_pcm_hw_params_any(params);
	_snd_pcm_hw_param_set(params, SNDRV_PCM_HW_PARAM_ACCESS,
			snd->access, 0);
	_snd_pcm_hw_param_set(params, SNDRV_PCM_HW_PARAM_FORMAT,
			snd->format, 0);
	_snd_pcm_hw_param_set(params, SNDRV_PCM_HW_PARAM_CHANNELS,
			snd->channels, 0);
	_snd_pcm_hw_param_set(params, SNDRV_PCM_HW_PARAM_RATE,
			snd->rate, 0);

	/* Store the hardware parameters */
	snd->access = params_access(params);
	snd->format = params_format(params);
	snd->channels = params_channels(params);
	snd->rate = params_rate(params);

	kfree(params);

	INFO(snd->card,
		"Hardware params: access %x, format %x, channels %d, rate %d\n",
		snd->access, snd->format, snd->channels, snd->rate);

	return 0;
}

static int capture_default_hw_params(struct gaudio_snd_dev *snd)
{
	struct snd_pcm_hw_params *params;
	struct gaudio *card = container_of(snd, struct gaudio, capture);
	struct f_uac1_opts *opts =
		container_of(card->func.fi, struct f_uac1_opts, func_inst);

	/*
	 * SNDRV_PCM_ACCESS_RW_INTERLEAVED,
	 * SNDRV_PCM_FORMAT_S16_LE
	 * CHANNELS: 1
	 * RATE: 16K default, user configurable
	 */
	snd->access = SNDRV_PCM_ACCESS_RW_INTERLEAVED;
	snd->format = SNDRV_PCM_FORMAT_S16_LE;
	snd->channels = 1;
	snd->rate = opts->sample_rate;

	params = kzalloc(sizeof(*params), GFP_KERNEL);
	if (!params)
		return -ENOMEM;

	_snd_pcm_hw_params_any(params);
	_snd_pcm_hw_param_set(params, SNDRV_PCM_HW_PARAM_ACCESS,
			snd->access, 0);
	_snd_pcm_hw_param_set(params, SNDRV_PCM_HW_PARAM_FORMAT,
			snd->format, 0);
	_snd_pcm_hw_param_set(params, SNDRV_PCM_HW_PARAM_CHANNELS,
			snd->channels, 0);
	_snd_pcm_hw_param_set(params, SNDRV_PCM_HW_PARAM_RATE,
			snd->rate, 0);

	/* Store the hardware parameters */
	snd->access = params_access(params);
	snd->format = params_format(params);
	snd->channels = params_channels(params);
	snd->rate = params_rate(params);

	kfree(params);

	INFO(snd->card,
		"capture params: access %x, format %x, channels %d, rate %d\n",
		snd->access, snd->format, snd->channels, snd->rate);

	return 0;
}

static int gaudio_open_playback_streams(struct gaudio *the_card)
{
	struct gaudio_snd_dev *snd;
	int res = 0;

	if (!the_card) {
		pr_err("%s: Card is NULL", __func__);
		return -ENODEV;
	}

	pr_debug("Initialize hw params");

	/* Open PCM playback device and setup substream */
	snd = &the_card->playback;
	res = playback_prepare_params(snd);
	if (res) {
		pr_err("Setting playback params failed: err %d", res);
		return res;
	}

	pr_debug("Initialized playback params");

	return 0;
}

static int gaudio_open_capture_streams(struct gaudio *the_card)
{
	struct gaudio_snd_dev *snd;
	int res = 0;

	if (!the_card) {
		pr_err("%s: Card is NULL", __func__);
		return -ENODEV;
	}

	pr_debug("Initialize hw params");

	/* Open PCM capture device and setup substream */
	snd = &the_card->capture;
	res = capture_prepare_params(snd);
	if (res) {
		pr_err("Setting capture params failed: err %d", res);
		return res;
	}

	pr_info("Initialized capture params");

	return 0;
}

void u_audio_clear(struct gaudio *card)
{
	card->audio_reinit_playback = false;
	card->audio_reinit_capture = false;
}

/**
 * Playback audio buffer data by ALSA PCM device
 */
size_t u_audio_playback(struct gaudio *card, void *buf, size_t count)
{
#ifndef QUECTEL_UAC_FEATURE
	struct gaudio_snd_dev	*snd = &card->playback;
	struct snd_pcm_substream *substream = snd->substream;
	struct snd_pcm_runtime *runtime = substream->runtime;
	mm_segment_t old_fs;
	ssize_t result;
	snd_pcm_sframes_t frames;
	int err = 0;
#else
	struct gaudio_snd_dev	*snd ;
	struct snd_pcm_substream *substream;
	struct snd_pcm_runtime *runtime;
	mm_segment_t old_fs;
	ssize_t result;
	snd_pcm_sframes_t frames;
	int err = 0;

	if (!card->usb_snd_opened)
		return 0;
	
	snd = &card->playback;
	substream = snd->substream;
	runtime = substream->runtime;
#endif

	if (!count) {
		pr_err("Buffer is empty, no data to play");
		return 0;
	}

	if (!card->audio_reinit_playback) {
		err = gaudio_open_playback_streams(card);
		if (err) {
			pr_err("Failed to init audio streams");
			return 0;
		}
		card->audio_reinit_playback = 1;
	}

try_again:
	if (runtime->status->state == SNDRV_PCM_STATE_XRUN ||
		runtime->status->state == SNDRV_PCM_STATE_SUSPENDED ||
		runtime->status->state == SNDRV_PCM_STATE_SETUP) {
		result = snd_pcm_kernel_ioctl(substream,
				SNDRV_PCM_IOCTL_PREPARE, NULL);
		if (result < 0) {
			ERROR(card, "Preparing sound card failed: %d\n",
					(int)result);
			return result;
		}
	}

	if (!runtime->frame_bits) {
		ERROR(card, "SND failure - runtime->frame_bits == 0");
		return 0;
	}

	frames = bytes_to_frames(runtime, count);
	pr_debug("runtime->frame_bits = %d, count = %d, frames = %d",
		runtime->frame_bits, (int)count, (int)frames);

	old_fs = get_fs();
	set_fs(KERNEL_DS);
	result = snd_pcm_lib_write(snd->substream, (void __user *)buf, frames);
	if (result != frames) {
		ERROR(card, "Playback error: %d\n", (int)result);
		set_fs(old_fs);
		goto try_again;
	}
	set_fs(old_fs);

	pr_debug("Done. Sent %d frames", (int)frames);

	return 0;
}

size_t u_audio_capture(struct gaudio *card, void *buf, size_t count)
{
	ssize_t result;
	mm_segment_t old_fs;
	snd_pcm_sframes_t frames;
	int err = 0;
	
#ifndef QUECTEL_UAC_FEATURE
	struct gaudio_snd_dev	 *snd = &card->capture;
	struct snd_pcm_substream *substream = snd->substream;
	struct snd_pcm_runtime   *runtime = substream->runtime;
#else
	struct gaudio_snd_dev	 *snd;
	struct snd_pcm_substream *substream;
	struct snd_pcm_runtime   *runtime;

	if (!card->usb_snd_opened)
		return 0;
	
	snd = &card->capture;
	substream = snd->substream;
	runtime = substream->runtime;
#endif

	if (!card->audio_reinit_capture) {
		err = gaudio_open_capture_streams(card);
		if (err) {
			pr_err("Failed to init audio streams: err %d", err);
			return 0;
		}
		card->audio_reinit_capture = 1;
	}

try_again:
	if (runtime->status->state == SNDRV_PCM_STATE_XRUN ||
		runtime->status->state == SNDRV_PCM_STATE_SUSPENDED ||
		runtime->status->state == SNDRV_PCM_STATE_SETUP) {
		result = snd_pcm_kernel_ioctl(substream,
				SNDRV_PCM_IOCTL_PREPARE, NULL);
		if (result < 0) {
			pr_err("Preparing capture failed: %d\n",
					(int)result);
			return result;
		}
	}

	frames = bytes_to_frames(runtime, count);

	old_fs = get_fs();
	set_fs(KERNEL_DS);

	pr_debug("frames = %d, count = %zd", (int)frames, count);

	result = snd_pcm_lib_read(substream, buf, frames);
	if (result != frames) {
		pr_err("Capture error: %d\n", (int)result);
		set_fs(old_fs);
		goto try_again;
	}

	set_fs(old_fs);

	return 0;
}

int u_audio_get_playback_channels(struct gaudio *card)
{
#ifdef QUECTEL_UAC_FEATURE
	if (!card->usb_snd_opened) {
		return 1;
	}
#endif

	pr_debug("Return %d", card->playback.channels);
	return card->playback.channels;
}

int u_audio_get_playback_rate(struct gaudio *card)
{
#ifdef QUECTEL_UAC_FEATURE
	if (!card->usb_snd_opened) {
		return UAC1_SAMPLE_RATE;
	}
#endif

	pr_debug("Return %d", card->playback.rate);
	return card->playback.rate;
}

int u_audio_get_capture_channels(struct gaudio *card)
{
#ifdef QUECTEL_UAC_FEATURE
	if (!card->usb_snd_opened) {
		return 1;
	}
#endif
	
	pr_debug("Return %d", card->capture.channels);
	return card->capture.channels;
}

int u_audio_get_capture_rate(struct gaudio *card)
{
#ifdef QUECTEL_UAC_FEATURE
	if (!card->usb_snd_opened) {
		return UAC1_SAMPLE_RATE;
	}
#endif

	pr_debug("Return %d", card->capture.rate);
	return card->capture.rate;
}


/**
 * Open ALSA PCM and control device files
 * Initial the PCM or control device
 */
static int gaudio_open_snd_dev(struct gaudio *card)
{
	struct snd_pcm_file *pcm_file;
	struct gaudio_snd_dev *snd;
	struct f_uac1_opts *opts;
	char *fn_play, *fn_cap, *fn_cntl;
	int res = 0;

#ifdef QUECTEL_UAC_FEATURE
	if (!usb_snd_open) {
		return 0;
	}
	
	if ((!card->usb_snd_setuped) || card->usb_snd_opened) {
		return 0;
	}
#endif

	opts = container_of(card->func.fi, struct f_uac1_opts, func_inst);
	fn_play = opts->fn_play;
	fn_cap = opts->fn_cap;
	fn_cntl = opts->fn_cntl;

	/* Open control device */
	snd = &card->control;
	snd->filp = filp_open(fn_cntl, O_RDWR, 0);
	if (IS_ERR(snd->filp)) {
		int ret = PTR_ERR(snd->filp);
		ERROR(card, "unable to open sound control device file: %s\n",
				fn_cntl);
		snd->filp = NULL;
		return ret;
	}
	snd->card = card;

	/* Open PCM playback device and setup substream */
	snd = &card->playback;
	snd->filp = filp_open(fn_play, O_WRONLY, 0);
	if (IS_ERR(snd->filp)) {
		int ret = PTR_ERR(snd->filp);

		ERROR(card, "No such PCM playback device: %s\n", fn_play);
		snd->filp = NULL;
		return ret;
	}
	pr_debug("Initialized PCM playback device: %s\n", fn_play);

	pcm_file = snd->filp->private_data;
	snd->substream = pcm_file->substream;
	snd->card = card;
	res = playback_default_hw_params(snd);
	if (res) {
		ERROR(card, "Setting playback HW params failed: err %d", res);
		return res;
	}

	/* Open PCM capture device and setup substream */
	snd = &card->capture;
	snd->filp = filp_open(fn_cap, O_RDONLY, 0);
	if (IS_ERR(snd->filp)) {
		ERROR(card, "No such PCM capture device: %s\n", fn_cap);
		snd->substream = NULL;
		snd->card = NULL;
		snd->filp = NULL;
		return PTR_ERR(snd->filp);
	}

	pcm_file = snd->filp->private_data;
	snd->substream = pcm_file->substream;
	snd->card = card;

	res = capture_default_hw_params(snd);

	if (res)
		ERROR(card, "Setting capture HW params failed: err %d", res);

#ifdef QUECTEL_UAC_FEATURE
	if (!card->audio_reinit_playback) {
		if (gaudio_open_playback_streams(card)) {
			pr_err("Failed to init audio playback streams");
			return 0;
		}
		card->audio_reinit_playback = 1;
	}

	if (!card->audio_reinit_capture) {
		if (gaudio_open_capture_streams(card)) {
			pr_err("Failed to init audio capture streams");
			return 0;
		}
		card->audio_reinit_capture = 1;
	}

	card->usb_snd_opened = 1;
#endif

	return res;
}

/**
 * Close ALSA PCM and control device files
 */
static int gaudio_close_snd_dev(struct gaudio *gau)
{
	struct gaudio_snd_dev	*snd;

#ifdef QUECTEL_UAC_FEATURE
	if (!gau->usb_snd_opened) {
		return 0;
	}
	gau->usb_snd_opened = 0;
#endif
	pr_debug("Enter");
	/* Close control device */
	snd = &gau->control;
	if (snd->filp) {
		filp_close(snd->filp, NULL);
		snd->filp = NULL;
	}
	snd->card = NULL;
#ifdef QUECTEL_UAC_FEATURE
	snd->substream = NULL;
	snd->card = NULL;
#endif
	/* Close PCM playback device and setup substream */
	snd = &gau->playback;
	if (snd->filp) {
		filp_close(snd->filp, NULL);
		snd->filp = NULL;
	}
	snd->card = NULL;
#ifdef QUECTEL_UAC_FEATURE
	snd->substream = NULL;
	snd->card = NULL;
#endif
	/* Close PCM capture device and setup substream */
	snd = &gau->capture;
	if (snd->filp) {
		filp_close(snd->filp, NULL);
		snd->filp = NULL;
	}
	snd->card = NULL;
#ifdef QUECTEL_UAC_FEATURE
	snd->substream = NULL;
	snd->card = NULL;
#endif
	return 0;
}

#ifdef QUECTEL_UAC_FEATURE
int quec_uac_open_snd_dev(struct gaudio *card, int open) {
	int	ret = 0;

	usb_snd_open = open;

	if (!open) {
		ret = gaudio_close_snd_dev(card);
		card->audio_reinit_playback = false;
		card->audio_reinit_capture = false;
	} else {
		ret = gaudio_open_snd_dev(card);
	}
	
	return ret;
}
#endif
/**
 * gaudio_setup - setup ALSA interface and preparing for USB transfer
 *
 * This sets up PCM, mixer or MIDI ALSA devices fore USB gadget using.
 *
 * Returns negative errno, or zero on success
 */
int gaudio_setup(struct gaudio *card)
{
	struct gaudio_snd_dev *snd;
	int	ret;

	snd = &card->control;
	if (snd->card) {
		pr_debug("snd devices already opened\n");
		return 0;
	}

#ifdef QUECTEL_UAC_FEATURE
	card->usb_snd_setuped = 1;
#endif

	pr_debug("trying to open snd devices\n");
	ret = gaudio_open_snd_dev(card);
	if (ret)
		ERROR(card, "we need at least one control device\n");

	return ret;

}

/**
 * gaudio_cleanup - remove ALSA device interface
 *
 * This is called to free all resources allocated by @gaudio_setup().
 */
void gaudio_cleanup(struct gaudio *the_card)
{
	if (the_card) {
#ifdef QUECTEL_UAC_FEATURE
		the_card->usb_snd_setuped = 0;
#endif
		gaudio_close_snd_dev(the_card);
	}
}
#else
#include <linux/mutex.h>
#include <linux/kfifo.h>

static atomic_t uac1_fifo_register = ATOMIC_INIT(0);
static atomic_t uac1_in_call = ATOMIC_INIT(0);
static struct kfifo uac1_play_fifo;
static struct kfifo uac1_capture_fifo;
static wait_queue_head_t uac1_read_wait;
static wait_queue_head_t uac1_write_wait;
static DEFINE_MUTEX(uac1_fifo_lock);

static int uac1_open (struct inode *inode, struct file *file)
{
    pr_info("%s\n", __func__);
    return 0;
}

static ssize_t uac1_read (struct file *file, char __user *buf, size_t count, loff_t *offset)
{
    int ret = 0;
    unsigned int copied = 0;

    if (!atomic_read(&uac1_in_call)) {
		return 0;
	}
    ret = wait_event_interruptible_timeout(uac1_read_wait, kfifo_len(&uac1_play_fifo) >= count, msecs_to_jiffies(25));
    if (ret > 0) {
        mutex_lock(&uac1_fifo_lock);
        ret = kfifo_to_user(&uac1_play_fifo, buf, count, &copied);
        mutex_unlock(&uac1_fifo_lock);
    }
   
    //pr_info("%s %d/%zd\n", __func__, copied, count);
    return ret ? ret : copied;
}

static ssize_t uac1_write (struct file *file, const char __user *buf, size_t count, loff_t *offset)
{
    int ret = 0;
    unsigned int copied = 0;

    mutex_lock(&uac1_fifo_lock);
    if (!atomic_read(&uac1_in_call)) {
        kfifo_reset(&uac1_play_fifo);
        kfifo_reset(&uac1_capture_fifo);        
        atomic_set(&uac1_in_call, 1);
    }
    mutex_unlock(&uac1_fifo_lock);

    ret = wait_event_interruptible_timeout(uac1_write_wait, !kfifo_is_full(&uac1_capture_fifo), msecs_to_jiffies(25));
    if (ret > 0) {
        mutex_lock(&uac1_fifo_lock);
        ret = kfifo_from_user(&uac1_capture_fifo, buf, count, &copied);
        mutex_unlock(&uac1_fifo_lock);
    }
    
    //pr_info("%s %d/%zd\n", __func__, copied, count);
    return ret ? ret : copied;
}

static long uac1_unlocked_ioctl (struct file *file, unsigned int cmd, unsigned long arg)
{
    atomic_set(&uac1_in_call, 0); //use tcflush to indicate call end
    pr_info("%s cmd = %d, arg=%ld\n", __func__, cmd, arg);
    return 0;
}

static int uac1_release (struct inode *inode, struct file *file)
{
    atomic_set(&uac1_in_call, 0);
    pr_info("%s\n", __func__);
    return 0;
}

static const struct file_operations uac1_fops =
{
    .open = uac1_open,
    .read = uac1_read,
    .write = uac1_write,
    .unlocked_ioctl = uac1_unlocked_ioctl,
    .release = uac1_release,
};

static struct miscdevice uac1_misc = 
{
    .minor = MISC_DYNAMIC_MINOR,
    .name = "uac1",
    .fops = &uac1_fops,
};

void u_audio_clear(struct gaudio *card)
{
	pr_info("%s\n", __func__);
}

size_t u_audio_playback(struct gaudio *card, void *buf, size_t count)
{
    unsigned int copied = 0;

    if (atomic_read(&uac1_in_call)) {
        mutex_lock(&uac1_fifo_lock);
        copied = kfifo_in(&uac1_play_fifo, buf, count);
        mutex_unlock(&uac1_fifo_lock);
        wake_up_interruptible(&uac1_read_wait);
    }

    //pr_info("%s %d/%zd\n", __func__, copied, count);
    return 0;
}

size_t u_audio_capture(struct gaudio *card, void *buf, size_t count)
{
    unsigned int copied = 0;

    if (atomic_read(&uac1_in_call)) {
        mutex_lock(&uac1_fifo_lock);
        copied = kfifo_out(&uac1_capture_fifo, buf, count);
        mutex_unlock(&uac1_fifo_lock);
        wake_up_interruptible(&uac1_write_wait);
    }

    //pr_info("%s %d/%zd\n", __func__, copied, count);
    return 0;
}

int u_audio_get_playback_channels(struct gaudio *card)
{
    if (!atomic_read(&uac1_fifo_register)) {
        kfifo_alloc(&uac1_play_fifo, 320*3, GFP_KERNEL);
        kfifo_alloc(&uac1_capture_fifo, 320*3, GFP_KERNEL);
        init_waitqueue_head(&uac1_read_wait);
        init_waitqueue_head(&uac1_write_wait);
        misc_register(&uac1_misc);
        atomic_inc(&uac1_fifo_register);
    }
    
    pr_info("%s\n", __func__);
    return 1;
}

int u_audio_get_playback_rate(struct gaudio *card)
{
    pr_info("%s\n", __func__);
    return 8000;
}

int u_audio_get_capture_channels(struct gaudio *card)
{
    pr_info("%s\n", __func__);
    return 1;
}

int u_audio_get_capture_rate(struct gaudio *card)
{
    pr_info("%s\n", __func__);
    return 8000;
}

int gaudio_setup(struct gaudio *card)
{
    //pr_info("%s\n", __func__);
    return 0;
}

void gaudio_cleanup(struct gaudio *the_card)
{
    pr_info("%s\n", __func__);
}
#endif
