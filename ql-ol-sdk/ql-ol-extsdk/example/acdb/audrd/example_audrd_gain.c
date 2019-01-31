/*
 * example_audrd_gain.c  --  read/write the volume of audio record 
 *
 * Author: Grady <grady.quan@quectel.com>
 *
 * Date: 2018-03-27
 */

#include "ql_oe.h"

int main(int argc, char* argv[])  
{
	int iRet = 0;
	int audrd_gain = 0;

	audrd_gain = ql_audrd_gain_read();
	if (audrd_gain < 0) {
		fprintf(stderr, "%s: audrd gain read false\n", __FUNCTION__);
	} else {
		fprintf(stderr, "%s: audrd gain read true, value: %d\n", __FUNCTION__, audrd_gain);
	}
	
	usleep(1000000);
	audrd_gain = 0x1000;
	iRet = ql_audrd_gain_write(audrd_gain);
	if (iRet < 0) {
		fprintf(stderr, "%s: audrd gain write false\n", __FUNCTION__);
	} else {
		fprintf(stderr, "%s: audrd gain write true, value: %d\n", __FUNCTION__, audrd_gain);
	}
	
	usleep(1000000);
	audrd_gain = 0;
	audrd_gain = ql_audrd_gain_read();
	if (audrd_gain < 0) {
		fprintf(stderr, "%s: audrd gain read false\n", __FUNCTION__);
	} else {
		fprintf(stderr, "%s: audrd gain read true, value: %d\n", __FUNCTION__, audrd_gain);
	}
		
	return 0;  
}
