#include "ql_oe.h"

int main(int argc, char* argv[])
{
	int wakelock_fd = Ql_SLP_WakeLock_Create("quec_wakelock_sample", sizeof("quec_wakelock_sample"));
	int wakelock_mode;
	
	if (wakelock_fd < 0)
	{
		printf("Create wakelock failed!");
		return -1;
	}

	printf("Please input the number (1-3) to set sleep mode. \n1. lock	the wakelock\n2. unlock the wakelock\n3. exit\n");
	while (1)
	{
		scanf("%d", &wakelock_mode);

		if (wakelock_mode == 1)
		{
			printf("The wakelock will be locked! So the system won't go to sleep.\n");

			if (Ql_SLP_WakeLock_Lock(wakelock_fd) != 0)
			{
				Ql_SLP_WakeLock_Destroy(wakelock_fd);
				printf("Lock wakelock failed! The programe will exit.\n");
				return -1;
			}		
		}
		else if (wakelock_mode == 2)
		{
			printf("The wakelock will be unlocked! So the system may went to sleep.\n");
			printf("Please unplug the usb, and send at commond at+qsclk=1 to enable sleep mode, then wait the system to go to sleep.\n");
			if (Ql_SLP_WakeLock_Unlock(wakelock_fd) != 0)
			{
				Ql_SLP_WakeLock_Destroy(wakelock_fd);
				printf("Unlock wakelock failed! The programe will exit.\n");
				return -1;
			}	
		}
		else if (wakelock_mode == 3)
		{
			break;
		}
	}
	
	Ql_SLP_WakeLock_Destroy(wakelock_fd);

	return 0;
}

