#include <stdio.h>
#include "ql_powerdown.h"

int main(int argc, char* argv[])
{
	int mode;

	printf("*	0 : gracefully power down\n");
	printf("*	1 : gracefully reboot\n");
	printf("*	2 : Immediately power down system, without unmounting or syncing filesystems\n");
	printf("*	3 : Immediately reboot the system, without unmounting or syncing filesystems\n");
	printf("*	other num : exit\n");

	scanf("%d", &mode);

	if(mode < 0 || mode > 3)
		return 0;

	Ql_Powerdown(mode);

	return 0;
}

