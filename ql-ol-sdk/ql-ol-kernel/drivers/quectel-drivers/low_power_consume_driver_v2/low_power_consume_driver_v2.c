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
 * Quectel low consume driver v2.0
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

#include <linux/of.h>
#include <linux/of_gpio.h>

#include <linux/types.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/cdev.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/wakelock.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

#define GPIO_MAJOR 199         	// major device NO.
#define GPIO_MINOR 0         	// minor device NO.
#define DEVICE_NAME "wakeup_out"  //driver device node.

#define SET_OUTPUT_LOW 0
#define SET_OUTPUT_HIGH 1
#define GET_VALUE 2
#define SET_INPUT 3

#define LOW_VALUE 0
#define HIGH_VALUE 1

static int wakeup_in = -1;
module_param(wakeup_in, int, S_IRUGO);
MODULE_PARM_DESC(wakeup_in, "A pin(input) use to wakeup 4G module.");

static int wakeup_out = -1;
module_param(wakeup_out, int, S_IRUGO);
MODULE_PARM_DESC(wakeup_out, "A pin(output) use to wakeup MCU.");

/* This pin user could use or not according to requirements */
static int sleep_ind = -1;
module_param(sleep_ind, int, S_IRUGO);
MODULE_PARM_DESC(sleep_ind, "A pin(output) could be used to indicate the suspend/active status of 4G module.");

static struct class *gpio_class;
static int wakeup_in_irq = 0;
static char irq_edge[8] = {0};
//static struct wake_lock wakelock;
static struct fasync_struct *gpio_async;

//static irqreturn_t quectel_wakeup_irq_func(void)
static irq_handler_t quectel_wakeup_irq_func(void)
{
	//TODO: debounce
	int value = gpio_get_value(wakeup_in);
	if(value == LOW_VALUE)
	{
		//wake_unlock(&wakelock);
		strcpy(irq_edge, "falling");
		kill_fasync(&gpio_async, SIGIO, POLL_IN);
	}
	else if(value == HIGH_VALUE)
	{
		//wake_lock(&wakelock);
		strcpy(irq_edge, "rising");
		kill_fasync(&gpio_async, SIGIO, POLL_IN );
	}

	return IRQ_HANDLED;
}

static int gpio_open(struct inode *inode,struct file *filp)
{
	//TODO: add lock means just open once
	pr_debug("Device gpio open!\n");
	return 0;
}

static int gpio_release(struct inode *inode, struct file *filp)
{
    pr_debug("Device gpio released!\n");
    fasync_helper(-1, filp, 0, &gpio_async);
    return 0;
}

static int gpio_ioctl(struct file *filp,unsigned int cmd, unsigned long arg)
{
	if(wakeup_out == -1)
	{
		pr_err("[%s]: forgot to assign wakeup_out pin when insmod this kmod\n", __FUNCTION__);
		return 0;
	}
	
	switch(cmd)
	{
		case SET_OUTPUT_LOW:
		{
			gpio_direction_output(wakeup_out, 0);
			break;
		}
		case SET_OUTPUT_HIGH:
		{
			gpio_direction_output(wakeup_out, 1);
			break;
		}
		case GET_VALUE:
		{
			return gpio_get_value(wakeup_out);
		}
		case SET_INPUT:
		{
			gpio_direction_input(wakeup_out);
			break;
		}
		default:
		{
			pr_err("GPIO command mistake!!!\n");
			break;
		}
	}
	return 0;
}
        
static int gpio_fasync (int fd, struct file *filp, int mode)  
{
        pr_debug("quectel low consume: gpio_fasync\n");

        return fasync_helper(fd, filp, mode, &gpio_async);  
}

static const struct file_operations gpio_fops =
{
	.owner = THIS_MODULE,
	.open = gpio_open,
	.release = gpio_release,
	.unlocked_ioctl = gpio_ioctl,
	.fasync = gpio_fasync,
};

static int sleep_ind_init(void)
{
	int err = 0;
	if(sleep_ind == -1)
		return 0;

	err = gpio_request(sleep_ind, "sleep_ind");
	if (err < 0)
	{
		pr_err("[%s]: request sleep_ind: %d failed, error: %d\n", __FUNCTION__, sleep_ind, err);
		goto err_gpio_request;
	}
	
	err = gpio_direction_output(sleep_ind, 0);
	if (err < 0)
	{
		pr_err("[%s]: set sleep_ind(%d): direction output failed: %d\n", __FUNCTION__, sleep_ind, err);
		goto err_gpio_direction;
	}
	return 0;

err_gpio_request:
err_gpio_direction:
	gpio_free(sleep_ind);

	return err;
}

static int wakeup_out_init(void)
{
    int ret;
    
    dev_t my_dev_no;
    struct cdev *gpio_cdev;
    gpio_cdev = cdev_alloc();
    if(gpio_cdev == NULL)
    {
        pr_err("Cannot alloc cdev\n");
        goto request_failed;
    }
    cdev_init(gpio_cdev,&gpio_fops);
    gpio_cdev->owner=THIS_MODULE;
    ret = alloc_chrdev_region(&my_dev_no,0,1,DEVICE_NAME);
    if(ret < 0)
    {
        pr_err("alloc_chrdev_region failed\n");
        goto request_failed;
    }

    ret=cdev_add(gpio_cdev,my_dev_no,1);
    
    ret = register_chrdev(GPIO_MAJOR, DEVICE_NAME, &gpio_fops);
    if(ret < 0)
    {
    	pr_err("[%s]: register chrdev failed\n", __FUNCTION__);
    	goto request_failed;
    }
    
    gpio_class = class_create(THIS_MODULE, DEVICE_NAME);
    device_create(gpio_class, NULL, MKDEV(GPIO_MAJOR, GPIO_MINOR), NULL, DEVICE_NAME);

    pr_info("[%s]: wakeup_out init completely\n", __FUNCTION__);

    return ret;
    
request_failed:
    return ret;
}

static int wakeup_in_init(void)
{
	int err = 0;

	if(wakeup_in == -1)
	{
		pr_err("[%s]: forgot to assign wakeup_out pin when insmod this kmod\n", __FUNCTION__);
		return -1;
	}
	
	err = gpio_request(wakeup_in, "wakeup_in");
	if (err < 0)
	{
		pr_err("[%s]: request wakeup_in: %d failed, error: %d\n", __FUNCTION__, wakeup_in, err);
		goto err_gpio_request;
	}
	
	err = gpio_direction_input(wakeup_in);
	if (err < 0)
	{
		pr_err("[%s]: set wakeup_in:  direction input (%d) failed: %d\n", __FUNCTION__, wakeup_in, err);
		goto err_gpio_to_irq;
	}

	err = gpio_to_irq(wakeup_in);
	if (err < 0)
	{
		pr_err("[%s]: wakeup_in: %d to irq failed, err: %d\n", __FUNCTION__, wakeup_in, err);
		goto err_gpio_to_irq;
	}
	
	wakeup_in_irq = err;
    
	err = request_any_context_irq(wakeup_in_irq, quectel_wakeup_irq_func,	\
			IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING, "wakeup_in_irq", NULL);
	if (err < 0)
	{
		pr_err("[%s]: Can't request %d IRQ for wakeup_in: %d\n", __FUNCTION__, wakeup_in_irq, err);
		goto err_free_irq;
	}

	//wake_lock_init(&wakelock, WAKE_LOCK_SUSPEND, "quectel_wakelock");

	return 0;

err_free_irq:
	free_irq(wakeup_in_irq, NULL);
err_gpio_to_irq:
	gpio_free(wakeup_in);
err_gpio_request:
	return err;
}

static void wakeup_in_exit(void)
{	
	free_irq(wakeup_in_irq, NULL);
	disable_irq(wakeup_in_irq);

	gpio_free(wakeup_in);
}

static int wakeup_in_edge_read(struct seq_file *file, void *v)
{  
	seq_printf(file, "%s", irq_edge);
	return 0;
} 

static int wakeup_in_edge_open(struct inode *inode, struct file *file)
{
	return single_open(file, wakeup_in_edge_read, NULL);
}

static int wakeup_in_edge_write(struct file *file, const char __user *buffer, size_t count, loff_t *f_pos)
{
	return 0;
}

struct file_operations wakeup_in_edge_fops = {
	.open = wakeup_in_edge_open,
	.read = seq_read,
	.write = wakeup_in_edge_write,
	.release = single_release,
};

static int quectel_low_consume_suspend(struct platform_device *pdev)
{
	pr_debug("[%s]\n", __FUNCTION__);

	/* enable wakeup_in wakeup function */
	if (enable_irq_wake(wakeup_in_irq) < 0)
	{
		pr_err("[%s]: enable wakeup_in wakeup function failed\n", __FUNCTION__);
		return -1;
	}
	
	/* Set wakeup_out to output low level, 4G module enter sleep mode, and notify mcu */
	if(wakeup_out != -1)
		gpio_direction_output(wakeup_out, 0);

	/* Set sleep_ind to output high level, indicate the suspend status of 4G module */
	if(sleep_ind != -1)
		gpio_direction_output(sleep_ind, 1);
	
	return 0;
}

static int quectel_low_consume_resume(struct platform_device *pdev)
{

	pr_debug("[%s]\n", __FUNCTION__);

	/* disable wakeup_in wakeup function */
	if (disable_irq_wake(wakeup_in_irq) < 0)
	{
		pr_err("[%s]: disable wakeup_in wakeup function failed\n", __FUNCTION__);
		return -1;
	}
	
	/* Set wakeup_out to output high level, 4G module enter active mode, and notify mcu */
	//gpio_direction_output(wakeup_out, 0);	//Here, I think should be control in user application.

	/* Set sleep_ind to output low level, indicate the active status of 4G module */
	if(sleep_ind != -1)
		gpio_direction_output(sleep_ind, 0);

	return 0;
}
#if 1
static int quectel_low_consume_probe(struct platform_device *pdev)
{
	printk("[%s]\n", __FUNCTION__);
	return 0;
}

static const struct of_device_id quectel_low_consume_match[] =
{
	{.compatible = "quec,quectel-low-consume-v2",},
	{},
};

//MODULE_DEVICE_TABLE(of, quectel_low_consume_match);
#endif

static struct platform_driver quectel_low_consume_driver = {
	.probe		= quectel_low_consume_probe,
	//.remove		= quectel_low_consume_remove,
	.suspend	= quectel_low_consume_suspend,
	.resume		= quectel_low_consume_resume,
	.driver		= {
		.name = "quectel-low-consume-v2",
		.owner = THIS_MODULE,
		.of_match_table = quectel_low_consume_match,
	},
};

static int __init quectel_low_consume_init(void)
{
	int ret = 0;
	if((ret = wakeup_in_init()) < 0)
	{
		pr_err("[%s]: wakeup_in init failed\n", __FUNCTION__);
		goto err_wakeup_in_exit;
	}	

	if((ret = wakeup_out_init()) < 0)
	{
		pr_err("[%s]: wakeup_out init failed\n", __FUNCTION__);
		goto err_wakeup_out_exit;
	}

	if((ret = sleep_ind_init()) < 0)
	{
		pr_err("[%s]: sleep_ind init failed\n", __FUNCTION__);
		goto err_sleep_ind_exit;
	}

	if(proc_create("irq_edge", 0644, NULL, &wakeup_in_edge_fops) == 0)
		return -ENOENT;

	platform_driver_register(&quectel_low_consume_driver);

	pr_info("[%s]: module init completely\n", __FUNCTION__);
	return 0;

err_sleep_ind_exit:
err_wakeup_out_exit:
	wakeup_in_exit();
err_wakeup_in_exit:
	return ret;
}
module_init(quectel_low_consume_init);

static void __exit quectel_low_consume_exit(void)
{
	wakeup_in_exit();

	if(sleep_ind != -1)
		gpio_free(sleep_ind);

	platform_driver_unregister(&quectel_low_consume_driver);

	unregister_chrdev(GPIO_MAJOR, DEVICE_NAME);	
	device_destroy(gpio_class, MKDEV(GPIO_MAJOR, GPIO_MINOR));
	class_unregister(gpio_class);
	class_destroy(gpio_class);
	
	remove_proc_entry("irq_edge", NULL);
	pr_info("[%s]: module exit completely\n", __FUNCTION__);
}
module_exit(quectel_low_consume_exit);

//module_platform_driver(quectel_low_consume_driver);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("quectel low consume driver v2.0");
MODULE_VERSION("2.0");
