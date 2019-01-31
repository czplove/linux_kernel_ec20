/* Copyright (c) 2011, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/soc.h>
#include <linux/i2c.h>

#include "quec_stub.h"

static int quec_stub_probe(struct snd_soc_codec *codec)
{
	printk("%s\n",__func__);
	return 0;
}

static int quec_stub_remove(struct snd_soc_codec *codec)
{
	printk("%s\n",__func__);
	return 0;
}

/* A dummy driver useful only to advertise hardware parameters */
static struct snd_soc_dai_driver quec_stub_dais[] = {
	{
		.name = "quec-stub-rx",
		.playback = { /* Support maximum range */
			.stream_name = "Playback",
			.channels_min = 1,
			.channels_max = 8,
			.rates = SNDRV_PCM_RATE_8000_48000,
			.formats = SNDRV_PCM_FMTBIT_S16_LE,
		},
	},
	{
		.name = "quec-stub-tx",
		.capture = { /* Support maximum range */
			.stream_name = "Record",
			.channels_min = 1,
			.channels_max = 4,
			.rates = SNDRV_PCM_RATE_8000_48000,
			.formats = SNDRV_PCM_FMTBIT_S16_LE,
		},
	}
};

static struct snd_soc_codec_driver soc_quec_stub = {
	.probe = quec_stub_probe,
	.remove = quec_stub_remove,
};

static const struct i2c_device_id quec_stub_i2c_id[] = {
	{ QUEC_STUB_DEV_NAME, 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, quec_stub_i2c_id);

static int quec_stub_i2c_probe(struct i2c_client *i2c,
			const struct i2c_device_id *id)
{
	int ret;

	ret = snd_soc_register_codec(&i2c->dev, &soc_quec_stub,
			quec_stub_dais, ARRAY_SIZE(quec_stub_dais));
	
	printk("%s: ret = %d\n",__func__, ret);

	return ret;
}

static int quec_stub_i2c_remove(struct i2c_client *i2c)
{
	snd_soc_unregister_codec(&i2c->dev);
	kfree(i2c_get_clientdata(i2c));
	return 0;
}

static void quec_stub_i2c_shutdown(struct i2c_client *client)
{
	printk("%s\n",__func__);
}

static const struct of_device_id quec_stub_of_match[] = {
	{ .compatible = "quec,quec-stub-i2c", },
	{ }
};
MODULE_DEVICE_TABLE(of, quec_stub_of_match);

struct i2c_driver quec_stub_i2c_driver = {
	.driver = {
		.name = QUEC_STUB_DEV_NAME,
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(quec_stub_of_match),
	},
	.probe = quec_stub_i2c_probe,
	.remove   = quec_stub_i2c_remove,
	.shutdown = quec_stub_i2c_shutdown,
	.id_table = quec_stub_i2c_id,
};

static int quec_stub_modinit(void)
{
	printk("enter %s\n",__func__);
	return i2c_add_driver(&quec_stub_i2c_driver);
}
module_init(quec_stub_modinit);

static void quec_stub_modexit(void)
{
	i2c_del_driver(&quec_stub_i2c_driver);
}
module_exit(quec_stub_modexit);

MODULE_DESCRIPTION("Generic QUEC CODEC driver");
MODULE_VERSION("1.0");
MODULE_LICENSE("GPL v2");
