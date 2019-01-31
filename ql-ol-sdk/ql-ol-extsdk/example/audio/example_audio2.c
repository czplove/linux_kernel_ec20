/**
 * @file example_audio2.c
 * @brief 
 *      This file is aimed at file playing/recording process using libql_lib_audio.
 * @author zichar 
 * @version 0.1
 * @date 2018-08-15
 *
 * @detail
 *      This file is an example for extended library libql_lib_audio.
 *      Users can esaily using this library to play wav, mp3, amr file and
 *      record wav, amr file.
 *
 *      The version bigger than 00.00.0006 for libql_lib_audio support this example.
 *
 *      Version check for libql_lib_audio:
 *          root@Linux:~/$ strings libql_lib_audio.so.1 | grep "Lib Version"
 *          Lib Version: 00.02.0002 (or 00_02_0002)
 *
 *          the version number is 00.02.0002
 *
 *          or using: function Ql_AudVersion_Get() witch returns the version string.
 *
 * @Usage:
 *      1. cp ql-ol-crosstool/sysroots/armv7a-vfp-neon-oe-linux-gnueabi/usr/include/ql_audio.h ql-ol-extsdk/include/ql_audio.h
 *      2. change target from example_audio to example_audio2 in
 *         ql-ol-extsdk/example/audio/Makefile (or add example_audio2 in target)
 *      3. using "make" in ql-ol-extsdk/example/audio/.
 *      
 */

#include "ql_oe.h"

#define QLLOG(...)    fprintf(stderr, __VA_ARGS__)

#define DEVICE0 "hw:0,0"
#define DEVICE1 "hw:0,13" //not support voice call data flow

static int play1Flag = 0;
static int play2Flag = 0;

static int volume = 0;
static int volume_left = 0;
static int volume_right = 0;

int Ql_cb_Player1(int hdl, int result)
{
    QLLOG("%s: hdl=%d, result=%d\n\r", __func__, hdl, result);
    if (result == AUD_PLAYER_FINISHED)
    {
        QLLOG("%s: play finished\n\r", __func__);
        play1Flag = 1;
    }
    return 0;
}

int Ql_cb_Player2(int hdl, int result)
{
    QLLOG("%s: hdl=%d, result=%d\n\r", __func__, hdl, result);
    if (result == AUD_PLAYER_FINISHED)
    {
        QLLOG("%s: play finished\n\r", __func__);
        play2Flag = 1;
    }
    return 0;
}

int Ql_cb_Recd1(int result, unsigned char *pBuf, unsigned int len)
{
    static int pos = 0;

    return 0;
}

static void print_usage(void)
{
    QLLOG(
            "\n\r--Useage:\n\r"
            "play file              : ./<process> play1 <file>\n\r"
            "play mixer(two files)  : ./<process> play2 <file1> <file2>\n\r"
            "play datas(20ms/frame) : ./<process> play3 <file(wav,8000/44100hz,mono)>\n\r"
            "play continues         : ./<process> test1 <file>\n\r"
            "play stop(5s)          : ./<process> test2 <file>\n\r"
            "play pause             : ./<process> test3 <file>\n\r"
            "play volume            : ./<process> test4 <file>\n\r"
            "play soft volume       : ./<process> test5 <file>\n\r"
            "voice volume control   : ./<process> test6\n\r"
            "soft loop test         : ./<process> loop\n\r"
            "record wav file(10s)   : ./<process> recdwav file\n\r"
            "record amr file(10s)   : ./<process> recdamrnb file\n\r"
            );
}

int main(int argc, char** argv)
{
    int devphdl1;//paly1
    int devphdl2;//play2
    int filefd1;
    int filefd2;
    int len;
    int state;
    int state2;
    char buffer[1024];

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
            QLLOG("< player needs more parameter\n\r");
            return 0;
        }

        filefd1 = open(argv[2], O_RDONLY);
        if (filefd1 < 0)
        {
            QLLOG("open audio file failed\n");
            return -1;
        }

        play1Flag = 0;
        devphdl1 = Ql_AudPlayer_PlayFile(DEVICE0, filefd1, -1, Ql_cb_Player1);
        if (devphdl1 < 0)
        {
            QLLOG("Play failed\n");
            return -1;
        }
        
        while(!play1Flag)
        {
            //if you don't care about the end time, goto do other things
            //the handle will be closed itself if the file playing end
            sleep(1);
        }
        QLLOG("Player state = %s(%d)\n", Ql_AudPlayer_GetState(devphdl1, &state), state);
        close(filefd1);
    }
    else if (strcmp(argv[1], "play2") == 0) 
    {
        if (argc != 4) 
        {
            QLLOG("< player needs more parameter\n\r");
            return 0;
        }

        filefd1 = open(argv[2], O_RDONLY);
        if (filefd1 < 0)
        {
            QLLOG("open audio file failed\n");
            return -1;
        }

        filefd2 = open(argv[3], O_RDONLY);
        if (filefd2 < 0)
        {
            QLLOG("open audio file failed\n");
            return -1;
        }

        play1Flag = 0;
        play2Flag = 0;

        devphdl1 = Ql_AudPlayer_PlayFile(DEVICE0, filefd1, -1, Ql_cb_Player1);
        if (devphdl1 < 0)
        {
            QLLOG("Play failed\n");
            return -1;
        }
        devphdl2 = Ql_AudPlayer_PlayFile(DEVICE1, filefd2, -1, Ql_cb_Player2);
        if (devphdl2 < 0)
        {
            QLLOG("Play failed\n");
            return -1;
        }
        
        while(!play1Flag || !play2Flag)
        {
            sleep(1);
        }
        QLLOG("Player state = %s(%d)\n", Ql_AudPlayer_GetState(devphdl1, &state), state);
        QLLOG("Player state = %s(%d)\n", Ql_AudPlayer_GetState(devphdl2, &state2), state2);
        close(filefd1);
        close(filefd2);
    }
    else if (strcmp(argv[1], "play3") == 0) 
    {
        char buff[3258];
        int bytes20ms;

        /* 
         * set buffer size, count in ms
         * if your network state is very bad, please set a bigger value
         * 320 means buffer can store and playing 320ms' sound
         */
        Ql_AudPlayer_SetBufsize_ms(DEVICE0, 320); //default value is 80ms

        filefd1 = open(argv[2], O_RDONLY);
        if (filefd1 < 0)
        {
            QLLOG("open audio file failed\n");
            return -1;
        }

        //devphdl1 = Ql_AudPlayer_Open(NULL, Ql_cb_Player1); //8000hz 1ch -> 20ms 320bytes
        //bytes20ms = 320;
        devphdl1 = Ql_AudPlayer_OpenExt(NULL, Ql_cb_Player1,
                    0, 2, 44100, SNDRV_PCM_FORMAT_S16_LE); //44.1khz 2ch -> 20ms 3528bytes
        bytes20ms = 3258;
        if (devphdl1 < 0) {
            QLLOG("open aud handle failed, handle = %d\n", devphdl1);
            return -1;
        }


        while(1)
        {
            len = read(filefd1, buff, bytes20ms);
            {
                if (len != bytes20ms)
                {
                    QLLOG("File end, len = %d\n", len);
                    break;
                }
            }
            
            usleep(20000);

            len = Ql_AudPlayer_Play(devphdl1, buff, bytes20ms);
            if (len <= 0)
            {
                QLLOG("Play error\n");
                break;
            }
        }

        //stop
        Ql_AudPlayer_Stop(devphdl1);
        //close handle
        Ql_AudPlayer_Close(devphdl1); //block till play thread end
        //close fd
        close(filefd1);

        return 0;
    }
    else if (strcmp(argv[1], "voice1") == 0) 
    {
        /*
         * some firmware have this problem:
         * the call hang up when the music still playing to the
         * other side of the call. next time, the other side of
         * the call could not hear the music any more */
        int ret;
        unsigned int inf;

        if (argc != 3) 
        {
            QLLOG("< player needs more parameter\n\r");
            return 0;
        }

        filefd1 = open(argv[2], O_RDONLY);
        if (filefd1 < 0)
        {
            QLLOG("open audio file failed\n");
            return -1;
        }

        ret = Ql_AudPlayer_GetInterface(DEVICE0, &inf);
        if (ret < 0)
        {
            QLLOG("Get interface failed, %s\n", DEVICE0);
            return -1;
        }
        ret = Ql_AudPlayer_SetInterface(DEVICE0, inf | VOICE_OUT_IF);
        if (ret < 0)
        {
            QLLOG("Set interface failed, %s\n", DEVICE0);
            return -1;
        }
        

        play1Flag = 0;
        devphdl1 = Ql_AudPlayer_PlayFile(DEVICE0, filefd1, -1, Ql_cb_Player1);
        if (devphdl1 < 0)
        {
            QLLOG("Play failed\n");
            return -1;
        }
        
        while(!play1Flag)
        {
            //if you don't care about the end time, goto do other things
            sleep(1);
        }
        Ql_AudPlayer_Stop(devphdl1);
        Ql_AudPlayer_Close(devphdl1);

        Ql_AudPlayer_GetInterface(DEVICE0, &inf);
        Ql_AudPlayer_SetInterface(DEVICE0, inf & (~VOICE_OUT_IF));
        
        close(filefd1);
    }
    else if (strcmp(argv[1], "test1") == 0)  //continus play
    {
        filefd1 = open(argv[2], O_RDONLY);
        if (filefd1 < 0)
        {
            QLLOG("open audio file failed\n");
            return -1;
        }
        while(1)
        {
            play1Flag = 0;
            devphdl1 = Ql_AudPlayer_PlayFile(DEVICE0, filefd1, -1, Ql_cb_Player1);
            if (devphdl1 < 0)
            {
                QLLOG("Play failed\n");
                return -1;
            }
            
            while(!play1Flag)
            {
                sleep(1);
            }
        }
        Ql_AudPlayer_Stop(devphdl1);
        Ql_AudPlayer_Close(devphdl1);

        close(filefd1);
    }
    else if (strcmp(argv[1], "test2") == 0)  //continus play stop every 5s
    {
        filefd1 = open(argv[2], O_RDONLY);
        if (filefd1 < 0)
        {
            QLLOG("open audio file failed\n");
            return -1;
        }

        while(1)
        {
            play1Flag = 0;
            devphdl1 = Ql_AudPlayer_PlayFile(DEVICE1, filefd1, -1, Ql_cb_Player1);
            if (devphdl1 < 0)
            {
                QLLOG("Play failed\n");
                return -1;
            }
            
            sleep(5);
        }
        Ql_AudPlayer_Stop(devphdl1);
        Ql_AudPlayer_Close(devphdl1);

        close(filefd1);
    }
    else if (strcmp(argv[1], "test3") == 0)  //play with pause
    {
        //continus play when play end

        filefd1 = open(argv[2], O_RDONLY);
        if (filefd1 < 0)
        {
            QLLOG("open audio file failed\n");
            return -1;
        }

        play1Flag = 0;
        devphdl1 = Ql_AudPlayer_PlayFile(DEVICE0, filefd1, -1, Ql_cb_Player1);
        if (devphdl1 < 0)
        {
            QLLOG("Play failed\n");
            return -1;
        }

        while(!play1Flag)
        {
            //pause test
            QLLOG("Player state = %s(%d)\n", Ql_AudPlayer_GetState(devphdl1, &state), state);
            sleep(1);
            Ql_AudPlayer_Pause(devphdl1);
            QLLOG("Player state = %s(%d)\n", Ql_AudPlayer_GetState(devphdl1, &state), state);
            sleep(1);
            Ql_AudPlayer_Resume(devphdl1);
            QLLOG("Player state = %s(%d)\n", Ql_AudPlayer_GetState(devphdl1, &state), state);
        }

        Ql_AudPlayer_Stop(devphdl1);
        Ql_AudPlayer_Close(devphdl1);

        close(filefd1);
    }
    else if (strcmp(argv[1], "test4") == 0) //play with volume change(alc5615, tlv320, max9867, nau88xx)
    {
        filefd1 = open(argv[2], O_RDONLY);
        if (filefd1 < 0)
        {
            QLLOG("open audio file failed\n");
            return -1;
        }
        while(1)
        {
            play1Flag = 0;
            devphdl1 = Ql_AudPlayer_PlayFile(DEVICE0, filefd1, -1, Ql_cb_Player1);
            if (devphdl1 < 0)
            {
                QLLOG("Play failed\n");
                return -1;
            }
            
            volume = 80;
            while(!play1Flag)
            {
                sleep(1);
                volume += 10;
                volume_left = (volume + 10) % 100;
                volume_right = (volume + 10) % 100;
                Ql_AudCodec_SetVolume(0, volume_left, volume_right);
            }
        }
        Ql_AudPlayer_Stop(devphdl1);
        Ql_AudPlayer_Close(devphdl1);

        close(filefd1);
    }
    else if (strcmp(argv[1], "test5") == 0) //play with soft volume change(alc5615, tlv320, max9867, nau88xx)
    {
        filefd1 = open(argv[2], O_RDONLY);
        if (filefd1 < 0)
        {
            QLLOG("open audio file failed\n");
            return -1;
        }
        while(1)
        {
            play1Flag = 0;
            devphdl1 = Ql_AudPlayer_PlayFile(DEVICE0, filefd1, -1, Ql_cb_Player1);
            if (devphdl1 < 0)
            {
                QLLOG("Play failed\n");
                return -1;
            }
            
            volume = 80;
            while(!play1Flag)
            {
                sleep(1);
                volume += 10;
                volume %= 100;
                Ql_AudSoft_SetVolume(devphdl1, volume);
            }
        }
        Ql_AudPlayer_Stop(devphdl1);
        Ql_AudPlayer_Close(devphdl1);

        close(filefd1);
    }
    else if (strcmp(argv[1], "test6") == 0)
    {
        char type;
        int val;


        while(1)
        {
            QLLOG("please start your voice call first\n");
            QLLOG("type:l [vol]  -> set Rx gain level, vol is 0-5 (can be set in or out the call)\n");
            QLLOG("type:m [type] -> mute, type=0 for RX, type=1 for TX (must be set in call,invalid after reset the call)\n");
            QLLOG("type:u [type] -> mute, type=0 for RX, type=1 for TX (must be set in call,invalid after reset the call)\n");
            QLLOG("type:r [vol]  -> set Rx gain, vol is 0-65535 (must be set before call start)\n");
            QLLOG("type:t [vol]  -> set Tx gain, vol is 0-65535 (must be set before call start)\n");
            QLLOG("\n:");

            scanf("%c %d", &type, &val);
            fflush(stdin);

            QLLOG("type = %c, val = %d\n", type, val);

            if (type == 'r')
            {
                    Ql_AudVoice_SetVolume(VOICE_RX_VOL, val);
            }
            else if (type == 't')
            {
                    Ql_AudVoice_SetVolume(VOICE_TX_VOL, val);
            }
            else if (type == 'm')
            {
                if (val)
                {
                    Ql_AudVoice_SetVolume(VOICE_TX_MUTE, 1);
                }
                else
                {
                    Ql_AudVoice_SetVolume(VOICE_RX_MUTE, 1);
                }
            }
            else if (type == 'u')
            {
                if (val)
                {
                    Ql_AudVoice_SetVolume(VOICE_TX_MUTE, 0);
                }
                else
                {
                    Ql_AudVoice_SetVolume(VOICE_RX_MUTE, 0);
                }
            }
            else if (type == 'l')
            {
                if (val < 0 || val > 5)
                {
                    QLLOG("Vol level must be 0-5\n");
                    return -1;
                }

                Ql_AudVoice_SetVolume(VOICE_RX_LEVEL, val);
            }
        }
    }
    else if (strcmp(argv[1], "loop") == 0)
    {
        int phdl;
        int chdl;
        char data[320];

        phdl = Ql_AudPlayer_Open(DEVICE0, Ql_cb_Player1);
        chdl = Ql_AudRecorder_Open(DEVICE0, Ql_cb_Recd1);

        while(1)
        {
            Ql_AudRecorder_Record(chdl, data, 320);
            Ql_AudPlayer_Play(phdl, data, 320);
            //QLLOG("---- sample ----\n");
        }

        Ql_AudRecorder_HClose(chdl);
        Ql_AudPlayer_Close(phdl);
    }
    else if (strcmp(argv[1], "recdwav") == 0)
    {
        int hdl;

        if (argc != 3) 
        {
            print_usage();
            return 0;
        }

        hdl = Ql_AudRecorder_RecordWAV(DEVICE0, argv[2],
                                        1, 8000, SNDRV_PCM_FORMAT_S16_LE);
        sleep(10);
        Ql_AudRecorder_HStop(hdl);
        Ql_AudRecorder_HClose(hdl);
    }
    else if (strcmp(argv[1], "recdamrnb") == 0)
    {
        int hdl;

        if (argc != 3) 
        {
            print_usage();
            return 0;
        }

        hdl = Ql_AudRecorder_RecordAMRNB(DEVICE0, argv[2]);
        sleep(10);
        Ql_AudRecorder_HStop(hdl);
        Ql_AudRecorder_HClose(hdl);
    }
    else {
        print_usage();
    }
    
    return 0;
}
