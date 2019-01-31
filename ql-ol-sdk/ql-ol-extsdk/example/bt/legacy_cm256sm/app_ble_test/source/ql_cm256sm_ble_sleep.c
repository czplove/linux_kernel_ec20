#include <ql_oe.h>
#include "ql_qcmap_client_api.h"
#include "ql_eint.h"
#include "ql_cm256sm_ble_sleep.h"

static int wakelock_fd = -1;
static int wakelock_flag = -1; /*lock or unlock flag. lock(wake):1 unlock(sleep):0*/

#if defined(__QUECTEL_PROJECT_AG35C__) || defined(__QUECTEL_PROJECT_AG35CE__)
    || defined(__QUECTEL_PROJECT_AG35CEN__)|| defined(__QUECTEL_PROJECT_AG35CEVBM__)
static Enum_PinName BT_HostWakePin = PINNAME_GPIO3;
#else
static Enum_PinName BT_HostWakePin = PINNAME_GPIO4;
#endif

/*BT_HOST_WAKE Pin Interrupt Handler*/
static void bt_eint_callback(Enum_PinName PinName, int level)
{
         if(0 == level)
             ql_bt_lock_wakelock();
}

/*Prepare for BLE sleep/wake*/
int ql_ble_sleep_init()
{
    int ret;
    Enum_EintType senseType = EINT_SENSE_FALLING;

    wakelock_fd = Ql_SLP_WakeLock_Create("low_power_bt", sizeof("low_power_bt"));
    if(wakelock_fd<0)
    {
        printf("Create Sleep Lock fail\n");
	return -1;
    }

    ret = Ql_GPIO_SetPullSelection(BT_HostWakePin, PINPULLSEL_PULLUP);
    if(ret<0)
    {
        printf("Set BT_HOST_WAKE fail\n");
	return -1;
    }

    ret = Ql_EINT_Enable(BT_HostWakePin, senseType, bt_eint_callback);
    if(ret<0)
    {
        printf("BT_HOST_WAKE Interrupt Enable fail\n");
	return -1;
    }

    return 0;
}

/*Lock wakeLock to keep wake*/
void ql_bt_lock_wakelock() //system awake
{
    if(wakelock_flag != 1)
    {
        wakelock_flag = 1;
        if(Ql_SLP_WakeLock_Lock(wakelock_fd) != 0)
	{
	    Ql_SLP_WakeLock_Destroy(wakelock_fd);
	    printf("Lock wakelock failed!\n");
	    wakelock_flag = 0;
	    return;
	}
	printf("Lock wakelock Success!\n");
    }
}

/*Unlock wakeLock to sleep*/
void ql_bt_unlock_wakelock() //system sleep
{
    if(wakelock_flag == 1)
    {
        wakelock_flag = 0;
        if(Ql_SLP_WakeLock_Unlock(wakelock_fd) != 0)
        {
	    printf("Unlock wakelock failed!\n");
	    Ql_SLP_WakeLock_Destroy(wakelock_fd);
            wakelock_flag = 1;
	    return;
        }
	printf("Unlock wakelock Success!\n");
    }
}


