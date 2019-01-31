#include "stdio.h"
#include "string.h"
#include "unistd.h"
#include "sys/wait.h"

#define FOTA_UPDATE_FILE "/usrdata/cache/fota/ipth_package.bin"

void help(char * prog)
{
	printf("%s package\n", prog);
}

char *cmd_tab1[] = {
    "rm -rf /usrdata/cache",
    "mkdir -p /usrdata/cache/fota",
    "mkdir -p /usrdata/cache/recovery",
};
char *cmd_tab2[] = {
    "ubiattach -m 15 -d 3 /dev/ubi_ctrl",
    "mkdir -p /tmp/mount_recovery",
    "mount -t ubifs /dev/ubi3_0 /tmp/mount_recovery -o bulk_read",
    "rm /tmp/mount_recovery/sbin/usb/boot_hsusb_composition",
    "ln -s /sbin/usb/compositions/recovery_9607 /tmp/mount_recovery/sbin/usb/boot_hsusb_composition",
    "sync",
    "sys_reboot recovery"
};

int do_system(char* str_cmd)
{
    int status;
    status = system(str_cmd);
    if( status < 0 ){
        printf("system error\n");
        return -1;
    }
    else{
        if( WIFEXITED(status) ){
            if ( 0 == WEXITSTATUS(status) ) printf("\"%s\" run successfully.\n", str_cmd);
            else printf("\"%s\" run fail, exit code [ %d ]\n", str_cmd,  WEXITSTATUS(status));
        }
        else{
            printf("exit status = [ %d ]\n", WEXITSTATUS(status));
            return -1;
        }
    }
    return 0;
}

int main(int argc, char* argv[])
{
    int i = 0;
	int ret = 0;
    int str_len = 0;
    char str_buf[128];

	if (argc < 2){
		help(argv[0]);
        return -1;
    }

    str_len = sizeof(cmd_tab1) / sizeof(*cmd_tab1);
    for ( i = 0; i < str_len; i++ ){
        ret = do_system(*(cmd_tab1 + i));
        if ( ret < 0 ) return ret;
    }

    memset(str_buf, 0, sizeof(str_buf));
    if ( access(argv[1], F_OK) == -1){
        printf("can't access file [ %s ]", argv[1]);
        return -1;
    }
    sprintf(str_buf, "cp %s %s", argv[1], FOTA_UPDATE_FILE);
    ret = do_system(str_buf);
    if ( ret < 0 ) return ret;

    str_len = sizeof(cmd_tab2) / sizeof(*cmd_tab2);
    for ( i = 0; i < str_len; i++ ){
        ret = do_system(*(cmd_tab2 + i));
        if ( ret < 0 ) return ret;
    }

	return 0;
}
