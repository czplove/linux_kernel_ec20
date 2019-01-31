/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
  Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.
=============================================================================*/
#ifndef LBS_API_V02_H
#define LBS_API_V02_H
#ifdef __cplusplus
extern "C"
{
#endif

#include <LocApiV02.h>
#include <LBSApiBase.h>

//Empty class declaration so that IzatApiV02
//can be declared as a friend of LBSApiV02
namespace izat_core {
    class IzatApiV02;
}

using namespace loc_core;
using namespace izat_core;

namespace lbs_core {

class LBSApiV02 : public LocApiV02, public LBSApiBase {
    //Declaring LocApiProxyV02 as a friend class so that it can have
    //access to clientHandle()
    friend class LocApiProxyV02;
    void WpsEvent(const qmiLocEventWifiReqIndMsgT_v02* event);
public:
    LBSApiV02(const MsgTask* msgTask,
              LOC_API_ADAPTER_EVENT_MASK_T exMask);
    virtual ~LBSApiV02();

    inline virtual void* getSibling() { return (LBSApiBase*)this; }
    inline virtual LocApiProxyBase* getLocApiProxy()
    {
        return mLocApiProxy;
    }

   /* event callback registered with the loc_api v02 interface */
    virtual void eventCb(locClientHandleType client_handle,
                         uint32_t loc_event_id,
                         locClientEventIndUnionType loc_event_payload);

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

#endif //LBS_API_V02_H
