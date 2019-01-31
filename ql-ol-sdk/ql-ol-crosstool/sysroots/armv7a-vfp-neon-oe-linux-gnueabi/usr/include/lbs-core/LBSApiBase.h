/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
  Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.
=============================================================================*/
#ifndef LBS_API_BASE_H
#define LBS_API_BASE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <Wiper.h>
#include <LocApiBase.h>
#define MAX_ADAPTERS 10

using namespace loc_core;

namespace lbs_core {

class LBSAdapterBase;

class LBSApiBase {
    LBSAdapterBase *mLBSAdapters[MAX_ADAPTERS];
protected:
    LocApiProxyBase* mLocApiProxy;
public:
    LBSApiBase(LocApiProxyBase* locApiProxy);
    inline virtual ~LBSApiBase() {}

    void addAdapter(LBSAdapterBase *adapter);
    void removeAdapter(LBSAdapterBase *adapter);

    void requestWps(enum WifiRequestType type);
    void requestWifiApData();

    virtual int cinfoInject(int cid, int lac, int mnc,
                            int mcc, bool roaming);
    virtual int oosInform();
    virtual int niSuplInit(char* supl_init, int length);
    virtual int chargerStatusInject(int status);
    virtual int wifiStatusInform();
    virtual int injectWifiPosition(WifiLocation wifiInfo);
    virtual int injectWifiApInfo(WifiApInfo wifiApInfo);
    virtual int setWifiWaitTimeoutValue(uint8_t timeout);
    virtual int shutdown();
};
}; //namespace lbs_core
#ifdef __cplusplus
}
#endif
#endif //LBS_API_BASE_H
