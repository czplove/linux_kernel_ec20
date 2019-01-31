/* Copyright (c) 2010-2011, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
/*
 * Quectel power_manager  driver
 *
 */

#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <asm/atomic.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/spinlock.h>
#include <linux/quectel_power_manager.h>

#include <linux/of.h>
#include <linux/of_gpio.h>


#define WAKEUP_FILE "/sys/devices/soc:quec,quectel-power-manager/wakeup_in"

static DEFINE_MUTEX(wakeup_lock);
/* Internal functions */

static ssize_t quectel_wakeup_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct quectel_power_manager *data = dev_get_drvdata(dev);
    ssize_t ret;

	mutex_lock(&wakeup_lock);

    if(data->wakeup_flag)
    {
        ret = snprintf(buf, PAGE_SIZE, "%s\n", "Sleep");
    }
    else
    {
        ret = snprintf(buf, PAGE_SIZE, "%s\n", "Wakeup");
    }
	wake_unlock(&data->wlock_wakeup);

	mutex_unlock(&wakeup_lock);
	
	return ret;//must return none-zero value or will call poll continuesly
}

extern char g_wakeup_src[];
static ssize_t quectel_wakeup_source_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
    ssize_t ret;
	struct quectel_power_manager *data = dev_get_drvdata(dev);

	//printk("[Navy] wakeup source: %s \n", g_wakeup_src);
	mutex_lock(&wakeup_lock);
    ret = snprintf(buf, PAGE_SIZE, "%s\n", g_wakeup_src);
	wake_unlock(&data->wlock_wakeup);

	mutex_unlock(&wakeup_lock);
	
	return ret;//must return none-zero value or will call poll continuesly
}

static ssize_t quectel_wakeup_source_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t n)
{
    ssize_t len = n;
	struct quectel_power_manager *data = dev_get_drvdata(dev);
    
	device_lock(dev);
	if(len > 32)
	{
	    len = 32;
	}
	memcpy(data->wakeup_source, buf, len);
	device_unlock(dev);
    
	return n;
}

static DEVICE_ATTR(wakeup_source, 0644, quectel_wakeup_source_show, quectel_wakeup_source_store);


static void quectel_sysfs_notify_work(struct work_struct *work) 
{
    struct quectel_power_manager *data = container_of(work, struct quectel_power_manager, sysfs_notify_work.work);

    if (atomic_read(&data->notify_wakeup_in)) 
    {
        atomic_set(&data->notify_wakeup_in, 0);
        if (data->wakeup_flag)
        {
	        printk("[Navy]  sysfs_notify wakeup_in\n");
            sysfs_notify(&data->pdev->dev.kobj, NULL, "wakeup_in");
        }
    }
}

static DEVICE_ATTR(wakeup_in, 0444, quectel_wakeup_show, NULL);

static int quectel_power_manager_probe(struct platform_device *pdev)
{
    struct quectel_power_manager *data = NULL;
    struct quectel_power_manager_pdata *pdata =NULL;
    struct device *dev = &(pdev->dev);

    pdata = devm_kzalloc(dev, sizeof(struct quectel_power_manager_pdata), GFP_KERNEL);
    if (pdata == NULL) 
    {
        return -ENOMEM;
    }

    pdata->of_data = NULL;

    data = devm_kzalloc(dev, sizeof(struct quectel_power_manager), GFP_KERNEL);
    if (data == NULL) 
    {
        return -ENOMEM;
    }

    INIT_DELAYED_WORK(&data->sysfs_notify_work, quectel_sysfs_notify_work);

    atomic_set(&data->notify_wakeup_in, 0);
    wake_lock_init(&data->wlock_wakeup, WAKE_LOCK_SUSPEND, "quectel_wakeup");
    data->pdev = pdev;
    device_init_wakeup(&pdev->dev, 1);
    data->pdata = pdata;
    platform_set_drvdata(pdev, data);
    data->wakeup_flag = 0;

    if (device_create_file(&pdev->dev, &dev_attr_wakeup_in) < 0)
    {
        printk("%s: dev file creation for wakeup_in failed\n", __func__);
    }

    if (device_create_file(&pdev->dev, &dev_attr_wakeup_source) < 0)
    {
        printk("%s: dev file creation for wakeup_source failed\n", __func__);
    }

    return 0;

err_wakeup_init:
    platform_set_drvdata(pdev, NULL);

    return -1;
}

static int quectel_power_manager_remove(struct platform_device *pdev)
{
	//struct quectel_power_manager *data = platform_get_drvdata(pdev);
	
	platform_set_drvdata(pdev, NULL);
	device_remove_file(&pdev->dev, &dev_attr_wakeup_in);
	
	return 0;
}

static int quectel_power_manager_suspend(struct platform_device *pdev, pm_message_t state)
{
	struct quectel_power_manager *data = platform_get_drvdata(pdev);
    
	data->wakeup_flag = 1;

	return 0;
}

static int quectel_power_manager_resume(struct platform_device *pdev)
{
	struct quectel_power_manager *data = platform_get_drvdata(pdev);

	atomic_set(&data->notify_wakeup_in, 1);
    
	data->wakeup_flag = 0;
    printk("Resume: sysfs_notify wakeup_in\n");
    
    sysfs_notify(&data->pdev->dev.kobj, NULL, "wakeup_in");
	
	return 0;
}

static const struct of_device_id quec_power_manager_match[] = 
{
	{.compatible = "quec,quectel-power-manager",},
	{},
};

static struct platform_driver quectel_power_manager_driver = {
	.probe		= quectel_power_manager_probe,
	.remove		= quectel_power_manager_remove,
	.suspend	= quectel_power_manager_suspend,
	.resume		= quectel_power_manager_resume,
	.driver		= {
		.name = MSM9615_QUECTEL_POWER_MANAGER_NAME,
		.owner = THIS_MODULE,
		.of_match_table = quec_power_manager_match,
	},
};

module_platform_driver(quectel_power_manager_driver);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("quectel sleep driver");
MODULE_VERSION("1.0");
