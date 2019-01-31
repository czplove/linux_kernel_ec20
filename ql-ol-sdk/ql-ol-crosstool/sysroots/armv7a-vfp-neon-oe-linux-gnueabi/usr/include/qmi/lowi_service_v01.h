#ifndef LOWI_SERVICE_01_H
#define LOWI_SERVICE_01_H
/**
  @file lowi_service_v01.h

  @brief This is the public header file which defines the lowi service Data structures.

  This header file defines the types and structures that were defined in
  lowi. It contains the constant values defined, enums, structures,
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
  Copyright (c) 2014 Qualcomm Technologies, Inc. All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.


  $Header: //source/qcom/qct/interfaces/qmi/lowi/main/latest/api/lowi_service_v01.h#2 $
 *====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====* 
 *THIS IS AN AUTO GENERATED FILE. DO NOT ALTER IN ANY WAY
 *====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* This file was generated with Tool version 6.13 
   It was generated on: Fri Sep  5 2014 (Spin 1)
   From IDL File: lowi_service_v01.idl */

/** @defgroup lowi_qmi_consts Constant values defined in the IDL */
/** @defgroup lowi_qmi_msg_ids Constant values for QMI message IDs */
/** @defgroup lowi_qmi_enums Enumerated types used in QMI messages */
/** @defgroup lowi_qmi_messages Structures sent as QMI messages */
/** @defgroup lowi_qmi_aggregates Aggregate types used in QMI messages */
/** @defgroup lowi_qmi_accessor Accessor for QMI service object */
/** @defgroup lowi_qmi_version Constant values for versioning information */

#include <stdint.h>
#include "qmi_idl_lib.h"
#include "common_v01.h"


#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup lowi_qmi_version
    @{
  */
/** Major Version Number of the IDL used to generate this file */
#define LOWI_V01_IDL_MAJOR_VERS 0x01
/** Revision Number of the IDL used to generate this file */
#define LOWI_V01_IDL_MINOR_VERS 0x00
/** Major Version Number of the qmi_idl_compiler used to generate this file */
#define LOWI_V01_IDL_TOOL_VERS 0x06
/** Maximum Defined Message ID */
#define LOWI_V01_MAX_MESSAGE_ID 0x0024
/**
    @}
  */


/** @addtogroup lowi_qmi_consts 
    @{ 
  */
#define LOWI_MAX_STRING_LENGTH_V01 255

/**  Maximum frequency allowed in discovery scan  */
#define LOWI_MAX_FREQ_LIST_SIZE_V01 20

/**  Maximum size for the list of AP's to scan  */
#define LOWI_MAX_AP_SSID_LIST_SIZE_V01 20

/**  Maximum SSID length  */
#define LOWI_MAX_SSID_LEN_V01 32

/**  Maximum number of APs allowed to be returned in a scan  */
#define LOWI_MAX_AP_ALLOWED_V01 25

/**  Maximum number of measurements per AP  */
#define LOWI_MAX_MEASUREMENTS_V01 1

/**  Maximum length of the country code  */
#define LOWI_MAX_COUNTRY_CODE_LENGTH_V01 5
#define LOWI_MAX_WLAN_STRING_LEN_V01 31

/**  Maximum interface length  */
#define LOWI_MAX_INTERFACE_LEN_V01 32
/**
    @}
  */

/** @addtogroup lowi_qmi_messages
    @{
  */
/** Response Message; Generic response definition. This message is used to tell
                    clients whether their message was accepted for further
                    processing or rejected. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  qmi_response_type_v01 resp;
}QmiLOWIGenResp_v01;  /* Message */
/**
    @}
  */

/** @addtogroup lowi_qmi_enums
    @{
  */
typedef enum {
  EQMILOWIDISCOVERYSCANTYPE_MIN_ENUM_VAL_V01 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
  LOWI_PASSIVE_SCAN_V01 = 0, /**<  Passive Scan. Waits for beacons on every channel for a specific time  */
  LOWI_ACTIVE_SCAN_V01 = 1, /**<  Active Scan. Sends a probe on the channel and waits to receive probe 
         response on the channel. Time spent on each channel is shorter compared 
         to Passive Scan  */
  EQMILOWIDISCOVERYSCANTYPE_MAX_ENUM_VAL_V01 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}eQmiLowiDiscoveryScanType_v01;
/**
    @}
  */

typedef uint64_t maskQmiLowiDiscoveryRequestBand_v01;
#define LOWI_TWO_POINT_FOUR_GHZ_V01 ((maskQmiLowiDiscoveryRequestBand_v01)0x00000001ull) /**<  2.4 Ghz only  */
#define LOWI_FIVE_GHZ_V01 ((maskQmiLowiDiscoveryRequestBand_v01)0x00000002ull) /**<  5 Ghz only  */
/** @addtogroup lowi_qmi_enums
    @{
  */
typedef enum {
  EQMILOWIDISCOVERYREQUESTMODE_MIN_ENUM_VAL_V01 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
  LOWI_FORCED_FRESH_V01 = 0, /**<  Request to perform a fresh scan. The request will trigger a request
       being sent to Wifi driver for fresh scan  */
  LOWI_FRESH_WITH_THROTTLING_V01 = 1, /**<  Request to perform fresh scan with throttling. If the last fresh
       scan results are relatively fresh then this request will be serviced
       from cache. Criteria to determine freshness is
       current time < last scan time + threshold (defined in config file)
    */
  EQMILOWIDISCOVERYREQUESTMODE_MAX_ENUM_VAL_V01 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}eQmiLowiDiscoveryRequestMode_v01;
/**
    @}
  */

/** @addtogroup lowi_qmi_aggregates
    @{
  */
typedef struct {

  uint8_t isSsidValid;

  uint32_t ssid_len;  /**< Must be set to # of elements in ssid */
  uint8_t ssid[LOWI_MAX_SSID_LEN_V01];
}sQmiLowiSsid_v01;  /* Type */
/**
    @}
  */

/** @addtogroup lowi_qmi_messages
    @{
  */
/** Request Message; LOWI Discovery Scan command. */
typedef struct {

  /* Mandatory */
  /*  Request ID */
  uint64_t requestId;
  /**<   Opaque request ID. Passed back in the response indication. */

  /* Optional */
  /*  Scan Type */
  uint8_t scanType_valid;  /**< Must be set to true if scanType is being passed */
  eQmiLowiDiscoveryScanType_v01 scanType;
  /**<   Passive scan or active scan*/

  /* Optional */
  /*  Request Mode */
  uint8_t requestMode_valid;  /**< Must be set to true if requestMode is being passed */
  eQmiLowiDiscoveryRequestMode_v01 requestMode;
  /**<   Request mode */

  /* Optional */
  /*  RF Band to Scan */
  uint8_t band_valid;  /**< Must be set to true if band is being passed */
  maskQmiLowiDiscoveryRequestBand_v01 band;
  /**<   RF band to scan. */

  /* Optional */
  /*  Measurement Age Filter */
  uint8_t measAgeFilterSec_valid;  /**< Must be set to true if measAgeFilterSec is being passed */
  uint32_t measAgeFilterSec;
  /**<   Measurement age filter in seconds. For cache request, at the time of 
       generating the response for the request, all the measurements which are 
       older than current time - filter age, will be filtered out from the response. 
       So '0' age filter could potentially return no measurements from the cache.
       For fresh scan request, at the time of generating the response for the request, 
       all the measurements which are older than request time to WLAN driver - 
       filter age, will be filtered out from the response.
   */

  /* Optional */
  /*  Fallback Tolerance */
  uint8_t fallbackToleranceSec_valid;  /**< Must be set to true if fallbackToleranceSec is being passed */
  uint32_t fallbackToleranceSec;
  /**<   Fallback tolerance in seconds. Only valid when eRequestMode is CACHE_FALLBACK.
       At the time of evaluating this request, all the requested channels should have last measurement
       timestamp equal to or greater than current time - fallback tolerance. If not, then fallback to
       trigger a fresh scan. Client can provide 0 if they want to trigger a fresh scan as well.
   */

  /* Optional */
  /*  Request Timeout */
  uint8_t requestTimeoutSec_valid;  /**< Must be set to true if requestTimeoutSec is being passed */
  int32_t requestTimeoutSec;
  /**<   Request timeout in seconds. On expiry of this timeout the request will be dropped
       if not processed already. Value should be 0 if the timeout is not valid.
   */

  /* Optional */
  /*  Channel Frequency */
  uint8_t channelFreq_valid;  /**< Must be set to true if channelFreq is being passed */
  uint32_t channelFreq_len;  /**< Must be set to # of elements in channelFreq */
  uint32_t channelFreq[LOWI_MAX_FREQ_LIST_SIZE_V01];
  /**<   Dynamic array containing the frequencies to be scanned.   Number of 
       entries in the vector should not be more than MAX_CHAN_INFO_SIZE as 
       that's the maximum LOWI supports as of now.
   */

  /* Optional */
  /*  SSID List */
  uint8_t ssidList_valid;  /**< Must be set to true if ssidList is being passed */
  uint32_t ssidList_len;  /**< Must be set to # of elements in ssidList */
  sQmiLowiSsid_v01 ssidList[LOWI_MAX_AP_SSID_LIST_SIZE_V01];
  /**<   List of access points SSID's to scan for. If not specified than the scan is not
         restricted to a set of AP's.*/
}QmiLowiDiscoveryRequest_v01;  /* Message */
/**
    @}
  */

/** @addtogroup lowi_qmi_enums
    @{
  */
typedef enum {
  EQMILOWISCANSTATUS_MIN_ENUM_VAL_V01 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
  LOWI_SCAN_STATUS_UNKNOWN_V01 = 0, 
  LOWI_SCAN_STATUS_SUCCESS_V01 = 1, /**<  Measurements were obtained successfully from the WLAN driver.
       Note that SUCCESS does not guarantee that there is at least one
       measurement in this packet. It is possible to have zero measurement
       and a SUCCESS; if there are no APs in the vicinity.  */
  LOWI_SCAN_STATUS_BUSY_V01 = 2, /**<  Indicates that the number of pending clients have reached the maximum  */
  LOWI_SCAN_STATUS_DRIVER_ERROR_V01 = 3, /**<  Unable to initiate request to driver  */
  LOWI_SCAN_STATUS_DRIVER_TIMEOUT_V01 = 4, /**<  Unable to get response from driver  */
  LOWI_SCAN_STATUS_INTERNAL_ERROR_V01 = 5, /**<  There is an internal error condition that causes 
       LOWI unable to provide any measurements  */
  LOWI_SCAN_STATUS_INVALID_REQ_V01 = 6, /**<  Invalid request  */
  LOWI_SCAN_STATUS_NOT_SUPPORTED_V01 = 7, /**<  Request not supported  */
  LOWI_SCAN_STATUS_NO_WIFI_V01 = 8, /**<  Wifi not enabled  */
  EQMILOWISCANSTATUS_MAX_ENUM_VAL_V01 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}eQmiLowiScanStatus_v01;
/**
    @}
  */

/** @addtogroup lowi_qmi_enums
    @{
  */
typedef enum {
  EQMILOWISCANTYPERESPONSE_MIN_ENUM_VAL_V01 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
  LOWI_WLAN_SCAN_TYPE_UNKNOWN_V01 = 0, 
  LOWI_WLAN_SCAN_TYPE_PASSIVE_V01 = 1, 
  LOWI_WLAN_SCAN_TYPE_ACTIVE_V01 = 2, 
  EQMILOWISCANTYPERESPONSE_MAX_ENUM_VAL_V01 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}eQmiLowiScanTypeResponse_v01;
/**
    @}
  */

/** @addtogroup lowi_qmi_enums
    @{
  */
typedef enum {
  EQMILOWINODETYPE_MIN_ENUM_VAL_V01 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
  LOWI_NODE_TYPE_UNKNOWN_V01 = 0, 
  LOWI_ACCESS_POINT_V01 = 1, 
  LOWI_PEER_DEVICE_V01 = 2, 
  EQMILOWINODETYPE_MAX_ENUM_VAL_V01 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}eQmiLowiNodeType_v01;
/**
    @}
  */

/** @addtogroup lowi_qmi_aggregates
    @{
  */
typedef struct {

  uint8_t valid;
  /**<   Indicates if the MSAP information is valid or not */

  uint8_t protocolVersion;

  uint32_t venueHash;

  uint8_t serverIdx;
}sQmiLowiMsapInfo_v01;  /* Type */
/**
    @}
  */

/** @addtogroup lowi_qmi_aggregates
    @{
  */
typedef struct {

  int32_t meas_age;
  /**<   Measurement age. In units of milliseconds, -1 means info not available
   */

  int16_t rssi;
  /**<   Signal strength in 0.5dBm steps. i.e. Signal strength of -10 is -5 in 0.5dBm steps */
}sLOWIDiscoveryMeasurementInfo_v01;  /* Type */
/**
    @}
  */

/** @addtogroup lowi_qmi_aggregates
    @{
  */
typedef struct {

  int32_t lo24;

  int32_t hi24;
}sQmiLowiMacAddress_v01;  /* Type */
/**
    @}
  */

/** @addtogroup lowi_qmi_aggregates
    @{
  */
typedef struct {

  sQmiLowiMacAddress_v01 bssid;
  /**<   BSSID of the Wifi node */

  uint32_t frequency;
  /**<   Frequency in Mhz at which the node is detected */

  uint8_t isSecure;
  /**<   Secure access point or not. */

  eQmiLowiNodeType_v01 type;
  /**<   Type of the Wifi node. */

  sQmiLowiSsid_v01 ssid;
  /**<   SSID. */

  sQmiLowiMsapInfo_v01 msapInfo;
  /**<   MsapInfo - valid if not NULL. */

  int8_t cellPowerLimitdBm;
  /**<   Cell power limit in dBm. Only valid for discovery scan results,
       if available. For ranging scan results will be always 0. */

  uint32_t transmitRate;
  /**<   Transmit rate of the node. 0 indicates not available */

  uint8_t rtt3_supported;
  /**<   Indicates if RTT3 is supported by the node or not */

  uint64_t tsf;
  /**<   Timestamp at which the beacon from the AP was last received */

  uint32_t band_center_freq_1;
  /**<     Band center frequency 1 in MHz */

  uint32_t band_center_freq_2;
  /**<     Band center frequency 2 in MHz */

  uint32_t measurementsInfo_len;  /**< Must be set to # of elements in measurementsInfo */
  sLOWIDiscoveryMeasurementInfo_v01 measurementsInfo[LOWI_MAX_MEASUREMENTS_V01];
  /**<   Dynamic array containing measurement info per Wifi node.
       Discovery scan will only have one measurements where as the
       the vector can contain multiple measurement information for a ranging scan.
   */
}sLowiDiscoveryScanMeasurement_v01;  /* Type */
/**
    @}
  */

/** @addtogroup lowi_qmi_messages
    @{
  */
/** Indication Message; LOWI Discovery Scan command. */
typedef struct {

  /* Mandatory */
  /*  Request ID */
  uint64_t requestId;
  /**<   Opaque Request ID. Passed back in the response indication. */

  /* Optional */
  /*  Scan Type */
  uint8_t scanType_valid;  /**< Must be set to true if scanType is being passed */
  eQmiLowiScanTypeResponse_v01 scanType;
  /**<   Type of measurement */

  /* Optional */
  /*  Scan Status */
  uint8_t scanStatus_valid;  /**< Must be set to true if scanStatus is being passed */
  eQmiLowiScanStatus_v01 scanStatus;
  /**<   Scan Status*/

  /* Optional */
  /*  Scan Measurement */
  uint8_t scanMeasurements_valid;  /**< Must be set to true if scanMeasurements is being passed */
  uint32_t scanMeasurements_len;  /**< Must be set to # of elements in scanMeasurements */
  sLowiDiscoveryScanMeasurement_v01 scanMeasurements[LOWI_MAX_AP_ALLOWED_V01];
  /**<   Dynamic array containing the scan measurements for each AP */

  /* Optional */
  /*  Scanned Frequency List */
  uint8_t scannedFreqList_valid;  /**< Must be set to true if scannedFreqList is being passed */
  uint32_t scannedFreqList_len;  /**< Must be set to # of elements in scannedFreqList */
  uint32_t scannedFreqList[LOWI_MAX_FREQ_LIST_SIZE_V01];
  /**<   Contains the scanned frequency list corresponding to the scan results */

  /* Optional */
  /*  Country Code */
  uint8_t countryCode_valid;  /**< Must be set to true if countryCode is being passed */
  uint32_t countryCode_len;  /**< Must be set to # of elements in countryCode */
  uint8_t countryCode[LOWI_MAX_COUNTRY_CODE_LENGTH_V01];
  /**<   Country code of the access point */

  /* Optional */
  /*  Last Indication */
  uint8_t isLast_valid;  /**< Must be set to true if isLast is being passed */
  uint8_t isLast;
  /**<   Indicates if this is the last indication of the series. true if last,
   false if more to follow */
}QmiLowiDiscoveryResponseInd_v01;  /* Message */
/**
    @}
  */

/** @addtogroup lowi_qmi_messages
    @{
  */
/** Request Message; LOWI capabilities command */
typedef struct {

  /* Mandatory */
  /*  Request ID */
  uint64_t requestId;
  /**<   Opaque Request ID. Passed back in the response indication. */
}QmiLowiCapabilitiesRequest_v01;  /* Message */
/**
    @}
  */

/** @addtogroup lowi_qmi_messages
    @{
  */
/** Indication Message; LOWI capabilities command */
typedef struct {

  /* Mandatory */
  /*  Request ID */
  uint64_t requestId;
  /**<   Opaque Request ID. Passed back in the response indication. */

  /* Optional */
  /*  LOWI LP Version Number */
  uint8_t lowi_lp_version_valid;  /**< Must be set to true if lowi_lp_version is being passed */
  char lowi_lp_version[LOWI_MAX_STRING_LENGTH_V01 + 1];
  /**<   LOWI-LP version number */

  /* Optional */
  /*  Ranging Scan Supported */
  uint8_t rangingScanSupported_valid;  /**< Must be set to true if rangingScanSupported is being passed */
  uint8_t rangingScanSupported;
  /**<   True if Ranging scan is supported*/

  /* Optional */
  /*  Active Scan Supported */
  uint8_t activeScanSupported_valid;  /**< Must be set to true if activeScanSupported is being passed */
  uint8_t activeScanSupported;
  /**<   True if Active scan is supported*/

  /* Optional */
  /*  Passive Scan Supported */
  uint8_t truePassiveScanSupported_valid;  /**< Must be set to true if truePassiveScanSupported is being passed */
  uint8_t truePassiveScanSupported;
  /**<   True if true passive scan is supported*/

  /* Optional */
  /*  Snoop Scan Supported */
  uint8_t snoopScanSupported_valid;  /**< Must be set to true if snoopScanSupported is being passed */
  uint8_t snoopScanSupported;
  /**<   True if snoop scan is supported*/

  /* Optional */
  /*  WLAN Events Supported */
  uint8_t wlanEventsSupported_valid;  /**< Must be set to true if wlanEventsSupported is being passed */
  uint8_t wlanEventsSupported;
  /**<   True if WLAN Events are supported*/

  /* Optional */
  /*  Chip OEM */
  uint8_t chipOEM_valid;  /**< Must be set to true if chipOEM is being passed */
  char chipOEM[LOWI_MAX_WLAN_STRING_LEN_V01 + 1];
  /**<   Null terminated string containing chip manufacturer */

  /* Optional */
  /*  Chip Name */
  uint8_t chipName_valid;  /**< Must be set to true if chipName is being passed */
  char chipName[LOWI_MAX_WLAN_STRING_LEN_V01 + 1];
  /**<   WLAN chipset name */

  /* Optional */
  /*  WLAN Chipset ID */
  uint8_t wlan_chip_id_valid;  /**< Must be set to true if wlan_chip_id is being passed */
  uint32_t wlan_chip_id;
  /**<   WLAN chipset ID */

  /* Optional */
  /*  WiFi Driver Version */
  uint8_t wifiDriverVersion_valid;  /**< Must be set to true if wifiDriverVersion is being passed */
  char wifiDriverVersion[LOWI_MAX_WLAN_STRING_LEN_V01 + 1];
  /**<   Null terminated string containing - Wifi driver host version */

  /* Optional */
  /*  FW Version */
  uint8_t fwVersion_valid;  /**< Must be set to true if fwVersion is being passed */
  char fwVersion[LOWI_MAX_WLAN_STRING_LEN_V01 + 1];
  /**<   Null terminated string containing - FW version */
}QmiLowiCapabilitiesResponseInd_v01;  /* Message */
/**
    @}
  */

/** @addtogroup lowi_qmi_enums
    @{
  */
typedef enum {
  EQMILOWIWLANINTERFACE_MIN_ENUM_VAL_V01 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
  LOWI_DEV_STA_V01 = 0, /**<  WLAN device is in station mode  */
  LOWI_DEV_P2P_CLI_V01 = 1, /**<  WLAN device is in P2P client mode  */
  LOWI_WLAN_DEV_ANY_V01 = 2, /**<  WLAN device is in any mode (used for subscription purpose)  */
  EQMILOWIWLANINTERFACE_MAX_ENUM_VAL_V01 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}eQmiLowiWlanInterface_v01;
/**
    @}
  */

/** @addtogroup lowi_qmi_messages
    @{
  */
/** Request Message; LOWI WLAN Events Subscription Command */
typedef struct {

  /* Mandatory */
  /*  Request ID */
  uint64_t requestId;
  /**<   Opaque Request ID. Passed back in the response indication. */

  /* Mandatory */
  /*  Subscribe / Unsubscribe */
  uint8_t subscribe;
  /**<   1 to subscribe, 0 to unsubscribe */

  /* Optional */
  /*  Interface */
  uint8_t interface_valid;  /**< Must be set to true if interface is being passed */
  eQmiLowiWlanInterface_v01 interface;
  /**<   Interface for which the subscription is needed (p2p / Wifi etc) */
}QmiLowiWlanInterfaceEventsSubscriptionRequest_v01;  /* Message */
/**
    @}
  */

typedef uint64_t maskWlanInterfaceEventType_v01;
#define LOWI_WLAN_ON_OFF_EVENT_V01 ((maskWlanInterfaceEventType_v01)0x00000001ull) 
#define LOWI_WLAN_CONNECTION_EVENT_V01 ((maskWlanInterfaceEventType_v01)0x00000002ull) 
#define LOWI_WLAN_HANDOFF_EVENT_V01 ((maskWlanInterfaceEventType_v01)0x00000004ull) 
/** @addtogroup lowi_qmi_aggregates
    @{
  */
typedef struct {

  sQmiLowiMacAddress_v01 bssid;
  /**<   BSSID of the Wifi node. */

  uint32_t frequency;
  /**<   Frequency in MHz at which the node is detected. */

  sQmiLowiSsid_v01 ssid;
  /**<   SSID of the Wifi node */

  int16_t rssi;
  /**<   Signal strength in 0.5dBm steps. i.e. Signal strength of -10 is -5 in 0.5dBm steps */
}sLowiNodeInfo_v01;  /* Type */
/**
    @}
  */

/** @addtogroup lowi_qmi_messages
    @{
  */
/** Indication Message; LOWI WLAN Events Subscription Command */
typedef struct {

  /* Mandatory */
  /*  Request ID */
  uint64_t requestId;
  /**<   Opaque Request ID. Passed back in the response indication. */

  /* Optional */
  /*  Event Type */
  uint8_t event_valid;  /**< Must be set to true if event is being passed */
  maskWlanInterfaceEventType_v01 event;
  /**<   Indicates the event type */

  /* Optional */
  /*  WLAN On */
  uint8_t wlan_on_valid;  /**< Must be set to true if wlan_on is being passed */
  uint8_t wlan_on;
  /**<   Status of WLAN. 0 indicates off and 1 indicates on. */

  /* Optional */
  /*  WLAN Connection Status */
  uint8_t connected_valid;  /**< Must be set to true if connected is being passed */
  uint8_t connected;
  /**<   Status of WLAN connection. 0 indicates disconnected and 1 indicates connected. */

  /* Optional */
  /*  WLAN Handoff Status */
  uint8_t handoff_valid;  /**< Must be set to true if handoff is being passed */
  uint8_t handoff;
  /**<   Status of WLAN handoff. 0 indicates no handoff and 1 indicates handoff. */

  /* Optional */
  /*  Connected Node Information */
  uint8_t connectedNodeInfo_valid;  /**< Must be set to true if connectedNodeInfo is being passed */
  sLowiNodeInfo_v01 connectedNodeInfo;
  /**<   Information regarding the node with which the device is associated.
   Only available when connected to a wifi node. */

  /* Optional */
  /*  Handoff Node Information */
  uint8_t handoffNodeInfo_valid;  /**< Must be set to true if handoffNodeInfo is being passed */
  sLowiNodeInfo_v01 handoffNodeInfo;
  /**<   Information regarding the node to which the WLAN handoff happened.
   Only available when handoff happened to a wifi node. */
}QmiLowiWlanInterfaceEventsResponseInd_v01;  /* Message */
/**
    @}
  */

/** @addtogroup lowi_qmi_messages
    @{
  */
/** Request Message; LOWI WLAN State Query Command */
typedef struct {

  /* Mandatory */
  /*  Base Request */
  uint64_t requestId;
  /**<   Opaque Request ID. Passed back in the response indication. */

  /* Optional */
  /*  Interface */
  uint8_t interface_valid;  /**< Must be set to true if interface is being passed */
  eQmiLowiWlanInterface_v01 interface;
  /**<   Interface for which the subscription is needed (p2p / Wifi etc) */
}QmiLowiWlanInterfaceEventTriggerRequest_v01;  /* Message */
/**
    @}
  */

/** @addtogroup lowi_qmi_messages
    @{
  */
/** Request Message; LOWI Snoop Scan Subscription Command */
typedef struct {

  /* Mandatory */
  /*  Base Request */
  uint64_t requestId;
  /**<   Opaque Request ID. Passed back in the response indication. */

  /* Mandatory */
  /*  Subscribe / Unsubscribe */
  uint8_t subscribe;
  /**<   1 to subscribe, 0 to unsubscribe */

  /* Mandatory */
  /*  Send Scans Not Sent to APSS. */
  uint8_t sendScansNotSentToApss;
  /**<   1 to only send scans not sent to APSS, 0 to send all scans */

  /* Mandatory */
  /*  Request Expiry Time */
  uint32_t request_timer_sec;
  /**<   Request expires after the request timer runs out */
}QmiLowiSnoopScanSubscriptionRequest_v01;  /* Message */
/**
    @}
  */

/* Conditional compilation tags for message removal */ 
//#define REMOVE_QMI_LOWI_CAPABILITIES_V01 
//#define REMOVE_QMI_LOWI_DISCOVERY_V01 
//#define REMOVE_QMI_LOWI_SNOOP_SCAN_SUBSCRIPTION_V01 
//#define REMOVE_QMI_LOWI_WLAN_INTERFACE_EVENTS_V01 
//#define REMOVE_QMI_LOWI_WLAN_INTERFACE_EVENT_TRIGGER_V01 

/*Service Message Definition*/
/** @addtogroup lowi_qmi_msg_ids
    @{
  */
#define QMI_LOWI_DISCOVERY_REQ_V01 0x0020
#define QMI_LOWI_DISCOVERY_RESP_V01 0x0020
#define QMI_LOWI_DISCOVERY_IND_V01 0x0020
#define QMI_LOWI_CAPABILITIES_REQ_V01 0x0021
#define QMI_LOWI_CAPABILITIES_RESP_V01 0x0021
#define QMI_LOWI_CAPABILITIES_IND_V01 0x0021
#define QMI_LOWI_WLAN_INTERFACE_EVENTS_REQ_V01 0x0022
#define QMI_LOWI_WLAN_INTERFACE_EVENTS_RESP_V01 0x0022
#define QMI_LOWI_WLAN_INTERFACE_EVENTS_IND_V01 0x0022
#define QMI_LOWI_WLAN_INTERFACE_EVENT_TRIGGER_REQ_V01 0x0023
#define QMI_LOWI_WLAN_INTERFACE_EVENT_TRIGGER_RESP_V01 0x0023
#define QMI_LOWI_WLAN_INTERFACE_EVENT_TRIGGER_IND_V01 0x0023
#define QMI_LOWI_SNOOP_SCAN_SUBSCRIPTION_REQ_V01 0x0024
#define QMI_LOWI_SNOOP_SCAN_SUBSCRIPTION_RESP_V01 0x0024
#define QMI_LOWI_SNOOP_SCAN_SUBSCRIPTION_IND_V01 0x0024
/**
    @}
  */

/* Service Object Accessor */
/** @addtogroup wms_qmi_accessor 
    @{
  */
/** This function is used internally by the autogenerated code.  Clients should use the
   macro lowi_get_service_object_v01( ) that takes in no arguments. */
qmi_idl_service_object_type lowi_get_service_object_internal_v01
 ( int32_t idl_maj_version, int32_t idl_min_version, int32_t library_version );
 
/** This macro should be used to get the service object */ 
#define lowi_get_service_object_v01( ) \
          lowi_get_service_object_internal_v01( \
            LOWI_V01_IDL_MAJOR_VERS, LOWI_V01_IDL_MINOR_VERS, \
            LOWI_V01_IDL_TOOL_VERS )
/** 
    @} 
  */


#ifdef __cplusplus
}
#endif
#endif

