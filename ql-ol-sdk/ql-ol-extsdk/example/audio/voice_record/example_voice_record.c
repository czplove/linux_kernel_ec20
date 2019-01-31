/*
 * example_voice_record.c
 *
 * Author: Grady <grady.quan@quectel.com>
 *
 * Date: 2018-03-27
 */

#include "ql_oe.h"

#define FILE_NAME "/data/record.wav"
#define QAUDRD_MAX_FILE_SIZE   	320 *1000

int main(int argc, char* argv[])  
{
	char *buffer;
        int fd;
	static struct wav_header hdr;
	int bufsize = 0, rec_size = 0;
	
	if((fd = open(FILE_NAME, O_WRONLY | O_CREAT | O_TRUNC, 0664)) < 0)
	{
		fprintf(stderr, "%s: cannot open file\n", __FUNCTION__);
		return -1;
	}
	memset(&hdr, 0, sizeof(struct wav_header));
	hdr.riff_id = ID_RIFF;
	hdr.riff_fmt = ID_WAVE;
	hdr.fmt_id = ID_FMT;
	hdr.fmt_sz = 16;
	hdr.audio_format = FORMAT_PCM;
	hdr.num_channels = 1;
	hdr.sample_rate = 8000;
	hdr.bits_per_sample = 16;
	hdr.byte_rate = (8000 * 1 * hdr.bits_per_sample) / 8;
	hdr.block_align = ( hdr.bits_per_sample * 1 ) / 8;
	hdr.data_id = ID_DATA;
	hdr.data_sz = 0;

	hdr.riff_sz = hdr.data_sz + 44 - 8;
	if (write(fd, &hdr, sizeof(hdr)) != sizeof(hdr))
	{
		return -errno;
	}
	
	//set up link device
	if (ql_voice_record_dev_set(AUD_DOWN_LINK) == FALSE)
	{
		fprintf(stderr, "%s: set device path error\n", __FUNCTION__);
	}

	//8k rate, single channel
	if (ql_voice_record_open(QUEC_PCM_8K, QUEC_PCM_MONO) == FALSE)
	{
		fprintf(stderr, "%s: voice open false\n", __FUNCTION__);
	}
	
	//get buffer lenth
	buffer = malloc(ql_get_voice_record_buffer_len());

	//while(!quec_read_pcm(pcm, buffer, bufsize))
	while(rec_size < QAUDRD_MAX_FILE_SIZE)
	{
		//read
		bufsize = ql_voice_record_read(buffer);

		if (write(fd, buffer, bufsize) != bufsize)
		{
			fprintf(stderr, "%s: cannot write %d bytes\n", __FUNCTION__, bufsize);
		    	break;
		}
		rec_size += bufsize;
		hdr.data_sz += bufsize;
		hdr.riff_sz = hdr.data_sz + 44 - 8;

		lseek(fd, 0, SEEK_SET);
		write(fd, &hdr, sizeof(hdr));
		lseek(fd, 0, SEEK_END);

		system("sync");
		memset(buffer, 0, sizeof(buffer));
	}
	close(fd);
	free(buffer);

	//close
	ql_voice_record_close();

	//clear up link device
	ql_voice_record_dev_clear(AUD_DOWN_LINK);

	return 0;  
}
