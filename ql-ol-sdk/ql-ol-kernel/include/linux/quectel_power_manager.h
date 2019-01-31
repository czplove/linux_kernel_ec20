/* include/linux/msm_adsp.h
 *
 * Copyright (C) 2007 Google, Inc.
 * Author: Iliyan Malchev <ibm@android.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#ifndef __LINUX_QUECTEL_SLEEP_H
#define	__LINUX_QUECTEL_SLEEP_H

#include <linux/types.h>
#include <linux/workqueue.h>
#include <linux/wakelock.h>

#define MSM9615_QUECTEL_POWER_MANAGER_NAME	"quectel_power_manager"

struct quectel_gpio_group {
	u32 no;
	const char *name;
	bool is_enabled;
};

struct quectel_power_manager_of_data{
	struct quectel_gpio_group *gpio;
	u8 size;
};

struct quectel_power_manager_pdata{
	int gpio_sleep_state;
	int gpio_wakeup;
	int gpio_dtr;
	struct quectel_power_manager_of_data *of_data;
};

struct quectel_power_manager {
	int irq_wakeup;
	int wakeup_flag;
	int irq_dtr;
	int dtr_flag;
    char    wakeup_source[32];
	struct platform_device *pdev;
	struct delayed_work sysfs_notify_work;
	atomic_t notify_wakeup_in;
	atomic_t notify_dtr_in;
	atomic_t notify_dtr_state;
	struct wake_lock wlock_dtr;
	struct wake_lock wlock_wakeup;
	struct quectel_power_manager_pdata *pdata;
	struct delayed_work dtr_smd_work;
};

#endif
