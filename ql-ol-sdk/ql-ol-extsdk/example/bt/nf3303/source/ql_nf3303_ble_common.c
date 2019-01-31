#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include <time.h>

#include "ql_oe.h"
#include "ql_gpio.h"
#include "ql_nf3303_ble_common.h"
#include "ql_nf3303_bt_debug.h"

/*
 * define the debug level of this file,
 * please see 'debug.h' for detail info
 */
DEBUG_SET_LEVEL(DEBUG_LEVEL_ERR);

/*Operate BT_EN to enable BT Module*/
void ql_bt_module_enable()
{
    /*Pull Up PM_ENABLE to Power the Module*/
    system("echo 1 > /sys/kernel/debug/regulator/rome_vreg/enable");
    
    /*Reset BT Module*/
    ql_bt_en_set_level(BT_EN_LOW); //pull Down BT_EN
    usleep(200000);
    ql_bt_en_set_level(BT_EN_HIGH); //pull Up BT_EN
}

/*========================================================================
  FUNCTION:  ql_bt_en_set_level
=========================================================================*/
/**
    @brief
        Set BT_EN Pin Value.
    
    @dependencies
       None
*/
/*=========================================================================*/
#define SYSFS_RFKILL_PATH "/sys/class/rfkill/rfkill0/state"
int ql_bt_en_set_level(Enum_BTEnLevel pin_value)
{
    char path[64];
    char attr[16];

    FILE *fp = NULL;

    snprintf(path, sizeof(path), "%s", SYSFS_RFKILL_PATH);
    snprintf(attr, sizeof(attr), "%d", pin_value);
    do
    {
        fp = fopen(path, "w");
    }
    while ((fp == NULL) && (errno == EINTR));

    if (!fp)
    {
        ERR("Error opening file %s for writing.\n", path);
        return -1;
    }

    ssize_t written = fwrite(attr, 1, strlen(attr), fp);
    fflush(fp);

    int file_err = ferror(fp);
    if (file_err != 0)
    {
        ERR("Failed to write %s to GPIO config %s. Error %s\n", attr, path, strerror(file_err));
        fclose(fp);
        return RES_IO_ERROR;
    }

    if (written < strlen(attr))
    {
        ERR("Data truncated while writing %s to GPIO config %s.\n", path, attr);
        fclose(fp);
        return RES_IO_ERROR;
    }
    fclose(fp);
    return 0;
}

