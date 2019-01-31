#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ql_dev.h"

int main(void)
{
    int i;
    ql_module_about_info_s *module_about;
    
    module_about = malloc(sizeof(ql_module_about_info_s ));
    
    QL_Module_About_Get(module_about);
    
    printf("QL_PRODUCT: %s\n\rQL_FW_VERSION: %s\n\rQL_FW_DATE: %s\n\rQL_APPS_VERSION: %s\n\n",
           module_about->product_name, module_about->firmware_version, module_about->firmware_date, module_about->apps_verison);
    for (i=0; i < module_about->oem_about_count; i++) {
         printf("%s: %s\n\r", module_about->oem_about[i].oem_info_name, module_about->oem_about[i].oem_info_vaule);
    }


    ql_oem_about_info_s oem_about[9] = {
        {"OEM_NAME", "CUSTOMER"},
        {"PRODUCT_NAME", "TBOX-01"},
        {"HARDWARE_VERSION", "HW01.01"},
        {"APP1_NAME", "Helloworld1"},
        {"APP1_VERSION", "APP1.01.01"},
        {"APP1_DATE", "2018-01-01,12:00"},
        {"APP2_NAME", "Helloworld2"},
        {"APP2_VERSION", "APP2.01.01"},
        {"APP2_DATE", "2018-01-01,12:00"},
    };
    
    QL_OEM_About_Set(oem_about, 9);
    
    ql_oem_about_info_s oem_about1 = {"APP2_VERSION", "APP2.01.02"};
    QL_OEM_About_Set_ByName(&oem_about1);
    
    memset(oem_about1.oem_info_vaule, 0x00, QL_OEM_ABOUT_INFO_MAX_SIZE);
    QL_OEM_About_Get_ByName(&oem_about1);

    printf("\n\n");
    
    QL_Module_About_Get(module_about);
    
    printf("QL_PRODUCT: %s\n\rQL_FW_VERSION: %s\n\rQL_FW_DATE: %s\n\rQL_APPS_VERSION: %s\n\n",
           module_about->product_name, module_about->firmware_version, module_about->firmware_date, module_about->apps_verison);
    for (i=0; i < module_about->oem_about_count; i++) {
         printf("%s: %s\n\r", module_about->oem_about[i].oem_info_name, module_about->oem_about[i].oem_info_vaule);
    }

    free(module_about);
    
    return 0;
}