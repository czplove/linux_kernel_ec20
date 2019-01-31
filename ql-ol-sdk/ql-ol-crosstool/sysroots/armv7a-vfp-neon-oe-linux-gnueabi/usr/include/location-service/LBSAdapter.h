/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
  Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.
=============================================================================*/

#ifndef LBS_ADAPTER_H
#define LBS_ADAPTER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <string.h>
#include <LBSAdapterBase.h>

using namespace loc_core;
using namespace lbs_core;

class LBSCallback {
public:
    LBSCallback() {}
    inline virtual ~LBSCallback() {}
    virtual void locationReport(GpsLocation& location) {};
};

class LBSAdapter : public LBSAdapterBase {
    static LBSAdapter* mMe;
    LBSCallback *mLBSCallbacks;
    virtual void getZppFixSync();
public:
    LBSAdapter(const LOC_API_ADAPTER_EVENT_MASK_T mask);
    inline virtual ~LBSAdapter() {}

    virtual bool requestWps(enum WifiRequestType type);
    virtual bool requestWifiApData();
    virtual void handleEngineUpEvent();

    int cinfoInject(int cid, int lac, int mnc, int mcc, bool roaming);
    int oosInform();
    int niSuplInit(char* supl_init, int length);
    int chargerStatusInject(int status);
    int wifiStatusInform();
    int injectWifiPosition(WifiLocation wifiInfo);
    int injectWifiApInfo(WifiApInfo wifiApInfo);
    int setWifiWaitTimeoutValue(uint8_t timeout);

    // Zpp related
    int getZppFixRequest();

};

#ifdef __cplusplus
}
#endif

#endif /* LBS_ADAPTER_H */
