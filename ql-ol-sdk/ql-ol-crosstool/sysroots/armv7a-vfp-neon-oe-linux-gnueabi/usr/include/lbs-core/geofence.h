/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
  Copyright (c) 2013-2014 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.
=============================================================================*/
#ifndef GEOFENCE_H
#define GEOFENCE_H

#include <stdint.h>
#include <stdbool.h>

#include <platform_lib_log_util.h>
#include "loc_log.h"

typedef enum {
    ADD_GEOFENCE,
    REMOVE_GEOFENCE,
    PAUSE_GEOFENCE,
    RESUME_GEOFENCE,
    MODIFY_GEOFENCE
} GeofenceResp;

typedef struct {
    uint64_t afwId;
    uint32_t breachMask;
    uint8_t confidence;
    uint32_t responsiveness;
    double latitude;
    double longitude;
    double radius;
    bool paused;
}GeoFenceData;

typedef struct {
    size_t          size;
    uint16_t        flags;
    double          latitude;
    double          longitude;
    double          altitude;
    float           speed;
    float           bearing;
    float           accuracy;
    int64_t         timestamp;
    uint32_t        sources_used;
} GpsExtLocation;

typedef struct {
    int last_transition;
    int monitor_transitions;
    int notification_responsivenes_ms;
    int unknown_timer_ms;
    uint32_t sources_to_use;
} GeofenceExtOptions;

typedef struct {
    double max_power_allocation_mW;
    uint32_t sources_to_use;
    uint32_t flags;
    int64_t period_ns;
} GpsExtBatchOptions;

#endif /* GEOFENCE_H */
