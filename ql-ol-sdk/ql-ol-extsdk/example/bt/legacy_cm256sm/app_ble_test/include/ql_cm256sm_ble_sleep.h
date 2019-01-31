#ifndef QL_CM256SM_BLE_SLEEP
#define QL_CM256SM_BLE_SLEEP

/*Prepare for BLE sleep/wake*/
int ql_ble_sleep_init(void);

/*Lock wakeLock to keep wake*/
void ql_bt_lock_wakelock(void);

/*Unlock wakeLock to sleep*/
void ql_bt_unlock_wakelock(void);

/*Pull Up BT_DEV_WAKE high*/
void ql_bt_wake_high(void);

/*Pull Down BT_DEV_WAKE high*/
void ql_bt_wake_low(void);

#endif
