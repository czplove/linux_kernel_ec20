#ifndef __QL_MCM_DM_H__
#define __QL_MCM_DM_H__


#define QL_MAX_DMS_IMEI_LEN 32      /**  Max IMEI limit */
#define QL_MAX_DMS_MEID_LEN 32      /**  Max MEIID limit */
#define QL_MAX_DMS_HARDWARE_REV_LEN 256

typedef uint32 dm_client_handle_type;

typedef struct 
 {
    uint8_t imei_valid; 
    char imei[QL_MAX_DMS_IMEI_LEN + 1];    /**<   imei.*/
    uint8_t meid_valid; 
    char meid[QL_MAX_DMS_MEID_LEN + 1];    /**<   meid.*/
}ql_dm_device_serial_numbers_t;

typedef struct 
 {
    char device_rev_id[QL_MAX_DMS_HARDWARE_REV_LEN + 1];    /**<   meid.*/
}ql_dm_device_rev_id_t;


int QL_MCM_DM_Client_Init(atc_client_handle_type  *ph_dm);

/* Stop getting coordinates */
int QL_MCM_DM_Client_Deinit(dm_client_handle_type h_dm);

E_QL_ERROR_CODE_T QL_MCM_DM_GetAirplaneMode
(
    dm_client_handle_type                   h_dm,
    E_QL_MCM_DM_AIRPLANE_MODE_TYPE_T        *pe_airplane_mode  ///< [OUT] Radio mode
);

E_QL_ERROR_CODE_T QL_MCM_DM_SetAirplaneMode
(
    dm_client_handle_type                   h_dm,
    E_QL_MCM_DM_AIRPLANE_MODE_TYPE_T        e_airplane_mode  ///< [IN] Airplane mode
);

E_QL_ERROR_CODE_T QL_MCM_DM_SetAirplaneModeChgInd
(
    dm_client_handle_type       h_dm,
    uint32_t                    ind_onoff  ///< [IN] 0: indication off, 1: on
);

/* Get serial numbers (IMEI MEID)*/
E_QL_ERROR_CODE_T QL_MCM_DM_GetSerialNumbers
(
    dm_client_handle_type       h_dm,
    ql_dm_device_serial_numbers_t  *serial_numbers
);

/* Get Firmware revisition identification */
E_QL_ERROR_CODE_T QL_MCM_DM_GetFirmware
(
    dm_client_handle_type       h_dm,
    ql_dm_device_rev_id_t      *hardware_rev
);


#endif // __QL_MCM_DM_H__

