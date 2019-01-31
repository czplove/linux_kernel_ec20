
/*
 *  Quectel at command handler common defines
 *
 *  file name: quectel_at_handler_defs.h
 *
 *  Design by: scott.hu
 *
 *  Date: 2015-06-15 
 */

#ifndef __QUECTEL_AT_HANDLER_DEFS__
#define __QUECTEL_AT_HANDLER_DEFS__

#ifdef __cplusplus
extern "C" {
#endif

#include <qmi_atcop_srvc.h>
#include "quectel-features-config.h" //running.qian 2017.05.12

#define	QUECTEL_AT_COMMON_SUPPORT	//should be removed in the future!!!!       
#ifdef  QUECTEL_AT_COMMON_SUPPORT       

#ifndef QUECTEL_FEATURE_OPENLINUX
#define QUECTEL_FEATURE_OPENLINUX   // Stanley-20170726, added for QuecOpen 
#endif

#define	QUECTEL_AUDIO_AT_CMD	//define audio macro 
#define QUECTEL_QTTS_AT_CMD     //define TTS macro, add by running.qian,20160625
#define QUECTEL_QWTTS_AT_CMD    //define WTTS macro, add by becking 20161026
#define QUECTEL_TTS_QLAPI_MODE    //rock.wang-20180428 change TTS interface
#define QUECTEL_QTONE_QDTMF_AT_CMD     //define TONE&DTMF macro, add by SUNDY.WANG,20160912
#define QUECTEL_QPCMV_AT_CMD        //define QPCMV macro, add by becking.bai,bflag20161215
#define QUECTEL_QPSND_AT_CMD 		//define QPSND macro, add by hertz.zhou
#define QUECTEL_QLDTMF_AT_CMD 		//define QLDTMF macro, add by hertz.zhou
#define QUECTEL_PCM16K_CONFIG 		//define PCM16K macro, add by hertz.zhou
#define QUECTEL_QIIR_AT_CMD        //define QIIR macro, add by becking.bai,bflag20170509
#define QUECTEL_POWER_AT			//define power macro
#define QUECTEL_DRIVER_ATC_QSCLK	//define sclk macro
#define QUECTEL_QNAND  // Ramos 20160215 add for AT+QNAND CMD
#define QUECTEL_USB_CONFIG //will.shao for usb vid/pid config
#define QUECTEL_ADC
#define QUECTEL_LINUX_PRINT_ATC         //Bob 20160416 add for AT+QPRINT CMD
#define QUECTEL_FASTBOOT_ATC         //Bob 20160428 add for AT+QFASTBOOT CMD
#define QUECTEL_QCFG_ITEM        // Ramos  20160622 add for qcfg
#define QUECTEL_FC20_WIFI_FEATURE  //add by hui.chen 20160718 for wifi at command
//#define QUECTEL_LINUX_APP_DETECT  //Ramos 20160908 add for detect linux file app
#define QUECTEL_LINUX_SDMOUNT_ATC  //Sundy 20160804 add for AT+QSDMOUNT CMD
#define QUECTEL_LINUX_GPS_UARTDEBUG  //Sundy 20160918 add 
#define QUECTEL_PSM_AWARE_SUPPORT //add by chao.zhou 2016/08/19 for psm feature
#define QUECTEL_VBATT_VOLTAGE_CHECK    //sundy 20161010 add for voltage check
#define QUECTEL_QTEMP
#define QUECTEL_CODEC_POWSAVE_ATC     //sundy 20161202 add for codec powsave
#define QUECTEL_QCFG_BOOTUP
#define QUECTEL_WIFI_DATA_ATC// frederic 2017.03.20 add for wifi client data count
#define QUECTEL_THERMAL_ATC
#define QUECTEL_MULTI_IP_PACKAGES
#define QUECTEL_COMMON_QMI //lory 2017/04/19
#define QUECTEL_AT_QCAUTOCONNECT_SUPPORT//duke  2017/04/19 for autoconnect to AP
#define QUECTEL_DECGAIN_5616_PWRCTL_NV	//add hertz for decgain and 5616pwrctl nv save
#define QUECTEL_NET_STATUS_SUPPORT
#define QUECTEL_AT_DEBUG_SUPPORT
#define QUECTEL_AT_SUBSYS_VERSION  //Wayne.wei -2018/5/25 :add an AT command :AT+QSUBSYSVER to query subsystem version of TZ/SBL/RPM 
#define QUECTEL_ABOUT_ATC // add by running.qian, 2018.3.20
//#define QUECTEL_QJSON
#define QUECTEL_FTCMD_ATC //[Quinn]2018/04/11 Add for support fct test
#define QUECTEL_THERMAL_EXTENSION_SUPPORT//duke  2018/06/19 for the third thermal config 
#define QUECTEL_QCUSTAT    1// add for by pass at cmd from modem to linux
#ifndef boolean
#define boolean unsigned int
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

typedef struct {
    int opcode;
    char *name;
    int ntokens;
    char **tokens;
} AtCmd;

#define Q_AT_CMD_REQ_TMP(name)  					\
{                   								\
    1,              								\
    {               								\
        { QMI_ATCOP_AT_CMD_NOT_ABORTABLE, name },  \
    }   											\
},

#define IS_READ_COMMAND(opcode)     (opcode == (QMI_ATCOP_NA | QMI_ATCOP_QU))
#define IS_WRITE_COMMAND(opcode)    (opcode == (QMI_ATCOP_NA | QMI_ATCOP_EQ | QMI_ATCOP_AR))
#define IS_EXEC_COMMAND(opcode)		(opcode == (QMI_ATCOP_NA))

#define quec_info(...) fprintf(stderr, "function: %s, line: %d, quec_info: " __VA_ARGS__, __FUNCTION__, __LINE__)	//should no argument!!!

#define quec_debug(...) fprintf(stderr, "quec_debug: " __VA_ARGS__)

#define quec_pr(...) (fprintf(stderr, "func: %s, line: %d, ", __FUNCTION__, __LINE__), fprintf(stderr, " " __VA_ARGS__))

typedef struct {
    int cmd;
    int opcode;
    void *para;
    AtCmd *at_cmd_para; 
} atfwd_param_s_type;

typedef struct {
    int result;
    char *response;
}quec_AtCmdResponse;


#ifdef QUECTEL_QCFG_ITEM
typedef struct{
char Item_name[20];
void *para;
}Qcfg_item_type;

typedef struct
{
     char *name;
     int (*proc_func)(AtCmd *cmd, atfwd_param_s_type *para);
}quectel_common_cfg_parameters_type;


typedef struct
{
     char *name;
     int (*proc_func)(void* param, quec_AtCmdResponse *response);
}quectel_common_cfg_handle_type;
#endif

#ifdef QUECTEL_QCFG_BOOTUP
#define ETC_RC5 "/etc/rc5.d/"

struct bootup
{
    /*
     * 0: SET disable services
     * 1: SET enable services
     * 2: GET services status
     */
    int enable;
    char name[64];
};

#endif

typedef int (*parse_handler)(AtCmd *, atfwd_param_s_type *);
typedef void(*at_handler)(void *, quec_AtCmdResponse *);
typedef void(*query_handler)(quec_AtCmdResponse *);
typedef void(*exec_handler)(quec_AtCmdResponse *);
typedef int (*fuc_handler )(void *param1, AtCmd *cmd, atfwd_param_s_type *param, quec_AtCmdResponse *response);

extern boolean strip_quotes_out
(
	const char *quoted_str, char *out_str, int str_len
);

extern int quec_atoi(const char *num);

extern int parse_1int_parameter(AtCmd *cmd, atfwd_param_s_type *para);
extern int parse_2int_parameter(AtCmd *cmd, atfwd_param_s_type *para);
extern int quectel_system(char *cmd, char *buff, int length, int need_return);

extern void atfwd_send_urc(char *urcString);
extern char* strlwr(char *str);//maxcodeflag20161013

#endif

typedef struct{
char Item_name[20];
void *para;
}Wificfg_item_type;

typedef struct
{
     char *name;
     int (*proc_func)(AtCmd *cmd, atfwd_param_s_type *para);
}quectel_common_wificfg_parameters_type;


typedef struct
{
     char *name;
     int (*proc_func)(void* param, quec_AtCmdResponse *response);
}quectel_common_wificfg_handle_type;

#ifdef __cplusplus
}
#endif

#endif /* __QUECTEL_AT_HANDLER_DEFS__ */

