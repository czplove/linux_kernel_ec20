/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
  Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.
=============================================================================*/
#ifndef __LBS_PROXY_H__
#define __LBS_PROXY_H__
#include <LBSProxyBase.h>
#include <LocAdapterBase.h>
#include <pthread.h>

using namespace loc_core;

namespace lbs_core {

class LBSProxy : public LBSProxyBase {
    static pthread_mutex_t mLock;
    static UlpProxyBase* mUlp;
    static LocAdapterBase* mAdapter;
    static unsigned long mCapabilities;
public:
    inline LBSProxy() : LBSProxyBase() {}
    inline ~LBSProxy() {}

    inline virtual void requestUlp(LocAdapterBase* adapter,
                                   unsigned long capabilities) const {
        locRequestUlp(adapter, capabilities);
    }
    virtual LocApiBase* getLocApi(const MsgTask* msgTask,
                                  LOC_API_ADAPTER_EVENT_MASK_T exMask) const;
    inline virtual bool hasAgpsExt() const { return true; }
    inline virtual bool hasCPIExt() const { return true; }

    static void locRequestUlp(LocAdapterBase* adapter,
                              unsigned long capabilities);
    static void ulpRequestLoc(UlpProxyBase* ulp);
};

}  // namespace lbs_core
#endif //__LBS_PROXY_H__
