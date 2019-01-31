/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Quectel Co., Ltd. 2017
*
*****************************************************************************/
/*****************************************************************************
 *
 * Filename:
 * ---------
 *   ql_audio.h 
 *
 * Project:
 * --------
 *   OpenLinux
 *
 * Description:
 * ------------
 *   Audio API definition.
 *
 * Author:
 * -------
 * Jun.wu
 * -------
 *
 *============================================================================
 *             HISTORY
 *----------------------------------------------------------------------------
 * WHO                  WHEN              WHAT
 *----------------------------------------------------------------------------
 * Jun.wu               18/07/2016		  Create
 * Running.qian         13/12/2016        Implement newly-designed APIs.
 * Stanley.yong         19/12/2016        Optimize the APIs, and add comments.
 * Zichar.zhang         19/04/2018        Add Ql_Update_wav_size function.
 *                                        Add Ql_AudPlayer_OpenExt function.
 *                                        Add Ql_AudRecorder_OpenExt function.
 * Zichar.zhang         31/05/2018        Add codec volume functions.
 * Zichar.zhang         31/05/2018        Add some new functions.
 * Zichar.zhang         31/05/2018        New library function and doxygen comments.
 ****************************************************************************/
#ifndef __AUD_H__
#define __AUD_H__


//this two headers for alsa params setting
#include "sound/asound.h"
#include "alsa-intf/alsa_audio.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef boolean
#define boolean unsigned int
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define ID_RIFF 0x46464952
#define ID_WAVE 0x45564157
#define ID_FMT  0x20746d66
#define ID_DATA 0x61746164
#define FORMAT_PCM 1

/**
 * @brief   Player state in user callback
 */
typedef enum {
    AUD_PLAYER_ERROR = -1,
    AUD_PLAYER_START = 0,
    AUD_PLAYER_PAUSE,
    AUD_PLAYER_RESUME,      ///<not used
    AUD_PLAYER_NODATA,      ///<not used
    AUD_PLAYER_LESSDATA,    ///<not used
    AUD_PLAYER_FINISHED,
} Enum_AudPlayer_State;

/**
 * @brief   Recorder state in user callback
 */
typedef enum {
    AUD_RECORDER_ERROR = -1,
    AUD_RECORDER_START = 0,
    AUD_RECORDER_PAUSE,     ///<not used
    AUD_RECORDER_RESUME,    ///<not used
    AUD_RECORDER_FINISHED,
} Enum_AudRecorder_State;

/**
 * @brief   Audio Format
 */
typedef enum {
    AUD_STREAM_FORMAT_PCM = 0,
    AUD_STREAM_FORMAT_MP3 = 1,
    AUD_STREAM_FORMAT_AMR = 2,
    AUD_STREAM_FORMAT_AMRNB,
    AUD_STREAM_FORMAT_AMRWB,
    AUD_STREAM_FORMAT_END
} Enum_AudStreamFormat;

/**
 * @brief   Audio Direct (not used now)
 */
typedef enum {
    AUD_LINK_REVERSE = 0,
    AUD_LINK_FORWARD = 1,
    AUD_LINK_BOTH = 2,
    AUD_LINK_INVALID
}Enum_AudStreamDirection;

/**
 * @brief   Audio codec volume type 
 * @details Used as parameter 'type' in function
 *          Ql_AudCodec_SetVolume(int type, int left, int right);
 */
typedef enum {
    CODEC_DOWN_VOL = 0, ///< codec headphone and spkear
    CODEC_UP_VOL,       ///< codec mic
    CODEC_HP_VOL,       ///< not support now
    CODEC_SPK_VOL,      ///< not support now
    CODEC_HP_MIC_VOL,   ///< not support now
    CODEC_MIC_VOL,      ///< not support now
} CODEC_VOLUME_TYPE;

/**
 * @brief   Audio Voice volume type 
 * @details Used as parameter 'type' in function
 *          Ql_AudVoice_SetVolume(int type, int vol);
 */
typedef enum {
    VOICE_RX_VOL = 0,   ///<can only be effective before voice call
    VOICE_TX_VOL,       ///<can only be effective before voice call
    VOICE_RX_MUTE,      ///<can only be effective in voice call
    VOICE_TX_MUTE,      ///<can only be effective in voice call
    VOICE_RX_LEVEL,     ///<recommended way to set volume in voice call
} VOICE_VOLUME_TYPE;

/**
 * @brief   Audio path mask
 * @details Used as parameter 'if_mask' in functions \n
 *          Ql_AudPlayer_SetInterface(char *dev, unsigned int if_mask); \n
 *          Ql_AudPlayer_GetInterface(char *dev, unsigned int *if_mask); \n
 *          Ql_AudRecorder_SetInterface(char *dev, unsigned int if_mask); \n
 *          Ql_AudRecorder_GetInterface(char *dev, unsigned int *if_mask); \n
 */
typedef enum {
    CLOSE_IF         = 0x0000,
    PCM_OUT_IF       = 0x0001,
    I2S_OUT_IF       = 0x0002,
    VOICE_OUT_IF     = 0x0004,
    
    PCM_IN_IF        = 0x0010,
    I2S_IN_IF        = 0x0020,
    VOICE_INUP_IF    = 0x0040, ///< VOICE you are hearing
    VOICE_INDOWN_IF  = 0x0080, ///< VOICE you are talking
} AUDIO_PATH_MASK;

/**
 * @brief wav file header
 */
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

typedef enum {
    AUD_VOLUME_LEVEL1 = 0,
    AUD_VOLUME_LEVEL2,
    AUD_VOLUME_LEVEL3,
    AUD_VOLUME_LEVEL4,
    AUD_VOLUME_LEVEL5,
    AUD_VOLUME_LEVEL_END
}Enum_AudVolumeLevel;

/**
 * @brief just for compitable with old eaxmple
 */
struct ST_MediaParams {
    Enum_AudStreamFormat     format;
    Enum_AudVolumeLevel      volume;
    Enum_AudStreamDirection     direct;
};

/**
 * @brief This callback function handles the result of audio player.
 *
 * @param hdl    Player handle, received from Ql_AudPlayer_Open().
 * @param result Reference to Enum_AudPlayer_State.
 *
 * @return Not care.
 */
typedef int(*_cb_onPlayer)(int hdl, int result);

/**
 * @brief This callback function handles the result of audio recorder.
 *
 * @param result
 *      Reference to Enum_AudRecorder_State.
 * @param pBuf
 *      Data buffer.
 * @param length
 *      Data length.
 *
 * @return
 *      Not care
 */
typedef int(*_cb_onRecorder)(int result, unsigned char* pBuf, unsigned int length);

/**
 * @brief Open player device.
 * @details
 *      Just call function Ql_AudPlayer_OpenExt(device, cb_func,
 *                               PCM_NMMAP, 1, 8000, SNDRV_PCM_FORMAT_S16_LE); \n
 *      Set default parameters MONO, 8khz samplerate, 16bit little endian.
 *
 * @param device
 *      Pcm device, eg: "hw:0,0"
 * @param cb_func
 *      User callback functions.
 *
 * @return 
 *      Succeed: player device handle. \n
 *      Fail:    -1.
 */
int  Ql_AudPlayer_Open(char* device, _cb_onPlayer cb_func);

/**
 * @brief Open player device.
 * @details
 *      open audio play device, specify the callback function, and set pcm format.
 *
 * @param dev
 *      A string that specifies the PCM device. \n
 *      NULL, means the audio will be played on the default PCM device. \n
 *      \n
 *      If you want to mixedly play audio sources, you can call this
 *      API twice with specifying different PCM device. \n
 *      The string devices available: \n
 *         "hw:0,0"  (the default play device) \n
 *         "hw:0,13" (this device can mix audio and TTS) \n
 * @param cb_fun
 *      Callback function for audio player. \n
 *      The results of all operations on audio player
 *      are informed in callback function.
 * @param flags
 *      Pcm flags, eg: PCM_MMAP, PCM_NMMAP.
 * @param channels
 *      Pcm sample channels.
 * @param rate
 *      Pcm sample rate.
 * @param format
 *      pcm sample rate. (only support SNDRV_PCM_FORMAT_S16_LE)
 *
 * @return
 *      Succeed: player device handle. \n
 *      Fail:    -1.
 */
int Ql_AudPlayer_OpenExt(
            char *dev,
            _cb_onPlayer cb_fun,
            int flags,
            int channels,
            int rate,
            int format);

/**
 * @brief Writes pcm data to pcm device to play.
 * @details
 *      If player thread is not started, this function will start play
 *      thread first.
 *
 * @param dev
 *      A string that specifies the PCM device. \n
 *      NULL, means the audio will be played on the default PCM device. \n
 *      \n
 *      If you want to mixedly play audio sources, you can call this
 *      API twice with specifying different PCM device. \n
 *      The string devices available: \n
 *         "hw:0,0"  (the default play device) \n
 *         "hw:0,13" (this device can mix audio and TTS) \n
 * @param cb_fun
 *      Callback function for audio player. \n
 *      The results of all operations on audio player
 *      are informed in callback function.
 * @param hdl
 *      Device handle
 * @param pData
 *      Pcm data
 * @param length
 *      Pcm data length
 *
 * @return
 *      Succeed: length has written. \n
 *      Fail: -1.
 */
int  Ql_AudPlayer_Play(int hdl, unsigned char* pData, unsigned int length);

/**
 * @brief Play pcm data from the specified file.
 * @details
 *      This functions just for compatibility with old example in example_audio.c. \n
 *      The player device was opened outside this function and pcm parameters (rate,
 *      channels, format) was already set which may be different with the audio file. \n
 *      eg: the player device is opened and set sample rate 8000 hz, 1 channel. But the 
 *          audio file test.wav is sample by 16000 hz, 1channel. Then the wav file will
 *          be played and extended.
 *      \n
 *      You are supposed to use this function when you want play the file with the
 *      special pcm format you want. Otherwise please use Ql_AudPlayer_PlayFile instead.
 *
 *
 * @param hdl
 *      Device handle.
 * @param fd
 *      Linux file discriper.
 * @param offset
 *      The position start to play in the file.
 *
 * @return
 *      Succeed: 0. \n 
 *      Fail:    -1.
 */
int  Ql_AudPlayer_PlayFrmFile(int hdl, int fd, int offset);

/**
 * @brief Play pcm data from the specified file.
 * @details
 *      This function is used to play audio file. The wav, amr(amrnb/amrwb), mp3
 *      formats are supported. \n
 *      This function will start three threads to perform the audio file playing
 *      progress. \n
 *      If you don't care about when the playing is end, you can go to do other
 *      things(let it go), it will end the playing with stop and close operations
 *      itself. \n
 *      But if you want cut the playing off manually, you can use
 *      Ql_AudPlayer_Stop(hdl) to stop it and use Ql_audPlayer_Close(hdl) to
 *      block till it end.
 *
 * @param dev
 *      A string that specifies the PCM device. \n
 *      NULL, means the audio will be played on the default PCM device. \n
 *      \n
 *      If you want to mixedly play audio sources, you can call this
 *      API twice with specifying different PCM device. \n
 *      The string devices available: \n
 *         "hw:0,0"  (the default play device) \n
 *         "hw:0,13" (this device can mix audio and TTS) \n
 * @param cb_fun
 *      Callback function for audio player. \n
 *      The results of all operations on audio player
 *      are informed in callback function.
 * @param hdl
 *      Device handle.
 * @param fd
 *      Linux file discriper.
 * @param offset
 *      The position start to play in the file.
 *
 * @return
 *      Succeed: player device handle. \n
 *      Fail:    -1.
 */
int Ql_AudPlayer_PlayFile(char *dev, int fd, int offset, _cb_onPlayer cb_fun);

/**
 * @brief Set player interface (pcm, i2s, voice)
 *
 * @param dev
 *      Player device ("hw:0,0", "hw:0,13")
 * @param if_mask
 *      interface mask, reference to @ref AUDIO_PATH_MASK
 *
 * @return 
 *      Succeed: 0. \n 
 *      Fail:    -1.
 */
int Ql_AudPlayer_SetInterface(char *dev, unsigned int if_mask);

/**
 * @brief Get player interface (pcm, i2s, voice)
 *
 * @param dev
 *      player device ("hw:0,0", "hw:0,13")
 * @param if_mask
 *      Interface mask, reference to @ref AUDIO_PATH_MASK
 *
 * @return 
 *      Succeed: 0. \n 
 *      Fail:    -1.
 */
int Ql_AudPlayer_GetInterface(char *dev, unsigned int *if_mask);

/**
 * @brief Set Player buffee size
 * @details
 *      Count in millisecond according to sample rate. \n
 *      This function may be used in some situation that input data flow \n
 *      is not stable. For example the network stream.
 *
 * @param dev
 *      Player device ("hw:0,0", "hw:0,13")
 * @param buf_size_ms
 *      Buffer size, count in millisecond according to sample rate.
 *      Default buffer size is 80ms.
 *
 * @return 
 */
int Ql_AudPlayer_SetBufsize_ms(char *dev, int buf_size_ms);

/**
 * @brief Get Player state 
 *
 * @param hdl
 *      device handle
 * @param playState
 *      Player state pointer \n
 *      Referrence to  Enum_AudPlayer_State
 *
 * @return 
 *      Player state string.
 */
const char* Ql_AudPlayer_GetState(int hdl, int *playState);

/**
 * @brief Pause playing
 *
 * @param hdl
 *      Device handle.
 *
 * @return 
 *      Succeed: 0. \n 
 *      Fail:    -1.
 */
int  Ql_AudPlayer_Pause(int hdl);

/**
 * @brief Resume from pause state
 *
 * @param hdl
 *      Device handle.
 *
 * @return 
 *      Succeed: 0. \n 
 *      Fail:    -1.
 */
int  Ql_AudPlayer_Resume(int hdl);

/**
 * @brief Stop playing
 * @details
 *      Just set stop flag, and playing progress will end itself.
 *
 * @param hdl
 *      Device handle.
 */
void Ql_AudPlayer_Stop(int hdl);

/**
 * @brief  Close player device.
 * @details
 *      This functions will close player device if it was opened. \n
 *      If the playing progress is still on, this function will call
 *      Ql_AudPlayer_Stop(hdl) first and block to wait playing progress
 *      end.
 *
 * @param hdl
 *      Device handle.
 */
void Ql_AudPlayer_Close(int hdl);

/**
 * @brief Open recorder device.
 * @details
 *      Just call function Ql_AudRecorder_OpenExt(device, cb_func,
 *                               PCM_NMMAP, 1, 8000, SNDRV_PCM_FORMAT_S16_LE); \n
 *      Set default parameters MONO, 8khz samplerate, 16bit little endian.
 *
 * @param device
 *      Pcm device, eg: "hw:0,0"
 * @param cb_func
 *      User callback functions.
 *
 * @return 
 *      Succeed: recorder device handle. \n
 *      Fail:    -1.
 */
int  Ql_AudRecorder_Open(char* device, _cb_onRecorder cb_fun);

/**
 * @brief Open recorder device.
 * @details
 *      open audio recorder device, specify the callback function, and set pcm format.
 *
 * @param dev
 *      A string that specifies the PCM device. \n
 *      NULL, means the audio will be played on the default PCM device. \n
 *      \n
 *      If you want to mixedly play audio sources, you can call this
 *      API twice with specifying different PCM device. \n
 *      The string devices available: \n
 *         "hw:0,0"  (the default play device) \n
 *         "hw:0,13" (this device can mix audio and TTS) \n
 * @param cb_fun
 *      Callback function for audio player. \n
 *      The results of all operations on audio recorder
 *      are informed in callback function.
 * @param flags
 *      Pcm flags, eg: PCM_MMAP, PCM_NMMAP.
 * @param channels
 *      Pcm sample channels.
 * @param rate
 *      Pcm sample rate.
 * @param format
 *      pcm sample rate. (only support SNDRV_PCM_FORMAT_S16_LE)
 *
 * @return
 *      Succeed: recorder device handle. \n
 *      Fail:    -1.
 */
int Ql_AudRecorder_OpenExt(
            char *dev,
            _cb_onRecorder cb_fun,
            int flags,
            int channels,
            int rate,
            int format);

/**
 * @brief Start to record
 * @details
 *      Just call Ql_AudRecorder_HStartRecord(0);
 *
 * @return 
 *      Succeed: recorder device handle. \n
 *      Fail:    -1.
 */
int Ql_AudRecorder_StartRecord(void);

/**
 * @brief Start to record
 * @details This function called after Ql_AudRecorder_Open or
 *          Ql_AudRecorder_OpenExt, and start to record. \n
 *          The record data is output in _cb_onRecorder set in 
 *          Ql_AudRecorder_Open or Ql_AudRecorder_Open.
 *
 * @param hdl
 *      Device handle.
 *
 * @return 
 *      Succeed: recorder device handle. \n
 *      Fail:    -1.
 */
int Ql_AudRecorder_HStartRecord(int hdl);

/**
 * @brief Start to record
 * @details
 *      This function called after Ql_AudRecorder_Open or
 *      Ql_AudRecorder_OpenExt, and start to record \n
 *      The record data is output in _cb_onRecorder, witch set in 
 *      Ql_AudRecorder_Open or Ql_AudRecorder_Open.
 *
 * @param hdl
 *      Device handle.
 *
 * @return 
 *      Succeed: data length that successfully get from recorder device. \n
 *      Fail:    -1.
 */
int Ql_AudRecorder_Record(int hdl, char *buf, int len);

/**
 * @brief Record in wav format and saved into file.
 * @details
 *      Start one thread to record and save into file in wav format. \n
 *      Using Ql_AudRecorder_Stop to stop recorder progress, and using
 *      Ql_AudRecorder_Close to close recorder device.
 *
 * @param dev
 *      Pcm device, eg: "hw:0,0"
 * @param fname
 *      Saved file path.
 * @param channels
 *      Pcm sample channels.
 * @param rate
 *      Pcm sample rate.
 * @param format
 *      pcm sample rate. (only support SNDRV_PCM_FORMAT_S16_LE)
 *
 * @return 
 *      Succeed: device handle. \n
 *      Fail:    -1.
 */
int Ql_AudRecorder_RecordWAV(char *dev, char *fname,
                                int channels, int rate, int format);
/**
 * @brief Record in amrnb format and saved into file.
 * @details
 *      Start one thread to record and save into file in amrnb format. \n
 *      Using Ql_AudRecorder_Stop to stop recorder progress, and using
 *      Ql_AudRecorder_Close to close recorder device.
 *
 * @param dev
 *      Pcm device, eg: "hw:0,0"
 * @param fname
 *      Saved file path.
 * @param channels
 *      Pcm sample channels.
 * @param rate
 *      Pcm sample rate.
 * @param format
 *      pcm sample rate. (only support SNDRV_PCM_FORMAT_S16_LE)
 *
 * @return 
 *      Succeed: device handle. \n
 *      Fail:    -1.
 */
int Ql_AudRecorder_RecordAMRNB(char *dev, char *fname);

/**
 * @brief Pause recording (not support now)
 * @details
 *      Just call Ql_AudRecorder_HPause(0).
 *
 * @return 
 *      Succeed: 0. \n
 *      Fail:    -1.
 */
int Ql_AudRecorder_Pause(void);

/**
 * @brief Pause recording (not support now)
 * @details
 *
 * @return 
 *      Succeed: 0. \n
 *      Fail:    -1.
 */
int Ql_AudRecorder_HPause(int hdl);

/**
 * @brief Resume recording (not support now)
 * @details
 *      Just call Ql_AudRecorder_HResume(0).
 *
 * @return 
 *      Succeed: 0. \n
 *      Fail:    -1.
 */
int Ql_AudRecorder_Resume(void);

/**
 * @brief Pause recording (not support now)
 * @details
 *
 * @return 
 *      Succeed: 0. \n
 *      Fail:    -1.
 */
int Ql_AudRecorder_HResume(int hdl);

/**
 * @brief Stop recording
 * @details
 *      Just call Ql_AudRecorder_Stop(0).
 *
 */
void Ql_AudRecorder_Stop(void);

/**
 * @brief Stop recording
 * @details
 *      Just set stop flag, and recording progress will end itself.
 *
 * @param hdl
 *      Device handle.
 */
void Ql_AudRecorder_HStop(int hdl);

/**
 * @brief Close recorder device, and free the resources.
 * @details
 *      Just call Ql_AudRecorder_Close(0).
 */
void Ql_AudRecorder_Close(void);

/**
 * @brief Close recorder device, and free the resources.
 * @details
 *      This functions will close recorder device if it was opened. \n
 *      If the recording progress is still on, this function will call
 *      Ql_AudRecorder_Stop(hdl) first and block to wait recording progress
 *      end.
 *
 * @param hdl
 *      Device handle.
 */
void Ql_AudRecorder_HClose(int hdl);

/**
 * @brief Get Recorder state 
 * @details
 *      Just call Ql_AudRecorder_GetState(capState).
 *
 * @param capState
 *      Recorder state pointer
 *
 * @return 
 *      Recorder state string. \n
 *      Referrence to Enum_AudRecorder_State
 */
const char* Ql_AudRecorder_GetState(int *capState);

/**
 * @brief Get Recorder state 
 *
 * @param hdl
 *      device handle
 * @param capState
 *      Recorder state pointer
 *
 * @return 
 *      Recorder state string. \n
 *      Referrence to Enum_AudRecorder_State
 */
const char* Ql_AudRecorder_HGetState(int hdl, int *capState);

/**
 * @brief Set alsa controls
 * @details
 *      eg: \n
 *      set codec 5616 main volume : \n
 *      Ql_ctl_set_mixer_value("DAC1 Playback Volume", 2, "175 175");
 *
 * @param device
 *      alsa controls' string.
 * @param count
 *      alsa controls' parameter numbers
 * @param value
 *      alsa contorls' parameter string
 *
 * @return 
 *      Succeed: TRUE. \n
 *      Fail:    FALSE.
 * @code{c}
 *      //Set one para opration:
 *      Ql_clt_set_mixer_value("SEC_MI2S_RX Audio Mixer MultiMedia1", 1, "1");
 *      
 *      //Set two para opration:
 *      Ql_clt_set_mixer_value("DAC1 Playback Volume", 2, "175 175");
 *      
 *      //Set three para opration:
 *      Ql_clt_set_mixer_value("Voice Rx Gain", 3, "0 -1 500");
 * @endcode
 */
boolean Ql_clt_set_mixer_value(const char *device, int count, const char *value);

/**
 * @brief Get alsa controls
 *
 * @param device
 *      alsa controls' string.
 *
 * @return 
 *      Succeed: parameter numbers. \n
 *      Fail:    -1.
 * @code{c}
 *      //Get codec alc5616 main volume :
 *      ret = Ql_ctl_Get_mixer_value("DAC1 Playback Volume", &ev);
 *      //ret must be 2. \n
 *      //ev->value.integer.value[0] is left channel volume for alc5616 main volume.
 *      //ev->value.integer.value[1] is right channel volume for alc5616 main volume.
 * @endcode
 */
int Ql_clt_get_mixer_value(const char *device, struct snd_ctl_elem_value *ev);

/**
 * @brief Set codec volume
 * @details
 *      Support codecs alc5616, tlv320, nau8810, nau8814, max9867
 *      If codec only support one channel, setting parameter left as volume.(ignore right)
 *
 * @param type
 *      referrence to CODEC_VOLUME_TYPE
 * @param left
 *      codec left channel volume, range 0-100
 * @param right
 *      codec right channel volume, range 0-100
 *
 * @return 
 *      Succeed: 0. \n
 *      Fail:    -1.
 */
int Ql_AudCodec_SetVolume(int type, int left, int right);

/**
 * @brief Get codec volume 
 * @details
 *      Support codecs alc5616, tlv320, nau8810, nau8814, max9867
 *
 * @param type
 *      referrence to CODEC_VOLUME_TYPE
 * @param left
 *      codec left channel volume
 * @param right
 *      codec right channel volume
 *
 * @return 
 *      Succeed: 0. \n
 *      Fail:    -1.
 */
int Ql_AudCodec_GetVolume(int type, int *left, int *right);

/**
 * @brief Set software volume
 *
 * @param hdl
 *      Device handle
 * @param volume
 *      Software volume, range 0-100
 *
 * @return 
 *      Succeed: 0. \n
 *      Fail:    -1.
 */
int Ql_AudSoft_SetVolume(int hdl, int volume);

/**
 * @brief Set volume in voice call.
 *
 * @param type
 *      Voice volume type, reference to @ref VOICE_VOLUME_TYPE
 * @param vol
 *      Voice volume
 *
 * @return 
 *      Succeed: 0. \n
 *      Fail:    -1.
 * @code{c}
 *      Ql_AudVoice_SetVolume(VOICE_RX_LEVEL, val);  //only support Rx line val range 0-5
 *      Ql_AudVoice_SetVolume(VOICE_RX_MUTE, 1);     //Rx mute, effective in voice call
 *      Ql_AudVoice_SetVolume(VOICE_RX_MUTE, 0);     //Rx unmute, effective in voice call
 *      Ql_AudVoice_SetVolume(VOICE_TX_MUTE, 1);     //Tx mute, effective in voice call
 *      Ql_AudVoice_SetVolume(VOICE_TX_MUTE, 0);     //Tx unmute, effective in voice call
 *      Ql_AudVoice_SetVolume(VOICE_RX_VOL, val);    //only support Rx line, effective before voice call
 *      Ql_AudVoice_SetVolume(VOICE_TX_VOL, val);    //only support Rx line, effective before voice call
 * @endcode
 */
int Ql_AudVoice_SetVolume(int type, int vol);


/**
 * @brief Set Recorder interface (pcm, i2s, voice)
 *
 * @param dev
 *      Recorder device ("hw:0,0", "hw:0,13")
 * @param if_mask
 *      interface mask, reference to @ref AUDIO_PATH_MASK
 *
 * @return 
 *      Succeed: 0. \n 
 *      Fail:    -1.
 */
int Ql_AudRecorder_SetInterface(char *dev, unsigned int if_mask);

/**
 * @brief Get Recorder interface (pcm, i2s, voice)
 *
 * @param dev
 *      Recorder device ("hw:0,0", "hw:0,13")
 * @param if_mask
 *      Interface mask, reference to @ref AUDIO_PATH_MASK
 *
 * @return 
 *      Succeed: 0. \n 
 *      Fail:    -1.
 */
int Ql_AudRecorder_GetInterface(char *dev, unsigned int *if_mask);

//****************QL TONE API************************//
/**
 * @brief 
 *
 * @param lowFreq
 *      dtmf low frequency, 100-4000HZ
 * @param highFreq
 *      dtmf high frequency, 100-4000HZ
 * @param volume
 *      play volume, 0-1000
 * @param duration
 *      play time, >0 ms
 */
struct Ql_TonePara {
    unsigned int lowFreq;   //100-4000HZ
    unsigned int highFreq;  //100-4000HZ
    unsigned int volume;    //0 -1000
    unsigned int duration;  // >0 ms
};

/**
 * @brief Open tone(dtmf) device
 *
 * @param device
 *      Tone device ("hw:0,7")
 * @param cb
 *      Not support now
 *
 * @return 
 *      Succeed: 0. \n 
 *      Fail:    -1.
 */
int Ql_AudTone_Open(char* device, _cb_onPlayer cb);//cb not support now

/**
 * @brief Start tone(dtmf) playing 
 *
 * @param hdl
 *      Device handle
 * @param para
 *      Tone parameter, referrence to struc Ql_TonePara
 *
 * @return 
 *      Succeed: 0. \n 
 *      Fail:    -1.
 */
int Ql_AudTone_Start(int hdl, struct Ql_TonePara *para);

/**
 * @brief Stop tone(dtmf) playing
 *
 * @param hdl
 *      Device handle
 */
void Ql_AudTone_Stop(int hdl);

/**
 * @brief Close tone(dtmf) device 
 *
 * @param hdl
 *      Device handle
 */
void Ql_AudTone_Close(int hdl);

/**
 * @brief Get library version string 
 *
 * @return 
 *      library version string
 */
char* Ql_AudVersion_Get(void);

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


//int Ql_AudPlayer_set_LessDataThreshold(int hdl, unsigned short threshSize);

//int Ql_AudPlayer_get_freeSpace(int hdl);

#ifdef __cplusplus
} //end extern C
#endif

#endif //__AUD_H__
