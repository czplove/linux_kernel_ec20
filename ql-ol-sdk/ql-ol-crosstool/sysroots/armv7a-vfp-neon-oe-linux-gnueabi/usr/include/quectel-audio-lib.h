/* file name: quectel-audio-lib.h
 * author	: jun
 * date		: 20170211
 * this file is quectel-audio-lib, provide some basic function for audio play and record 
 */

#include <comdef.h>
#include "alsa_audio.h"

/*
 * describe : this function is use to set mixer
 * paras	:
 *		device: point to the mixer string 
 *		count : the number of the para for device
 *		value : digital string seprate by empty char, the number of digital should equel to count
 *				para must be seprated by empty char !!!!!!!!!!!!!!
 * return	:
 *		return true if set sucess, else return false.
 */
boolean quectel_clt_set_mixer_value(const char *device, int count, const char *value);

/*
 * describe : this function is use to stop play wav file  
 * paras	:
 * return	:
 */
void quec_stop_play_wav_file(void);

/*
 * describe : this function is use to stop play wav file  
 * paras	:
 * return	:
 */
int quec_audio_is_file_playing(void);

/*
 * describe : this function is use to play wav file  
 * paras	:
 *		device : this should be fix to hw:0,0 
 *		filename: wav file name
 * return	:
 */
int quec_play_aud_file(const char *filename);

/*
 * describe : this function is use to stop play file  
 * paras	:
 * return	:
 */
void quec_stop_play_file(void);

/*
 * describe : this function is use to open pcm device 
 * paras	:
 *		device : this should be fix to hw:0,0 
 *		flags ; pcm play flags
 *		rate: sample rate 
 *		channels  : audio channal 1 or 2      
 *		format: format to play or record, 16bit line,MP3
 *		hostless: if there is no file it is true
 * return	:
 *		pcm : pcm handle, use can use this handle to read write data
 */
struct pcm *quec_pcm_open(char *device, unsigned flags, unsigned rate, unsigned channels, unsigned format, unsigned hostless);

/*
 * describe : this function is use to close pcm handle 
 * paras	:
 *		pcm : pcm handle to close  
 * return	:
 */
int quec_pcm_close(struct pcm *pcm );

/*
 * describe : this function is use to write buffer 
 * paras	:
 *		pcm : pcm handle to write date 
 *		buffer: data buffer 
 *		lenth: data length
 * return	:
 */
int quec_write_pcm(struct pcm *pcm, void * buffer, int lenrth);

/*
 * describe : this function is use to read pcm buffer 
 * paras	:
 *		pcm : pcm handle to write date 
 *		buffer: data buffer 
 *		lenth: data length
 * return	:
 */
int quec_read_pcm(struct pcm *pcm, void * buffer, int length);

/*
 * describe : this function is use to get pcm buffer lenth 
 * paras	:
 *		lenth: data length
 * return
 *		buffer length 	
 */
int quec_get_pem_buffer_len(struct pcm *pcm);

/*
 * describe : this function is use to open mixer device 
 * paras	:
 *		device: mixer device
 * return
 *		mixer handle 	
 */
struct mixer *quec_mixer_open(const char *device);

/*
 * describe : this function is use to close mixer device
 * paras	:
 *		mixer: mixer handle
 * return
 *		none 	
 */
void quec_mixer_close(struct mixer *mixer);

/*
 * describe : this function is use to get mixer devie control 
 * paras	:
 *		mixer: mixer handle
 *		name: mixer device
 *		index: mixer index
 * return
 *		mixer control 	
 */
struct mixer_ctl *quec_mixer_get_control(struct mixer *mixer, const char *name, unsigned index);

/*
 * describe : this function is use to set mulvalues
 * paras	:
 *		mixer: mixer handle
 *		count: count
 *		argv: data
 * return	:
 *			
 */
int quec_mixer_ctl_mulvalues(struct mixer_ctl *ctl, int count, char ** argv);

