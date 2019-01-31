#ifndef MCM_SIM_SERVICE_01_H
#define MCM_SIM_SERVICE_01_H

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
  Copyright (c) 2013 Qualcomm Technologies, Inc.
  All rights reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
 *====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/



#include <stdint.h>
#include "mcm_common_v01.h"


#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup mcm_sim_consts
    @{
  */

/**  Maximum length of IMSI data. */
#define MCM_SIM_IMSI_LEN_V01 16

/**  Maximum length of ICCID data. */
#define MCM_SIM_ICCID_LEN_V01 20

/**  Maximum number of PLMN data sets. */
#define MCM_SIM_NUM_PLMN_MAX_V01 24

/**  Maximum length of a full file path in ASCII format. */
#define MCM_SIM_CHAR_PATH_MAX_V01 20

/**  Maximum size of data to be read/written. */
#define MCM_SIM_DATA_MAX_V01 4096

/**  Maximum length of PIN data. */
#define MCM_SIM_PIN_MAX_V01 8

/**  Maximum number of cards. */
#define MCM_SIM_MAX_NUM_CARDS_V01 2

/**  Maximum number of refresh files. */
#define MCM_SIM_MAX_REFRESH_FILES_V01 35

/**  Maximum length of a full file path in binary format. */
#define MCM_SIM_MAX_BINARY_PATHS_V01 10

/**  Maximum length of personalization control key data */
#define MCM_SIM_CK_MAX_V01 16

/**  Length of the MCC. */
#define MCM_SIM_MCC_LEN_V01 3

/**  Maximum length of the MNC. */
#define MCM_SIM_MNC_MAX_V01 3

/**  Maximum phone number length. */
#define MCM_SIM_PHONE_NUMBER_MAX_V01 82

/**  Length of the IRM code. */
#define MCM_SIM_IRM_CODE_LEN_V01 4

/**  Maximum length of the MSIN. */
#define MCM_SIM_MSIN_MAX_V01 10

/**  Maximum number of network personalization data sets. */
#define MCM_SIM_PERSO_NUM_NW_MAX_V01 85

/**  Maximum number of network subset personalization data sets. */
#define MCM_SIM_PERSO_NUM_NS_MAX_V01 64

/**  Maximum number of service provider personalization data sets. */
#define MCM_SIM_PERSO_NUM_GW_SP_MAX_V01 64

/**  Maximum number of corporate personalization data sets. */
#define MCM_SIM_PERSO_NUM_GW_CP_MAX_V01 51

/**  Maximum number of SIM personalization data sets. */
#define MCM_SIM_PERSO_NUM_SIM_MAX_V01 32

/**  Maximum number of network type 2 personalization data sets. */
#define MCM_SIM_PERSO_NUM_1X_NW2_MAX_V01 128
/**
    @}
  */

/** @addtogroup mcm_sim_enums
    @{
  */
typedef enum {
  MCM_SIM_SLOT_ID_T_MIN_ENUM_VAL_V01 = -2147483647, /* To force a 32 bit signed enum.  Do not change or use*/
  MCM_SIM_SLOT_ID_1_V01 = 0xB01, /**< Identify card in slot 1.  */
  MCM_SIM_SLOT_ID_2_V01 = 0xB02, /**< Identify card in slot 2.  */
  MCM_SIM_SLOT_ID_T_MAX_ENUM_VAL_V01 = 2147483647 /* To force a 32 bit signed enum.  Do not change or use*/
}mcm_sim_slot_id_t_v01;
/**
    @}
  */

/** @addtogroup mcm_sim_enums
    @{
  */
typedef enum {
  MCM_SIM_APP_TYPE_T_MIN_ENUM_VAL_V01 = -2147483647, /* To force a 32 bit signed enum.  Do not change or use*/
  MCM_SIM_APP_TYPE_UNKNOWN_V01 = 0xB00, /**<  Unknown application type  */
  MCM_SIM_APP_TYPE_3GPP_V01 = 0xB01, /**< Identify the SIM/USIM application on the card.  */
  MCM_SIM_APP_TYPE_3GPP2_V01 = 0xB02, /**< Identify the RUIM/CSIM application on the card.  */
  MCM_SIM_APP_TYPE_ISIM_V01 = 0xB03, /**< Identify the ISIM application on the card.  */
  MCM_SIM_APP_TYPE_T_MAX_ENUM_VAL_V01 = 2147483647 /* To force a 32 bit signed enum.  Do not change or use*/
}mcm_sim_app_type_t_v01;
/**
    @}
  */

/** @addtogroup mcm_sim_enums
    @{
  */
typedef enum {
  MCM_SIM_FILE_TYPE_T_MIN_ENUM_VAL_V01 = -2147483647, /* To force a 32 bit signed enum.  Do not change or use*/
  MCM_SIM_FILE_TYPE_UNKNOWN_V01 = 0xB00, /**<  Unknown file type  */
  MCM_SIM_FILE_TYPE_TRANSPARENT_V01 = 0xB01, /**< File structure consisting of a sequence of bytes.  */
  MCM_SIM_FILE_TYPE_CYCLIC_V01 = 0xB02, /**< File structure consisting of a sequence of records, each
       containing the same fixed size in chronological order.
       Once all the records have been used, the oldest data is
       overwritten.  */
  MCM_SIM_FILE_TYPE_LINEAR_FIXED_V01 = 0xB03, /**< File structure consisting of a sequence of records, each
       containing the same fixed size.  */
  MCM_SIM_FILE_TYPE_T_MAX_ENUM_VAL_V01 = 2147483647 /* To force a 32 bit signed enum.  Do not change or use*/
}mcm_sim_file_type_t_v01;
/**
    @}
  */

/** @addtogroup mcm_sim_enums
    @{
  */
typedef enum {
  MCM_SIM_PIN_ID_T_MIN_ENUM_VAL_V01 = -2147483647, /* To force a 32 bit signed enum.  Do not change or use*/
  MCM_SIM_PIN_ID_1_V01 = 0xB01, /**< Level 1 user verification.  */
  MCM_SIM_PIN_ID_2_V01 = 0xB02, /**< Level 2 user verification.  */
  MCM_SIM_PIN_ID_T_MAX_ENUM_VAL_V01 = 2147483647 /* To force a 32 bit signed enum.  Do not change or use*/
}mcm_sim_pin_id_t_v01;
/**
    @}
  */

/** @addtogroup mcm_sim_enums
    @{
  */
typedef enum {
  MCM_SIM_PERSO_FEATURE_T_MIN_ENUM_VAL_V01 = -2147483647, /* To force a 32 bit signed enum.  Do not change or use*/
  MCM_SIM_PERSO_FEATURE_STATUS_UNKNOWN_V01 = 0xB00, /**<  Unknown personalization feature.  */
  MCM_SIM_PERSO_FEATURE_STATUS_3GPP_NETWORK_V01 = 0xB01, /**<  Featurization based on 3GPP MCC and MNC.  */
  MCM_SIM_PERSO_FEATURE_STATUS_3GPP_NETWORK_SUBSET_V01 = 0xB02, /**<  Featurization based on 3GPP MCC, MNC, and IMSI digits 6 and 7.  */
  MCM_SIM_PERSO_FEATURE_STATUS_3GPP_SERVICE_PROVIDER_V01 = 0xB03, /**<  Featurization based on 3GPP MCC, MNC, and GID1.  */
  MCM_SIM_PERSO_FEATURE_STATUS_3GPP_CORPORATE_V01 = 0xB04, /**<  Featurization based on 3GPP MCC, MNC, GID1, and GID2.  */
  MCM_SIM_PERSO_FEATURE_STATUS_3GPP_SIM_V01 = 0xB05, /**<  Featurization based on the 3GPP IMSI.  */
  MCM_SIM_PERSO_FEATURE_STATUS_3GPP2_NETWORK_TYPE_1_V01 = 0xB06, /**<  Featurization based on 3GPP2 MCC and MNC.  */
  MCM_SIM_PERSO_FEATURE_STATUS_3GPP2_NETWORK_TYPE_2_V01 = 0xB07, /**<  Featurization based on 3GPP2 IRM code.  */
  MCM_SIM_PERSO_FEATURE_STATUS_3GPP2_RUIM_V01 = 0xB08, /**<  Featurization based on 3GPP2 IMSI_M.  */
  MCM_SIM_PERSO_FEATURE_T_MAX_ENUM_VAL_V01 = 2147483647 /* To force a 32 bit signed enum.  Do not change or use*/
}mcm_sim_perso_feature_t_v01;
/**
    @}
  */

/** @addtogroup mcm_sim_enums
    @{
  */
typedef enum {
  MCM_SIM_PERSO_OPERATION_T_MIN_ENUM_VAL_V01 = -2147483647, /* To force a 32 bit signed enum.  Do not change or use*/
  MCM_SIM_PERSO_OPERATION_DEACTIVATE_V01 = 0xB00, /**<  Disable an active personalization feature.  */
  MCM_SIM_PERSO_OPERATION_UNBLOCK_V01 = 0xB01, /**<  Unblock a personalization feature that has been blocked.  */
  MCM_SIM_PERSO_OPERATION_T_MAX_ENUM_VAL_V01 = 2147483647 /* To force a 32 bit signed enum.  Do not change or use*/
}mcm_sim_perso_operation_t_v01;
/**
    @}
  */

/** @addtogroup mcm_sim_enums
    @{
  */
typedef enum {
  MCM_SIM_CARD_T_MIN_ENUM_VAL_V01 = -2147483647, /* To force a 32 bit signed enum.  Do not change or use*/
  MCM_SIM_CARD_TYPE_UNKNOWN_V01 = 0xB00, /**<  Unidentified card type.  */
  MCM_SIM_CARD_TYPE_ICC_V01 = 0xB01, /**<  Card of SIM or RUIM type.  */
  MCM_SIM_CARD_TYPE_UICC_V01 = 0xB02, /**<  Card of USIM or CSIM type.  */
  MCM_SIM_CARD_T_MAX_ENUM_VAL_V01 = 2147483647 /* To force a 32 bit signed enum.  Do not change or use*/
}mcm_sim_card_t_v01;
/**
    @}
  */

/** @addtogroup mcm_sim_enums
    @{
  */
typedef enum {
  MCM_SIM_SUBSCRIPTION_T_MIN_ENUM_VAL_V01 = -2147483647, /* To force a 32 bit signed enum.  Do not change or use*/
  MCM_SIM_PROV_STATE_NONE_V01 = 0xB00, /**<  Nonprovisioning.  */
  MCM_SIM_PROV_STATE_PRI_V01 = 0xB01, /**<  Primary provisioning subscription.  */
  MCM_SIM_PROV_STATE_SEC_V01 = 0xB02, /**<  Secondary provisioning subscription.  */
  MCM_SIM_SUBSCRIPTION_T_MAX_ENUM_VAL_V01 = 2147483647 /* To force a 32 bit signed enum.  Do not change or use*/
}mcm_sim_subscription_t_v01;
/**
    @}
  */

/** @addtogroup mcm_sim_enums
    @{
  */
typedef enum {
  MCM_SIM_APP_STATE_T_MIN_ENUM_VAL_V01 = -2147483647, /* To force a 32 bit signed enum.  Do not change or use*/
  MCM_SIM_APP_STATE_UNKNOWN_V01 = 0xB00, /**< Application state unknown. */
  MCM_SIM_APP_STATE_DETECTED_V01 = 0xB01, /**<  Detected state.  */
  MCM_SIM_APP_STATE_PIN1_REQ_V01 = 0xB02, /**<  PIN1 required.  */
  MCM_SIM_APP_STATE_PUK1_REQ_V01 = 0xB03, /**<  PUK1 required.  */
  MCM_SIM_APP_STATE_INITALIZATING_V01 = 0xB04, /**<  Initializing.  */
  MCM_SIM_APP_STATE_PERSO_CK_REQ_V01 = 0xB05, /**<  Personalization control key required.  */
  MCM_SIM_APP_STATE_PERSO_PUK_REQ_V01 = 0xB06, /**<  Personalization unblock key required.  */
  MCM_SIM_APP_STATE_PERSO_PERMANENTLY_BLOCKED_V01 = 0xB07, /**<  Personalization is permanently blocked.  */
  MCM_SIM_APP_STATE_PIN1_PERM_BLOCKED_V01 = 0xB08, /**<  PIN1 is permanently blocked.  */
  MCM_SIM_APP_STATE_ILLEGAL_V01 = 0xB09, /**<  Illegal application state.  */
  MCM_SIM_APP_STATE_READY_V01 = 0xB0A, /**<  Application ready state.  @newpage */
  MCM_SIM_APP_STATE_T_MAX_ENUM_VAL_V01 = 2147483647 /* To force a 32 bit signed enum.  Do not change or use*/
}mcm_sim_app_state_t_v01;
/**
    @}
  */

/** @addtogroup mcm_sim_enums
    @{
  */
typedef enum {
  MCM_SIM_PIN_STATE_T_MIN_ENUM_VAL_V01 = -2147483647, /* To force a 32 bit signed enum.  Do not change or use*/
  MCM_SIM_PIN_STATE_UNKNOWN_V01 = 0xB01,   /**< Unknown PIN state. */
  MCM_SIM_PIN_STATE_ENABLED_NOT_VERIFIED_V01 = 0xB02, /**<  PIN required, but has not been verified.  */
  MCM_SIM_PIN_STATE_ENABLED_VERIFIED_V01 = 0xB03, /**<  PIN required and has been verified.  */
  MCM_SIM_PIN_STATE_DISABLED_V01 = 0xB04, /**<  PIN not required.  */
  MCM_SIM_PIN_STATE_BLOCKED_V01 = 0xB05, /**<  PIN verification has failed too many times and is blocked. Recoverable through PUK verification.  */
  MCM_SIM_PIN_STATE_PERMANENTLY_BLOCKED_V01 = 0xB06, /**<  PUK verification has failed too many times and is not recoverable.  */
  MCM_SIM_PIN_STATE_T_MAX_ENUM_VAL_V01 = 2147483647 /* To force a 32 bit signed enum.  Do not change or use*/
}mcm_sim_pin_state_t_v01;
/**
    @}
  */

/** @addtogroup mcm_sim_enums
    @{
  */
typedef enum {
  MCM_SIM_CARD_STATE_T_MIN_ENUM_VAL_V01 = -2147483647, /* To force a 32 bit signed enum.  Do not change or use*/
  MCM_SIM_CARD_STATE_UNKNOWN_V01 = 0xB01,  /**< Card state unknown. */
  MCM_SIM_CARD_STATE_ABSENT_V01 = 0xB02,   /**< Card is absent. */
  MCM_SIM_CARD_STATE_PRESENT_V01 = 0xB03,  /**< Card is present. */
  MCM_SIM_CARD_STATE_ERROR_UNKNOWN_V01 = 0xB04,     /**< Unknown error state. */
  MCM_SIM_CARD_STATE_ERROR_POWER_DOWN_V01 = 0xB05,  /**< Power down. */
  MCM_SIM_CARD_STATE_ERROR_POLL_ERROR_V01 = 0xB06,  /**< Poll error. */
  MCM_SIM_CARD_STATE_ERROR_NO_ATR_RECEIVED_V01 = 0xB07, /**<  Failed to receive an answer to reset.  */
  MCM_SIM_CARD_STATE_ERROR_VOLT_MISMATCH_V01 = 0xB08,   /**< Voltage mismatch. */
  MCM_SIM_CARD_STATE_ERROR_PARITY_ERROR_V01 = 0xB09,    /**< Parity error. */
  MCM_SIM_CARD_STATE_ERROR_SIM_TECHNICAL_PROBLEMS_V01 = 0xB0A,  /**< Card returned technical problems. */
  MCM_SIM_CARD_STATE_T_MAX_ENUM_VAL_V01 = 2147483647 /* To force a 32 bit signed enum.  Do not change or use*/
}mcm_sim_card_state_t_v01;  /**< Card state. */
/**
    @}
  */

/** @addtogroup mcm_sim_enums
    @{
  */
typedef enum {
  MCM_SIM_REFRESH_MODE_T_MIN_ENUM_VAL_V01 = -2147483647, /* To force a 32 bit signed enum.  Do not change or use*/
  MCM_SIM_REFRESH_RESET_V01 = 0xB01,     /**< Refresh reset. */
  MCM_SIM_REFRESH_NAA_INIT_V01 = 0xB02,  /**< Refresh NAA initialization. */
  MCM_SIM_REFRESH_NAA_FCN_V01 = 0xB03,   /**< Refresh NAA file change notification. */
  MCM_SIM_REFRESH_NAA_INIT_FCN_V01 = 0xB04,  /**< Refresh NAA initalization and file change notification. */
  MCM_SIM_REFRESH_NAA_INIT_FULL_FCN_V01 = 0xB05, /**< Refresh NAA initalization and full file change notification. */
  MCM_SIM_REFRESH_NAA_APP_RESET_V01 = 0xB06,     /**< Refresh NAA application reset. */
  MCM_SIM_REFRESH_3G_SESSION_RESET_V01 = 0xB07,  /**< Refresh 3G session reset. */
  MCM_SIM_REFRESH_MODE_T_MAX_ENUM_VAL_V01 = 2147483647 /* To force a 32 bit signed enum.  Do not change or use*/
}mcm_sim_refresh_mode_t_v01;  /**< SIM refresh modes. */
/**
    @}
  */

/** @addtogroup mcm_sim_aggregates
    @{
  */
typedef struct {

  mcm_sim_slot_id_t_v01 slot_id;
  /**< Indicates the slot to be used. Valid values: \n
        - 1 -- Slot 1
        - 2 -- Slot 2 @tablebulletend
  */

  /*  Application Type */
  mcm_sim_app_type_t_v01 app_t;
  /**< Indicates the type of the application. Valid values: \n
        - 0 -- Unknown
        - 1 -- 3GPP application
        - 2 -- 3GPP2 application
        - 3 -- ISIM application

       Other values are reserved for the future and are to be handled as
       Unknown.
  */
}mcm_sim_application_identification_info_t_v01;  /* Type */
/**
    @}
  */

/** @addtogroup mcm_sim_aggregates
    @{
  */
typedef struct {

  char mcc[MCM_SIM_MCC_LEN_V01];
  /**< MCC value in ASCII characters.*/

  uint32_t mnc_len;  /**< Must be set to the number of elements in the MNC. */
  char mnc[MCM_SIM_MNC_MAX_V01];
  /**< MNC value in ASCII characters.*/
}mcm_sim_plmn_t_v01;  /* Type */
/**
    @}
  */

/** @addtogroup mcm_sim_aggregates
    @{
  */
typedef struct {

  uint16_t offset;
  /**< Offset is only required for write file access where data length is indicated.*/

  uint8_t record_num;
  /**< Number of records involved in file access. A record number of 0 indicates transparent file access.*/

  uint32_t path_len;  /**< Must be set to the number of elements in the path. */
  char path[MCM_SIM_CHAR_PATH_MAX_V01];
  /**< File path in ASCII characters. @newpagetable */
}mcm_sim_file_access_t_v01;  /* Type */
/**
    @}
  */

/** @addtogroup mcm_sim_aggregates
    @{
  */
typedef struct {

  uint8_t sw1;
  /**< SW1 received from the card.*/

  uint8_t sw2;
  /**< SW2 received from the card.*/
}mcm_sim_card_result_t_v01;  /* Type */
/**
    @}
  */

/** @addtogroup mcm_sim_aggregates
    @{
  */
typedef struct {

  mcm_sim_file_type_t_v01 file_t;
  /**<   File type: \n
         - 0xB00 -- Unknown
         - 0xB01 -- Transparent
         - 0xB02 -- Cyclic
         - 0xB03 -- Linear fixed @tablebulletend
  */

  uint16_t file_size;
  /**<   Size of transparent files.*/

  uint16_t record_size;
  /**<   Size of each cyclic or linear fixed file record.*/

  uint16_t record_count;
  /**<   Number of cyclic or linear fixed file records.*/
}mcm_sim_file_info_t_v01;  /* Type */
/**
    @}
  */

/** @addtogroup mcm_sim_aggregates
    @{
  */
typedef struct {

  mcm_sim_perso_feature_t_v01 feature;
  /**< Indicates the personalization feature to deactivate or unblock.
       Valid values: \n
         - 0 -- GW network personalization
         - 1 -- GW network subset personalization
         - 2 -- GW service provider personalization
         - 3 -- GW corporate personalization
         - 4 -- GW UIM personalization
         - 5 -- 1X network type 1 personalization
         - 6 -- 1X network type 2 personalization
         - 7 -- 1X HRPD personalization
         - 8 -- 1X service provider personalization
         - 9 -- 1X corporate personalization
         - 10 -- 1X RUIM personalization @tablebulletend
  */

  mcm_sim_perso_operation_t_v01 operation;
  /**< Indicates the operation to perform. Valid values: \n
        - 0 -- Deactivate personalization
        - 1 -- Unblock personalization @tablebulletend
  */

  uint32_t ck_value_len;  /**< Must be set to the number of elements in ck_value. */
  char ck_value[MCM_SIM_CK_MAX_V01];
  /**< Control key value. This value is a sequence of ASCII characters.*/
}mcm_sim_depersonalization_t_v01;  /* Type */
/**
    @}
  */

/** @addtogroup mcm_sim_aggregates
    @{
  */
typedef struct {

  char mcc[MCM_SIM_MCC_LEN_V01];
  /**< MCC value in ASCII characters.*/

  uint32_t mnc_len;  /**< Must be set to the number of elements in the MNC. */
  char mnc[MCM_SIM_MNC_MAX_V01];
  /**< MNC value in ASCII characters.*/
}mcm_sim_network_perso_t_v01;  /* Type */
/**
    @}
  */

/** @addtogroup mcm_sim_aggregates
    @{
  */
typedef struct {

  mcm_sim_network_perso_t_v01 network;
  /**< MCC and MNC network information.*/

  char digit6;
  /**< Digit 6 of the IMSI in ASCII character.*/

  char digit7;
  /**< Digit 7 of the IMSI in ASCII character.*/
}mcm_sim_gw_network_subset_perso_t_v01;  /* Type */
/**
    @}
  */

/** @addtogroup mcm_sim_aggregates
    @{
  */
typedef struct {

  mcm_sim_network_perso_t_v01 network;
  /**< MCC and MNC network information.*/

  uint8_t gid1;
  /**< Service provider code found in GID1.*/
}mcm_sim_gw_sp_perso_t_v01;  /* Type */
/**
    @}
  */

/** @addtogroup mcm_sim_aggregates
    @{
  */
typedef struct {

  mcm_sim_network_perso_t_v01 network;
  /**< MCC and MNC network information.*/

  uint8_t gid1;
  /**< Service provider code found in GID1.*/

  uint8_t gid2;
  /**< Corporate customer code found in GID2. @newpagetable */
}mcm_sim_gw_corporate_perso_t_v01;  /* Type */
/**
    @}
  */

/** @addtogroup mcm_sim_aggregates
    @{
  */
typedef struct {

  mcm_sim_network_perso_t_v01 network;
  /**< MCC and MNC network information.*/

  uint32_t msin_len;  /**< Must be set to the number of elements in MSIN. */
  char msin[MCM_SIM_MSIN_MAX_V01];
  /**< MSIN value stored on the card in ASCII characters.*/
}mcm_sim_sim_perso_t_v01;  /* Type */
/**
    @}
  */

/** @addtogroup mcm_sim_aggregates
    @{
  */
typedef struct {

  char irm_code[MCM_SIM_IRM_CODE_LEN_V01];
  /**< First 4 digits of the IRM-based MIN of IMSI_M in ASCII characters.*/
}mcm_sim_1x_network_type2_perso_t_v01;  /* Type */
/**
    @}
  */

/** @addtogroup mcm_sim_aggregates
    @{
  */
typedef struct {

  uint8_t verify_left;
  /**< Number of the remaining attempts to verify the personalization.*/

  uint8_t unblock_left;
  /**< Number of the remaining attempts to unblock the personalization.*/
}mcm_sim_perso_retries_left_t_v01;  /* Type */
/**
    @}
  */

/** @addtogroup mcm_sim_aggregates
    @{
  */
typedef struct {

  mcm_sim_subscription_t_v01 subscription;
  /**<   Type of subscription (i.e., primary, secondary, etc.). */

  mcm_sim_app_state_t_v01 app_state;
  /**<   Current state of the application. */

  mcm_sim_perso_feature_t_v01 perso_feature;
  /**<   Current personalization state and feature enabled. */

  uint8_t perso_retries;
  /**<   Number of personalization retries. */

  uint8_t perso_unblock_retries;
  /**<   Number of personalization unblock retries. */

  mcm_sim_pin_state_t_v01 pin1_state;
  /**<   Current PIN 1 state. */

  uint8_t pin1_num_retries;
  /**<   Number of PIN 1 retries. */

  uint8_t puk1_num_retries;
  /**<   Number of PUK 1 retries. */

  mcm_sim_pin_state_t_v01 pin2_state;
  /**<   Current PIN 2 state. */

  uint8_t pin2_num_retries;
  /**<   Number of PIN 2 retries. */

  uint8_t puk2_num_retries;
  /**<   Number of PUK 2 retries. */
}mcm_sim_app_info_t_v01;  /* Type */
/**
    @}
  */

/** @addtogroup mcm_sim_aggregates
    @{
  */
typedef struct {

  mcm_sim_app_info_t_v01 app_3gpp;
  /**<   Stores 3GPP application information. */

  mcm_sim_app_info_t_v01 app_3gpp2;
  /**<   Stores 3GPP2 application information. */

  mcm_sim_app_info_t_v01 app_isim;
  /**<   Stores ISIM application information. */
}mcm_sim_card_app_info_t_v01;  /* Type */
/**
    @}
  */

/** @addtogroup mcm_sim_aggregates
    @{
  */
typedef struct {

  mcm_sim_card_state_t_v01 card_state;
  /**<   Current card and card error state. */

  mcm_sim_card_t_v01 card_t;
  /**<   Card type. */

  mcm_sim_card_app_info_t_v01 card_app_info;
  /**<   Stores all relevant application information. */
}mcm_sim_card_info_t_v01;  /* Type */
/**
    @}
  */

/** @addtogroup mcm_sim_aggregates
    @{
  */
typedef struct {

  uint32_t path_value_len;  /**< Must be set to the number of elements in path_value. */
  char path_value[MCM_SIM_CHAR_PATH_MAX_V01];
  /**< Path value. */
}mcm_sim_refresh_file_list_t_v01;  /* Type */
/**
    @}
  */

/** @addtogroup mcm_sim_aggregates
    @{
  */
typedef struct {

  mcm_sim_application_identification_info_t_v01 app_info;
  /**< Application identification information. */

  /*  Refresh Mode */
  mcm_sim_refresh_mode_t_v01 refresh_mode;
  /**< Refresh mode. */

  /*  Refresh File Data */
  uint32_t refresh_files_len;  /**< Must be set to the number of elements in refresh_files. */
  mcm_sim_refresh_file_list_t_v01 refresh_files[MCM_SIM_MAX_REFRESH_FILES_V01];
  /**< Refresh file data. */
}mcm_sim_refresh_event_t_v01;  /* Type */
/**
    @}
  */

/** @addtogroup mcm_sim_messages
    @{
  */
/** Request message; Retrieves the International Mobile Subscriber Identity (IMSI) value
           stored in the specified application. */
typedef struct {

  /* Mandatory */
  /*  Application Identification */
  mcm_sim_application_identification_info_t_v01 app_info;
  /**< Application identification information. */
}mcm_sim_get_subscriber_id_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_sim_messages
    @{
  */
/** Response message; Retrieves the International Mobile Subscriber Identity (IMSI) value
           stored in the specified application. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;
  /**< Standard response type. Contains the following data members: \n
       - mcm_result_t -- MCM_RESULT_SUCCESS or MCM_RESULT_FAILURE
       - mcm_error_t  -- Error code. Possible error code values are described
                          in the error codes section of each message
                          definition. @tablebulletend
  */

  /* Optional */
  /*  IMSI Data */
  uint8_t imsi_valid;  /**< Must be set to TRUE if imsi is being passed. */
  uint32_t imsi_len;  /**< Must be set to the number of elements in imsi. */
  char imsi[MCM_SIM_IMSI_LEN_V01];
  /**< IMSI data in ASCII characters.*/
}mcm_sim_get_subscriber_id_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_sim_messages
    @{
  */
/** Request message; Retrieves the Integrated Circuit Card ID (ICCID) stored on the card. */
typedef struct {

  /* Mandatory */
  /*  Slot ID */
  mcm_sim_slot_id_t_v01 slot_id;
  /**< Indicates the slot to be used. Valid values: \n
        - 1 -- Slot 1
        - 2 -- Slot 2 @tablebulletend
  */
}mcm_sim_get_card_id_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_sim_messages
    @{
  */
/** Response message; Retrieves the Integrated Circuit Card ID (ICCID) stored on the card. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;
  /**< Standard response type. Contains the following data members: \n
       - mcm_result_t -- MCM_RESULT_SUCCESS or MCM_RESULT_FAILURE
       - mcm_error_t  -- Error code. Possible error code values are described
                          in the error codes section of each message
                          definition. @tablebulletend
  */

  /* Optional */
  /*  ICCID data */
  uint8_t iccid_valid;  /**< Must be set to TRUE if iccid is being passed. */
  uint32_t iccid_len;  /**< Must be set to the number of elements in iccid. */
  char iccid[MCM_SIM_ICCID_LEN_V01];
  /**< ICCID data in ASCII characters.*/
}mcm_sim_get_card_id_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_sim_messages
    @{
  */
/** Request message; Retrieves the device phone number stored on the card. */
typedef struct {

  /* Mandatory */
  /*  Application Identification */
  mcm_sim_application_identification_info_t_v01 app_info;
  /**< Application identification information. */
}mcm_sim_get_device_phone_number_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_sim_messages
    @{
  */
/** Response message; Retrieves the device phone number stored on the card. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;
  /**< Standard response type. Contains the following data members: \n
       - mcm_result_t -- MCM_RESULT_SUCCESS or MCM_RESULT_FAILURE
       - mcm_error_t  -- Error code. Possible error code values are described
                          in the error codes section of each message
                          definition. @tablebulletend
  */

  /* Optional */
  /*  Device Phone Number */
  uint8_t phone_number_valid;  /**< Must be set to TRUE if phone_number is being passed. */
  uint32_t phone_number_len;  /**< Must be set to the number of elements in phone_number. */
  char phone_number[MCM_SIM_PHONE_NUMBER_MAX_V01];
  /**< Parsed phone number in ASCII characters.*/
}mcm_sim_get_device_phone_number_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_sim_messages
    @{
  */
/** Request message; Retrieves the preferred operator list stored on the card.

    @latexonly \hangindent1.085cm {\sf\small\bf Note: \bf} @endlatexonly
        This command is only supported by 3GPP applications. */
typedef struct {

  /* Mandatory */
  /*  Slot ID */
  mcm_sim_slot_id_t_v01 slot_id;
  /**< Indicates the slot to be used. Valid values: \n
        - 1 -- Slot 1
        - 2 -- Slot 2 @tablebulletend
  */
}mcm_sim_get_preferred_operator_list_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_sim_messages
    @{
  */
/** Response message; Retrieves the preferred operator list stored on the card.

    @latexonly \hangindent1.085cm {\sf\small\bf Note: \bf} @endlatexonly 
        This command is only supported by 3GPP applications. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;
  /**< Standard response type. Contains the following data members: \n
       - mcm_result_t -- MCM_RESULT_SUCCESS or MCM_RESULT_FAILURE
       - mcm_error_t  -- Error code. Possible error code values are described
                          in the error codes section of each message
                          definition. @tablebulletend
  */

  /* Optional */
  /*  Preferred Operator List */
  uint8_t preferred_operator_list_valid;  /**< Must be set to TRUE if preferred_operator_list is being passed. */
  uint32_t preferred_operator_list_len;  /**< Must be set to the number of elements in preferred_operator_list. */
  mcm_sim_plmn_t_v01 preferred_operator_list[MCM_SIM_NUM_PLMN_MAX_V01];
  /**< Preferred operator list. */
}mcm_sim_get_preferred_operator_list_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_sim_messages
    @{
  */
/** Request message; Reads data to a specific file on a specified application
    on the card. The type of file is determined by the record number field,
    which indicates a transparent file when zero and a record-based file
    otherwise.*/
typedef struct {

  /* Mandatory */
  /*  Application Identification */
  mcm_sim_application_identification_info_t_v01 app_info;
  /**< Application identification information. */

  /* Mandatory */
  /*  File Access Information */
  mcm_sim_file_access_t_v01 file_access;
  /**< File access information. */
}mcm_sim_read_file_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_sim_messages
    @{
  */
/** Response message; Reads data to a specific file on a specified application
    on the card. The response contains the status code received from the card
    (SW1 and SW2) when the card responded to the read request, as well as the
    data that was read from the file.*/
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;
  /**< Standard response type. Contains the following data members: \n
       - mcm_result_t -- MCM_RESULT_SUCCESS or MCM_RESULT_FAILURE
       - mcm_error_t  -- Error code. Possible error code values are described
                          in the error codes section of each message
                          definition. @tablebulletend
  */

  /* Optional */
  /*  Card Result */
  uint8_t card_result_valid;  /**< Must be set to TRUE if card_result is being passed. */
  mcm_sim_card_result_t_v01 card_result;
  /**< Card result. */

  /* Optional */
  /*  Data Retrieved from the Card */
  uint8_t data_valid;  /**< Must be set to TRUE if data is being passed. */
  uint32_t data_len;  /**< Must be set to the number of elements in data. */
  uint8_t data[MCM_SIM_DATA_MAX_V01];
  /**< Data retrieved from the card. */
}mcm_sim_read_file_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_sim_messages
    @{
  */
/** Request message; Writes data to a specific file on a specified application
    on the card. The type of file is determined by the record number field,
    which indicates a transparent file when zero and a record-based file otherwise. */
typedef struct {

  /* Mandatory */
  /*  Application Identification */
  mcm_sim_application_identification_info_t_v01 app_info;
  /**< Application identification information. */

  /* Mandatory */
  /*  File Access Information */
  mcm_sim_file_access_t_v01 file_access;
  /**< File access information. */

  /* Mandatory */
  /*  Data to be Updated on Card */
  uint32_t data_len;  /**< Must be set to the number of elements in data. */
  uint8_t data[MCM_SIM_DATA_MAX_V01];
  /**< Data to be updated on the card. */
}mcm_sim_write_file_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_sim_messages
    @{
  */
/** Response message; Writes data to a specific file on a specified application
    on the card. The response contains the status code received from the card
    (SW1 and SW2) when the card responded to the write request. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;
  /**< Standard response type. Contains the following data members: \n
       - mcm_result_t -- MCM_RESULT_SUCCESS or MCM_RESULT_FAILURE
       - mcm_error_t  -- Error code. Possible error code values are described
                          in the error codes section of each message
                          definition. @tablebulletend
  */

  /* Optional */
  /*  Card Result */
  uint8_t card_result_valid;  /**< Must be set to TRUE if card_result is being passed. */
  mcm_sim_card_result_t_v01 card_result;
  /**< Card result. */
}mcm_sim_write_file_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_sim_messages
    @{
  */
/** Request message; Retrieves the size of a specific file on a specified application on
           the card. */
typedef struct {

  /* Mandatory */
  /*  Application Identification */
  mcm_sim_application_identification_info_t_v01 app_info;
  /**< Application identification information. */

  /* Mandatory */
  /*  File Path */
  uint32_t path_len;  /**< Must be set to the number of elements in path. */
  char path[MCM_SIM_CHAR_PATH_MAX_V01];
  /**< File path in ASCII characters.*/
}mcm_sim_get_file_size_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_sim_messages
    @{
  */
/** Response message; Retrieves the size of a specific file on a specified
    application on the card. The response contains the status code received
    from the card (SW1 and SW2) when the card responded to the get file size
    request. The response also contains the type of file associated with the size. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;
  /**< Standard response type. Contains the following data members: \n
       - mcm_result_t -- MCM_RESULT_SUCCESS or MCM_RESULT_FAILURE
       - mcm_error_t  -- Error code. Possible error code values are described
                          in the error codes section of each message
                          definition. @tablebulletend
  */

  /* Optional */
  /*  Card Result */
  uint8_t card_result_valid;  /**< Must be set to TRUE if card_result is being passed. */
  mcm_sim_card_result_t_v01 card_result;
  /**< Card result. */

  /* Optional */
  /*  File Information */
  uint8_t file_info_valid;  /**< Must be set to TRUE if file_info is being passed. */
  mcm_sim_file_info_t_v01 file_info;
  /**< File information. */
}mcm_sim_get_file_size_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_sim_messages
    @{
  */
/** Request message; Verifies the PIN value of an application. The same PIN can
    be used by multiple sessions (i.e., the PIN is shared between GSM and RUIM
    in an ICC card). The PIN is automatically verified for all the sessions
    when the command is executed. */
typedef struct {

  /* Mandatory */
  /*  Application Identification */
  mcm_sim_application_identification_info_t_v01 app_info;
  /**< Application identification information. */

  /* Mandatory */
  /*  PIN ID */
  mcm_sim_pin_id_t_v01 pin_id;
  /**< PIN ID. */

  /* Mandatory */
  /*  Value of the PIN */
  uint32_t pin_value_len;  /**< Must be set to the number of elements in pin_value. */
  char pin_value[MCM_SIM_PIN_MAX_V01];
  /**< PIN value. */
}mcm_sim_verify_pin_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_sim_messages
    @{
  */
/** Response message; Verifies the PIN value of an application. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;
  /**< Standard response type. Contains the following data members: \n
       - mcm_result_t -- MCM_RESULT_SUCCESS or MCM_RESULT_FAILURE
       - mcm_error_t  -- Error code. Possible error code values are described
                          in the error codes section of each message
                          definition. @tablebulletend
  */

  /* Optional */
  /*  Verify Retries Remaining */
  uint8_t retries_left_valid;  /**< Must be set to TRUE if retries_left is being passed. */
  uint8_t retries_left;
  /**< Retries remaining. */
}mcm_sim_verify_pin_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_sim_messages
    @{
  */
/** Request message; Changes the PIN value of an application. The application
    must pass both the new and the old values of the PIN to complete the
    operation. 
    
    The same PIN can be used by multiple sessions (i.e., the PIN is shared
    between GSM and RUIM in an ICC card). The PIN is automatically verified
    for all the sessions when the command is executed. */
typedef struct {

  /* Mandatory */
  /*  Application Identification */
  mcm_sim_application_identification_info_t_v01 app_info;
  /**< Application identification information. */

  /* Mandatory */
  /*  PIN ID */
  mcm_sim_pin_id_t_v01 pin_id;
  /**< PIN ID. */

  /* Mandatory */
  /*  Value of the Old PIN in Sequence of ASCII Characters */
  uint32_t old_pin_value_len;  /**< Must be set to the number of elements in old_pin_value. */
  char old_pin_value[MCM_SIM_PIN_MAX_V01];
  /**< Value of the old PIN as a sequence of ASCII characters. */

  /* Mandatory */
  /*  Value of the New PIN in Sequence of ASCII Characters */
  uint32_t new_pin_value_len;  /**< Must be set to the number of elements in new_pin_value. */
  char new_pin_value[MCM_SIM_PIN_MAX_V01];
  /**< Value of the new PIN as a sequence of ASCII characters. */
}mcm_sim_change_pin_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_sim_messages
    @{
  */
/** Response message; Changes the PIN value of an application. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;
  /**< Standard response type. Contains the following data members: \n
       - mcm_result_t -- MCM_RESULT_SUCCESS or MCM_RESULT_FAILURE
       - mcm_error_t  -- Error code. Possible error code values are described
                          in the error codes section of each message
                          definition. @tablebulletend
  */

  /* Optional */
  /*  Verify Retries Remaining */
  uint8_t retries_left_valid;  /**< Must be set to TRUE if retries_left is being passed. */
  uint8_t retries_left;
  /**< Retries remaining. */
}mcm_sim_change_pin_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_sim_messages
    @{
  */
/** Request message; Unblocks a blocked PIN using the PUK code. The client must
    pass PUK1 to unblock PIN1 or PUK2 to unblock PIN2.
    
    The same PIN can be used by multiple sessions (i.e., the PIN is shared
    between GSM and RUIM in an ICC card). The PIN is automatically verified
    for all the sessions when the command is executed. */
typedef struct {

  /* Mandatory */
  /*  Application Identification */
  mcm_sim_application_identification_info_t_v01 app_info;
  /**< Application identification information. */

  /* Mandatory */
  /*  PIN ID */
  mcm_sim_pin_id_t_v01 pin_id;
  /**< PIN ID. */

  /* Mandatory */
  /*  Value of the PUK in Sequence of ASCII Characters */
  uint32_t puk_value_len;  /**< Must be set to the number of elements in puk_value. */
  char puk_value[MCM_SIM_PIN_MAX_V01];
  /**< Value of the PUK as a sequence of ASCII characters. */

  /* Mandatory */
  /*  Value of the New PIN in Sequence of ASCII Characters */
  uint32_t new_pin_value_len;  /**< Must be set to the number of elements in new_pin_value. */
  char new_pin_value[MCM_SIM_PIN_MAX_V01];
  /**< Value of the new PIN as a sequence of ASCII characters. */
}mcm_sim_unblock_pin_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_sim_messages
    @{
  */
/** Response message; Unblocks a blocked PIN using the PUK code. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;
  /**< Standard response type. Contains the following data members: \n
       - mcm_result_t -- MCM_RESULT_SUCCESS or MCM_RESULT_FAILURE
       - mcm_error_t  -- Error code. Possible error code values are described
                          in the error codes section of each message
                          definition. @tablebulletend
  */

  /* Optional */
  /*  Unblock Retries Remaining */
  uint8_t retries_left_valid;  /**< Must be set to TRUE if retries_left is being passed. */
  uint8_t retries_left;
  /**< Retries remaining. */
}mcm_sim_unblock_pin_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_sim_messages
    @{
  */
/** Request message; Enables the PIN on an application. */
typedef struct {

  /* Mandatory */
  /*  Application Identification */
  mcm_sim_application_identification_info_t_v01 app_info;
  /**< Application identification information. */

  /* Mandatory */
  /*  PIN ID */
  mcm_sim_pin_id_t_v01 pin_id;
  /**< PIN ID. */

  /* Mandatory */
  /*  Value of the PIN in Sequence of ASCII Characters */
  uint32_t pin_value_len;  /**< Must be set to the number of elements in pin_value. */
  char pin_value[MCM_SIM_PIN_MAX_V01];
  /**< Value of the PIN as a sequence of ASCII characters. */
}mcm_sim_enable_pin_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_sim_messages
    @{
  */
/** Response message; Enables the PIN on an application. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;
  /**< Standard response type. Contains the following data members: \n
       - mcm_result_t -- MCM_RESULT_SUCCESS or MCM_RESULT_FAILURE
       - mcm_error_t  -- Error code. Possible error code values are described
                          in the error codes section of each message
                          definition. @tablebulletend
  */

  /* Optional */
  /*  Verify Retries Remaining */
  uint8_t retries_left_valid;  /**< Must be set to TRUE if retries_left is being passed. */
  uint8_t retries_left;
  /**< Retries remaining. */
}mcm_sim_enable_pin_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_sim_messages
    @{
  */
/** Request message; Enables or disables the PIN of an application, */
typedef struct {

  /* Mandatory */
  /*  Application Identification */
  mcm_sim_application_identification_info_t_v01 app_info;
  /**< Application identification information. */

  /* Mandatory */
  /*  PIN ID */
  mcm_sim_pin_id_t_v01 pin_id;
  /**< PIN ID. */

  /* Mandatory */
  /*  Value of the PIN in Sequence of ASCII Characters */
  uint32_t pin_value_len;  /**< Must be set to the number of elements in pin_value */
  char pin_value[MCM_SIM_PIN_MAX_V01];
  /**< Value of the PIN as a sequence of ASCII characters. */
}mcm_sim_disable_pin_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_sim_messages
    @{
  */
/** Response message; Enables or disables the PIN of an application, */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;
  /**< Standard response type. Contains the following data members: \n
       - mcm_result_t -- MCM_RESULT_SUCCESS or MCM_RESULT_FAILURE
       - mcm_error_t  -- Error code. Possible error code values are described
                          in the error codes section of each message
                          definition. @tablebulletend
  */

  /* Optional */
  /*  Verify Retries Remaining */
  uint8_t retries_left_valid;  /**< Must be set to TRUE if retries_left is being passed. */
  uint8_t retries_left;
  /**< Retries remaining. */
}mcm_sim_disable_pin_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_sim_messages
    @{
  */
/** Request message; Deactivates or unblocks the personalization on the phone.
    Each feature can be deactivated/unblocked independently of the other features. */
typedef struct {

  /* Mandatory */
  /*  Depersonalization */
  mcm_sim_depersonalization_t_v01 depersonalization;
  /**< Depersonalization. */
}mcm_sim_depersonalization_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_sim_messages
    @{
  */
/** Response message; Deactivates or unblocks the personalization on the phone. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;
  /**< Standard response type. Contains the following data members: \n
       - mcm_result_t -- MCM_RESULT_SUCCESS or MCM_RESULT_FAILURE
       - mcm_error_t  -- Error code. Possible error code values are described
                          in the error codes section of each message
                          definition. @tablebulletend
  */

  /* Optional */
  /*  Retries Remaining */
  uint8_t retries_left_valid;  /**< Must be set to TRUE if retries_left is being passed. */
  mcm_sim_perso_retries_left_t_v01 retries_left;
  /**< Retries remaining. */
}mcm_sim_depersonalization_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_sim_messages
    @{
  */
/** Request message; Activates and sets the personalization data on the phone.
    Each feature can be activated independently of one another; however,
    network data configurations must be consistent across activated
    personalization modes in order to prevent contradicting featurization,
    and only one feature can be activated per message.
    
    If personalization is already activated, it must first be deactivated
    before being reactivated with new data. */
typedef struct {

  /* Mandatory */
  /*  Control Key */
  uint32_t ck_value_len;  /**< Must be set to the number of elements in ck_value. */
  char ck_value[MCM_SIM_CK_MAX_V01];
  /**< Control key value. This value is a sequence of ASCII characters.*/

  /* Optional */
  /*  GW network personalization */
  uint8_t feature_gw_network_perso_valid;  /**< Must be set to TRUE if feature_gw_network_perso is being passed. */
  uint32_t feature_gw_network_perso_len;  /**< Must be set to the number of elements in feature_gw_network_perso. */
  mcm_sim_network_perso_t_v01 feature_gw_network_perso[MCM_SIM_PERSO_NUM_NW_MAX_V01];
  /**< GW network personalization. */

  /* Optional */
  /*  GW Network Subset Personalization */
  uint8_t feature_gw_network_subset_perso_valid;  /**< Must be set to TRUE if feature_gw_network_subset_perso is being passed. */
  uint32_t feature_gw_network_subset_perso_len;  /**< Must be set to the number of elements in feature_gw_network_subset_perso. */
  mcm_sim_gw_network_subset_perso_t_v01 feature_gw_network_subset_perso[MCM_SIM_PERSO_NUM_NS_MAX_V01];
  /**< GW network subset personalization. */

  /* Optional */
  /*  GW Service Provider Personalization */
  uint8_t feature_gw_sp_perso_valid;  /**< Must be set to TRUE if feature_gw_sp_perso is being passed. */
  uint32_t feature_gw_sp_perso_len;  /**< Must be set to the number of elements in feature_gw_sp_perso. */
  mcm_sim_gw_sp_perso_t_v01 feature_gw_sp_perso[MCM_SIM_PERSO_NUM_GW_SP_MAX_V01];
  /**< GW service provider personalization. */

  /* Optional */
  /*  GW Corporate Personalization */
  uint8_t feature_gw_corporate_perso_valid;  /**< Must be set to TRUE if feature_gw_corporate_perso is being passed. */
  uint32_t feature_gw_corporate_perso_len;  /**< Must be set to the number of elements in feature_gw_corporate_perso. */
  mcm_sim_gw_corporate_perso_t_v01 feature_gw_corporate_perso[MCM_SIM_PERSO_NUM_GW_CP_MAX_V01];
  /**< GW corporate personalization. */

  /* Optional */
  /*  GW SIM Personalization */
  uint8_t feature_gw_sim_perso_valid;  /**< Must be set to TRUE if feature_gw_sim_perso is being passed. */
  uint32_t feature_gw_sim_perso_len;  /**< Must be set to the number of elements in feature_gw_sim_perso. */
  mcm_sim_sim_perso_t_v01 feature_gw_sim_perso[MCM_SIM_PERSO_NUM_SIM_MAX_V01];
  /**< GW SIM personalization. */

  /* Optional */
  /*  1x Network Type 1 Personalization */
  uint8_t feature_1x_network1_perso_valid;  /**< Must be set to TRUE if feature_1x_network1_perso is being passed. */
  uint32_t feature_1x_network1_perso_len;  /**< Must be set to the number of elements in feature_1x_network1_perso. */
  mcm_sim_network_perso_t_v01 feature_1x_network1_perso[MCM_SIM_PERSO_NUM_NW_MAX_V01];
  /**< 1X network type 1 personalization. */

  /* Optional */
  /*  1x Network Type 2 Personalization */
  uint8_t feature_1x_network2_perso_valid;  /**< Must be set to TRUE if feature_1x_network2_perso is being passed. */
  uint32_t feature_1x_network2_perso_len;  /**< Must be set to the number of elements in feature_1x_network2_perso. */
  mcm_sim_1x_network_type2_perso_t_v01 feature_1x_network2_perso[MCM_SIM_PERSO_NUM_1X_NW2_MAX_V01];
  /**< 1X network type 3 personalization. */

  /* Optional */
  /*  1x RUIM Personalization */
  uint8_t feature_1x_ruim_perso_valid;  /**< Must be set to TRUE if feature_1x_ruim_perso is being passed. */
  uint32_t feature_1x_ruim_perso_len;  /**< Must be set to the number of elements in feature_1x_ruim_perso. */
  mcm_sim_sim_perso_t_v01 feature_1x_ruim_perso[MCM_SIM_PERSO_NUM_SIM_MAX_V01];
  /**< 1X RUIM personalization. */
}mcm_sim_personalization_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_sim_messages
    @{
  */
/** Response message; Activates and sets the personalization data on the phone. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;
  /**< Standard response type. Contains the following data members:\n
       - qmi_result_type -- QMI_RESULT_SUCCESS or QMI_RESULT_FAILURE
       - qmi_error_type  -- Error code. Possible error code values are described
                          in the error codes section of each message
                          definition. @tablebulletend
  */

  /* Optional */
  /*  Retries Remaining */
  uint8_t retries_left_valid;  /**< Must be set to TRUE if retries_left is being passed. */
  mcm_sim_perso_retries_left_t_v01 retries_left;
  /**< This value is returned only when activation and setting personalization
       data fails. */
}mcm_sim_personalization_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_sim_messages
    @{
  */
/** Request message; Retrieves the card status stored on a card. */
typedef struct {

  /* Mandatory */
  /*  Slot */
  mcm_sim_slot_id_t_v01 slot_id;
  /**< Slot ID. */
}mcm_sim_get_card_status_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_sim_messages
    @{
  */
/** Response message; Retrieves the card status stored on a card. The result of
    this function can be used by the client to determine the number of slots
    supported by the specific target. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;
  /**< Standard response type. Contains the following data members: \n
       - mcm_result_t -- MCM_RESULT_SUCCESS or MCM_RESULT_FAILURE
       - mcm_error_t  -- Error code. Possible error code values are described
                          in the error codes section of each message
                          definition. @tablebulletend
  */

  /* Optional */
  /*  Card Information */
  uint8_t card_info_valid;  /**< Must be set to TRUE if card_info is being passed. */
  mcm_sim_card_info_t_v01 card_info;
  /**< Card information. */

}mcm_sim_get_card_status_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_sim_messages
    @{
  */
/** Request message; Registers/deregisters for unsolicited SIM event indications. */
typedef struct {

  /* Optional */
  /*  Register for Card Status Events */
  uint8_t register_card_status_event_valid;  /**< Must be set to TRUE if register_card_status_event is being passed. */
  uint8_t register_card_status_event;
  /**< Register for card status events. */

  /* Optional */
  /*  Register for Refresh Events */
  uint8_t register_refresh_event_valid;  /**< Must be set to TRUE if register_refresh_event is being passed. */
  uint8_t register_refresh_event;
  /**< Register for refresh events. */
}mcm_sim_event_register_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_sim_messages
    @{
  */
/** Response message; Registers/deregisters for unsolicited SIM event
    indications. The client is notified only when any file that belongs
    to the requested session type is modified by the Refresh procedure.
    The client does not need to specify a list of files.
    
    The client can deregister from card status and/or refresh events by
    indicating a FALSE Boolean value. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;
  /**< Standard response type. Contains the following data members: \n
       - mcm_result_t -- MCM_RESULT_SUCCESS or MCM_RESULT_FAILURE
       - mcm_error_t  -- Error code. Possible error code values are described
                          in the error codes section of each message
                          definition. @tablebulletend
  */
}mcm_sim_event_register_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_sim_messages
    @{
  */
/** Indication message; Indication corresponding to MCM_SIM_CARD_STATUS_EVENT_IND. */
typedef struct {

  /* Optional */
  /*  Slot ID */
  uint8_t slot_id_valid;  /**< Must be set to TRUE if slot_id is being passed. */
  mcm_sim_slot_id_t_v01 slot_id;
  /**< Identifies to which card the indication corresponds. */

  /* Optional */
  /*  Card Information Indication */
  uint8_t card_info_valid;  /**< Must be set to TRUE if card_info is being passed. */
  mcm_sim_card_info_t_v01 card_info;
  /**< Card information indication. */
}mcm_sim_card_status_event_ind_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_sim_messages
    @{
  */
/** Indication message; Indication corresponding to MCM_SIM_REFRESH_EVENT_IND. */
typedef struct {

  /* Optional */
  /*  Refresh Indication Information */
  uint8_t refresh_event_valid;  /**< Must be set to TRUE if refresh_event is being passed. */
  mcm_sim_refresh_event_t_v01 refresh_event;
  /**< Refresh indication information. */
}mcm_sim_refresh_event_ind_msg_v01;  /* Message */
/**
    @}
  */

/*Service Message Definition*/
/** @addtogroup mcm_sim_msg_ids
    @{
  */

/** @name
@{ */

#define MCM_SIM_GET_SUBSCRIBER_ID_REQ_V01 0x0B00
#define MCM_SIM_GET_SUBSCRIBER_ID_RESP_V01 0x0B00
#define MCM_SIM_GET_CARD_ID_REQ_V01 0x0B01
#define MCM_SIM_GET_CARD_ID_RESP_V01 0x0B01
#define MCM_SIM_GET_DEVICE_PHONE_NUMBER_REQ_V01 0x0B02
#define MCM_SIM_GET_DEVICE_PHONE_NUMBER_RESP_V01 0x0B02
#define MCM_SIM_GET_PREFERRED_OPERATOR_LIST_REQ_V01 0x0B03
#define MCM_SIM_GET_PREFERRED_OPERATOR_LIST_RESP_V01 0x0B03
#define MCM_SIM_READ_FILE_REQ_V01 0x0B04
#define MCM_SIM_READ_FILE_RESP_V01 0x0B04
#define MCM_SIM_WRITE_FILE_REQ_V01 0x0B05
#define MCM_SIM_WRITE_FILE_RESP_V01 0x0B05
#define MCM_SIM_GET_FILE_SIZE_REQ_V01 0x0B06
#define MCM_SIM_GET_FILE_SIZE_RESP_V01 0x0B06
#define MCM_SIM_VERIFY_PIN_REQ_V01 0x0B07
#define MCM_SIM_VERIFY_PIN_RESP_V01 0x0B07
#define MCM_SIM_CHANGE_PIN_REQ_V01 0x0B08
#define MCM_SIM_CHANGE_PIN_RESP_V01 0x0B08
#define MCM_SIM_UNBLOCK_PIN_REQ_V01 0x0B09
#define MCM_SIM_UNBLOCK_PIN_RESP_V01 0x0B09
#define MCM_SIM_ENABLE_PIN_REQ_V01 0x0B0A
#define MCM_SIM_ENABLE_PIN_RESP_V01 0x0B0A
#define MCM_SIM_DISABLE_PIN_REQ_V01 0x0B0B
#define MCM_SIM_DISABLE_PIN_RESP_V01 0x0B0B
#define MCM_SIM_GET_CARD_STATUS_REQ_V01 0x0B0C
#define MCM_SIM_GET_CARD_STATUS_RESP_V01 0x0B0C
#define MCM_SIM_DEPERSONALIZATION_REQ_V01 0x0B0D
#define MCM_SIM_DEPERSONALIZATION_RESP_V01 0x0B0D
#define MCM_SIM_PERSONALIZATION_REQ_V01 0x0B0E
#define MCM_SIM_PERSONALIZATION_RESP_V01 0x0B0E
#define MCM_SIM_EVENT_REGISTER_REQ_V01 0x0B0F
#define MCM_SIM_EVENT_REGISTER_RESP_V01 0x0B0F
#define MCM_SIM_CARD_STATUS_EVENT_IND_V01 0x0B10
#define MCM_SIM_REFRESH_EVENT_IND_V01 0x0B11

/** @} */ /* end_namegroup */

/** @} */

#ifdef __cplusplus
}
#endif
#endif

