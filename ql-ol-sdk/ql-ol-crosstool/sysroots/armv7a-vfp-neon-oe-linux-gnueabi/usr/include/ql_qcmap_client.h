#ifdef __cplusplus
extern "C" {
#endif
#define QL_QCMAPC_LOG(format, ...) fprintf(stderr, "[QL_WWAN][%d]:"format, __LINE__, ##__VA_ARGS__)

int ql_wds_client_init(void);
int ql_nas_client_init(void);
int ql_pdp_type_get(int profile_id);
int ql_pdp_type_set(int profile_id);
void ql_apn_enable(int profile_id);
void ql_apn_disable(int profile_id);
void ql_net_deattach(void);
void ql_net_attach(void);

int  ql_qcmap_client_init(void);
int  ql_qcmap_policy_set(int profile_id);
int  ql_qcmap_policy_get(int *profile_id);
int  ql_qcmap_autoconnect_set(boolean enable);
int  ql_qcmap_autoconnect_get(boolean *enable);
int  ql_qcmap_up(int call_type);
int  ql_qcmap_down(int call_type);

#ifdef __cplusplus
}
#endif

