#ifndef __AUDIO_LOG_H__
#define __AUDIO_LOG_H__

//#include <unistd.h>
//#include <stdlib.h>
//#include <pthread.h>
//#include <getopt.h>
//#include <ctype.h>
//#include "errno.h"
////#include "event.h"
//#include "msg.h"
////#include "log.h"
//#include "diagpkt.h"
//#include "diagcmd.h"
//#include "diagdiag.h"
//#include "diag_lsm.h"
////#include "../src/ts_linux.h"
//#include <limits.h>

#include <cutils/logger.h>
#include <android/log.h>

enum AUDIO_LOG_LEVEL {
    AUDIO_DEBUG = 0,
    AUDIO_INFO,
    AUDIO_WARN,
    AUDIO_ERROR,
    AUDIO_FATAL,
};

//#define PRINT_LEVEL QL_AUDIO_DEBUG

#define AUDIO_LOG(level, format, ...) \
{ \
    switch(level) \
    { \
        case AUDIO_DEBUG: \
            __quectel_log_print(ANDROID_LOG_DEBUG, "", "%s[%d]:"format, __FILE__, __LINE__, ##__VA_ARGS__); \
            break; \
        case AUDIO_INFO: \
            __quectel_log_print(ANDROID_LOG_INFO, "", "%s[%d]:"format, __FILE__, __LINE__, ##__VA_ARGS__); \
            break; \
        case AUDIO_WARN: \
            __quectel_log_print(ANDROID_LOG_WARN, "", "%s[%d]:"format, __FILE__, __LINE__, ##__VA_ARGS__); \
            break; \
        case AUDIO_ERROR: \
            __quectel_log_print(ANDROID_LOG_ERROR, "", "%s[%d]:"format, __FILE__, __LINE__, ##__VA_ARGS__); \
            break; \
        case AUDIO_FATAL: \
            __quectel_log_print(ANDROID_LOG_FATAL, "", "%s[%d]:"format, __FILE__, __LINE__, ##__VA_ARGS__); \
            break; \
        default: \
            break; \
    } \
}
//    if (access(QXDM_CONF_FILE, F_OK) != -1) \
//    { \
//        Diag_LSM_Init(NULL); \
//        char buffer[256] = {0}; \
//        snprintf(buffer, 256, format, __VA_ARGS__); \
//        switch(level) \
//        { \
//            case AUDIO_DEBUG: \
//                MSG_SPRINTF_1(MSG_SSID_LINUX_DATA, MSG_LVL_LOW, "%s", buffer); \
//                break; \
//            case AUDIO_INFO: \
//                MSG_SPRINTF_1(MSG_SSID_LINUX_DATA, MSG_LVL_MED, "%s", buffer); \
//                break; \
//            case AUDIO_WARN: \
//                MSG_SPRINTF_1(MSG_SSID_LINUX_DATA, MSG_LVL_HIGH, "%s", buffer); \
//                break; \
//            case AUDIO_ERROR: \
//                MSG_SPRINTF_1(MSG_SSID_LINUX_DATA, MSG_LVL_ERROR, "%s", buffer); \
//                break; \
//            case AUDIO_FATAL: \
//                MSG_SPRINTF_1(MSG_SSID_LINUX_DATA, MSG_LVL_FATAL, "%s", buffer); \
//                break; \
//            default: \
//                break; \
//        } \
//        Diag_LSM_DeInit(); \
//    } \
//}

#endif //__AUDIO_LOG_H__
