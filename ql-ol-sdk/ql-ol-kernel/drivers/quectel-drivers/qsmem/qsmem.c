/* Modification contents:
   [ID] [Name] [Date] [SearchFlag] : [ForWhat]. just like below:
   [066] [MaxTANG] [2016-06-06] [maxcodeflag20160606] : [solve a major bug that will be cause memory leak] 
*/

/* - - - - - - - - - - - - - - Modification Descriptions - - - - - - - - - - - - - - 
[000] [MaxTANG] [2016-06-16] [maxcodeflag20160616] : [调整smem初始化机制，由主动查询改为由modem主动上报SMEM ready
状态后，再进行初始化操作。优化锁机制，避免在无进程上下文的情况进行LOCK/UNLOCK操作]
*/


#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/wait.h>
#include <linux/sched.h>
#include <linux/miscdevice.h>
#include <linux/workqueue.h>
#include <linux/uaccess.h>
#include <soc/qcom/smem.h>


#define QSMEM_DATASIZE   1000
#define ALIVE_TIME_MAX   10 
#define QSMEM_READ_CHECK_INTERVAL (jiffies + (HZ/4))
/*maxcodeflag20160616 -begin*/
#define MAGIC_NUM	'l'

#define QSMEM_IOCTL_GET_RDY_STATE _IOWR(MAGIC_NUM, 1, unsigned int)
#define QSMEM_IOCTL_SET_RDY_STATE  _IOW(MAGIC_NUM, 2, unsigned int)
static unsigned int qsmem_ready_state = 0;

extern void qsmem_poll_timer_continue(void);
/*maxcodeflag20160616 -end*/

extern void *smem_find(unsigned id, unsigned size_in, unsigned to_proc, unsigned flags);

/* smem buf define */
typedef struct qsmem_buf
{
    unsigned int  qsmem_msg_spinlock;
    unsigned char Status; /* 'A'-APPS write,'M'-Modem wirte,'H'-Handle,'C'-Clean */
    unsigned char AliveTime;
    unsigned int  MsgId;
    unsigned int  DataLen;
    unsigned char Data[QSMEM_DATASIZE];
}QSMEM_BUF;

static DEFINE_MUTEX(qsmem_rx_buf_lock);
static DECLARE_WAIT_QUEUE_HEAD(qsmem_wait_queue);

struct qsmem_device_t {
	struct miscdevice misc;
	unsigned char condition;
	unsigned char time_run;
};

struct qsmem_device_t *qsmem_devp;
QSMEM_BUF *qsmem_addr=NULL;
struct timer_list qsmem_poll_timer;
static bool alive_time_start = false;

static int qsmem_check_addr(void)
{
    if(NULL != qsmem_addr)
    {
        return 0;
    }
    
    qsmem_addr = smem_find(SMEM_ID_VENDOR1,sizeof(QSMEM_BUF),SMEM_MODEM,0);
	if(!qsmem_addr)
    {
        printk("[Max][SMEM][%s] cannot find SMEM_ID_VENDOR1\n", __func__);
        return -1; 
    }
    else
    {
		/*maxcodeflag2016061*/
		if(0 == qsmem_ready_state)
		{
			qsmem_ready_state = 1;
			printk("[Max][SMEM][%s] find SMEM_ID_VENDOR1 OK!\n", __func__);
		}
    }
    return 0;
}

#define CHECK_ADDR_RETURN \
{ \
    if(0 != qsmem_check_addr()) \
    { \
        printk("[Max][SMEM][%s][%d] CHECK QSMEM ADDR FAILED!\n", __func__, __LINE__); \
        return ; \
    } \
}

#define CHECK_ADDR_RETURN_VAL(X) \
{ \
    if(0 != qsmem_check_addr()) \
    { \
        printk("[Max][SMEM][%s][%d] CHECK QSMEM ADDR FAILED!\n", __func__, __LINE__); \
        return X; \
    } \
}

static void qsmem_poll_timer_cb(void)
{
	printk("[Max][SMEM] [%s:%d] Status=[%c],AliveTime=%d\,condition=%d\n",
		   __func__,__LINE__,qsmem_addr->Status,qsmem_addr->AliveTime,qsmem_devp->condition);
	
	if(qsmem_addr && qsmem_addr->Status == 'M')
    {
        alive_time_start = true;
//		mutex_lock(&qsmem_rx_buf_lock);
		qsmem_addr->Status = 'H';
		qsmem_addr->AliveTime = 0;
//		mutex_unlock(&qsmem_rx_buf_lock);
		qsmem_devp->condition = 1;
        wake_up_interruptible(&qsmem_wait_queue);
		printk("[Max][SMEM] %s:%d Modem Msg received.\n",__func__,__LINE__);
	}
    
	if(alive_time_start)
    {
//      mutex_lock(&qsmem_rx_buf_lock);
        if(qsmem_addr->AliveTime >= ALIVE_TIME_MAX)
        {
			alive_time_start = false;
			qsmem_devp->condition = 1;
			qsmem_addr->Status = 'C';
            wake_up_interruptible(&qsmem_wait_queue);
			printk("[Max][SMEM] %s:%d Modem rsp timeout.\n",__func__,__LINE__);
		}
//      mutex_unlock(&qsmem_rx_buf_lock);
        qsmem_poll_timer_continue();
	}
}

/*maxcodeflag2016061-begin*/

void qsmem_poll_timer_start(void)
{
	init_timer(&qsmem_poll_timer);
    qsmem_poll_timer.function = (void *)qsmem_poll_timer_cb;
    qsmem_poll_timer.expires = QSMEM_READ_CHECK_INTERVAL;
    add_timer(&qsmem_poll_timer);
	alive_time_start = true;
	qsmem_addr->AliveTime = 0;
}

void qsmem_poll_timer_continue(void)
{
	mod_timer(&qsmem_poll_timer, QSMEM_READ_CHECK_INTERVAL);
	qsmem_addr->AliveTime++;
}

void qsmem_poll_timer_del(void)
{
	del_timer_sync(&qsmem_poll_timer);
	alive_time_start = false;
}

static long qsmem_unlocked_ioctl (struct file *fp, unsigned int cmd, unsigned long arg)
{
	unsigned int tmp_state = 0;
	void __user *argp = (void __user *)arg;
	
	if(NULL == fp)
	{
		printk("[Max][SMEM] [%s][%d] ERROR: fp is NULL!\n", __func__, __LINE__);
		return -EINVAL;
	}
	
	switch(cmd)
	{
		case QSMEM_IOCTL_GET_RDY_STATE:
		{
			if(copy_to_user(argp, &qsmem_ready_state, sizeof(qsmem_ready_state)))
			{
				printk(KERN_ERR "[Max][SMEM][%s][%d] ERROR! QSMEM_IOCTL_GET_RDY_STATE: failed to copy data to user[qsmem_ready_state=%d]\n",__func__,__LINE__,qsmem_ready_state);
				return -EFAULT;
			}
			else
			{
				printk(KERN_ERR "[Max][SMEM][%s][%d] OK! QSMEM_IOCTL_GET_RDY_STATE: successful to copy data to user[qsmem_ready_state=%d]\n",__func__,__LINE__,qsmem_ready_state);
			}
			break;
		}
		case QSMEM_IOCTL_SET_RDY_STATE:
		{
			tmp_state = qsmem_ready_state;
			if(copy_from_user(&qsmem_ready_state, argp, sizeof (qsmem_ready_state)))
			{
				printk(KERN_ERR "[Max][SMEM][%s][%d] ERROR! QSMEM_IOCTL_SET_RDY_STATE: failed to copy data to user[qsmem_ready_state=%d]\n",__func__,__LINE__,qsmem_ready_state);
				return -EFAULT;
			}
			else
			{
				printk(KERN_ERR "[Max][SMEM][%s][%d] OK! QSMEM_IOCTL_SET_RDY_STATE: successful to copy data to user[qsmem_ready_state=%d,pre_state=%d]\n",__func__,__LINE__,qsmem_ready_state,tmp_state);
			}

			if((0==tmp_state) && (1 == qsmem_ready_state))
			{
				qsmem_addr = smem_find(SMEM_ID_VENDOR1,sizeof(QSMEM_BUF),SMEM_MODEM,0);
				if(!qsmem_addr)
				{
					printk("[Max][SMEM] cannot find SMEM_ID_VENDOR1 even if qsmem ready notification has come\r\n");
					qsmem_ready_state = 0;
				}
				else
				{
					printk("[Max][SMEM] find SMEM_ID_VENDOR1 OK when qsmem ready notification has come\r\n");
				}
			}
			break;
		}
		default:
		{
			printk(KERN_ERR "[Max][SMEM][%s][%d] ERROR! unkown cmd:%d\n",__func__,__LINE__,cmd);
			return -EINVAL;
		}
	}

	return 0;
}
/*maxcodeflag2016061-end*/

static ssize_t qsmem_read(struct file *fp, char __user *buf, size_t count, loff_t *pos)
{
	int r;
	int bytes_read;
    
    CHECK_ADDR_RETURN_VAL(-1);
	
	qsmem_poll_timer_start();
	
	printk("[Max][SMEM] qsmem_read wait - begin\n");
	r = wait_event_interruptible(qsmem_wait_queue, qsmem_devp->condition);
	printk("[Max][SMEM] qsmem_read wait end and del time![r=%d]\n",r);
	qsmem_poll_timer_del();
	if (r < 0)
    {
        if(r != -ERESTARTSYS)
        {
            printk("[Max][SMEM] ERROR:%s:%i: wait_event_interruptible ret %i\n", __func__, __LINE__, r);
		}
        
		mutex_lock(&qsmem_rx_buf_lock);
        qsmem_devp->condition = 0;
        qsmem_addr->Status = 'C';
        mutex_unlock(&qsmem_rx_buf_lock);

        return r;
	}
    
	if(qsmem_addr->AliveTime >= ALIVE_TIME_MAX)
    {
		mutex_lock(&qsmem_rx_buf_lock);
		qsmem_devp->condition = 0;
		qsmem_addr->Status = 'C';
		mutex_unlock(&qsmem_rx_buf_lock);
		printk("[Max][SMEM] %s:%d AliveTime:%d ret:%d.\n",__func__,__LINE__,qsmem_addr->AliveTime,-ETIMEDOUT);
 		return -ETIMEDOUT;
	}

	mutex_lock(&qsmem_rx_buf_lock);
	qsmem_devp->condition = 0;
	bytes_read = sizeof(QSMEM_BUF);
	r = copy_to_user(buf, qsmem_addr, bytes_read);
	qsmem_addr->Status = 'C';
	mutex_unlock(&qsmem_rx_buf_lock);

	if (r > 0) 
	{
		printk("[Max][SMEM] ERROR:%s:%i: copy_to_user could not copy %i bytes.\n", __func__, __LINE__, r);
		return r;
	}

	return bytes_read;
}
static ssize_t qsmem_write(struct file *fp, const char __user *buf, size_t count, loff_t *pos)
{
	int res;

    CHECK_ADDR_RETURN_VAL(-1);
    
	if(count != sizeof(QSMEM_BUF))
    {
		printk("[Max][SMEM] ERROR:%s:%i: size(%d!=%d) err!\n", __func__, __LINE__, count, sizeof(QSMEM_BUF));
		return -1;
	}
	
	if(qsmem_addr->Status != 'C')
    {
		printk("[Max][SMEM] ERROR:%s:%i qsmem status is %c\n", __func__,__LINE__,qsmem_addr->Status);
		return -1;
	}
    
	mutex_lock(&qsmem_rx_buf_lock);
	res = copy_from_user(qsmem_addr, buf, count);
	if(0 == res)
	{
		qsmem_addr->Status = 'A';
	}
	mutex_unlock(&qsmem_rx_buf_lock);
	printk("[Max][SMEM] qsmem_write over! res=%d\n", res);
    if(res)
    {
		printk("[Max][SMEM] ERROR:%s:%i:%d: copy err!\n", __func__, __LINE__, res);
		return -1;
	}
	return count;
}

static const struct file_operations qsmem_fops = {
	.owner = THIS_MODULE,
	.read = qsmem_read,
	.write = qsmem_write,
	.unlocked_ioctl = qsmem_unlocked_ioctl, /*maxcodeflag2016061*/
};

static struct qsmem_device_t qsmem_device = {
	.misc = {
		.minor = MISC_DYNAMIC_MINOR,
		.name = "qsmem",
		.fops = &qsmem_fops,
	}
};

static void __exit qsmem_exit(void)
{
	misc_deregister(&qsmem_device.misc);
}

static int __init qsmem_init(void)
{
	int ret;
	
	qsmem_device.condition = 0;
	qsmem_devp = &qsmem_device;
	ret = misc_register(&qsmem_device.misc);
    printk("[Max][SMEM] qsmem_init running: misc_register return %d\r\n", ret);
	
	return ret;
}


module_init(qsmem_init);
module_exit(qsmem_exit);

MODULE_DESCRIPTION("QUECTEL Shared Memory Driver");
MODULE_LICENSE("GPL v2");

