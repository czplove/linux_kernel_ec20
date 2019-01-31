#ifndef MCM_RIL_SERVICE_SERVICE_01_H
#define MCM_RIL_SERVICE_SERVICE_01_H
/**
  @file mcm_service_object_v01.h

  @brief This is the public header file which defines the mcm_ril_service service Data structures.

  This header file defines the types and structures that were defined in
  mcm_ril_service. It contains the constant values defined, enums, structures,
  messages, and service message IDs (in that order) Structures that were
  defined in the IDL as messages contain mandatory elements, optional
  elements, a combination of mandatory and optional elements (mandatory
  always come before optionals in the structure), or nothing (null message)

  An optional element in a message is preceded by a uint8_t value that must be
  set to true if the element is going to be included. When decoding a received
  message, the uint8_t values will be set to true or false by the decode
  routine, and should be checked before accessing the values that they
  correspond to.

  Variable sized arrays are defined as static sized arrays with an unsigned
  integer (32 bit) preceding it that must be set to the number of elements
  in the array that are valid. For Example:

  uint32_t test_opaque_len;
  uint8_t test_opaque[16];

  If only 4 elements are added to test_opaque[] then test_opaque_len must be
  set to 4 before sending the message.  When decoding, the _len value is set
  by the decode routine and should be checked so that the correct number of
  elements in the array will be accessed.

*/
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
  Copyright (c) 2013-2014 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential. 

  $Header$
 *====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
 *THIS IS AN AUTO GENERATED FILE. DO NOT ALTER IN ANY WAY
 *====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* This file was generated with Tool version 6.5
   It was generated on: Wed Nov 20 2013 (Spin )
   From IDL File: mcm_service_object_v01.idl */

/** @defgroup mcm_ril_service_qmi_consts Constant values defined in the IDL */
/** @defgroup mcm_ril_service_qmi_msg_ids Constant values for QMI message IDs */
/** @defgroup mcm_ril_service_qmi_enums Enumerated types used in QMI messages */
/** @defgroup mcm_ril_service_qmi_messages Structures sent as QMI messages */
/** @defgroup mcm_ril_service_qmi_aggregates Aggregate types used in QMI messages */
/** @defgroup mcm_ril_service_qmi_accessor Accessor for QMI service object */
/** @defgroup mcm_ril_service_qmi_version Constant values for versioning information */

#include <stdint.h>
#include "qmi_idl_lib.h"
#include "mcm_voice_v01.h"
#include "mcm_common_v01.h"
#include "mcm_sms_v01.h"
#include "mcm_nw_v01.h"
#include "mcm_dm_v01.h"
#include "mcm_client_v01.h"


#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup mcm_ril_service_qmi_version
    @{
  */
/** Major Version Number of the IDL used to generate this file */
#define MCM_RIL_SERVICE_V01_IDL_MAJOR_VERS 0x01
/** Revision Number of the IDL used to generate this file */
#define MCM_RIL_SERVICE_V01_IDL_MINOR_VERS 0x01
/** Major Version Number of the qmi_idl_compiler used to generate this file */
#define MCM_RIL_SERVICE_V01_IDL_TOOL_VERS 0x06
/** Maximum Defined Message ID */
#define MCM_RIL_SERVICE_V01_MAX_MESSAGE_ID 0x101D;
/**
    @}
  */
/** @addtogroup mcm_data_qmi_version
    @{
  */
/** Major Version Number of the IDL used to generate this file */
#define MCM_DATA_V01_IDL_MAJOR_VERS 0x01
/** Revision Number of the IDL used to generate this file */
#define MCM_DATA_V01_IDL_MINOR_VERS 0x01
/** Major Version Number of the qmi_idl_compiler used to generate this file */
#define MCM_DATA_V01_IDL_TOOL_VERS 0x06
/** Maximum Defined Message ID */
#define MCM_DATA_V01_MAX_MESSAGE_ID 0x010C;
/**
    @}
  */


/** @addtogroup mcm_sim_qmi_version
    @{
  */
/** Major Version Number of the IDL used to generate this file */
#define MCM_SIM_V01_IDL_MAJOR_VERS 0x01
/** Revision Number of the IDL used to generate this file */
#define MCM_SIM_V01_IDL_MINOR_VERS 0x01
/** Major Version Number of the qmi_idl_compiler used to generate this file */
#define MCM_SIM_V01_IDL_TOOL_VERS 0x06
/** Maximum Defined Message ID */
#define MCM_SIM_V01_MAX_MESSAGE_ID 0x090E;
/**
    @}
  */

/** @addtogroup mcm_mobileap_qmi_version
    @{
  */
/** Major Version Number of the IDL used to generate this file */
#define MCM_MOBILEAP_V01_IDL_MAJOR_VERS 0x01
/** Revision Number of the IDL used to generate this file */
#define MCM_MOBILEAP_V01_IDL_MINOR_VERS 0x01
/** Major Version Number of the qmi_idl_compiler used to generate this file */
#define MCM_MOBILEAP_V01_IDL_TOOL_VERS 0x06
/** Maximum Defined Message ID */
#define MCM_MOBILEAP_V01_MAX_MESSAGE_ID 0x0427;
/**
    @}
  */


/** @addtogroup mcm_loc_qmi_version
    @{
  */
/** Major Version Number of the IDL used to generate this file */
#define MCM_LOC_V01_IDL_MAJOR_VERS 0x01
/** Revision Number of the IDL used to generate this file */
#define MCM_LOC_V01_IDL_MINOR_VERS 0x01
/** Major Version Number of the qmi_idl_compiler used to generate this file */
#define MCM_LOC_V01_IDL_TOOL_VERS 0x06
/** Maximum Defined Message ID */
#define MCM_LOC_V01_MAX_MESSAGE_ID 0x0317
/**
    @}
  */


/** @addtogroup mcm_atcop_qmi_version
    @{
  */
/** Major Version Number of the IDL used to generate this file */
#define MCM_ATCOP_V01_IDL_MAJOR_VERS 0x01
/** Revision Number of the IDL used to generate this file */
#define MCM_ATCOP_V01_IDL_MINOR_VERS 0x01
/** Major Version Number of the qmi_idl_compiler used to generate this file */
#define MCM_ATCOP_V01_IDL_TOOL_VERS 0x06
/** Maximum Defined Message ID */
#define MCM_ATCOP_V01_MAX_MESSAGE_ID 0x0600;
/**
    @}
  */

/** @addtogroup mcm_client_qmi_version
    @{
  */
/** Major Version Number of the IDL used to generate this file */
#define MCM_CLIENT_V01_IDL_MAJOR_VERS 0x01
/** Revision Number of the IDL used to generate this file */
#define MCM_CLIENT_V01_IDL_MINOR_VERS 0x01
/** Major Version Number of the qmi_idl_compiler used to generate this file */
#define MCM_CLIENT_V01_IDL_TOOL_VERS 0x06
/** Maximum Defined Message ID */
#define MCM_CLIENT_V01_MAX_MESSAGE_ID 0x0802
/**
    @}
  */

extern const qmi_idl_type_table_object mcm_common_qmi_idl_type_table_object_v01;
extern const qmi_idl_type_table_object mcm_dm_qmi_idl_type_table_object_v01;
extern const qmi_idl_type_table_object mcm_nw_qmi_idl_type_table_object_v01;
extern const qmi_idl_type_table_object mcm_sms_qmi_idl_type_table_object_v01;
extern const qmi_idl_type_table_object mcm_voice_qmi_idl_type_table_object_v01;

/* Service Object Accessor */
/** @addtogroup sms_qmi_accessor
 @{
*/
/** This function is used internally by the autogenerated code.  Clients should use the
  macro mcm_sim_get_service_object_v01( ) that takes in no arguments. */
qmi_idl_service_object_type mcm_sim_get_service_object_internal_v01
( int32_t idl_maj_version, int32_t idl_min_version, int32_t library_version );

/** This macro should be used to get the service object */
#define mcm_sim_get_service_object_v01( ) \
     mcm_sim_get_service_object_internal_v01( \
     MCM_SIM_V01_IDL_MAJOR_VERS, MCM_SIM_V01_IDL_MINOR_VERS, \
     MCM_SIM_V01_IDL_TOOL_VERS )
/**
  @}
  */



/* Service Object Accessor */
/** @addtogroup wms_qmi_accessor
    @{
  */
/** This function is used internally by the autogenerated code.  Clients should use the
   macro mcm_ril_service_get_service_object_v01( ) that takes in no arguments. */
qmi_idl_service_object_type mcm_ril_service_get_service_object_internal_v01
 ( int32_t idl_maj_version, int32_t idl_min_version, int32_t library_version );

/** This macro should be used to get the service object */
#define mcm_ril_service_get_service_object_v01( ) \
          mcm_ril_service_get_service_object_internal_v01( \
            MCM_RIL_SERVICE_V01_IDL_MAJOR_VERS, MCM_RIL_SERVICE_V01_IDL_MINOR_VERS, \
            MCM_RIL_SERVICE_V01_IDL_TOOL_VERS )
/**
    @}
  */

/* Service Object Accessor */
/** @addtogroup sms_qmi_accessor
    @{
  */
/** This function is used internally by the autogenerated code.  Clients should use the
   macro mcm_mobileap_get_service_object_v01( ) that takes in no arguments. */
qmi_idl_service_object_type mcm_mobileap_get_service_object_internal_v01
 ( int32_t idl_maj_version, int32_t idl_min_version, int32_t library_version );

/** This macro should be used to get the service object */
#define mcm_mobileap_get_service_object_v01( ) \
          mcm_mobileap_get_service_object_internal_v01( \
            MCM_MOBILEAP_V01_IDL_MAJOR_VERS, MCM_MOBILEAP_V01_IDL_MINOR_VERS, \
            MCM_MOBILEAP_V01_IDL_TOOL_VERS )
/**
    @}
  */

/* Service Object Accessor */
/** @addtogroup sms_qmi_accessor
    @{
  */
/** This function is used internally by the autogenerated code.  Clients should use the
   macro mcm_data_get_service_object_v01( ) that takes in no arguments. */
qmi_idl_service_object_type mcm_data_get_service_object_internal_v01
 ( int32_t idl_maj_version, int32_t idl_min_version, int32_t library_version );

/** This macro should be used to get the service object */
#define mcm_data_get_service_object_v01( ) \
          mcm_data_get_service_object_internal_v01( \
            MCM_DATA_V01_IDL_MAJOR_VERS, MCM_DATA_V01_IDL_MINOR_VERS, \
            MCM_DATA_V01_IDL_TOOL_VERS )
/**
    @}
  */


/* Service Object Accessor */
/** @addtogroup wms_qmi_accessor
    @{
  */
/** This function is used internally by the autogenerated code.  Clients should use the
   macro mcm_loc_get_service_object_v01( ) that takes in no arguments. */
qmi_idl_service_object_type mcm_loc_get_service_object_internal_v01
 ( int32_t idl_maj_version, int32_t idl_min_version, int32_t library_version );

/** This macro should be used to get the service object */
#define mcm_loc_get_service_object_v01( ) \
          mcm_loc_get_service_object_internal_v01( \
            MCM_LOC_V01_IDL_MAJOR_VERS, MCM_LOC_V01_IDL_MINOR_VERS, \
            MCM_LOC_V01_IDL_TOOL_VERS )
/**
    @}
  */

 /* Service Object Accessor */
 /** @addtogroup atcop_qmi_accessor
    @{
  */
/** This function is used internally by the autogenerated code.  Clients should use the
   macro mcm_atcop_get_service_object_v01( ) that takes in no arguments. */
qmi_idl_service_object_type mcm_atcop_get_service_object_internal_v01
 ( int32_t idl_maj_version, int32_t idl_min_version, int32_t library_version );

/** This macro should be used to get the service object */
#define mcm_atcop_get_service_object_v01( ) \
          mcm_atcop_get_service_object_internal_v01( \
            MCM_ATCOP_V01_IDL_MAJOR_VERS, MCM_ATCOP_V01_IDL_MINOR_VERS, \
            MCM_ATCOP_V01_IDL_TOOL_VERS )
/**
    @}
  */


/* Service Object Accessor */
/** @addtogroup wms_qmi_accessor
    @{
  */
/** This function is used internally by the autogenerated code.  Clients should use the
   macro mcm_client_get_service_object_v01( ) that takes in no arguments. */
qmi_idl_service_object_type mcm_client_get_service_object_internal_v01
 ( int32_t idl_maj_version, int32_t idl_min_version, int32_t library_version );

/** This macro should be used to get the service object */
#define mcm_client_get_service_object_v01( ) \
          mcm_client_get_service_object_internal_v01( \
            MCM_CLIENT_V01_IDL_MAJOR_VERS, MCM_CLIENT_V01_IDL_MINOR_VERS, \
            MCM_CLIENT_V01_IDL_TOOL_VERS )
/**
    @}
  */

#ifdef __cplusplus
}
#endif
#endif
