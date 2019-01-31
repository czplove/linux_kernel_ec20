/*
 * example_qspk.c  --  read/write the volume of uplink
 *
 * Author: Grady <grady.quan@quectel.com>
 *
 * Date: 2018-06-06
 */

#include "ql_oe.h"

int main(int argc, char* argv[])  
{
	uint16_t spk_gain = 0;

	if (ql_spk_gain_read(&spk_gain) == TRUE)
	{
		fprintf(stderr, "%s: qspk read true, spk_gain: %d\n", __FUNCTION__, spk_gain);
	} else {
		fprintf(stderr, "%s: qspk read false\n", __FUNCTION__);
	}
	
	spk_gain = 0x4000;

	if (ql_spk_gain_write(spk_gain) == TRUE)
	{
		fprintf(stderr, "%s: qspk write true, spk_gain: %d\n", __FUNCTION__, spk_gain);
	} else {
		fprintf(stderr, "%s: qspk write false\n", __FUNCTION__);
	}

	if (ql_spk_gain_read(&spk_gain) == TRUE)
	{
		fprintf(stderr, "%s: qspk read true, spk_gain: %d\n", __FUNCTION__, spk_gain);
	} else {
		fprintf(stderr, "%s: qspk read false\n", __FUNCTION__);
	}
	
	return 0;  
}
