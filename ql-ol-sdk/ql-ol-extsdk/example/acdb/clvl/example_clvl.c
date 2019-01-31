/*
 * example_clvl.c  --  set clvl
 *
 * Author: Grady <grady.quan@quectel.com>
 *
 * Date: 2018-06-02
 */

#include "ql_oe.h"

int main(int argc, char* argv[])  
{
	int iRet = 0;
	int clvl = 0;

	clvl = ql_clvl_read();
	fprintf(stderr, "%s: clvl value: %d\n", __FUNCTION__, clvl);
	
	if (ql_clvl_write(5))
	{
		fprintf(stderr, "%s: clvl write true\n", __FUNCTION__);
	} else {
		fprintf(stderr, "%s: clvl write false\n", __FUNCTION__);
	}
	
	clvl = ql_clvl_read();
	fprintf(stderr, "%s: clvl value: %d\n", __FUNCTION__, clvl);
		
	return 0;  
}
