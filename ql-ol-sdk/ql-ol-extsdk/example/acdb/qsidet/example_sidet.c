/*
 * example_qsidet.c  --  set/query sidetone
 *
 * Author: Grady <grady.quan@quectel.com>
 *
 * Date: 2018-06-20
 */

#include "ql_oe.h"

int main(int argc, char* argv[])  
{
	int iRet = 0;
	int sidet_value = 0;

	sidet_value = ql_sidet_read();
	fprintf(stderr, "%s: sidet value: %d\n", __FUNCTION__, sidet_value);
	
	sidet_value = 20000;
	if (ql_sidet_write(20000))
	{
		fprintf(stderr, "%s: set sidetone: %d\n", __FUNCTION__, sidet_value);
	} else {
		fprintf(stderr, "%s: set sidetone false\n", __FUNCTION__);
	}
	sidet_value = ql_sidet_read();
	fprintf(stderr, "%s: sidet value: %d\n", __FUNCTION__, sidet_value);
	
	return 0;  
}
