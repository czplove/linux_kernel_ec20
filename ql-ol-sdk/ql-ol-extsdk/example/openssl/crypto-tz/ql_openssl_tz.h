/**
 *@file     quectel_openssl_tz.h
 *@date     2017-12-27
 *@author   running.qian
 *@brief    Quecte Openssl-TZ
 */

#ifndef __QUECTEL_OPENSSL_TZ_H__
#define __QUECTEL_OPENSSL_TZ_H__

#define QL_TZ_ENGINE_ID             "QuecTZ"
#define QL_TZ_AES_TAG_SIZE          24
#define QL_TZ_AES_KEY_SIZE          56
#define QL_TZ_AES_DATA_MAX_SIZE     2048 //block size 16 bytes(128bit), 128,256,512,1024,2048
#define QL_TZ_RSA_DATA_MAX_SIZE     2048

#endif