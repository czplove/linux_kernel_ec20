/**
 *@file     ql_dev.h
 *@date     2017-05-02
 *@author   
 *@brief    
 */  

#ifndef __QL_DEV_H__
#define __QL_DEV_H__


#define QL_OEM_ABOUT_MAX_COUNT          32
#define QL_OEM_ABOUT_NAME_MAX_SIZE      32
#define QL_OEM_ABOUT_INFO_MAX_SIZE      128

typedef struct {
    char oem_info_name[QL_OEM_ABOUT_NAME_MAX_SIZE + 1];
    char oem_info_vaule[QL_OEM_ABOUT_INFO_MAX_SIZE + 1];  
} ql_oem_about_info_s;


typedef struct {
    char manufacturer[33];
    char product_name[33];
    char firmware_version[65];
    char firmware_date[33];
    char apps_verison[65];
    
    int oem_about_count;
    ql_oem_about_info_s oem_about[QL_OEM_ABOUT_MAX_COUNT];//only support max 32 item info
} ql_module_about_info_s;


/*
    Get the IMEI string.
     -imei      : input buffer
     -imeiLen   : input buffer length
*/
extern int QL_DEV_GetImei(char* imei, size_t imeiLen);


/*
    Get the FirmwareVersion string.
     -version   : input buffer
     -versionLen: input buffer length
*/
extern int QL_DEV_GetFirmwareVersion(char* version, size_t versionLen);


/*
    Get the DeviceModel string.
     -model     : input buffer
     -modelLen  : input buffer length
*/
extern int QL_DEV_GetDeviceModel(char* model, size_t modelLen);

/*
    Get the MEID string.
     -meid      : input buffer
     -meidLen   : input buffer length
*/
extern int QL_DEV_GetMeid(char* meid, size_t meidLen);

/*
    Get the ESN string.
     -esn       : input buffer
     -esnLen    : input buffer length
*/
extern int QL_DEV_GetEsn(char* esn, size_t esnLen);


/*
 * Get OEM About's Info
 * -about: save about result,
 * -about_count: about buffer num
 * Return: actual about count
 */
extern int QL_OEM_About_Get(ql_oem_about_info_s *about, int about_count);

/*
 * Set OEM About's Info
 * -about: input about info,
 * -about_count: about nums
 * Return: actual about count
 * Note: it will cleal all old info
 */
extern int QL_OEM_About_Set(ql_oem_about_info_s *about, int about_count);

extern int QL_OEM_About_Get_ByName(ql_oem_about_info_s *about);

extern int QL_OEM_About_Set_ByName(ql_oem_about_info_s *about);

extern int QL_Module_About_Get(ql_module_about_info_s *about);

#endif // __QL_DEV_H__

