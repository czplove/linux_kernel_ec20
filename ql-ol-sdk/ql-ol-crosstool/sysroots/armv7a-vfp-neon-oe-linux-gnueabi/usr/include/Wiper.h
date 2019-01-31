/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
  Copyright (c) 2013-2014 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.
=============================================================================*/
#ifndef WIPER_H
#define WIPER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>

#if USE_GLIB
#include <stdint.h>
#endif

#define MAX_REPORTED_APS 50
#define MAC_ADDRESS_LENGTH 6

#define WIFI_APDATA_MASK_AP_RSSI 0x10
#define WIFI_APDATA_MASK_AP_CHANNEL 0x20

typedef struct {

  int apLen;
  /* Represents whether access point len*/
  uint8_t mac_address[MAX_REPORTED_APS * MAC_ADDRESS_LENGTH];
  /*  Represents mac address of the wifi access point*/
  uint32_t rssi[MAX_REPORTED_APS];
  /*  Represents received signal strength indicator of the wifi access point*/
  uint16_t channel[MAX_REPORTED_APS];
  /*  Represents  wiFi channel on which a beacon was received of the wifi access point */

} WifiApInfo;

typedef struct {

  unsigned char positionValid;
  /* Represents info on whether position is valid */
  double          latitude;
  /* Represents latitude in degrees */
  double          longitude;
  /* Represents longitude in degrees */
  float           accuracy;
  /* Represents expected accuracy in meters */
  int fixError;
  /* Represents Wifi position error code */
  unsigned char numApsUsed;
  /* Represents  number of Access Points (AP) used to generate a fix */
  unsigned char apInfoValid;
  /* Represents whether access point info is valid*/
  WifiApInfo apInfo;
  /* Represents access point information */

} WifiLocation;


/* Wifi request types from modem*/
enum WifiRequestType
{
   HIGH = 0,
   LOW = 1,
   STOP = 2,
   UNKNOWN
};

#ifdef __cplusplus
}
#endif

#endif /* WIPER_H */
