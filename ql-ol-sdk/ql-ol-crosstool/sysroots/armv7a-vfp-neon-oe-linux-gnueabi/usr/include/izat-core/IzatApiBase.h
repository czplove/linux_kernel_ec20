/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
  Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.
============================================================================*/
#ifndef IZAT_API_BASE_H
#define IZAT_API_BASE_H

#include <Wiper.h>
#include <LocApiBase.h>
#include <geofence.h>

using namespace loc_core;

namespace izat_core {

class IzatAdapterBase;

class IzatApiBase {
    IzatAdapterBase* mIzatAdapters[MAX_ADAPTERS];
protected:
    LocApiProxyBase *mLocApiProxy;
public:
    IzatApiBase(LocApiProxyBase *locApiProxy);
    inline virtual ~IzatApiBase() {}

    void addAdapter(IzatAdapterBase* adapter);
    void removeAdapter(IzatAdapterBase* adapter);
    // For Geofence
    void saveGeofence (uint32_t hwId, GeoFenceData geofenceInfo);
    void geofenceBreach(int32_t hwId, GpsExtLocation& gpsLocation, int32_t transition);
    void geofenceStatus(uint64_t status);
    void geofenceResponse(GeofenceResp resp, int32_t status, uint32_t hwId);
    virtual int addGeofence(GeoFenceData geofenceInfo, bool needsResponse);
    virtual int removeGeofence(uint32_t hwId,int32_t afwId);
    virtual int pauseGeofence(uint32_t hwId,int32_t afwId);
    virtual int resumeGeofence(uint32_t hwId, int32_t afwId, uint32_t breachMask);
    virtual int modifyGeofence(uint32_t hwId, GeoFenceData data);
    // set user preference
    virtual int setUserPref(bool user_pref);

    virtual bool getBatchingSupport();
    virtual int cleanUpBatching();
    virtual int getBatchSize(int32_t size);
    virtual int startBatching(int32_t id, const GpsExtBatchOptions& options, int32_t accuracy);
    virtual int updateBatching(int32_t id, const GpsExtBatchOptions& options, int32_t accuracy);
    virtual int stopBatching(int32_t id);
    virtual int getBatchedLocation(int32_t lastNLocations, bool needSaveInAP);
    virtual int injectLocation(GpsExtLocation location);
    void reportLocations(GpsExtLocation* location,
                        int32_t number_query,
                        int32_t last_n_locations,
                        bool needSaveInAP);
    void reportSingleFix(GpsExtLocation& location);
    void readLocationsfromModem(int32_t number, bool needSaveInAP);
    void setModemBatchingSize(int32_t modemBatchSize);
    void clearApBuffer();
};

}  // namespace izat_core

#endif //IZAT_API_BASE_H
