/*
 * Copyright (C) 2009 The QUECTEL Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _QL_SYS_LOG_H_ 
#define _QL_SYS_LOG_H_ 

/*
 * Support routines to send messages to the Android in-kernel log buffer,
 * which can later be accessed through the 'logcat' utility.
 *
 * Each log message must have
 *   - a priority
 *   - a log tag
 *   - some text
 *
 * The tag normally corresponds to the component that emits the log message,
 * and should be reasonably small.
 *
 * Log message text may be truncated to less than an implementation-specific
 * limit (e.g. 1023 characters max).
 *
 * Note that a newline character ("\n") will be appended automatically to your
 * log message, if not already there. It is not possible to send several messages
 * and have them appear on a single line in logcat.
 *
 * PLEASE USE LOGS WITH MODERATION:
 *
 *  - Sending log messages eats CPU and slow down your application and the
 *    system.
 *
 *  - The circular log buffer is pretty small (<64KB), sending many messages
 *    might push off other important log messages from the rest of the system.
 *
 *  - In release builds, only send log messages to account for exceptional
 *    conditions.
 */


#ifdef __cplusplus
extern "C" {
#endif

/*
 *  Log priority values, in ascending priority order.(wapper ANDROID_LOG_XXX)
 */
typedef enum {
    QL_SYS_LOG_MIN = 0,
    QL_SYS_LOG_DEFAULT,    /* only for SetMinPriority() */
    QL_SYS_LOG_VERBOSE,
    QL_SYS_LOG_DEBUG,
    QL_SYS_LOG_INFO,
    QL_SYS_LOG_WARN,
    QL_SYS_LOG_ERROR,
    QL_SYS_LOG_FATAL,
    QL_SYS_LOG_SILENT,     /* only for SetMinPriority(); must be last */
    QL_SYS_LOG_MAX
} QL_SYS_LOG_PRIORITY_E;
#define QL_SYS_LOG_PRORIFY_VERIFY(prio) ((prio)>QL_SYS_LOG_MIN && (prio)<QL_SYS_LOG_MAX)

typedef enum {
    QL_SYS_LOG_ID_MIN = -1,
    QL_SYS_LOG_ID_MAIN = 0,
    QL_SYS_LOG_ID_RADIO = 1,
    QL_SYS_LOG_ID_EVENTS = 2,
    QL_SYS_LOG_ID_SYSTEM = 3, 
    QL_SYS_LOG_ID_MAX
} QL_SYS_LOG_ID_E;
#define QL_SYS_LOG_ID_VERIFY(log_id)    ((log_id)>QL_SYS_LOG_ID_MIN && (log_id)<QL_SYS_LOG_ID_MAX)

/* wapper ql_sys_log_print */
#define QLOGV(tag, ...) ql_sys_log_print(QL_SYS_LOG_ID_MAIN,  QL_SYS_LOG_VERBOSE, tag, __VA_ARGS__)
#define QLOGD(tag, ...) ql_sys_log_print(QL_SYS_LOG_ID_MAIN,  QL_SYS_LOG_DEBUG,   tag, __VA_ARGS__)
#define QLOGI(tag, ...) ql_sys_log_print(QL_SYS_LOG_ID_MAIN,  QL_SYS_LOG_INFO,    tag, __VA_ARGS__)
#define QLOGW(tag, ...) ql_sys_log_print(QL_SYS_LOG_ID_MAIN,  QL_SYS_LOG_WARN,    tag, __VA_ARGS__)
#define QLOGE(tag, ...) ql_sys_log_print(QL_SYS_LOG_ID_MAIN,  QL_SYS_LOG_ERROR,   tag, __VA_ARGS__)
#define QLOGF(tag, ...) ql_sys_log_print(QL_SYS_LOG_ID_MAIN,  QL_SYS_LOG_FATAL,   tag, __VA_ARGS__)

/* wapper ql_sys_log_puts */
#define QLOGSV(tag, msg) ql_sys_log_puts(QL_SYS_LOG_ID_MAIN, QL_SYS_LOG_VERBOSE, tag, msg)
#define QLOGSD(tag, msg) ql_sys_log_puts(QL_SYS_LOG_ID_MAIN, QL_SYS_LOG_DEBUG,   tag, msg)
#define QLOGSI(tag, msg) ql_sys_log_puts(QL_SYS_LOG_ID_MAIN, QL_SYS_LOG_INFO,    tag, msg)
#define QLOGSW(tag, msg) ql_sys_log_puts(QL_SYS_LOG_ID_MAIN, QL_SYS_LOG_WARN,    tag, msg)
#define QLOGSE(tag, msg) ql_sys_log_puts(QL_SYS_LOG_ID_MAIN, QL_SYS_LOG_ERROR,   tag, msg)
#define QLOGSF(tag, msg) ql_sys_log_puts(QL_SYS_LOG_ID_MAIN, QL_SYS_LOG_FATAL,   tag, msg)

#define __QL_TRIGGER_DUMP__ do{ \
    ((int*)0)[0]=0x1234; \
}while(0)

/*
 * Send a simple string to the log.
 */
int ql_sys_log_puts(QL_SYS_LOG_ID_E log_id, QL_SYS_LOG_PRIORITY_E prio, const char *tag, const char *text);

/*
 * Send a formatted string to the log, used like printf(fmt,...)
 */
int ql_sys_log_print(QL_SYS_LOG_ID_E log_id, QL_SYS_LOG_PRIORITY_E prio, const char *tag,  const char *fmt, ...);

/*
 * Send signal info to the log, used in sigaction, sig_info type is siginfo_t defined by signal.h
 */
int ql_sys_log_signal(QL_SYS_LOG_ID_E log_id, QL_SYS_LOG_PRIORITY_E prio, const char *tag, int sig_num, void *sig_info, void *sig_ptr);


#ifdef __cplusplus
}
#endif

#endif /* _QL_SYS_LOG_H_ */
