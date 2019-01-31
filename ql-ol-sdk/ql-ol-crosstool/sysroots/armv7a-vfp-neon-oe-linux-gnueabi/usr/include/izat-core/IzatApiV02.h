/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
  Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.
=============================================================================*/
#ifndef IZAT_API_V02_H
#define IZAT_API_V02_H

#include <LocApiV02.h>
#include <IzatApiBase.h>
#include <geofence.h>

namespace lbs_core {
    class LocApiProxyV02;
};

using namespace loc_core;
using namespace lbs_core;

namespace izat_core {

class IzatApiV02 : public IzatApiBase {
    uint32_t mBatchSize;
    bool mBatchingSupported;
    bool mBatchingInProgress;
public:
    IzatApiV02(LocApiProxyV02* locApiProxy);
    inline virtual ~IzatApiV02() {}

    inline virtual void* getSibling() { return (IzatApiBase*)this; }

    /* event callback registered with the loc_api v02 interface */
    virtual int eventCb(locClientHandleType client_handle,
                         uint32_t loc_event_id,
                         locClientEventIndUnionType loc_event_payload);

    // For Geofence
    void GeofenceBreach(const qmiLocEventGeofenceBreachIndMsgT_v02* breachInfo);
    void GeofenceStatus(const qmiLocEventGeofenceGenAlertIndMsgT_v02* alertInfo);
    virtual int addGeofence(GeoFenceData geofenceData, bool needsResponse);
    virtual int removeGeofence(uint32_t hwId, int32_t afwId);
    virtual int pauseGeofence(uint32_t hwId, int32_t afwId);
    virtual int resumeGeofence(uint32_t hwId, int32_t afwId, uint32_t breachMask);
    virtual int modifyGeofence(uint32_t hwId, GeoFenceData geofenceData);
    // set user preference
    virtual int setUserPref(bool user_pref);

    virtual bool getBatchingSupport();
    virtual int cleanUpBatching();
    virtual int getBatchSize(int32_t size);
    virtual int startBatching(int32_t id, const GpsExtBatchOptions& options, int32_t accuracy);
    virtual int updateBatching(int32_t id, const GpsExtBatchOptions& options, int32_t accuracy);
    virtual int stopBatching(int32_t id);
    virtual int getBatchedLocation(int32_t lastNlocations, bool needSaveInAP);

    virtual int injectLocation(GpsExtLocation location);
    GpsExtLocation convertQmiMsg (const qmiLocEventLiveBatchedPositionReportIndMsgT_v02 *location_report_ptr);
    GpsExtLocation* readModemLocations(int32_t lastNlocations, int32_t& numbOfEntries);
};

}  // namespace izat_core

#endif //IZAT_API_V02_H
