#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/wait.h>
#include <linux/sched.h>
#include <linux/miscdevice.h>
#include <linux/workqueue.h>
#include <linux/uaccess.h>
#include <linux/mtd/mtd.h>
#include <linux/slab.h>
#include <linux/reboot.h>

#include "quectel-features-config.h" // quectel add

#if 1 // def  QUECTEL_SYSTEM_BACKUP    // Ramos add for quectel for linuxfs restore

#define QUEC_BACKUP_MAGIC1        (0x78E5D4C2)
#define QUEC_BACKUP_MAGIC2        (0x54F7D60E)
#define QUEC_BACKUP_INFO_BLOCK_NUMS (3)  // keep with modem , bootloader aboot.c
//"sys_rev" partiton last QUEC_BACKUP_INFO_BLOCK_NUMS block use record restore information

#define QUEC_ALL_RESTORE_FLAG_BLOCK_INDEX (6) // the 3 block reserved for All parition restoring flag and fota upgraded flag
//the "sys_rev" partition last QUEC_ALL_RESTORE_FLAG_BLOCK_INDEX to QUEC_BACKUP_INFO_BLOCK_NUMS reserved for fota upgraded flag.

#define QUEC_START_TIMER_TOUT	30

#define BACKUP_INFO_BLOCK_NUMS (3)

typedef struct
{
  uint32_t magic1;  
  uint32_t magic2;

  uint32_t cefs_restore_flag;
  uint32_t cefs_restore_times;
  uint32_t cefs_backup_times;
  uint32_t cefs_crash[10];  // ¼10طcefsļcrashĴ
   
  // Ramos add for linux fs backup restore times
    uint32_t linuxfs_restore_flag;
    uint32_t linuxfs_restore_times;
    uint32_t linuxfs_backup_times;  //ΪLinux fsǺʹһ¼ģûõʶǷLinuxļϵͳԭ̡Ϊ0xAA55 򲻽лԭ
    uint32_t linuxfs_crash[10];  // ¼10طļϵͳcrashĴ

	// modem backup restore flag
	uint32_t modem_restore_flag;
	uint32_t modem_restore_times;
	uint32_t modem_backup_times;
	uint32_t modem_crash[10];  

	//  recovery restore flag
    uint32_t recovery_restore_flag;
    uint32_t recovery_restore_times;
    uint32_t recovery_backup_times;
    uint32_t recovery_crash[10]; 

	// other image restore flag
	uint32_t image_restoring_flag;
	uint32_t reserved1;
	uint32_t reserved2;

    
} quec_backup_info_type;

typedef struct
{
	uint32_t All_Restoring_flag;
	uint32_t fota_upgradedFlag; // module have upgraded fota flag 
	uint32_t reserved2;
} quec_All_RestoringInfo;


struct qstart_device_t{
	struct miscdevice misc;
};

static struct mtd_info *mtd = NULL;
static loff_t qfirst_goodblock_addr = 0;
static loff_t g_fota_upgradedFlag_goodblock = 0;

struct qstart_device_t *qstart_devp;
struct timer_list qstart_poll_timer;
struct work_struct qstart_timer_work;

static DEFINE_MUTEX(qstart_timer_lock);

static int start_mode_set(const char *val, struct kernel_param *kp);
static int start_mode = 0;
module_param_call(start_mode, start_mode_set, param_get_int, &start_mode, 0644);

bool boot_mode = false;

//add by len, get the recovery value from cmdline, 2018-1-18
static int __init quectel_set_bootmode(char *str)
{
    if (str) {
        boot_mode = true;
    }
    else {
        boot_mode = false;
    }
    return 0;
}
early_param("recovery", quectel_set_bootmode);
//add end

/**
 * Author : Darren
 * Date : 2017/6/30
 * get_bootmode - will return current start mode
 * 0 -- normode, system mount linuxfs as rootfs
 * 1 -- recoverymode, system mount recoveryfs as rootfs
 */
//modify by len 2018-1-18 for boot mode
bool get_bootmode(unsigned int *mode)
{
    return boot_mode;
}
//modify end
EXPORT_SYMBOL(get_bootmode);

static int start_mode_set(const char *val, struct kernel_param *kp)
{
	int ret;

	ret = param_set_int(val,kp);

	if(ret)
		return ret;
	
	return 0;
}

static void qstart_poll_timer_cb(void)
{
    static unsigned int count = 0;

	mutex_lock(&qstart_timer_lock);
	count++;
    if(count >=  QUEC_START_TIMER_TOUT && 0 == start_mode)
    {
		printk("\n\n\nStart mode = %d, count=%d\n\n\n", start_mode,count);
		schedule_work(&qstart_timer_work);	
   		//panic("Quec Start Timer error!");     
    }
	mutex_unlock(&qstart_timer_lock);

	if(1 == start_mode)
		del_timer(&qstart_poll_timer);
	else
 		mod_timer(&qstart_poll_timer,jiffies + HZ);
}

static struct qstart_device_t qstart_device = {
	.misc = {
		.minor = MISC_DYNAMIC_MINOR,
		.name = "qstart",
	}
};

unsigned int Quectel_Is_BackupPartition_Exist(const char * sourc_partition)
{
	struct mtd_info *mtd = NULL;

	if(!strcmp(sourc_partition, "modem")) 
	{
		mtd = get_mtd_device_nm("qdsp6sw_b");
		if(IS_ERR(mtd))
		{
			printk("@Ramos can't get modem backup partition !!!\r\n");
			return 0;
		}
		else
		{
			return 1;
		}
	}
	else if (!strcmp(sourc_partition, "system"))
	{
		return 1;
	}
	else if(!strcmp(sourc_partition, "cefs"))
	{
		return 1;
	}
	else if (!strcmp(sourc_partition, "recoveryfs"))
	{
		mtd = get_mtd_device_nm("recoveryfs_b");
		if(IS_ERR(mtd))
		{
			printk("@Ramos can't get modem backup partition !!!\r\n");
			return 0;
		}
		else
		{
			return 1;
		}
	}
	else
	{
		return 0;
	}
}

unsigned int Quectel_Set_Partition_RestoreFlag(const char * partition_name, int where)
{
	unsigned char *onepage = NULL;
	quec_backup_info_type Flag_msg;
	struct erase_info ei;
	size_t write = 0;
	size_t readlen = 0;
	int err,i;
	int ret = 0;
	uint64_t tmp;
	uint32_t blocksize=0;
#if 1 //quectel_fota
	quec_All_RestoringInfo fota_upgradedFlag_msg = {0,0,0};
#endif

	if (!Quectel_Is_BackupPartition_Exist(partition_name))
	{
		printk("@Ramos the [%s] partition no backup partitino  !!!!!\r\n", partition_name);
		return 0;
	}
	
	//  sys_rev дflagbootloader ʱflagжǷԭӦķ
	mtd = get_mtd_device_nm("sys_rev");
	if(IS_ERR(mtd))
	{
		printk("@Ramos get sys_rev mtd fail.!\r\n");
		return 0;
	}
	else
	{
		tmp = mtd->size; // totle size .
		blocksize =  mtd->erasesize;
		
		printk("@Ramos :mtdsize:%llx, mtd->writesize =%d, mtd->erasesize:%d  blockcount\n", tmp, mtd->writesize,mtd->erasesize);
		for(i=0; i<QUEC_BACKUP_INFO_BLOCK_NUMS; i++)
		{
			qfirst_goodblock_addr =tmp -(QUEC_BACKUP_INFO_BLOCK_NUMS- i) * mtd->erasesize; // ҪҪƫƣʼӴflagĵַblock
			if(!mtd_block_isbad(mtd,qfirst_goodblock_addr)) 
				break;
		}

		onepage = kmalloc(mtd->writesize, GFP_KERNEL);
		if(NULL == onepage)
		{
			printk("@Ramos memory is not enough to onepage\n");
			return ret;
		}
		
		printk("@Ramos qfirst_goodblock_addr=%d \n", qfirst_goodblock_addr);
		memset(onepage, 0x00, mtd->writesize);
		mtd_read(mtd, qfirst_goodblock_addr, mtd->writesize, &readlen ,onepage);
		if(readlen != mtd->writesize )
		{
			printk("@Ramos read Flag Failed!!\r\n\r\n");
		}

		// ¼ݻԭ
				memcpy((void *)&Flag_msg,(void *)onepage,sizeof(Flag_msg));
		printk("@Ramos set partition(%s) restore, offset=%lx,print magic1=%lx,magic2=%lx\r\n", partition_name, qfirst_goodblock_addr,Flag_msg.magic1,Flag_msg.magic2);
#if 1 //QUECTEL_FOTA, 
		for(i=0; i<QUEC_BACKUP_INFO_BLOCK_NUMS; i++)
		{
			g_fota_upgradedFlag_goodblock =tmp -(QUEC_ALL_RESTORE_FLAG_BLOCK_INDEX- i) * mtd->erasesize; //find fota upgraded flag 
			if(!mtd_block_isbad(mtd,g_fota_upgradedFlag_goodblock)) 
				break;
		}
		memset(onepage, 0x00, mtd->writesize);
		readlen = 0 ;
		mtd_read(mtd, g_fota_upgradedFlag_goodblock, mtd->writesize, &readlen ,onepage);
		if(readlen == mtd->writesize )
		{	
			memcpy((void *)&fota_upgradedFlag_msg,(void *)onepage,sizeof(fota_upgradedFlag_msg));
			printk("@Ramos g_fota_upgradedFlag_goodblock=%d fota_upgradedFlag=%d\r\n", g_fota_upgradedFlag_goodblock,fota_upgradedFlag_msg.fota_upgradedFlag);
		}
		else
		{
			printk("@Ramos g_fota_upgradedFlag_goodblock Failed!!\r\n\r\n");
		}
#endif

		if( 0xAA55 == Flag_msg.linuxfs_backup_times)
		{
			printk("@Ramos AT configed	not allow  linux fs restore  linuxfs_backup_times=%d\r\n\r\n\r\n", Flag_msg.linuxfs_backup_times);
			return 0;
		}
		if((QUEC_BACKUP_MAGIC1 != Flag_msg.magic1) ||(QUEC_BACKUP_MAGIC2 != Flag_msg.magic2))// ˵˿һд
		{
			memset((void *)&Flag_msg, 0x00, sizeof(Flag_msg));
					Flag_msg.magic1 =  QUEC_BACKUP_MAGIC1;
			Flag_msg.magic2 =  QUEC_BACKUP_MAGIC2;
			if(1 == fota_upgradedFlag_msg.fota_upgradedFlag)//QUECTEL_FOTA, if one need restore, we restore the whole system, keep the system,modem,boot,recoveryfs are the same version. 
			//if(0)//EC25AUTL update beckup partition,not need restore the whole system 
			{
				Flag_msg.linuxfs_restore_flag = 1;
				Flag_msg.recovery_restore_flag = 1;
				Flag_msg.modem_restore_flag = 1;
				Flag_msg.image_restoring_flag =1;
			}
			else// the module not upgraded by fota  never
			{
				if(!strcmp(partition_name, "system"))
				{
					Flag_msg.linuxfs_restore_flag = 1;
				}
				else if(!strcmp(partition_name, "modem")) // 设置modem的还原flag
				{
					Flag_msg.modem_restore_flag = 1;
				}
				else if(!strcmp(partition_name, "efs2")) // efs2 设置还原在modme那边，所以这了没有代码
				{

				}
			}

		}
		else
		{
			printk("@Ramos set partition magic	right !!!!!\n" );
			if(1 == fota_upgradedFlag_msg.fota_upgradedFlag)// if one need restore, we restore the whole system, keep the system,modem,boot,recoveryfs are the same version. 
			//if(0)//EC25AUTL update beckup partition,not need restore the whole system 
			{
				Flag_msg.linuxfs_restore_flag = 1;
				Flag_msg.recovery_restore_flag = 1;
				Flag_msg.modem_restore_flag = 1;
				Flag_msg.image_restoring_flag =1;
			}

			if(!strcmp(partition_name, "system"))
			{
				Flag_msg.linuxfs_restore_flag = 1;
				if(where <10)  // ֹ
				{
					Flag_msg.linuxfs_crash[where] +=1;
				}
			}
			else if(!strcmp(partition_name, "recoveryfs"))
			{
				Flag_msg.recovery_restore_flag = 1;
				if(where <10)  // 止
				{
					Flag_msg.recovery_crash[where] +=1;
				}
			}
			else if (!strcmp(partition_name, "modem"))
			{
				Flag_msg.modem_restore_flag = 1;
				if(where <10)  // ֹ
				{
					Flag_msg.modem_crash[where] +=1;
				}
			}
			//else if (!strcmp(partition_name, "modem"))  ûԭflag
		}
		
		memset(&ei, 0, sizeof(struct erase_info));
		ei.mtd = mtd;
		ei.addr = qfirst_goodblock_addr;
		ei.len = mtd->erasesize;
		err = mtd_erase(mtd, &ei); //ȲһҪд
		
				memcpy((void *)onepage,(void *)&Flag_msg, sizeof(Flag_msg));
		err = mtd_write(mtd, qfirst_goodblock_addr, mtd->writesize, &write, onepage);
		if(err || write != mtd->writesize)
		{
			printk("@Ramos set partition(%s) Flag  failed at %#llx\n", partition_name,(long long)qfirst_goodblock_addr);
			goto exit;
		}
		if(!strcmp(partition_name, "system"))
		{
			printk("@Ramos set partition(%s) flag=%d,successd restore times=%d !!!\n", partition_name, Flag_msg.linuxfs_restore_flag,Flag_msg.linuxfs_restore_times );
		}
		else if (!strcmp(partition_name, "modem"))
		{
			printk("@Ramos set partition(%s) flag=%d,successd restore times=%d !!!\n", partition_name, Flag_msg.modem_restore_flag,Flag_msg.modem_restore_times );
		}
/*
		memset(onepage, 0x00, mtd->writesize);
		mtd_read(mtd, qfirst_goodblock_addr, mtd->writesize, &readlen ,onepage);
		printk("@Ramos debug Restore flag is:%s\n",  ((struct restorflag_message *)onepage)->command);
*/
		
	}	
	ret = 1;
	machine_restart(NULL);
exit:
	if(onepage != NULL)
	{
		kfree(onepage);
		onepage = NULL;
	}
	return ret;
}

void Quectel_Partition_Restore(const char * partition_name, int where)
{

    Quectel_Set_Partition_RestoreFlag(partition_name,where);
    return 0;

}

void Quectel_Erase_Partition(const char * partition_name)
{
	struct mtd_info *mtd = NULL;
	struct erase_info ei;
	int err,i;
	
	printk("@Ramos there are fatal errror on the %s partition , we must erase it !!!\r\n", partition_name);
	mtd = get_mtd_device_nm(partition_name);
	if(IS_ERR(mtd))
	{
		printk("ERROR!!!!!  @Ramos get  %s mtd fail.!\r\n", partition_name);
		return 0;
	}
	else
	{
		memset(&ei, 0, sizeof(struct erase_info));
		ei.mtd = mtd;
		ei.len = mtd->erasesize;
		for(i=0 ;  ; i++)
		{
			ei.addr = i*(mtd->erasesize);
			if(ei.addr  > mtd->size)
			{
				break;
			}
			err = mtd_erase(mtd, &ei); //
		}
	}
		
	machine_restart(NULL);

return 0;
}

EXPORT_SYMBOL(Quectel_Set_Partition_RestoreFlag);
EXPORT_SYMBOL(Quectel_Partition_Restore);
EXPORT_SYMBOL(Quectel_Erase_Partition);

static void qstart_timer_handle(struct work_struct *work)
{

	Quectel_Set_Partition_RestoreFlag("system",10);
}

static void __exit qstart_exit(void)
{
	misc_deregister(&qstart_device.misc);
}

static int __init qstart_init(void)
{
	int ret;

	printk("@Ramos Qstart_init entry !!!\r\n");

#if 0   //QUECTEL_LINUX_APP_DETECT
	qstart_devp = &qstart_device;
	init_timer(&qstart_poll_timer);
                qstart_poll_timer.function = (void *)qstart_poll_timer_cb; 
                qstart_poll_timer.expires = jiffies + HZ;
                add_timer(&qstart_poll_timer);
	
	INIT_WORK(&qstart_timer_work, qstart_timer_handle);		
	
	ret = misc_register(&qstart_device.misc);
#endif
	return ret;
}

module_init(qstart_init);
module_exit(qstart_exit);

MODULE_DESCRIPTION("QUECTEL Start Driver");
MODULE_LICENSE("GPL v2");

#endif

