#ifndef QL_NF3303_BLE_COMMON
#define QL_NF3303_BLE_COMMON

/*Operate BT_EN to enable BT Module*/
void ql_bt_module_enable(void );

typedef enum{
    BT_EN_LOW  = 0,
    BT_EN_HIGH = 1
}Enum_BTEnLevel;
int ql_bt_en_set_level(Enum_BTEnLevel pin_value);

#endif /*QL_NF3303_BLE_COMMON*/
