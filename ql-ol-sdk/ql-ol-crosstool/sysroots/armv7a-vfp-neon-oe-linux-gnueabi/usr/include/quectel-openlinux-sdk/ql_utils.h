#ifndef __QL_UTILS_H__
#define __QL_UTILS_H__
/**
 @file ql_utils.h 
 @brief
    QuecOpen utils 
*/
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

/** Check whether the string is empty 0:empty, 1:not empty */
#define QL_STR_EMPTY(str)  (!(str) || !((str)[0]))

typedef enum {
    QL_SDK_LOG_MIN = 0,
    QL_SDK_LOG_DEFAULT,
    QL_SDK_LOG_VERBOSE,
    QL_SDK_LOG_DEBUG,
    QL_SDK_LOG_INFO,
    QL_SDK_LOG_WARN,
    QL_SDK_LOG_ERROR,
    QL_SDK_LOG_FATAL,
    QL_SDK_LOG_MAX
} QL_SDK_LOG_PRIORITY_E;

#define QL_API_LOGV(fmt, ...) ql_log_print(__FUNCTION__, __LINE__, QL_SDK_LOG_VERBOSE, fmt, ##__VA_ARGS__)
#define QL_API_LOGD(fmt, ...) ql_log_print(__FUNCTION__, __LINE__, QL_SDK_LOG_DEBUG,   fmt, ##__VA_ARGS__)
#define QL_API_LOGI(fmt, ...) ql_log_print(__FUNCTION__, __LINE__, QL_SDK_LOG_INFO,    fmt, ##__VA_ARGS__)
#define QL_API_LOGW(fmt, ...) ql_log_print(__FUNCTION__, __LINE__, QL_SDK_LOG_WARN,    fmt, ##__VA_ARGS__)
#define QL_API_LOGE(fmt, ...) ql_log_print(__FUNCTION__, __LINE__, QL_SDK_LOG_ERROR,   fmt, ##__VA_ARGS__)
#define QL_API_LOGF(fmt, ...) ql_log_print(__FUNCTION__, __LINE__, QL_SDK_LOG_FATAL,   fmt, ##__VA_ARGS__)


/**-------------------------------------------------------------------------------------------------
 * @brief Get file size 
 * @param[in] file_path File path
 * @return   -1 : error \n
 *            >=0 : succeed \n
 -------------------------------------------------------------------------------------------------*/
int ql_file_get_size(const char *file_path);

/**-------------------------------------------------------------------------------------------------
 * @brief Check whether the file exists
 * @param[in] file_path File path
 * @return   0 : file exists \n
 *           1 : file is not exists \n
 *          -1 : error ocurred, exp, file_path is empty"
 -------------------------------------------------------------------------------------------------*/
int ql_file_exist(const char *file_path);

/**-------------------------------------------------------------------------------------------------
 * @brief Writeback the file immediately, wapper of shell command fsync
 * @param[in] file_path File path
 * @return   0 : succeed \n
 *          -1 : error ocurred, exp, file_path is empty"
 -------------------------------------------------------------------------------------------------*/
int ql_file_sync(const char *file_path);

/**-------------------------------------------------------------------------------------------------
 * @brief Check whether the progress exists
 * @param[in] app_name Progress name
 * @return   0 : exists \n
 *          -1 : not exists or error ocurred"
 -------------------------------------------------------------------------------------------------*/
int ql_app_exist(const char *app_name);

/**-------------------------------------------------------------------------------------------------
 * @brief Wapper of system call : system, max length of the shell command if 256 
 * @param[in] fmt 
 * @return  Same to  the system call : system
 -------------------------------------------------------------------------------------------------*/
int ql_system(const char *fmt, ...);

/**-------------------------------------------------------------------------------------------------
 * @brief Get current time in millisecond 
 * @return Current time in millisecond 
 -------------------------------------------------------------------------------------------------*/
uint64_t ql_get_tmr_ms(void);

/**-------------------------------------------------------------------------------------------------
 * @brief Log API, max length 512 
 * @param[in] func 
 * @param[in] line
 * @param[in] prio
 * @param[in] fmt
 * @return 0:succeed 
 -------------------------------------------------------------------------------------------------*/
int ql_log_print(const char *func, int line, QL_SDK_LOG_PRIORITY_E prio, const char *fmt, ...);


#ifdef __cplusplus
}
#endif


#endif

