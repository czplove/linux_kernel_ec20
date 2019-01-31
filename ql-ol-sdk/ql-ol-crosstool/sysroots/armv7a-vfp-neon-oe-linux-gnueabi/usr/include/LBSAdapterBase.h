/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
  Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.
=============================================================================*/
#ifndef LBS_ADAPTER_BASE_H
#define LBS_ADAPTER_BASE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <Wiper.h>
#include <LocAdapterBase.h>
#include <LBSApiBase.h>

using namespace loc_core;
using namespace lbs_core;
namespace lbs_core {

class LBSAdapterBase : public LocAdapterBase {
protected:
    LBSApiBase *mLBSApi;
    LBSAdapterBase(const LOC_API_ADAPTER_EVENT_MASK_T mask,
                   ContextBase* context);
    virtual ~LBSAdapterBase();
public:
virtual bool requestWps(enum WifiRequestType type);
virtual bool requestWifiApData();
};
};

#ifdef __cplusplus
}
#endif

#endif // LBS_ADAPTER_BASE_H
