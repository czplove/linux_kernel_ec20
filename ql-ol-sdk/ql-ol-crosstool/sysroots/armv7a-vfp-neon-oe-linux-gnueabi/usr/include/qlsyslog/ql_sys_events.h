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

#ifndef _QL_SYS_EVENTS_H_ 
#define _QL_SYS_EVENTS_H_ 
#include <qlsyslog/ql_sys_log.h>

#ifdef __cplusplus
extern "C" {
#endif

#define QL_SYS_LOGV(tag, ...) ql_sys_log_print(QL_SYS_LOG_ID_SYSTEM,  QL_SYS_LOG_VERBOSE, tag, __VA_ARGS__)
#define QL_SYS_LOGD(tag, ...) ql_sys_log_print(QL_SYS_LOG_ID_SYSTEM,  QL_SYS_LOG_DEBUG,   tag, __VA_ARGS__)
#define QL_SYS_LOGI(tag, ...) ql_sys_log_print(QL_SYS_LOG_ID_SYSTEM,  QL_SYS_LOG_INFO,    tag, __VA_ARGS__)
#define QL_SYS_LOGW(tag, ...) ql_sys_log_print(QL_SYS_LOG_ID_SYSTEM,  QL_SYS_LOG_WARN,    tag, __VA_ARGS__)
#define QL_SYS_LOGE(tag, ...) ql_sys_log_print(QL_SYS_LOG_ID_SYSTEM,  QL_SYS_LOG_ERROR,   tag, __VA_ARGS__)
#define QL_SYS_LOGF(tag, ...) ql_sys_log_print(QL_SYS_LOG_ID_SYSTEM,  QL_SYS_LOG_FATAL,   tag, __VA_ARGS__)


#define QL_EVT_LOGV(tag, ...) ql_sys_log_print(QL_SYS_LOG_ID_EVENTS,  QL_SYS_LOG_VERBOSE, tag, __VA_ARGS__)
#define QL_EVT_LOGD(tag, ...) ql_sys_log_print(QL_SYS_LOG_ID_EVENTS,  QL_SYS_LOG_DEBUG,   tag, __VA_ARGS__)
#define QL_EVT_LOGI(tag, ...) ql_sys_log_print(QL_SYS_LOG_ID_EVENTS,  QL_SYS_LOG_INFO,    tag, __VA_ARGS__)
#define QL_EVT_LOGW(tag, ...) ql_sys_log_print(QL_SYS_LOG_ID_EVENTS,  QL_SYS_LOG_WARN,    tag, __VA_ARGS__)
#define QL_EVT_LOGE(tag, ...) ql_sys_log_print(QL_SYS_LOG_ID_EVENTS,  QL_SYS_LOG_ERROR,   tag, __VA_ARGS__)
#define QL_EVT_LOGF(tag, ...) ql_sys_log_print(QL_SYS_LOG_ID_EVENTS,  QL_SYS_LOG_FATAL,   tag, __VA_ARGS__)

#define QL_EVT_SIG_LOGV(tag, sig, info, ptr) ql_sys_log_signal(QL_SYS_LOG_ID_EVENTS,  QL_SYS_LOG_VERBOSE, tag, sig, info, ptr)
#define QL_EVT_SIG_LOGD(tag, sig, info, ptr) ql_sys_log_signal(QL_SYS_LOG_ID_EVENTS,  QL_SYS_LOG_DEBUG,   tag, sig, info, ptr)
#define QL_EVT_SIG_LOGI(tag, sig, info, ptr) ql_sys_log_signal(QL_SYS_LOG_ID_EVENTS,  QL_SYS_LOG_INFO,    tag, sig, info, ptr)
#define QL_EVT_SIG_LOGW(tag, sig, info, ptr) ql_sys_log_signal(QL_SYS_LOG_ID_EVENTS,  QL_SYS_LOG_WARN,    tag, sig, info, ptr)
#define QL_EVT_SIG_LOGE(tag, sig, info, ptr) ql_sys_log_signal(QL_SYS_LOG_ID_EVENTS,  QL_SYS_LOG_ERROR,   tag, sig, info, ptr)
#define QL_EVT_SIG_LOGF(tag, sig, info, ptr) ql_sys_log_signal(QL_SYS_LOG_ID_EVENTS,  QL_SYS_LOG_FATAL,   tag, sig, info, ptr)


int ql_sys_log_signal(QL_SYS_LOG_ID_E log_id, QL_SYS_LOG_PRIORITY_E prio, const char *tag, int sig_num, void *info, void *ptr);


#ifdef __cplusplus
}
#endif

#endif /* _QL_SYS_LOG_H_ */
