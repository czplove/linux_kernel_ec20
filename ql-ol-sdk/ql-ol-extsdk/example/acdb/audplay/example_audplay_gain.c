/*
 * example_audplay_gain.c  --  read/write the volume of audio playback
 *
 * Author: Grady <grady.quan@quectel.com>
 *
 * Date: 2018-03-27
 */

#include "ql_oe.h"

int main(int argc, char* argv[])  
{
	int iRet = 0;
	int audplay_gain = 0;

	audplay_gain = ql_audplay_gain_read();
	if (audplay_gain < 0) {
		fprintf(stderr, "%s: audplay gain read false\n", __FUNCTION__);
	} else {
		fprintf(stderr, "%s: audplay gain read true, value: %d\n", __FUNCTION__, audplay_gain);
	}
	
	usleep(1000000);
	audplay_gain = 0x1000;
	iRet = ql_audplay_gain_write(audplay_gain);
	if (iRet < 0) {
		fprintf(stderr, "%s: audplay gain write false\n", __FUNCTION__);
	} else {
		fprintf(stderr, "%s: audplay gain write true, value: %d\n", __FUNCTION__, audplay_gain);
	}
	
	usleep(1000000);
	audplay_gain = 0;
	audplay_gain = ql_audplay_gain_read();
	if (audplay_gain < 0) {
		fprintf(stderr, "%s: audplay gain read false\n", __FUNCTION__);
	} else {
		fprintf(stderr, "%s: audplay gain read true, value: %d\n", __FUNCTION__, audplay_gain);
	}
		
	return 0;  
}
