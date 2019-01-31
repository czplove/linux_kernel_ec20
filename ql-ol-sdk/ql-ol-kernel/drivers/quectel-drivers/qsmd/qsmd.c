/***************************************************
 *                             Add the smd file /dev/qsmd                       *
 *                                    add by sundy                                    *
 ***************************************************/
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/wait.h>
#include <linux/sched.h>
#include <linux/miscdevice.h>
#include <linux/workqueue.h>
#include <linux/uaccess.h>
#include <soc/qcom/smd.h>
#include <linux/string.h>




/* 2015/6/16 BUG#19545 */
//#define QUECTEL_BUG19545
#ifdef QUECTEL_BUG19545
#include <linux/delay.h>
#endif
#define MAX_BUF_SIZE 200

static DEFINE_MUTEX(qsmd_ch_lock);
static DEFINE_MUTEX(qsmd_rx_buf_lock);

static DECLARE_WAIT_QUEUE_HEAD(qsmd_wait_queue);

struct qsmd_device_t {
	struct miscdevice misc;

	struct smd_channel *ch;

	unsigned char rx_buf[MAX_BUF_SIZE];
	unsigned int bytes_read;
};

unsigned char tmp_buf[300];
unsigned int tmp = 0;

struct qsmd_device_t *qsmd_devp;

static void qsmd_work_func(struct work_struct *ws)
{
	int sz;
	for (;;) {
		sz = smd_cur_packet_size(qsmd_devp->ch);
		if (sz == 0)
			break;
		if (sz > smd_read_avail(qsmd_devp->ch))
			break;
		if (sz > MAX_BUF_SIZE) {
			smd_read(qsmd_devp->ch, 0, sz);
			continue;
		}

		mutex_lock(&qsmd_rx_buf_lock);
		if (smd_read(qsmd_devp->ch, qsmd_devp->rx_buf, sz) != sz) {
			mutex_unlock(&qsmd_rx_buf_lock);
			printk("\n\n======%s---%d==%d==%s===========\n\n",qsmd_devp->rx_buf,sz,__LINE__, __func__);
			printk(KERN_ERR "qsmd: not enough data?!\n");
			continue;
		}
		qsmd_devp->bytes_read = sz;
		tmp += qsmd_devp->bytes_read ;
		strcat(tmp_buf,qsmd_devp->rx_buf);
		printk("\n\n======%s---%d==%d==%s===========\n\n",qsmd_devp->rx_buf,sz,__LINE__, __func__);
		mutex_unlock(&qsmd_rx_buf_lock);
#ifdef QUECTEL_BUG19545
		mdelay(5);
#endif
		//wake_up_interruptible(&qsmd_wait_queue);
	}
}


static void qsmd_work_w_func(struct work_struct *ws)
{
	int wirte_avalib;

	for (;;) {
		wirte_avalib = smd_write_avail(qsmd_devp->ch);
		if (wirte_avalib < 0)
			break;

		mutex_lock(&qsmd_rx_buf_lock);
		if (smd_write(qsmd_devp->ch, qsmd_devp->rx_buf, wirte_avalib) != wirte_avalib) {
			mutex_unlock(&qsmd_rx_buf_lock);
			printk(KERN_ERR "qsmd: not enough space?!\n");
			continue;
		}
		mutex_unlock(&qsmd_rx_buf_lock);
		wake_up_interruptible(&qsmd_wait_queue);
	}
}


struct workqueue_struct *qsmd_wq;
static DECLARE_WORK(qsmd_work, qsmd_work_func);
static DECLARE_WORK(qsmd_work_w, qsmd_work_w_func);

static void qsmd_notify(void *priv, unsigned event)
{
	switch (event) {
		printk(KERN_ERR "[Sundy][SMD] %i:%s====%d:\n",  __LINE__,  __func__,event);
	case SMD_EVENT_DATA: {
		int sz;
		sz = smd_cur_packet_size(qsmd_devp->ch);
		printk("\n\n===%d===\n\n",sz);
		if ((sz > 0) && (sz <= smd_read_avail(qsmd_devp->ch)))
			queue_work(qsmd_wq, &qsmd_work);
		//if (smd_write_avail(qsmd_devp->ch))
			//queue_work(qsmd_wq, &qsmd_work_w);
		break;
	}
	case SMD_EVENT_OPEN:
		printk(KERN_INFO "qsmd: smd opened\n");
		break;
	case SMD_EVENT_CLOSE:
		printk(KERN_INFO "qsmd: smd closed\n");
		break;
	}
}

static ssize_t qsmd_write(struct file *fp, const char __user *buf,
                         size_t count, loff_t *pos)
{
	int res;
	int re_len;
    	int wirte_avalib;
		printk(KERN_ERR "[Sundy][SMD] ERROR:%s:%i:%s: "
                        "size(%d!=%d) err!\n",
                        __FILE__,
                        __LINE__,
                        __func__,
                        count,qsmd_devp->bytes_read);
	if(count >= MAX_BUF_SIZE)
      {
		printk(KERN_ERR "[Sundy][SMD] ERROR:%s:%i:%s: "
                        "size(%d!=%d) err!\n",
                        __FILE__,
                        __LINE__,
                        __func__,
                        count,qsmd_devp->bytes_read);
		return -1;
	}
	mutex_lock(&qsmd_rx_buf_lock);
	res = copy_from_user(qsmd_devp->rx_buf, buf, count);
	if(res)
    	{
		printk(KERN_ERR "[Sundy][SMD] ERROR:%s:%i:%s: "
                        "copy err!\n",
                        __FILE__,
                        __LINE__,
                        __func__);
		   memset(qsmd_devp->rx_buf, 0, MAX_BUF_SIZE);
		   mutex_unlock(&qsmd_rx_buf_lock);
                return -1;
	}
	wirte_avalib = smd_write_avail(qsmd_devp->ch);
	
	if(wirte_avalib > count)
	{
		re_len = smd_write(qsmd_devp->ch, qsmd_devp->rx_buf, count);
		printk(KERN_ERR "[Sundy] [SMD] line:%d, func=%s, re_len:%d\n", __LINE__, __func__, re_len);
		if(re_len < 0)
		{
			mutex_unlock(&qsmd_rx_buf_lock);
			memset(qsmd_devp->rx_buf, 0, MAX_BUF_SIZE);
			printk(KERN_ERR "[Sundy]============>>[SMD] %i:%s:\n",  __LINE__,  __func__);
			return -1;
		}
	}
	else if(wirte_avalib < count)
	{
		count = wirte_avalib;
		printk(KERN_ERR "[Sundy][SMD] %i:%s:\n",  __LINE__,  __func__);
		re_len = smd_write(qsmd_devp->ch, qsmd_devp->rx_buf, count);
		if(re_len < 0)
		{
			memset(qsmd_devp->rx_buf, 0, MAX_BUF_SIZE);
			mutex_unlock(&qsmd_rx_buf_lock);
			printk(KERN_ERR "[Sundy]============>>[SMD] %i:%s:\n",  __LINE__,  __func__);
			return -1;
		}
	}
	mutex_unlock(&qsmd_rx_buf_lock);
	//wake_up_interruptible(&qsmd_wait_queue);
	memset(qsmd_devp->rx_buf, 0, MAX_BUF_SIZE);
	return count;
}

static ssize_t qsmd_read(struct file *fp, char __user *buf,
			 size_t count, loff_t *pos)
{
	int r;
	int bytes_read;
	printk(KERN_ERR "[Sundy] [SMD] line:%d, func:%s, count:%d\n", __LINE__, __func__, count);
#if 0
	r = wait_event_interruptible(qsmd_wait_queue,
				qsmd_devp->bytes_read);
	if (r < 0) {
		/* qualify error message */
		if (r != -ERESTARTSYS) {
			/* we get this anytime a signal comes in */
			printk(KERN_ERR "ERROR:%s:%i:%s: "
				"wait_event_interruptible ret %i\n",
				__FILE__,
				__LINE__,
				__func__,
				r
				);
		}
		return r;
	}
#endif
	mutex_lock(&qsmd_rx_buf_lock);
	bytes_read = tmp;
	//printk("\n\n======%s---%d==%d==%s===========\n\n",qsmd_devp->rx_buf,qsmd_devp->bytes_read,__LINE__, __func__);
	//qsmd_devp->bytes_read = 0;
	r = copy_to_user(buf, tmp_buf, bytes_read);
	mutex_unlock(&qsmd_rx_buf_lock);

	qsmd_devp->bytes_read = 0;
	tmp = 0 ;
	memset(qsmd_devp->rx_buf,0,sizeof(qsmd_devp->rx_buf));
	memset(tmp_buf,0,sizeof(tmp_buf));

	if (r > 0) {
		printk(KERN_ERR "ERROR:%s:%i:%s: "
			"copy_to_user could not copy %i bytes.\n",
			__FILE__,
			__LINE__,
			__func__,
			r);
		return r;
	}
	//printk(KERN_ERR "[Sundy][SMD] %i:%s=====%d:\n",  __LINE__,  __func__,bytes_read);

	return bytes_read;
}

static int qsmd_open(struct inode *ip, struct file *fp)
{
	int r = 0;

	mutex_lock(&qsmd_ch_lock);
	if (qsmd_devp->ch == 0)
		r = smd_named_open_on_edge("DATA2", SMD_APPS_MODEM, &qsmd_devp->ch, qsmd_devp, qsmd_notify);
	mutex_unlock(&qsmd_ch_lock);

	return r;
}

static int qsmd_release(struct inode *ip, struct file *fp)
{
	int r = 0;

	mutex_lock(&qsmd_ch_lock);
	if (qsmd_devp->ch != 0) {
		r = smd_close(qsmd_devp->ch);
		qsmd_devp->ch = 0;
	}
	mutex_unlock(&qsmd_ch_lock);

	return r;
}

static const struct file_operations qsmd_fops = {
	.owner = THIS_MODULE,
	.read = qsmd_read,
	.write = qsmd_write,
	.open = qsmd_open,
	.release = qsmd_release,
};

static struct qsmd_device_t qsmd_device = {
	.misc = {
		.minor = MISC_DYNAMIC_MINOR,
		.name = "qsmd",
		.fops = &qsmd_fops,
	}
};

static void __exit qsmd_exit(void)
{
	destroy_workqueue(qsmd_wq);
	misc_deregister(&qsmd_device.misc);
}

static int __init qsmd_init(void)
{
	int ret;

      printk("[Sunyd]==========>>[SMD] qsmd_init running...\r\n");
	qsmd_device.bytes_read = 0;
	qsmd_devp = &qsmd_device;

	qsmd_wq = create_singlethread_workqueue("qsmd");
	if (qsmd_wq == 0)
		return -ENOMEM;

	ret = misc_register(&qsmd_device.misc);
	return ret;
}

module_init(qsmd_init);
module_exit(qsmd_exit);

MODULE_DESCRIPTION("QUECTEL Shared Memory SMD Driver");
MODULE_LICENSE("GPL v2");

