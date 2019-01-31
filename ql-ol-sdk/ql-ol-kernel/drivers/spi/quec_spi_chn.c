/*
 * spidev.c -- simple synchronous userspace interface to SPI devices
 *
 * Copyright (C) 2006 SWAPP
 *	Andrea Paterniani <a.paterniani@swapp-eng.it>
 * Copyright (C) 2007 David Brownell (simplification, cleanup)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/ioctl.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/list.h>
#include <linux/errno.h>
#include <linux/mutex.h>
#include <linux/completion.h>
#include <linux/slab.h>
#include <linux/compat.h>
#include <linux/spi/spi.h>
#include <linux/spi/spidev.h>
#include <linux/kfifo.h>
#include <linux/poll.h>
#include <asm/uaccess.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/workqueue.h>
#include <linux/io.h>
#include <linux/debugfs.h>
#include <asm/atomic.h>
#include <linux/mutex.h>
#include <linux/gpio.h>
//#include <mach/gpiomux.h>

#ifndef INIT_COMPLETION
#define INIT_COMPLETION(x) reinit_completion(&(x))
#endif
#ifndef MIN
#define	MIN(a, b)		(((a) < (b))?(a):(b))
#endif
#define SPI_MAX_CHN 8

/* Set only the required bits */
#define BIT_SET(var, shift, mask, val) (var = ((var) & ~((mask) << (shift))) | ((val) << (shift)))
#define BIT_GET(var, shift, mask) (((var >> shift) & mask))
#define QMSG_CTS_BIT 0
#define QMSG_GET_CHN(msg) ((msg->chn_len1 & 0xF0) >> 4)
#define QMSG_SET_CHN(msg, chn) do { BIT_SET(msg->chn_len1, 4, 0xF, chn); } while(0)
#define QMSG_GET_LEN(msg) (((msg->chn_len1 & 0xF) << 8) | msg->len2)
#define QMSG_SET_LEN(msg, len) do { BIT_SET(msg->chn_len1, 0, 0xF, ((len>>8) & 0xF)); msg->len2 = (len & 0xFF); } while(0)

struct quectel_msg {
    u8 flag;  //0x5a
    u8 control;
    u8 chn_len1;
    u8 len2;
    u8 rev[4];	//20170500,add by cullen for intest mcu spi bug
    u8 data[0];
};

struct quectel_channel_info {
    unsigned int ch_num;
    unsigned int users;
    unsigned long control;
    unsigned int kfifo_initialized;
    struct kfifo tx_fifo;
    struct mutex tx_mutex;
    wait_queue_head_t tx_wq;
    struct kfifo rx_fifo;
    struct list_head tx_queue;
    int tx_busy;
    struct mutex rx_mutex;
    wait_queue_head_t rx_wq;
    struct device *dev;
};

struct quectel_spidev {
    struct spi_device *spidev;
    spinlock_t dev_lock;
    struct mutex dev_mutex;
    struct list_head msg_queue;
    struct workqueue_struct *msg_workqueue;
    struct work_struct msg_work;
    struct mutex msg_work_mutex;
    struct completion tx_done;
    bool slave_ready_flag;
    wait_queue_head_t slave_ready_wq;
    struct quectel_msg *tx_msg;
    struct quectel_msg *rx_msg;
    int gpio_master_ready;
    int gpio_slave_ready;
    int irq_slave_enabled;
    int irq_slave_ready;
    int major;
   struct quectel_channel_info ch_info[SPI_MAX_CHN];
};

static struct quectel_spidev *ql_spidev;
static unsigned int frame_size = 512;
static unsigned int speed_hz = 9600000;
static unsigned int spi_mode = SPI_MODE_0;
static unsigned int debug = 0x00;
static unsigned int rev_enable=false;	//20170522, add by cullen  for intest mcu spi bug ,default:disable reserve bytes 
module_param(frame_size, uint, 0644);
module_param(speed_hz, uint, 0644);
module_param(spi_mode, uint, 0644);
module_param(debug, uint, 0644);
module_param(rev_enable,uint,0644);

static int busnum = 6;
static int chipselect = 0;
static int gpiomodemready = 34;
static int gpiomcuready = 52;
module_param(busnum, int, 0644);
module_param(chipselect, int, 0644);
module_param(gpiomodemready, int, 0644);
module_param(gpiomcuready, int, 0644);

static int spidev_get_slave_ready(struct quectel_spidev *ql_spidev) {
    return gpio_get_value(ql_spidev->gpio_slave_ready) && ql_spidev->slave_ready_flag;
}

static void spidev_set_master_ready(struct quectel_spidev *ql_spidev, int ready) {
    gpio_set_value(ql_spidev->gpio_master_ready, !!ready);
}

static irqreturn_t spidev_slave_ready_irq_handler(int irq, void *para)  {
    struct quectel_spidev *ql_spidev = (struct quectel_spidev *)para;
    ql_spidev->slave_ready_flag = true;
    wake_up_interruptible(&ql_spidev->slave_ready_wq);
    queue_work(ql_spidev->msg_workqueue, &ql_spidev->msg_work);
    return IRQ_HANDLED;
}

static void spidev_complete(void *arg) {
    complete(arg);
}

/* workqueue - pull messages from queue & process */
static void spidev_workq(struct work_struct *work) {
    struct quectel_spidev *ql_spidev =  container_of(work, struct quectel_spidev, msg_work);
    int status;
    unsigned long flags;

    mutex_lock(&ql_spidev->msg_work_mutex);
    if (debug & 0x01) printk("+++ %s\n", __func__);

    while (1) {
        //unsigned long long master_ready_usec;
        struct quectel_channel_info *tx_ch_info = NULL;

        spin_lock_irqsave(&ql_spidev->dev_lock, flags);
        if (!list_empty(&ql_spidev->msg_queue)) {
            tx_ch_info = list_entry(ql_spidev->msg_queue.next, struct quectel_channel_info, tx_queue);
            if (kfifo_len(&tx_ch_info->tx_fifo) <= (frame_size - sizeof(struct quectel_msg))) {
                list_del_init(&tx_ch_info->tx_queue);
                tx_ch_info->tx_busy = 0;
            }
        }
        spin_unlock_irqrestore(&ql_spidev->dev_lock, flags);

        if (!tx_ch_info && !spidev_get_slave_ready(ql_spidev)) {
            break;
        }
        //printk("tx_ch %d\n", tx_ch_info ? tx_ch_info->ch_num : 0);

        spidev_set_master_ready(ql_spidev, 1);
        //master_ready_usec = cpu_clock(0) / 1024;
        status = wait_event_interruptible(ql_spidev->slave_ready_wq, spidev_get_slave_ready(ql_spidev));
        if (status == 0) {
            struct spi_transfer t = {
                .tx_buf = ql_spidev->tx_msg,
                .rx_buf = ql_spidev->rx_msg,
                .len = frame_size,
                //.delay_usecs = 0,
                //.speed_hz = tx_spidev->spidev->max_speed_hz,
                //.bits_per_word = tx_spidev->spidev->bits_per_word,
            };
            struct spi_message m;

            spi_message_init(&m);
            spi_message_add_tail(&t, &m);
            m.complete = spidev_complete;
            m.context = &ql_spidev->tx_done;

            ql_spidev->tx_msg->flag = 0x5a;
            ql_spidev->tx_msg->control = 0x00;
            QMSG_SET_CHN(ql_spidev->tx_msg, 0);
            QMSG_SET_LEN(ql_spidev->tx_msg, 0);
           
			//20170522 add by cullen for intest mcu spi bug
	   		 if(rev_enable == true) {
                ql_spidev->tx_msg->rev[0] = ql_spidev->tx_msg->rev[1] = ql_spidev->tx_msg->rev[2] = ql_spidev->tx_msg->rev[3] = 0;
	   		 }
           
            if (tx_ch_info) {
                int tx_len = MIN(kfifo_len(&tx_ch_info->tx_fifo), (frame_size - sizeof(struct quectel_msg)));
                if (tx_len) {
                    int kfifo_out_ret;
                    QMSG_SET_LEN(ql_spidev->tx_msg, tx_len);
		   			//20170522 add by cullen for mcu spi bug
					if(rev_enable==true)
                		kfifo_out_ret = kfifo_out(&tx_ch_info->tx_fifo, ql_spidev->tx_msg->data, tx_len);
		    		else
						kfifo_out_ret = kfifo_out(&tx_ch_info->tx_fifo, ql_spidev->tx_msg->rev, tx_len);
                    wake_up_interruptible(&tx_ch_info->tx_wq);
                }
                QMSG_SET_CHN(ql_spidev->tx_msg, tx_ch_info->ch_num);
                ql_spidev->tx_msg->control = tx_ch_info->control & 0xFF;
            } else {
                memset(ql_spidev->tx_msg->data, 0x00, 8);
            }

            //printk("> flag: %02x control: %02x ch: %x len: %d\n", ql_spidev->tx_msg->flag,
            //    ql_spidev->tx_msg->control, QMSG_GET_CHN(ql_spidev->tx_msg), QMSG_GET_LEN(ql_spidev->tx_msg));
             //make gpio_master_ready keep high at least 3 usec, make sure slave can receive falling interrupt!
            //while ((master_ready_usec + 3) > (cpu_clock(0) / 1024))
            //    udelay(1);
            ql_spidev->slave_ready_flag = false;
            spidev_set_master_ready(ql_spidev, 0);

            status = spi_async(ql_spidev->spidev, &m);
            if (status == 0) {
                wait_for_completion(&ql_spidev->tx_done);
                status = m.status;
                if (status == 0) {			
                    if (debug & 0x01) {
                        unsigned char *recv = (unsigned char *)ql_spidev->rx_msg;
                        printk("< %02x %02x %02x %02x %02x %02x %02x %02x\n", recv[0],recv[1],recv[2],recv[3],recv[4],recv[5],recv[6],recv[7]);
                    }
                    if (debug & 0x8000) {
                        if (QMSG_GET_CHN(ql_spidev->rx_msg) == 4)
                            QMSG_SET_CHN(ql_spidev->rx_msg, 1);
                        else if (QMSG_GET_CHN(ql_spidev->rx_msg) == 1)
                            QMSG_SET_CHN(ql_spidev->rx_msg, 4);
                    }

                    if (/*m.actual_length && */(ql_spidev->rx_msg->flag == 0x5a) && (QMSG_GET_CHN(ql_spidev->rx_msg) < SPI_MAX_CHN)
                        && (QMSG_GET_LEN(ql_spidev->rx_msg) <= (frame_size - sizeof(struct quectel_msg)))) {
                        struct quectel_channel_info *rx_ch_info = NULL;
                        //ql_spidev->rx_msg->ch = jiffies % SPI_NUM_CHIPSELECTS;
                        rx_ch_info = &ql_spidev->ch_info[QMSG_GET_CHN(ql_spidev->rx_msg)];
                        if (rx_ch_info && rx_ch_info->kfifo_initialized) {
                            if (QMSG_GET_LEN(ql_spidev->rx_msg) == 0) {
                            
                            } else if (QMSG_GET_LEN(ql_spidev->rx_msg) <= kfifo_avail(&rx_ch_info->rx_fifo)) {
						//20170522 add by cullen for intest mcu spi bug
                                if(rev_enable == true)
                                    kfifo_in(&rx_ch_info->rx_fifo, ql_spidev->rx_msg->data, QMSG_GET_LEN(ql_spidev->rx_msg));
				else
				    kfifo_in(&rx_ch_info->rx_fifo, ql_spidev->rx_msg->rev, QMSG_GET_LEN(ql_spidev->rx_msg));
                                wake_up_interruptible(&rx_ch_info->rx_wq);
                            } else {
                                if (debug & 0x04) dev_info(rx_ch_info->dev,  "over run!\n");
                            }

                            if (kfifo_avail(&rx_ch_info->rx_fifo) < (frame_size * 2)) {
                                if (!test_and_set_bit(QMSG_CTS_BIT, &rx_ch_info->control)) {
                                    if (debug & 0x02) dev_info(rx_ch_info->dev,  "CTS %d\n", test_bit(QMSG_CTS_BIT, &rx_ch_info->control));
                                    spin_lock_irqsave(&ql_spidev->dev_lock, flags);
                                    if (!rx_ch_info->tx_busy) {
                                        list_add(&rx_ch_info->tx_queue, &ql_spidev->msg_queue);
                                        rx_ch_info->tx_busy = 1;
                                    } else {
                                        list_move(&rx_ch_info->tx_queue, &ql_spidev->msg_queue);
                                    }
                                    spin_unlock_irqrestore(&ql_spidev->dev_lock, flags);
                                }
                            }
                        }
                        else {
                            if (debug & 0x04) dev_info(rx_ch_info->dev,  "no users\n");
                        }
                    }
                    else {
                       if (debug & 0x04) printk("< flag: %02x control: %02x ch: %x len: %d\n", ql_spidev->rx_msg->flag,
                            ql_spidev->rx_msg->control, QMSG_GET_CHN(ql_spidev->rx_msg), QMSG_GET_LEN(ql_spidev->rx_msg));
                    }
                }
                else {
                    printk("m.status = %d\n", m.status);
                }
            }
            else {
                printk("spi_async = %d\n", status);
            }
        }
        else {
            printk("wait_event_interruptible = %d\n", status);
            break;
        }
    }

    if (debug & 0x01) printk("--- %s\n", __func__);
    mutex_unlock(&ql_spidev->msg_work_mutex);
}

/*-------------------------------------------------------------------------*/
static unsigned int spidev_poll(struct file *filp, struct poll_table_struct *wait) {
    struct quectel_channel_info *ch_info = (struct quectel_channel_info *)  filp->private_data;
    unsigned int mask = 0;

    mutex_lock(&ch_info->tx_mutex);
    poll_wait(filp, &ch_info->tx_wq, wait);
    if(!kfifo_is_full(&ch_info->tx_fifo))
        mask |=  POLLOUT;
    mutex_unlock(&ch_info->tx_mutex);

    mutex_lock(&ch_info->rx_mutex);
    poll_wait(filp, &ch_info->rx_wq, wait);
    if(!kfifo_is_empty(&ch_info->rx_fifo))
        mask |=  POLLIN;
    mutex_unlock(&ch_info->rx_mutex);

    if (debug & 0x01) dev_info(ch_info->dev, "%s mask %x\n", __func__, mask);

    return mask;
}

/* Read-only message with current device setup */
static ssize_t spidev_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos) {
    struct quectel_channel_info *ch_info = (struct quectel_channel_info *)  filp->private_data;
    int status = 0, copied;

    mutex_lock(&ch_info->rx_mutex);
    if (!(filp->f_mode & FMODE_NDELAY)) {
        while (kfifo_is_empty(&ch_info->rx_fifo)) {
            int wait_ret;
            mutex_unlock(&ch_info->rx_mutex);
            wait_ret = wait_event_interruptible(ch_info->rx_wq, !kfifo_is_empty(&ch_info->rx_fifo));
            if (wait_ret) {
                if(debug & 0x01) dev_info(ch_info->dev, "%s wait_ret = %d\n", __func__, wait_ret);
                return -EINTR;
            }
            mutex_lock(&ch_info->rx_mutex);
        }
    }
    status = kfifo_to_user(&ch_info->rx_fifo, buf, count, &copied);
    if (!status)
        status = copied;
    if (test_bit(QMSG_CTS_BIT, &ch_info->control) && (kfifo_avail(&ch_info->rx_fifo) > (frame_size * 2))) {
        unsigned long flags;
        //int add_list = 0;
        clear_bit(QMSG_CTS_BIT, &ch_info->control);
        spin_lock_irqsave(&ql_spidev->dev_lock, flags);
        if (!ch_info->tx_busy) {
            list_add_tail(&ch_info->tx_queue, &ql_spidev->msg_queue);
            ch_info->tx_busy = 1;
            //add_list = 1;
        }
        spin_unlock_irqrestore(&ql_spidev->dev_lock, flags);
        //if (add_list)
        //    printk("%s list_add_tail %d\n", __func__, ch_info->ch_num);
        queue_work(ql_spidev->msg_workqueue, &ql_spidev->msg_work);
        if (debug & 0x02) dev_info(ch_info->dev,  "CTS %d\n", test_bit(QMSG_CTS_BIT, &ch_info->control));
    }
    if (debug & 0x01) dev_info(ch_info->dev, "%s status = %d, %d / %d\n", __func__, status,
        kfifo_len(&ch_info->rx_fifo), kfifo_avail(&ch_info->rx_fifo));
    mutex_unlock(&ch_info->rx_mutex);

    return status;
}

/* Write-only message with current device setup */
static ssize_t spidev_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos) {
    struct quectel_channel_info *ch_info = (struct quectel_channel_info *)  filp->private_data;
    int status = 0, copied;

    mutex_lock(&ch_info->tx_mutex);
    if (debug & 0x01) dev_info(ch_info->dev, "%s count = %ld\n", __func__, (long)count);
    if (!(filp->f_mode & FMODE_NDELAY)) {
        while (kfifo_is_full(&ch_info->tx_fifo)) {
            int wait_ret;
			mutex_unlock(&ch_info->tx_mutex);
            wait_ret = wait_event_interruptible(ch_info->tx_wq, !kfifo_is_full(&ch_info->tx_fifo));
            if (wait_ret) {
				if(debug & 0x01) dev_info(ch_info->dev, "%s wait_ret = %d\n", __func__, wait_ret);
                return -EINTR;
            }
			mutex_lock(&ch_info->tx_mutex);
        }
    }
    status = kfifo_from_user(&ch_info->tx_fifo, buf, count, &copied);
    if (!status) {
        status = copied;
        if (copied > 0) {
            unsigned long flags;
            //int add_list = 0;
            spin_lock_irqsave(&ql_spidev->dev_lock, flags);
            if (!ch_info->tx_busy) {
                list_add_tail(&ch_info->tx_queue, &ql_spidev->msg_queue);
                ch_info->tx_busy = 1;
                //add_list = 1;
            }
            spin_unlock_irqrestore(&ql_spidev->dev_lock, flags);
            //if (add_list)
            //    printk("%s list_add_tail %d\n", __func__, ch_info->ch_num);
            queue_work(ql_spidev->msg_workqueue, &ql_spidev->msg_work);
        }
    }
    if (debug & 0x01) dev_info(ch_info->dev, "%s status = %d, %d / %d\n", __func__, status,
        kfifo_len(&ch_info->tx_fifo), kfifo_avail(&ch_info->tx_fifo));
    mutex_unlock(&ch_info->tx_mutex);

    return status;
}

static int spidev_open(struct inode *inode, struct file *filp) {
    struct quectel_channel_info *ch_info = &ql_spidev->ch_info[MINOR(inode->i_rdev)];
    int status = 0;


    mutex_lock(&ql_spidev->dev_mutex);
    if (debug & 0x01) dev_info(ch_info->dev, "+++ %s\n", __func__);

    if (!ch_info->kfifo_initialized) {
        status = kfifo_alloc(&ch_info->tx_fifo, PAGE_SIZE, GFP_KERNEL);
        if (!status) {
            //by now, channel 1 is used for AT, channel 2 used for FILE, channel 3 for WIFI
            int rx_fifo_size = PAGE_SIZE;
            if ((ch_info->ch_num == 2) || (ch_info->ch_num == 3))
                rx_fifo_size = PAGE_SIZE * 4;
            status = kfifo_alloc(&ch_info->rx_fifo, rx_fifo_size, GFP_KERNEL);
            if (!status)
                ch_info->kfifo_initialized = 1;
            else {
                kfifo_free(&ch_info->tx_fifo);
                dev_err(ch_info->dev, "kfifo_alloc rx_fifo status = %d\n", status);
            }
        } else {
            dev_err(ch_info->dev, "kfifo_alloc tx_fifo status = %d\n", status);
        }
    }

    if (status == 0) {
        ch_info->users++;
        filp->private_data = ch_info;
        nonseekable_open(inode, filp);
    }

    //20170527 add by cullen for intest control logic ,move interrupt from probe  stage to open stage
    if (ql_spidev->irq_slave_enabled == 0)
	enable_irq(ql_spidev->irq_slave_ready);
    ql_spidev->irq_slave_enabled++;
    
    mutex_unlock(&ql_spidev->dev_mutex);

    return status;
}

static int spidev_release(struct inode *inode, struct file *filp) {
    struct quectel_channel_info *ch_info = (struct quectel_channel_info *)  filp->private_data;
    int status = 0;

    mutex_lock(&ql_spidev->dev_mutex);
    mutex_lock(&ch_info->tx_mutex);
    mutex_lock(&ch_info->rx_mutex);
    if (debug & 0x01) dev_info(ch_info->dev, "+++ %s\n", __func__);
    filp->private_data = NULL;
    ch_info->users--;
    if (!ch_info->users) {
       //kfifo_free(&ch_info->tx_fifo);
       //kfifo_free(&ch_info->rx_fifo);
       //ch_info->kfifo_initialized = 0;
    }

    mutex_unlock(&ch_info->rx_mutex);
    mutex_unlock(&ch_info->tx_mutex);

    //20170527 add by cullen for intest app close state GPIO/memory config
    spidev_set_master_ready(ql_spidev, 0);

    if (ql_spidev->irq_slave_enabled) {
        ql_spidev->irq_slave_enabled--;
        if (ql_spidev->irq_slave_enabled == 0)
            disable_irq(ql_spidev->irq_slave_ready);
    }

    mutex_unlock(&ql_spidev->dev_mutex);

    return status;
}

static const struct file_operations spidev_fops = {
    .owner =	THIS_MODULE,
    .write =	spidev_write,
    .read =    	spidev_read,
    .poll = spidev_poll,
    .open =    	spidev_open,
    .release =	spidev_release,
    .llseek =	no_llseek,
};


static struct class *spidev_class;
static int spidev_probe(struct spi_device *spi) {
    int status;
    int i;

    dev_info(&spi->dev, "tony--------------+++ %s\n", __func__);
    mutex_lock(&ql_spidev->dev_mutex);
    if (debug & 0x01) dev_info(&spi->dev, "+++ %s\n", __func__);
    ql_spidev->spidev = spi;
    spi_set_drvdata(spi, ql_spidev);

    spi->mode = spi_mode;
    spi->max_speed_hz = speed_hz;
    status = spi_setup(spi);
    dev_info(&spi->dev, "setup mode %d, %s%s%s%s%u bits/w, %u Hz max --> %d\n",
    		(int) (spi->mode & (SPI_CPOL | SPI_CPHA)),
    		(spi->mode & SPI_CS_HIGH) ? "cs_high, " : "",
    		(spi->mode & SPI_LSB_FIRST) ? "lsb, " : "",
    		(spi->mode & SPI_3WIRE) ? "3wire, " : "",
    		(spi->mode & SPI_LOOP) ? "loopback, " : "",
    		spi->bits_per_word, spi->max_speed_hz,
    		status);

    for (i = 0; i < SPI_MAX_CHN; i++) {
        struct quectel_channel_info *ch_info = &ql_spidev->ch_info[i];
        struct device *dev = NULL;

        dev = device_create(spidev_class, &spi->dev, MKDEV(ql_spidev->major, i), ql_spidev, "spi%d_%d_%d", spi->master->bus_num, spi->chip_select, i);
        status = IS_ERR(dev) ? PTR_ERR(dev) : 0;
        if (status)
            break;
        ch_info->dev = dev;
        ch_info->ch_num = i;
        ch_info->control = 0;
        ch_info->kfifo_initialized = 0;
        mutex_init(&ch_info->tx_mutex);
        init_waitqueue_head(&ch_info->tx_wq);
        INIT_LIST_HEAD(&ch_info->tx_queue);
        ch_info->tx_busy = 0;
        mutex_init(&ch_info->rx_mutex);
        init_waitqueue_head(&ch_info->rx_wq);
    }

    if (debug & 0x01) dev_info(&spi->dev, "%s status = %d\n", __func__, status);
    mutex_unlock(&ql_spidev->dev_mutex);
	//20170527 add by cullen for intest spi control logic
//    enable_irq(ql_spidev->irq_slave_ready);

    return status;
}

static int spidev_remove(struct spi_device *spi) {
    struct quectel_spidev *ql_spidev = spi_get_drvdata(spi);
    unsigned long flags;
    int i;
    if (debug & 0x01) dev_info(&spi->dev, "%s\n", __func__);
    /* make sure ops on existing fds can abort cleanly */
    spin_lock_irqsave(&ql_spidev->dev_lock, flags);
    ql_spidev->spidev = NULL;
    spin_unlock_irqrestore(&ql_spidev->dev_lock, flags);

    for (i = 1; i < SPI_MAX_CHN; i++) {
        device_destroy(spidev_class, MKDEV(ql_spidev->major, i));
    }

    return 0;
}

static struct spi_driver spidev_spi_driver = {
    .driver = {
        .name =        "spichn",
        .owner =	THIS_MODULE,
    },
    .probe =	spidev_probe,
    .remove = spidev_remove,
};

/*-------------------------------------------------------------------------*/

static int __init spidev_init(void) {
    int status;
    struct spi_master *master;
    struct spi_board_info chip = {
        .modalias = "spichn",
        .mode = spi_mode,
        .bus_num = busnum,
        .chip_select = chipselect,
        .max_speed_hz = speed_hz,
    };

    ql_spidev = kzalloc(sizeof(struct quectel_spidev), GFP_KERNEL);
    if (ql_spidev == NULL)
        return -ENOMEM;

    spin_lock_init(&ql_spidev->dev_lock);
    mutex_init(&ql_spidev->dev_mutex);
    INIT_LIST_HEAD(&ql_spidev->msg_queue);
    INIT_WORK(&ql_spidev->msg_work, spidev_workq);
    mutex_init(&ql_spidev->msg_work_mutex);
    init_completion(&ql_spidev->tx_done);
    init_waitqueue_head(&ql_spidev->slave_ready_wq);
    ql_spidev->msg_workqueue = create_singlethread_workqueue("spidev");
    ql_spidev->tx_msg = (struct quectel_msg *) __get_dma_pages(GFP_KERNEL, get_order(frame_size));
    ql_spidev->rx_msg = (struct quectel_msg *) __get_dma_pages(GFP_KERNEL, get_order(frame_size));
    if (!ql_spidev->tx_msg || !ql_spidev->tx_msg) {
        kfree(ql_spidev);
        return -ENOMEM;
    }
    ql_spidev->gpio_master_ready = gpiomodemready; //used in msm9615_quectel_power_manager_pdata
    ql_spidev->gpio_slave_ready = gpiomcuready; //used in dsp2.mbn

    status = gpio_request(ql_spidev->gpio_master_ready, "QUECTEL_SPIDEV_MRDY");
    if (status) {
        printk("Fail to request gpio_master_ready PIN %d.\n", ql_spidev->gpio_master_ready);
    } else {
#if 0
        struct gpiomux_setting gpiomux_out = {
            .func = GPIOMUX_FUNC_GPIO,
            .drv = GPIOMUX_DRV_4MA,
            .pull = GPIOMUX_PULL_NONE,
            .dir = GPIOMUX_OUT_LOW,
        };
        if (msm_gpiomux_write(ql_spidev->gpio_slave_ready, GPIOMUX_ACTIVE, &gpiomux_out, NULL))
            printk("Fail to configure SPI_MRDY\n");
#endif
        gpio_direction_output(ql_spidev->gpio_master_ready, 0);
    }

    status = gpio_request(ql_spidev->gpio_slave_ready, "QUECTEL_SPIDEV_SRDY");
    if (status) {
        printk("Fail to request gpio_slave_ready PIN %d.\n", ql_spidev->gpio_slave_ready);
    } else {
#if 0
        struct gpiomux_setting gpiomux_in = {
            .func = GPIOMUX_FUNC_GPIO,
            .drv = GPIOMUX_DRV_4MA,
            .pull = GPIOMUX_PULL_NONE,
            .dir = GPIOMUX_IN,
        };
        if (msm_gpiomux_write(ql_spidev->gpio_slave_ready, GPIOMUX_ACTIVE, &gpiomux_in, NULL))
            printk("Fail to configure SPI_SRDY\n");
#endif
        gpio_direction_input(ql_spidev->gpio_slave_ready); //SPI_SRDY has external pull-up
     
	  status = request_irq(gpio_to_irq(ql_spidev->gpio_slave_ready), spidev_slave_ready_irq_handler,
                                            IRQF_TRIGGER_RISING | IRQF_DISABLED, "ql-spi", ql_spidev);
        if (status) {
            printk("Fail to request gpio_slave_ready IRQ\n");
        } else {
            ql_spidev->irq_slave_ready = gpio_to_irq(ql_spidev->gpio_slave_ready);
            disable_irq(ql_spidev->irq_slave_ready);
        }
    }

    status = register_chrdev(0, "spichn", &spidev_fops);
    if (status < 0)
        return status;
    ql_spidev->major = status;

    spidev_class = class_create(THIS_MODULE, "spichn");
    if (IS_ERR(spidev_class)) {
        status = PTR_ERR(spidev_class);
        goto error_class;
    }

    status = spi_register_driver(&spidev_spi_driver);
    if (status < 0)
        goto error_register;

    master = spi_busnum_to_master(busnum);
    if (!master) {
		printk("Fail to request spi master %d\n", busnum);
        status = -ENODEV;
        goto error_busnum;
    }
    spi_new_device(master, &chip);

    return 0;

error_busnum:
    spi_unregister_driver(&spidev_spi_driver);
error_register:
    class_destroy(spidev_class);
error_class:
    unregister_chrdev(ql_spidev->major, spidev_spi_driver.driver.name);
    free_page((unsigned long) ql_spidev->rx_msg);
    free_page((unsigned long) ql_spidev->tx_msg);
    kfree(ql_spidev);
    return status;
}
module_init(spidev_init);

static void __exit spidev_exit(void) {
    spi_unregister_driver(&spidev_spi_driver);
    class_destroy(spidev_class);
    unregister_chrdev(ql_spidev->major, spidev_spi_driver.driver.name);
    gpio_free(ql_spidev->gpio_master_ready);
    gpio_free(ql_spidev->gpio_slave_ready);
    if (ql_spidev->irq_slave_ready)
        free_irq(ql_spidev->irq_slave_ready, ql_spidev);
    free_page((unsigned long)ql_spidev->tx_msg);
    free_page((unsigned long)ql_spidev->rx_msg);
    kfree(ql_spidev);
}
module_exit(spidev_exit);

MODULE_LICENSE("GPL");
