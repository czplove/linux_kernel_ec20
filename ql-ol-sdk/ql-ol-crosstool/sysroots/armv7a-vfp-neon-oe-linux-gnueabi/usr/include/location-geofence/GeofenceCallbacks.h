/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
  Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.
=============================================================================*/
#ifndef GEOFENCER_CALLBACKS_H
#define GEOFENCER_CALLBACKS_H

class GeofenceCallbacks {

public:

    GeofenceCallbacks() {}
    inline virtual ~GeofenceCallbacks() {}

    virtual void geofence_transition_callback (int32_t geofence_id,
                                              GpsExtLocation* location,
                                              int32_t transition,
                                              int64_t timestamp,
                                              uint32_t sources_used) = 0;
    virtual void geofence_status_callback (int32_t status,
                                          uint32_t source,
                                          GpsExtLocation* last_location) = 0;
    virtual void geofence_add_callback (int32_t geofence_id, int32_t result) = 0;
    virtual void geofence_remove_callback (int32_t geofence_id, int32_t result) = 0;
    virtual void geofence_pause_callback (int32_t geofence_id, int32_t result) = 0;
    virtual void geofence_resume_callback (int32_t geofence_id, int32_t result) = 0;
};

#endif /* GEOFENCER_CALLBACKS_H */
