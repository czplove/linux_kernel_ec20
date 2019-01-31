/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

       GeoFencer module

GENERAL DESCRIPTION
  GeoFencer module

  Copyright (c) 2013 Qualcomm Atheros, Inc.
  All Rights Reserved.
  Qualcomm Atheros Confidential and Proprietary.

  Copyright (c) 2012 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential
=============================================================================*/

#ifndef GEOFENCER_H
#define GEOFENCER_H

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <map>
#include "hardware/gps.h"
#include "geofence.h"
#include "GeofenceCallbacks.h"
#include "GeofenceAdapter.h"

typedef void (*gps_ext_set_thread_event)();

class GeoFencer {

public:

    GeofenceCallbacks* mCallbacks;

    // constructor for gps client
    GeoFencer(GeofenceCallbacks* callbacks, gps_create_thread create_thread_cb);

    // constructor for flp client
    GeoFencer(GeofenceCallbacks* callbacks, gps_ext_set_thread_event set_thread_event_cb);

    ~GeoFencer();

    GeofenceAdapter* mAdapter;

    void addCommand(int32_t geofence_id, double latitude,
                   double longitude, double radius_meters,
                   int last_transition, int monitor_transitions,
                   int notification_responsiveness_ms,
                   int unknown_timer_ms);

    void removeCommand(int32_t geofence_id);

    void pauseCommand(int32_t geofence_id);

    void resumeCommand(int32_t geofence_id, int monitor_transitions);

    void modifyCommand(int32_t geofence_id, GeofenceExtOptions* options);

    bool getHwIdFromAfwId(uint64_t afwId, uint32_t& hwId);

    uint64_t combineClinetId(int32_t geofence_id, uint32_t clientId);
};

#endif /* GEOFENCER_H */
