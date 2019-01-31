/*=================================================================================================

    2015 (c) nFore Technology Co., Ltd.  All rights reserved.

    description : Header file for nFore BlueGate library
    version     : 0.0.0.0
    author      :

=================================================================================================*/

#ifndef __NFORE_BLUEGATE_H__
#define __NFORE_BLUEGATE_H__

#include <stdint.h>

#ifdef __cplusplus
 extern "C" {
#endif // __cplusplus

/*************************************************
**          Basic function type define          **
*************************************************/

#ifndef BD_ADDR_LEN
#define BD_ADDR_LEN                 (6)
#endif

#define MAX_DEVICE_NAME_SIZE        (249)
#define MAX_VERSION_SIZE            (21)

#define AVRCP_MAX_ATTR_STR_LEN      (255)
#define AVRCP_MAX_APP_SETTINGS      (8)
#define AVRCP_MAX_ELEM_ATTR_SIZE    (7)

#define MAX_PB_NAME_SIZE            (100)
#define MAX_PB_NUM_SIZE             (100)
#define MAX_PB_TIME_SIZE            (15)
#define MAX_PB_NUMBER               (5)         /* MAX PB number in each contact*/
#define MAX_PB_PATH_SIZE            (1024)

#define MAX_MAP_MSG_HANDLE_LEN      (21)

typedef uint8_t               nfUINT8;
typedef uint16_t              nfUINT16;
typedef uint32_t              nfUINT32;
typedef uint64_t              nfUINT64;
typedef int8_t                nfINT8;
typedef int16_t               nfINT16;
typedef int32_t               nfINT32;
typedef int64_t               nfINT64;
typedef int32_t               nfBOOLEAN;

/*  MIBenum charset */
/*  ref : http://www.iana.org/assignments/character-sets/character-sets.xhtml */
typedef enum
{
    MIBENUM_ISO_8859_1      =   4,
    MIBENUM_ISO_8859_2      =   5,
    MIBENUM_ISO_8859_3      =   6,
    MIBENUM_ISO_8859_4      =   7,
    MIBENUM_ISO_8859_5      =   8,
    MIBENUM_ISO_8859_6      =   9,
    MIBENUM_ISO_8859_7      =   10,
    MIBENUM_ISO_8859_8      =   11,
    MIBENUM_ISO_8859_9      =   12,
    MIBENUM_ISO_8859_10     =   13,
    MIBENUM_SHIFT_JIS       =   17,

    MIBENUM_ISO_2022_JP     =   40,

    MIBENUM_EUC_JP          =   18,
    MIBENUM_EUC_KR          =   38,
    MIBENUM_UTF_8           =   106,
    MIBENUM_GBK             =   113,
    MIBENUM_BIG5            =   2026,
    MIBENUM_GB2312          =   2025,
    MIBENUM_KOI8_R          =   2084,
}MIBENUM_CHARSET_TYPE;

/*  Bluetooth status's */
typedef enum
{
    NFBT_STATUS_SUCCESS = 0,
    NFBT_STATUS_FAIL,
    NFBT_STATUS_NOT_READY,
    NFBT_STATUS_NOMEM,
    NFBT_STATUS_BUSY,
    NFBT_STATUS_DONE,
    NFBT_STATUS_UNSUPPORTED,
    NFBT_STATUS_PARM_INVALID,
    NFBT_STATUS_UNHANDLED,
    NFBT_STATUS_AUTH_FAILURE,
    NFBT_STATUS_RMT_DEV_DOWN,
    NFBT_STATUS_AUTH_REJECTED,
    NFBT_STATUS_WRONG_STATUS
}bt_status;

/*  Bluetooth enable state */
typedef enum
{
    NFBT_BT_ENABLE = 0,
    NFBT_BT_DISABLE,
    NFBT_SYS_WARNING,
}ENABLE_STATES;


/*  Diagnostic responese */
/*implement to bt Diagnostic event*/
typedef enum
{
    NFBT_NOERR                      = 0x0,
    NFBT_CRITICAL_BASE              = 0x80,
    NFBT_CRITICAL_BT_PCM_IN_ERROR,
    NFBT_CRITICAL_SYS_A2DP_AUDIO_ERROR,
    NFBT_CRITICAL_SYS_RING_AUDIO_ERROR,
    NFBT_CRITICAL_SYS_SCO_AUDIO_ERROR,
    NFBT_CRITICAL_SYS_SCO_MIC_ERROR,
    NFBT_CRITICAL_BT_PCM_OUT_ERROR,

    NFBT_ERR_BASE                   = 0x100,
    NFBT_ERR_BT_HW_ERROR,
    NFBT_ERR_BT_HW_SEND_TIMEOUT,
    NFBT_ERR_BT_HW_RECV_INVALID,
    NFBT_ERR_MEM_CORRUPTION,
    NFBT_ERR_MEM_NOT_ENOUGH,
    NFBT_ERR_BT_ADDR_INVALID,
    NFBT_ERR_TACK_CREATE_ERROR,
    NFBT_ERR_BT_PCM_IN_BLOCKING,
    NFBT_ERR_BT_PCM_OUT_BLOCKING,
    NFBT_ERR_SYS_SCO_MIC_BLOCKING,
    NFBT_ERR_SYS_SCO_AUDIO_BLOCKING,
    NFBT_ERR_SYS_A2DP_AUDIO_BLOCKING,
    NFBT_ERR_SYS_RING_AUDIO_BLOCKING,
    NFBT_ERR_BT_IOR_BLOCKING,
    NFBT_ERR_BT_IOW_BLOCKING,
    NFBT_ERR_BT_RFCOMM_READ,
    NFBT_ERR_BT_RFCOMM_WRITE,

    NFBT_WARNING_BASE               = 0x200,
    NFBT_WARNING_SYS_REC_DAMAGE,
    NFBT_WARNING_BT_MUST_HW_RESET,
    NFBT_WARNING_SYS_REC_SAVE_FAILED,
    NFBT_WARNING_SYS_RINGTONE_FILE_DAMAGE,
    NFBT_WARNING_BT_CB_BLOCKING,
    NFBT_WARNING_BT_PCM_IN_XRUN,
    NFBT_WARNING_BT_PCM_OUT_XRUN,
    NFBT_WARNING_SYS_SCO_MIC_XRUN,
    NFBT_WARNING_SYS_SCO_AUDIO_XRUN,
    NFBT_WARNING_SYS_A2DP_AUDIO_XRUN,
    NFBT_WARNING_SYS_RING_AUDIO_XRUN,
    NFBT_WARNING_BT_ALLOW_SLEEP,
    NFBT_WARNING_BT_WAKE,
    NFBT_WARNING_BT_USERIAL_FD,

    NFBT_DBG_BT_BASE                = 0x400,
    NFBT_DBG_BT_TESTMODE,
    NFBT_DBG_SYS_A2DP_AUDIO_ACTIVE,
    NFBT_DBG_SYS_A2DP_AUDIO_RELEASE,
    NFBT_DBG_SYS_RING_AUDIO_ACTIVE,
    NFBT_DBG_SYS_RING_AUDIO_RELEASE,
    NFBT_DBG_SYS_SCO_AUDIO_ACTIVE,
    NFBT_DBG_SYS_SCO_AUDIO_RELEASE,
    NFBT_DBG_SYS_SCO_MIC_ACTIVE,
    NFBT_DBG_SYS_SCO_MIC_RELEASE,
    NFBT_DBG_SYS_SCO_AUDIO_REQUEST,
    NFBT_DBG_SYS_SCO_MIC_REQUEST,
    NFBT_DBG_SYS_A2DP_AUDIO_REQUEST,
    NFBT_DBG_SYS_RING_AUDIO_REQUEST,
    NFBT_DBG_SYS_A2DP_AUDIO_SILENCE,
    NFBT_DBG_BT_PCM_IN_REQUEST,
    NFBT_DBG_BT_PCM_IN_ACTIVE,
    NFBT_DBG_BT_PCM_IN_RELEASE,
    NFBT_DBG_BT_PCM_OUT_REQUEST,
    NFBT_DBG_BT_PCM_OUT_ACTIVE,
    NFBT_DBG_BT_PCM_OUT_RELEASE,

    NFBT_DBG_A310_BASE              = 0x800,
    NFBT_DBG_A310_CLEAR_PAIREDLIST,
    NFBT_DBG_A310_ENABLE_ECNR,
    NFBT_DBG_A310_DISABLE_ECNR,
    NFBT_DBG_A310_LMIC_LSPK,
    NFBT_DBG_A310_LMIC_RSPK,
    NFBT_DBG_A310_RMIC_LSPK,
    NFBT_DBG_A310_RMIC_RSPK,

}DIAG_ID;

/* Bluetooth scan mode */
typedef enum
{
    NFBT_SCAN_MODE_NONE = 0,
    NFBT_SCAN_MODE_CONNECTABLE,
    NFBT_SCAN_MODE_CONNECTABLE_DISCOVERABLE,
    NFBT_SCAN_MODE_SHUTDOWN,
    NFBT_SCAN_MODE_NONE_HOLDING,
}SCAN_MODE;

/*  Bluetooth pair states */
typedef enum
{
    NFBT_PAIR_STATE_NONE = 0,
    NFBT_PAIR_STATE_PAIRING,
    NFBT_PAIR_STATE_PAIRED
}PAIR_STATES;

/*  Bluetooth inquiry actions */
typedef enum
{
    NFBT_INQUIRY_STOPPED=0,
    NFBT_INQUIRY_STARTED
}INQUIRY_ACTION;

/*  Bluetooth inquiry states */
typedef enum
{
    NFBT_INQ_START = 0,
    NFBT_INQ_INQUIRING,
    NFBT_INQ_END,
    NFBT_LINK_RSSI,
}INQ_STATES;

/*  Bluetooth pair actions */
typedef enum
{
    NFBT_REJECT_PAIR = 0,
    NFBT_ACCEPT_PAIR
}PAIR_ACTION;

/** Passkey or pincode type use for pin request */
typedef enum {
    NFBT_LEGACY_PIN_REQ = 0,
    NFBT_SSP_PASSKEY_REQ = 1
}PAIR_PIN_TYPE;

/*  Bluetooth pair mode */
typedef enum
{
    NFBT_PAIR_FORWARD_REQ = 0,
    NFBT_PAIR_REJECT_REQ,
    NFBT_PAIR_ACCEPT_REQ
}PAIR_CTRL;

/*  Bluetooth Auto reconnect mode */
typedef enum
{
    NFBT_AUTORECONNECT_LAST_CONNECTED = 0,
    NFBT_AUTORECONNECT_ROTATION

}AUTO_RECONN_MODE;

/*  Bluetooth Paired device soft */
typedef enum
{
    NFBT_PAIREDSOFT_LAST_PAIRED_FIRST = 0,
    NFBT_PAIREDSOFT_LAST_CONNECTED_FIRST

}PAIRED_SOFT_MODE;

/*  Bluetooth disconnect reason */
typedef enum
{
    NFBT_DISCONNECT_REASON_NOT_FIND  = 0x04,
    NFBT_DISCONNECT_REASON_LINK_LOSS  = 0x08,
    NFBT_DISCONNECT_REASON_REMOTE_USER  = 0x13,
    NFBT_DISCONNECT_REASON_REMOTE_POWER_OFF  = 0x15,
}NFBT_DISCONNECT_REASON;

/*  Bluetooth profile index */
typedef enum
{
    NFBT_PINDEX_HFP             = 0,
    NFBT_PINDEX_A2DP_SINK       = 1,
    NFBT_PINDEX_AVRCP_CT        = 2,
    NFBT_PINDEX_PBAP_CLIENT     = 3,
    NFBT_PINDEX_MAP_CLIENT      = 4,
    NFBT_PINDEX_SPP             = 5,
    NFBT_PINDEX_IAP             = 6,
    NFBT_PINDEX_HIDD            = 7,
    NFBT_PINDEX_OPP             = 8,
    NFBT_PINDEX_A2DP_SRC        = 9,
    NFBT_PINDEX_AVRCP_TG        = 10,
    NFBT_PINDEX_DUN_CLIENT      = 11,
    NFBT_PINDEX_FTP_CLIENT      = 12,
    NFBT_PINDEX_AG              = 13,
    NFBT_PINDEX_MAP_SERVER      = 14,
    NFBT_PINDEX_RFCOMM_1        = 17,
    NFBT_PINDEX_RFCOMM_2        = 18,
    NFBT_PINDEX_RFCOMM_3        = 19,
    NFBT_PINDEX_RFCOMM_4        = 20,
    NFBT_PINDEX_RFCOMM_5        = 21,
    NFBT_PINDEX_RFCOMM_6        = 22,
    NFBT_PINDEX_RFCOMM_7        = 23,
    NFBT_PINDEX_RFCOMM_8        = 24,
    NFBT_PINDEX_RFCOMM_9        = 25,
    NFBT_PINDEX_GATT            = 26,
    NFBT_PINDEX_HSP_HS          = 27,

    NFBT_PINDEX_END,
    NFBT_PINDEX_ALL_PROFILE     = 0xffffffff
} PROFILE_INDEX;

/*  Bluetooth profile id */
typedef enum
{
    NFBT_PID_HFP                = 1 << NFBT_PINDEX_HFP,
    NFBT_PID_A2DP_SINK          = 1 << NFBT_PINDEX_A2DP_SINK,
    NFBT_PID_AVRCP_CT           = 1 << NFBT_PINDEX_AVRCP_CT,
    NFBT_PID_A2DP_SRC           = 1 << NFBT_PINDEX_A2DP_SRC,
    NFBT_PID_AVRCP_TG           = 1 << NFBT_PINDEX_AVRCP_TG,
    NFBT_PID_DUN_CLIENT         = 1 << NFBT_PINDEX_DUN_CLIENT,
    NFBT_PID_FTP_CLIENT         = 1 << NFBT_PINDEX_FTP_CLIENT,
    NFBT_PID_AG                 = 1 << NFBT_PINDEX_AG,
    NFBT_PID_OPP                = 1 << NFBT_PINDEX_OPP,
    NFBT_PID_PBAP_CLIENT        = 1 << NFBT_PINDEX_PBAP_CLIENT,
    NFBT_PID_MAP_CLIENT         = 1 << NFBT_PINDEX_MAP_CLIENT,
    NFBT_PID_MAP_SERVER         = 1 << NFBT_PINDEX_MAP_SERVER,
    NFBT_PID_HIDD               = 1 << NFBT_PINDEX_HIDD,
    NFBT_PID_SPP                = 1 << NFBT_PINDEX_SPP,
    NFBT_PID_IAP                = 1 << NFBT_PINDEX_IAP,
    NFBT_PID_RFCOMM_1           = 1 << NFBT_PINDEX_RFCOMM_1,
    NFBT_PID_RFCOMM_2           = 1 << NFBT_PINDEX_RFCOMM_2,
    NFBT_PID_RFCOMM_3           = 1 << NFBT_PINDEX_RFCOMM_3,
    NFBT_PID_RFCOMM_4           = 1 << NFBT_PINDEX_RFCOMM_4,
    NFBT_PID_RFCOMM_5           = 1 << NFBT_PINDEX_RFCOMM_5,
    NFBT_PID_RFCOMM_6           = 1 << NFBT_PINDEX_RFCOMM_6,
    NFBT_PID_RFCOMM_7           = 1 << NFBT_PINDEX_RFCOMM_7,
    NFBT_PID_RFCOMM_8           = 1 << NFBT_PINDEX_RFCOMM_8,
    NFBT_PID_RFCOMM_9           = 1 << NFBT_PINDEX_RFCOMM_9,
    NFBT_PID_GATT               = 1 << NFBT_PINDEX_GATT,
    NFBT_PID_HSP_HS             = 1 << NFBT_PINDEX_HSP_HS,
    NFBT_PID_END                = 1 << NFBT_PINDEX_END,
    NFBT_PID_ALL_PROFILE        = 0xffffffff
} PROFILE_ID;

/*  Bluetooth device address */
typedef struct {
    nfUINT8 address[BD_ADDR_LEN];
}BT_ADDR;

typedef struct
{
    nfUINT8                     version_size;
    nfUINT8                     version[MAX_VERSION_SIZE];
}BT_VERSION;


/*  Bluetooth local device information*/
typedef struct
{
    nfUINT8                     name_size;
    nfINT8                      name[MAX_DEVICE_NAME_SIZE];
    BT_ADDR                     addr;
    SCAN_MODE                   scan_mode;
}LOCAL_DEVICE_INFO;

/*  Bluetooth Paired device information*/
typedef struct
{
    nfUINT8                     name_size;
    nfUINT8                     name[MAX_DEVICE_NAME_SIZE];
    BT_ADDR                     addr;
}PAIRED_DEVICE_INFO;

/*  Bluetooth inquiry device information */
typedef struct
{
    nfUINT8                     name_size;
    nfUINT8                     name[MAX_DEVICE_NAME_SIZE];
    BT_ADDR                     addr;
    nfUINT32                    cod;
    nfINT16                     rssi;
}INQUIRY_DEVICE_INFO;

typedef nfUINT64 AVRCP_UID;

typedef enum
{
    AR_NONE,
    AR_BY_PRIORITY,
    AR_BY_RECENTLY,
    AR_BY_LIST,
    AR_BY_LINKLOSS,
}AR_MODE;

typedef enum
{
    NONE,
    UPDATE,
}UPDATE_NAME_ACTIOM;

typedef struct
{
    nfUINT16 BuffByteSize;       /*the size of pBuffer*/
    nfUINT16 ReturnByteNum;      /*actual returned byte number.*/
    nfUINT8* pBuffer;            /*actual data to be stored.*/
} NF_BT_BufferType;

/*
    callback function. If audio buffer is ready, call this function.
    if decode is OK, set DecodeResult to be 0.
*/
typedef void (*NF_BT_GetAudioBlockCbkType)(nfINT32 DecodeResult, const NF_BT_BufferType* pBuffer);


/**********************************************
**          HFP profile type define          **
**********************************************/
/*  Bluetooth hfp states */
typedef enum
{
    NFBT_HFP_DISCONNECTED = 0,
    NFBT_HFP_CONNECTING,
    NFBT_HFP_CONNECTED,
    NFBT_HFP_DISCONNECTING
}HFP_STATES;

/*  Bluetooth hfp call status*/
typedef enum {
    HFP_CALL_STATUS_INCOMING = 0,
    HFP_CALL_STATUS_OUTGOING,
    HFP_CALL_STATUS_ACTIVE,
    HFP_CALL_STATUS_TERMINATE,
    HFP_CALL_STATUS_HOLD,

    HFP_CALL_STATUS_MPTYCALLHOLD,
    HFP_CALL_STATUS_MPTYCALLACTIVE,
    HFP_CALL_STATUS_MPTYCALLTERMINATE
}HFP_CALL_STATUS;

typedef struct
{
    char            *number;
    unsigned char   idx;
    unsigned char   mpty;
}HFP_MPTY_INFO;


/*  Bluetooth audio connection states */
typedef enum
{
    HFP_AUDIO_DISCONNECTED = 0,
    HFP_AUDIO_CONNECTING,
    HFP_AUDIO_CONNECTED,
    HFP_AUDIO_DISCONNECTING,
    HFP_SCO_CONNECTED,
    HFP_SCO_DISCONNECTED
}HFP_AUDIO_CONNECTION_STATES;

/*  Bluetooth hfp audio trnasfer actions */
typedef enum
{
    HFP_AUDIO_HF_TO_AG = 0,
    HFP_AUDIO_AG_TO_HF
}HFP_AUDIO_TRANSFER_ACTION;

/*  Bluetooth hfp voice dial actions */
typedef enum
{
    HFP_VR_STOP = 0,
    HFP_VR_START
}HFP_VOICE_RECOGNITION_ACTION;

/*  Bluetooth hfp device notification(roaming, signal, battery charge, service) */
typedef enum
{
    HFP_ROAMING_STATE = 0,
    HFP_SIGNAL_STRENGTH,
    HFP_BATTCHG_STRENGTH,
    HFP_SERVICE_STATE,
    HFP_DEVICE_IMEI_NUMBER = 0xC0 
}HFP_DEVICE_NOTIFY_PROP;

/* Bluetooth hfp multi call control action*/
typedef enum {
    HFP_CALL_ACTION_CHLD_0 = 0, // Terminate all held or set UDUB("busy") to a waiting call
    HFP_CALL_ACTION_CHLD_1,     // Terminate all active calls and accepts a waiting/held call
    HFP_CALL_ACTION_CHLD_2,     // Hold all active calls and accepts a waiting/held call
    HFP_CALL_ACTION_CHLD_3,     // Add all held calls to a conference
    HFP_CALL_ACTION_CHLD_4,
    HFP_CALL_ACTION_CHLD_1x,
    HFP_CALL_ACTION_CHLD_2x
} HFP_CALL_ACTION_CHLD;

/* Bluetooth hfp subscriber info service type*/
typedef enum {
    HFP_SERVICE_UNKNOWN = 0,
    HFP_SERVICE_VOICE,
    HFP_SERVICE_FAX,
} HFP_SUBSCRIBER_SERVICE_TYPE;

/* Bluetooth hfp dsp control*/
typedef enum
{
    HFP_DSP_STOP = 0,
    HFP_DSP_START,
}HFP_DSP_CTRL;

/* Bluetooth hfp mic control*/
typedef enum
{
    HFP_MIC_UNMUTE = 0,
    HFP_MIC_MUTE,
}HFP_MIC_CTRL;

/* Bluetooth hfp speaker control sync*/
typedef enum
{
    HFP_SPK_USE_REQ,              /*speaker use request*/
    HFP_SPK_REL_RSP,              /*speaker release responese*/
}HFP_SPK_SYNC_CTRL;

/* Bluetooth hfp command ID */
typedef enum
{
    HFP_CMD_ANSWER = 0,
    HFP_CMD_REJECT_OR_TERMINATE,
    HFP_CMD_DIAL,
    HFP_CMD_REDIAL,
    HFP_CMD_VOICE_RECONGNITION,
    HFP_CMD_SEND_DTMF,
    HFP_CMD_CALL_HOLD,
    HFP_CMD_SPK_VOLUME,
    HFP_CMD_MIC_VOLUME,
    HFP_CMD_SUBSCRIBER_INFO,
    HFP_CMD_SET_XAPL,
    HFP_CMD_SET_APLEFM,
    HFP_CMD_MANUFACTURER_IDENTIFICATION,
    HFP_CMD_MANUFACTURER_MODEL_IDENTIFICATION,
    HFP_CMD_GET_IMEI_NUMBER,
    HFP_CMD_END,
}HFP_COMMAND_ID;

/* Bluetooth hfp volume type */
typedef enum {
    HFP_VOLUME_TYPE_SPK = 0,
    HFP_VOLUME_TYPE_MIC
} HFP_VOLUME_TYPE;

/* Bluetooth hfp auto reaction */
typedef enum {
    HF_AUTO_NO_ACTION,
    HF_AUTO_ANSWER,
    HF_AUTO_REJECT,
    HF_AUTO_REJECT_3WAY,
} HFP_AUTO_REACTION;

/*  Bluetooth hfp Apple eyes free mode actions */
typedef enum
{
    HFP_APLEFM_STOP = 0,
    HFP_APLEFM_START
}HFP_APLEFM_ACTION;

/**********************************************
**          HSP profile type define          **
**********************************************/
/*  Bluetooth hsp states */
typedef enum
{
    NFBT_HSP_DISCONNECTED = 0,
    NFBT_HSP_CONNECTING,
    NFBT_HSP_CONNECTED,
    NFBT_HSP_DISCONNECTING
}HSP_STATES;

/* Bluetooth hsp command ID */
typedef enum
{
    // HSP_CMD_ANSWER = 0,
    
    HSP_CMD_SPK_VOLUME,
    HSP_CMD_MIC_VOLUME,
    HSP_CMD_SEND_CKPD,

    HSP_CMD_END,
}HSP_COMMAND_ID;

/*  Bluetooth hsp audio connection states */
typedef enum
{
    HSP_AUDIO_DISCONNECTED = 0,
    HSP_AUDIO_CONNECTING,
    HSP_AUDIO_CONNECTED,
    HSP_AUDIO_DISCONNECTING
}HSP_AUDIO_CONNECTION_STATES;

/* Bluetooth hsp volume type */
typedef enum {
    HSP_VOLUME_TYPE_SPK = 0,
    HSP_VOLUME_TYPE_MIC
} HSP_VOLUME_TYPE;

/***********************************************
**          A2DP profile type define          **
***********************************************/
/*  Bluetooth a2dp stats */
typedef enum
{
    NFBT_A2DP_CONN_STATE_DISCONNECTED,
    NFBT_A2DP_CONN_STATE_CONNECTING,
    NFBT_A2DP_CONN_STATE_DISCONNECTING,
    NFBT_A2DP_CONN_STATE_CONNECTED,
}A2DP_CONN_STATES;

typedef enum
{
    NFBT_A2DP_AUDIO_STATE_STREAMING,
    NFBT_A2DP_AUDIO_STATE_STOPPED,
}A2DP_AUDIO_STATES;

/************************************************
**          AVRCP profile type define          **
************************************************/
/*  Bluetooth avrcp states  */
typedef enum
{
    NFBT_AVRCP_STATE_DISCONNECTED,
    NFBT_AVRCP_STATE_CONNECTED,
}AVRCP_CONN_STATES;

/*  Bluetooth avrcp control id  */
typedef enum
{
    AVRCP_OPID_VOL_UP = 0x41,
    AVRCP_OPID_VOL_DOWN,
    AVRCP_OPID_MUTE,
    AVRCP_OPID_PLAY,
    AVRCP_OPID_STOP,
    AVRCP_OPID_PAUSE,
    AVRCP_OPID_RECORD,/*Reserved, not use*/
    AVRCP_OPID_REWIND,
    AVRCP_OPID_FAST_FORWARD,
    AVRCP_OPID_EJECT,/*Reserved, not use*/
    AVRCP_OPID_FORWARD,
    AVRCP_OPID_BACKWARD,

    AVRCP_OPID_REWIND_STOP = 0x100,
    AVRCP_OPID_FAST_FORWARD_STOP = 0x101,
}AVRCP_OP_ID;

/*  Bluetooth avrcp button actions  */
typedef enum
{
    AVRCP_BUTTON_PRESS,
    AVRCP_BUTTON_RELEASE,
    AVRCP_BUTTON_PRESS_AND_RELEASE
}AVRCP_BUTTON_STATUS;

/*  Remote device support version  */
typedef enum
{
    AVRCP_FEATURE_PASSTHROUGH = 1,
    AVRCP_FEATURE_META        = 2,
    AVRCP_FEATURE_BROWSE      = 4,
}AVRCP_FEATURE;

/*====AVRCP1.3====*/
/*  Group Control  */
typedef enum
{
    AVRCP_GROUP_NEXT = 0,
    AVRCP_GROUP_PREV
}AVRCP_GROUP_CONTROL;

/*  Play status  */
typedef enum
{
    AVRCP_PLAYSTATE_STOPPED  = 0x00,    /* Stopped */
    AVRCP_PLAYSTATE_PLAYING  = 0x01,    /* Playing */
    AVRCP_PLAYSTATE_PAUSED   = 0x02,    /* Paused  */
    AVRCP_PLAYSTATE_FWD_SEEK = 0x03,    /* Fwd Seek*/
    AVRCP_PLAYSTATE_REV_SEEK = 0x04,    /* Rev Seek*/
    AVRCP_PLAYSTATE_ERROR    = 0xFF,    /* Error   */
}AVRCP_PLAY_STATE;

/*  Player attribute support type  */
typedef enum
{
    AVRCP_PLAYER_ATTR_EQUALIZER = 0x01,
    AVRCP_PLAYER_ATTR_REPEAT    = 0x02,
    AVRCP_PLAYER_ATTR_SHUFFLE   = 0x03,
    AVRCP_PLAYER_ATTR_SCAN      = 0x04,
}AVRCP_PLAYER_ATTR;

typedef enum
{
    AVRCP_EQUALIZER_OFF = 0x01,
    AVRCP_EQUALIZER_ON,
}AVRCP_EQUALIZER_VAL;

/*  Repeat setting value  */
typedef enum
{
    AVRCP_REPEAT_OFF    = 0x01,
    AVRCP_REPEAT_SINGLE = 0x02,
    AVRCP_REPEAT_ALL    = 0x03,
    AVRCP_REPEAT_GROUP  = 0x04
}AVRCP_REPEAT_VAL;

/*  Shuffle setting value  */
typedef enum
{
    AVRCP_SHUFFLE_OFF   = 0x01,
    AVRCP_SHUFFLE_ALL   = 0x02,
    AVRCP_SHUFFLE_GROUP = 0x03
}AVRCP_SHUFFLE_VAL;

typedef enum
{
    AVRCP_SCAN_OFF       = 0x01,
    AVRCP_SCAN_ALL_TRACK = 0x02,
    AVRCP_SCAN_GROUP     = 0x03
}AVRCP_SCAN_VAL;

typedef enum
{
    AVRCP_BAD_CMD        = 0x00, /* Invalid command */
    AVRCP_BAD_PARAM      = 0x01, /* Invalid parameter */
    AVRCP_NOT_FOUND      = 0x02, /* Specified parameter is wrong or not found */
    AVRCP_INTERNAL_ERR   = 0x03, /* Internal Error */
    AVRCP_NO_ERROR       = 0x04, /* Operation Success */
    AVRCP_UID_CHANGED    = 0x05, /* UID Changed */
    AVRCP_BAD_DIR        = 0x07, /* Invalid Direction */
    AVRCP_NOT_DIR        = 0x08, /* Not a Directory */
    AVRCP_NOT_EXIST      = 0x09, /* Does Not Exist */
    AVRCP_BAD_SCOPE      = 0x0a, /* Invalid Scope  */
    AVRCP_BAD_RANGE      = 0x0b, /* Range Out of Bounds */
    AVRCP_UID_IS_DIR     = 0x0c, /* UID is a Directory */
    AVRCP_IN_USE         = 0x0d, /* Media in Use */
    AVRCP_NOW_LIST_FULL  = 0x0e, /* Now Playing List Full */
    AVRCP_SEARCH_NOT_SUP = 0x0f, /* Search Not Supported */
    AVRCP_SEARCH_BUSY    = 0x10, /* Search in Progress */
    AVRCP_BAD_PLAYER_ID  = 0x11, /* Invalid Player Id */
    AVRCP_PLAYER_N_BR    = 0x12, /* Player Not Browsable */
    AVRCP_PLAYER_N_ADDR  = 0x13, /* Player Not Addressed */
    AVRCP_BAD_SEARCH_RES = 0x14, /* No valid Search Results */
    AVRCP_NO_AVAL_PLAYER = 0x15, /* No available players ALL */
    AVRCP_ADDR_PLAYER_CHG= 0x16, /* Addressed Player Changed */
}AVRCP_RSP_STATUS;

/* Define the command ID */
typedef enum
{
    AVRCP_CMD_GET_CAPABILITIES             = 0x10,
    AVRCP_CMD_LIST_PLAYER_APP_ATTR         = 0x11,
    AVRCP_CMD_LIST_PLAYER_APP_VALUES       = 0x12,
    AVRCP_CMD_GET_CUR_PLAYER_APP_VALUE     = 0x13,
    AVRCP_CMD_SET_PLAYER_APP_VALUE         = 0x14,
    AVRCP_CMD_GET_ELEMENT_ATTR             = 0x20,
    AVRCP_CMD_GET_PLAY_STATUS              = 0x30,
    AVRCP_CMD_REGISTER_NOTIFICATION        = 0x31,
    AVRCP_CMD_SET_ADDRESSED_PLAYER         = 0x60,
    AVRCP_CMD_SET_BROWSED_PLAYER           = 0x70,
    AVRCP_CMD_GET_FOLDER_ITEMS             = 0x71,
    AVRCP_CMD_CHANGE_PATH                  = 0x72,
    AVRCP_CMD_GET_ITEM_ATTRIBUTES          = 0x73,
    AVRCP_CMD_PLAY_ITEM                    = 0x74,
}AVRCP_COMMAND_ID;

/* Response code */
typedef enum
{
    AVRCP_RSP_NO_RESPONSE       = 0x0,
    AVRCP_RSP_NOT_IMPLEMENTED   = 0x8,  /* not implemented */
    AVRCP_RSP_ACCEPTED          = 0x9,  /* "accepted" control command */
    AVRCP_RSP_REJECTED          = 0xA,  /* rejected command */
    AVRCP_RSP_IN_TRANSACTION    = 0xB,  /* in a state of transaction */
    AVRCP_RSP_STABLE            = 0xC,  /* "accepted" status command */
    AVRCP_RSP_CHANGED           = 0xD,  /* state has changed for notify */
    AVRCP_RSP_INTERIM           = 0xF,  /* accepted register event notify */
}AVRCP_RSP_CODE;

/*  Notification event id  */
typedef enum
{
    AVRCP_EVT_PLAY_STATUS_CHANGED   = 0x01,
    AVRCP_EVT_TRACK_CHANGE          = 0x02,
    AVRCP_EVT_TRACK_REACHED_END     = 0x03,
    AVRCP_EVT_TRACK_REACHED_START   = 0x04,
    AVRCP_EVT_PLAY_POS_CHANGED      = 0x05,
    AVRCP_EVT_BATT_STATUS_CHANGED   = 0x06,
    AVRCP_EVT_SYSTEM_STATUS_CHANGED = 0x07,
    AVRCP_EVT_APP_SETTINGS_CHANGED  = 0x08,
    AVRCP_EVT_NOWPLAYING_CONTENT_CHANGED = 0x09,
    AVRCP_EVT_AVAILABLE_PLAYERS_CHANGED = 0x0a,
    AVRCP_EVT_ADDRESSED_PLAYER_CHANGED = 0x0b,
    AVRCP_EVT_UIDS_CHANGED          = 0x0c,
    AVRCP_EVT_VOLUME_CHANGED        = 0x0d,
    AVRCP_EVT_MAX,
    AVRCP_EVT_PLAY_STATUS_PREFER    = 0xF0, /*Rrefer play status not bluetooth standard*/
    AVRCP_EVT_VOLUME_NOTIFICATION   = 0xF1, /*Recvice Remote CT Volume control*/
}AVRCP_EVENT_ID;

typedef enum
{
    AVRCP_ELEMENT_ATTR_TITLE        = 0x01,
    AVRCP_ELEMENT_ATTR_ARTIST       = 0x02,
    AVRCP_ELEMENT_ATTR_ALBUM        = 0x03,
    AVRCP_ELEMENT_ATTR_TRACK_NUMBER = 0x04,
    AVRCP_ELEMENT_ATTR_TRACKS       = 0x05,
    AVRCP_ELEMENT_ATTR_GENRE        = 0x06,
    AVRCP_ELEMENT_ATTR_PLAYING_TIME = 0x07,
}AVRCP_ELEMENT_ID;


/*  Notification response type  */
typedef enum
{
    AVRCP_NOTIFICATION_TYPE_INTERIM = 0,
    AVRCP_NOTIFICATION_TYPE_CHANGED = 1,
}AVRCP_NOTIFICATION_TYPE;

typedef enum {
    AVRCP_SCOPE_PLAYER_LIST = 0x00,
    AVRCP_SCOPE_FILE_SYSTEM = 0x01,
    AVRCP_SCOPE_SEARCH      = 0x02,
    AVRCP_SCOPE_NOW_PLAYING = 0x03
} AVRCP_SCOPE_TYPE;

typedef enum {
    AVRCP_PATH_UP   = 0x00,
    AVRCP_PATH_DOWN = 0x01,
} AVRCP_CHANGE_PATH;

/* Player Item Type ID*/
typedef enum
{
    AVRCP_PLAYERITEM_TYPE_AUDIO                 = 0x01,
    AVRCP_PLAYERITEM_TYPE_VIDEO                 = 0x02,
    AVRCP_PLAYERITEM_TYPE_BROADCASTING_AUDIO    = 0x04,
    AVRCP_PLAYERITEM_TYPE_BROADCASTING_VIDEO    = 0x08,
}AVRCP_PLAYERITEM_TYPE;

/* Player Item Sub Type ID*/
typedef enum
{
    AVRCP_PLAYERITEM_SUBTYPE_AUDIO_BOOK     = 0x01,
    AVRCP_PLAYERITEM_SUBTYPE_PODCAST,
}AVRCP_PLAYERITEM_SUBTYPE;

/* Folder Item Type ID*/
typedef enum
{
    AVRCP_FOLDERITEM_TYPE_MIXED     = 0x00,
    AVRCP_FOLDERITEM_TYPE_TITLES,
    AVRCP_FOLDERITEM_TYPE_ALBUMS,
    AVRCP_FOLDERITEM_TYPE_ARTISTS,
    AVRCP_FOLDERITEM_TYPE_GENRES,
    AVRCP_FOLDERITEM_TYPE_PLAYLISTS,
    AVRCP_FOLDERITEM_TYPE_YEARS,
}AVRCP_FOLDERITEM_TYPE;

/* Media Item Type ID*/
typedef enum
{
    AVRCP_MEDIAITEM_TYPE_AUDIO      = 0x01,
    AVRCP_MEDIAITEM_TYPE_VIDEO      = 0x02,
}AVRCP_MEDIAITEM_TYPE;

typedef struct
{
    nfUINT8 count; /* number of support events */
    nfUINT8 events[AVRCP_EVT_MAX];/* refer Notification event id*/
}AVRCP_CAPABILITY;

typedef struct
{
    nfUINT8         count;
    nfUINT32        attr_id[AVRCP_MAX_ELEM_ATTR_SIZE];
    nfUINT16        text_len[AVRCP_MAX_ELEM_ATTR_SIZE];
    nfUINT16        char_set[AVRCP_MAX_ELEM_ATTR_SIZE];
    nfINT8          text[AVRCP_MAX_ELEM_ATTR_SIZE][AVRCP_MAX_ATTR_STR_LEN];
}AVRCP_ELEMENT_ATTR_STRUCT;

/* Battery status */
typedef enum {
    AVRCP_BATTERY_STATUS_NORMAL = 0x00,
    AVRCP_BATTERY_STATUS_WARNING,
    AVRCP_BATTERY_STATUS_CRITICAL,
    AVRCP_BATTERY_STATUS_EXTERNAL,
    AVRCP_BATTERY_STATUS_FULL_CHARGE,
} AVRCP_BATTERY_STATUS;

/*  Player setting struct  */
typedef struct
{
    nfUINT8 num_attr;
    nfUINT8 attr_ids[AVRCP_MAX_APP_SETTINGS];
    nfUINT8 attr_values[AVRCP_MAX_APP_SETTINGS];
}AVRCP_PLAYER_SETTING;

typedef struct
{
    nfUINT16 player_id;
    nfUINT16 uid_counter;
}AVRCP_ADDR_PLAYER;

typedef struct
{
    AVRCP_PLAY_STATE        play_status;
    AVRCP_UID               track; /* queue position in NowPlaying */
    nfUINT32                song_pos;
    AVRCP_BATTERY_STATUS    battery_status;
    AVRCP_PLAYER_SETTING    player_setting;
    nfINT8                  AbsoluteVolume;
    AVRCP_ADDR_PLAYER       addr_player;
	nfUINT16    			uid_counter; 
}AVRCP_NOTIFICATION;

#define AVRCP_UNKNOWN_TIME 0xffffffff

typedef struct
{
    nfUINT16    uid_counter;                        /**< UID counter */
    nfUINT16    charset_id;                         /**< Character set ID */
    nfUINT8     num_items;                          /**< Number of items */
    nfUINT8     folder_depth;                       /**< Folder depth */
    char        name[AVRCP_MAX_ATTR_STR_LEN];       /**< Player name */
}AVRCP_BROWSED_FOLDER;

/** Attribute of player item */
typedef struct{
    nfUINT16                    uid_counter;                        /**< UID counter */
    nfUINT16                    player_id;                          /**< Player ID */
    AVRCP_PLAYERITEM_TYPE       type;                               /**< Type */
    AVRCP_PLAYERITEM_SUBTYPE    subtype;                            /**< Sub type */
    AVRCP_PLAY_STATE            play_status_id;                     /**< Play status ID */
    MIBENUM_CHARSET_TYPE        charset_id;                         /**< Character set ID */
    char                        name[AVRCP_MAX_ATTR_STR_LEN];       /**< Player name */
}AVRCP_PLAYERITEM;

typedef struct
{
    nfUINT16                uid_counter;        /**< UID counter */
    AVRCP_UID               uid;                /**< UID */
    AVRCP_FOLDERITEM_TYPE   type;               /**< Type */
    nfBOOLEAN               playable;           /**< Is folder playable */
    MIBENUM_CHARSET_TYPE    charset_id;         /**< Character set ID */
    char                    name[AVRCP_MAX_ATTR_STR_LEN];   /**< Directory name */
}AVRCP_FOLDERITEM;

typedef struct
{
    nfUINT16                uid_counter;        /**< UID counter */
    AVRCP_UID               uid;                /**< UID */
    AVRCP_MEDIAITEM_TYPE    type;               /**< Type */
    MIBENUM_CHARSET_TYPE    charset_id;         /**< Character set ID */
    char                    name[AVRCP_MAX_ATTR_STR_LEN];   /**< Media name */
    AVRCP_ELEMENT_ATTR_STRUCT *attr;
}AVRCP_MEDIAITEM;

/***********************************************
**         PBAP profile type define           **
***********************************************/
/*  Bluetooth phone book download states */
typedef enum
{
    NFBT_PBDL_START = 0,
    NFBT_PBDL_DLING,
    NFBT_PBDL_END,
    NFBT_PBDL_FAIL,
    NFBT_PBDL_COMPLETE,
    NFBT_PBDL_CANCEL,
    NFBT_PBDL_NOTFIND,
}PB_DOWNLOAD_STATES;

/* Bluetooth phone book download with photo or not */
typedef enum
{
    PB_PHOTO_OFF = 0,
    PB_PHOTO_ON,
}PB_PHOTO_ONOFF;

/*  Bluetooth phone book download  access types */
typedef enum
{
    PB_ACCESS_NULL = 0,
    PB_ACCESS_SIM,
    PB_ACCESS_PHONE,
    PB_ACCESS_MISSED,
    PB_ACCESS_RECEIVED,
    PB_ACCESS_DIALED,
    PB_ACCESS_COMBINE_CALLLOG,
    PB_ACCESS_COMBINE_PHONEBOOK,
    PB_ACCESS_COMBINE_PB_CALLLOG,
}PB_ACCESS;
typedef enum
{
    PB_CONTACT_SIM,
    PB_CONTACT_PHONE,
    PB_CALLLOG_MISSED,
    PB_CALLLOG_RECEIVED,
    PB_CALLLOG_DIALED,
    PB_CALLLOG_UNKNOWN,
}PB_CONTACT_TYPE;

/*  Bluetooth phone book number types */
typedef enum
{
    PB_NULL,
    PB_PREF,
    PB_WORK,
    PB_HOME,
    PB_VOICE,
    PB_FAX,
    PB_MSG,
    PB_CELL,
    PB_PAGER,
}PB_PHONE_NUMBER_TYPE;

/*  Bluetooth phone book download attributes */
typedef enum
{
    PB_NAME = 0,
    PB_NUM
}PB_DOWNLOAD_ATTRIBUTE;

/*  Bluetooth phone book number struct define */
typedef struct
{
    nfUINT8                 number_id;
    nfUINT8                 number_size;
    nfUINT8                 *number;
    PB_PHONE_NUMBER_TYPE    type;
}PB_NUMBER_STRUCT;

/* Bluetooth phone boot photo type */
typedef enum
{
    PB_PHOTO_NULL,
    PB_PHOTO_URI,
    PB_PHOTO_URL,
    PB_PHOTO_JPEG,
    PB_PHOTO_GIF,
}PB_PHOTO_TYPE;

/* Bluetooth phone book photo struct define */
typedef struct
{
    nfUINT16                len;
    PB_PHOTO_TYPE           type;
    nfUINT8                 *path;
}PB_PHOTO_STRUCT;

/*  Bluetooth phone book struct define */
typedef struct
{
    nfUINT16                contacts_id;
    MIBENUM_CHARSET_TYPE    charset_id;
    nfUINT8                 first_name_size;
    nfUINT8                 *first_name;
    nfUINT8                 middle_name_size;
    nfUINT8                 *middle_name;
    nfUINT8                 last_name_size;
    nfUINT8                 *last_name;
    nfUINT8                 prefix_name_size;
    nfUINT8                 *prefix_name;
    nfUINT8                 suffix_name_size;
    nfUINT8                 *suffix_name;
    nfUINT8                 full_name_size;
    nfUINT8                 *full_name;
    nfUINT8                 number_count;
    PB_NUMBER_STRUCT        *phone_number;
    nfUINT8                 *timestemp;
    PB_CONTACT_TYPE         contact_type;
    PB_PHOTO_STRUCT         photo;
    nfUINT8                 first_name_phonetic_size;
    nfUINT8                 *first_name_phonetic;
    nfUINT8                 last_name_phonetic_size;
    nfUINT8                 *last_name_phonetic;    
}PB_STRUCT;

/***********************************************
**          MAP profile type define           **
***********************************************/
/* MAS instance description */
typedef struct
{
    int  id;
    int  scn;
    int  psm;
    int  msg_types;
    char *p_name;
}MAP_MAS_INSTANCE;

/*  Bluetooth phone book download states */
typedef enum
{
    MAP_DOWNLOAD_START = 0,
    MAP_DOWNLOAD_DLING,
    MAP_DOWNLOAD_END,
    MAP_DOWNLOAD_FAIL,
    MAP_DOWNLOAD_COMPLETE,
}MAP_DOWNLOAD_STATES;

/*  Bluetooth phone book download states */
typedef enum
{
    MAP_NOTIFICATION_OFF = 0,
    MAP_NOTIFICATION_ON,
}MAP_NOTIFICATION_STATES;

/* MAP folder structure */
typedef enum
{
    MAP_FOLDER_STRUCTURE_INBOX = 0,
    MAP_FOLDER_STRUCTURE_SENT,
    MAP_FOLDER_STRUCTURE_DELETED,
    MAP_FOLDER_STRUCTURE_OUTBOX,
    MAP_FOLDER_STRUCTURE_DRAFT,
}MAP_FOLDER_STRUCT;

/* MAP message type */
typedef enum
{
    MAP_MSG_TYPE_SMS_GSM = 0,
    MAP_MSG_TYPE_SMS_CDMA,
    MAP_MSG_TYPE_MMS,
    MAP_MSG_TYPE_EMAIL,
}MAP_MSG_TYPE;

/* MAP send command status */
typedef enum
{
    MAP_STATUS_SUCCESS = 0,
    MAP_STATUS_BAD_PARAMS,
    MAP_STATUS_TIME_OUT,
    MAP_STATUS_FAIL,
}MAP_COMMAND_STATUS;

/* MAP event report */
typedef enum
{
    MAP_EVENT_NEW_MESSAGE = 1,
    MAP_EVENT_DELIVERY_SUCCESS,
    MAP_EVENT_SENDING_SUCCESS,
    MAP_EVENT_DELIVERY_FAILURE,
    MAP_EVENT_SENDING_FAILURE,
    MAP_EVENT_MEMORY_FULL,
    MAP_EVENT_MEMORY_AVAILABLE,
    MAP_EVENT_MESSAGE_DELETED,
    MAP_EVENT_MESSAGE_SHIFT,
}MAP_EVENT_REPORT;

/* MAP message structure */
typedef struct
{
    nfUINT8             *msg_handle;
    nfUINT8             msg_handle_len;
    nfUINT8             *sender_name;
    nfUINT8             sender_name_len;
    nfUINT8             *sender_addr;
    nfUINT8             sender_addr_len;
    nfUINT8             *recipient_addr;
    nfUINT8             recipient_addr_len;
    nfUINT8             *date;
    nfUINT8             date_len;
    MAP_MSG_TYPE        msg_type;
    MAP_FOLDER_STRUCT   folder;
    nfUINT8             *subject;
    nfUINT8             subject_len;
    nfUINT8             *msg;
    nfUINT16            msg_len;
    nfUINT8             read_status;
    nfUINT32            curr_count;
}MAP_SMS_GSM;

/**********************************************
**          RFCOMM profile type define          **
**********************************************/
/*  Bluetooth RFCOMM states */
typedef enum
{
    NFBT_RFCOMM_DISCONNECTED = 0,
    NFBT_RFCOMM_CONNECTED,
}RFCOMM_STATES;

/************************************************
**          HID profile type define          **
************************************************/
/*  Bluetooth hid states  */
typedef enum
{
    NFBT_HID_STATE_DISCONNECTED,
    NFBT_HID_STATE_CONNECTING,
    NFBT_HID_STATE_CONNECTED,
    NFBT_HID_STATE_DISCONNECTING,
}HID_CONN_STATES;

typedef enum
{
    NFBT_HID_KEY_MENU,
    NFBT_HID_KEY_HOME,
    NFBT_HID_KEY_BACK,
    NFBT_HID_KEY_RELEASE,
}HID_KEY;

/***********************************************
**              OPP type define               **
***********************************************/
typedef enum
{
    NFBT_OPP_START = 0,
    NFBT_OPP_DLING,
    NFBT_OPP_ULING,
    NFBT_OPP_END,
    NFBT_OPP_COMPLETE,
    NFBT_OPP_FAIL,
}OPP_STATES;

typedef enum {
    NFBT_OPP_DOWNLOAD_ALLOW = 0,
    NFBT_OPP_DOWNLOAD_REJECT
}OPP_DOWNLOAD_REPLY;

/* MAP message structure */
typedef struct
{
    nfUINT8             *file_name;
    nfUINT8             *file_type;
}OPP_FILE_INFO;

#define DUT_FREQ_MIN    2402
#define DUT_FREQ_MAX    2480
#define DUT_FREQ_MID    2440

#define DUT_STOP_TEST   0x00
#define DUT_RX_MODE     0x01
#define DUT_TX_MODE 0x02
#define DUT_TX_MOD_MODE 0x03

#define DUT_POWER_MIN   0
#define DUT_POWER_MAX   7

typedef enum
{
	GATT_ADV_SPEED_SLOW = 0,
	GATT_ADV_SPEED_NORMAL,
	GATT_ADV_SPEED_FAST,

}GATT_ADV_SPEED;
typedef enum
{
	GATT_CONN_SPEED_SLOW = 0, //1.28~1s
	GATT_CONN_SPEED_NORMAL,   //500~300ms
	GATT_CONN_SPEED_FAST,     //100~150ms
	GATT_CONN_SPEED_TOP       //30ms~60ms

}GATT_CONN_LEVEL;

typedef enum 
{
    NFBT_GATT_SUCCESS = 0,
    NFBT_GATT_FAIL,
    NFBT_GATT_UNSUPPORTED,
    NFBT_GATT_BUSY,
    NFBT_GATT_INVALID_PARAMETERS,
    NFBT_GATT_INSUFF_PERMIT,
    NFBT_GATT_WRITE_NOT_PERMIT,
    NFBT_GATT_READ_NOT_PERMIT,
    NFBT_GATT_SERVICE_STATUS_WRONG,
    NFBT_GATT_CONN_STATUS_WRONG,
} GATT_STATUS;

/* connection status used for CLIENT/SERVER connection status */
typedef enum
{
    GATT_CONN_DISCONNECT = 0,
    GATT_CONN_START_LISTENING = 1, /* ONLY PERIPHERAL(SERVER) TO USED */
    GATT_CONN_LISTENING = 2, /* ONLY PERIPHERAL(SERVER) TO USED */
    GATT_CONN_CONNECTING = 3,
    GATT_CONN_CONNECTED = 4,

    GATT_CONN_SRV_REG_ING = 5, /* ONLY PERIPHERAL(SERVER) TO USED */
    GATT_CONN_SRV_UNREG_ING = 6 /* ONLY PERIPHERAL(SERVER) TO USED */
} GATT_CONN_STATUS;

typedef enum
{
    GATT_SERVICE_STOP = 0,
    GATT_SERVICE_START = 1
} GATT_SERVICE_STATUS;

/* role status */
typedef enum
{
    GATT_ROLE_PERIPHERAL = 0,
    GATT_ROLE_CENTRAL = 1
} GATT_ROLE;

#define ATTRTYPE_CHARACTERISTIC 1
#define ATTRTYPE_DESCRIPTOR     2

/* GATT characteristic property/permit definition */
#define GATT_PERMIT_READ        0x0001
#define GATT_PERMIT_READ_ENCRYPTED      0x0002
#define GATT_PERMIT_READ_ENC_MITM       0x0004
#define GATT_PERMIT_WRITE       0x0010
#define GATT_PERMIT_WRITE_ENCRYPTED 0x0020
#define GATT_PERMIT_WRITE_ENC_MITM      0x0040
#define GATT_PERMIT_WRITE_SIGNED        0x0080
#define GATT_PERMIT_WRITE_SIGNED_MITM   0x0100

#define GATT_PROP_READ          0x0002
#define GATT_PROP_WRITE_NR  0x0004
#define GATT_PROP_WRITE     0x0008
#define GATT_PROP_NOTIFY        0x0010
#define GATT_PROP_INDICATE      0x0020
#define GATT_PROP_EXT_PROP  0x0080

typedef enum
{
    SIG_SRV_BATTERY = 0,
    SIG_SRV_BPM,


    N4_SRV_SPP,
    SIG_SRV_MAX = N4_SRV_SPP,

    SIG_SRV_NONE = 0xffff
} srv_index_e;

/* Blood Pressure Feature support */
#define BPM_BODY_MOV_DETECT_SUP 0x0001
#define BPM_CUFF_FIT_DETECT_SUP 0x0002
#define BPM_IRREGULAR_PULSE_DETECT_SUP  0x0004
#define BPM_PULSE_RATE_RANGE_DETECT_SUP 0x0008
#define BPM_MEASUREMENT_POSITION_DETECT_SUP 0x0010
#define BPM_MULTIPLE_BONDS_SUP  0x0020

#define BPM_FEATURE_SUPPORT (BPM_BODY_MOV_DETECT_SUP|BPM_MEASUREMENT_POSITION_DETECT_SUP)

/********************************************************
**          Basic CBK function pointer define          **
********************************************************/
typedef void(*NFCBK_LocalDeviceInfo)(LOCAL_DEVICE_INFO *info);
typedef void(*NFCBK_PariedDeviceInfo)(nfUINT8 total, PAIRED_DEVICE_INFO *info);
typedef void(*NFCBK_InquiryStates)(INQ_STATES inq_state, INQUIRY_DEVICE_INFO *info);
typedef void(*NFCBK_PairStates)(BT_ADDR *addr, PAIR_STATES states);
typedef void(*NFCBK_DiagnosticMessage)(DIAG_ID code, void* data);
typedef void(*NFCBK_PairingRequest)(BT_ADDR *addr, nfUINT8 name_size, nfUINT8 *name, PAIR_PIN_TYPE type);
typedef void(*NFCBK_SspRequest)(BT_ADDR *addr, nfUINT8 name_size, nfUINT8 *name, nfUINT32 passkey, nfBOOLEAN display_only);
typedef void(*NFCBK_Disconnect_Reason)(BT_ADDR *addr, NFBT_DISCONNECT_REASON reason);
typedef void(*NFCBK_BtEnableState)(ENABLE_STATES state);

/******************************************************
**          HFP CBK function pointer define          **
******************************************************/
typedef void (*NFCBK_HFP_ConnectStates)(BT_ADDR *addr, HFP_STATES hfp_state);
typedef void (*NFCBK_HFP_CallStatus)(BT_ADDR *addr, HFP_CALL_STATUS call_status, nfUINT8* number, nfUINT8 idx);
typedef void (*NFCBK_HFP_MptyCallStatus)(BT_ADDR* addr, HFP_CALL_STATUS mpty_status, HFP_MPTY_INFO *pMptyInfo, nfUINT8 mpty_line_count);
typedef void (*NFCBK_HFP_AudioStates)(BT_ADDR *addr, HFP_AUDIO_CONNECTION_STATES audio_state);
typedef void (*NFCBK_HFP_VoiceRecognitionStates)(BT_ADDR *addr, HFP_VOICE_RECOGNITION_ACTION vr_action);
typedef void (*NFCBK_HFP_VolumeChange)(BT_ADDR *addr, HFP_VOLUME_TYPE type, int volume);
typedef void (*NFCBK_HFP_DeviceNotification)(BT_ADDR *addr, HFP_DEVICE_NOTIFY_PROP notify_prop, nfUINT8 notify_prop_state, nfUINT8* prop_data);
typedef void (*NFCBK_HFP_NetworkOperator)(BT_ADDR *addr, nfUINT8 *operator_name);
typedef void (*NFCBK_HFP_SubscriberInfo)(BT_ADDR *addr, nfUINT8 *number, HFP_SUBSCRIBER_SERVICE_TYPE service_type);
typedef void (*NFCBK_HFP_CommandOk)(BT_ADDR *addr, HFP_COMMAND_ID cmd);
typedef void (*NFCBK_HFP_CommandError)(BT_ADDR *addr, HFP_COMMAND_ID cmd);
typedef void (*NFCBK_HFP_ManufacturerID)(BT_ADDR *addr, nfUINT8 *manufacturer_id);
typedef void (*NFCBK_HFP_ManufacturerModelID)(BT_ADDR *addr, nfUINT8 *manufacturer_model_id);

/*******************************************************
**          A2DP CBK function pointer define          **
*******************************************************/
typedef void(*NFCBK_A2DP_ConnectStates)(BT_ADDR *addr, A2DP_CONN_STATES a2dp_state);
typedef void(*NFCBK_A2DP_AudioStates)(BT_ADDR *addr, A2DP_AUDIO_STATES a2dp_state);

/********************************************************
**          AVRCP CBK function pointer define          **
********************************************************/
typedef void(*NFCBK_AVRCP_States)(BT_ADDR *addr, AVRCP_CONN_STATES avrcp_state);
typedef void(*NFCBK_AVRCP_RemoteFeature)(BT_ADDR *addr, AVRCP_FEATURE feat);
typedef void(*NFCBK_AVRCP_PassthroughRsp)(BT_ADDR *addr, AVRCP_OP_ID command, AVRCP_RSP_CODE rsp_code);
/*  AVRCP 1.3  */
typedef void(*NFCBK_AVRCP_PlayStates)(BT_ADDR *addr, AVRCP_PLAY_STATE play_states, nfUINT32 song_pos, nfUINT32 song_total_len);
typedef void(*NFCBK_AVRCP_ElementAttribute)(BT_ADDR *addr, AVRCP_ELEMENT_ATTR_STRUCT *element_attrs);
typedef void(*NFCBK_AVRCP_Capability)(BT_ADDR *addr, AVRCP_CAPABILITY *events);
typedef void(*NFCBK_AVRCP_Notification)(BT_ADDR *addr, AVRCP_EVENT_ID event_id, AVRCP_NOTIFICATION_TYPE type, AVRCP_NOTIFICATION *notification_info);
typedef void(*NFCBK_AVRCP_ListPlayerAppAttr)(BT_ADDR *addr, int num_attr, AVRCP_PLAYER_ATTR *p_attrs);
typedef void(*NFCBK_AVRCP_ListPlayerAppValue)(BT_ADDR *addr, AVRCP_PLAYER_ATTR attr_id, int num_val, nfUINT8 *p_vals);
typedef void(*NFCBK_AVRCP_GetPlayerAppValue)(BT_ADDR *addr, AVRCP_PLAYER_SETTING *player_settings);
typedef void(*NFCBK_AVRCP_SetPlayerAppValue)(BT_ADDR *addr);
typedef void(*NFCBK_AVRCP_CommandError)(BT_ADDR *bd_addr, AVRCP_COMMAND_ID command, AVRCP_RSP_STATUS reason, AVRCP_RSP_CODE rsp_code, AVRCP_EVENT_ID event_id);
/*  AVRCP 1.4  */
typedef void(*NFCBK_AVRCP_SetAddressedPlayer)(BT_ADDR *addr);
typedef void(*NFCBK_AVRCP_SetBrowsedPlayer)(BT_ADDR *addr, AVRCP_BROWSED_FOLDER *browsed_player);
typedef void(*NFCBK_AVRCP_ChangePath)(BT_ADDR *addr);
typedef void(*NFCBK_AVRCP_PlayerItem)(BT_ADDR *addr, AVRCP_PLAYERITEM *player_item);
typedef void(*NFCBK_AVRCP_FolderItem)(BT_ADDR *addr, AVRCP_FOLDERITEM *folder_item);
typedef void(*NFCBK_AVRCP_MediaItem)(BT_ADDR *addr, AVRCP_MEDIAITEM *media_item);
typedef void(*NFCBK_AVRCP_GetItemSuccess)(BT_ADDR *addr, int count);
typedef void(*NFCBK_AVRCP_GetItemAttribute)(BT_ADDR *addr, AVRCP_UID uid, AVRCP_ELEMENT_ATTR_STRUCT *element_attrs);
typedef void(*NFCBK_AVRCP_PlayItem)(BT_ADDR *addr);

/*******************************************************
**          PBAP CBK function pointer define          **
*******************************************************/
typedef void(*NFCBK_PB_ContactsInfo)(BT_ADDR *addr, PB_STRUCT *pbstruct);
typedef void(*NFCBK_PB_DownloadStates)(BT_ADDR *addr, PB_DOWNLOAD_STATES states);
typedef void(*NFCBK_PB_TotalContact)(PB_ACCESS access, nfUINT16 total_contact, BT_ADDR *addr);

/*******************************************************
**          MAP CBK function pointer define          **
*******************************************************/
/** callback for get_remote_mas_instances */
typedef void (*NFCBK_MAP_DownloadState) (BT_ADDR *addr , MAP_DOWNLOAD_STATES state);
typedef void (*NFCBK_MAP_NotificationState) (BT_ADDR *addr , MAP_NOTIFICATION_STATES state);
typedef void (*NFCBK_MAP_TotalMessage) (BT_ADDR *addr, MAP_MSG_TYPE type, MAP_FOLDER_STRUCT folder, nfUINT8 total);
typedef void (*NFCBK_MAP_DownloadMessage) (BT_ADDR *addr, MAP_SMS_GSM *message, nfBOOLEAN is_new);
typedef void (*NFCBK_MAP_SendMessage) (BT_ADDR *addr, nfUINT8 *tg_num, MAP_COMMAND_STATUS send_status);
typedef void (*NFCBK_MAP_DeleteMessage) (BT_ADDR *addr, nfUINT8 *msg_handle, MAP_COMMAND_STATUS reason);
typedef void (*NFCBK_MAP_ChangeReadStatus) (BT_ADDR *addr, nfUINT8 *msg_handle, MAP_COMMAND_STATUS reason);
typedef void (*NFCBK_MAP_EventReport) (BT_ADDR *addr, MAP_EVENT_REPORT event, nfUINT8 *msg_handle, MAP_MSG_TYPE type, MAP_FOLDER_STRUCT folder);

/******************************************************
**          RFCOMM CBK function pointer define          **
******************************************************/
typedef void (*NFCBK_RFCOMM_ConnectStates)(BT_ADDR *addr, PROFILE_ID id, RFCOMM_STATES rfcomm_state);

/*******************************************************
**          HID CBK function pointer define          **
*******************************************************/
typedef void(*NFCBK_HID_ConnectStates)(BT_ADDR *addr, HID_CONN_STATES hid_state);

/*******************************************************
**          OPP CBK function pointer define          **
*******************************************************/
typedef void(*NFCBK_OPP_DownloadStates)(BT_ADDR *addr, OPP_STATES state);
typedef void(*NFCBK_OPP_AccessRequest)(BT_ADDR *addr, nfUINT8 *dev_name, OPP_FILE_INFO *file_info);

/*******************************************************
**          GATT CBK function pointer define          **
*******************************************************/
/* client */
typedef void(*NFCBK_GATT_ScanResult)(BT_ADDR *addr, int rssi, nfUINT8 *adv_data);
typedef void(*NFCBK_GATT_SearchComplete)(GATT_STATUS status);
typedef void(*NFCBK_GATT_SearchResult)(nfUINT8 *srvc_uuid, nfUINT8 is_primary);
typedef void(*NFCBK_GATT_GetIncludeServiceResult)(GATT_STATUS status, nfUINT8 *srvc_uuid, nfUINT8 *inc_srvc_uuid);
typedef void(*NFCBK_GATT_GetCharacteristicResult)(GATT_STATUS status, nfUINT8 *srvc_uuid, nfUINT8 *char_uuid, int char_prop);
typedef void(*NFCBK_GATT_GetDescriptorResult)(GATT_STATUS status, nfUINT8 *srvc_uuid, nfUINT8 *char_uuid, nfUINT8 *descr_uuid);
typedef void(*NFCBK_GATT_RegisterNotify)(GATT_STATUS status, int registered);
typedef void(*NFCBK_GATT_Notify)(BT_ADDR *addr, nfUINT8 *srvc_uuid, nfUINT8 *char_uuid, int data_len, nfUINT8 *data);
typedef void(*NFCBK_GATT_ReadCharacteristicResult)(GATT_STATUS status, nfUINT8 *srvc_uuid, nfUINT8 *char_uuid, int data_type, int data_len, nfUINT8 *data);
typedef void(*NFCBK_GATT_WriteCharacteristicResult)(GATT_STATUS status, nfUINT8 *srvc_uuid, nfUINT8 *char_uuid);
typedef void(*NFCBK_GATT_ReadDescriptorResult)(GATT_STATUS status, nfUINT8 *srvc_uuid, nfUINT8 *char_uuid, nfUINT8 *descr_uuid, int data_type, int data_len, nfUINT8 *data);
typedef void(*NFCBK_GATT_WriteDescriptorResult)(GATT_STATUS status, nfUINT8 *srvc_uuid, nfUINT8 *char_uuid, nfUINT8 *descr_uuid);
typedef void(*NFCBK_GATT_ReadRssi)(GATT_STATUS status, BT_ADDR *addr, int rssi);
typedef void(*NFCBK_GATT_SetAdvDataResult)(GATT_STATUS status);
typedef void(*NFCBK_GATT_ClientRegister)(GATT_STATUS status);

/* server */
typedef void (*NFCBK_GATT_ServiceAddedResult)(GATT_STATUS status, nfUINT8 *srvc_uuid, int srvc_handle);
typedef void (*NFCBK_GATT_CharAddedResult)(GATT_STATUS status, nfUINT8 *char_uuid, int srvc_handle, int char_handle);
typedef void (*NFCBK_GATT_DescriptorAddedResult)(GATT_STATUS status, nfUINT8 *descr_uuid, int srvc_handle, int descr_handle);
typedef void (*NFCBK_GATT_ServiceStartResult)(GATT_STATUS status, int srvc_handle);
typedef void (*NFCBK_GATT_ServiceStopResult)(GATT_STATUS status, int srvc_handle);
typedef void (*NFCBK_GATT_ServiceDeleteResult)(GATT_STATUS status, int srvc_handle);
typedef void (*NFCBK_GATT_RequestReadEvent)(BT_ADDR *addr, int trans_id, int attr_handle, int offset, int is_long);
typedef void (*NFCBK_GATT_RequestWriteEvent)(BT_ADDR *addr, int trans_id, int attr_handle, int offset, int is_prep, nfUINT8 *data, int length, int need_rsp);
typedef void (*NFCBK_GATT_RequestExecWriteEvent)(BT_ADDR *addr, int trans_id, int exec_write);
typedef void (*NFCBK_GATT_ResponseConfirmResult)(GATT_STATUS status);
typedef void (*NFCBK_GATT_IndicatorSentResult)(GATT_STATUS status);
typedef void (*NFCBK_GATT_CongestionResult)(int congested);
typedef void (*NFCBK_GATT_ConnectStatus)(GATT_CONN_STATUS status, BT_ADDR *addr, GATT_ROLE role);
typedef void (*NFCBK_GATT_SetRoleResult)(GATT_STATUS status, GATT_ROLE role);
typedef void (*NFCBK_GATT_PeripheralAttributeListResult)(int handle, nfUINT8 *uuid, int property, int type);
typedef void (*NFCBK_GATT_PeripheralServiceListResult)(int handle, nfUINT8 *uuid);
typedef void (*NFCBK_GATT_PeripheralListComplete)(GATT_STATUS status);
typedef void (*NFCBK_GATT_ServiceRegister)(GATT_STATUS status);
typedef void (*NFCBK_GATT_ServiceUnRegister)(GATT_STATUS status);

/* service included in server */
typedef void (*NFCBK_GATT_BatteryLevelReadEvent)(int trans_id, int attr_handle);
typedef void (*NFCBK_GATT_BloodPressureFeatureReadEvent)(int trans_id, int attr_handle);
typedef void (*NFCBK_GATT_SppRxReceiveEvent)(int offset, nfUINT8 *value, int length);
typedef void (*NFCBK_GATT_CharNotificationReq)(int srvc_idx, int char_id, int enable);

/* BLE RF test number of packet received */
typedef void(*NFCBK_LE_TestPacketRecv)(nfUINT16 number_of_packet);

/******************************************
**    nFMW callback struct definition    **
******************************************/
typedef struct          /*need to be implement*/
{
    unsigned long                       size;

    /*Basic functional cbk*/
    NFCBK_LocalDeviceInfo                   local_device_info_nfbt_cbk;
    NFCBK_PariedDeviceInfo                  paired_device_info_nfbt_cbk;
    NFCBK_InquiryStates                     inquiry_states_nfbt_cbk;
    NFCBK_PairStates                        pair_states_nfbt_cbk;
    NFCBK_DiagnosticMessage                 diag_message_nfbt_cbk;
    NFCBK_PairingRequest                    pairing_requset_nfbt_cbk;
    NFCBK_SspRequest                        ssp_request_nfbt_cbk;
    NFCBK_Disconnect_Reason                 disconnect_reason_nfbt_cbk;
    NFCBK_BtEnableState                     bt_enable_state_nfbt_cbk;

    /*HFP functional cbk*/
    NFCBK_HFP_ConnectStates                 hfp_connect_states_nfbt_cbk;
    NFCBK_HFP_CallStatus                    hfp_call_status_nfbt_cbk;
    NFCBK_HFP_MptyCallStatus                hfp_call_mpty_status_nfbt_cbk;
    NFCBK_HFP_AudioStates                   hfp_audio_states_nfbt_cbk;
    NFCBK_HFP_DeviceNotification            hfp_device_notification_nfbt_cbk;
    NFCBK_HFP_NetworkOperator               hfp_network_operator_nfbt_cbk;
    NFCBK_HFP_VoiceRecognitionStates        hfp_voice_recognition_states_nfbt_cbk;
    NFCBK_HFP_CommandOk                     hfp_command_ok_nfbt_cbk;
    NFCBK_HFP_CommandError                  hfp_command_error_nfbt_cbk;
    NFCBK_HFP_VolumeChange                  hfp_volume_change_nfbt_cbk;
    NFCBK_HFP_SubscriberInfo                hfp_device_subscriber_info_nfbt_cbk;
    NFCBK_HFP_ManufacturerID                hfp_device_manufacturer_id_nfbt_cbk;
    NFCBK_HFP_ManufacturerModelID           hfp_device_manufacturer_model_id_nfbt_cbk;

    /*A2DP functional cbk*/
    NFCBK_A2DP_ConnectStates                a2dp_conn_states_nfbt_cbk;
    NFCBK_A2DP_AudioStates                  a2dp_audio_states_nfbt_cbk;

    /*AVRCP functional cbk*/
    NFCBK_AVRCP_States                      avrcp_states_nfbt_cbk;
    NFCBK_AVRCP_RemoteFeature               avrcp_remote_feature_nfbt_cbk;
    NFCBK_AVRCP_PlayStates                  avrcp_playstates_nfbt_cbk;
    NFCBK_AVRCP_ElementAttribute            avrcp_element_attribute_nfbt_cbk;
    NFCBK_AVRCP_Capability                  avrcp_capability_nfbt_cbk;
    NFCBK_AVRCP_Notification                avrcp_notification_nfbt_cbk;
    NFCBK_AVRCP_ListPlayerAppAttr           avrcp_list_player_app_attr_nfbt_cbk;
    NFCBK_AVRCP_ListPlayerAppValue          avrcp_list_player_app_value_nfbt_cbk;
    NFCBK_AVRCP_GetPlayerAppValue           avrcp_get_player_app_value_nfbt_cbk;
    NFCBK_AVRCP_SetPlayerAppValue           avrcp_set_player_app_value_nfbt_cbk;
    NFCBK_AVRCP_CommandError                avrcp_command_error_nfbt_cbk;
    NFCBK_AVRCP_PassthroughRsp              avrcp_passthrough_rsp_nfbt_cbk;
    NFCBK_AVRCP_SetAddressedPlayer          avrcp_setaddressedplayer_nfbt_cbk;
    NFCBK_AVRCP_SetBrowsedPlayer            avrcp_setbrowsedplayer_nfbt_cbk;
    NFCBK_AVRCP_ChangePath                  avrcp_changepath_nfbt_cbk;
    NFCBK_AVRCP_PlayerItem                  avrcp_playeritem_nfbt_cbk;
    NFCBK_AVRCP_FolderItem                  avrcp_folderitem_nfbt_cbk;
    NFCBK_AVRCP_MediaItem                   avrcp_mediaitem_nfbt_cbk;
    NFCBK_AVRCP_GetItemSuccess              avrcp_get_item_success_nfbt_cbk;
    NFCBK_AVRCP_GetItemAttribute            avrcp_get_item_attribute_nfbt_cbk;
    NFCBK_AVRCP_PlayItem                    avrcp_play_item_nfbt_cbk;

    /*PBAP functional cbk*/
    NFCBK_PB_ContactsInfo                   pb_contacts_info_nfbt_cbk;
    NFCBK_PB_DownloadStates                 pb_download_states_nfbt_cbk;
    NFCBK_PB_TotalContact                   pb_total_contact_nfbt_cbk;

    /*MAP functional cbk*/
    NFCBK_MAP_DownloadState                 map_download_state_cbk;
    NFCBK_MAP_NotificationState             map_notification_state_cbk;
    NFCBK_MAP_TotalMessage                  map_total_message_cbk;
    NFCBK_MAP_DownloadMessage               map_download_message_cbk;
    NFCBK_MAP_SendMessage                   map_send_message_cbk;
    NFCBK_MAP_DeleteMessage                 map_delete_message_cbk;
    NFCBK_MAP_ChangeReadStatus              map_change_read_status_cbk;
    NFCBK_MAP_EventReport                   map_notification_event_cbk;

    /*RFCOMM functional cbk*/
    NFCBK_RFCOMM_ConnectStates              rfcomm_connect_states_nfbt_cbk;

    /*HID functional cbk*/
    NFCBK_HID_ConnectStates                 hid_states_nfbt_cbk;

    /*OPP functional cbk*/
    NFCBK_OPP_DownloadStates                opp_download_states_nfbt_cbk;
    NFCBK_OPP_AccessRequest                 opp_access_request_nfbt_cbk;

    /*GATT functional cbk*/
    /* client */
    NFCBK_GATT_ScanResult                   gatt_scan_result_nfbt_cbk;
    NFCBK_GATT_SearchComplete               gatt_search_complete_nfbt_cbk;
    NFCBK_GATT_SearchResult                 gatt_search_result_nfbt_cbk;
    NFCBK_GATT_GetIncludeServiceResult      gatt_get_include_service_nfbt_cbk;
    NFCBK_GATT_GetCharacteristicResult      gatt_get_characteristic_nfbt_cbk;
    NFCBK_GATT_GetDescriptorResult          gatt_get_descriptor_nfbt_cbk;
    NFCBK_GATT_RegisterNotify               gatt_get_notification_register_nfbt_cbk;
    NFCBK_GATT_Notify                       gatt_get_notify_nfbt_cbk;
    NFCBK_GATT_ReadCharacteristicResult     gatt_read_characteristic_nfbt_cbk;
    NFCBK_GATT_WriteCharacteristicResult    gatt_write_characteristic_nfbt_cbk;
    NFCBK_GATT_ReadDescriptorResult         gatt_read_descriptor_nfbt_cbk;
    NFCBK_GATT_WriteDescriptorResult        gatt_write_descriptor_nfbt_cbk;
    NFCBK_GATT_ReadRssi                     gatt_read_rssi_nfbt_cbk;
    NFCBK_GATT_SetAdvDataResult             gatt_set_adv_data_result_nfbt_cbk;
    NFCBK_GATT_ClientRegister               gatt_client_register_nfbt_cbk;

    /* server */
    NFCBK_GATT_ServiceAddedResult           gatt_service_added_nfbt_cbk;
    NFCBK_GATT_CharAddedResult              gatt_char_added_nfbt_cbk;
    NFCBK_GATT_DescriptorAddedResult        gatt_des_added_nfbt_cbk;
    NFCBK_GATT_ServiceStartResult           gatt_service_start_nfbt_cbk;
    NFCBK_GATT_ServiceStopResult            gatt_service_stop_nfbt_cbk;
    NFCBK_GATT_ServiceDeleteResult          gatt_service_delete_nfbt_cbk;
    NFCBK_GATT_RequestReadEvent             gatt_request_read_nfbt_cbk;
    NFCBK_GATT_RequestWriteEvent            gatt_request_write_nfbt_cbk;
    NFCBK_GATT_RequestExecWriteEvent        gatt_request_exec_write_nfbt_cbk;
    NFCBK_GATT_ResponseConfirmResult        gatt_response_confirm_nfbt_cbk;
    NFCBK_GATT_IndicatorSentResult          gatt_indicator_sent_nfbt_cbk;
    NFCBK_GATT_CongestionResult             gatt_congestion_nfbt_cbk;
    NFCBK_GATT_ConnectStatus                gatt_connect_status_nfbt_cbk;
    NFCBK_GATT_SetRoleResult                gatt_set_role_result_nfbt_cbk;
    NFCBK_GATT_PeripheralAttributeListResult    gatt_peri_attribute_list_nfbt_cbk;
    NFCBK_GATT_PeripheralServiceListResult  gatt_peri_service_list_nfbt_cbk;
    NFCBK_GATT_PeripheralListComplete       gatt_peri_list_complete_nfbt_cbk;
    NFCBK_GATT_ServiceRegister              gatt_service_register_nfbt_cbk;
    NFCBK_GATT_ServiceUnRegister            gatt_service_unregister_nfbt_cbk;

    /* service included in server */
    NFCBK_GATT_BatteryLevelReadEvent        gatt_srv_battery_level_read_event_nfbt_cbk;
    NFCBK_GATT_BloodPressureFeatureReadEvent    gatt_srv_bpf_read_event_nfbt_cbk;
    NFCBK_GATT_SppRxReceiveEvent            gatt_srv_spp_rx_receive_event_nfbt_cbk;
    NFCBK_GATT_CharNotificationReq          gatt_char_notification_req_nfbt_cbk;

    /* BLE RF test number of packet received */
    NFCBK_LE_TestPacketRecv                 le_test_packet_num_recv;
}NFBT_CBK_STRUCT;

/*****************************
**    Basic API function    **
*****************************/
#ifdef __GNUC__
  #define BLUEGATE_PUBLIC_API __attribute__ ((visibility ("default")))
#else
  #define BLUEGATE_PUBLIC_API 
#endif

BLUEGATE_PUBLIC_API BT_VERSION NFBT_GetBtVersion(void);
BLUEGATE_PUBLIC_API bt_status NFBT_Init(NFBT_CBK_STRUCT *bt_cb);
BLUEGATE_PUBLIC_API bt_status NFBT_Deinit(void);
BLUEGATE_PUBLIC_API bt_status NFBT_SetAddr(BT_ADDR *addr);
BLUEGATE_PUBLIC_API bt_status NFBT_GetAddr(BT_ADDR *addr);
BLUEGATE_PUBLIC_API bt_status NFBT_Enable(void);
BLUEGATE_PUBLIC_API bt_status NFBT_Disable(void);
BLUEGATE_PUBLIC_API bt_status NFBT_GetLocalDeviceInfo(void);
BLUEGATE_PUBLIC_API bt_status NFBT_GetPairedDeviceInfo(void);
BLUEGATE_PUBLIC_API bt_status NFBT_SetLocalDeviceName(nfUINT8 name_size, const nfUINT8 *name);
BLUEGATE_PUBLIC_API bt_status NFBT_SetScanMode(SCAN_MODE mode);
BLUEGATE_PUBLIC_API bt_status NFBT_SetInquiryTimeout(nfUINT8 timeout);
BLUEGATE_PUBLIC_API bt_status NFBT_GetInquiryTimeout(nfUINT8 *timeout);
BLUEGATE_PUBLIC_API bt_status NFBT_InquiryDevice(INQUIRY_ACTION action);
BLUEGATE_PUBLIC_API bt_status NFBT_PairReply(BT_ADDR *addr, PAIR_ACTION action, nfUINT8 pincode_size, nfUINT8 *pincode);
BLUEGATE_PUBLIC_API bt_status NFBT_SspReply(BT_ADDR *addr, PAIR_ACTION action);
BLUEGATE_PUBLIC_API bt_status NFBT_PairRemoteDevice(BT_ADDR *addr);
BLUEGATE_PUBLIC_API bt_status NFBT_SetPairedDeviceNum(int num );
BLUEGATE_PUBLIC_API bt_status NFBT_GetPairedDeviceNum(int *num );
BLUEGATE_PUBLIC_API bt_status NFBT_ConnectProfile(BT_ADDR *addr, PROFILE_ID id);
BLUEGATE_PUBLIC_API bt_status NFBT_DisconnectProfile(BT_ADDR *addr, PROFILE_ID id);
BLUEGATE_PUBLIC_API bt_status NFBT_ForceConnectProfile(BT_ADDR *addr, PROFILE_ID id);
BLUEGATE_PUBLIC_API bt_status NFBT_DeletePairedDevice(BT_ADDR *addr);
BLUEGATE_PUBLIC_API bt_status NFBT_DeletePairedDeviceData(BT_ADDR *addr);
BLUEGATE_PUBLIC_API bt_status NFBT_SetAutoReconnection(nfUINT32 count);
BLUEGATE_PUBLIC_API bt_status NFBT_GetAutoReconnection(nfUINT32 *count);
BLUEGATE_PUBLIC_API bt_status NFBT_SetLinklossReconnection(nfUINT32 count);
BLUEGATE_PUBLIC_API bt_status NFBT_GetLinklossReconnection(nfUINT32 *count);
BLUEGATE_PUBLIC_API bt_status NFBT_SetAutoReconnectionPeriod(nfUINT32 period);
BLUEGATE_PUBLIC_API bt_status NFBT_GetAutoReconnectionPeriod(nfUINT32 *period);
BLUEGATE_PUBLIC_API bt_status NFBT_SetAutoReconnectionMode(AUTO_RECONN_MODE mode);
BLUEGATE_PUBLIC_API bt_status NFBT_SetPairedDeviceSoft(PAIRED_SOFT_MODE mode);
BLUEGATE_PUBLIC_API bt_status NFBT_SetSSPMode(nfBOOLEAN action);
BLUEGATE_PUBLIC_API bt_status NFBT_SetShowLog(nfBOOLEAN action);
BLUEGATE_PUBLIC_API bt_status NFBT_SetDumpHci(nfBOOLEAN action);
BLUEGATE_PUBLIC_API bt_status NFBT_SetAudioDevice(char *name);                   //deprecation
BLUEGATE_PUBLIC_API bt_status NFBT_SetA2DPDevice(char *name);
BLUEGATE_PUBLIC_API bt_status NFBT_SetRingDevice(char *name);
BLUEGATE_PUBLIC_API bt_status NFBT_GetA2DPDevice(char *name, int size);
BLUEGATE_PUBLIC_API bt_status NFBT_GetRingDevice(char *name, int size);
BLUEGATE_PUBLIC_API bt_status NFBT_SetRingPath(char *path);
BLUEGATE_PUBLIC_API bt_status NFBT_GetRingPath(char *path, int size);
BLUEGATE_PUBLIC_API bt_status NFBT_SetLocalDeviceProfile(PROFILE_ID id, nfBOOLEAN enable);
BLUEGATE_PUBLIC_API bt_status NFBT_SetRemoteDeviceProfile(BT_ADDR *addr, PROFILE_ID id);
BLUEGATE_PUBLIC_API bt_status NFBT_GetRemoteDeviceProfile(BT_ADDR *addr, PROFILE_ID *id);
BLUEGATE_PUBLIC_API bt_status NFBT_GetRemoteDeviceSupport(BT_ADDR *addr, PROFILE_ID *id);
BLUEGATE_PUBLIC_API bt_status NFBT_GetConnectedRemoteAddr(BT_ADDR *addr, PROFILE_ID id);
BLUEGATE_PUBLIC_API bt_status NFBT_SetTestMode(nfBOOLEAN action);
BLUEGATE_PUBLIC_API bt_status NFBT_SetBrEdrTestMode(nfUINT8 mode, nfUINT16 freq, nfUINT8 power, nfUINT8 data_type, nfUINT8 modulation);
BLUEGATE_PUBLIC_API bt_status NFBT_SetBleTestMode(nfUINT8 mode, nfUINT16 freq);
BLUEGATE_PUBLIC_API bt_status NFBT_IsPaired(BT_ADDR *addr, nfBOOLEAN *paired);
BLUEGATE_PUBLIC_API bt_status NFBT_UpdateRssi(nfBOOLEAN enable, nfUINT8 time_interval);

/***************************
**    HFP API function    **
***************************/
BLUEGATE_PUBLIC_API bt_status NFBT_HFP_Answer(BT_ADDR *addr);
BLUEGATE_PUBLIC_API bt_status NFBT_HFP_Reject(BT_ADDR *addr);
BLUEGATE_PUBLIC_API bt_status NFBT_HFP_Terminate(BT_ADDR *addr);
BLUEGATE_PUBLIC_API bt_status NFBT_HFP_Dial(BT_ADDR *addr, nfUINT8 number_size, const nfUINT8 *number);
BLUEGATE_PUBLIC_API bt_status NFBT_HFP_Redial(BT_ADDR* addr);
BLUEGATE_PUBLIC_API bt_status NFBT_HFP_SendDTMF(BT_ADDR *addr, const nfINT8 number);
BLUEGATE_PUBLIC_API bt_status NFBT_HFP_AudioTransfer(BT_ADDR *addr, HFP_AUDIO_TRANSFER_ACTION action);
BLUEGATE_PUBLIC_API bt_status NFBT_HFP_VoiceRecognition(BT_ADDR *addr, HFP_VOICE_RECOGNITION_ACTION vr_action);
BLUEGATE_PUBLIC_API bt_status NFBT_HFP_CallHold(BT_ADDR *addr, HFP_CALL_ACTION_CHLD action, nfUINT8 index);
BLUEGATE_PUBLIC_API bt_status NFBT_HFP_SpkVolume(BT_ADDR *addr, nfUINT8 volume);
BLUEGATE_PUBLIC_API bt_status NFBT_HFP_MicVolume(BT_ADDR *addr, nfUINT8 volume);
BLUEGATE_PUBLIC_API bt_status NFBT_HFP_SubscriberInfo(BT_ADDR *addr);
BLUEGATE_PUBLIC_API bt_status NFBT_HFP_SetCallAutoReaction( BT_ADDR *addr, HFP_AUTO_REACTION auto_reaction );
BLUEGATE_PUBLIC_API bt_status NFBT_HFP_GetCallAutoReaction( BT_ADDR *addr, HFP_AUTO_REACTION *auto_reaction );
BLUEGATE_PUBLIC_API bt_status NFBT_HFP_SetCallAutoReactionTime( BT_ADDR *addr, int sec );
BLUEGATE_PUBLIC_API bt_status NFBT_HFP_GetCallAutoReactionTime( BT_ADDR *addr, int *sec );
BLUEGATE_PUBLIC_API bt_status NFBT_HFP_SetPlayRing(nfBOOLEAN action);
BLUEGATE_PUBLIC_API bt_status NFBT_HFP_GetPlayRing(nfBOOLEAN *action);
BLUEGATE_PUBLIC_API bt_status NFBT_HFP_SetRejectInBandRing(nfBOOLEAN action);
BLUEGATE_PUBLIC_API bt_status NFBT_HFP_SetSpkRender(nfBOOLEAN action);
BLUEGATE_PUBLIC_API bt_status NFBT_HFP_GetSpkRender(nfBOOLEAN *action);
BLUEGATE_PUBLIC_API bt_status NFBT_HFP_SetXAPL(BT_ADDR *addr, unsigned int vid, unsigned int pid, unsigned int version, unsigned int features);
BLUEGATE_PUBLIC_API bt_status NFBT_HFP_SetSiriEyesFreeMode(BT_ADDR *addr, HFP_APLEFM_ACTION aplefm_action);
BLUEGATE_PUBLIC_API bt_status NFBT_HFP_GetDevManufacturerId(BT_ADDR *addr);
BLUEGATE_PUBLIC_API bt_status NFBT_HFP_GetDevManufacturerModelId(BT_ADDR *addr);
BLUEGATE_PUBLIC_API bt_status NFBT_HFP_GetDevIMEINumber(BT_ADDR *addr);

/***************************
**    DSP API function    **
***************************/
BLUEGATE_PUBLIC_API bt_status NFBT_HFP_SetMicMute(BT_ADDR *addr, nfBOOLEAN action);
BLUEGATE_PUBLIC_API bt_status NFBT_HFP_GetMicMute(BT_ADDR *addr, nfBOOLEAN *action);
BLUEGATE_PUBLIC_API bt_status NFBT_DSP_SpkVolume(BT_ADDR *addr, nfINT8 volume);
BLUEGATE_PUBLIC_API bt_status NFBT_DSP_MicVolume(BT_ADDR *addr, nfINT8 volume);

/*****************************
**    A2DP API function    **
*****************************/
BLUEGATE_PUBLIC_API bt_status NFBT_A2DP_SetMute(nfBOOLEAN action);
BLUEGATE_PUBLIC_API bt_status NFBT_A2DP_GetMute(nfBOOLEAN *action);

/*****************************
**    AVRCP API function    **
*****************************/
/*  AVRCP 1.0 API  */
BLUEGATE_PUBLIC_API bt_status NFBT_AVRCP_ControlCmd(BT_ADDR *addr, AVRCP_OP_ID id, AVRCP_BUTTON_STATUS press);
/*  AVRCP 1.3 API  */
BLUEGATE_PUBLIC_API bt_status NFBT_AVRCP_GetPlayStatus(BT_ADDR *addr);
BLUEGATE_PUBLIC_API bt_status NFBT_AVRCP_GetElementAttributes(BT_ADDR *addr);
BLUEGATE_PUBLIC_API bt_status NFBT_AVRCP_ListPlayerAppAttr(BT_ADDR *addr);
BLUEGATE_PUBLIC_API bt_status NFBT_AVRCP_ListPlayerAppValue(BT_ADDR *addr, AVRCP_PLAYER_ATTR attr_id);
BLUEGATE_PUBLIC_API bt_status NFBT_AVRCP_GetPlayerAPPValue(BT_ADDR *addr);
BLUEGATE_PUBLIC_API bt_status NFBT_AVRCP_SetPlayerAPPValue(BT_ADDR *addr, AVRCP_PLAYER_ATTR attr_id, nfUINT8 setting_val);
BLUEGATE_PUBLIC_API bt_status NFBT_AVRCP_GetCapability(BT_ADDR *addr);
BLUEGATE_PUBLIC_API bt_status NFBT_AVRCP_EnableNotification(BT_ADDR *addr);
BLUEGATE_PUBLIC_API bt_status NFBT_AVRCP_EnableNotificationOne(BT_ADDR *addr, AVRCP_EVENT_ID id);
BLUEGATE_PUBLIC_API bt_status NFBT_AVRCP_DisableNotification(BT_ADDR *addr);
/*  AVRCP 1.4 API  */
BLUEGATE_PUBLIC_API bt_status NFBT_AVRCP_SetAddressedPlayer(BT_ADDR *addr, nfUINT16 player_id);
BLUEGATE_PUBLIC_API bt_status NFBT_AVRCP_SetBrowsedPlayer(BT_ADDR *addr, nfUINT16 player_id);
BLUEGATE_PUBLIC_API bt_status NFBT_AVRCP_GetFolderItem(BT_ADDR *addr, AVRCP_SCOPE_TYPE scope);
BLUEGATE_PUBLIC_API bt_status NFBT_AVRCP_GetFolderItemEx(BT_ADDR *addr, AVRCP_SCOPE_TYPE scope, nfUINT32 offset, nfUINT32 count);
BLUEGATE_PUBLIC_API bt_status NFBT_AVRCP_ChangePath(BT_ADDR *addr, AVRCP_CHANGE_PATH updown, nfUINT16 UID_Cnt, AVRCP_UID UID);
BLUEGATE_PUBLIC_API bt_status NFBT_AVRCP_GetItemAttributes(BT_ADDR *addr, nfUINT16 UID_Cnt, AVRCP_UID UID);
BLUEGATE_PUBLIC_API bt_status NFBT_AVRCP_PlayItem(BT_ADDR *addr, AVRCP_SCOPE_TYPE scope, nfUINT16 UID_Cnt, AVRCP_UID UID);
BLUEGATE_PUBLIC_API bt_status NFBT_AVRCP_SetVolumeChange(BT_ADDR *addr, nfUINT8 AbsoluteVolume);
/****************************
**    PBAP API function    **
****************************/
BLUEGATE_PUBLIC_API bt_status NFBT_PB_Download(BT_ADDR *addr, PB_ACCESS access, PB_PHOTO_ONOFF photo);
BLUEGATE_PUBLIC_API bt_status NFBT_PB_DownloadStop(BT_ADDR *addr);
BLUEGATE_PUBLIC_API bt_status NFBT_PB_SetMaxCount(nfUINT32 max_count);           //deprecation
BLUEGATE_PUBLIC_API bt_status NFBT_PB_GetMaxCount(nfUINT32 *max_count);          //deprecation
BLUEGATE_PUBLIC_API bt_status NFBT_PB_SetMaxContactCount(nfUINT32 max_count);
BLUEGATE_PUBLIC_API bt_status NFBT_PB_SetMaxCalllogCount(nfUINT32 max_count);
BLUEGATE_PUBLIC_API bt_status NFBT_PB_SetAlwaysConnected(nfBOOLEAN enable);

/***************************
**    MAP API function    **
***************************/
BLUEGATE_PUBLIC_API bt_status NFBT_MAP_DownloadAllMessage(BT_ADDR *addr, MAP_MSG_TYPE type, MAP_FOLDER_STRUCT folder, nfUINT8 is_detail);
BLUEGATE_PUBLIC_API bt_status NFBT_MAP_DownloadMessage(BT_ADDR *addr, nfUINT8 *handle, MAP_MSG_TYPE type, MAP_FOLDER_STRUCT folder);
BLUEGATE_PUBLIC_API bt_status NFBT_MAP_DownloadStop(BT_ADDR *addr);
BLUEGATE_PUBLIC_API bt_status NFBT_MAP_EnableNotification(BT_ADDR *addr);
BLUEGATE_PUBLIC_API bt_status NFBT_MAP_DisableNotification(BT_ADDR *addr);
BLUEGATE_PUBLIC_API bt_status NFBT_MAP_SendMessage(BT_ADDR *addr, nfUINT8 *tgNumber, nfUINT8 *message);
BLUEGATE_PUBLIC_API bt_status NFBT_MAP_DeleteMessage(BT_ADDR *addr, MAP_FOLDER_STRUCT folder, nfUINT8 *handle);
BLUEGATE_PUBLIC_API bt_status NFBT_MAP_ChangeReadStatus(BT_ADDR *addr, MAP_FOLDER_STRUCT folder, nfUINT8 *handle, nfBOOLEAN read);
BLUEGATE_PUBLIC_API bt_status NFBT_MAP_SetMaxCount(nfUINT16 max_count);
BLUEGATE_PUBLIC_API bt_status NFBT_MAP_GetMaxCount(nfUINT16 *max_count);

/****************************
**   RFCOMM API function   **
****************************/
BLUEGATE_PUBLIC_API bt_status NFBT_RFCOMM_Register(PROFILE_ID id, char *uuid_str);
BLUEGATE_PUBLIC_API bt_status NFBT_RFCOMM_Unregister(PROFILE_ID id);
BLUEGATE_PUBLIC_API int NFBT_RFCOMM_Read(PROFILE_ID id, void *buf, int len, int timeout);
BLUEGATE_PUBLIC_API int NFBT_RFCOMM_Write(PROFILE_ID id, void *buf, int len);
BLUEGATE_PUBLIC_API int NFBT_RFCOMM_GetFD(PROFILE_ID id);


/****************************
**    OPP API function     **
****************************/
BLUEGATE_PUBLIC_API bt_status NFBT_OPP_Push(BT_ADDR *addr, const nfUINT8* path);
BLUEGATE_PUBLIC_API bt_status NFBT_OPP_AccessReply(BT_ADDR *addr, OPP_DOWNLOAD_REPLY reply);
BLUEGATE_PUBLIC_API bt_status NFBT_OPP_Stop(BT_ADDR *addr);
BLUEGATE_PUBLIC_API bt_status NFBT_OPP_SetDownloadPath(const nfUINT8* path);
BLUEGATE_PUBLIC_API bt_status NFBT_OPP_GetDownloadPath(nfUINT8** path);

/****************************
**    HID API function     **
****************************/
BLUEGATE_PUBLIC_API bt_status NFBT_HID_SendKey(BT_ADDR *addr, nfUINT16 key);
BLUEGATE_PUBLIC_API bt_status NFBT_HID_SendMouseRel(BT_ADDR *addr, nfINT16 x, nfINT16 y, nfUINT8 button);
BLUEGATE_PUBLIC_API bt_status NFBT_HID_SendMouseAbs(BT_ADDR *addr, nfINT16 x, nfINT16 y, nfUINT8 button);
BLUEGATE_PUBLIC_API bt_status NFBT_HID_SetResolution(BT_ADDR *addr, int x, int y);

/****************************
**    GATT Client API function     **
****************************/
/* client */
BLUEGATE_PUBLIC_API bt_status NFBT_GATT_Listen(nfUINT8 start, nfUINT8 type);
BLUEGATE_PUBLIC_API bt_status NFBT_GATT_SearchService(void);
BLUEGATE_PUBLIC_API bt_status NFBT_GATT_GetIncludeService(char *srvc_uuid);
BLUEGATE_PUBLIC_API bt_status NFBT_GATT_GetCharacteristic(char *srvc_uuid);
BLUEGATE_PUBLIC_API bt_status NFBT_GATT_GetDescriptor(char *srvc_uuid, char *char_uuid);
BLUEGATE_PUBLIC_API bt_status NFBT_GATT_NotificationRegister(char * srvc_uuid, char * char_uuid, int registy);
BLUEGATE_PUBLIC_API bt_status NFBT_GATT_ReadCharacteristic(char *srvc_uuid, char *char_uuid);
BLUEGATE_PUBLIC_API bt_status NFBT_GATT_WriteCharacteristic(char *srvc_uuid, char *char_uuid, char *descr_data, int data_len);
BLUEGATE_PUBLIC_API bt_status NFBT_GATT_ReadDescriptor(char *srvc_uuid, char *char_uuid, char *descr_uuid);
BLUEGATE_PUBLIC_API bt_status NFBT_GATT_WriteDescriptor(char *srvc_uuid, char *char_uuid, char *descr_uuid, char *descr_data, int data_len);
BLUEGATE_PUBLIC_API bt_status NFBT_GATT_GetRssi(void);
BLUEGATE_PUBLIC_API bt_status NFBT_GATT_Scan(nfUINT8 start);
BLUEGATE_PUBLIC_API bt_status NFBT_GATT_SetAdvertisingData(int show_name, int show_txpower, char *srvc_uuid, int srvc_uuid_num, GATT_ADV_SPEED speed);
BLUEGATE_PUBLIC_API bt_status NFBT_GATT_ConnParameterUpdt(BT_ADDR *addr, GATT_CONN_LEVEL speed);
BLUEGATE_PUBLIC_API bt_status NFBT_GATT_AccessWhileList(nfUINT8 add, BT_ADDR *addr);
/* server */
BLUEGATE_PUBLIC_API bt_status NFBT_GATT_ServiceAdd(char *srvc_uuid);
BLUEGATE_PUBLIC_API bt_status NFBT_GATT_CharacteristicAdd(int srvc_hdl, char *char_uuid, nfUINT16 property, nfUINT16 permit);
BLUEGATE_PUBLIC_API bt_status NFBT_GATT_DescriptorAdd(int srvc_hdl, char *descr_uuid, nfUINT16 permit);
BLUEGATE_PUBLIC_API bt_status NFBT_GATT_StartService(int start, int srvc_hdl);
BLUEGATE_PUBLIC_API bt_status NFBT_GATT_DeleteService(int srvc_hdl);
BLUEGATE_PUBLIC_API bt_status NFBT_GATT_SendIndication(int attr_hdl, char *data, int data_len, int notification);
BLUEGATE_PUBLIC_API bt_status NFBT_GATT_SendResponse(int trans_id, GATT_STATUS status, int attr_hdl, int attr_hdl_rsp, char *data, int data_len);
BLUEGATE_PUBLIC_API bt_status NFBT_GATT_SetRole(GATT_ROLE role);
BLUEGATE_PUBLIC_API bt_status NFBT_GATT_PeripheralListService(void);
BLUEGATE_PUBLIC_API bt_status NFBT_GATT_PeripheralListAttribute(int srvc_hdl);
BLUEGATE_PUBLIC_API bt_status NFBT_GATT_PeripheralServiceRegister(int service_index, int mask);
BLUEGATE_PUBLIC_API bt_status NFBT_GATT_PeripheralServiceUnRegister(int service_index);
BLUEGATE_PUBLIC_API bt_status NFBT_GATT_BatteryLevelNotify(char *ind_data, int data_len);
BLUEGATE_PUBLIC_API bt_status NFBT_GATT_BloodPressureMeasurementIndicate(char *ind_data, int data_len);
BLUEGATE_PUBLIC_API bt_status NFBT_GATT_InterMediateCuffPressureNotify(char *ind_data, int data_len);
BLUEGATE_PUBLIC_API bt_status NFBT_GATT_SppSendData(char *ind_data, int data_len);

/* EIR_UUID */
BLUEGATE_PUBLIC_API bt_status NFBT_EIR_UUID_Register(char *uuid_str);
BLUEGATE_PUBLIC_API bt_status NFBT_EIR_UUID_Unregister(char *uuid_str);

#ifdef __cplusplus
 } // extern "C"
#endif // __cplusplus

#endif /*__NFORE_BLUEGATE_H__*/
