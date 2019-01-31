/*
 * example_qmic.c  --  read/write the volume of uplink
 *
 * Author: Grady <grady.quan@quectel.com>
 *
 * Date: 2018-06-06
 */

#include "ql_oe.h"

int main(int argc, char* argv[])  
{
	uint16_t vol = 0, mic_gain = 0;

	if (ql_mic_gain_read(&vol, &mic_gain) == TRUE)
	{
		fprintf(stderr, "%s: qmic read true, vol: %d, mic_gain: %d\n", __FUNCTION__, vol, mic_gain);
	} else {
		fprintf(stderr, "%s: qmic read false\n", __FUNCTION__);
	}
	
	vol = 0x4000;
	mic_gain = 0x4000;

	if (ql_mic_gain_write(vol, mic_gain) == TRUE)
	{
		fprintf(stderr, "%s: qmic write true, vol: %d, mic_gain: %d\n", __FUNCTION__, vol, mic_gain);
	} else {
		fprintf(stderr, "%s: qmic write false\n", __FUNCTION__);
	}

	if (ql_mic_gain_read(&vol, &mic_gain) == TRUE)
	{
		fprintf(stderr, "%s: qmic read true, vol: %d, mic_gain: %d\n", __FUNCTION__, vol, mic_gain);
	} else {
		fprintf(stderr, "%s: qmic read false\n", __FUNCTION__);
	}
	
	return 0;  
}
