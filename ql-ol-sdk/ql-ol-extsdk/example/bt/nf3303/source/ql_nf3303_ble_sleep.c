#include <ql_oe.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include "ql_qcmap_client_api.h"
#include "ql_nf3303_ble_sleep.h"
#include "ql_nf3303_bt_debug.h"
#include "ql_eint.h"

/*
 * define the debug level of this file,
 * please see 'debug.h' for detail info
 */
DEBUG_SET_LEVEL(DEBUG_LEVEL_ERR);

int serial_port_fd = -1;

static int wakelock_fd = -1;
static int wakelock_flag = 0; /*lock or unlock flag. lock(wake):1 unlock(sleep):0*/
static int bt_dev_wake_state = 0; /*BT_DEV_WAKE status*/
static int pin_value = 0; /*BT_HOST_WAKE Pin Value*/

#if defined(__QUECTEL_PROJECT_AG35C__) || defined(__QUECTEL_PROJECT_AG35CE__) \
    || defined(__QUECTEL_PROJECT_AG35CEN__)|| defined(__QUECTEL_PROJECT_AG35CEVBM__)
static Enum_PinName BT_HostWakePin = PINNAME_WLAN_WAKE;
static Enum_PinName BT_DevWakePin = PINNAME_GPIO4;
#else
static Enum_PinName BT_HostWakePin = PINNAME_GPIO1;
static Enum_PinName BT_DevWakePin = PINNAME_GPIO6;
#endif


/*========================================================================
  FUNCTION:  wakelock_timer_thread
=========================================================================*/
/**
    @brief
        wakelock timer expire handler.
    
    @dependencies
       None
*/
/*=========================================================================*/
static void wakelock_timer_thread(union sigval v)
{
    DEBUG("pin_value:%d\n", pin_value);
    if(0 == pin_value)
    {
        ql_bt_unlock_wakelock();
    }
}

/*========================================================================
  FUNCTION:  wakelock_timer_init
=========================================================================*/
/**
    @brief
        wakelock timer initialize function.
    
    @dependencies
       None
*/
/*=========================================================================*/
#define CLOCKID (0)	/*CLOCK_REALTIME, a postix timer*/
static int wakelock_timer_init()
{
    timer_t timer_id;
    struct sigevent evp;
    int ret;
    
    memset(&evp, 0, sizeof(struct sigevent));
    evp.sigev_value.sival_int = 0;
    evp.sigev_notify = SIGEV_THREAD;
    evp.sigev_notify_function = wakelock_timer_thread;
    
    if(timer_create(CLOCKID, &evp, &timer_id) == -1)
    {
    	ERR("fail to timer_create");
    	return -1;
    }
    
    struct itimerspec it;
    it.it_interval.tv_sec = 5;
    it.it_interval.tv_nsec = 0;
    it.it_value.tv_sec = 5;
    it.it_value.tv_nsec = 0;
    
    if(timer_settime(timer_id, 0, &it, NULL) == -1)
    {
    	ERR("fail to timer_settime");
    	goto delete_posix_timer;
    }
    
    return 0;

delete_posix_timer:
    if(timer_delete(timer_id) == -1)
    {
    	if(errno == EINVAL)
    	{
    	    ERR("this posix timer is invalid\n");
    	}
    	return -1;
    }
    ERR("this posix timer has been deleted\n");

    return 0;
}

/*BT_HOST_WAKE Pin Interrupt Handler*/
static void bt_eint_callback(Enum_PinName PinName, int level)
{
    pin_value = level;

    if(1 == pin_value)
    {
        DEBUG("pin_value:%d\n", pin_value);
        ql_bt_lock_wakelock();
    }
}

/*Prepare for BLE sleep/wake*/
int ql_ble_sleep_init()
{
    int ret;
    Enum_EintType senseType = EINT_SENSE_BOTH;

    wakelock_fd = Ql_SLP_WakeLock_Create("low_power_bt", sizeof("low_power_bt"));
    if(wakelock_fd<0)
    {
        ERR("Create Sleep Lock fail\n");
	return -1;
    }

    /*Enable BT_HOST_WAKE Interrupt*/
    ret = Ql_EINT_Enable(BT_HostWakePin, senseType, bt_eint_callback);
    if(ret<0)
    {
        ERR("BT_HOST_WAKE Interrupt Enable fail\n");
	return -1;
    }

    if(wakelock_timer_init() < 0)
    {
        ERR("Initialize wakelock timer fail\n");
	return -1;
    }

    INFO("Initialize wakelock timer Success\n");
err:
    return -1;

    return 0;
}

/*Lock wakeLock to keep awake*/
static void ql_bt_lock_wakelock() //system awake
{
    if(0 == wakelock_flag)
    {
        wakelock_flag = 1;
        ql_pull_down_rts();
        if(Ql_SLP_WakeLock_Lock(wakelock_fd) != 0)
	{
	    Ql_SLP_WakeLock_Destroy(wakelock_fd);
	    ERR("Lock wakelock failed!\n");
	    wakelock_flag = 0;
	    return;
	}
	DEBUG("Lock wakelock Success!\n");
    }
}

/*Unlock wakeLock to sleep*/
static void ql_bt_unlock_wakelock() //system sleep
{
    if(1 == wakelock_flag)
    {
        wakelock_flag = 0;
	ql_pull_up_rts();
        if(Ql_SLP_WakeLock_Unlock(wakelock_fd) != 0)
        {
	    ERR("Unlock wakelock failed!\n");
	    Ql_SLP_WakeLock_Destroy(wakelock_fd);
            wakelock_flag = 1;
	    return;
        }
	DEBUG("Unlock wakelock Success!\n");
    }
}

/*========================================================================
  FUNCTION:  ql_bt_sleepwake_pin_uninit
=========================================================================*/
/**
    @brief
        Uninitialize BT_HOST_WAKE&&BT_DEV_WAKE Pin.
    
    @dependencies
       None
*/
/*=========================================================================*/
int ql_bt_sleepwake_pin_uninit()
{
    int ret;

    /*Uninitialize BT_HOST_WAKE Pin*/
    ret = Ql_EINT_Disable(BT_HostWakePin);
    if(ret<0)
    {
        ERR("Disable BT_HOST_WAKE Interrupt fail\n");
	return -1;
    }

    /*Initialize BT_DEV_WAKE Pin*/
    ret = Ql_GPIO_Uninit(BT_DevWakePin);
    if(ret<0)
    {
        ERR("Uninitialize BT_DEV_WAKE fail\n");
	return -1;
    }
}

/*========================================================================
  FUNCTION:  ql_bt_sleepwake_pin_init
=========================================================================*/
/**
    @brief
        Initialize BT_HOST_WAKE&&BT_DEV_WAKE Pin.
    
    @dependencies
       None
*/
/*=========================================================================*/
int ql_bt_sleepwake_pin_init()
{
    int ret;

    /*Initialize BT_HOST_WAKE Pin*/
    ret = Ql_GPIO_SetPullSelection(BT_HostWakePin, PINPULLSEL_DISABLE);
    if(ret<0)
    {
        ERR("Set BT_HOST_WAKE fail\n");
	return -1;
    }

    /*Initialize BT_DEV_WAKE Pin*/
    ret = Ql_GPIO_Init(BT_DevWakePin, PINDIRECTION_OUT, PINLEVEL_HIGH, PINPULLSEL_DISABLE);
    if(ret<0)
    {
        ERR("Initialize BT_DEV_WAKE fail\n");
	return -1;
    }
}

/*Pull Up BT_DEV_WAKE*/
void ql_bt_wake_high()
{
    if(0 == bt_dev_wake_state) {
        Ql_GPIO_SetLevel(BT_DevWakePin, PINLEVEL_HIGH); //pull up BT_DEV_WAKE
        bt_dev_wake_state = 1;
     }
}

/*Pull Down BT_DEV_WAKE*/
void ql_bt_wake_low()
{
    if(1 == bt_dev_wake_state){
	Ql_GPIO_SetLevel(BT_DevWakePin, PINLEVEL_LOW); //pull down BT_DEV_WAKE
         bt_dev_wake_state = 0;
     }
}

static void ql_pull_up_rts(void)
{
    int flag = 0;
    int err = 0;

   DEBUG("Pull Up rts\n");

   flag &= ~TIOCM_RTS;
   if(serial_port_fd>0)
   {
       err = ioctl(serial_port_fd, TIOCMSET, &flag);
       if(err < 0)
           ERR("Pull up RTS Failed\n");   
   }
   else
       ERR("Illegal File descriptors, Pull up RTS Failed\n");
}

static void ql_pull_down_rts(void)
{
    int flag = 0;
    int err = 0;

   DEBUG("Pull Down rts\n");

   flag |= TIOCM_RTS;
   if(serial_port_fd>0)
   {
       err = ioctl(serial_port_fd, TIOCMSET, &flag);
       if(err < 0)
           ERR("Pull down RTS Failed\n");
   }
   else
       ERR("Illegal File descriptors, Pull down RTS Failed\n");
}

