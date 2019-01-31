/*
 * Copyright (C) 2005 The Android Open Source Project
 * Copyright (c) 2009, The Linux Foundation. All rights reserved.
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

//
// C/C++ logging functions.  See the logging documentation for API details.
//
// We'd like these to be available from C code (in case we import some from
// somewhere), so this has a C interface.
//
// The output will be correct when the log file is shared between multiple
// threads and/or multiple processes so long as the operating system
// supports O_APPEND.  These calls have mutex-protected data structures
// and so are NOT reentrant.  Do not use LOG in a signal handler.
//
#ifndef _LIBS_CUTILS_LOG_H
#define _LIBS_CUTILS_LOG_H

#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#ifdef HAVE_PTHREADS
#include <pthread.h>
#endif
#include <stdarg.h>

#include <cutils/uio.h>
#include <cutils/logd.h>

#ifdef __cplusplus
extern "C" {
#endif

// ---------------------------------------------------------------------

/*
 * Normally we strip LOGV (VERBOSE messages) from release builds.
 * You can modify this (for example with "#define LOG_NDEBUG 0"
 * at the top of your source file) to change that behavior.
 */
#ifndef LOG_NDEBUG
#ifdef NDEBUG
#define LOG_NDEBUG 1
#else
#define LOG_NDEBUG 0
#endif
#endif

#ifndef LOG_NIDEBUG
#ifdef NDEBUG
#define LOG_NIDEBUG 1
#else
#define LOG_NIDEBUG 0
#endif
#endif

#ifndef LOG_NDDEBUG
#ifdef NDEBUG
#define LOG_NDDEBUG 1
#else
#define LOG_NDDEBUG 0
#endif
#endif

/*
 * This is the local tag used for the following simplified
 * logging macros.  You can change this preprocessor definition
 * before using the other macros to change the tag.
 */
#ifndef LOG_TAG
#define LOG_TAG NULL
#endif

/* basic log macros for each level
 */

#ifndef LOGLOG_VERBOSE
#if LOG_NDEBUG
#define LOGLOG_VERBOSE(tag, ...) ((void)0)
#else
#define LOGLOG_VERBOSE(tag, ...) \
    (void)android_printLog(ANDROID_LOG_VERBOSE, tag, __VA_ARGS__)
#endif
#endif

#ifndef LOGLOG_DEBUG
#if LOG_NDDEBUG
#define LOGLOG_DEBUG(tag, ...) ((void)0)
#else
#define LOGLOG_DEBUG(tag, ...) \
    (void)android_printLog(ANDROID_LOG_DEBUG, tag, __VA_ARGS__)
#endif
#endif

#ifndef LOGLOG_INFO
#if LOG_NIDEBUG
#define LOGLOG_INFO(tag, ...) ((void)0)
#else
#define LOGLOG_INFO(tag, ...) \
    (void)android_printLog(ANDROID_LOG_INFO, tag, __VA_ARGS__)
#endif
#endif

#ifndef LOGLOG_WARN
#define LOGLOG_WARN(tag, ...) \
    (void)android_printLog(ANDROID_LOG_WARN, tag, __VA_ARGS__)
#endif

#ifndef LOGLOG_ERROR
#define LOGLOG_ERROR(tag, ...) \
    (void)android_printLog(ANDROID_LOG_ERROR, tag, __VA_ARGS__)
#endif

// ---------------------------------------------------------------------

/*
 * Simplified macro to send a verbose log message using the current LOG_TAG.
 */
#ifndef LOGV
#if LOG_NDEBUG
#define LOGV(...)   ((void)0)
#else
#define LOGV(...) LOG(LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#endif
#endif

#define CONDITION(cond)     (__builtin_expect((cond)!=0, 0))

#ifndef LOGV_IF
#if LOG_NDEBUG
#define LOGV_IF(cond, ...)   ((void)0)
#else
#define LOGV_IF(cond, ...) \
    ( (CONDITION(cond)) \
    ? LOG(LOG_VERBOSE, LOG_TAG, __VA_ARGS__) \
    : (void)0 )
#endif
#endif

/*
 * Simplified macro to send a debug log message using the current LOG_TAG.
 */
#ifndef LOGD
#define LOGD(...) LOG(LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#endif

#ifndef LOGD_IF
#if LOG_NDDEBUG
#define LOGD_IF(cond, ...) ((void)0)
#else
#define LOGD_IF(cond, ...) \
    ( (CONDITION(cond)) \
    ? LOG(LOG_DEBUG, LOG_TAG, __VA_ARGS__) \
    : (void)0 )
#endif
#endif

/*
 * Simplified macro to send an info log message using the current LOG_TAG.
 */
#ifndef LOGI
#define LOGI(...) LOG(LOG_INFO, LOG_TAG, __VA_ARGS__)
#endif

#ifndef LOGI_IF
#if LOG_NIDEBUG
#define LOGI_IF(cond, ...) ((void)0)
#else
#define LOGI_IF(cond, ...) \
    ( (CONDITION(cond)) \
    ? LOG(LOG_INFO, LOG_TAG, __VA_ARGS__) \
    : (void)0 )
#endif
#endif

/*
 * Simplified macro to send a warning log message using the current LOG_TAG.
 */
#ifndef LOGW
#define LOGW(...) LOG(LOG_WARN, LOG_TAG, __VA_ARGS__)
#endif

#ifndef LOGW_IF
#define LOGW_IF(cond, ...) \
    ( (CONDITION(cond)) \
    ? LOG(LOG_WARN, LOG_TAG, __VA_ARGS__) \
    : (void)0 )
#endif

/*
 * Simplified macro to send an error log message using the current LOG_TAG.
 */
#ifndef LOGE
#define LOGE(...) LOG(LOG_ERROR, LOG_TAG, __VA_ARGS__)
#endif

#ifndef LOGE_IF
#define LOGE_IF(cond, ...) \
    ( (CONDITION(cond)) \
    ? LOG(LOG_ERROR, LOG_TAG, __VA_ARGS__) \
    : (void)0 )
#endif

// ---------------------------------------------------------------------

/*
 * Conditional based on whether the current LOG_TAG is enabled at
 * verbose priority.
 */
#ifndef IF_LOGV
#if LOG_NDEBUG
#define IF_LOGV() if (false)
#else
#define IF_LOGV() IF_LOG(LOG_VERBOSE, LOG_TAG)
#endif
#endif

/*
 * Conditional based on whether the current LOG_TAG is enabled at
 * debug priority.
 */
#ifndef IF_LOGD
#define IF_LOGD() IF_LOG(LOG_DEBUG, LOG_TAG)
#endif

/*
 * Conditional based on whether the current LOG_TAG is enabled at
 * info priority.
 */
#ifndef IF_LOGI
#define IF_LOGI() IF_LOG(LOG_INFO, LOG_TAG)
#endif

/*
 * Conditional based on whether the current LOG_TAG is enabled at
 * warn priority.
 */
#ifndef IF_LOGW
#define IF_LOGW() IF_LOG(LOG_WARN, LOG_TAG)
#endif

/*
 * Conditional based on whether the current LOG_TAG is enabled at
 * error priority.
 */
#ifndef IF_LOGE
#define IF_LOGE() IF_LOG(LOG_ERROR, LOG_TAG)
#endif


// ---------------------------------------------------------------------

/*
 * Simplified macro to send a verbose system log message using the current LOG_TAG.
 */
#ifndef SLOGV
#if LOG_NDEBUG
#define SLOGV(...)   ((void)0)
#else
#define SLOGV(...) ((void)__android_log_buf_print(LOG_ID_SYSTEM, ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__))
#endif
#endif

#define CONDITION(cond)     (__builtin_expect((cond)!=0, 0))

#ifndef SLOGV_IF
#if LOG_NDEBUG
#define SLOGV_IF(cond, ...)   ((void)0)
#else
#define SLOGV_IF(cond, ...) \
    ( (CONDITION(cond)) \
    ? ((void)__android_log_buf_print(LOG_ID_SYSTEM, ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)) \
    : (void)0 )
#endif
#endif

/*
 * Simplified macro to send a debug system log message using the current LOG_TAG.
 */
#ifndef SLOGD
#define SLOGD(...) ((void)__android_log_buf_print(LOG_ID_SYSTEM, ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__))
#endif

#ifndef SLOGD_IF
#define SLOGD_IF(cond, ...) \
    ( (CONDITION(cond)) \
    ? ((void)__android_log_buf_print(LOG_ID_SYSTEM, ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)) \
    : (void)0 )
#endif

/*
 * Simplified macro to send an info system log message using the current LOG_TAG.
 */
#ifndef SLOGI
#define SLOGI(...) ((void)__android_log_buf_print(LOG_ID_SYSTEM, ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__))
#endif

#ifndef SLOGI_IF
#define SLOGI_IF(cond, ...) \
    ( (CONDITION(cond)) \
    ? ((void)__android_log_buf_print(LOG_ID_SYSTEM, ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)) \
    : (void)0 )
#endif

/*
 * Simplified macro to send a warning system log message using the current LOG_TAG.
 */
#ifndef SLOGW
#define SLOGW(...) ((void)__android_log_buf_print(LOG_ID_SYSTEM, ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__))
#endif

#ifndef SLOGW_IF
#define SLOGW_IF(cond, ...) \
    ( (CONDITION(cond)) \
    ? ((void)__android_log_buf_print(LOG_ID_SYSTEM, ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)) \
    : (void)0 )
#endif

/*
 * Simplified macro to send an error system log message using the current LOG_TAG.
 */
#ifndef SLOGE
#define SLOGE(...) ((void)__android_log_buf_print(LOG_ID_SYSTEM, ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__))
#endif

#ifndef SLOGE_IF
#define SLOGE_IF(cond, ...) \
    ( (CONDITION(cond)) \
    ? ((void)__android_log_buf_print(LOG_ID_SYSTEM, ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)) \
    : (void)0 )
#endif

    

// ---------------------------------------------------------------------

/*
 * Log a fatal error.  If the given condition fails, this stops program
 * execution like a normal assertion, but also generating the given message.
 * It is NOT stripped from release builds.  Note that the condition test
 * is -inverted- from the normal assert() semantics.
 */
#define LOG_ALWAYS_FATAL_IF(cond, ...) \
    ( (CONDITION(cond)) \
    ? ((void)android_printAssert(#cond, LOG_TAG, __VA_ARGS__)) \
    : (void)0 )

#define LOG_ALWAYS_FATAL(...) \
    ( ((void)android_printAssert(NULL, LOG_TAG, __VA_ARGS__)) )

/*
 * Versions of LOG_ALWAYS_FATAL_IF and LOG_ALWAYS_FATAL that
 * are stripped out of release builds.
 */
#if LOG_NDEBUG

#define LOG_FATAL_IF(cond, ...) ((void)0)
#define LOG_FATAL(...) ((void)0)

#else

#define LOG_FATAL_IF(cond, ...) LOG_ALWAYS_FATAL_IF(cond, __VA_ARGS__)
#define LOG_FATAL(...) LOG_ALWAYS_FATAL(__VA_ARGS__)

#endif

/*
 * Assertion that generates a log message when the assertion fails.
 * Stripped out of release builds.  Uses the current LOG_TAG.
 */
#define LOG_ASSERT(cond, ...) LOG_FATAL_IF(!(cond), __VA_ARGS__)
//#define LOG_ASSERT(cond) LOG_FATAL_IF(!(cond), "Assertion failed: " #cond)

// ---------------------------------------------------------------------

/*
 * Basic log message macro.
 *
 * Example:
 *  LOG(LOG_WARN, NULL, "Failed with error %d", errno);
 *
 * The second argument may be NULL or "" to indicate the "global" tag.
 */
#ifndef LOG
#define LOG(priority, tag, ...) \
    LOG##priority(tag, __VA_ARGS__)
#endif

/*
 * Log macro that allows you to specify a number for the priority.
 *
 * Since all of the above variations end up here, this is where the lower
 * priority messages can be filtered out. The if statement below can be
 * optimizaed out by the compiler since all of the expressions are
 * constant.
 */
#ifndef LOG_PRI
#define LOG_PRI(priority, tag, ...)                                     \
    ({                                                                  \
       if (((priority == ANDROID_LOG_VERBOSE) && (LOG_NDEBUG == 0)) ||  \
           ((priority == ANDROID_LOG_DEBUG) && (LOG_NDDEBUG == 0))  ||  \
           ((priority == ANDROID_LOG_INFO) && (LOG_NIDEBUG == 0))   ||  \
            (priority == ANDROID_LOG_WARN)                          ||  \
            (priority == ANDROID_LOG_ERROR)                         ||  \
            (priority == ANDROID_LOG_FATAL))                            \
                (void)android_printLog(priority, tag, __VA_ARGS__);     \
    })
#endif

/*
 * Log macro that allows you to pass in a varargs ("args" is a va_list).
 */
#ifndef LOG_PRI_VA
#define LOG_PRI_VA(priority, tag, fmt, args) \
    android_vprintLog(priority, NULL, tag, fmt, args)
#endif

/*
 * Conditional given a desired logging priority and tag.
 */
#ifndef IF_LOG
#define IF_LOG(priority, tag) \
    if (android_testLog(ANDROID_##priority, tag))
#endif

// ---------------------------------------------------------------------

/*
 * Event logging.
 */

/*
 * Event log entry types.  These must match up with the declarations in
 * java/android/android/util/EventLog.java.
 */
typedef enum {
    EVENT_TYPE_INT      = 0,
    EVENT_TYPE_LONG     = 1,
    EVENT_TYPE_STRING   = 2,
    EVENT_TYPE_LIST     = 3,
} AndroidEventLogType;


#define LOG_EVENT_INT(_tag, _value) {                                       \
        int intBuf = _value;                                                \
        (void) android_btWriteLog(_tag, EVENT_TYPE_INT, &intBuf,            \
            sizeof(intBuf));                                                \
    }
#define LOG_EVENT_LONG(_tag, _value) {                                      \
        long long longBuf = _value;                                         \
        (void) android_btWriteLog(_tag, EVENT_TYPE_LONG, &longBuf,          \
            sizeof(longBuf));                                               \
    }
#define LOG_EVENT_STRING(_tag, _value)                                      \
    ((void) 0)  /* not implemented -- must combine len with string */
/* TODO: something for LIST */

/*
 * ===========================================================================
 *
 * The stuff in the rest of this file should not be used directly.
 */

#define android_printLog(prio, tag, fmt...) \
    __android_log_print(prio, tag, fmt)

#define android_vprintLog(prio, cond, tag, fmt...) \
    __android_log_vprint(prio, tag, fmt)

#define android_printAssert(cond, tag, fmt...) \
    __android_log_assert(cond, tag, fmt)

#define android_writeLog(prio, tag, text) \
    __android_log_write(prio, tag, text)

#define android_bWriteLog(tag, payload, len) \
    __android_log_bwrite(tag, payload, len)
#define android_btWriteLog(tag, type, payload, len) \
    __android_log_btwrite(tag, type, payload, len)
	
// TODO: remove these prototypes and their users
#define android_testLog(prio, tag) (1)
#define android_writevLog(vec,num) do{}while(0)
#define android_write1Log(str,len) do{}while (0)
#define android_setMinPriority(tag, prio) do{}while(0)
//#define android_logToCallback(func) do{}while(0)
#define android_logToFile(tag, file) (0)
#define android_logToFd(tag, fd) (0)

typedef enum {
    LOG_ID_MAIN = 0,
    LOG_ID_RADIO = 1,
    LOG_ID_EVENTS = 2,
    LOG_ID_SYSTEM = 3,

    LOG_ID_MAX
} log_id_t;

/*
 * Send a simple string to the log.
 */
int __android_log_buf_write(int bufID, int prio, const char *tag, const char *text);
int __android_log_buf_print(int bufID, int prio, const char *tag, const char *fmt, ...);


#ifdef __cplusplus
}
#endif

#endif // _LIBS_CUTILS_LOG_H
