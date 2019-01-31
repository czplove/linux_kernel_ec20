/**
  @file
  dsi_netctrl_qos.h

  @brief
  This file provides an API to manage Quality of Service elements.

*/

/*===========================================================================

  Copyright (c) 2011, 2013 Qualcomm Technologies, Inc. All Rights Reserved

  Qualcomm Technologies Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies, Inc. and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies, Inc.

===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header:  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/23/13   leo     (Tech Pubs) Edited/added Doxygen comments and markup.
04/14/11   ar      Initial version

===========================================================================*/

#ifndef DSI_NETCTRL_QOS_H
#define DSI_NETCTRL_QOS_H

#include <linux/socket.h> /* sockaddr_storage */

/*===========================================================================
                           DECLARATIONS
===========================================================================*/

#define QMI_QOS_MAX_FLOW_FILTER         16
#define QMI_QOS_MAX_FLOW_EVENTS         8
#define QMI_QOS_MAX_PROFILES            8
#define QMI_QOS_IPV6_ADDR_SIZE_IN_BYTES 16

#define DSI_QOS_REQUEST                 (QMI_QOS_REQUEST)
#define DSI_QOS_CONFIGURE               (QMI_QOS_CONFIGURE)

#define DSI_QOS_MAX_FLOW_FILTERS        (QMI_QOS_MAX_FLOW_FILTER)
#define DSI_QOS_MAX_PROFILES            (QMI_QOS_MAX_PROFILES)


/* PARAM Masks */
#define QMI_QOS_UMTS_FLOW_PARAM_TRAFFIC_CLASS      0x0001
#define QMI_QOS_UMTS_FLOW_PARAM_DATA_RATE          0x0002

#define QMI_QOS_CDMA_FLOW_PARAM_PROFILE_ID         0x0001

#define QMI_QOS_FILTER_PARAM_SRC_ADDR             0x00000001
#define QMI_QOS_FILTER_PARAM_DEST_ADDR            0x00000002
#define QMI_QOS_FILTER_PARAM_TRANS_PROTOCOL       0x00000004

#define QMI_QOS_FILTER_PARAM_TCP_SRC_PORTS        0x00000010
#define QMI_QOS_FILTER_PARAM_TCP_DEST_PORTS       0x00000020
#define QMI_QOS_FILTER_PARAM_UDP_SRC_PORTS        0x00000040
#define QMI_QOS_FILTER_PARAM_UDP_DEST_PORTS       0x00000080


typedef unsigned char qmi_qos_ipv6_addr_type[QMI_QOS_IPV6_ADDR_SIZE_IN_BYTES];

/* QOS deinitions */
typedef enum
{
  QMI_QOS_TRANS_PROT_TCP     = 0x06,
  QMI_QOS_TRANS_PROT_UDP     = 0x11,
  QMI_QOS_TRANS_PROT_TCP_UDP = 0xFD
} qmi_qos_trans_protocol_filter_type;

typedef enum
{
  QMI_QOS_IP_VERSION_4 = 0x04,
  QMI_QOS_IP_VERSION_6 = 0x06
} qmi_qos_ip_version_type;

typedef unsigned long    dsi_qos_id_type;

/** @addtogroup datatypes
@{ */

/** Event status types associated with QoS operations. These are
 *  included along with DSI_EVT_QOS_STATUS_IND. */
typedef enum
{
  DSI_QOS_ACTIVATED_EV          = 0x01,  /**< QoS flow is activated. */
  DSI_QOS_SUSPENDED_EV          = 0x02,  /**< QoS flow is suspended. */
  DSI_QOS_GONE_EV               = 0x03,  /**< QoS flow is released. */
  DSI_QOS_MODIFY_ACCEPTED_EV    = 0x04,  /**< QoS flow modify operation is successful. */
  DSI_QOS_MODIFY_REJECTED_EV    = 0x05,  /**< QoS flow modify operation is rejected. */
  DSI_QOS_INFO_CODE_UPDATED_EV  = 0x06,  /**< New information code is available
                                              regarding the QoS status. */
  DSI_QOS_FLOW_ENABLED_EV       = 0x10,  /**< QoS data path flow is enabled. */
  DSI_QOS_FLOW_DISABLED_EV      = 0x11   /**< QoS data path flow is disabled. */
} dsi_qos_status_event_type;

/** @} */ /* end_addtogroup datatypes */

typedef enum
{
  DSI_QOS_FLOW_TYPE_INVALID  = -1,
  DSI_QOS_FLOW_TYPE_UE_INIT  =  0,
  DSI_QOS_FLOW_TYPE_NW_INIT  =  1
} dsi_qos_flow_type;

typedef enum
{
  DSI_QOS_INVALID_PARAMS                          = 0x01,
  DSI_QOS_INTERNAL_CALL_ENDED                     = 0x02,
  DSI_QOS_INTERNAL_ERROR                          = 0x03,
  DSI_QOS_INSUFFICIENT_LOCAL_RESOURCES            = 0x04,
  DSI_QOS_TIMED_OUT_OPERATION                     = 0x05,
  DSI_QOS_INTERNAL_UNKNOWN_CAUSE_CODE             = 0x06,
  DSI_QOS_INTERNAL_MODIFY_IN_PROGRESS             = 0x07,
  DSI_QOS_NOT_SUPPORTED                           = 0x08,
  DSI_QOS_NOT_AVAILABLE                           = 0x09,
  DSI_QOS_NOT_GUARANTEED                          = 0x0A,
  DSI_QOS_INSUFFICIENT_NETWORK_RESOURCES          = 0x0B,
  DSI_QOS_AWARE_SYSTEM                            = 0x0C,
  DSI_QOS_UNAWARE_SYSTEM                          = 0x0D,
  DSI_QOS_REJECTED_OPERATION                      = 0x0E,
  DSI_QOS_WILL_GRANT_WHEN_QOS_RESUMED             = 0x0F,
  DSI_QOS_NETWORK_CALL_ENDED                      = 0x10,
  DSI_QOS_NETWORK_SERVICE_NOT_AVAILABLE           = 0x11,
  DSI_QOS_NETWORK_L2_LINK_RELEASED                = 0x12,
  DSI_QOS_NETWORK_L2_LINK_REESTAB_REJ             = 0x13,
  DSI_QOS_NETWORK_L2_LINK_REESTAB_IND             = 0x14,
  DSI_QOS_NETWORK_UNKNOWN_CAUSE_CODE              = 0x15,
  DSI_QOS_NETWORK_BUSY                            = 0x16,
  DSI_QOS_NETWORK_DISJOINT_PROFILE_SET_SUGGESTED  = 0x17,
  DSI_QOS_NETWORK_NULL_PROFILE_SUGGESTED          = 0x18,
  DSI_QOS_NETWORK_UE_NOT_AUTHORIZED               = 0x19
} dsi_qmi_qos_reason_code_type;

/* typedefs related to flow descriptors */
typedef enum
{
  QMI_QOS_UMTS_TC_CONVERSATIONAL = 0,
  QMI_QOS_UMTS_TC_STREAMING      = 1,
  QMI_QOS_UMTS_TC_INTERACTIVE    = 2,
  QMI_QOS_UMTS_TC_BACKGROUND     = 3
} qmi_qos_umts_traffic_class_type;

typedef enum
{
  QMI_QOS_UMTS_BER_5X10_NEG2 = 0,
  QMI_QOS_UMTS_BER_1X10_NEG2 = 1,
  QMI_QOS_UMTS_BER_5X10_NEG3 = 2,
  QMI_QOS_UMTS_BER_4X10_NEG3 = 3,
  QMI_QOS_UMTS_BER_1X10_NEG3 = 4,
  QMI_QOS_UMTS_BER_1X10_NEG4 = 5,
  QMI_QOS_UMTS_BER_1X10_NEG5 = 6,
  QMI_QOS_UMTS_BER_1X10_NEG6 = 7,
  QMI_QOS_UMTS_BER_6X10_NEG8 = 8
} qmi_qos_umts_residual_ber_type;


typedef enum
{
  QMI_QOS_UMTS_PRIO_1 = 0,
  QMI_QOS_UMTS_PRIO_2 = 1,
  QMI_QOS_UMTS_PRIO_3 = 2
} qmi_qos_umts_handling_prio_type;


typedef enum
{
  QMI_QOS_UMTS_TECH_TYPE,
  QMI_QOS_CDMA_TECH_TYPE,
  QMI_QOS_DVBH_TECH_TYPE,
  QMI_QOS_MBMS_TECH_TYPE,
  QMI_QOS_BCMCS_TECH_TYPE,
  QMI_QOS_MEDIAFLO_TECH_TYPE,
  QMI_QOS_INVALID_TECH_TYPE = 0xFFFF
}qmi_qos_technology_type;

typedef int             qmi_qos_dss_errno;

typedef struct
{
  unsigned short              param_mask;
  qmi_qos_dss_errno           dss_errno;
}qmi_qos_dss_errno_type;


typedef struct
{
  unsigned long max_rate;
  unsigned long guaranteed_rate;
} qmi_qos_umts_data_rate_type;


typedef struct
{
  unsigned long peak_rate;
  unsigned long token_rate;
  unsigned long bucket_size;
} qmi_qos_umts_token_bucket_type;

typedef struct
{
  unsigned long multiplier;
  unsigned long exponent;
} qmi_qos_umts_packet_err_rate_type;

typedef enum
{
  QMI_QOS_FALSE   = 0x00,
  QMI_QOS_TRUE    = 0x01
}qmi_qos_bool_type;


/* This bit definition is overloaded.  But since this is a CDMA
** definition which is exclusive of the UMTS definitions, this
** is OK.  This is only used in the error reporting mask, not
** in QoS initiation, sice the CMDA profile ID is a required
** parameter for CDMA technology
*/


typedef struct
{
  /* Mask of which optional parameters below are valid */
  unsigned short param_mask;

  /* UMTS optional QoS parameters.  At least one of these
  ** must be included in a setup request
  */
  qmi_qos_umts_traffic_class_type     traffic_class;
  qmi_qos_umts_data_rate_type         data_rate;
  qmi_qos_umts_token_bucket_type      bucket_info;
  unsigned long                       max_delay;
  unsigned long                       max_jitter;
  qmi_qos_umts_packet_err_rate_type   pkt_err_rate;
  unsigned long                       min_policed_pkt_sz;
  unsigned long                       max_allowed_pkt_sz;
  qmi_qos_umts_residual_ber_type      residual_ber;
  qmi_qos_umts_handling_prio_type     handling_prio;
  unsigned char                       flow_priority_3gpp2;
  qmi_qos_bool_type                   im_cn_flag;/*Valid only for 3gpp*/
  qmi_qos_bool_type                   ip_flow_sig_ind;/*valid only for 3gpp*/
  unsigned char                       lte_qci;
} qmi_qos_umts_flow_desc_type;

typedef struct
{
  /* Mask of which optional parameters below are valid */
  unsigned short param_mask;

  unsigned long profile_id;
} qmi_qos_cdma_flow_desc_type;

typedef struct
{
  qmi_qos_cdma_flow_desc_type   cdma_flow_desc;
  qmi_qos_umts_flow_desc_type   umts_flow_desc;
} qmi_qos_flow_req_type;


/* typedefs related to filter descriptors */
typedef struct
{
  unsigned long ipv4_ip_addr;
  unsigned long ipv4_subnet_mask;
} qmi_qos_ipv4_addr_filter_type;

typedef struct
{
  qmi_qos_ipv6_addr_type   ipv6_ip_addr;
  unsigned char            ipv6_filter_prefix_len;
}qmi_qos_ipv6_addr_filter_type;

typedef struct
{
  unsigned char tos_value;
  unsigned char tos_mask;
} qmi_qos_tos_filter_type;


typedef struct
{
  unsigned short start_port;
  unsigned short range;
} qmi_qos_port_range_filter_type;

typedef struct
{
  /* Mask of which optional parameters below are valid */
  unsigned long param_mask;

  /* UMTS optional QoS parameters.  At least one of these
  ** must be included in a setup request
  */
  qmi_qos_ipv4_addr_filter_type       src_addr;
  qmi_qos_ipv4_addr_filter_type       dest_addr;
  qmi_qos_trans_protocol_filter_type  protocol;
  qmi_qos_tos_filter_type             tos;
  qmi_qos_port_range_filter_type      tcp_src_ports;
  qmi_qos_port_range_filter_type      tcp_dest_ports;
  qmi_qos_port_range_filter_type      udp_src_ports;
  qmi_qos_port_range_filter_type      udp_dest_ports;
  qmi_qos_ipv6_addr_filter_type       ipv6_src_addr;
  qmi_qos_ipv6_addr_filter_type       ipv6_dest_addr;
  struct
  {
    unsigned char     traffic_class_value;
    unsigned char     traffic_class_mask;
  }ipv6_traffic_class;
  unsigned long                       ipv6_flow_label;
  unsigned long                       esp_security_policy_index;
  unsigned char                       precedence;
  unsigned char                       filter_id;
  qmi_qos_port_range_filter_type      transport_src_ports;
  qmi_qos_port_range_filter_type      transport_dest_ports;

} qmi_qos_filter_desc_type;


/* This is the filter request data structure.  Note that the ip_version
** parameter is required and must be set for each request.  The filter_desc
** optional parameters must follow rules outlined in QC QoS document
*/
typedef struct
{
  qmi_qos_ip_version_type        ip_version;
  qmi_qos_filter_desc_type         filter_desc;
} qmi_qos_filter_req_type;

typedef enum
{
  QMI_QOS_UNASSIGNED_TYPE   = 0x00,
  QMI_QOS_REQUEST           = 0x01,
  QMI_QOS_CONFIGURE         = 0x02
}qmi_qos_req_opcode_type;

/* This is the main QoS specification data structure.  For each
** of the request types, there is a count of the number
** request elements provided (can be an array of elements,
** treated in priority order) and a pointer to the request
** elements.
*/
typedef struct
{
  unsigned long             qos_identifier;     /*To be used only for QOS Modify*/

  unsigned long             num_tx_flow_req;
  qmi_qos_flow_req_type     *tx_flow_req_array;

  unsigned long             num_rx_flow_req;
  qmi_qos_flow_req_type     *rx_flow_req_array;

  unsigned long             num_tx_filter_req;
  qmi_qos_filter_req_type   *tx_filter_req_array;

  unsigned long             num_rx_filter_req;
  qmi_qos_filter_req_type   *rx_filter_req_array;

} qmi_qos_spec_type;

/* QOS request error response data type.  If the err_present flag is
** set then one or more of the included error masks will have bit(s)
** set.  The bits will indicate which of the required or optional
** parameters had errors for each of the request types.  The array
** elements correspond to the possible array of requests passed in
** in the request.
*/
typedef struct
{
  int            errs_present;
  unsigned short tx_flow_req_err_mask[QMI_QOS_MAX_FLOW_FILTER];
  unsigned short rx_flow_req_err_mask[QMI_QOS_MAX_FLOW_FILTER];
  unsigned short tx_filter_req_err_mask[QMI_QOS_MAX_FLOW_FILTER];
  unsigned short rx_filter_req_err_mask[QMI_QOS_MAX_FLOW_FILTER];
} qmi_qos_err_rsp_type;

/************************************************************************
* Definitions associated with qmi_qos_get_nw_supported_qos_profiles()
************************************************************************/
typedef enum
{
  QMI_QOS_INVALID_IFACE_NAME = 0x0000,

  QMI_QOS_ANY_DEFAULT_GROUP  = 0x0001,
  QMI_QOS_WWAN_GROUP         = 0x0002,
  QMI_QOS_RM_GROUP           = 0x0004,
  QMI_QOS_BCAST_MCAST_GROUP  = 0x0008,
  QMI_QOS_IFACE_3GPP_GROUP   = 0x0010,
  QMI_QOS_IFACE_3GPP2_GROUP  = 0x0020,
  QMI_QOS_IFACE_EPC_GROUP    = 0x0040,
  QMI_QOS_ANY_IFACE_GROUP    = 0x7FFF,

  QMI_QOS_IFACE_MASK         = 0x8000,
  QMI_QOS_CDMA_SN_IFACE      = 0x8001,
  QMI_QOS_CDMA_AN_IFACE      = 0x8002,
  QMI_QOS_UMTS_IFACE         = 0x8004,
  QMI_QOS_SIO_IFACE          = 0x8008,
  QMI_QOS_CDMA_BCAST_IFACE   = 0x8010,
  QMI_QOS_WLAN_IFACE         = 0x8020,
  QMI_QOS_DUN_IFACE          = 0x8040,
  QMI_QOS_FLO_IFACE          = 0x8080,
  QMI_QOS_DVBH_IFACE         = 0x8100,
  QMI_QOS_STA_IFACE          = 0x8200,
  QMI_QOS_IPSEC_IFACE        = 0x8400,
  QMI_QOS_LO_MODE_B_IFACE    = 0x8401,
  QMI_QOS_LO_IFACE           = 0x8800,
  QMI_QOS_MBMS_IFACE         = 0x8801,
  QMI_QOS_IWLAN_3GPP_IFACE   = 0x8802,
  QMI_QOS_IWLAN_3GPP2_IFACE  = 0x8804,
  QMI_QOS_MIP6_IFACE         = 0x8808,
  QMI_QOS_SLIP_IFACE         = 0x8810,
  QMI_QOS_UICC_IFACE         = 0x8820,
  QMI_QOS_UW_FMC_IFACE       = 0x8840,
  QMI_QOS_EPC_IFACE          = 0x8880,
  QMI_QOS_NAT_IFACE          = 0x8881
} qmi_qos_iface_name_type;

typedef enum
{
  QMI_QOS_PROFILE_TECH_3GPP  = 0x01,
  QMI_QOS_PROFILE_TECH_3GPP2 = 0x02
} qmi_qos_profile_tech_type;

typedef struct
{
  qmi_qos_iface_name_type     iface_type;
  unsigned char               num_profiles;
  unsigned short              profile[QMI_QOS_MAX_PROFILES]; //TODO:?
}qmi_qos_profile_info_type;

typedef struct
{
  unsigned char             num_instances;
  qmi_qos_profile_info_type profile_info[QMI_QOS_MAX_PROFILES];
}qmi_qos_nw_supported_qos_profiles;


typedef struct
{
  qmi_qos_nw_supported_qos_profiles     qos_profiles;
  qmi_qos_dss_errno_type                  dss_err;
}qmi_qos_nw_supported_qos_profiles_rsp_type;

/*Event Report related structures*/

/* For turning event reporting on/off */

typedef enum
{
  QMI_QOS_EVENT_REPORTING_OFF = 0,
  QMI_QOS_EVENT_REPORTING_ON = 1
} qmi_qos_global_flow_reporting_state_type;

typedef enum
{
  QMI_QOS_PROFILE_CHANGE_REPORTING_ENABLE   = 0x01,
  QMI_QOS_PROFILE_CHANGE_REPORTING_DISABLE  = 0x00
}qmi_qos_profile_change_status;

typedef struct
{
  qmi_qos_profile_change_status      profile_change_status;
  qmi_qos_iface_name_type            iface_type;/*IMPORTANT: USE ONLY "3GPP2"*/
}qmi_qos_net_supported_profile_change_type;

typedef struct
{
  unsigned short               param_mask;

  qmi_qos_global_flow_reporting_state_type      flow_state;
  qmi_qos_net_supported_profile_change_type     profile_change_state;

}qmi_qos_event_report_state_type;

/*Status report set response enum*/

typedef enum
{
  QMI_QOS_STATUS_ACTIVATED = 0x01,
  QMI_QOS_STATUS_SUSPENDED = 0x02,
  QMI_QOS_STATUS_GONE = 0x03
} qmi_qos_status_info;

typedef enum
{
  QMI_QOS_ACTIVATED_EV          = 0x01,
  QMI_QOS_SUSPENDED_EV          = 0x02,
  QMI_QOS_GONE_EV               = 0x03,
  QMI_QOS_MODIFY_ACCEPTED_EV    = 0x04,
  QMI_QOS_MODIFY_REJECTED_EV    = 0x05,
  QMI_QOS_INFO_CODE_UPDATED_EV  = 0x06,
} qmi_qos_status_event;

typedef enum
{
  QMI_QOS_INVALID_PARAMS                          = 0x01,
  QMI_QOS_INTERNAL_CALL_ENDED                     = 0x02,
  QMI_QOS_INTERNAL_ERROR                          = 0x03,
  QMI_QOS_INSUFFICIENT_LOCAL_RESOURCES            = 0x04,
  QMI_QOS_TIMED_OUT_OPERATION                     = 0x05,
  QMI_QOS_INTERNAL_UNKNOWN_CAUSE_CODE             = 0x06,
  QMI_QOS_INTERNAL_MODIFY_IN_PROGRESS             = 0x07,
  QMI_QOS_NOT_SUPPORTED                           = 0x08,
  QMI_QOS_NOT_AVAILABLE                           = 0x09,
  QMI_QOS_NOT_GUARANTEED                          = 0x0A,
  QMI_QOS_INSUFFICIENT_NETWORK_RESOURCES          = 0x0B,
  QMI_QOS_AWARE_SYSTEM                            = 0x0C,
  QMI_QOS_UNAWARE_SYSTEM                          = 0x0D,
  QMI_QOS_REJECTED_OPERATION                      = 0x0E,
  QMI_QOS_WILL_GRANT_WHEN_QOS_RESUMED             = 0x0F,
  QMI_QOS_NETWORK_CALL_ENDED                      = 0x10,
  QMI_QOS_NETWORK_SERVICE_NOT_AVAILABLE           = 0x11,
  QMI_QOS_NETWORK_L2_LINK_RELEASED                = 0x12,
  QMI_QOS_NETWORK_L2_LINK_REESTAB_REJ             = 0x13,
  QMI_QOS_NETWORK_L2_LINK_REESTAB_IND             = 0x14,
  QMI_QOS_NETWORK_UNKNOWN_CAUSE_CODE              = 0x15,
  QMI_QOS_NETWORK_BUSY                            = 0x16,
  QMI_QOS_NETWORK_DISJOINT_PROFILE_SET_SUGGESTED  = 0x17,
  QMI_QOS_NETWORK_NULL_PROFILE_SUGGESTED          = 0x18,
  QMI_QOS_NETWORK_UE_NOT_AUTHORIZED               = 0x19
} qmi_qos_reason_code;

typedef struct
{
  struct
  {
    unsigned long          qos_identifier;
    qmi_qos_status_info    qos_status;
    qmi_qos_status_event   qos_event;
  }qos_status_information;

  int qos_reason_is_valid;
  qmi_qos_reason_code   qos_reason;

}qmi_qos_status_report_type;

/************************************************************************
* Definitions associated with qmi_qos_get_granted_qos_info()
************************************************************************/

typedef enum
{
  QMI_QOS_FLOW_ACTIVATED = 0x01,
  QMI_QOS_FLOW_MODIFIED  = 0x02,
  QMI_QOS_FLOW_DELETED   = 0x03,
  QMI_QOS_FLOW_SUSPENDED = 0x04,
  QMI_QOS_FLOW_ENABLED   = 0x05,
  QMI_QOS_FLOW_DISABLED  = 0x06
}global_flow_state_change;

typedef struct
{
  unsigned short           ip_flow_index;
  qmi_qos_flow_req_type    qos_flow_granted;
}qmi_qos_granted_flow_data_type;

typedef struct
{
  unsigned short           filter_index;
  qmi_qos_filter_req_type    qos_filter;
}qmi_qos_granted_filter_data_type;


/*current granted qos info*/
typedef struct
{
  int                             tx_granted_flow_data_is_valid;
  qmi_qos_granted_flow_data_type  tx_granted_flow_data;

  int                             rx_granted_flow_data_is_valid;
  qmi_qos_granted_flow_data_type  rx_granted_flow_data;

  int                              tx_filter_count;
  qmi_qos_granted_filter_data_type tx_granted_filter_data[QMI_QOS_MAX_FLOW_FILTER];

  int                              rx_filter_count;
  qmi_qos_granted_filter_data_type rx_granted_filter_data[QMI_QOS_MAX_FLOW_FILTER];

  qmi_qos_dss_errno_type          dss_errno_type;
}qmi_qos_granted_info_rsp_type;


typedef struct
{
  unsigned short          param_mask;

  struct
  {
    unsigned long     qos_identifier;
    unsigned char     new_flow;
    global_flow_state_change  report_flow_state_chng;
  } qos_flow_state;

  unsigned char           flow_type;

  /*If the Corresponding bit is set in the event mask*/
  int                     tx_granted_flow_data_is_valid;
  qmi_qos_granted_flow_data_type   tx_granted_flow_data;

  /*If the Corresponding bit is set in the event mask*/
  int                     rx_granted_flow_data_is_valid;
  qmi_qos_granted_flow_data_type   rx_granted_flow_data;

  unsigned int            tx_filter_count;/* other suggestions for this variable ? */
  qmi_qos_granted_filter_data_type  tx_granted_filter_data[QMI_QOS_MAX_FLOW_FILTER];

  unsigned int            rx_filter_count;
  qmi_qos_granted_filter_data_type  rx_granted_filter_data[QMI_QOS_MAX_FLOW_FILTER];

}qmi_qos_event_report_flow_info_type;


/*typedef event report indication structure*/

typedef struct
{
  unsigned short                            param_mask; //PARAM MASK for nw_supported_qos_profiles

  int                                       num_flows; // Number of valid flows in the indication message.
  qmi_qos_event_report_flow_info_type       flow_info[QMI_QOS_MAX_FLOW_EVENTS];

  qmi_qos_profile_info_type                 nw_supported_qos_profiles;

}qmi_qos_event_report_type;



typedef enum
{
  NW_DOESNT_SUPPORT_QOS = 0x00,
  NW_SUPPORTS_QOS       = 0x01
} qmi_qos_nw_support_status;

typedef struct
{
  qmi_qos_nw_support_status  qos_supported;
  qmi_qos_iface_name_type    iface_name;
} qmi_qos_nw_status_type;

typedef enum
{
  QMI_QOS_PRIMARY_FLOW_MODIFY_SUCCESS    = 0x0001,
  QMI_QOS_PRIMARY_FLOW_MODIFY_FAILURE   = 0x0002
}qmi_qos_primrary_qos_event_type;

typedef qmi_qos_req_opcode_type         dsi_qos_req_opcode_type;
typedef qmi_qos_err_rsp_type            dsi_qos_err_rsp_type;
typedef qmi_qos_granted_info_rsp_type   dsi_qos_granted_info_type;
typedef qmi_qos_granted_flow_data_type  dsi_qos_granted_flow_type;
typedef qmi_qos_status_info             dsi_qos_status_type;

typedef qmi_qos_spec_type               dsi_qos_spec_type;
typedef qmi_qos_granted_filter_data_type dsi_qos_granted_filter_data_type;
typedef qmi_qos_flow_req_type           dsi_qos_flow_req_type;
typedef qmi_qos_filter_req_type         dsi_qos_filter_req_type;
typedef qmi_qos_umts_traffic_class_type dsi_qos_umts_traffic_class_type;
typedef qmi_qos_tos_filter_type         dsi_qos_tos_filter_type;
typedef qmi_qos_ipv4_addr_filter_type   dsi_qos_ipv4_addr_filter_type;
typedef qmi_qos_ipv6_addr_filter_type   dsi_qos_ipv6_addr_filter_type;

typedef qmi_qos_nw_status_type dsi_qos_nw_status_type;
typedef qmi_qos_status_report_type dsi_qos_status_report_type;
typedef qmi_qos_event_report_type dsi_qos_event_report_type;
typedef qmi_qos_primrary_qos_event_type dsi_qos_primrary_qos_event_type;
typedef qmi_qos_event_report_state_type dsi_qos_event_report_state_type;


/*===========================================================================
                    EXTERNAL FUNCTION PROTOTYPES
===========================================================================*/

/*===========================================================================
  FUNCTION:  dsi_request_qos
===========================================================================*/
/** @ingroup dsi_request_qos

    Requests a new QoS flow and filter on a pre-existing network connection.

    Multiple QoS specifications may be specified per invocation; however, the
    number of specifications supported is network service dependent. Each QoS
    specification contains one or more flow/filter specifications.

    For network service supporting multiple flow/filter specifications (e.g.,
    3GPP2), only one flow/filter specification in a set is granted via
    negotiation with the network.

    The return code indicates a successful start of the transaction by the
    modem, while the final outcome is determined via asynchronous event
    indications per Section (?*? fill in later). Subsequent QoS status event indications
    are automatically generated for each flow created on the modem

    @param[in] hndl Handle received from dsi_get_data_srvc_hndl().
    @param[in] num_qos_specs Number of QoS specifications passed in the
                             qos_spec_list parameter (limited to a maximum
                             of 10). This also dictates the size of
                             qos_id_list and qos_spec_err_list parameters,
                             for which the client must allocate storage.
    @param[in] qos_spec_list Array of QoS specifications. One or more
                             instances of this type may be present.
    @param[in] req_opcode Request operation code. Indicates a request or
                          configure operation.
    @param[out] qos_id_list List of opaque handles for each QoS flow created.
    @param[out] qos_spec_err_list Array of QoS specification errors returned
                                  from the modem.

    @return
    DSI_SUCCESS -- The request was sent successfully. \n
    DSI_ERROR -- The request was unsuccessful.

    @dependencies
    dsi_init() must be called. \n
    The handle must be a valid handle obtained by dsi_get_data_srvc_hndl(). \n
    There must be an active data call for the handle.
*/
/*=========================================================================*/
extern int dsi_request_qos
(
  dsi_hndl_t              hndl,
  unsigned int            num_qos_specs,
  dsi_qos_spec_type      *qos_spec_list,
  dsi_qos_req_opcode_type req_opcode,
  dsi_qos_id_type        *qos_id_list,
  dsi_qos_err_rsp_type   *qos_spec_err_list
);


/*===========================================================================
  FUNCTION:  dsi_release_qos
===========================================================================*/
/** @ingroup dsi_release_qos

    Deletes existing QoS flow and filters from a network connection.

    The return code indicates a successful start of the transaction by the
    modem, while the final outcome is determined via an asynchronous event
    indication per Section ?*?.

    @param[in] hndl Handle received from dsi_get_data_srvc_hndl().
    @param[in] num_qos_ids Number of QoS flow handles passed in the
                           qos_id_list parameter.
    @param[in] qos_id_list Array of QoS flow handles. One or more
                           instances of this type may be present.

    @return
    DSI_SUCCESS -- The release operation was successful. \n
    DSI_ERROR -- The release operation was unsuccessful.

    @dependencies
    dsi_init() must be called. \n
    The handle must be a valid handle obtained by dsi_get_data_srvc_hndl(). \n
    QoS flows must be allocated using dsi_request_qos().
*/
/*=========================================================================*/
extern int dsi_release_qos
(
  dsi_hndl_t              hndl,
  unsigned int            num_qos_ids,
  dsi_qos_id_type        *qos_id_list
);


/*===========================================================================
  FUNCTION:  dsi_modify_qos
===========================================================================*/
/** @ingroup dsi_modify_qos

    Changes existing QoS flows on a network connection.

    The return code indicates a successful start of the transaction by the
    modem, while the final outcome is determined via an asynchronous event
    indication per Section ?*?.

    @param[in] hndl Handle received from dsi_get_data_srvc_hndl().
    @param[in] num_qos_specs Number of QoS specifications passed in the
                             qos_spec_list parameter. This also dictates the
                             size of the qos_id_list and qos_spec_err_list
                             parameters, for which the client must allocate
                             storage.
    @param[in] qos_spec_list Array of QoS specifications. One or more
                             instances of this type may be present.
    @param[out] qos_spec_err_list Array of QoS specification errors returned
                                  from the modem.

    @return
    DSI_SUCCESS -- The modify operation was successful. \n
    DSI_ERROR -- The modify operation was unsuccessful.

    @dependencies
    dsi_init() must be called. \n
    The handle must be a valid handle obtained by dsi_get_data_srvc_hndl(). \n
    QoS flows must be allocated using dsi_request_qos().
*/
/*=========================================================================*/
extern int dsi_modify_qos
(
  dsi_hndl_t              hndl,
  unsigned int            num_qos_specs,
  dsi_qos_spec_type      *qos_spec_list,
  dsi_qos_err_rsp_type   *qos_spec_err_list
);


/*===========================================================================
  FUNCTION:  dsi_suspend_qos
===========================================================================*/
/** @ingroup dsi_suspend_qos

    Disables prioritized packet handling for existing QoS flows
    on a network connection. Further packet transmission on those QoS
    flows is treated as best-effort traffic.

    The return code indicates a successful start of the transaction by the
    modem, while the final outcome is determined via an asynchronous event
    indication per Section ?*?.

    @param[in] hndl Handle received from dsi_get_data_srvc_hndl().
    @param[in] num_qos_ids Number of QoS flow handles passed in the
                           qos_id_list parameter.
    @param[in] qos_id_list Array of QoS flow handles. One or more
                           instances of this type may be present.

    @return
    DSI_SUCCESS -- The suspend operation was successful. \n
    DSI_ERROR -- The suspend operation was unsuccessful.

    @dependencies
    dsi_init() must be called. \n
    The handle must be a valid handle obtained by dsi_get_data_srvc_hndl(). \n
    QoS flows must be allocated using dsi_request_qos().
*/
/*=========================================================================*/
extern int dsi_suspend_qos
(
  dsi_hndl_t              hndl,
  unsigned int            num_qos_ids,
  dsi_qos_id_type        *qos_id_list
);


/*===========================================================================
  FUNCTION:  dsi_resume_qos
===========================================================================*/
/** @ingroup dsi_resume_qos

    Enables prioritized packet handling for existing QoS flows on a network
    connection.

    The return code indicates a successful start of the transaction by the
    modem, while the final outcome is determined via an asynchronous event
    indication per Section ?*?.

    @param[in] hndl Handle received from dsi_get_data_srvc_hndl().
    @param[in] num_qos_ids Number of QoS flow handles passed in the
                           qos_id_list parameter.
    @param[in] qos_id_list Array of QoS flow handles. One or more
                           instances of this type may be present.

    @return
    DSI_SUCCESS -- The resume operation was successful. \n
    DSI_ERROR -- The resume operation was unsuccessful.

    @dependencies
    dsi_init() must be called. \n
    The handle must be a valid handle obtained by dsi_get_data_srvc_hndl(). \n
    QoS flows must be allocated using dsi_request_qos(). \n
    Only a suspended QoS flow can be resumed.
*/
/*=========================================================================*/
extern int dsi_resume_qos
(
  dsi_hndl_t              hndl,
  unsigned int            num_qos_ids,
  dsi_qos_id_type        *qos_id_list
);


/*===========================================================================
  FUNCTION:  dsi_get_granted_qos
===========================================================================*/
/** @ingroup dsi_get_granted_qos

    Queries the QoS information for an existing flow on a network
    connection.

    @param[in] hndl Handle received from dsi_get_data_srvc_hndl().
    @param[in] qos_id QoS flow handle.
    @param[in] ip_family specify ip family - AF_INET or AF_INET6
    @param[out] qos_info QoS information returned from the modem.

    @return
    DSI_SUCCESS -- The query was successful. \n
    DSI_ERROR -- The query operation was unsuccessful.

    @dependencies
    dsi_init() must be called. \n
    The handle must be a valid handle obtained by dsi_get_data_srvc_hndl(). \n
    QoS flows must be allocated using dsi_request_qos().
*/
/*=========================================================================*/
extern int dsi_get_granted_qos
(
  dsi_hndl_t                 hndl,
  dsi_qos_id_type            qos_id,
  int                        ip_family,
  dsi_qos_granted_info_type *qos_info
);

/*===========================================================================
  FUNCTION:  dsi_get_qos_status
===========================================================================*/
/** @ingroup dsi_get_qos_status

    Queries the QoS activated/suspended/gone state for an
    existing flow on a network connection.

    @param[in] hndl Handle received from dsi_get_data_srvc_hndl().
    @param[in] qos_id QoS flow handle.
    @param[out] qos_status QoS flow status returned from the modem.

    @return
    DSI_SUCCESS -- The query was successful. \n
    DSI_ERROR -- The query operation was unsuccessful.

    @dependencies
    dsi_init() must be called. \n
    The handle must be a valid handle obtained by dsi_get_data_srvc_hndl(). \n
    QoS flows must be allocated using dsi_request_qos().
*/
/*=========================================================================*/
extern int dsi_get_qos_status
(
  dsi_hndl_t           hndl,
  dsi_qos_id_type      qos_id,
  dsi_qos_status_type *qos_status
);

#endif /* DSI_NETCTRL_QOS_H */
