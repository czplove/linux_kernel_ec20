/*
 * example_uac.c  --  enable/disable uac
 *
 * Author: Grady <grady.quan@quectel.com>
 *
 * Date: 2018-06-28
 */

#include "ql_oe.h"

int main(int argc, char* argv[])  
{
	int iRet = 0;
	int audloop_value = 0;

	if (ql_uac_enable())
	{
		fprintf(stderr, "%s: uac enable\n", __FUNCTION__);
	} else {
		fprintf(stderr, "%s: uac enable false\n", __FUNCTION__);
	}
	
	sleep(20);

	if (ql_uac_disable())
	{
		fprintf(stderr, "%s: uac disable\n", __FUNCTION__);
	} else {
		fprintf(stderr, "%s: uac disable false\n", __FUNCTION__);
	}
		
	return 0;  
}
