/**
 * @file quectel_log.h
 * @brief Quectel log public function declarations.
 * 
 * @note Provide write log interface. the log is written to include file, syslog, /dev/console, standard I/O streams
 *
 * @copyright Copyright (c) 2010-2017 @ Quectel Wireless Solutions Co., Ltd.
 */

#ifndef __QUECTEL_LOG_H__
#define __QUECTEL_LOG_H__

#include <stdbool.h>

#ifndef __func__
#define __func__ __FUNCTION__
#endif

#ifndef __line__
#define __line__ __LINE__
#endif

#ifndef __file__
#define __file__ __FILE__
#endif

#ifdef __GNUC__
#define PRINTF_FORMAT(n, m) __attribute ((format(printf, (n), (m))))
#else
#define PRINTF_FORMAT(n, m)
#endif

#ifdef QL_SYS_LOG
#include <qlsyslog/ql_sys_log.h>
#endif

#define __LOG_FATAL   0 /*!< action must be taken immediately */
#define __LOG_ERROR   1 /*!< error conditions */
#define __LOG_WARN    2 /*!< warning conditions */
#define __LOG_INFO    3 /*!< informational message */
#define __LOG_DEBUG   4 /*!< debug-level message */

/**
 * Initialization the log library.
 * 
 * @param[in] enable               The log switch to be set.
 * @param[in] level                The log level configuration.
 * @param[in] syslog_enable        The log output syslog to be set.
 * @param[in] stdout_enable        The log output standard I/O streams(stdout) to be set.
 * @param[in] console_enable       The log output /dev/console device to be set.
 * @param[in] file                 The log output file to be set.
 * @param[in] file_max_size        Maximun file size(KB).
 * @param[in] file_max_count       Maximun files keep.
 *
 * @return None
 * 
 */
extern void quectel_log_init(bool enable,
			     short level,
			     bool syslog_enable,
			     bool stdout_enable,
			     bool console_enable,
			     char *file,
			     int file_max_size,
			     int file_max_count);

/**
 * Enable or disable the log switch.
 * 
 * @param[in] enable    The log switch to be set.
 *
 * @return None
 * 
 */
extern void quectel_log_enable_set(bool enable);

/**
 * The log level configuration.
 * 
 * @param[in] level  The log level configuration(0-4).    
 *
 * @return On success, Zero is returned. On error, -1 is returned.
 * 
 */
extern int quectel_log_level_set(short level);

/**
 * Enable or disable log to syslog. if the syslogd will be already running.
 * 
 * @param[in] enable    The log output syslog to be set.
 *
 * @return None
 * 
 */
extern void quectel_log_syslog_enable_set(bool enable);

/**
 * Enable or disable log to standard I/O streams(stdout).
 * 
 * @param[in] enable    The log output stdout to be set.
 *
 * @return None
 * 
 */
extern void quectel_log_stdout_enable_set(bool enable);

/**
 * Enable or disable log to /dev/console device.
 * 
 * @param[in] enable    The log output stdout to be set.
 *
 * @return None
 * 
 */
extern void quectel_log_console_enable_set(bool enable);

/**
 * Log to File
 * 
 * @param[in] file                 The log output file to be set.
 * @param[in] file_max_size        Maximun file size(KB).
 * @param[in] file_max_count       Maximun files keep.
 *
 * @return None
 * 
 */
extern int quectel_log_file_set(char *file, int file_max_size, int file_max_count);

/**
 * Write buffer to log.
 *
 * @param[in] file           File name of source code.
 * @param[in] line           Line of source code file.
 * @param[in] func           Function name of source code.
 * @param[in] level          The log level configuration.
 * @param[in] format         Character string.
 * @param[in] ...
 *
 * @return None
 */
extern void __ql_log(const char *file,
		  const int line,
		  const char *func,
		  const int level,
		  const char *format,
		  ...)
PRINTF_FORMAT(5, 6);

/**
 * Write the buffer of hexadecimal to standard I/O streams(stdout).
 *
 * @param[in] file           File name of source code.
 * @param[in] line           Line of source code file.
 * @param[in] func           Function name of source code.
 * @param[in] buffer         Write buffer to log.
 * @param[in] size           Write total size of buffer.
 *
 * @return None
 *
 */
extern void __print_buffer(const char *file,
			   const int line,
			   const char *func,
			   unsigned char *buffer,
			   int size);

/**
 * Write fatal message to log - action must be taken immediately.
 * 
 * @param[in] fmt     Character string.
 * @param[in] ...
 *
 * @return None
 *
 */
#ifdef QL_SYS_LOG
#define log_fatal(fmt, ...)	do{ \
    __ql_log(__file__, __line__, __func__, __LOG_FATAL, (fmt), ##__VA_ARGS__); \
    ql_sys_log_print(QL_SYS_LOG_ID_SYSTEM, QL_SYS_LOG_FATAL, QL_SYS_LOG_TAG, "[%s  %d]: "fmt, __func__, __LINE__, ##__VA_ARGS__);\
}while(0)
#else
#define log_fatal(fmt, ...)						\
	__ql_log(__file__, __line__, __func__, __LOG_FATAL, (fmt), ##__VA_ARGS__)
#endif
/**
 * Write error message to log.
 * 
 * @param[in] fmt     Character string.
 * @param[in] ...
 *
 * @return None
 *
 */
#ifdef QL_SYS_LOG
#define log_err(fmt, ...) do{ \
    __ql_log(__file__, __line__, __func__, __LOG_ERROR, (fmt), ##__VA_ARGS__); \
    ql_sys_log_print(QL_SYS_LOG_ID_SYSTEM, QL_SYS_LOG_ERROR, QL_SYS_LOG_TAG, "[%s  %d]: "fmt, __func__, __LINE__, ##__VA_ARGS__);\
}while(0)
#else
#define log_err(fmt, ...)						\
	__ql_log(__file__, __line__, __func__, __LOG_ERROR, (fmt), ##__VA_ARGS__)
#endif
/**
 * Write warning message to log
 * 
 * @param[in] fmt     Character string.
 * @param[in] ...
 *
 * @return None
 *
 */
#ifdef QL_SYS_LOG
#define log_warn(fmt, ...) do{ \
    __ql_log(__file__, __line__, __func__, __LOG_WARN, (fmt), ##__VA_ARGS__); \
    ql_sys_log_print(QL_SYS_LOG_ID_SYSTEM, QL_SYS_LOG_INFO, QL_SYS_LOG_TAG, "[%s  %d]: "fmt, __func__, __LINE__, ##__VA_ARGS__);\
}while(0)
#else
#define log_warn(fmt, ...)						\
	__ql_log(__file__, __line__, __func__, __LOG_WARN, (fmt), ##__VA_ARGS__)
#endif

/**
 * Write informational message to log
 * 
 * @param[in] fmt     Character string.
 * @param[in] ...
 *
 * @return None
 *
 */
#ifdef QL_SYS_LOG
#define log_info(fmt, ...) do{\
	__ql_log(__file__, __line__, __func__, __LOG_INFO, (fmt), ##__VA_ARGS__); \
    ql_sys_log_print(QL_SYS_LOG_ID_SYSTEM, QL_SYS_LOG_INFO, QL_SYS_LOG_TAG, "[%s  %d]: "fmt, __func__, __LINE__, ##__VA_ARGS__); \
}while(0)
#else
#define log_info(fmt, ...)						\
	__ql_log(__file__, __line__, __func__, __LOG_INFO, (fmt), ##__VA_ARGS__)
#endif

/**
 * Write deubg message to log
 * 
 * @param[in] fmt     Character string.
 * @param[in] ...
 *
 * @return None
 *
 */
#ifdef QL_SYS_LOG
#define log_dbg(fmt, ...) do{\
	__ql_log(__file__, __line__, __func__, __LOG_DEBUG, (fmt), ##__VA_ARGS__);\
    ql_sys_log_print(QL_SYS_LOG_ID_SYSTEM, QL_SYS_LOG_DEBUG, QL_SYS_LOG_TAG, "[%s  %d]: "fmt, __func__, __LINE__,##__VA_ARGS__); \
}while(0)
#else
#define log_dbg(fmt, ...)						\
	__ql_log(__file__, __line__, __func__, __LOG_DEBUG, (fmt), ##__VA_ARGS__)
#endif

#define print_buffer(buffer, size) \
	__print_buffer(__file__, __line__, __func__, (buffer), (size))

#endif /* end of __QUECTEL_LOG_H__ */
