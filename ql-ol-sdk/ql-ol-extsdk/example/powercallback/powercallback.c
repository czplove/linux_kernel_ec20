#include <stdio.h>
#include "ql_power_callback.h"

void myfun(int status)
{
	if(0 == status) {
		//short key pressed
		printf("short key pressed....\n");
	}else if(1 == status) {
		//long key pressed
		printf("long key pressed....\n");	
	}

}


int main(void)
{
	ql_powerkey_register_cb(400000,myfun);
	while(1) {
		printf("=====test=====\n");
		sleep(1);
	}

	return 0;
}






