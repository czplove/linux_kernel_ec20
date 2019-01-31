/*
 * example_audloop.c  --  enable/disable audio loop back
 *
 * Author: Grady <grady.quan@quectel.com>
 *
 * Date: 2018-05-31
 */

#include "ql_oe.h"

int main(int argc, char* argv[])  
{
	int iRet = 0;
	int audloop_value = 0;

	audloop_value = ql_audloop_read();
	fprintf(stderr, "%s: audloop value: %d\n", __FUNCTION__, audloop_value);
	
	if (ql_audloop_write(ENABLE))
	{
		fprintf(stderr, "%s: audloop enable\n", __FUNCTION__);
	} else {
		fprintf(stderr, "%s: audloop enable false\n", __FUNCTION__);
	}
	
	sleep(10);

	if (ql_audloop_write(DISABLE))
	{
		fprintf(stderr, "%s: audloop disable\n", __FUNCTION__);
	} else {
		fprintf(stderr, "%s: audloop disable false\n", __FUNCTION__);
	}
		
	return 0;  
}
