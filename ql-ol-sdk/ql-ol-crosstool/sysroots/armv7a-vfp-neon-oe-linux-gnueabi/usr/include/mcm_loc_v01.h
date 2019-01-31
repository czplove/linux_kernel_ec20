#ifndef MCM_LOC_SERVICE_01_H
#define MCM_LOC_SERVICE_01_H
/**
  @file mcm_loc_v01.h

  @brief This is the public header file which defines the mcm_loc service Data structures.

  This header file defines the types and structures that were defined in
  mcm_loc. It contains the constant values defined, enums, structures,
  messages, and service message IDs (in that order) Structures that were
  defined in the IDL as messages contain mandatory elements, optional
  elements, a combination of mandatory and optional elements (mandatory
  always come before optionals in the structure), or nothing (null message)

  An optional element in a message is preceded by a uint8_t value that must be
  set to true if the element is going to be included. When decoding a received
  message, the uint8_t values will be set to true or false by the decode
  routine, and should be checked before accessing the values that they
  correspond to.

  Variable sized arrays are defined as static sized arrays with an unsigned
  integer (32 bit) preceding it that must be set to the number of elements
  in the array that are valid. For Example:

  uint32_t test_opaque_len;
  uint8_t test_opaque[16];

  If only 4 elements are added to test_opaque[] then test_opaque_len must be
  set to 4 before sending the message.  When decoding, the _len value is set
  by the decode routine and should be checked so that the correct number of
  elements in the array will be accessed.

*/
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
  Copyright (c) 2013 Qualcomm Technologies, Inc.
  All rights reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.


  $Header$
 *====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
 *THIS IS AN AUTO GENERATED FILE. DO NOT ALTER IN ANY WAY
 *====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* This file was generated with Tool version 6.14.7
   It was generated on: Tue Oct 27 2015 (Spin 0)
   From IDL File: mcm_loc_v01.idl */

/** @defgroup mcm_loc_qmi_consts Constant values defined in the IDL */
/** @defgroup mcm_loc_qmi_msg_ids Constant values for QMI message IDs */
/** @defgroup mcm_loc_qmi_enums Enumerated types used in QMI messages */
/** @defgroup mcm_loc_qmi_messages Structures sent as QMI messages */
/** @defgroup mcm_loc_qmi_aggregates Aggregate types used in QMI messages */
/** @defgroup mcm_loc_qmi_accessor Accessor for QMI service object */
/** @defgroup mcm_loc_qmi_version Constant values for versioning information */

#include <stdint.h>
#include "qmi_idl_lib.h"
#include "mcm_common_v01.h"


#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup mcm_loc_qmi_version
    @{
  */
/** Major Version Number of the IDL used to generate this file */
#define MCM_LOC_V01_IDL_MAJOR_VERS 0x01
/** Revision Number of the IDL used to generate this file */
#define MCM_LOC_V01_IDL_MINOR_VERS 0x02
/** Major Version Number of the qmi_idl_compiler used to generate this file */
#define MCM_LOC_V01_IDL_TOOL_VERS 0x06
/** Maximum Defined Message ID */
#define MCM_LOC_V01_MAX_MESSAGE_ID 0x0317
/**
    @}
  */


/** @addtogroup mcm_loc_qmi_consts
    @{
  */

/**  Maximum generic server address length for the host name. */
#define MCM_LOC_MAX_SEVER_ADDR_LENGTH_CONST_V01 255

/**  Maximum generic string length. */
#define MCM_LOC_MAX_GENERIC_STR_LENGTH_CONST_V01 255

/**  APN name maximum length. */
#define MCM_LOC_MAX_APN_NAME_LENGTH_CONST_V01 100

/**  Maximum length of XTRA data injected. */
#define MCM_LOC_MAX_XTRA_DATA_LENGTH_CONST_V01 64512

/**  Location map URL maximum size
 (used for indoor positioning). */
#define MCM_LOC_GPS_LOCATION_MAP_URL_SIZE_CONST_V01 399

/**  Location map index maximum size
 (used for indoor positioning). */
#define MCM_LOC_GPS_LOCATION_MAP_INDEX_SIZE_CONST_V01 16

/**  Maximum number of satellites in view. */
#define MCM_LOC_GPS_MAX_SVS_CONST_V01 32

/**  Maximum SSID (Service Set Identifier) buffer size. */
#define MCM_LOC_GPS_SSID_BUF_SIZE_CONST_V01 32

/**  IPv6 address length. */
#define MCM_LOC_IPV6_ADDR_LEN_CONST_V01 16

/**  NI short string maximum length. */
#define MCM_LOC_GPS_NI_SHORT_STRING_MAXLEN_CONST_V01 255

/**  NI long string maximum length. */
#define MCM_LOC_GPS_NI_LONG_STRING_MAXLEN_CONST_V01 2047

/**  Raw data maximum size. */
#define MCM_LOC_GPS_RAW_DATA_MAX_SIZE_CONST_V01 256

/**  NMEA string maximum length. */
#define MCM_LOC_GPS_NMEA_MAX_LENGTH_CONST_V01 255
/**
    @}
  */

/** @addtogroup mcm_loc_qmi_enums
    @{
  */
typedef enum {
  MCM_GPS_POSITION_MODE_T_MIN_ENUM_VAL_V01 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
  MCM_LOC_POSITION_MODE_STANDALONE_V01 = 0, /**<  Mode for running GPS standalone (no assistance).  */
  MCM_LOC_POSITION_MODE_MS_BASED_V01 = 1, /**<  AGPS MS-Based mode.  */
  MCM_LOC_POSITION_MODE_MS_ASSISTED_V01 = 2, /**<  AGPS MS-Assisted mode.  */
  MCM_GPS_POSITION_MODE_T_MAX_ENUM_VAL_V01 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}mcm_gps_position_mode_t_v01;
/**
    @}
  */

/** @addtogroup mcm_loc_qmi_enums
    @{
  */
typedef enum {
  MCM_GPS_POSITION_RECURRENCE_T_MIN_ENUM_VAL_V01 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
  MCM_LOC_POSITION_RECURRENCE_PERIODIC_V01 = 0, /**<  Receive GPS fixes on a recurring basis at a specified period.  */
  MCM_LOC_POSITION_RECURRENCE_SINGLE_V01 = 1, /**<  Request a single-shot GPS fix.  */
  MCM_GPS_POSITION_RECURRENCE_T_MAX_ENUM_VAL_V01 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}mcm_gps_position_recurrence_t_v01;
/**
    @}
  */

/** @addtogroup mcm_loc_qmi_enums
    @{
  */
typedef enum {
  MCM_GPS_AIDING_DATA_T_MIN_ENUM_VAL_V01 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
  MCM_LOC_DELETE_EPHEMERIS_V01 = 0x00000001, /**<  Delete ephemeris data.  */
  MCM_LOC_DELETE_ALMANAC_V01 = 0x00000002, /**<  Delete almanac data.  */
  MCM_LOC_DELETE_POSITION_V01 = 0x00000004, /**<  Delete position data.  */
  MCM_LOC_DELETE_TIME_V01 = 0x00000008, /**<  Delete time data.  */
  MCM_LOC_DELETE_IONO_V01 = 0x00000010, /**<  Delete IONO data.  */
  MCM_LOC_DELETE_UTC_V01 = 0x00000020, /**<  Delete UTC data.  */
  MCM_LOC_DELETE_HEALTH_V01 = 0x00000040, /**<  Delete health data.  */
  MCM_LOC_DELETE_SVDIR_V01 = 0x00000080, /**<  Delete SVDIR data.  */
  MCM_LOC_DELETE_SVSTEER_V01 = 0x00000100, /**<  Delete SVSTEER data.  */
  MCM_LOC_DELETE_SADATA_V01 = 0x00000200, /**<  Delete SA data.  */
  MCM_LOC_DELETE_RTI_V01 = 0x00000400, /**<  Delete RTI data.  */
  MCM_LOC_DELETE_CELLDB_INFO_V01 = 0x00000800, /**<  Delete cell DB information.  */
  MCM_LOC_DELETE_ALMANAC_CORR_V01 = 0x00001000, /**<  Delete almanac correction data.  */
  MCM_LOC_DELETE_FREQ_BIAS_EST_V01 = 0x00002000, /**<  Delete frequency bias estimate.  */
  MCM_LOC_DELETE_EPHEMERIS_GLO_V01 = 0x00004000, /**<  Delete ephemeris GLO data.  */
  MCM_LOC_DELETE_ALMANAC_GLO_V01 = 0x00008000, /**<  Delete almanac GLO data.  */
  MCM_LOC_DELETE_SVDIR_GLO_V01 = 0x00010000, /**<  Delete SVDIR GLO data.  */
  MCM_LOC_DELETE_SVSTEER_GLO_V01 = 0x00020000, /**<  Delete SVSTEER GLO data.  */
  MCM_LOC_DELETE_ALMANAC_CORR_GLO_V01 = 0x00040000, /**<  Delete almanac correction GLO data.  */
  MCM_LOC_DELETE_TIME_GPS_V01 = 0x00080000, /**<  Delete time GPS data.  */
  MCM_LOC_DELETE_TIME_GLO_V01 = 0x00100000, /**<  Delete time GLO data.  */
  MCM_LOC_DELETE_ALL_V01 = 0xFFFFFFFF, /**<  Delete all location data.  */
  MCM_GPS_AIDING_DATA_T_MAX_ENUM_VAL_V01 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}mcm_gps_aiding_data_t_v01;
/**
    @}
  */

/** @addtogroup mcm_loc_qmi_enums
    @{
  */
typedef enum {
  MCM_AGPS_T_MIN_ENUM_VAL_V01 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
  MCM_LOC_AGPS_TYPE_INVALID_V01 = -1, /**<  Invalid.  */
  MCM_LOC_AGPS_TYPE_ANY_V01 = 0, /**<  Any.  */
  MCM_LOC_AGPS_TYPE_SUPL_V01 = 1, /**<  SUPL.  */
  MCM_LOC_AGPS_TYPE_C2K_V01 = 2, /**<  C2K.  */
  MCM_LOC_AGPS_TYPE_WWAN_ANY_V01 = 3, /**<  WWAN any.  */
  MCM_LOC_AGPS_TYPE_WIFI_V01 = 4, /**<  Wi-Fi.  */
  MCM_LOC_AGPS_TYPE_SUPL_ES_V01 = 5, /**<  SUPL_ES.  */
  MCM_AGPS_T_MAX_ENUM_VAL_V01 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}mcm_agps_t_v01;
/**
    @}
  */

/** @addtogroup mcm_loc_qmi_enums
    @{
  */
typedef enum {
  MCM_AGPS_BEARER_T_MIN_ENUM_VAL_V01 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
  MCM_LOC_AGPS_APN_BEARER_INVALID_V01 = -1, /**<  Invalid.  */
  MCM_LOC_AGPS_APN_BEARER_IPV4_V01 = 0, /**<  IPv4.  */
  MCM_LOC_AGPS_APN_BEARER_IPV6_V01 = 1, /**<  IPv6.  */
  MCM_LOC_AGPS_APN_BEARER_IPV4V6_V01 = 2, /**<  IPv4/v6.  */
  MCM_AGPS_BEARER_T_MAX_ENUM_VAL_V01 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}mcm_agps_bearer_t_v01;
/**
    @}
  */

/** @addtogroup mcm_loc_qmi_enums
    @{
  */
typedef enum {
  MCM_GPS_USER_RESPONSE_T_MIN_ENUM_VAL_V01 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
  MCM_LOC_NI_RESPONSE_ACCEPT_V01 = 1, /**<  Accept.  */
  MCM_LOC_NI_RESPONSE_DENY_V01 = 2, /**<  Deny.  */
  MCM_LOC_NI_RESPONSE_NORESP_V01 = 3, /**<  No response.  */
  MCM_GPS_USER_RESPONSE_T_MAX_ENUM_VAL_V01 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}mcm_gps_user_response_t_v01;
/**
    @}
  */

/** @addtogroup mcm_loc_qmi_enums
    @{
  */
typedef enum {
  MCM_GPS_LOCATION_FLAG_T_MIN_ENUM_VAL_V01 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
  MCM_LOC_GPS_LOCATION_HAS_LAT_LONG_V01 = 0x0001, /**<  GPS location has valid latitude and longitude.  */
  MCM_LOC_GPS_LOCATION_HAS_ALTITUDE_V01 = 0x0002, /**<  GPS location has a valid altitude.  */
  MCM_LOC_GPS_LOCATION_HAS_SPEED_V01 = 0x0004, /**<  GPS location has a valid speed.  */
  MCM_LOC_GPS_LOCATION_HAS_BEARING_V01 = 0x0008, /**<  GPS location has a valid bearing.  */
  MCM_LOC_GPS_LOCATION_HAS_ACCURACY_V01 = 0x0010, /**<  GPS location has valid accuracy.  */
  MCM_LOC_GPS_LOCATION_HAS_SOURCE_INFO_V01 = 0x0020, /**<  GPS location has valid source information.  */
  MCM_LOC_GPS_LOCATION_HAS_IS_INDOOR_V01 = 0x0040, /**<  GPS location has a valid "is indoor?" flag.  */
  MCM_LOC_GPS_LOCATION_HAS_FLOOR_NUMBE_V01 = 0x0080, /**<  GPS location has a valid floor number.  */
  MCM_LOC_GPS_LOCATION_HAS_MAP_URL_V01 = 0x0100, /**<  GPS location has a valid map URL.  */
  MCM_LOC_GPS_LOCATION_HAS_MAP_INDEX_V01 = 0x0200, /**<  GPS location has a valid map index.  */
  MCM_GPS_LOCATION_FLAG_T_MAX_ENUM_VAL_V01 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}mcm_gps_location_flag_t_v01;
/**
    @}
  */

/** @addtogroup mcm_loc_qmi_enums
    @{
  */
typedef enum {
  MCM_GPS_STATUS_VALUE_T_MIN_ENUM_VAL_V01 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
  MCM_LOC_GPS_STATUS_NONE_V01 = 0, /**<  GPS status unknown.  */
  MCM_LOC_GPS_STATUS_SESSION_BEGIN_V01 = 1, /**<  GPS has begun navigating.  */
  MCM_LOC_GPS_STATUS_SESSION_END_V01 = 2, /**<  GPS has stopped navigating.  */
  MCM_LOC_GPS_STATUS_ENGINE_ON_V01 = 3, /**<  GPS has powered on but is not navigating.  */
  MCM_LOC_GPS_STATUS_ENGINE_OFF_V01 = 4, /**<  GPS is powered off.  */
  MCM_GPS_STATUS_VALUE_T_MAX_ENUM_VAL_V01 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}mcm_gps_status_value_t_v01;
/**
    @}
  */

/** @addtogroup mcm_loc_qmi_enums
    @{
  */
typedef enum {
  MCM_GPS_CAPABILITIES_T_MIN_ENUM_VAL_V01 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
  MCM_LOC_GPS_CAPABILITY_SCHEDULING_V01 = 0x0000001, /**<  GPS HAL schedules fixes for GPS_POSITION_RECURRENCE_PERIODIC mode.
         If this is not set, the framework uses \n 1000 ms for min_interval
         and will call start() and stop() to schedule the GPS.
      */
  MCM_LOC_GPS_CAPABILITY_MSB_V01 = 0x0000002, /**<  GPS supports MS-Based AGPS mode.  */
  MCM_LOC_GPS_CAPABILITY_MSA_V01 = 0x0000004, /**<  GPS supports MS-Assisted AGPS mode.  */
  MCM_LOC_GPS_CAPABILITY_SINGLE_SHOT_V01 = 0x0000008, /**<  GPS supports single-shot fixes.  */
  MCM_LOC_GPS_CAPABILITY_ON_DEMAND_TIME_V01 = 0x0000010, /**<  GPS supports on-demand time injection.  */
  MCM_GPS_CAPABILITIES_T_MAX_ENUM_VAL_V01 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}mcm_gps_capabilities_t_v01;
/**
    @}
  */

/** @addtogroup mcm_loc_qmi_enums
    @{
  */
typedef enum {
  MCM_AGPS_STATUS_VALUE_T_MIN_ENUM_VAL_V01 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
  MCM_LOC_GPS_REQUEST_AGPS_DATA_CONN_V01 = 1, /**<  GPS requests a data connection for AGPS.  */
  MCM_LOC_GPS_RELEASE_AGPS_DATA_CONN_V01 = 2, /**<  GPS releases the AGPS data connection.  */
  MCM_LOC_GPS_AGPS_DATA_CONNECTED_V01 = 3, /**<  AGPS data connection is initiated  */
  MCM_LOC_GPS_AGPS_DATA_CONN_DONE_V01 = 4, /**<  AGPS data connection is completed.  */
  MCM_LOC_GPS_AGPS_DATA_CONN_FAILED_V01 = 5, /**<  AGPS data connection failed.  */
  MCM_AGPS_STATUS_VALUE_T_MAX_ENUM_VAL_V01 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}mcm_agps_status_value_t_v01;
/**
    @}
  */

/** @addtogroup mcm_loc_qmi_enums
    @{
  */
typedef enum {
  MCM_GPS_NI_T_MIN_ENUM_VAL_V01 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
  MCM_LOC_GPS_NI_TYPE_VOICE_V01 = 1, /**<  Voice.  */
  MCM_LOC_GPS_NI_TYPE_UMTS_SUPL_V01 = 2, /**<  UMTS SUPL.  */
  MCM_LOC_GPS_NI_TYPE_UMTS_CTRL_PLANE_V01 = 3, /**<  UMTS control plane.  */
  MCM_GPS_NI_T_MAX_ENUM_VAL_V01 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}mcm_gps_ni_t_v01;
/**
    @}
  */

/** @addtogroup mcm_loc_qmi_enums
    @{
  */
typedef enum {
  MCM_GPS_NI_NOTIFY_FLAGS_T_MIN_ENUM_VAL_V01 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
  MCM_LOC_GPS_NI_NEED_NOTIFY_V01 = 0x0001, /**<  NI requires notification.  */
  MCM_LOC_GPS_NI_NEED_VERIFY_V01 = 0x0002, /**<  NI requires verification.  */
  MCM_LOC_GPS_NI_PRIVACY_OVERRIDE_V01 = 0x0004, /**<  NI requires privacy override; no notification/minimal trace.  */
  MCM_GPS_NI_NOTIFY_FLAGS_T_MAX_ENUM_VAL_V01 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}mcm_gps_ni_notify_flags_t_v01;
/**
    @}
  */

/** @addtogroup mcm_loc_qmi_enums
    @{
  */
typedef enum {
  MCM_GPS_NI_ENCODING_T_MIN_ENUM_VAL_V01 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
  MCM_LOC_GPS_ENC_NONE_V01 = 0, /**<  None.  */
  MCM_LOC_GPS_ENC_SUPL_GSM_DEFAULT_V01 = 1, /**<  SUPL GSM default.  */
  MCM_LOC_GPS_ENC_SUPL_UTF8_V01 = 2, /**<  SUPL UTF8.  */
  MCM_LOC_GPS_ENC_SUPL_UCS2_V01 = 3, /**<  SUPL UCS2.  */
  MCM_LOC_GPS_ENC_UNKNOWN_V01 = -1, /**<  Unknown.  */
  MCM_GPS_NI_ENCODING_T_MAX_ENUM_VAL_V01 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}mcm_gps_ni_encoding_t_v01;
/**
    @}
  */

/** @addtogroup mcm_loc_qmi_enums
    @{
  */
typedef enum {
  MCM_GPS_POSITION_SOURCE_T_MIN_ENUM_VAL_V01 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
  MCM_LOC_ULP_LOCATION_IS_FROM_HYBRID_V01 = 0x0001, /**<  Position source is ULP.  */
  MCM_LOC_ULP_LOCATION_IS_FROM_GNSS_V01 = 0x0002, /**<  Position source is GNSS only.  */
  MCM_GPS_POSITION_SOURCE_T_MAX_ENUM_VAL_V01 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}mcm_gps_position_source_t_v01;
/**
    @}
  */

/** @addtogroup mcm_loc_qmi_aggregates
    @{
  */
typedef struct {

  uint32_t size;
  /**<   Set to the size of mcm_gps_location_t. */

  mcm_gps_location_flag_t_v01 flags;
  /**<   Contains GPS location flags bits. */

  mcm_gps_position_source_t_v01 position_source;
  /**<   Provider indicator for HYBRID or GPS. */

  double latitude;
  /**<   Latitude in degrees. */

  double longitude;
  /**<   Longitude in degrees. */

  double altitude;
  /**<   Altitude in meters above the WGS 84 reference
         ellipsoid. */

  float speed;
  /**<   Speed in meters per second. */

  float bearing;
  /**<   Heading in degrees. */

  float accuracy;
  /**<   Expected accuracy in meters. */

  int64_t timestamp;
  /**<   Timestamp for the location fix. */

  uint32_t raw_data_len;  /**< Must be set to # of elements in raw_data */
  uint8_t raw_data[MCM_LOC_GPS_RAW_DATA_MAX_SIZE_CONST_V01];
  /**<   Allows the HAL to pass additional information related to the location. */

  int32_t is_indoor;
  /**<   Location is indoors. */

  float floor_number;
  /**<   Indicates the floor number. */

  char map_url[MCM_LOC_GPS_LOCATION_MAP_URL_SIZE_CONST_V01 + 1];
  /**<   Map URL. */

  uint8_t map_index[MCM_LOC_GPS_LOCATION_MAP_INDEX_SIZE_CONST_V01];
  /**<   Map index. */
}mcm_gps_location_t_v01;  /* Type */
/**
    @}
  */

/** @addtogroup mcm_loc_qmi_aggregates
    @{
  */
typedef struct {

  uint32_t size;
  /**<   Set to the size of mcm_gps_status_t. */

  mcm_gps_status_value_t_v01 status;
  /**<   Status. */
}mcm_gps_status_t_v01;  /* Type */
/**
    @}
  */

/** @addtogroup mcm_loc_qmi_aggregates
    @{
  */
typedef struct {

  uint32_t size;
  /**<   Set to the size of mcm_gps_sv_info_t. */

  int32_t prn;
  /**<   Pseudo-random number for the SV. */

  float snr;
  /**<   Signal-to-noise ratio. */

  float elevation;
  /**<   Elevation of the SV in degrees. */

  float azimuth;
  /**<   Azimuth of the SV in degrees. */
}mcm_gps_sv_info_t_v01;  /* Type */
/**
    @}
  */

/** @addtogroup mcm_loc_qmi_aggregates
    @{
  */
typedef struct {

  uint32_t size;
  /**<   Set to the size of mcm_gps_sv_status_t. */

  int32_t num_svs;
  /**<   Number of SVs currently visible. */

  mcm_gps_sv_info_t_v01 sv_list[MCM_LOC_GPS_MAX_SVS_CONST_V01];
  /**<   Contains an array of SV information. */

  uint32_t ephemeris_mask;
  /**<   Bitmask indicating which SVs
         have ephemeris data.
     */

  uint32_t almanac_mask;
  /**<   Bitmask indicating which SVs
         have almanac data.
     */

  uint32_t used_in_fix_mask;
  /**<   Bitmask indicating which SVs
         were used for computing the most recent position fix.
     */
}mcm_gps_sv_status_t_v01;  /* Type */
/**
    @}
  */

/** @addtogroup mcm_loc_qmi_aggregates
    @{
  */
typedef struct {

  uint32_t size;
  /**<   Set to the size of mcm_agps_status_t. */

  mcm_agps_t_v01 type;
  /**<   Type. */

  mcm_agps_status_value_t_v01 status;
  /**<   Status. */

  int32_t ipv4_addr;
  /**<   IPv4 address. */

  char ipv6_addr[MCM_LOC_IPV6_ADDR_LEN_CONST_V01 + 1];
  /**<   IPv6 address. */

  char ssid[MCM_LOC_GPS_SSID_BUF_SIZE_CONST_V01 + 1];
  /**<   SSID. */

  char password[MCM_LOC_GPS_SSID_BUF_SIZE_CONST_V01 + 1];
  /**<   Password. */
}mcm_agps_status_t_v01;  /* Type */
/**
    @}
  */

/** @addtogroup mcm_loc_qmi_aggregates
    @{
  */
typedef struct {

  uint32_t size;
  /**<   Set to the size of mcm_gps_ni_notification_t. */

  int32_t notification_id;
  /**<   An ID generated by the HAL to associate NI notifications and UI
         responses.  */

  mcm_gps_ni_t_v01 ni_type;
  /**<   An NI type used to distinguish different categories of NI
         events, such as GPS_NI_TYPE_VOICE, GPS_NI_TYPE_UMTS_SUPL, etc.  */

  mcm_gps_ni_notify_flags_t_v01 notify_flags;
  /**<   Notification/verification options; combinations of GpsNiNotifyFlags
         constants.  */

  int32_t timeout;
  /**<   Timeout period to wait for a user response.
         Set to 0 for no timeout limit.  */

  mcm_gps_user_response_t_v01 default_response;
  /**<   Default response when the response times out.  */

  char requestor_id[MCM_LOC_GPS_NI_SHORT_STRING_MAXLEN_CONST_V01 + 1];
  /**<   Requestor ID.  */

  char text[MCM_LOC_GPS_NI_LONG_STRING_MAXLEN_CONST_V01 + 1];
  /**<   Notification message. It can also be used to store the client ID in some cases.  */

  mcm_gps_ni_encoding_t_v01 requestor_id_encoding;
  /**<   Client ID encoding scheme.  */

  mcm_gps_ni_encoding_t_v01 text_encoding;
  /**<   Client name encoding scheme.  */

  char extras[MCM_LOC_GPS_NI_LONG_STRING_MAXLEN_CONST_V01 + 1];
  /**<   Pointer to extra data. Format:\n
     - key_1 = value_1
     - key_2 = value_2 @tablebulletend
     */
}mcm_gps_ni_notification_t_v01;  /* Type */
/**
    @}
  */

/** @addtogroup mcm_loc_qmi_messages
    @{
  */
/** Request Message; Registers for location indications. */
typedef struct {

  /* Mandatory */
  uint8_t register_location_info_ind;
  /**<   Register for location information indications.*/

  /* Mandatory */
  uint8_t register_status_info_ind;
  /**<   Register for status information indications.*/

  /* Mandatory */
  uint8_t register_sv_info_ind;
  /**<   Register for SV information indications.*/

  /* Mandatory */
  uint8_t register_nmea_info_ind;
  /**<   Register for NMEA information indications.*/

  /* Mandatory */
  uint8_t register_capabilities_info_ind;
  /**<   Register for capabilities information indications.*/

  /* Mandatory */
  uint8_t register_utc_time_req_ind;
  /**<   Register for UTC time request indications.*/

  /* Mandatory */
  uint8_t register_xtra_data_req_ind;
  /**<   Register for XTRA data request indications.*/

  /* Mandatory */
  uint8_t register_agps_data_conn_cmd_req_ind;
  /**<   Register for AGPS data connection command request indications.*/

  /* Mandatory */
  uint8_t register_ni_notify_user_response_req_ind;
  /**<   Register for NI notify user response request indications.*/

  /* Optional */
  uint8_t register_xtra_report_server_ind_valid;  /**< Must be set to true if register_xtra_report_server_ind is being passed */
  uint8_t register_xtra_report_server_ind;
  /**<   Register for XTRA report server indications.*/
}mcm_loc_set_indications_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_loc_qmi_messages
    @{
  */
/** Response Message; Registers for location indications. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;
  /**<   Result code.*/
}mcm_loc_set_indications_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_loc_qmi_messages
    @{
  */
/** Request Message; Sets the position mode. */
typedef struct {

  /* Mandatory */
  /*  Mode */
  mcm_gps_position_mode_t_v01 mode;
  /**<   Position mode.*/

  /* Mandatory */
  /*  Recurrence */
  mcm_gps_position_recurrence_t_v01 recurrence;
  /**<   Recurrence.*/

  /* Mandatory */
  /*  Minimum Interval */
  uint32_t min_interval;
  /**<   Minimum interval.*/

  /* Mandatory */
  /*  Preferred Accuracy */
  uint32_t preferred_accuracy;
  /**<   Preferred accuracy.*/

  /* Mandatory */
  /*  Preferred Time */
  uint32_t preferred_time;
  /**<   Preferred time.*/
}mcm_loc_set_position_mode_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_loc_qmi_messages
    @{
  */
/** Response Message; Sets the position mode. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;
  /**<   Result code.*/
}mcm_loc_set_position_mode_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_loc_qmi_messages
    @{
  */
/** Request Message; Starts navigation. */
typedef struct {
  /* This element is a placeholder to prevent the declaration of
     an empty struct.  DO NOT USE THIS FIELD UNDER ANY CIRCUMSTANCE */
  char __placeholder;
}mcm_loc_start_nav_req_msg_v01;

  /* Message */
/**
    @}
  */

/** @addtogroup mcm_loc_qmi_messages
    @{
  */
/** Response Message; Starts navigation. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;
  /**<   Result code.*/
}mcm_loc_start_nav_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_loc_qmi_messages
    @{
  */
/** Request Message; Stops navigation. */
typedef struct {
  /* This element is a placeholder to prevent the declaration of
     an empty struct.  DO NOT USE THIS FIELD UNDER ANY CIRCUMSTANCE */
  char __placeholder;
}mcm_loc_stop_nav_req_msg_v01;

  /* Message */
/**
    @}
  */

/** @addtogroup mcm_loc_qmi_messages
    @{
  */
/** Response Message; Stops navigation. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;
  /**<   Result code.*/
}mcm_loc_stop_nav_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_loc_qmi_messages
    @{
  */
/** Request Message; Deletes location-aiding data. */
typedef struct {

  /* Mandatory */
  /*  Aiding Data Flags */
  mcm_gps_aiding_data_t_v01 flags;
  /**<   Aiding data flags.*/
}mcm_loc_delete_aiding_data_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_loc_qmi_messages
    @{
  */
/** Response Message; Deletes location-aiding data. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;
  /**<   Result code.*/
}mcm_loc_delete_aiding_data_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_loc_qmi_messages
    @{
  */
/** Request Message; Injects time. */
typedef struct {

  /* Mandatory */
  /*  Time */
  int64_t time;
  /**<   Inject time.*/

  /* Mandatory */
  /*  Time Reference */
  int64_t time_reference;
  /**<   Time reference.*/

  /* Mandatory */
  /*  Uncertainty */
  int32_t uncertainty;
  /**<   Uncertainty.*/
}mcm_loc_inject_time_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_loc_qmi_messages
    @{
  */
/** Response Message; Injects time. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;
  /**<   Result code.*/
}mcm_loc_inject_time_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_loc_qmi_messages
    @{
  */
/** Request Message; Injects a location. */
typedef struct {

  /* Mandatory */
  /*  Latitude */
  double latitude;
  /**<   Latitude.*/

  /* Mandatory */
  /*  Longitude */
  double longitude;
  /**<   Longitude.*/

  /* Mandatory */
  /*  Accuracy */
  float accuracy;
  /**<   Accuracy.*/
}mcm_loc_inject_location_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_loc_qmi_messages
    @{
  */
/** Response Message; Injects a location. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;
  /**<   Result code.*/
}mcm_loc_inject_location_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_loc_qmi_messages
    @{
  */
/** Request Message; Injects XTRA data.
           Since the IPC mechanism puts a limit on the size of the data
           transferable in one message at 64 KB, the application using this
           command must break the data down into chunks of a smaller
           size and repeatedly call this API until all the data has
           been injected. */
typedef struct {

  /* Mandatory */
  /*  Xtra Data */
  uint32_t data_len;  /**< Must be set to # of elements in data */
  uint8_t data[MCM_LOC_MAX_XTRA_DATA_LENGTH_CONST_V01];
  /**<   XTRA data.*/
}mcm_loc_xtra_inject_data_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_loc_qmi_messages
    @{
  */
/** Response Message; Injects XTRA data.
           Since the IPC mechanism puts a limit on the size of the data
           transferable in one message at 64 KB, the application using this
           command must break the data down into chunks of a smaller
           size and repeatedly call this API until all the data has
           been injected. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;
  /**<   Result code.*/
}mcm_loc_xtra_inject_data_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_loc_qmi_messages
    @{
  */
/** Request Message; Indicates that the AGPS data connection is open. */
typedef struct {

  /* Mandatory */
  /*  AGPS Type */
  mcm_agps_t_v01 agps_type;
  /**<   AGPS type.*/

  /* Mandatory */
  /*  APN */
  char apn[MCM_LOC_MAX_APN_NAME_LENGTH_CONST_V01 + 1];
  /**<   APN.*/

  /* Mandatory */
  /*  AGPS Bearer Type */
  mcm_agps_bearer_t_v01 bearer_type;
  /**<   Bearer type.*/
}mcm_loc_agps_data_conn_open_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_loc_qmi_messages
    @{
  */
/** Response Message; Indicates that the AGPS data connection is open. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;
  /**<   Result code.*/
}mcm_loc_agps_data_conn_open_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_loc_qmi_messages
    @{
  */
/** Request Message; Indicates that the AGPS data connection is closed. */
typedef struct {

  /* Mandatory */
  /*  AGPS Type */
  mcm_agps_t_v01 agps_type;
  /**<   AGPS type.*/
}mcm_loc_agps_data_conn_closed_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_loc_qmi_messages
    @{
  */
/** Response Message; Indicates that the AGPS data connection is closed. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;
  /**<   Result code.*/
}mcm_loc_agps_data_conn_closed_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_loc_qmi_messages
    @{
  */
/** Request Message; Indicates that the AGPS data connection failed to start. */
typedef struct {

  /* Mandatory */
  /*  AGPS Type */
  mcm_agps_t_v01 agps_type;
  /**<   AGPS type.*/
}mcm_loc_agps_data_conn_failed_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_loc_qmi_messages
    @{
  */
/** Response Message; Indicates that the AGPS data connection failed to start. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;
  /**<   Result code.*/
}mcm_loc_agps_data_conn_failed_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_loc_qmi_messages
    @{
  */
/** Request Message; Sets the AGPS server. */
typedef struct {

  /* Mandatory */
  /*  AGPS Type */
  mcm_agps_t_v01 agps_type;
  /**<   AGPS type.*/

  /* Mandatory */
  /*  Host Name */
  char host_name[MCM_LOC_MAX_SEVER_ADDR_LENGTH_CONST_V01 + 1];
  /**<   Host name.*/

  /* Mandatory */
  /*  Port */
  uint32_t port;
  /**<   Port.*/
}mcm_loc_agps_set_server_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_loc_qmi_messages
    @{
  */
/** Response Message; Sets the AGPS server. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;
  /**<   Result code.*/
}mcm_loc_agps_set_server_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_loc_qmi_messages
    @{
  */
/** Request Message; Sends a user response for NI. */
typedef struct {

  /* Mandatory */
  /*  Notification ID */
  int32_t notif_id;
  /**<   Notification ID.*/

  /* Mandatory */
  /*  User Response */
  mcm_gps_user_response_t_v01 user_response;
  /**<   User response.*/
}mcm_loc_ni_respond_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_loc_qmi_messages
    @{
  */
/** Response Message; Sends a user response for NI. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;
  /**<   Result code.*/
}mcm_loc_ni_respond_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_loc_qmi_messages
    @{
  */
/** Request Message; Updates the network availability status. */
typedef struct {

  /* Mandatory */
  /*  Available */
  int32_t available;
  /**<   Available.*/

  /* Mandatory */
  /*  APN */
  char apn[MCM_LOC_MAX_APN_NAME_LENGTH_CONST_V01 + 1];
  /**<   APN.*/
}mcm_loc_agps_ril_update_network_availability_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_loc_qmi_messages
    @{
  */
/** Response Message; Updates the network availability status. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;
  /**<   Result code.*/
}mcm_loc_agps_ril_update_network_availability_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_loc_qmi_messages
    @{
  */
/** Indication Message; Indication with the location payload. */
typedef struct {

  /* Mandatory */
  /*  Location Info */
  mcm_gps_location_t_v01 location;
  /**<   Location information.*/
}mcm_loc_location_info_ind_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_loc_qmi_messages
    @{
  */
/** Indication Message; Indication with the location status payload. */
typedef struct {

  /* Mandatory */
  /*  Gps Status */
  mcm_gps_status_t_v01 status;
  /**<   GPS status.*/
}mcm_loc_status_info_ind_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_loc_qmi_messages
    @{
  */
/** Indication Message; Indication with the satellites in view payload. */
typedef struct {

  /* Mandatory */
  /*  Gps SV status */
  mcm_gps_sv_status_t_v01 sv_info;
  /**<   GPS SV status.*/
}mcm_loc_sv_info_ind_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_loc_qmi_messages
    @{
  */
/** Indication Message; Indication with the NMEA string payload. */
typedef struct {

  /* Mandatory */
  /*  Timestamp */
  int64_t timestamp;
  /**<   Timestamp.*/

  /* Mandatory */
  /*  NMEA String */
  char nmea[MCM_LOC_GPS_NMEA_MAX_LENGTH_CONST_V01 + 1];
  /**<   NMEA string.*/

  /* Mandatory */
  /*  Length */
  int32_t length;
  /**<   NMEA string length. @newpagetable*/
}mcm_loc_nmea_info_ind_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_loc_qmi_messages
    @{
  */
/** Indication Message; Indication with the location capabilities. */
typedef struct {

  /* Mandatory */
  /*  Capabilities */
  mcm_gps_capabilities_t_v01 capabilities;
  /**<   Capabilities.*/
}mcm_loc_capabilities_info_ind_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_loc_qmi_messages
    @{
  */
/** Indication Message; Indication request for SNTP time. */
typedef struct {
  /* This element is a placeholder to prevent the declaration of
     an empty struct.  DO NOT USE THIS FIELD UNDER ANY CIRCUMSTANCE */
  char __placeholder;
}mcm_loc_utc_time_req_ind_msg_v01;

  /* Message */
/**
    @}
  */

/** @addtogroup mcm_loc_qmi_messages
    @{
  */
/** Indication Message; Indication request to download XTRA data. */
typedef struct {
  /* This element is a placeholder to prevent the declaration of
     an empty struct.  DO NOT USE THIS FIELD UNDER ANY CIRCUMSTANCE */
  char __placeholder;
}mcm_loc_xtra_data_req_ind_msg_v01;

  /* Message */
/**
    @}
  */

/** @addtogroup mcm_loc_qmi_messages
    @{
  */
/** Indication Message; Indication with the AGPS status. */
typedef struct {

  /* Mandatory */
  /*  AGPS Status */
  mcm_agps_status_t_v01 status;
  /**<   AGPS status.*/
}mcm_loc_agps_status_ind_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_loc_qmi_messages
    @{
  */
/** Indication Message; Indication with the NI notification payload. */
typedef struct {

  /* Mandatory */
  /*  NI Notification */
  mcm_gps_ni_notification_t_v01 notification;
  /**<   NI notification.*/
}mcm_loc_ni_notification_ind_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_loc_qmi_messages
    @{
  */
/** Indication Message; Indication with the reported XTRA server URLs. */
typedef struct {

  /* Mandatory */
  /*  Server1 */
  char server1[MCM_LOC_MAX_SEVER_ADDR_LENGTH_CONST_V01 + 1];
  /**<   server1.*/

  /* Mandatory */
  /*  Server2 */
  char server2[MCM_LOC_MAX_SEVER_ADDR_LENGTH_CONST_V01 + 1];
  /**<   server2.*/

  /* Mandatory */
  /*  Server3 */
  char server3[MCM_LOC_MAX_SEVER_ADDR_LENGTH_CONST_V01 + 1];
  /**<   server3.*/
}mcm_loc_xtra_report_server_ind_msg_v01;  /* Message */
/**
    @}
  */

/* Conditional compilation tags for message removal */
//#define REMOVE_MCM_LOC_AGPS_DATA_CONN_CLOSED_V01
//#define REMOVE_MCM_LOC_AGPS_DATA_CONN_FAILED_V01
//#define REMOVE_MCM_LOC_AGPS_DATA_CONN_OPEN_V01
//#define REMOVE_MCM_LOC_AGPS_RIL_UPDATE_NETWORK_AVAILABILITY_V01
//#define REMOVE_MCM_LOC_AGPS_SET_SERVER_V01
//#define REMOVE_MCM_LOC_AGPS_STATUS_IND_V01
//#define REMOVE_MCM_LOC_CAPABILITIES_INFO_IND_V01
//#define REMOVE_MCM_LOC_DELETE_AIDING_DATA_V01
//#define REMOVE_MCM_LOC_INJECT_LOCATION_V01
//#define REMOVE_MCM_LOC_INJECT_TIME_V01
//#define REMOVE_MCM_LOC_LOCATION_INFO_IND_V01
//#define REMOVE_MCM_LOC_NI_NOTIFICATION_IND_V01
//#define REMOVE_MCM_LOC_NI_RESPOND_V01
//#define REMOVE_MCM_LOC_NMEA_INFO_IND_V01
//#define REMOVE_MCM_LOC_SET_INDICATIONS_V01
//#define REMOVE_MCM_LOC_SET_POSITION_MODE_V01
//#define REMOVE_MCM_LOC_START_NAV_V01
//#define REMOVE_MCM_LOC_STATUS_INFO_IND_V01
//#define REMOVE_MCM_LOC_STOP_NAV_V01
//#define REMOVE_MCM_LOC_SV_INFO_IND_V01
//#define REMOVE_MCM_LOC_UTC_TIME_REQ_IND_V01
//#define REMOVE_MCM_LOC_XTRA_DATA_REQ_IND_V01
//#define REMOVE_MCM_LOC_XTRA_INJECT_DATA_V01
//#define REMOVE_MCM_LOC_XTRA_REPORT_SERVER_IND_V01

/*Service Message Definition*/
/** @addtogroup mcm_loc_qmi_msg_ids
    @{
  */
#define MCM_LOC_SET_INDICATIONS_REQ_V01 0x0300
#define MCM_LOC_SET_INDICATIONS_RESP_V01 0x0300
#define MCM_LOC_SET_POSITION_MODE_REQ_V01 0x0301
#define MCM_LOC_SET_POSITION_MODE_RESP_V01 0x0301
#define MCM_LOC_START_NAV_REQ_V01 0x0302
#define MCM_LOC_START_NAV_RESP_V01 0x0302
#define MCM_LOC_STOP_NAV_REQ_V01 0x0303
#define MCM_LOC_STOP_NAV_RESP_V01 0x0303
#define MCM_LOC_DELETE_AIDING_DATA_REQ_V01 0x0304
#define MCM_LOC_DELETE_AIDING_DATA_RESP_V01 0x0304
#define MCM_LOC_INJECT_TIME_REQ_V01 0x0305
#define MCM_LOC_INJECT_TIME_RESP_V01 0x0305
#define MCM_LOC_INJECT_LOCATION_REQ_V01 0x0306
#define MCM_LOC_INJECT_LOCATION_RESP_V01 0x0306
#define MCM_LOC_XTRA_INJECT_DATA_REQ_V01 0x0307
#define MCM_LOC_XTRA_INJECT_DATA_RESP_V01 0x0307
#define MCM_LOC_AGPS_DATA_CONN_OPEN_REQ_V01 0x0308
#define MCM_LOC_AGPS_DATA_CONN_OPEN_RESP_V01 0x0308
#define MCM_LOC_AGPS_DATA_CONN_CLOSED_REQ_V01 0x0309
#define MCM_LOC_AGPS_DATA_CONN_CLOSED_RESP_V01 0x0309
#define MCM_LOC_AGPS_DATA_CONN_FAILED_REQ_V01 0x030A
#define MCM_LOC_AGPS_DATA_CONN_FAILED_RESP_V01 0x030A
#define MCM_LOC_AGPS_SET_SERVER_REQ_V01 0x030B
#define MCM_LOC_AGPS_SET_SERVER_RESP_V01 0x030B
#define MCM_LOC_NI_RESPOND_REQ_V01 0x030C
#define MCM_LOC_NI_RESPOND_RESP_V01 0x030C
#define MCM_LOC_AGPS_RIL_UPDATE_NETWORK_AVAILABILITY_REQ_V01 0x030D
#define MCM_LOC_AGPS_RIL_UPDATE_NETWORK_AVAILABILITY_RESP_V01 0x030D
#define MCM_LOC_LOCATION_INFO_IND_V01 0x030E
#define MCM_LOC_STATUS_INFO_IND_V01 0x030F
#define MCM_LOC_SV_INFO_IND_V01 0x0310
#define MCM_LOC_NMEA_INFO_IND_V01 0x0311
#define MCM_LOC_CAPABILITIES_INFO_IND_V01 0x0312
#define MCM_LOC_UTC_TIME_REQ_IND_V01 0x0313
#define MCM_LOC_XTRA_DATA_REQ_IND_V01 0x0314
#define MCM_LOC_AGPS_STATUS_IND_V01 0x0315
#define MCM_LOC_NI_NOTIFICATION_IND_V01 0x0316
#define MCM_LOC_XTRA_REPORT_SERVER_IND_V01 0x0317
/**
    @}
  */

/* Service Object Accessor */
/** @addtogroup wms_qmi_accessor
    @{
  */
/** This function is used internally by the autogenerated code.  Clients should use the
   macro mcm_loc_get_service_object_v01( ) that takes in no arguments. */
qmi_idl_service_object_type mcm_loc_get_service_object_internal_v01
 ( int32_t idl_maj_version, int32_t idl_min_version, int32_t library_version );

/** This macro should be used to get the service object */
#define mcm_loc_get_service_object_v01( ) \
          mcm_loc_get_service_object_internal_v01( \
            MCM_LOC_V01_IDL_MAJOR_VERS, MCM_LOC_V01_IDL_MINOR_VERS, \
            MCM_LOC_V01_IDL_TOOL_VERS )
/**
    @}
  */


#ifdef __cplusplus
}
#endif
#endif

