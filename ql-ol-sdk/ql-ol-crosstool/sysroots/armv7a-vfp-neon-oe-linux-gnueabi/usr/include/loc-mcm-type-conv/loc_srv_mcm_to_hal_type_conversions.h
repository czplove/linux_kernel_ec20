//=============================================================================
// FILE: loc_srv_mcm_to_hal_type_conversions.h
//
// DESCRIPTION:
// Type conversions from MCM LOC data structures to HAL LOC data structures
//
// Copyright (c) 2013-2014 Qualcomm Technologies, Inc.  All Rights Reserved.
// Qualcomm Technologies Proprietary and Confidential.
//=============================================================================

#ifndef LOC_SRV_MCM_TO_HAL_TYPE_CONVERSIONS_H
#define LOC_SRV_MCM_TO_HAL_TYPE_CONVERSIONS_H

#ifdef __cplusplus
extern "C" {
#endif

GpsPositionMode* loc_srv_conv_mcm_gps_position_mode(mcm_gps_position_mode_t_v01 *from,
                                       GpsPositionMode *to);

GpsPositionRecurrence* loc_srv_conv_mcm_gps_position_recurrence(
                                  mcm_gps_position_recurrence_t_v01 *from,
                                  GpsPositionRecurrence *to);

GpsStatusValue* loc_srv_conv_mcm_gps_status_value(mcm_gps_status_value_t_v01 *from,
                                      GpsStatusValue *to);

GpsLocationFlags* loc_srv_conv_mcm_gps_location_flags(mcm_gps_location_flag_t_v01 *from,
                                        GpsLocationFlags *to);

GpsAidingData* loc_srv_conv_mcm_gps_aiding_data(mcm_gps_aiding_data_t_v01 *from,
                                               GpsAidingData *to);

AGpsExtType* loc_srv_conv_mcm_agps_type(mcm_agps_t_v01 *from, AGpsExtType *to);

AGpsBearerType* loc_srv_conv_mcm_agps_bearer(mcm_agps_bearer_t_v01 *from,
                                 AGpsBearerType *to);

GpsNiType* loc_srv_conv_mcm_gps_ni(mcm_gps_ni_t_v01 *from, GpsNiType *to);

GpsNiNotifyFlags* loc_srv_conv_mcm_gps_ni_notify_flags(
                                      mcm_gps_ni_notify_flags_t_v01 *from,
                                      GpsNiNotifyFlags *to);

GpsUserResponseType* loc_srv_conv_mcm_gps_user_response(
                                       mcm_gps_user_response_t_v01 *from,
                                       GpsUserResponseType *to);

GpsNiEncodingType* loc_srv_conv_mcm_gps_ni_encoding(
                                     mcm_gps_ni_encoding_t_v01 *from,
                                     GpsNiEncodingType *to);

AGpsStatusValue* loc_srv_conv_mcm_agps_status_value(
                                       mcm_agps_status_value_t_v01 *from,
                                       AGpsStatusValue *to);

GpsLocation* loc_srv_conv_mcm_gps_location(
                                  mcm_gps_location_t_v01 *from,
                                  GpsLocation *to);

GpsStatus* loc_srv_conv_mcm_gps_status(mcm_gps_status_t_v01 *from,
                                      GpsStatus *to);

GpsSvInfo* loc_srv_conv_mcm_gps_sv_info(mcm_gps_sv_info_t_v01 *from,
                                 GpsSvInfo  *to);

GpsSvStatus* loc_srv_conv_mcm_gps_sv_status(mcm_gps_sv_status_t_v01 *from,
                                   GpsSvStatus *to);

AGpsStatus* loc_srv_conv_mcm_agps_status(mcm_agps_status_t_v01 *from,
                                 AGpsExtStatus *to);

GpsNiNotification* loc_srv_conv_mcm_gps_ni_notification(
                                 mcm_gps_ni_notification_t_v01 *from,
                                 GpsNiNotification *to);

uint32_t* loc_srv_conv_mcm_gps_capabilities(mcm_gps_capabilities_t_v01 *from,
                                      uint32_t *to);

uint16_t* loc_srv_conv_mcm_gps_position_source(
                                      mcm_gps_position_source_t_v01 *from,
                                      uint16_t *to);

#ifdef __cplusplus
}
#endif

#endif // LOC_SRV_MCM_TO_HAL_TYPE_CONVERSIONS_H
