/**
 * @file ql_nf3303_ble_server.c
 * @brief Quectel Openlinux BT public function declarations.
 * 
 * @note this file contains internally used ATT definitions 
 *
 * @copyright Copyright (c) 2009-2017 @ Quectel Wireless Solutions Co., Ltd.
 */

#ifndef  QL_NF3303_BLE_GATTDEFS_H
#define  QL_NF3303_BLE_GATTDEFS_H

/* Define common 16-bit protocol UUIDs
*/
#define UUID_PROTOCOL_SDP                    0x0001
#define UUID_PROTOCOL_UDP                    0x0002
#define UUID_PROTOCOL_RFCOMM                 0x0003
#define UUID_PROTOCOL_TCP                    0x0004
#define UUID_PROTOCOL_TCS_BIN                0x0005
#define UUID_PROTOCOL_TCS_AT                 0x0006
#define UUID_PROTOCOL_OBEX                   0x0008
#define UUID_PROTOCOL_IP                     0x0009
#define UUID_PROTOCOL_FTP                    0x000A
#define UUID_PROTOCOL_HTTP                   0x000C
#define UUID_PROTOCOL_WSP                    0x000E
#define UUID_PROTOCOL_BNEP                   0x000F
#define UUID_PROTOCOL_UPNP                   0x0010
#define UUID_PROTOCOL_HIDP                   0x0011
#define UUID_PROTOCOL_HCRP_CTRL              0x0012
#define UUID_PROTOCOL_HCRP_DATA              0x0014
#define UUID_PROTOCOL_HCRP_NOTIF             0x0016
#define UUID_PROTOCOL_AVCTP                  0x0017
#define UUID_PROTOCOL_AVDTP                  0x0019
#define UUID_PROTOCOL_CMTP                   0x001B
#define UUID_PROTOCOL_UDI                    0x001D
#define UUID_PROTOCOL_MCAP_CTRL              0x001E
#define UUID_PROTOCOL_MCAP_DATA              0x001F
#define UUID_PROTOCOL_L2CAP                  0x0100
#define UUID_PROTOCOL_ATT                    0x0007

/* Define common 16-bit service class UUIDs
*/
#define UUID_SERVCLASS_SERVICE_DISCOVERY_SERVER 0X1000
#define UUID_SERVCLASS_BROWSE_GROUP_DESCRIPTOR  0X1001
#define UUID_SERVCLASS_PUBLIC_BROWSE_GROUP      0X1002
#define UUID_SERVCLASS_SERIAL_PORT              0X1101
#define UUID_SERVCLASS_LAN_ACCESS_USING_PPP     0X1102
#define UUID_SERVCLASS_DIALUP_NETWORKING        0X1103
#define UUID_SERVCLASS_IRMC_SYNC                0X1104
#define UUID_SERVCLASS_OBEX_OBJECT_PUSH         0X1105
#define UUID_SERVCLASS_OBEX_FILE_TRANSFER       0X1106
#define UUID_SERVCLASS_IRMC_SYNC_COMMAND        0X1107
#define UUID_SERVCLASS_HEADSET                  0X1108
#define UUID_SERVCLASS_CORDLESS_TELEPHONY       0X1109
#define UUID_SERVCLASS_AUDIO_SOURCE             0X110A
#define UUID_SERVCLASS_AUDIO_SINK               0X110B
#define UUID_SERVCLASS_AV_REM_CTRL_TARGET       0X110C  /* Audio/Video Control profile */
#define UUID_SERVCLASS_ADV_AUDIO_DISTRIBUTION   0X110D  /* Advanced Audio Distribution profile */
#define UUID_SERVCLASS_AV_REMOTE_CONTROL        0X110E  /* Audio/Video Control profile */
#define UUID_SERVCLASS_AV_REM_CTRL_CONTROL      0X110F  /* Audio/Video Control profile */
#define UUID_SERVCLASS_INTERCOM                 0X1110
#define UUID_SERVCLASS_FAX                      0X1111
#define UUID_SERVCLASS_HEADSET_AUDIO_GATEWAY    0X1112
#define UUID_SERVCLASS_WAP                      0X1113
#define UUID_SERVCLASS_WAP_CLIENT               0X1114
#define UUID_SERVCLASS_PANU                     0X1115  /* PAN profile */
#define UUID_SERVCLASS_NAP                      0X1116  /* PAN profile */
#define UUID_SERVCLASS_GN                       0X1117  /* PAN profile */
#define UUID_SERVCLASS_DIRECT_PRINTING          0X1118  /* BPP profile */
#define UUID_SERVCLASS_REFERENCE_PRINTING       0X1119  /* BPP profile */
#define UUID_SERVCLASS_IMAGING                  0X111A  /* Imaging profile */
#define UUID_SERVCLASS_IMAGING_RESPONDER        0X111B  /* Imaging profile */
#define UUID_SERVCLASS_IMAGING_AUTO_ARCHIVE     0X111C  /* Imaging profile */
#define UUID_SERVCLASS_IMAGING_REF_OBJECTS      0X111D  /* Imaging profile */
#define UUID_SERVCLASS_HF_HANDSFREE             0X111E  /* Handsfree profile */
#define UUID_SERVCLASS_AG_HANDSFREE             0X111F  /* Handsfree profile */
#define UUID_SERVCLASS_DIR_PRT_REF_OBJ_SERVICE  0X1120  /* BPP profile */
#define UUID_SERVCLASS_REFLECTED_UI             0X1121  /* BPP profile */
#define UUID_SERVCLASS_BASIC_PRINTING           0X1122  /* BPP profile */
#define UUID_SERVCLASS_PRINTING_STATUS          0X1123  /* BPP profile */
#define UUID_SERVCLASS_HUMAN_INTERFACE          0X1124  /* HID profile */
#define UUID_SERVCLASS_CABLE_REPLACEMENT        0X1125  /* HCRP profile */
#define UUID_SERVCLASS_HCRP_PRINT               0X1126  /* HCRP profile */
#define UUID_SERVCLASS_HCRP_SCAN                0X1127  /* HCRP profile */
#define UUID_SERVCLASS_COMMON_ISDN_ACCESS       0X1128  /* CAPI Message Transport Protocol*/
#define UUID_SERVCLASS_VIDEO_CONFERENCING_GW    0X1129  /* Video Conferencing profile */
#define UUID_SERVCLASS_UDI_MT                   0X112A  /* Unrestricted Digital Information profile */
#define UUID_SERVCLASS_UDI_TA                   0X112B  /* Unrestricted Digital Information profile */
#define UUID_SERVCLASS_VCP                      0X112C  /* Video Conferencing profile */
#define UUID_SERVCLASS_SAP                      0X112D  /* SIM Access profile */
#define UUID_SERVCLASS_PBAP_PCE                 0X112E  /* Phonebook Access - PCE */
#define UUID_SERVCLASS_PBAP_PSE                 0X112F  /* Phonebook Access - PSE */
#define UUID_SERVCLASS_PHONE_ACCESS             0x1130
#define UUID_SERVCLASS_HEADSET_HS               0x1131  /* Headset - HS, from HSP v1.2 */
#define UUID_SERVCLASS_3DD                      0x1137  /* 3D Sync (Display role) */
#define UUID_SERVCLASS_3DG                      0x1138  /* 3D Sync (Glasses role) */
#define UUID_SERVCLASS_3DS                      0x1139  /* 3D Sync Profile */
#define UUID_SERVCLASS_PNP_INFORMATION          0X1200  /* Device Identification */
#define UUID_SERVCLASS_GENERIC_NETWORKING       0X1201
#define UUID_SERVCLASS_GENERIC_FILETRANSFER     0X1202
#define UUID_SERVCLASS_GENERIC_AUDIO            0X1203
#define UUID_SERVCLASS_GENERIC_TELEPHONY        0X1204
#define UUID_SERVCLASS_UPNP_SERVICE             0X1205  /* UPNP_Service [ESDP] */
#define UUID_SERVCLASS_UPNP_IP_SERVICE          0X1206  /* UPNP_IP_Service [ESDP] */
#define UUID_SERVCLASS_ESDP_UPNP_IP_PAN         0X1300  /* UPNP_IP_PAN [ESDP] */
#define UUID_SERVCLASS_ESDP_UPNP_IP_LAP         0X1301  /* UPNP_IP_LAP [ESDP] */
#define UUID_SERVCLASS_ESDP_UPNP_IP_L2CAP       0X1302  /* UPNP_L2CAP [ESDP] */
#define UUID_SERVCLASS_VIDEO_SOURCE             0X1303  /* Video Distribution Profile (VDP) */
#define UUID_SERVCLASS_VIDEO_SINK               0X1304  /* Video Distribution Profile (VDP) */
#define UUID_SERVCLASS_VIDEO_DISTRIBUTION       0X1305  /* Video Distribution Profile (VDP) */
#define UUID_SERVCLASS_HDP_PROFILE              0X1400  /* Health Device profile (HDP) */
#define UUID_SERVCLASS_HDP_SOURCE               0X1401  /* Health Device profile (HDP) */
#define UUID_SERVCLASS_HDP_SINK                 0X1402  /* Health Device profile (HDP) */
#define UUID_SERVCLASS_MAP_PROFILE              0X1134  /* MAP profile UUID */
#define UUID_SERVCLASS_MESSAGE_ACCESS           0X1132  /* Message Access Service UUID */
#define UUID_SERVCLASS_MESSAGE_NOTIFICATION     0X1133  /* Message Notification Service UUID */

#define UUID_SERVCLASS_GAP_SERVER               0x1800
#define UUID_SERVCLASS_GATT_SERVER              0x1801
#define UUID_SERVCLASS_IMMEDIATE_ALERT          0x1802      /* immediate alert */
#define UUID_SERVCLASS_LINKLOSS                 0x1803      /* Link Loss Alert */
#define UUID_SERVCLASS_TX_POWER                 0x1804      /* TX power */
#define UUID_SERVCLASS_CURRENT_TIME             0x1805      /* Link Loss Alert */
#define UUID_SERVCLASS_DST_CHG                  0x1806      /* DST Time change */
#define UUID_SERVCLASS_REF_TIME_UPD             0x1807      /* reference time update */
#define UUID_SERVCLASS_THERMOMETER              0x1809      /* Thermometer UUID */
#define UUID_SERVCLASS_DEVICE_INFO              0x180A      /* device info service */
#define UUID_SERVCLASS_NWA                      0x180B      /* Network availability */
#define UUID_SERVCLASS_HEART_RATE               0x180D      /* Heart Rate service */
#define UUID_SERVCLASS_PHALERT                  0x180E      /* phone alert service */
#define UUID_SERVCLASS_BATTERY                  0x180F     /* battery service */
#define UUID_SERVCLASS_BPM                      0x1810      /*  blood pressure service */
#define UUID_SERVCLASS_ALERT_NOTIFICATION       0x1811      /* alert notification service */
#define UUID_SERVCLASS_LE_HID                   0x1812     /*  HID over LE */
#define UUID_SERVCLASS_SCAN_PARAM               0x1813      /* Scan Parameter service */
#define UUID_SERVCLASS_GLUCOSE                  0x1808      /* Glucose Meter Service */
#define UUID_SERVCLASS_RSC                      0x1814      /* Runners Speed and Cadence Service */
#define UUID_SERVCLASS_CSC                      0x1816      /* Cycling Speed and Cadence Service */
#define UUID_SERVCLASS_BAV_SOURCE               0x8000      /* Broadcast AV service*/ /* BSA_SPECIFIC */
#define UUID_SERVCLASS_CP                       0x1818      /* Cycling Power Service (pre-Adoption IOP) */
#define UUID_SERVCLASS_LN                       0x1819      /* Location and Navigation Service (pre-Adoption IOP) */

/* GATT attribute types
*/
#define GATT_UUID_PRI_SERVICE           0x2800
#define GATT_UUID_SEC_SERVICE           0x2801
#define GATT_UUID_INCLUDE_SERVICE       0x2802
#define GATT_UUID_CHAR_DECLARE          0x2803      /*  Characteristic Declaration*/

#define GATT_UUID_CHAR_EXT_PROP         0x2900      /*	Characteristic Extended Properties */
#define GATT_UUID_CHAR_DESCRIPTION      0x2901      /*  Characteristic User Description*/
#define GATT_UUID_CHAR_CLIENT_CONFIG    0x2902      /*  Client Characteristic Configuration */
#define GATT_UUID_CHAR_SRVR_CONFIG      0x2903      /*  Server Characteristic Configuration */
#define GATT_UUID_CHAR_PRESENT_FORMAT   0x2904      /*  Characteristic Presentation Format*/
#define GATT_UUID_CHAR_AGG_FORMAT       0x2905      /*  Characteristic Aggregate Format*/
#define GATT_UUID_CHAR_VALID_RANGE       0x2906      /*  Characteristic Valid Range */
#define GATT_UUID_EXT_RPT_REF_DESCR     0x2907
#define GATT_UUID_RPT_REF_DESCR         0x2908


/* GAP Profile Attributes
*/
#define GATT_UUID_GAP_DEVICE_NAME       0x2A00
#define GATT_UUID_GAP_ICON              0x2A01
#define GATT_UUID_GAP_PREF_CONN_PARAM   0x2A04
#define GATT_UUID_GAP_CENTRAL_ADDR_RESOL    0x2AA6

/* Attribute Profile Attribute UUID */
#define GATT_UUID_GATT_SRV_CHGD         0x2A05
/* Attribute Protocol Test */

/* Link Loss Service */
#define GATT_UUID_ALERT_LEVEL           0x2A06      /* Alert Level */
#define GATT_UUID_TX_POWER_LEVEL        0x2A07      /* TX power level */

/* Time Profile */
/* Current Time Service */
#define GATT_UUID_CURRENT_TIME          0x2A2B      /* Current Time */
#define GATT_UUID_LOCAL_TIME_INFO       0x2A0F      /* Local time info */
#define GATT_UUID_REF_TIME_INFO         0x2A14      /* reference time information */

/* NwA Profile */
#define GATT_UUID_NW_STATUS             0x2A18      /* network availability status */
#define GATT_UUID_NW_TRIGGER            0x2A1A      /* Network availability trigger */

/* phone alert */
#define GATT_UUID_ALERT_STATUS          0x2A40    /* alert status */
#define GATT_UUID_RINGER_CP             0x2A42    /* ringer control point */
#define GATT_UUID_RINGER_SETTING        0x2A41    /* ringer setting */

/* Glucose Service */
#define GATT_UUID_GM_MEASUREMENT        0x2A18
#define GATT_UUID_GM_CONTEXT            0x2A34
#define GATT_UUID_GM_CONTROL_POINT      0x2A52
#define GATT_UUID_GM_FEATURE            0x2A51

/* device infor characteristic */
#define GATT_UUID_SYSTEM_ID             0x2A23
#define GATT_UUID_MODEL_NUMBER_STR      0x2A24
#define GATT_UUID_SERIAL_NUMBER_STR     0x2A25
#define GATT_UUID_FW_VERSION_STR        0x2A26
#define GATT_UUID_HW_VERSION_STR        0x2A27
#define GATT_UUID_SW_VERSION_STR        0x2A28
#define GATT_UUID_MANU_NAME             0x2A29
#define GATT_UUID_IEEE_DATA             0x2A2A
#define GATT_UUID_PNP_ID                0x2A50

/* HID characteristics */
#define GATT_UUID_HID_INFORMATION       0x2A4A
#define GATT_UUID_HID_REPORT_MAP        0x2A4B
#define GATT_UUID_HID_CONTROL_POINT     0x2A4C
#define GATT_UUID_HID_REPORT            0x2A4D
#define GATT_UUID_HID_PROTO_MODE        0x2A4E
#define GATT_UUID_HID_BT_KB_INPUT       0x2A22
#define GATT_UUID_HID_BT_KB_OUTPUT      0x2A32
#define GATT_UUID_HID_BT_MOUSE_INPUT    0x2A33

/* Battery Service char */
#define GATT_UUID_BATTERY_LEVEL         0x2A19

#define GATT_UUID_SC_CONTROL_POINT      0x2A55
#define GATT_UUID_SENSOR_LOCATION       0x2A5D

/* RUNNERS SPEED AND CADENCE SERVICE      */
#define GATT_UUID_RSC_MEASUREMENT       0x2A53
#define GATT_UUID_RSC_FEATURE           0x2A54

/* CYCLING SPEED AND CADENCE SERVICE      */
#define GATT_UUID_CSC_MEASUREMENT       0x2A5B
#define GATT_UUID_CSC_FEATURE           0x2A5C

/* CYCLING POWER SERVICE  (Temp for IOP)    */
#define GATT_UUID_CP_MEASUREMENT        0x2A63
#define GATT_UUID_CP_VECTOR             0x2A64
#define GATT_UUID_CP_FEATURE            0x2A65
#define GATT_UUID_CP_CONTROL_POINT      0x2A66

/* LOCATION AND NAVIGATION SERVICE  (Temp for IOP)    */
#define GATT_UUID_LN_LOC_AND_SPEED      0x2A67
#define GATT_UUID_LN_NAVIGATION         0x2A68
#define GATT_UUID_LN_POSITION_QUALITY   0x2A69
#define GATT_UUID_LN_FEATURE            0x2A6A
#define GATT_UUID_LN_CONTROL_POINT      0x2A6B



/* Scan Parameter charatceristics */
#define GATT_UUID_SCAN_INT_WINDOW       0x2A4F
#define GATT_UUID_SCAN_REFRESH          0x2A31

#endif /*QL_NF3303_BLE_GATTDEFS_H*/
