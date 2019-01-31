/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
  Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.
=============================================================================*/
#ifndef GEOFENCE_ADAPTER_H
#define GEOFENCE_ADAPTER_H

#include <IzatAdapterBase.h>
#include <IzatApiBase.h>
#include <geofence.h>
#include <LocDualContext.h>
#include <map>

using namespace loc_core;
using namespace izat_core;

typedef std::map<uint32_t, GeoFenceData> GeoFencesMap;
typedef std::map<uint64_t, uint32_t> GeoFenceIdMap;

struct MsgGeofenceBreachEvent : public LocMsg {
    uint32_t mClientId;
    int32_t mAfwId;
    GpsExtLocation mLocation;
    int32_t mTransition;
    inline MsgGeofenceBreachEvent(uint32_t clientId, int32_t afwId, const GpsExtLocation& gpsLocation, int32_t transition) :
        LocMsg(), mClientId(clientId), mAfwId(afwId), mLocation(gpsLocation), mTransition(transition) {}
    virtual void proc() const;
};

struct MsgGeofenceStatusEvent : public LocMsg {
    uint64_t mStatus;
    uint32_t mClient;
    inline MsgGeofenceStatusEvent(uint64_t status, uint32_t client) :
        LocMsg(), mStatus(status), mClient(client){}
    virtual void proc() const;
};

struct MsgGeofenceResp : public LocMsg {
    GeofenceResp mResp;
    int32_t mStatus;
    uint32_t mAfwId;
    uint32_t mClientId;
    uint32_t mAdapter;
    uint32_t mHwId;
    uint64_t mCombinedId;
    inline MsgGeofenceResp(GeofenceResp resp,
                          int32_t status,
                          uint32_t afwId,
                          uint32_t clientId,
                          uint32_t adapterId,
                          uint32_t hwId,
                          uint64_t combinedId) :
        LocMsg(),
        mResp(resp),
        mStatus(status),
        mAfwId(afwId),
        mClientId(clientId),
        mAdapter(adapterId),
        mHwId(hwId),
        mCombinedId(combinedId){}
    virtual void proc() const;
};

struct MsgGeofenceSSREvent : public LocMsg {
    uint32_t mAdapter;
    inline MsgGeofenceSSREvent(uint32_t adapterId) :
        LocMsg(), mAdapter(adapterId){}
    virtual void proc() const;
};

class GeofenceAdapter : public IzatAdapterBase {

    static GeofenceAdapter* mMe;
    static const LOC_API_ADAPTER_EVENT_MASK_T mMask;
    GeofenceAdapter(MsgTask::tCreate tCreator);
    GeofenceAdapter(MsgTask::tAssociate tAsso);
    inline virtual ~GeofenceAdapter() {}

public:

    GeoFencesMap mGeoFences; //map geofence hardware id to GeoFenceData
    GeoFenceIdMap mGeoFenceIds; //map afw geofence id to geofence hardware id

    uint32_t mClients[3];
    static GeofenceAdapter* get(uint32_t client, MsgTask::tCreate tCreator);
    static GeofenceAdapter* get(uint32_t client, MsgTask::tAssociate tAsso);
    static pthread_mutex_t mGetAdapterMutex;

    void addGfClients(uint32_t client);
    bool gfBreachEvent(int32_t hwId, GpsExtLocation& gpsLocation, int32_t transition);
    bool gfStatusEvent(uint64_t status);
    bool gfResponse(GeofenceResp resp, int32_t status, uint32_t hwId);

    bool addGeofence(GeoFenceData geofenceData, bool needsResponse);
    bool removeGeofence(uint32_t hwId, int32_t afwId);
    bool pauseGeofence(uint32_t hwId, int32_t afwId);
    bool resumeGeofence(uint32_t hwId, int32_t afwId, uint32_t breachMask);
    bool modifyGeofence(uint32_t hwId, GeoFenceData geofenceData);
    bool saveGeofence(uint32_t hwId, GeoFenceData geofenceData);

    void eraseGeoFenceItem(uint64_t afwId, uint32_t hwId);
    void pauseGeoFenceItem(uint64_t afwId, uint32_t hwId);
    bool modifyGeoFenceItem(uint64_t afwId, uint32_t& hwId);
    void resumeGeoFenceItem(uint64_t afwId, uint32_t hwId);
    bool getAfwIdFromHwId(uint32_t hwId, uint64_t& afwId);
    void dump();
    virtual void handleEngineUpEvent();

};

#endif /* GEOFENCE_ADAPTER_H */
