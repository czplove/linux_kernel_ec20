/**
 *@file     ql_mcm_sim.h
 *@date     2017-05-02
 *@author   
 *@brief    
 */   

#ifndef __QL_MCM_SIM_H__
#define __QL_MCM_SIM_H__

#include "ql_mcm.h"

#define QL_SIM_IMSI_LEN_MAX     16  /**  Maximum length of IMSI data. */
#define QL_SIM_ICCID_LEN_MAX    20  /**  Maximum length of ICCID data. */

typedef uint32 sim_client_handle_type;

typedef enum 
{
    E_QL_MCM_SIM_SLOT_ID_1          = 0xB01,    /**< Identify card in slot 1.  */
    E_QL_MCM_SIM_SLOT_ID_2          = 0xB02,    /**< Identify card in slot 2.  */
}E_QL_MCM_SIM_SLOT_ID_TYPE_T;

typedef enum 
{
    E_QL_MCM_SIM_APP_TYPE_UNKNOWN   = 0xB00,    /**<  Unknown application type  */
    E_QL_MCM_SIM_APP_TYPE_3GPP      = 0xB01,    /**< Identify the SIM/USIM application on the card.  */
    E_QL_MCM_SIM_APP_TYPE_3GPP2     = 0xB02,    /**< Identify the RUIM/CSIM application on the card.  */
    E_QL_MCM_SIM_APP_TYPE_ISIM      = 0xB03,    /**< Identify the ISIM application on the card.  */
}E_QL_MCM_SIM_APP_TYPE_T;

typedef struct 
{
    E_QL_MCM_SIM_SLOT_ID_TYPE_T     e_slot_id;  /**< Indicates the slot to be used. */
    E_QL_MCM_SIM_APP_TYPE_T         e_app;      /**< Indicates the type of the application. */
}QL_SIM_APP_ID_INFO_T;  /* Type */


#define QL_SIM_MCC_LEN      3   /**  Length of the MCC. */
#define QL_SIM_MNC_MAX      3   /**  Maximum length of the MNC. */
#define QL_SIM_PLMN_NUM_MAX 24  /**  Maximum number of PLMN data sets. */
typedef struct 
{
    char mcc[QL_SIM_MCC_LEN];  /**< MCC value in ASCII characters.*/
    uint32_t mnc_len;               /**< Must be set to the number of elements in the MNC. */
    char mnc[QL_SIM_MNC_MAX];  /**< MNC value in ASCII characters.*/
}QL_SIM_PLMN_INFO_T;

typedef struct 
{
    uint32_t            preferred_operator_list_len;                    /**< Must be set to the number of elements in preferred_operator_list. */
    QL_SIM_PLMN_INFO_T  preferred_operator_list[QL_SIM_PLMN_NUM_MAX];   /**< Preferred operator list. */
}QL_SIM_PREFERRED_OPERATOR_LIST_T;  /* Message */


#define QL_MCM_SIM_PIN_LEN_MAX  8   /**  Maximum length of PIN data. */
typedef enum 
{
    E_QL_MCM_SIM_PIN_ID_1 = 0xB01,  /**< Level 1 user verification.  */
    E_QL_MCM_SIM_PIN_ID_2 = 0xB02,  /**< Level 2 user verification.  */
}E_QL_MCM_SIM_PIN_ID_TYPE_T;

typedef struct 
{
    QL_SIM_APP_ID_INFO_T        app_info;                           /**< Application identification information. */
    E_QL_MCM_SIM_PIN_ID_TYPE_T  pin_id;                             /**< PIN ID. */
    uint32_t                    pin_value_len;                      /**< Must be set to the number of elements in pin_value. */
    char                        pin_value[QL_MCM_SIM_PIN_LEN_MAX];  /*  Value of the PIN */
}QL_SIM_VERIFY_PIN_INFO_T;

/** Changes the PIN value of an application. The application must pass both the 
    new and the old values of the PIN to complete the operation.     
    The same PIN can be used by multiple sessions (i.e., the PIN is shared
    between GSM and RUIM in an ICC card). The PIN is automatically verified
    for all the sessions when the command is executed. */
typedef struct 
{
    QL_SIM_APP_ID_INFO_T        app_info;                               /**< Application identification information. */
    E_QL_MCM_SIM_PIN_ID_TYPE_T  pin_id;                                 /**< PIN ID. */
    uint32_t                    old_pin_value_len;                      /**< Must be set to the number of elements in old_pin_value. */
    char                        old_pin_value[QL_MCM_SIM_PIN_LEN_MAX];  /**< Value of the old PIN as a sequence of ASCII characters. */
    uint32_t                    new_pin_value_len;                      /**< Must be set to the number of elements in new_pin_value. */
    char                        new_pin_value[QL_MCM_SIM_PIN_LEN_MAX];  /**< Value of the new PIN as a sequence of ASCII characters. */
}QL_SIM_CHANGE_PIN_INFO_T;

/** Unblocks a blocked PIN using the PUK code. The client must pass PUK1 to unblock PIN1 or PUK2 to unblock PIN2.
    The same PIN can be used by multiple sessions (i.e., the PIN is shared between GSM and RUIM in an ICC card). 
    The PIN is automatically verified for all the sessions when the command is executed. */
typedef struct 
{
    QL_SIM_APP_ID_INFO_T        app_info;                               /**< Application identification information. */
    E_QL_MCM_SIM_PIN_ID_TYPE_T  pin_id;                                 /**< PIN ID. */
    uint32_t                    puk_value_len;                          /**< Must be set to the number of elements in puk_value. */
    char                        puk_value[QL_MCM_SIM_PIN_LEN_MAX];      /**< Value of the PUK as a sequence of ASCII characters. */
    uint32_t                    new_pin_value_len;                      /**< Must be set to the number of elements in new_pin_value. */
    char                        new_pin_value[QL_MCM_SIM_PIN_LEN_MAX];  /**< Value of the new PIN as a sequence of ASCII characters. */
}QL_SIM_UNBLOCK_PIN_INFO_T;

/** Enables the PIN on an application. */
typedef  QL_SIM_VERIFY_PIN_INFO_T QL_SIM_ENABLE_PIN_INFO_T; //Same 

/** Disables the PIN of an application, */
typedef  QL_SIM_VERIFY_PIN_INFO_T QL_SIM_DISABLE_PIN_INFO_T; //Same 


typedef enum 
{
    E_QL_MCM_SIM_PERSO_FEATURE_TYPE_UNKNOWN                 = 0xB00, /**<  Unknown personalization feature.  */
    E_QL_MCM_SIM_PERSO_FEATURE_TYPE_3GPP_NETWORK            = 0xB01, /**<  Featurization based on 3GPP MCC and MNC.  */
    E_QL_MCM_SIM_PERSO_FEATURE_TYPE_3GPP_NETWORK_SUBSET     = 0xB02, /**<  Featurization based on 3GPP MCC, MNC, and IMSI digits 6 and 7.  */
    E_QL_MCM_SIM_PERSO_FEATURE_TYPE_3GPP_SERVICE_PROVIDER   = 0xB03, /**<  Featurization based on 3GPP MCC, MNC, and GID1.  */
    E_QL_MCM_SIM_PERSO_FEATURE_TYPE_3GPP_CORPORATE          = 0xB04, /**<  Featurization based on 3GPP MCC, MNC, GID1, and GID2.  */
    E_QL_MCM_SIM_PERSO_FEATURE_TYPE_3GPP_SIM                = 0xB05, /**<  Featurization based on the 3GPP IMSI.  */
    E_QL_MCM_SIM_PERSO_FEATURE_TYPE_3GPP2_NETWORK_TYPE_1    = 0xB06, /**<  Featurization based on 3GPP2 MCC and MNC.  */
    E_QL_MCM_SIM_PERSO_FEATURE_TYPE_3GPP2_NETWORK_TYPE_2    = 0xB07, /**<  Featurization based on 3GPP2 IRM code.  */
    E_QL_MCM_SIM_PERSO_FEATURE_TYPE_3GPP2_RUIM              = 0xB08, /**<  Featurization based on 3GPP2 IMSI_M.  */
}E_QL_MCM_SIM_PERSO_FEATURE_TYPE_T;

typedef enum 
{
    E_QL_MCM_SIM_PERSO_OPERATION_DEACTIVATE                 = 0xB00, /**<  Disable an active personalization feature.  */
    E_QL_MCM_SIM_PERSO_OPERATION_UNBLOCK                    = 0xB01, /**<  Unblock a personalization feature that has been blocked.  */
}E_QL_MCM_SIM_PERSO_OPERATION_TYPE_T;


#define QL_MCM_SIM_CONTROL_KEY_LEN_MAX 16   /**  Maximum length of personalization control key data */
/** Deactivates or unblocks the personalization on the phone.
    Each feature can be deactivated/unblocked independently of the other features. */
typedef struct 
{
    E_QL_MCM_SIM_PERSO_FEATURE_TYPE_T       e_feature_type;                /**< Indicates the personalization feature to deactivate or unblock. */
    E_QL_MCM_SIM_PERSO_OPERATION_TYPE_T     e_operation;              /**< Indicates the operation to perform. */
    uint32_t                                ctrl_key_value_len;     /**< Must be set to the number of elements in ck_value. */
    char                                    ctrl_key_value[QL_MCM_SIM_CONTROL_KEY_LEN_MAX];/**< Control key value. This value is a sequence of ASCII characters.*/
}QL_MCM_SIM_DEPERSONALIZE_INFO_T;  /* Type */


typedef struct 
{
    char        mcc[MCM_SIM_MCC_LEN_V01];       /**< MCC value in ASCII characters.*/
    uint32_t    mnc_len;                        /**< Must be set to the number of elements in the MNC. */
    char        mnc[MCM_SIM_MNC_MAX_V01];       /**< MNC value in ASCII characters.*/
}QL_MCM_SIM_NW_PERSONAL_INFO_T;

typedef struct 
{
    QL_MCM_SIM_NW_PERSONAL_INFO_T   network;    /**< MCC and MNC network information.*/
    char                            digit6;     /**< Digit 6 of the IMSI in ASCII character.*/
    char                            digit7;     /**< Digit 7 of the IMSI in ASCII character.*/
}QL_MCM_SIM_NW_SUBSET_PERSONAL_INFO_T;

typedef struct 
{
    QL_MCM_SIM_NW_PERSONAL_INFO_T   network;    /**< MCC and MNC network information.*/
    uint8_t                         gid1;       /**< Service provider code found in GID1.*/
}QL_MCM_SIM_NW_SP_PERSONAL_INFO_T;

typedef struct 
{
    QL_MCM_SIM_NW_PERSONAL_INFO_T   network;    /**< MCC and MNC network information.*/
    uint8_t                         gid1;       /**< Service provider code found in GID1.*/
    uint8_t                         gid2;       /**< Corporate customer code found in GID2. @newpagetable */
}QL_MCM_SIM_GW_CORPORATE_PERSONAL_INFO_T;

#define QL_MCM_SIM_MSIN_LEN_MAX     10          /**  Maximum length of the MSIN. */
typedef struct 
{
    QL_MCM_SIM_NW_PERSONAL_INFO_T   network;    /**< MCC and MNC network information.*/
    uint32_t                        msin_len;   /**< Must be set to the number of elements in MSIN. */
    char                            msin[QL_MCM_SIM_MSIN_LEN_MAX];/**< MSIN value stored on the card in ASCII characters.*/
}QL_MCM_SIM_SIM_PERSONAL_INFO_T;  /* Type */

typedef struct 
{
    char                            irm_code[4];/**< First 4 digits of the IRM-based MIN of IMSI_M in ASCII characters.*/
}QL_MCM_SIM_1X_NW_TYPE2_PERSONAL_INFO_T;


#define QL_MCM_SIM_PERSO_NUM_NW_MAX     85  /**  Maximum number of network personalization data sets. */
#define QL_MCM_SIM_PERSO_NUM_NS_MAX     64  /**  Maximum number of network subset personalization data sets. */
#define QL_MCM_SIM_PERSO_NUM_GW_SP_MAX  64  /**  Maximum number of service provider personalization data sets. */
#define QL_MCM_SIM_PERSO_NUM_GW_CP_MAX  51  /**  Maximum number of corporate personalization data sets. */
#define QL_MCM_SIM_PERSO_NUM_SIM_MAX    32  /**  Maximum number of SIM personalization data sets. */
#define QL_MCM_SIM_PERSO_NUM_1X_NW2_MAX 128 /**  Maximum number of network type 2 personalization data sets. */

typedef union 
{
    QL_MCM_SIM_NW_PERSONAL_INFO_T               feature_gw_network_perso[QL_MCM_SIM_PERSO_NUM_NW_MAX];          /**< GW network personalization. */
    QL_MCM_SIM_NW_SUBSET_PERSONAL_INFO_T        feature_gw_network_subset_perso[QL_MCM_SIM_PERSO_NUM_NS_MAX];   /**< GW network subset personalization. */
    QL_MCM_SIM_NW_SP_PERSONAL_INFO_T            feature_gw_sp_perso[QL_MCM_SIM_PERSO_NUM_GW_SP_MAX];            /**< GW service provider personalization. */
    QL_MCM_SIM_GW_CORPORATE_PERSONAL_INFO_T     feature_gw_corporate_perso[QL_MCM_SIM_PERSO_NUM_GW_CP_MAX];     /**< GW corporate personalization. */
    QL_MCM_SIM_SIM_PERSONAL_INFO_T              feature_gw_sim_perso[QL_MCM_SIM_PERSO_NUM_SIM_MAX];             /**< GW SIM personalization. */
    QL_MCM_SIM_NW_PERSONAL_INFO_T               feature_1x_network1_perso[QL_MCM_SIM_PERSO_NUM_NW_MAX];         /**< 1X network type 1 personalization. */
    QL_MCM_SIM_1X_NW_TYPE2_PERSONAL_INFO_T      feature_1x_network2_perso[QL_MCM_SIM_PERSO_NUM_1X_NW2_MAX];     /**< 1X network type 3 personalization. */
    QL_MCM_SIM_SIM_PERSONAL_INFO_T              feature_1x_ruim_perso[QL_MCM_SIM_PERSO_NUM_SIM_MAX];            /*  1x RUIM Personalization */
}QL_MCM_SIM_PERSONAL_FEATURE_INFO_DATA_T;

/** Request message; Activates and sets the personalization data on the phone. Each feature can be activated 
    independently of one another; however, network data configurations must be consistent across activated personalization 
    modes in order to prevent contradicting featurization, and only one feature can be activated per message. 
    If personalization is already activated, it must first be deactivated before being reactivated with new data. */
typedef struct 
{
    uint32_t                                    ctrl_key_value_len;                 /**< Must be set to the number of elements in ctrl_key_value. */
    char                                        ctrl_key_value[MCM_SIM_CK_MAX_V01]; /**< Control key value. This value is a sequence of ASCII characters.*/
    E_QL_MCM_SIM_PERSO_FEATURE_TYPE_T           e_feature_type;
    uint32_t                                    feature_data_len; //Valid array count of t_feature_data
    QL_MCM_SIM_PERSONAL_FEATURE_INFO_DATA_T     t_feature_data;
}QL_MCM_SIM_PERSONALIZE_FEATURE_INFO_T;


typedef enum 
{
    E_QL_MCM_SIM_CARD_STATE_UNKNOWN                     = 0xB01,    /**< Card state unknown. */
    E_QL_MCM_SIM_CARD_STATE_ABSENT                      = 0xB02,    /**< Card is absent. */
    E_QL_MCM_SIM_CARD_STATE_PRESENT                     = 0xB03,    /**< Card is present. */
    E_QL_MCM_SIM_CARD_STATE_ERROR_UNKNOWN               = 0xB04,    /**< Unknown error state. */
    E_QL_MCM_SIM_CARD_STATE_ERROR_POWER_DOWN            = 0xB05,    /**< Power down. */
    E_QL_MCM_SIM_CARD_STATE_ERROR_POLL_ERROR            = 0xB06,    /**< Poll error. */
    E_QL_MCM_SIM_CARD_STATE_ERROR_NO_ATR_RECEIVED       = 0xB07,    /**<  Failed to receive an answer to reset.  */
    E_QL_MCM_SIM_CARD_STATE_ERROR_VOLT_MISMATCH         = 0xB08,    /**< Voltage mismatch. */
    E_QL_MCM_SIM_CARD_STATE_ERROR_PARITY_ERROR          = 0xB09,    /**< Parity error. */
    E_QL_MCM_SIM_CARD_STATE_ERROR_SIM_TECHNICAL_PROBLEMS= 0xB0A,    /**< Card returned technical problems. */
}E_QL_MCM_SIM_CARD_STATE_TYPE_T;  /**< Card state. */

typedef enum 
{
    E_QL_MCM_SIM_CARD_TYPE_UNKNOWN  = 0xB00,    /**<  Unidentified card type.  */
    E_QL_MCM_SIM_CARD_TYPE_ICC      = 0xB01,    /**<  Card of SIM or RUIM type.  */
    E_QL_MCM_SIM_CARD_TYPE_UICC     = 0xB02,    /**<  Card of USIM or CSIM type.  */
}E_QL_MCM_SIM_CARD_TYPE_T;

typedef enum 
{
    E_QL_MCM_SIM_PROV_STATE_NONE    = 0xB00,    /**<  Nonprovisioning.  */
    E_QL_MCM_SIM_PROV_STATE_PRI     = 0xB01,    /**<  Primary provisioning subscription.  */
    E_QL_MCM_SIM_PROV_STATE_SEC     = 0xB02,    /**<  Secondary provisioning subscription.  */
}QL_MCM_SIM_SUBSCRIPTION_TYPE_T;

typedef enum 
{
    E_QL_MCM_SIM_APP_STATE_UNKNOWN                  = 0xB00,    /**< Application state unknown. */
    E_QL_MCM_SIM_APP_STATE_DETECTED                 = 0xB01,    /**<  Detected state.  */
    E_QL_MCM_SIM_APP_STATE_PIN1_REQ                 = 0xB02,    /**<  PIN1 required.  */
    E_QL_MCM_SIM_APP_STATE_PUK1_REQ                 = 0xB03,    /**<  PUK1 required.  */
    E_QL_MCM_SIM_APP_STATE_INITALIZATING            = 0xB04,    /**<  Initializing.  */
    E_QL_MCM_SIM_APP_STATE_PERSO_CK_REQ             = 0xB05,    /**<  Personalization control key required.  */
    E_QL_MCM_SIM_APP_STATE_PERSO_PUK_REQ            = 0xB06,    /**<  Personalization unblock key required.  */
    E_QL_MCM_SIM_APP_STATE_PERSO_PERMANENTLY_BLOCKED= 0xB07,    /**<  Personalization is permanently blocked.  */
    E_QL_MCM_SIM_APP_STATE_PIN1_PERM_BLOCKED        = 0xB08,    /**<  PIN1 is permanently blocked.  */
    E_QL_MCM_SIM_APP_STATE_ILLEGAL                  = 0xB09,    /**<  Illegal application state.  */
    E_QL_MCM_SIM_APP_STATE_READY                    = 0xB0A,    /**<  Application ready state.  @newpage */
}QL_MCM_SIM_APP_STATE_TYPE_T;

typedef enum 
{
    E_QL_MCM_SIM_PIN_STATE_UNKNOWN                  = 0xB01,    /**< Unknown PIN state. */
    E_QL_MCM_SIM_PIN_STATE_ENABLED_NOT_VERIFIED     = 0xB02,    /**<  PIN required, but has not been verified.  */
    E_QL_MCM_SIM_PIN_STATE_ENABLED_VERIFIED         = 0xB03,    /**<  PIN required and has been verified.  */
    E_QL_MCM_SIM_PIN_STATE_DISABLED                 = 0xB04,    /**<  PIN not required.  */
    E_QL_MCM_SIM_PIN_STATE_BLOCKED                  = 0xB05,    /**<  PIN verification has failed too many times and is blocked. Recoverable through PUK verification.  */
    E_QL_MCM_SIM_PIN_STATE_PERMANENTLY_BLOCKED      = 0xB06,    /**<  PUK verification has failed too many times and is not recoverable.  */
}QL_MCM_SIM_PIN_STATE_TYPE_T;

typedef struct 
{
    QL_MCM_SIM_SUBSCRIPTION_TYPE_T      subscription;           /**<   Type of subscription (i.e., primary, secondary, etc.). */
    QL_MCM_SIM_APP_STATE_TYPE_T         app_state;              /**<   Current state of the application. */
    E_QL_MCM_SIM_PERSO_FEATURE_TYPE_T   perso_feature;          /**<   Current personalization state and feature enabled. */
    uint8_t                             perso_retries;          /**<   Number of personalization retries. */
    uint8_t                             perso_unblock_retries;  /**<   Number of personalization unblock retries. */
    QL_MCM_SIM_PIN_STATE_TYPE_T         pin1_state;             /**<   Current PIN 1 state. */
    uint8_t                             pin1_num_retries;       /**<   Number of PIN 1 retries. */
    uint8_t                             puk1_num_retries;       /**<   Number of PUK 1 retries. */
    QL_MCM_SIM_PIN_STATE_TYPE_T         pin2_state;             /**<   Current PIN 2 state. */
    uint8_t                             pin2_num_retries;       /**<   Number of PIN 2 retries. */
    uint8_t                             puk2_num_retries;       /**<   Number of PUK 2 retries. */
}QL_MCM_SIM_CARD_APP_INFO_T;

typedef struct 
{
    QL_MCM_SIM_CARD_APP_INFO_T          app_3gpp;               /**<   Stores 3GPP application information. */
    QL_MCM_SIM_CARD_APP_INFO_T          app_3gpp2;              /**<   Stores 3GPP2 application information. */
    QL_MCM_SIM_CARD_APP_INFO_T          app_isim;               /**<   Stores ISIM application information. */
}QL_MCM_SIM_CARD_ALL_APP_INFO_T;

typedef struct 
{
    E_QL_MCM_SIM_CARD_STATE_TYPE_T      e_card_state;/**<   Current card and card error state. */
    E_QL_MCM_SIM_CARD_TYPE_T            e_card_type; /**<   Card type. */
    QL_MCM_SIM_CARD_ALL_APP_INFO_T      card_app_info; /**<   Stores all relevant application information. */
}QL_MCM_SIM_CARD_STATUS_INFO_T;

#define QL_MCM_SIM_PATH_LEN_MAX     20  /**  Maximum length of a full file path in ASCII format. */
typedef struct 
{
    uint16_t    offset;                         /**< Offset is only required for write file access where data length is indicated.*/
    uint8_t     record_num;                     /**< Number of records involved in file access. A record number of 0 indicates transparent file access.*/
    uint32_t    path_len;                       /**< Must be set to the number of elements in the path. */
    char        path[QL_MCM_SIM_PATH_LEN_MAX];  /**< File path in ASCII characters. @newpagetable */
}QL_MCM_SIM_CARD_FILE_ACCESS_INFO_T;

#define QL_MCM_SIM_DATA_LEN_MAX     4096        /**  Maximum size of data to be read/written. */
typedef struct 
{
    QL_SIM_APP_ID_INFO_T                app_info;                       /**< Application identification information. */
    QL_MCM_SIM_CARD_FILE_ACCESS_INFO_T  file_access;                    /**< File access information. */
    uint32_t                            data_len;                       /**< Must be set to the number of elements in data. */
    uint8_t                             data[QL_MCM_SIM_DATA_LEN_MAX];  /**< Data to be updated on the card. */
}QL_MCM_SIM_CARD_WRITE_FILE_INFO_T;


/** Request message; Reads data to a specific file on a specified application on the card. The type of file is determined 
    by the record number field, which indicates a transparent file when zero and a record-based file otherwise.*/
typedef struct 
{
    QL_SIM_APP_ID_INFO_T                app_info;                       /**< Application identification information. */
    QL_MCM_SIM_CARD_FILE_ACCESS_INFO_T  file_access;                    /**< File access information. */
}QL_MCM_SIM_CARD_FILE_INFO_TO_READ_T;

typedef struct 
{
    uint32_t    data_len;                                               /**< Must be set to the number of elements in data. */
    uint8_t     data[QL_MCM_SIM_DATA_LEN_MAX];                          /**< Data retrieved from the card. */
}QL_MCM_SIM_CARD_FILE_DATA_T;

/** Retrieves the size of a specific file on a specified application on the card. */
typedef struct 
{
    QL_SIM_APP_ID_INFO_T    app_info;                       /**< Application identification information. */
    uint32_t                path_len;                       /**< Must be set to the number of elements in path. */
    char                    path[QL_MCM_SIM_PATH_LEN_MAX];  /**< File path in ASCII characters.*/
}QL_MCM_SIM_CARD_GET_FILE_SIZE_INPUT_INFO_T;

typedef enum 
{
    E_QL_MCM_SIM_FILE_TYPE_UNKNOWN      = 0xB00,/**<  Unknown file type  */
    E_QL_MCM_SIM_FILE_TYPE_TRANSPARENT  = 0xB01,/**< File structure consisting of a sequence of bytes.  */
    E_QL_MCM_SIM_FILE_TYPE_CYCLIC       = 0xB02,/**< File structure consisting of a sequence of records, each containing the same fixed size in 
                                                     chronological order. Once all the records have been used, the oldest data is overwritten.  */
    MCM_SIM_FILE_TYPE_LINEAR_FIXED  = 0xB03,    /**< File structure consisting of a sequence of records, each containing the same fixed size.  */
}E_QL_MCM_SIM_FILE_TYPE_T;

typedef struct 
{
    E_QL_MCM_SIM_FILE_TYPE_T    e_file_type;    /**<   File type: */
    uint16_t                    file_size;      /**<   Size of transparent files.*/
    uint16_t                    record_size;    /**<   Size of each cyclic or linear fixed file record.*/
    uint16_t                    record_count;   /**<   Number of cyclic or linear fixed file records.*/
}QL_MCM_SIM_FILE_SIZE_INFO_T; 

typedef enum 
{
    E_QL_MCM_SIM_REFRESH_EVENT              = 0xB00,    /**< Card refresh event  */
    E_QL_MCM_SIM_CARD_STATUS_UPDATE_EVENT   = 0xB01,    /**< Card status update event.  */
}E_QL_SIM_NFY_MSG_ID_T;


typedef void (*QL_SIM_CardStatusIndMsgHandlerFunc_t)   
(    
    sim_client_handle_type  h_sim,
    E_QL_SIM_NFY_MSG_ID_T   e_msg_id,
    void                    *pv_data,
    void                    *contextPtr    
);


int QL_MCM_SIM_Client_Init(sim_client_handle_type  *ph_sim);

int QL_MCM_SIM_Client_Deinit(sim_client_handle_type h_sim);

/* Add callback function if anything changed specified by the mask in QL_LOC_Set_Indications*/
int QL_MCM_SIM_AddRxIndMsgHandler(QL_SIM_CardStatusIndMsgHandlerFunc_t handlerPtr, void* contextPtr);


/*===========================================================================

  FUNCTION:  QL_MCM_SIM_GetIMSI

===========================================================================*/
/*
    @brief
    Function sends a command to the modem to retrieve the IMSI (for 3GPP)
    or IMSI_M (for 3GPP2) from the SIM in ASCII form

    @return
    void
*/
/*=========================================================================*/
E_QL_ERROR_CODE_T QL_MCM_SIM_GetIMSI
(
    sim_client_handle_type          h_sim,
    QL_SIM_APP_ID_INFO_T            *pt_info,   ///< [IN] The SIM identifier info.
    char                            *imsi,      ///< [OUT] IMSI buffer
    size_t                          imsiLen     ///< [IN] IMSI buffer length
);


/*===========================================================================

  FUNCTION:  QL_MCM_SIM_GetICCID

===========================================================================*/
/*
    @brief
    Function sends a command to the modem to retrieve the ICCID from
    SIM in ASCII form

    @return
    E_QL_ERROR_CODE_T
*/
/*=========================================================================*/
E_QL_ERROR_CODE_T QL_MCM_SIM_GetICCID
(
    sim_client_handle_type          h_sim,
    E_QL_MCM_SIM_SLOT_ID_TYPE_T     simId,     ///< [IN] The SIM identifier.
    char                            *iccid,    ///< [OUT] ICCID
    size_t                          iccidLen   ///< [IN] ICCID buffer length
);


/*===========================================================================

  FUNCTION:  QL_MCM_SIM_GetPhoneNumber

===========================================================================*/
/*
    @brief
    Function sends a command to the modem to retrieve the device phone
    number from MSISDN (for 3GPP) or MDN (for 3GPP2) from the SIM in
    ASCII form

    @return
    E_QL_ERROR_CODE_T
*/
/*=========================================================================*/
E_QL_ERROR_CODE_T QL_MCM_SIM_GetPhoneNumber
(
    sim_client_handle_type          h_sim,
    QL_SIM_APP_ID_INFO_T            *pt_info,   ///< [IN] The SIM identifier.
    char                            *phone_num, ///< [OUT] phone number
    size_t                          phoneLen    ///< [IN] phone number buffer length
);

/*===========================================================================

  FUNCTION:  QL_MCM_SIM_GetOperatorPlmnList

===========================================================================*/
/*
    @brief
    Function sends multiple record based read commands to the modem
    to retrieve a list of operator preffered PLMN.

    @note
    Function should only be called for SIM/USIM

    @return
    E_QL_ERROR_CODE_T
*/
/*=========================================================================*/
E_QL_ERROR_CODE_T QL_MCM_SIM_GetOperatorPlmnList
(
    sim_client_handle_type              h_sim,
    E_QL_MCM_SIM_SLOT_ID_TYPE_T         simId,      ///< [IN] The SIM identifier.
    QL_SIM_PREFERRED_OPERATOR_LIST_T    *pt_info    ///< [OUT] Preferred operator list
);

/*===========================================================================

  FUNCTION:  QL_MCM_SIM_VerifyPIN

===========================================================================*/
/*
    @brief
    Function sends a command to the modem to verify either PIN1 or PIN2

    @return
    E_QL_ERROR_CODE_T
*/
/*=========================================================================*/
E_QL_ERROR_CODE_T QL_MCM_SIM_VerifyPIN
(
    sim_client_handle_type      h_sim,
    QL_SIM_VERIFY_PIN_INFO_T    *pt_info   ///< [IN] Verify PIN infor
);


/*===========================================================================

  FUNCTION:  QL_MCM_SIM_ChangePin

===========================================================================*/
/*
    @brief
    Function sends a command to the modem to change the value of
    either PIN1 or PIN2

    @return
    E_QL_ERROR_CODE_T
*/
/*=========================================================================*/
E_QL_ERROR_CODE_T QL_MCM_SIM_ChangePin
(
    sim_client_handle_type      h_sim,
    QL_SIM_CHANGE_PIN_INFO_T    *pt_info   ///< [IN] Change PIN infor
);


/*===========================================================================

  FUNCTION:  QL_MCM_SIM_UnblockPIN

===========================================================================*/
/*
    @brief
    Function sends a command to the modem to unblock a PIN1 or PIN2 that
    has been blocked

    @return
    E_QL_ERROR_CODE_T
*/
/*=========================================================================*/
E_QL_ERROR_CODE_T QL_MCM_SIM_UnblockPIN
(
    sim_client_handle_type      h_sim,
    QL_SIM_UNBLOCK_PIN_INFO_T   *pt_info   ///< [IN] Unblock PIN infor
);

/*===========================================================================

  FUNCTION:  QL_MCM_SIM_EnablePIN

===========================================================================*/
/*
    @brief
    Function sends a command to the modem to enable PIN1 or PIN2

    @return
    E_QL_ERROR_CODE_T
*/
/*=========================================================================*/
E_QL_ERROR_CODE_T QL_MCM_SIM_EnablePIN
(
    sim_client_handle_type      h_sim,
    QL_SIM_ENABLE_PIN_INFO_T    *pt_info   ///< [IN] Enable PIN infor
);

/*===========================================================================

  FUNCTION:  QL_MCM_SIM_DisablePIN

===========================================================================*/
/*
    @brief
    Function sends a command to the modem to disable PIN1 or PIN2

    @return
    E_QL_ERROR_CODE_T
*/
/*=========================================================================*/
E_QL_ERROR_CODE_T QL_MCM_SIM_DisablePIN
(
    sim_client_handle_type      h_sim,
    QL_SIM_DISABLE_PIN_INFO_T   *pt_info   ///< [IN] Disable PIN infor
);


/*===========================================================================

  FUNCTION:  QL_MCM_SIM_GetCardStatus

===========================================================================*/
/*
    @brief
    Function retrieves the server cached card status informations and
    sends the information to the client

    @return
    E_QL_ERROR_CODE_T
*/
/*=========================================================================*/
E_QL_ERROR_CODE_T QL_MCM_SIM_GetCardStatus
(
    sim_client_handle_type          h_sim,
    E_QL_MCM_SIM_SLOT_ID_TYPE_T     simId,     ///< [IN] The SIM identifier.
    QL_MCM_SIM_CARD_STATUS_INFO_T   *pt_info   ///< [OUT] Cart status infor output
);

/*===========================================================================

  FUNCTION:  QL_MCM_SIM_Depersonalization

===========================================================================*/
/*
    @brief
    Function sends a command to the modem to disable or unblock SIM Lock

    @return
    E_QL_ERROR_CODE_T
*/
/*=========================================================================*/
E_QL_ERROR_CODE_T QL_MCM_SIM_Depersonalization
(
    sim_client_handle_type              h_sim,
    QL_MCM_SIM_DEPERSONALIZE_INFO_T     *pt_info   ///< [OUT] Depersonalize information
);


/*===========================================================================

  FUNCTION:  QL_MCM_SIM_Personalization

===========================================================================*/
/*
    @brief
    Function sends a command to the modem to set the SIM Lock data
    and concurrently enable SIM Lock.

    @return
    E_QL_ERROR_CODE_T
*/
/*=========================================================================*/
E_QL_ERROR_CODE_T QL_MCM_SIM_Personalization
(
    sim_client_handle_type                  h_sim,
    QL_MCM_SIM_PERSONALIZE_FEATURE_INFO_T   *pt_info   ///< [OUT] Unblock PIN infor
);


/*===========================================================================

  FUNCTION:  QL_MCM_SIM_WriteFile

===========================================================================*/
/*
    @brief
    Function will determine whether the request is for transparent or
    record based write command based on the record number and calls the
    corresponding function to send the message to the card.

    record number = 0  -  transparent
    record number > 0  -  record based

    @return
    E_QL_ERROR_CODE_T
*/
/*=========================================================================*/
E_QL_ERROR_CODE_T QL_MCM_SIM_WriteFile
(
    sim_client_handle_type              h_sim,
    QL_MCM_SIM_CARD_WRITE_FILE_INFO_T   *pt_info   ///< [OUT] Verify PIN infor
);


/*===========================================================================

  FUNCTION:  QL_MCM_SIM_ReadFile

===========================================================================*/
/*
    @brief
    Function will determine whether the request is for transparent or
    record based read command based on the record number and calls the
    corresponding function to send the message to the card.

    record number = 0  -  transparent
    record number > 0  -  record based

    @return
    E_QL_ERROR_CODE_T
*/
/*=========================================================================*/
E_QL_ERROR_CODE_T QL_MCM_SIM_ReadFile
(
    sim_client_handle_type                  h_sim,
    QL_MCM_SIM_CARD_FILE_INFO_TO_READ_T     *pt_info,   ///< [IN] input file info which to be read
    QL_MCM_SIM_CARD_FILE_DATA_T             *pt_out     ///< [OUT] File data of the specified one by pt_info
);

/*===========================================================================

  FUNCTION:  QL_MCM_SIM_GetFileSize

===========================================================================*/
/*
    @brief
    Function sends a command to the modem to retrieve the file type, file
    size(transparent files) or record size and record count (record based files)

    @return
    E_QL_ERROR_CODE_T
*/
/*=========================================================================*/
E_QL_ERROR_CODE_T QL_MCM_SIM_GetFileSize
(
    sim_client_handle_type                          h_sim,
    QL_MCM_SIM_CARD_GET_FILE_SIZE_INPUT_INFO_T      *pt_info,   ///< [IN] input file info which to be read
    QL_MCM_SIM_FILE_SIZE_INFO_T                     *pt_out     ///< [OUT] output file size info
);



#endif // __QL_MCM_SIM_H__

