#ifndef __QL_SLEEP_WAKELOCK_H__
#define __QL_SLEEP_WAKELOCK_H__

/*
 * create wakelock, return the file description of the wakelock
 */
extern int Ql_SLP_WakeLock_Create(const char *name, size_t len);

/*
 * lock the wakelock by the file description of the wakelock
 */
extern int Ql_SLP_WakeLock_Lock(int fd);


/*
 * unlock the wakelock by the file description of the wakelock
 */
extern int Ql_SLP_WakeLock_Unlock(int fd);

/*
 * destroy the wakelock by the file description of the wakelock
 */
extern int Ql_SLP_WakeLock_Destroy(int fd);

/*
 * Enable/Disable autosleep function
 */
extern int Ql_Autosleep_Enable(char enable);

#endif  //__QL_SLEEP_WAKELOCK_H__
