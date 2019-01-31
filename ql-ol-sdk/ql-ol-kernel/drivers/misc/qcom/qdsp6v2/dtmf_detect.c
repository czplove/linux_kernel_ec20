/* Copyright (c) 2016, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/platform_device.h>
#include <linux/cdev.h>
#include <linux/qdsp6v2/apr_tal.h>
#include <linux/qdsp6v2/apr.h>
#include <sound/dtmf_detect.h>

#define MINOR_NUMBER 1
#define MAX_RESPONSE 10
#define TIMEOUT_MS 1000

#define DTMF_DETECT_DRIVER_NAME "dtmf_detect"

struct dtmf_detect_device {
	struct cdev *cdev;
	struct device *dev;
	int major;
};

struct dtmf_detect_prvt {
	uint16_t response_count;
	struct list_head response_queue;
	wait_queue_head_t response_wait;
	spinlock_t response_lock;
};

struct dtmf_response_list {
	struct list_head list;
	struct dtmf_detect_cmd_response resp;
};

static struct dtmf_detect_device *dtmf_detect_dev;
static struct class *dtmf_detect_class;
static dev_t device_number;

/*Only one session open at one time */
static struct dtmf_detect_prvt *sess_data;

static ssize_t dtmf_detect_read(struct file *file, char __user *arg,
						size_t count, loff_t *ppos)
{
	int ret = 0;
	struct dtmf_detect_prvt *prtd;
	struct dtmf_response_list *resp;
	unsigned long spin_flags;
	int size;

	pr_debug("%s\n", __func__);

	prtd = (struct dtmf_detect_prvt *)file->private_data;
	if ((prtd == NULL) || (prtd != sess_data)) {
		pr_err("%s: prtd is invalid\n", __func__);
		return -EINVAL;
	}

	spin_lock_irqsave(&prtd->response_lock, spin_flags);

	if (list_empty(&prtd->response_queue)) {
		spin_unlock_irqrestore(&prtd->response_lock, spin_flags);
		pr_debug("%s: wait for a response\n", __func__);

		ret = wait_event_interruptible(prtd->response_wait,
					!list_empty(&prtd->response_queue));
		if (ret == 0) {
			pr_debug("%s: event triggered\n", __func__);
		} else if (ret < 0) {
			pr_debug("%s: Interrupted by SIGNAL %d\n",
				 __func__, ret);
			return ret;
		}
		spin_lock_irqsave(&prtd->response_lock, spin_flags);
	}

	resp = list_first_entry(&prtd->response_queue,
				struct dtmf_response_list, list);

	spin_unlock_irqrestore(&prtd->response_lock, spin_flags);

	size = sizeof(struct dtmf_detect_cmd_response);

	if (count < size) {
		pr_err("%s: Invalid payload size %zd, %d\n",
					 __func__, count, size);
		return -ENOMEM;
	}

	if (!access_ok(VERIFY_WRITE, arg, size)) {
		pr_err("%s: Access denied to write\n", __func__);
		return -EPERM;
	}

	ret = copy_to_user(arg, &resp->resp,
			 sizeof(struct dtmf_detect_cmd_response));
	if (ret) {
		pr_err("%s: copy_to_user failed %d\n", __func__, ret);
		return -EPERM;
	}

	spin_lock_irqsave(&prtd->response_lock, spin_flags);

	list_del(&resp->list);
	prtd->response_count--;
	kfree(resp);

	spin_unlock_irqrestore(&prtd->response_lock,
				spin_flags);
	return count;
}


static int dtmf_detect_open(struct inode *inode, struct file *file)
{
	struct dtmf_detect_prvt *prtd;

	pr_debug("%s\n", __func__);
	if (sess_data != NULL) {
		pr_err("%s: Single Session already opened\n", __func__);
		return -EEXIST;
	}
	prtd = kmalloc(sizeof(struct dtmf_detect_prvt), GFP_KERNEL);
	if (prtd == NULL)
		return -ENOMEM;

	memset(prtd, 0, sizeof(struct dtmf_detect_prvt));
	prtd->response_count = 0;
	INIT_LIST_HEAD(&prtd->response_queue);
	init_waitqueue_head(&prtd->response_wait);
	spin_lock_init(&prtd->response_lock);
	file->private_data = (void *)prtd;
	sess_data = prtd;
	return 0;
}

static int dtmf_detect_release(struct inode *inode, struct file *file)
{
	struct dtmf_response_list *resp = NULL;
	unsigned long spin_flags;
	struct dtmf_detect_prvt *prtd = NULL;

	pr_debug("%s\n", __func__);

	prtd = (struct dtmf_detect_prvt *)file->private_data;
	if (prtd == NULL) {
		pr_err("%s: prtd is NULL\n", __func__);
		return -EINVAL;
	}

	spin_lock_irqsave(&prtd->response_lock, spin_flags);

	while (!list_empty(&prtd->response_queue)) {
		pr_debug("%s: Remove item from response queue\n", __func__);

		resp = list_first_entry(&prtd->response_queue,
					struct dtmf_response_list, list);
		list_del(&resp->list);
		prtd->response_count--;
		kfree(resp);
	}

	spin_unlock_irqrestore(&prtd->response_lock, spin_flags);

	sess_data = NULL;
	kfree(file->private_data);
	file->private_data = NULL;
	return 0;
}

int dtmf_detection_event_process(struct dtmf_detect_cmd_response dtmf_tone)
{
	unsigned long spin_flags;
	struct dtmf_response_list *response_list = NULL;

	pr_debug("%s Tone Detected = [%d], high = [%d] \n",
		__func__, dtmf_tone.low_freq, dtmf_tone.high_freq);
	if (sess_data != NULL) {
		/* check to see if there is a client for this DTMF Tone event */
		spin_lock_irqsave(&sess_data->response_lock, spin_flags);
		if (sess_data->response_count < MAX_RESPONSE) {
			response_list = kmalloc(
				sizeof(struct dtmf_response_list), GFP_ATOMIC);

			if (response_list == NULL) {
				pr_err("%s: kmalloc failed\n", __func__);
				spin_unlock_irqrestore(&sess_data->response_lock
					, spin_flags);
				return -ENOMEM;
			}
			response_list->resp.low_freq = dtmf_tone.low_freq;
			response_list->resp.high_freq = dtmf_tone.high_freq;
			list_add_tail(&response_list->list,
				&sess_data->response_queue);
			sess_data->response_count++;
			pr_debug("%s: added DTMF tone to list\n", __func__);
			spin_unlock_irqrestore(&sess_data->response_lock,
				spin_flags);
			wake_up(&sess_data->response_wait);
		} else {
			spin_unlock_irqrestore(&sess_data->response_lock,
				spin_flags);
			pr_err("%s: Queue Full, dropping event\n", __func__);
		}
	} else {
		pr_err("%s: No client, dropping event\n", __func__);
	}
	return 0;
}

static const struct file_operations dtmf_detect_fops = {
	.owner = THIS_MODULE,
	.open = dtmf_detect_open,
	.read = dtmf_detect_read,
	.release = dtmf_detect_release,
};

static int dtmf_detect_probe(struct platform_device *pdev)
{
	int ret = 0;

	dtmf_detect_dev = devm_kzalloc(&pdev->dev,
		sizeof(struct dtmf_detect_device), GFP_KERNEL);
	if (!dtmf_detect_dev) {
		pr_err("%s: kzalloc failed\n", __func__);
		return -ENOMEM;
	}

	ret = alloc_chrdev_region(&device_number, 0, MINOR_NUMBER,
					DTMF_DETECT_DRIVER_NAME);
	if (ret) {
		pr_err("%s: Failed to alloc chrdev\n", __func__);
		ret = -ENODEV;
		goto chrdev_err;
	}
	dtmf_detect_dev->major = MAJOR(device_number);
	dtmf_detect_class = class_create(THIS_MODULE, DTMF_DETECT_DRIVER_NAME);
	if (IS_ERR(dtmf_detect_class)) {
		ret = PTR_ERR(dtmf_detect_class);
		pr_err("%s: Failed to create class; err = %d\n", __func__,
			ret);
		goto class_err;
	}
	dtmf_detect_dev->dev = device_create(dtmf_detect_class,
		NULL, device_number, NULL, DTMF_DETECT_DRIVER_NAME);
	if (IS_ERR(dtmf_detect_dev->dev)) {
		ret = PTR_ERR(dtmf_detect_dev->dev);
		pr_err("%s: Failed to create device; err = %d\n", __func__,
			ret);
		goto dev_err;
	}
	dtmf_detect_dev->cdev = cdev_alloc();
	if (!dtmf_detect_dev->cdev) {
		pr_err("%s: Failed to alloc cdev\n", __func__);
		ret = -ENOMEM;
		goto cdev_alloc_err;
	}
	cdev_init(dtmf_detect_dev->cdev, &dtmf_detect_fops);
	ret = cdev_add(dtmf_detect_dev->cdev, device_number, MINOR_NUMBER);
	if (ret) {
		pr_err("%s: Failed to register chrdev; err = %d\n", __func__,
			ret);
		goto add_err;
	}
	goto done;

add_err:
	cdev_del(dtmf_detect_dev->cdev);
	kfree(dtmf_detect_dev->cdev);
cdev_alloc_err:
	device_destroy(dtmf_detect_class, device_number);
dev_err:
	class_destroy(dtmf_detect_class);
class_err:
	unregister_chrdev_region(0, MINOR_NUMBER);
chrdev_err:
	kfree(dtmf_detect_dev);
done:
	return ret;
}

static int dtmf_detect_remove(struct platform_device *pdev)
{
	cdev_del(dtmf_detect_dev->cdev);
	kfree(dtmf_detect_dev->cdev);
	device_destroy(dtmf_detect_class, device_number);
	class_destroy(dtmf_detect_class);
	unregister_chrdev_region(0, MINOR_NUMBER);
	kfree(dtmf_detect_dev);

	return 0;
}

static struct of_device_id dtmf_detect_of_match[] = {
	{.compatible = "qcom,msm-dtmf-detect"},
	{ }
};
MODULE_DEVICE_TABLE(of, dtmf_detect_of_match);

static struct platform_driver dtmf_detect_driver = {
	.probe	= dtmf_detect_probe,
	.remove = dtmf_detect_remove,
	.driver = {
	.name   = "msm-dtmf-detect",
	.owner	= THIS_MODULE,
	.of_match_table = dtmf_detect_of_match,
	},
};

static int __init dtmf_detect_init(void)
{
	return platform_driver_register(&dtmf_detect_driver);
}

static void __exit dtmf_detect_exit(void)
{
	platform_driver_unregister(&dtmf_detect_driver);
}

module_init(dtmf_detect_init);
module_exit(dtmf_detect_exit);

MODULE_DESCRIPTION("Soc QDSP6v2 DTMF Detection driver");
MODULE_LICENSE("GPL v2");
