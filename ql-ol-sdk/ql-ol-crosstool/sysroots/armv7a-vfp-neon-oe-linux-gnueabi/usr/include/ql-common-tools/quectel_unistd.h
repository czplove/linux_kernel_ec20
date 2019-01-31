/**
 * @file quectel_unistd.h
 * @brief Quectel utils public function declarations.
 * 
 * @note 
 *
 * @copyright Copyright (c) 2009-2017 @ Quectel Wireless Solutions Co., Ltd.
 */

#ifndef __QUECTEL_UNISTD_H__
#define __QUECTEL_UNISTD_H__

/**
 * calling thread sleep until seconds seconds have elapsed
 * 
 * @param[in] sec              seconds
 * @param[in] usec             microseconds
 *
 * @return 
 *   none
 * 
 */
extern void os_wait(long usec, long sec);

/**
 * system call
 * 
 * @param[in] fmt              command for format string.
 *
 * @return On success, Zero is returned. On error, -1 is returned.
 * 
 */

extern int quectel_system_call(const char *fmt, ...)
__attribute ((format(printf, 1, 2)));

#endif /* end of __QUECTEL_UNISTD_H__ */

