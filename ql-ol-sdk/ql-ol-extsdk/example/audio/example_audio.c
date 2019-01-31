#include "ql_oe.h"


#define quec_log(...) fprintf(stderr, __VA_ARGS__)
#define QLLOG(...)    fprintf(stderr, __VA_ARGS__)

//#define TEST_CODEC_5616

static int __ql_wav_pcm16Le_check(int fd)
{
    struct wav_header hdr;
    int offset;

    if (read(fd, &hdr, sizeof(hdr)) != sizeof(hdr)) {
        QLLOG("%s: cannot read header\n", __FUNCTION__);
        return -1;
    }

    QLLOG("read wav hdr\n");
    if ((hdr.riff_id != ID_RIFF)
        || (hdr.riff_fmt != ID_WAVE)
        || (hdr.fmt_id != ID_FMT)) {
        QLLOG("%s:  is not a riff/wave file\n", __FUNCTION__);
        return -1;
    }
        
    if ((hdr.audio_format != FORMAT_PCM) || (hdr.fmt_sz != 16)) {
        QLLOG("%s: is not pcm format\n", __FUNCTION__);
        return -1;
    }
        
    if (hdr.bits_per_sample != 16) {
        QLLOG("%s: is not 16bit per sample\n", __FUNCTION__);
        return -1;
    }

    offset = lseek(fd, 0, SEEK_CUR);
    QLLOG("get wav hdr offset\n");
    return offset;
}


static int __ql_wav_amr_check(int fd)
{
    return 0;
}


static int __ql_wav_pcm16Le_set(int fd)
{
    struct wav_header hdr;

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
    hdr.block_align = (hdr.bits_per_sample * 1) / 8;
    hdr.data_id = ID_DATA;
    hdr.data_sz = 0;

    hdr.riff_sz = hdr.data_sz + 44 - 8;
    if (write(fd, &hdr, sizeof(hdr)) != sizeof(hdr)) {
        return -1;
    }

    return 0;
}


static int __ql_wav_amr_set(int fd)
{
    return 0;
}

#define ID_AMR  0x4D412123  // #!AMR
#define AMR2WAV_TMP_FILE_NAME   "/var/amr2wav.wav"
static int ql_open_and_convert_file(char *filename)
{
    int fd, retval;
    struct wav_header hdr;

    if(!filename)
    {
        return -EINVAL;
    }

    fd = open(filename, O_RDONLY);
    if (fd < 0)
    {
        fprintf(stderr, "%s: cannot open '%s'\n", __FUNCTION__, filename);
        return fd;
    }

    if (read(fd, &hdr, sizeof(hdr)) != sizeof(hdr))
    {
        fprintf(stderr, "%s: cannot read header\n", __FUNCTION__);
        return -errno;
    }
    lseek(fd, 0, SEEK_SET);

    if(hdr.riff_id == ID_AMR) // #!AMR\n
    {
#if 0 //You need make sure the audio lib contains this API and enable it or it will build error.
        close(fd);
        retval = DecodeAMRFileToWAVEFile(filename, AMR2WAV_TMP_FILE_NAME);//retval: frame count
        if(retval > 0)
        {
            fd = open(AMR2WAV_TMP_FILE_NAME, O_RDONLY);
            if (fd < 0)
            {
                fprintf(stderr, "%s: cannot open '%s'\n", __FUNCTION__, filename);
            }
        }
#endif
    }

    return fd;
}


/**
 * param '*offset' value is the first wav data position
 * @Note if offset == NULL, then 'offset' should be -1 when call Ql_AudioPlayFileStart()
 */
int Ql_AudFileOpen(char *file, struct ST_MediaParams *mediaParams, int *offset)
{
    int fd;
    int ret;

    if (file == NULL && mediaParams == NULL) 
    {
        QLLOG("%s: args invalid\n", __FUNCTION__);
        goto openfile_failed;
    }

    fd = ql_open_and_convert_file(file);
    if (fd < 0) {
        QLLOG("%s: open file failed\n", __FUNCTION__);
        goto openfile_failed;
    }

    /* check media params */
    switch (mediaParams->format) {
    case AUD_STREAM_FORMAT_AMR:
        __ql_wav_amr_check(fd);
        break;

    case AUD_STREAM_FORMAT_PCM:
        ret = __ql_wav_pcm16Le_check(fd);
        if (ret < 0) {
            QLLOG("parseMediafilePcm16Le failed\n");
            goto openfile_failed;
        }
        if (offset != NULL) {
            *offset = ret;
        }
        break;

    default:
        QLLOG("parse Unknown File\n");
        goto openfile_failed;
    }

    return fd;
openfile_failed:
    return -1;
}


int Ql_PcmFileCreat(char *file, struct ST_MediaParams *mediaParams, int *offset)
{
    int fd = -1;
    int ret = -1;

    if(access(file, 0) == 0)    {
        QLLOG("%s: file already exist.\n", __FUNCTION__);
        return -1;
    }

    fd = open(file, O_RDWR | O_CREAT, 0x0666);
    if (fd < 0) {
        QLLOG("%s: open file failed\n", __FUNCTION__);
        return -1;
    }

    switch (mediaParams->format) {
    case AUD_STREAM_FORMAT_AMR:
        ret = __ql_wav_amr_set(fd);
        break;

    case AUD_STREAM_FORMAT_PCM:
        ret = __ql_wav_pcm16Le_set(fd);
        break;

    default:
        ret = -1;
        break;
    }

    if (ret < 0) {
        close(fd);
        return -1;
    }

    return fd;
}


static int recdfd;//save record data file fd

static int play1Flag = 0;
static int play2Flag = 0;
static int recordFlag = 0;
static unsigned char pcmBuf[5 * 8000 * 2]; //5s sample data

int Ql_cb_Player1(int hdl, int result)
{
    quec_log("%s: hdl=%d, result=%d\n\r", __func__, hdl, result);
    if (result == AUD_PLAYER_FINISHED || result == AUD_PLAYER_NODATA)
    {
        quec_log("%s: play finished\n\r", __func__);
        play1Flag = 1;
    }
    return 0;
}


int Ql_cb_Player2(int hdl, int result)
{
    quec_log("%s: hdl=%d, result=%d\n\r", __func__, hdl, result);
    if (result == AUD_PLAYER_FINISHED)
    {
        quec_log("%s: play finished\n\r", __func__);
        play2Flag = 1;
    }
    return 0;
}


int Ql_cb_Recd1(int result, unsigned char *pBuf, unsigned int len)
{
    static int pos = 0;

    switch (result) {
    case AUD_RECORDER_ERROR:
        quec_log("%s: error\n\r", __FUNCTION__);
        break;

    case AUD_RECORDER_START:
        if (pos + len > sizeof(pcmBuf)) {
            return -1;// end record
        }

        quec_log("%s: save rocord...\n\r", __FUNCTION__);
        memcpy(&pcmBuf[pos], pBuf, len);
        pos += len;
        return 0;//continue record

    case AUD_RECORDER_PAUSE:
        break;

    case AUD_RECORDER_RESUME:
        break;

    case AUD_RECORDER_FINISHED:
        quec_log("%s: record finished\n\r", __func__);
        recordFlag = 1;
        break;

    default:
        break;
    }

    return 0;
}


int Ql_cb_playback(int hdl, int result)
{
    quec_log("%s: hdl=%d, result=%d\n\r", __func__, hdl, result);
    if (result == AUD_PLAYER_FINISHED || result == AUD_PLAYER_NODATA)
    {
        quec_log("%s: play finished\n\r", __func__);
        play1Flag = 1;
    }
    return 0;
}


int Ql_cb_Recd2(int result, unsigned char *pBuf, unsigned int len)
{
    switch (result) {
    case AUD_RECORDER_ERROR:
        recordFlag = 1;
        quec_log("%s: error\n\r", __FUNCTION__);
        break;

    case AUD_RECORDER_START:
        quec_log("%s: write data...\n\r", __FUNCTION__);
        write(recdfd, pBuf, len);
        return 0;//must return 0 or len

    case AUD_RECORDER_PAUSE:
        break;

    case AUD_RECORDER_RESUME:
        break;

    case AUD_RECORDER_FINISHED:
        quec_log("%s: record finished\n\r", __func__);
        recordFlag = 1;
        break;

    default:
        break;
    }

    return 0;
}

static void print_usage(void)
{
    quec_log(
            "\n\r--Useage:\n\r"
            "play one file: ./<process> play1 <file>\n\r"
            "play two file: ./<process> play2 <file1> <file2>\n\r"
            "recd and play: ./<process> recd1\n\r"
            "recd and save: ./<process> recd2 <file>\n\r"
            "play     tone: ./<process> tone [<freq> <time> <volume>]\n\r"
            "alc5616 drcagc: ./<process> alc5616_drcagc ctrl1_mask ctrl1_val ctrl2_mask ctrl2_val ctrl3_mask ctrl3_val\n\r"
            );
}

//
// Usage:
//   Play the specified audio file: 
//        <process> play1 <audio file>
//   Mixer player:
//        <process> play2 <audio file1> <audio file2>
//   Record and playback (without saving record data):
//        <process> recd1
//   Record and playback (save record data to file):
//        <process> recd2 <file name>
//   change alc5616 DRC/AGC:
//        <process> alc5616_drcagc ctrl1_mask ctrl1_val ctrl2_mask ctrl2_val ctrl3_mask ctrl3_val
/////////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
    int iRet;
    int devphdl1;//paly1
    int devphdl2;//play2
    int filefd1;
    int filefd2;
    int recdTime; //seconds
    struct Ql_TonePara tonePara;

    struct ST_MediaParams mediaParas;

    unsigned int tmp32;
#ifdef TEST_CODEC_5616
    struct Ql_ALC5616_DRCAGC drcagc;
#endif
    if (argc < 2)
    {
        print_usage();
        return 0;
    }

    // Play the specified audio file
    // Command: <process> play1 <audio file>
    if (strcmp(argv[1], "play1") == 0) 
    {
        if (argc != 3) 
        {
            quec_log("< player needs more parameter\n\r");
            return 0;
        }

        mediaParas.format = AUD_STREAM_FORMAT_PCM;
        filefd1 = Ql_AudFileOpen(argv[2], &mediaParas, NULL);

        Ql_clt_set_mixer_value("SEC_AUX_PCM_RX Audio Mixer MultiMedia1", 1, "1");
      //  Ql_clt_set_mixer_value("MultiMedia1 Mixer SEC_AUX_PCM_UL_TX", 1, "1");
        
        devphdl1 = Ql_AudPlayer_Open(NULL, Ql_cb_Player1);
        if (devphdl1 < 0) {
            quec_log("open aud play%d failed\n\r", devphdl1);
            return 0;
        }
#ifdef TEST_CODEC_5616
        drcagc.control1_mask = 0xffff;
        drcagc.control1_value = 0x4326;
        drcagc.control2_mask = 0xffff;
        drcagc.control2_value = 0x1fe5;
        drcagc.control3_mask = 0xffff;
        drcagc.control3_value = 0x0580;
        if(Ql_AudCodec_Set_ALC5616_DRCAGC(NULL, &drcagc) < 0) {
            quec_log("Ql_AudCodec_Set_ALC5616_DRCAGC failed\n\r");
        } else {
            quec_log("Ql_AudCodec_Set_ALC5616_DRCAGC success\n\r");
        }
#endif
        play1Flag = 0;
        iRet = Ql_AudPlayer_PlayFrmFile(devphdl1, filefd1, -1);
        if (iRet != 0) {
            quec_log("start aud play%d failed\n\r", devphdl1);
        }

        while (play1Flag != 1) {
            sleep(1);
        }

        Ql_AudPlayer_Stop(devphdl1);
        Ql_AudPlayer_Close(devphdl1);
    }

    // Mixer player
    // Command: <process> play2 <audio file1> <audio file2>
    else if (strcmp(argv[1], "play2") == 0)    
    {
        if (argc != 4) {
            quec_log("< player needs more parameter\n\r");
            return 0;
        }
        mediaParas.format = AUD_STREAM_FORMAT_PCM;
        filefd1 = Ql_AudFileOpen(argv[2], &mediaParas, NULL);
        filefd2 = Ql_AudFileOpen(argv[3], &mediaParas, NULL);

        Ql_clt_set_mixer_value("SEC_AUX_PCM_RX Audio Mixer MultiMedia1", 1, "1");        
        //system("amix 'SEC_AUX_PCM_RX Audio Mixer MultiMedia2' 1");
        Ql_clt_set_mixer_value("SEC_AUX_PCM_RX Audio Mixer MultiMedia2", 1, "1");

        devphdl1 = Ql_AudPlayer_Open(NULL, Ql_cb_Player1);
        if (devphdl1 < 0) {
            quec_log("open aud play%d failed\n\r", devphdl1);
            return 0;
        }

        devphdl2 = Ql_AudPlayer_Open("hw:0,13", Ql_cb_Player2);
        if (devphdl2 < 0) {
            quec_log("open aud play%d failed\n\r", devphdl1);
            return 0;
        }

        play1Flag = 0;
        iRet = Ql_AudPlayer_PlayFrmFile(devphdl1, filefd1, -1);
        if (iRet != 0) {
            quec_log("start aud play%d failed\n\r", devphdl1);
        }

        sleep(1);
        play2Flag = 0;
        iRet = Ql_AudPlayer_PlayFrmFile(devphdl2, filefd2, -1);
        if (iRet != 0) {
            quec_log("start aud play%d failed\n\r", devphdl1);
        }
        
        
        while (play1Flag == 0 || play2Flag == 0) {
            sleep(1);
        }

        Ql_AudPlayer_Stop(devphdl1);
        Ql_AudPlayer_Stop(devphdl2);
        Ql_AudPlayer_Close(devphdl1);
        Ql_AudPlayer_Close(devphdl2);
    }
    else if (strcmp(argv[1], "recd1") == 0) 
    {//record and playback
        if (argc != 2) 
        {
            quec_log("< recorder needs more parameter\n\r");
            return 0;
        }

        Ql_clt_set_mixer_value("SEC_AUX_PCM_RX Audio Mixer MultiMedia1", 1, "1");
        Ql_clt_set_mixer_value("MultiMedia1 Mixer SEC_AUX_PCM_UL_TX", 1, "1");
        iRet = Ql_AudRecorder_Open(NULL, Ql_cb_Recd1);
        if (iRet != 0) {
            quec_log("open recd failed\n\r");
        }

        /* record */
        recordFlag = 0;
        iRet = Ql_AudRecorder_StartRecord();
        if (iRet != 0) {
            quec_log("start aud recd failed\n\r");
        }

        recdTime = sizeof(pcmBuf) / (8000 * 2) + 1;
        while (recordFlag == 0) {
            sleep(1);
            if (--recdTime == 0) {
                break;
            }
        }
        Ql_AudRecorder_Stop();
        Ql_AudRecorder_Close();

        /* playback */
        int pos;

        devphdl1 = Ql_AudPlayer_Open(NULL, Ql_cb_playback);
        if (devphdl1 < 0) {
            quec_log("open aud play%d failed\n\r", devphdl1);
            return 0;
        }

        pos = 0;
        play1Flag = 0;
        while (pos < sizeof(pcmBuf)) {
            iRet = Ql_AudPlayer_Play(devphdl1, &pcmBuf[pos], sizeof(pcmBuf));
            if (iRet >= 0) {
                pos += iRet;
            }
            else {
                break;
            }
        }
        while (play1Flag != 1) {
            sleep(1);
        }

        sleep(1);
        Ql_AudPlayer_Stop(devphdl1);
        Ql_AudPlayer_Close(devphdl1);
    }
    else if (strcmp(argv[1], "recd2") == 0) 
    {//record and playback and save
        if (argc != 3) {
            quec_log("< recorder needs more parameter\n\r");
            return 0;
        }

        mediaParas.format = AUD_STREAM_FORMAT_PCM;
        iRet = Ql_PcmFileCreat(argv[2], &mediaParas, NULL);
        if (iRet < 0) {
            quec_log("create file failed\n\r");
            return 0;
        }

        recdfd = iRet;

        //Ql_clt_set_mixer_value("SEC_AUX_PCM_RX Audio Mixer MultiMedia1", 1, "1");
        Ql_clt_set_mixer_value("MultiMedia1 Mixer SEC_AUX_PCM_UL_TX", 1, "1");
        iRet = Ql_AudRecorder_Open(NULL, Ql_cb_Recd2);
        if (iRet != 0) {
            quec_log("open recd failed\n\r");
        }
#ifdef TEST_CODEC_5616
        drcagc.control1_mask = 0xffff;
        drcagc.control1_value = 0xc325;
        drcagc.control2_mask = 0xffff;
        drcagc.control2_value = 0x1fa0;
        drcagc.control3_mask = 0xffff;
        drcagc.control3_value = 0x794d;
        if(Ql_AudCodec_Set_ALC5616_DRCAGC(NULL, &drcagc) < 0) {
            quec_log("Ql_AudCodec_Set_ALC5616_DRCAGC failed\n\r");
        } else {
            quec_log("Ql_AudCodec_Set_ALC5616_DRCAGC success\n\r");
        }
#endif
        recordFlag = 0;
        iRet = Ql_AudRecorder_StartRecord();
        if (iRet != 0) {
            quec_log("start aud recd failed\n\r");
        }

        recdTime = 3;
        while (recordFlag == 0) {
            sleep(1);
            if (--recdTime == 0) {
                break;
            }
        }

        close(recdfd);
        Ql_AudRecorder_Stop();
        Ql_AudRecorder_Close();
    }
    else if (strcmp(argv[1], "tone") == 0) 
    {//play tone
        if (argc == 2) {
            tonePara.lowFreq  = 500;
            tonePara.highFreq = 500;
            tonePara.duration = 1000;
            tonePara.volume   = 250;
        }
        else if (argc == 5) {
            tonePara.lowFreq  = atoi(argv[2]);
            tonePara.highFreq = atoi(argv[2]);
            tonePara.duration = atoi(argv[3]);
            tonePara.volume   = atoi(argv[4]);
        }
        else if (argc != 5) {
            quec_log("< tone needs 4 parameter\n\r");
            return 0;
        }
        
        devphdl1 = Ql_AudTone_Open(NULL, NULL);
        if (devphdl1 < 0) {
             quec_log("open tone dev failed\n\r");
             return 0;
        }

        Ql_AudTone_Start(devphdl1, &tonePara);        
        sleep(tonePara.duration / 1000 + 1);
        Ql_AudTone_Stop(devphdl1);
        Ql_AudTone_Close(devphdl1);
    }
#ifdef TEST_CODEC_5616
    else if (strcmp(argv[1], "alc5616_drcagc") == 0) 
    {//alc5616 drcagc
        if(argc != 8) {
            print_usage();
            return 0;
        }

        tmp32 = 0;
        sscanf(argv[2], "%04x", &tmp32);
        drcagc.control1_mask = (unsigned short)tmp32;
        tmp32 = 0;
        sscanf(argv[3], "%04x", &tmp32);
        drcagc.control1_value= (unsigned short)tmp32;
        tmp32 = 0;
        sscanf(argv[4], "%04x", &tmp32);
        drcagc.control2_mask = (unsigned short)tmp32;
        tmp32 = 0;
        sscanf(argv[5], "%04x", &tmp32);
        drcagc.control2_value= (unsigned short)tmp32;
        tmp32 = 0;
        sscanf(argv[6], "%04x", &tmp32);
        drcagc.control3_mask = (unsigned short)tmp32;
        tmp32 = 0;
        sscanf(argv[7], "%04x", &tmp32);
        drcagc.control3_value= (unsigned short)tmp32;

        if(Ql_AudCodec_Set_ALC5616_DRCAGC(NULL, &drcagc) < 0) {
            quec_log("Ql_AudCodec_Set_ALC5616_DRCAGC failed\n\r");
        } else {
            quec_log("Ql_AudCodec_Set_ALC5616_DRCAGC success\n\r");
        }
    }
#endif
    else {
        print_usage();
    }
    
    return 0;
}
