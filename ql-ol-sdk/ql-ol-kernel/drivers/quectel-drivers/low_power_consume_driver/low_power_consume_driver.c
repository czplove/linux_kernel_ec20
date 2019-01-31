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
 * Quectel low consume driver
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

#define GPIO_MAJOR 199         	// major device NO.
#define GPIO_MINOR 0         	// minor device NO.
#define DEVICE_NAME "gpio_notify"  //driver device node.

#define SET_OUTPUT_LOW 0
#define SET_OUTPUT_HIGH 1
#define GET_VALUE 2
#define SET_INPUT 3

#define LOW_VALUE 0
#define HIGH_VALUE 1

#define WAKEUP_PINCTRL_DEFAULT "wakeup_default"

static struct class *gpio_class;

static int notify_gpio = 0;
static int sleep_gpio = 0;
static int wakeup_gpio = 0;
static int wakeup_irq = 0;
//static struct wake_lock wakelock;
static struct fasync_struct *gpio_async;
static volatile char edge[8] = {0};

struct gpio_pinctrl_info {
	struct pinctrl *pinctrl;
	struct pinctrl_state *pinctrl_active;
	struct pinctrl_state *pinctrl_sleep;	
	struct pinctrl_state *wakeup_pinctrl_default;
};

struct gpio_pinctrl_info *gpio_data;

static irqreturn_t quectel_wakeup_irq_func(void)
{
	int value = gpio_get_value(wakeup_gpio);
	if(value == LOW_VALUE)
	{
		//wake_unlock(&wakelock);
		strcpy(edge, "falling");
		kill_fasync(&gpio_async, SIGIO, POLL_IN);
	}
	else if(value == HIGH_VALUE)
	{
		//wake_lock(&wakelock);*/
		strcpy(edge, "rising");
		kill_fasync(&gpio_async, SIGIO, POLL_IN );
	}

	return IRQ_HANDLED;
}

static ssize_t irq_edge_show(struct device* dev, struct device_attribute *attr, char* buf)
{  
	ssize_t ret = 0;  
  
	sprintf(buf, "%s", edge);  
  
	ret = strlen(buf) + 1;  
	return ret;  
} 

static DEVICE_ATTR(irq_edge, S_IRUGO, irq_edge_show, NULL);



static int gpio_open(struct inode *inode,struct file *filp)
{
    printk(KERN_INFO "Device gpio open!\n");
    return 0;
}

static int gpio_release(struct inode *inode, struct file *filp)
{
    printk (KERN_INFO "Device gpio released!\n");
    fasync_helper(-1, filp, 0, &gpio_async);
    return 0;
}

static int gpio_ioctl(struct file *filp,unsigned int cmd)
{
    if(0 == notify_gpio)
    	return -1;
        
    switch(cmd)
    {
        case SET_OUTPUT_LOW://0
        {
            gpio_direction_output(notify_gpio, 0);
            break;
        }
        case SET_OUTPUT_HIGH://1
        {
            gpio_direction_output(notify_gpio, 1);
            break;
        }
        case GET_VALUE://2
        {
            return gpio_get_value(notify_gpio);    
        }
        case SET_INPUT://3
        {
            gpio_direction_input(notify_gpio);
            break;
        }
        default:
        {
            printk(KERN_EMERG "GPIO command mistake!!!\n");
            break;
        }
    }
    return 0;
}
        
static int gpio_fasync (int fd, struct file *filp, int mode)  
{  
        printk("quectel low consume: gpio_fasync\n");  
          
        return fasync_helper(fd, filp, mode, &gpio_async);  
}

static const struct file_operations gpio_fops =
{
  .owner = THIS_MODULE,
  .open = gpio_open,
  .release = gpio_release,
  //.read = gpio_read,
  //.write = gpio_write,
  .unlocked_ioctl = gpio_ioctl,
  .fasync = gpio_fasync,
};

static int gpio_init(void)
{
    int ret;
    
    dev_t my_dev_no;
    struct cdev *gpio_cdev;
    gpio_cdev = cdev_alloc();
    if(gpio_cdev == NULL)
    {
        printk(KERN_EMERG "Cannot alloc cdev\n");
        goto request_failed;
    }
    cdev_init(gpio_cdev,&gpio_fops);
    gpio_cdev->owner=THIS_MODULE;
    ret = alloc_chrdev_region(&my_dev_no,0,1,DEVICE_NAME);
    if(ret < 0)
    {
        printk(KERN_EMERG "alloc_chrdev_region failed\n");
        goto request_failed;
    }

    ret=cdev_add(gpio_cdev,my_dev_no,1);
    
    ret = register_chrdev(GPIO_MAJOR, DEVICE_NAME, &gpio_fops);
    if(ret < 0)
    {
    	printk(KERN_EMERG "GPIO register failed\n");
    	goto request_failed;
    }
    
    gpio_class = class_create(THIS_MODULE, DEVICE_NAME);
    device_create(gpio_class, NULL, MKDEV(GPIO_MAJOR, GPIO_MINOR), NULL, DEVICE_NAME);

    printk("[gale] %s: OK ###############################\n", __func__);

    return ret;
    
request_failed:
    return ret;
}

static void gpio_exit(void)
{
	device_destroy(gpio_class, MKDEV(GPIO_MAJOR, GPIO_MINOR));
	class_unregister(gpio_class);
	class_destroy(gpio_class);
	unregister_chrdev(GPIO_MAJOR, DEVICE_NAME);
}

static int quectel_wakeup_init(void)
{
	int err;
	
	err = gpio_request(wakeup_gpio, "wakeup_in");
	if (err < 0)
	{
		printk("[gale] %s: request gpio: %d error: %d\n", __func__, wakeup_gpio, err);
		goto err_gpio_request;
	}
	
	err = gpio_direction_input(wakeup_gpio);
	if (err < 0)
	{
		printk("[gale] %s: set gpio direction input (%d) fail\n", __func__, wakeup_gpio);
		goto err_gpio_to_irq;
	}

	err = gpio_to_irq(wakeup_gpio);
	printk("[gale] %s: gpio to irq: %d : %d\n", __func__, wakeup_gpio, err);
	if (err < 0)
	{
		printk("[gale] %s: gpio to irq: %d error: %d\n", __func__, wakeup_gpio, err);
		goto err_gpio_to_irq;
	}
	
	wakeup_irq = err;
    
	err = request_any_context_irq(wakeup_irq, quectel_wakeup_irq_func, \
			IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING, "quectel_wakeup_in", NULL);
	if (err < 0)
	{
		printk("[gale] %s: Can't get %d IRQ for wakeup_in: %d\n", __func__, wakeup_irq, err);
		goto err_gpio_free_irq;
	}

	//wake_lock_init(&wakelock, WAKE_LOCK_SUSPEND, "quectel_wakelock");

	return 0;

err_gpio_free_irq:
	gpio_free(wakeup_irq);
err_gpio_to_irq:
	gpio_free(wakeup_gpio);
err_gpio_request:
	return err;
}


static int quectel_low_consume_suspend(struct platform_device *pdev)
{
	int ret;	

	if (enable_irq_wake(wakeup_irq) < 0)
	{
		printk("[gale] %s: enable irq wake fail\n", __func__);
		return -1;
	}
	
	/* Set pinctrl state to sleep */
	ret = pinctrl_select_state(gpio_data->pinctrl,
				   gpio_data->pinctrl_sleep);
	if (ret)
		dev_err(&pdev->dev, "%s: set gpio sleep state fail: %d\n",
			__func__, ret);

	//gpio_direction_output(notify_gpio, 0); //ensure 4G module enter sleep mode, and notify mcu .
	
	return 0;
}

static int quectel_low_consume_resume(struct platform_device *pdev)
{
	int ret;

	//disable wakeup_gpio wake irq
	if (disable_irq_wake(wakeup_irq) < 0)
	{
		printk("[gale] %s: disable irq wake fail\n", __func__);
		return -1;
	}
	
	/* Set pinctrl state to active */
	ret = pinctrl_select_state(gpio_data->pinctrl,
				   gpio_data->pinctrl_active);
	if (ret)
		dev_err(&pdev->dev, "%s: set gpio active state fail: %d\n",
			__func__, ret);

	return 0;
}


static int quectel_low_consume_probe(struct platform_device *pdev)
{
	int ret = 0;

	gpio_data = devm_kzalloc(&pdev->dev,
				 sizeof(struct gpio_pinctrl_info),
				 GFP_KERNEL);
	if (!gpio_data)
		return -ENOMEM;

	gpio_data->pinctrl = devm_pinctrl_get(&pdev->dev);
	if (IS_ERR_OR_NULL(gpio_data->pinctrl)) {
		dev_err(&pdev->dev, "%s: Cannot get gpio pinctrl:%ld\n",
			__func__, PTR_ERR(gpio_data->pinctrl));
		ret = PTR_ERR(gpio_data->pinctrl);
		goto err_pctrl_get;
	}

	gpio_data->pinctrl_active = pinctrl_lookup_state(
					gpio_data->pinctrl, PINCTRL_STATE_DEFAULT);
	if (IS_ERR_OR_NULL(gpio_data->pinctrl_active)) {
		dev_err(&pdev->dev, "%s: Cannot get active pinctrl state:%s\n",
			__func__, PTR_ERR(gpio_data->pinctrl_active));
		ret = PTR_ERR(gpio_data->pinctrl_active);
		goto err_lookup_state;
	}

	gpio_data->pinctrl_sleep = pinctrl_lookup_state(
					gpio_data->pinctrl, PINCTRL_STATE_SLEEP);
	if (IS_ERR_OR_NULL(gpio_data->pinctrl_sleep)) {
		dev_err(&pdev->dev, "%s: Cannot get sleep pinctrl state:%ld\n",
			__func__, PTR_ERR(gpio_data->pinctrl_sleep));
		ret = PTR_ERR(gpio_data->pinctrl_sleep);
		goto err_lookup_state;
	}

	gpio_data->wakeup_pinctrl_default = pinctrl_lookup_state(
					gpio_data->pinctrl, WAKEUP_PINCTRL_DEFAULT);
	if (IS_ERR_OR_NULL(gpio_data->wakeup_pinctrl_default)) {
		dev_err(&pdev->dev, "%s: Cannot get wakeup pinctrl default state:%ld\n",
			__func__, PTR_ERR(gpio_data->wakeup_pinctrl_default));
		ret = PTR_ERR(gpio_data->wakeup_pinctrl_default);
		goto err_lookup_state;
	}
	
	/* Set pinctrl state to default */
	ret = pinctrl_select_state(gpio_data->pinctrl,
				   gpio_data->pinctrl_active);
	if (ret)
		dev_err(&pdev->dev, "%s: set gpio active state fail: %d\n",
			__func__, ret);

	ret = pinctrl_select_state(gpio_data->pinctrl,
				   gpio_data->wakeup_pinctrl_default);
	if (ret)
		dev_err(&pdev->dev, "%s: set wakeup gpio default state fail: %d\n",
			__func__, ret);
			
	dev_set_drvdata(&pdev->dev, gpio_data);

	wakeup_gpio = of_get_named_gpio(pdev->dev.of_node, "wakeup_gpio", 0);
	if(quectel_wakeup_init() < 0)
        {
                printk("[gale] %s: init wakeup gpio fail\n", __func__);
                goto err_wakeup_init;
        }	

	notify_gpio = of_get_named_gpio(pdev->dev.of_node, "notify_gpio", 0);
	if((ret = gpio_init()) < 0)
	{
    		printk("[gale] %s: notify pin init failed\n", __func__);
                goto err_gpio_init;
	}

//	if(proc_create("wakeup_edge", 0644, NULL, &wakeup_edge_fops) == 0)
//		return -ENOENT;

	if ((ret = device_create_file(&pdev->dev, &dev_attr_irq_edge)) != 0)
	{
    		printk("[gale] %s: create attr file failed\n", __func__);
		goto err_create_file; 
	}

	return 0;

err_create_file:
err_gpio_init:
err_wakeup_init:
err_lookup_state:
	devm_pinctrl_put(gpio_data->pinctrl);
err_pctrl_get:
	devm_kfree(&pdev->dev, gpio_data);
	return ret;
}

static int quectel_low_consume_remove(struct platform_device *pdev)
{
	free_irq(wakeup_irq, NULL);
	gpio_free(wakeup_gpio);
	gpio_free(sleep_gpio);
	disable_irq_wake(wakeup_irq);
	
	platform_set_drvdata(pdev, NULL);

	gpio_exit();
	
	return 0;
}


static const struct of_device_id quectel_low_consume_match[] = 
{
	{.compatible = "quec,quectel-low-consume",},
	{},
};

static struct platform_driver quectel_low_consume_driver = {
	.probe		= quectel_low_consume_probe,
	.remove		= quectel_low_consume_remove,
	.suspend	= quectel_low_consume_suspend,
	.resume		= quectel_low_consume_resume,
	.driver		= {
		.name = "quectel-low-consume",
		.owner = THIS_MODULE,
		.of_match_table = quectel_low_consume_match,
	},
};

module_platform_driver(quectel_low_consume_driver);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("quectel low consume driver");
MODULE_VERSION("1.0");
