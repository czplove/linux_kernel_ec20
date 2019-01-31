#ifndef QL_NF3303_BLE_SLEEP
#define QL_NF3303_BLE_SLEEP

/*Prepare for BLE sleep/wake*/
int ql_ble_sleep_init(void);

/*Lock wakeLock to keep wake*/
static void ql_bt_lock_wakelock(void);

/*Unlock wakeLock to sleep*/
static void ql_bt_unlock_wakelock(void);

/*Pull Up BT_DEV_WAKE high*/
void ql_bt_wake_high(void);

/*Pull Down BT_DEV_WAKE high*/
void ql_bt_wake_low(void);

/*Pull Up RTS high*/
static void ql_pull_up_rts(void);

/*Pull Down RTS high*/
static void ql_pull_down_rts(void);

#endif
