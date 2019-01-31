/**  
  @file
  ql_audio.h

  @brief
  This file provides the definitions for audio & record, and declares the 
  API functions.

*/
/*============================================================================
  Copyright (c) 2017 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
 =============================================================================*/
/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.


WHEN        WHO            WHAT, WHERE, WHY
----------  ------------   ----------------------------------------------------
07/06/2018  Stanley.yong   Revise the comments for API.
19/04/2018  Zichar.zhang   Add Ql_Update_wav_size function.
                           Add Ql_AudPlayer_OpenExt function.
                           Add Ql_AudRecorder_OpenExt function.
19/12/2016  Stanley.yong   Optimize the APIs, and add comments.
13/12/2016  Running.qian   Implement newly-designed APIs.
18/07/2016	Jun.wu         Create
=============================================================================*/

#ifndef __AUD_H__
#define __AUD_H__

//this two headers for alsa params setting
#include "sound/asound.h"
#include "alsa-intf/alsa_audio.h"

#ifndef boolean
#define boolean unsigned int
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define QUEC_PCM_8K 8000
#define QUEC_PCM_16K 16000
#define QUEC_PCM_44k 44100
#define QUEC_PCM_MONO 1
#define QUEC_PCM_STEREO 2

typedef enum {
        AUD_UP_LINK = 0,
        AUD_DOWN_LINK,
} Enum_AudDlink;

typedef enum {
	AUD_PLAYER_ERROR = -1,
	AUD_PLAYER_START = 0,
	AUD_PLAYER_PAUSE,
	AUD_PLAYER_RESUME,
	AUD_PLAYER_NODATA,  //Buff no data and play tread will sleep
    AUD_PLAYER_LESSDATA, //Buff has less data
	AUD_PLAYER_FINISHED,
} Enum_AudPlayer_State;

typedef enum {
	AUD_RECORDER_ERROR = -1,
	AUD_RECORDER_START = 0,
	AUD_RECORDER_PAUSE,
	AUD_RECORDER_RESUME,
	AUD_RECORDER_FINISHED,
} Enum_AudRecorder_State;


/****************************************************************************
*  Audio Volume Level Definition
***************************************************************************/
typedef enum {
	AUD_VOLUME_LEVEL1 = 0,
	AUD_VOLUME_LEVEL2,
	AUD_VOLUME_LEVEL3,
	AUD_VOLUME_LEVEL4,
	AUD_VOLUME_LEVEL5,
	AUD_VOLUME_LEVEL_END
}Enum_AudVolumeLevel;

/****************************************************************************
*  Audio Format
***************************************************************************/
typedef enum {
	AUD_STREAM_FORMAT_MP3 = 0,
	AUD_STREAM_FORMAT_AMR = 1,
	AUD_STREAM_FORMAT_PCM = 2,
	AUD_STREAM_FORMAT_END
} Enum_AudStreamFormat;

/****************************************************************************
*  Audio Direct
***************************************************************************/
typedef enum {
	AUD_LINK_REVERSE = 0,
	AUD_LINK_FORWARD = 1,
	AUD_LINK_BOTH = 2,
	AUD_LINK_INVALID
}Enum_AudStreamDirection;


/*****************************************************************
********************New Advanced Audio High API******************
* ***************************************************************/
struct wav_header {
	uint32_t riff_id;
	uint32_t riff_sz;
	uint32_t riff_fmt;
	uint32_t fmt_id;
	uint32_t fmt_sz;
	uint16_t audio_format;
	uint16_t num_channels;
	uint32_t sample_rate;
	uint32_t byte_rate;       /* sample_rate * num_channels * bps / 8 */
	uint16_t block_align;     /* num_channels * bps / 8 */
	uint16_t bits_per_sample;
	uint32_t data_id;
	uint32_t data_sz;
};

#define ID_RIFF 0x46464952
#define ID_WAVE 0x45564157
#define ID_FMT  0x20746d66
#define ID_DATA 0x61746164
#define FORMAT_PCM 1

struct ST_MediaParams {
	Enum_AudStreamFormat     format;
	Enum_AudVolumeLevel      volume;
	Enum_AudStreamDirection     direct;
};


//
// Function:  _cb_onPlayer
//
// Description:
//   This callback function handles the result of audio player.
//
// @param hdl:
//   Handle received from Ql_AudPlayer_Open().
// @param result:
//   the executing result for previous operation, such as Open, Play, Pause, Resume, Stop.
//   see the definition of Enum_AudPlayer_State for the specific meaning.
typedef int(*_cb_onPlayer)(int hdl, int result);
//
// Function:  _cb_onRecorder
//
// Description:
//   This callback function handles the result of audio recorder.
//   If result < 0, the recorder will stop the recording automatically.
//
// @param result:
//   the executing result for previous operation, such as Open, StartRecord, Pause, Resume, Stop.
//   see the definition of Enum_AudRecorder_State for the specific meaning.
// @param pBuf:
//   pointer to the output recording (PCM) data.
// @param length:
//   the length of the output recording (PCM) data.
typedef int(*_cb_onRecorder)(int result, unsigned char* pBuf, unsigned int length);  //if result < 0 will stop record, else  continue record

/*****************************************************************
* Function:     Ql_AudPlayer_Open
*
* Description:
*               Open audio play device, and specify the callback function.
*               This function can be called twice to play different audio sources.
*
* Parameters:
*               device  : a string that specifies the PCM device.
*                         NULL, means the audio will be played on the default PCM device.
*
*                         If you want to mixedly play audio sources, you can call this
*                         API twice with specifying different PCM device.
*                         The string devices available:
*                            "hw:0,0"  (the default play device)
*                            "hw:0,13" (this device can mix audio and TTS)
*                            "hw:0,14"
*
*		        cb_func : callback function for audio player. 
*                         The results of all operations on audio player
*                         are informed in callback function.
*
* Return:
*               pcm device handle on success
*               -1 for failure
*****************************************************************/
int  Ql_AudPlayer_Open(char* device, _cb_onPlayer cb_func);

/*========================================================================
  FUNCTION:  Ql_AudPlayer_Play
=========================================================================*/
/** @brief 
    This function writes pcm data to pcm device to play.

    @param[in] hdl, the handle returned by Ql_AudPlayer_Open().
    @param[in] pData, pointer to the start address of pcm data.
    @param[in] length, the length of pcm data.

    @return
    on success, the return value is the number of bytes to play
    on failure, the return value is -1;

    @dependencies
    Ql_AudPlayer_Open() must be first called successfully.
*/
/*=======================================================================*/
int  Ql_AudPlayer_Play(int hdl, unsigned char* pData, unsigned int length);

/*========================================================================
  FUNCTION:  Ql_AudPlayer_PlayFrmFile
=========================================================================*/
/** @brief 
    This function plays the pcm data from the specified file.

    @param[in] hdl, the handle returned by Ql_AudPlayer_Open().
    @param[in] fd, a file descriptor that contains pcm data.
               Note:
                 the file offset should be set to the start position of pcm
                 data region, which means you should move the file offset 
                 skipping the file header (such as wave header, amr header).
    @param[in] offset, file offset. Please set it to -1 if no need to use.

    @return
       0 on success
      -1 on failure

    @dependencies
    Ql_AudPlayer_Open() must be first called successfully.
*/
/*=======================================================================*/
int  Ql_AudPlayer_PlayFrmFile(int hdl, int fd, int offset);
//
// Function:  Ql_AudPlayer_Pause
//
// Description:
//   Pause playing.
// @param hdl:
//   Handle received from Ql_AudPlayer_Open().
int  Ql_AudPlayer_Pause(int hdl);
//
// Function:  Ql_AudPlayer_Resume
//
// Description:
//   Resume playing.
// @param hdl:
//   Handle received from Ql_AudPlayer_Open().
int  Ql_AudPlayer_Resume(int hdl);
//
// Function:  Ql_AudPlayer_Stop
//
// Description:
//   Stop playing audio
// hdl:
//   Handle received from Ql_AudPlayer_Open().
void Ql_AudPlayer_Stop(int hdl);
//
// Function:  Ql_AudPlayer_Close
//
// Description:
//   Close player, and free the resource.
// @param hdl:
//   Handle received from Ql_AudPlayer_Open().
void Ql_AudPlayer_Close(int hdl);


int Ql_AudPlayer_set_LessDataThreshold(int hdl, unsigned short threshSize);

int Ql_AudPlayer_get_freeSpace(int hdl);


/*****************************************************************
* Function:     Ql_AudRecorder_Open
*
* Description:
*               Open audio record device, and specify the callback function.
*
* Parameters:
*               device  : not used. MUST be NULL.
*
*		        cb_func : callback function for audio player.
*                         The results of all operations on audio recorder
*                         are informed in callback function.
*
* Return:
*               pcm device handle
*               -1 for failure
*****************************************************************/
int  Ql_AudRecorder_Open(char* device, _cb_onRecorder cb_fun);
//
// Function:  Ql_AudRecorder_StartRecord
//
// Description:
//   Start to record.
//   The record data is output in _cb_onRecorder.
//
// Return:
//            0 on success
//            -1 on failure
int  Ql_AudRecorder_StartRecord(void);
//
// Function:  Ql_AudRecorder_Pause
//
// Description:
//   Pause recording
int  Ql_AudRecorder_Pause(void);
//
// Function:  Ql_AudRecorder_Resume
//
// Description:
//   Resume recording
int  Ql_AudRecorder_Resume(void);
//
// Function:  Ql_AudRecorder_Stop
//
// Description:
//   Stop recording
void Ql_AudRecorder_Stop(void);
//
// Function:  Ql_AudRecorder_Close
//
// Description:
//   Close recorder, and free the resource
void Ql_AudRecorder_Close(void);

//
// Function:  Ql_clt_set_mixer_value
//
// Description:
//   Close recorder, and free the resource
boolean Ql_clt_set_mixer_value(const char *device, int count, const char *value);


//****************QL TONE API************************//
struct Ql_TonePara {
    unsigned int lowFreq;   //100-4000HZ
    unsigned int highFreq;  //100-4000HZ
    unsigned int volume;    //0 -1000
    unsigned int duration;  // >0 ms
};

int Ql_AudTone_Open(char* device, _cb_onPlayer cb);//cb not support now
int Ql_AudTone_Start(int hdl, struct Ql_TonePara *para);
void Ql_AudTone_Stop(int hdl);
void Ql_AudTone_Close(int hdl);


//****************QL Codec API************************//
struct Ql_ALC5616_DRCAGC {
    unsigned short control1_mask;
    unsigned short control1_value;
    unsigned short control2_mask;
    unsigned short control2_value;
    unsigned short control3_mask;
    unsigned short control3_value;
};

//
// Function:  Ql_AudCodec_Set_ALC5616_DRCAGC
//
// Description:
//   Set ALC5616 DRC/AGC configuration
int Ql_AudCodec_Set_ALC5616_DRCAGC(const char *i2c, struct Ql_ALC5616_DRCAGC *cfg);

//
// Function:   Ql_Update_wav_size
//
// Description:
//   update wav format file size in the header
// @param fd:
//   wav file discriptor
// @param size:
//   wav file size to update
int Ql_Update_wav_size(int fd, int size);

//add by grady, 2018-5-29
/*
 * describe : this function is use to open pcm device 
 * paras    :
 *        device : this should be fix to hw:0,0 
 *        flags ; pcm play flags
 *        rate: sample rate 
 *        channels  : audio channal 1 or 2      
 *        format: format to play or record, 16bit line,MP3
 *        hostless: if there is no file it is true
 * return    :
 *        pcm : pcm handle, use can use this handle to read write data
 */
struct pcm *quec_pcm_open(char *device, unsigned flags, unsigned rate, unsigned channels, unsigned format, unsigned hostless);

/*
 * describe : this function is use to close pcm handle 
 * paras    :
 *        pcm : pcm handle to close  
 * return    :
 */
int quec_pcm_close(struct pcm *pcm );

/*
 * describe : this function is use to read pcm buffer 
 * paras    :
 *        pcm : pcm handle to write date 
 *        buffer: data buffer 
 *        lenth: data length
 * return    :
 */
int quec_read_pcm(struct pcm *pcm, void * buffer, int length);

/*
 * describe : this function is use to get pcm buffer lenth 
 * paras    :
 *        lenth: data length
 * return
 *        buffer length     
 */
int quec_get_pem_buffer_len(struct pcm *pcm);

//add by grady, 2018-6-2
/*
 * describe : this function is use to open mixer device 
 * paras        :
 *              device: mixer device
 * return
 *              mixer handle    
 */
struct mixer *quec_mixer_open(const char *device);

/*
 * describe : this function is use to close mixer device
 * paras        :
 *              mixer: mixer handle
 * return
 *              none    
 */
void quec_mixer_close(struct mixer *mixer);

/*
 * describe : this function is use to get mixer devie control 
 * paras        :
 *              mixer: mixer handle
 *              name: mixer device
 *              index: mixer index
 * return
 *              mixer control   
 */
struct mixer_ctl *quec_mixer_get_control(struct mixer *mixer, const char *name, unsigned index);

/*
 * describe : this function is use to set mulvalues
 * paras        :
 *              mixer: mixer handle
 *              count: count
 *              argv: data
 * return       :
 *                      
 */
int quec_mixer_ctl_mulvalues(struct mixer_ctl *ctl, int count, char ** argv);


//end grady

/*****************************************************************
* Function:     Ql_AudPlayer_OpenExt
*
* Description:
*               expend function from Ql_AudPlayer_OpenExt
*               Open audio play device, and specify the callback function.
*               This function can be called twice to play different audio sources.
*
* Parameters:
*               device  : a string that specifies the PCM device.
*                         NULL, means the audio will be played on the default PCM device.
*
*                         If you want to mixedly play audio sources, you can call this
*                         API twice with specifying different PCM device.
*                         The string devices available:
*                            "hw:0,0"  (the default play device)
*                            "hw:0,13" (this device can mix audio and TTS)
*                            "hw:0,14"
*
*		        cb_func : callback function for audio player. 
*                         The results of all operations on audio player
*                         are informed in callback function.
*
*               flags   : pcm flags, eg: PCM_MMAP, PCM_NMMAP.
*
*               channels: pcm sample channels.
*               
*               rate    : pcm sample rate.
*               
*               format  : pcm sample fromat
*
* Return:
*               pcm device handle
*               NULL, fail
*****************************************************************/
int Ql_AudPlayer_OpenExt(
            char *dev,
            _cb_onPlayer cb_fun,
            int flags,
            int channels,
            int rate,
            int format);

/*****************************************************************
* Function:     Ql_AudRecorder_Open
*
* Description:
*               Open audio record device, and specify the callback function.
*
* Parameters:
*               device  : not used. MUST be NULL.
*
*		        cb_func : callback function for audio player.
*                         The results of all operations on audio recorder
*                         are informed in callback function.
*
*               flags   : pcm flags, eg: PCM_MMAP, PCM_NMMAP.
*
*               channels: pcm sample channels.
*               
*               rate    : pcm sample rate.
*               
*               format  : pcm sample fromat
*
* Return:
*               pcm device handle
*               NULL, fail
*****************************************************************/
int Ql_AudRecorder_OpenExt(
            char *dev,
            _cb_onRecorder cb_fun,
            int flags,
            int channels,
            int rate,
            int format);

/*
* Function:     uac enable
* 
* Description:
*               uac enable
*
* Parameters:
*               none
* Return:        
*               TURE or FALSE
*/
int ql_uac_enable(void);

/*
* Function:     uac disable
* 
* Description:
*               uac disable
*
* Parameters:
*               none
* Return:        
*               TURE or FALSE
*/
int ql_uac_disable(void);


#endif //__AUD_H__
