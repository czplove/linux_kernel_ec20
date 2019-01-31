#include <linux/device.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/capability.h>
#include <linux/export.h>
#include <linux/suspend.h>
#include <linux/seq_file.h>
#include <linux/proc_fs.h>
#include <linux/types.h>
#include <trace/events/power.h>

#define QUEC_WAKELOCK_NAME_MAX_LEN	(40)
#define QUEC_IOCTL_CREATE_WAKELOCK	(0x00050001)
#define QUEC_IOCTL_LOCK_WAKELOCK	(0x00050002)
#define QUEC_WAKELOCK_MAX_NUMBER	(512)

struct quec_wakelock {
	char* name;
	struct rb_node node;
	struct wakeup_source ws;
};

struct wakelock_name {
	char name[QUEC_WAKELOCK_NAME_MAX_LEN];
	int name_len;
};

static DEFINE_MUTEX(wakelocks_lock);

static struct rb_root quec_wakelocks_tree = RB_ROOT;

static unsigned int wakelocks_number;

static inline bool wakelocks_number_exceeded(void)
{
	return wakelocks_number >= QUEC_WAKELOCK_MAX_NUMBER;
}

static inline void inc_wakelocks_number(void)
{
	wakelocks_number++;
}

static inline void dec_wakelocks_number(void)
{
	wakelocks_number--;
}

static int wakelock_open(struct inode *inode, struct file *file) {
	mutex_lock(&wakelocks_lock);
	file->private_data = NULL;
	mutex_unlock(&wakelocks_lock);
	return 0;
}

static int wakelock_add(const char *name, size_t len, struct quec_wakelock **wakelock) {
	struct rb_node **node = NULL; 
	struct rb_node *parent = NULL;
	struct quec_wakelock *wl;
	
	if (wakelocks_number_exceeded()) {
		return -EMFILE;
	}

	node = &quec_wakelocks_tree.rb_node;
	parent = *node;
			
	while (*node) {
		int diff;

		parent = *node;
		wl = rb_entry(*node, struct quec_wakelock, node);
		diff = strncmp(name, wl->name, len);
		if (diff == 0) {
			if (wl->name[len]) {
				diff = -1;
			} else {
				return -EEXIST;
			}
		}
		if (diff < 0)
			node = &(*node)->rb_left;
		else
			node = &(*node)->rb_right;
	}

	wl = kzalloc(sizeof(*wl), GFP_KERNEL);
	if (!wl) {
		return -ENOMEM;
	}

	wl->name = kstrndup(name, len, GFP_KERNEL);
	if (!wl->name) {
		kfree(wl);
		return -ENOMEM;
	}
	wl->ws.name = wl->name;
	wakeup_source_add(&wl->ws);
	rb_link_node(&wl->node, parent, node);
	rb_insert_color(&wl->node, &quec_wakelocks_tree);

	*wakelock = wl;
	inc_wakelocks_number();
	
	return 0;
}

static long quec_lock_wakelock(struct quec_wakelock *wakelock, bool lock) {
	if (lock) {
		__pm_stay_awake(&wakelock->ws);
	} else {
		__pm_relax(&wakelock->ws);
	}
	
	return 0;
}

static long quec_wakelock_delete(struct quec_wakelock *wakelock) {
	wakeup_source_remove(&wakelock->ws);
	del_timer_sync(&wakelock->ws.timer);
	__pm_relax(&wakelock->ws);
	rb_erase(&wakelock->node, &quec_wakelocks_tree);
	kfree(wakelock->name);
	kfree(wakelock);
	dec_wakelocks_number();

	return 0;
}

static int wakelock_release(struct inode *inode, struct file *file) {
	struct quec_wakelock* wakelock;
	int error;
	
	mutex_lock(&wakelocks_lock);
	wakelock = (struct quec_wakelock *) file->private_data;
	
	if (!wakelock) {
		mutex_unlock(&wakelocks_lock);
		return 0;
	}
	
	file->private_data = NULL;
	error = quec_wakelock_delete(wakelock);
	mutex_unlock(&wakelocks_lock);
	
	return error;
}

static long wakelock_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	struct wakelock_name name;
	struct quec_wakelock *wakelock;
	int error = 0;
	
	mutex_lock(&wakelocks_lock);
	
	switch (cmd) {
	case QUEC_IOCTL_CREATE_WAKELOCK:
		if (file->private_data) {
			error = -EEXIST;
			break;
		}
		
		if (copy_from_user(&name, (struct wakelock_name __user *) arg,
			   sizeof(struct wakelock_name))) {
			error = -EFAULT;
			break;
		}
				
		error = wakelock_add(name.name, name.name_len, &wakelock);
		if (error) {
			break;
		}
		
		file->private_data = wakelock;
		break;
		
	case QUEC_IOCTL_LOCK_WAKELOCK:
		if (!file->private_data) {
			error = -EPERM;
			break;
		}
		error = quec_lock_wakelock((struct quec_wakelock *)file->private_data, !!arg);
		break;
		
	default:
		error = -EPERM;
		break;
	}

	mutex_unlock(&wakelocks_lock);
	return error;
}

static const struct file_operations wakelock_fops = {
	.owner = THIS_MODULE,
	.open = wakelock_open,
	.unlocked_ioctl = wakelock_ioctl,
	.read = NULL,
	.llseek = NULL,
	.release = wakelock_release,
};

static int __init quec_wakelock_proc_init(void)
{
	proc_create("quec_wakelock", 0, NULL, &wakelock_fops);
	return 0;
}

__initcall(quec_wakelock_proc_init);

