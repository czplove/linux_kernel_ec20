
/*
 *  Quectel WIFI defines
 *
 *  file name: quectel_wifi_defs.h
 *
 *  Design by: scott.hu
 *
 *  Date: 2015-06-16 
 */

#ifndef __QUECTEL_WIFI_DEFS__
#define __QUECTEL_WIFI_DEFS__

#ifdef __cplusplus
extern "C" {
#endif
#define QWIFI_PIPE_NAME		                "/data/wifi_pipe"
#define QWIFI_PIPE_NAME_RESPONSE            "/data/wifi_pipe_response"
#define QWIFI_PIPE_NAME_FIND                     "/data/wifi_pipe_find"
#define QWIFI_HOSTAP_CONF_FILE                  "/etc/hostapd.conf"
#define QWIFI_STA_MODE_HOSTAP_CONF_FILE                  "/etc/sta_mode_hostapd.conf"
#define QWIFI_WPA_SUPPLICANT_CONF_FILE                   "/etc/wpa_supplicant.conf"
#define QWIFI_WPA_SUPPLICANT_CONF_FILE_TEMP              "/tmp/wpa_supplicant.conf"
#define QWIFI_HOSTAP_ACCEPT_FILE                  "/etc/hostapd.accept"
#define QWIFI_HOSTAP_DENY_FILE                  "/etc/hostapd.deny"
#define QWIFI_HOSTAP_CONF_BAK                           "/etc/hostapd.conf.bak"
#define QWIFI_HOSTAP_CONF_FILE_FACTORY                  "/etc/hostapd-factory.conf"
#define QWIFI_HOSTAP_CONF_FILE_TEMP        "/etc/hostapd.conf.tmp"
#define QWIFI_HOSTAP_ACCEPT_FILE_BAK                  "/etc/hostapd.accept.bak"
#define QWIFI_HOSTAP_DENY_FILE_BAK                  "/etc/hostapd.deny.bak"

#define QWIFI_HOSTAP_CONF_FILE_LAST       "/etc/hostapd.conf.1"
#define QWIFI_MOBILEAP_CONF_FILE		  "/etc/mobileap_cfg.xml"
#define QWIFI_MOBILEAP_CONF_FILE_TEMP      "/etc/mobileap_cfg.xml.tmp"
#define QWIFI_MOBILEAP_CONF_FILE_BAK      "/etc/mobileap_cfg.xml.bak"

/*wlan0 mac*/
#define QWIFI_WLAN_MAC_CONF_FILE_LINK		  "/firmware/image/bdwlan.bin"
#define QWIFI_WLAN_MAC_CONF_FILE		  "/lib/firmware/bdwlan.bin"
#define QWIFI_WLAN_MAC_CONF_FILE_TEMP		  "/lib/firmware/bdwlan.bin.tmp"
/*TCP*/
#define WIFI_BRIDGE_TO_CLI_PIPE	"/data/wifi_bridge_in_pipe"
#define WIFI_BRIDGE_TO_AT_PIPE	"/data/wifi_bridge_out_pipe"
#ifdef QUECTEL_WIFI_DATA_ATC// frederic 2017.03.20 add for wifi client data count
#define WIFI_USRDATA_TO_LST  "/data/wifi_usrdata_to_lst"
#endif
typedef enum {
    Q_WIFI_CMD_STATUS,
    Q_WIFI_CMD_ENABLE,
    Q_WIFI_CMD_RESET_HOSTAPD,
    Q_WIFI_CMD_SET_NAT_TYPE,
    Q_WIFI_CMD_RESPONSE,
    Q_WIFI_CMD_RESTORE_FACTORY,
    Q_WIFI_CMD_SET_WEBSERVER,
    Q_WIRED_CMD_ENABLE,
#ifdef QUECTEL_WIFI_DATA_ATC// frederic 2017.03.20 add for wifi client data
    Q_WIFI_USEDATA_CMD,//frederic 
#endif 
    Q_WIFI_WIRED_WWAN_CMD_ENABLE,
    Q_WIFI_CMD_SET_WLAN_MODE,
    Q_WIFI_CMD_GET_WLAN_MODE,
    Q_WIFI_CMD_MAX,
} qwifi_cmd_e_type;

typedef enum {
    Q_SSID_INIT=-1,
    Q_SSID_ASCII=0,
    Q_SSID_HEX,

    Q_SSID_MAX,
} qwifi_ssid_uncode;


typedef struct {
    char s_mac[64];
    int flag;
}QW_FIND;
typedef struct {
	QW_FIND qw_find[40];
    int connected_num;
} qwifi_device_info_type;
#ifdef QUECTEL_WIFI_DATA_ATC// frederic 2017.03.20 add for wifi client data count
typedef struct
{
    char s_mac[64];
    char wifi_usr_IP[32];
    char wifi_usr_tx[32];
    char wifi_usr_rx[32];
    int  is_valid_cnt;
    int  online_flag;
}qwifi_usr_device_info;
typedef struct
{
    qwifi_usr_device_info usr_list[32];
    int  count;
}device_record_info;
typedef struct
{
    int flag_mac;
    int flag_ip;
    int flag_tx;
    int flag_rx;
    int flag_state;
}optionplay_type;
typedef enum
{
    QUEC_MAC = 0,
    QUEC_IP,
    QUEC_TX,
    QUEC_RX,
    QUEC_STATE,
    QUEC_MAX_OPT
}quec_wifi_opt_type;
#endif	
#define QCMAP_MSGR_MAC_ADDR_LEN_V01 6
#define QCMAP_MSGR_DEVICE_NAME_MAX_V01 100
#define QCMAP_MSGR_IPV6_ADDR_LEN_V01  16
#define QCMAP_MSGR_MAX_CONNECTED_DEVICES_V01 40

typedef enum {
  QCMAP_TYPE_ENUM_MIN_ENUM_VAL_V01 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
  QCMAP_TYPE_PRIMARY_AP_V01 = 0x01, /**<  Device type is Primary AP  */
  QCMAP_TYPE_GUEST_AP_V01 = 0x02, /**<  Device type is Guest AP  */
  QCMAP_TYPE_USB_V01 = 0x03, /**<  Device type is USB  */
  QCMAP_TYPE_ETHERNET_V01 = 0x04, /**<  Device type is ETHERNET  */
  QCMAP_TYPE_ENUM_MAX_ENUM_VAL_V01 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}qcmap_msgr_device_type_enum_v02;

typedef struct {

  uint8_t client_mac_addr[QCMAP_MSGR_MAC_ADDR_LEN_V01];
  /**<   MAC address of the device. */

  qcmap_msgr_device_type_enum_v02 device_type;
  /**<   Device type primary AP/guest AP/USB client. */

  char host_name[QCMAP_MSGR_DEVICE_NAME_MAX_V01];
  /**<   Host name of dynamically IP allocated WLAN/USB clients. */

  uint32_t lease_expiry_time;
  /**<   Lease expiry time in minutes for dynamically IP allocated WLAN/USB clients. */

  uint32_t ipv4_addr;
  /**<   IPv4 address of the client. */

  uint8_t ll_ipv6_addr[QCMAP_MSGR_IPV6_ADDR_LEN_V01];
  /**<   Link Local IPv6 address of the client. */

  uint8_t ipv6_addr[QCMAP_MSGR_IPV6_ADDR_LEN_V01];
  /**<   IPv6 address of the client. */
}qcmap_msgr_connected_device_info_v02;  /* Type */


typedef struct {
    qcmap_msgr_connected_device_info_v02
            connected_devices[QCMAP_MSGR_MAX_CONNECTED_DEVICES_V01];
    int device_num;
}DEV_INFO;


typedef struct {
	qwifi_cmd_e_type cmd;
        int ap_status;
        int wlan_status;
        qwifi_ssid_uncode ssid_uncode;
        int nat_type;
        int websrv_status;
        DEV_INFO dev_info;
        QW_FIND qw_find;
        int wifi_enable;
        int wired_enable;
        int wwan_enable;//0 disable ,1 only enable wwan ,null enable 
        int wlan_mode; // 1 AP Mode, 3 AP+STA Mode
} qwifi_cmd_para_type;

typedef struct {
	int r_cmd;
	int command;
	int high_limit;
	int wifi_mode;
	int reserved;
	char *str;
} qwparam_para_s_type;

typedef struct {
    char ssid[32];
    char key_mgnt[12];
    char passwd[64];
} wpa_supplicant_conf;

#ifdef __cplusplus
}
#endif

#define QCMAP_WIFI_LOG(...)                         \
  fprintf( stderr, "\n%s %d:", __FILE__, __LINE__); \
  fprintf( stderr, __VA_ARGS__ )

extern qwifi_cmd_para_type qwifi_cmd;

#endif /* __QUECTEL_WIFI_DEFS__ */

