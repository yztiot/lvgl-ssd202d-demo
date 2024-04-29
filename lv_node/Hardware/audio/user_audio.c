#include "user_audio.h"
#include <mad.h>
#include "ui_app.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/soundcard.h>
#include <pthread.h>


#define BUFSIZE 8192


/*
 * This is a private message structure. A generic pointer to this structure
 * is passed to each of the callback functions. Put here any data you need
 * to access from within the callbacks.
 */
struct buffer
{
    FILE *fp;                    /*file pointer*/
    unsigned int flen;           /*file length*/
    unsigned int fpos;           /*current position*/
    unsigned char fbuf[BUFSIZE]; /*buffer*/
    unsigned int fbsize;         /*indeed size of buffer*/
    unsigned char init;
    void *user_data;
};
typedef struct buffer mp3_file;
// mp3_file *mp3fp;

/* Time duration for a MP3 file 总时长 */
static unsigned long long duration_frac = 0;
static double duration_sec = 0;
/* 当前时长 */
static unsigned long long current_frac = 0;
static double current_sec = 0;

// int soundfd;              /*soundcard file*/
// unsigned int prerate = 0; /*the pre simple rate*/
static MI_S32 AoDevId = 0;
static MI_S32 AoChn = 0;
static pthread_t tid_playmp3;

bool Mp3Playing = FALSE;
static uint32_t frame_count = 0; // 帧数
FrameList *current_frame = NULL; // 当前帧位置

static int Volume = -40; // 音量


// int writedsp(int c)
// {
//     return write(soundfd, (char *)&c, 1);
// }

int set_dsp(signed short channels, unsigned int dwSamplesPerSec,
            int s32AoVolume)
{
    MI_AUDIO_Attr_t stAoSetAttr, stAoGetAttr;
    MI_AO_AdecConfig_t stAoSetAdecConfig, stAoGetAdecConfig;
    MI_AO_VqeConfig_t stAoSetVqeConfig, stAoGetVqeConfig;
    MI_S32 s32AoGetVolume;
    MI_AO_ChnParam_t stAoChnParam;
    MI_U32 u32DmaBufSize;
    MI_AUDIO_SampleRate_e eAoInSampleRate = E_MI_AUDIO_SAMPLE_RATE_INVALID;

    memset(&stAoSetAttr, 0x0, sizeof(MI_AUDIO_Attr_t));
    stAoSetAttr.eBitwidth = E_MI_AUDIO_BIT_WIDTH_16;
    stAoSetAttr.eWorkmode = E_MI_AUDIO_MODE_I2S_MASTER;
    stAoSetAttr.WorkModeSetting.stI2sConfig.bSyncClock = FALSE;
    stAoSetAttr.WorkModeSetting.stI2sConfig.eFmt = E_MI_AUDIO_I2S_FMT_I2S_MSB;
    stAoSetAttr.WorkModeSetting.stI2sConfig.eMclk = E_MI_AUDIO_I2S_MCLK_0;
    stAoSetAttr.u32PtNumPerFrm = 1024; // MI_AUDIO_SAMPLE_PER_FRAME;
    stAoSetAttr.u32ChnCnt = channels;

    if (stAoSetAttr.u32ChnCnt == 2)
    {
        stAoSetAttr.eSoundmode = E_MI_AUDIO_SOUND_MODE_STEREO;
    }
    else if (stAoSetAttr.u32ChnCnt == 1)
    {
        stAoSetAttr.eSoundmode = E_MI_AUDIO_SOUND_MODE_MONO;
    }

    stAoSetAttr.eSamplerate = (MI_AUDIO_SampleRate_e)dwSamplesPerSec;
    eAoInSampleRate = (MI_AUDIO_SampleRate_e)dwSamplesPerSec;

    stAoSetVqeConfig.bAgcOpen = FALSE;
    stAoSetVqeConfig.bAnrOpen = FALSE;
    stAoSetVqeConfig.bEqOpen = FALSE;
    stAoSetVqeConfig.bHpfOpen = FALSE;
    stAoSetVqeConfig.s32FrameSample = 128;
    stAoSetVqeConfig.s32WorkSampleRate = eAoInSampleRate;
    // memcpy(&stAoSetVqeConfig.stAgcCfg, &stAgcCfg, sizeof(MI_AUDIO_AgcConfig_t));
    // memcpy(&stAoSetVqeConfig.stAnrCfg, &stAnrCfg, sizeof(MI_AUDIO_AnrConfig_t));
    // memcpy(&stAoSetVqeConfig.stEqCfg, &stEqCfg, sizeof(MI_AUDIO_EqConfig_t));
    // memcpy(&stAoSetVqeConfig.stHpfCfg, &stHpfCfg, sizeof(MI_AUDIO_HpfConfig_t));

    ExecFunc(MI_AO_SetPubAttr(AoDevId, &stAoSetAttr), MI_SUCCESS);
    ExecFunc(MI_AO_GetPubAttr(AoDevId, &stAoGetAttr), MI_SUCCESS);
    ExecFunc(MI_AO_Enable(AoDevId), MI_SUCCESS);
    ExecFunc(MI_AO_EnableChn(AoDevId, AoChn), MI_SUCCESS);

    if (FALSE)
    {
        ExecFunc(MI_AO_SetVqeAttr(AoDevId, AoChn, &stAoSetVqeConfig), MI_SUCCESS);
        ExecFunc(MI_AO_GetVqeAttr(AoDevId, AoChn, &stAoGetVqeConfig), MI_SUCCESS);
        ExecFunc(MI_AO_EnableVqe(AoDevId, AoChn), MI_SUCCESS);
    }

    ExecFunc(MI_AO_SetVolume(AoDevId, s32AoVolume), MI_SUCCESS);
    ExecFunc(MI_AO_GetVolume(AoDevId, &s32AoGetVolume), MI_SUCCESS);

     printf("set dsp volume value [%d %d]\n", s32AoVolume, s32AoGetVolume);

    return 0;
}

/*
 * This is perhaps the simplest example use of the MAD high-level API.
 * Standard input is mapped into memory via mmap(), then the high-level API
 * is invoked with three callbacks: input, output, and error. The output
 * callback converts MAD's high-resolution PCM samples to 16 bits, then
 * writes them to standard output in little-endian, stereo-interleaved
 * format.mp3file
 */

static int decode(mp3_file *mp3fp);
static int decode_preprocess(mp3_file *mp3fp);

void Mp3PlayStopDec(void)
{
    // 
    if (Mp3Playing == FALSE)
        return;
    
    Mp3Playing = FALSE;
    pthread_join(tid_playmp3, NULL);
    MI_AO_ResumeChn(AoDevId, AoChn);
    MI_AO_DisableChn(AoDevId, AoChn);
    MI_AO_Disable(AoDevId);
}

// 解码并输出音频
void *decMp3Init(void *mp3file)
{
    long flen, fsta, fend;
    
    mp3_file *mp3fp;

    mp3fp = (mp3_file *)malloc(sizeof(mp3_file));
    if ((mp3fp->fp = fopen((const char *)mp3file, "r")) == NULL)
    {
        printf("can't open source file : %s \n", (char *)mp3file);
        return NULL;
    }
    printf("start play : %s \n\r", (char *)mp3file);

    fsta = ftell(mp3fp->fp);
    fseek(mp3fp->fp, 0, SEEK_END);
    fend = ftell(mp3fp->fp);
    flen = fend - fsta;
    if (flen > 0)
        fseek(mp3fp->fp, 0, SEEK_SET);
    fread(mp3fp->fbuf, 1, BUFSIZE, mp3fp->fp);
    mp3fp->fbsize = BUFSIZE;
    mp3fp->fpos = BUFSIZE;
    mp3fp->flen = flen;
    mp3fp->init = 0x00;
    decode(mp3fp); // 开始解码
    
    fclose(mp3fp->fp);
    
    printf("[%s] play completed!\n\r", (char *)mp3file);

    destroy_frame_list(frame_head);
    Mp3PlayStopDec();
    return NULL;
}

// 获取音频时长
int get_duration(const char *mp3file)
{
    // 清空帧链表
    destroy_frame_list(frame_head);
    long flen, fsta, fend;
   
    mp3_file *mp3fp;

    mp3fp = (mp3_file *)malloc(sizeof(mp3_file));
    if ((mp3fp->fp = fopen((const char *)mp3file, "r")) == NULL)
    {
        printf("can't open source file : %s \n", (char *)mp3file);
        return 0;
    }
    printf("start preprocess : %s \n\r", (char *)mp3file);

    fsta = ftell(mp3fp->fp);
    fseek(mp3fp->fp, 0, SEEK_END);
    fend = ftell(mp3fp->fp);
    flen = fend - fsta;
    if (flen > 0)
        fseek(mp3fp->fp, 0, SEEK_SET);
    fread(mp3fp->fbuf, 1, BUFSIZE, mp3fp->fp);
    mp3fp->fbsize = BUFSIZE;
    mp3fp->fpos = BUFSIZE;
    mp3fp->flen = flen;
    mp3fp->init = 0x00;
    decode_preprocess(mp3fp);

    fclose(mp3fp->fp);
    Mp3PlayStopDec();
    return duration_sec;
}

// 调整播放中的音频到任意帧
void to_any_frame(int count_per)
{
    int need_frame = frame_count * count_per / 100;
    // printf("count_per %d need_frame%d\n", count_per,  need_frame);
    
    FrameList *curr = frame_head;
    while(curr != NULL)
    {
        
        if(curr->count == need_frame)
        {
            break;
        }
            
        curr = curr->next;
    }
    
    // 修改当前帧位置和时间
    current_frame = curr;
    current_frac = curr->dura;
    
}


static enum mad_flow input(void *data, struct mad_stream *stream)
{
    mp3_file *mp3fp = (mp3_file *)data;
    enum mad_flow ret_code = MAD_FLOW_CONTINUE;
    
    int unproc_data_size = stream->bufend - stream->next_frame; // 未处理数据的大小
    int copy_size;
    // 使用链表中保存的帧位置
    if (current_frame != NULL)
    {
        // 打印当前帧的位置、大小和计数信息
        //printf("pos %d, size %d, count %d\n", current_frame->pos, current_frame->size, current_frame->count);

        // 将文件指针移动到当前帧的位置
        fseek(mp3fp->fp, current_frame->pos, SEEK_SET);

        // 将未处理的数据移到缓冲区前面
        memcpy(mp3fp->fbuf, mp3fp->fbuf + mp3fp->fbsize - unproc_data_size, unproc_data_size);

        // 计算要复制的数据的大小
        copy_size = BUFSIZE - unproc_data_size;

        // 如果剩余文件大小不足以填充缓冲区，减小要复制的大小
        if (current_frame->size < copy_size)
        {
            copy_size = current_frame->size;
        }

        // 从文件中读取数据到缓冲区
        fread(mp3fp->fbuf + unproc_data_size, 1, copy_size, mp3fp->fp);

        // 更新缓冲区的大小和文件位置
        mp3fp->fbsize = unproc_data_size + copy_size;
        mp3fp->fpos += copy_size;

        // 将缓冲区的数据提供给解码器的输入流
        mad_stream_buffer(stream, mp3fp->fbuf, mp3fp->fbsize);

        // 移动到下一个帧
        current_frame = current_frame->next;
    
    }
    else
    {
       
        ret_code = MAD_FLOW_STOP; // 如果没有下一个帧，停止解码
    }

    return ret_code;
}

// 预处理输入函数
static enum mad_flow input_preprocess(void *data, struct mad_stream *stream)
{
    mp3_file *mp3fp;
    enum mad_flow ret_code;
    int unproc_data_size; /*the unprocessed data's size*/
    int copy_size;
    
    mp3fp = (mp3_file *)data;
    if (mp3fp->fpos < mp3fp->flen)
    {
        unproc_data_size = stream->bufend - stream->next_frame;
        
        memcpy(mp3fp->fbuf, mp3fp->fbuf + mp3fp->fbsize - unproc_data_size,
               unproc_data_size);
            
        copy_size = BUFSIZE - unproc_data_size;
        if (mp3fp->fpos + copy_size > mp3fp->flen)
        {
            copy_size = mp3fp->flen - mp3fp->fpos;
        }
        // 当前帧插入链表
        frame_list_insert(&frame_head, mp3fp->fpos, copy_size, ++frame_count);
        mp3fp->user_data = get_node_by_count(frame_count); // 用来记录当前帧的持续时间，在帧头处理函数获取
        // printf("unproc_data_size %d, mp3fp->fpos %d, copy_size %d, count %d\n", unproc_data_size, mp3fp->fpos, copy_size, frame_count);
        fread(mp3fp->fbuf + unproc_data_size, 1, copy_size, mp3fp->fp);
        mp3fp->fbsize = unproc_data_size + copy_size;
        mp3fp->fpos += copy_size;

        /*Hand off the buffer to the mp3 input stream 将缓冲区切换到mp3输入流 */
        mad_stream_buffer(stream, mp3fp->fbuf, mp3fp->fbsize);
        ret_code = MAD_FLOW_CONTINUE;
    }
    else
    {
        ret_code = MAD_FLOW_STOP;
    }

    return ret_code;
}

/*
 * The following utility routine performs simple rounding, clipping, and
 * scaling of MAD's high-resolution samples down to 16 bits. It does not
 * perform any dithering or noise shaping, which would be recommended to
 * obtain any exceptional audio quality. It is therefore not recommended to
 * use this routine if high-quality output is desired.
 */

static inline signed int scale(mad_fixed_t sample)
{
    /* round */
    sample += (1L << (MAD_F_FRACBITS - 16));

    /* clip */
    if (sample >= MAD_F_ONE)
        sample = MAD_F_ONE - 1;
    else if (sample < -MAD_F_ONE)
        sample = -MAD_F_ONE;

    /* quantize */
    return sample >> (MAD_F_FRACBITS + 1 - 16);
}

/*
 * This is the output callback function. It is called after each frame of
 * MPEG audio data has been completely decoded. The purpose of this callback
 * is to output (or play) the decoded PCM audio.
 */

// 输出函数做相应的修改，目的是解决播放音乐时声音卡的问题。
static enum mad_flow output(void *data, struct mad_header const *header,
                            struct mad_pcm *pcm)
{
    unsigned int nchannels, nsamples;
    mad_fixed_t const *left_ch, *right_ch;
    // pcm->samplerate contains the sampling frequency
    nchannels = pcm->channels;
    nsamples = pcm->length;
    left_ch = pcm->samples[0];
    right_ch = pcm->samples[1];
    short buf[nsamples * 2];
    int i = 0;
    // printf(">>%d\n", nsamples);

    mp3_file *p = (mp3_file *)data;
    if (p->init == 0x00)
    {
        set_dsp(nchannels, pcm->samplerate, Volume);
        p->init = 0x01;
    }

    while (nsamples--)
    {
        signed int sample;
        // output sample(s) in 16-bit signed little-endian PCM
        sample = scale(*left_ch++);
        buf[i++] = sample & 0xFFFF;
        if (nchannels == 2)
        {
            sample = scale(*right_ch++);
            buf[i++] = sample & 0xFFFF;
        }
        if (Mp3Playing == FALSE)
        {
            printf("stop mp3 \n\r");
            // Mp3PlayStop();
            return MAD_FLOW_STOP;
        }
    }
    // fprintf(stderr, ".");
    //  write(soundfd, &buf[0], i * 2);

    MI_AUDIO_Frame_t stAoSendFrame;
    memset(&stAoSendFrame, 0x0, sizeof(MI_AUDIO_Frame_t));
    stAoSendFrame.u32Len = i * 2;
    stAoSendFrame.apVirAddr[0] = &buf[0];
    stAoSendFrame.apVirAddr[1] = NULL;
    int s32Ret = MI_SUCCESS;
    do
    {
        s32Ret = MI_AO_SendFrame(AoDevId, AoChn, &stAoSendFrame, -1);

    } while (s32Ret == MI_AO_ERR_NOBUF);

    MI_AO_ChnState_t stChnState;
    do
    {
        s32Ret = MI_AO_QueryChnStat(AoDevId, AoChn, &stChnState);
        if (Mp3Playing == FALSE)
        {
            printf("stop mp3 \n\r");
            return MAD_FLOW_STOP;
        }
    } while (stChnState.u32ChnBusyNum > 500);

    if (s32Ret != MI_SUCCESS)
    {
        printf("[Warning]: MI_AO_SendFrame fail, error is 0x%x: \n", s32Ret);
    }

    return MAD_FLOW_CONTINUE;
}

/*
 * This is the error callback function. It is called whenever a decoding
 * error occurs. The error is indicated by stream->error; the list of
 * possible MAD_ERROR_* errors can be found in the mad.h (or stream.h)
 * header file.
 */

static enum mad_flow error(void *data,
                           struct mad_stream *stream,
                           struct mad_frame *frame)
{
    mp3_file *mp3fp = (mp3_file *)data;
#if 0
    printf("decoding error 0x%04x (%s) at byte offset %u\n",
           stream->error, mad_stream_errorstr(stream),
           stream->this_frame - mp3fp->fbuf);
#endif
    /* return MAD_FLOW_BREAK here to stop decoding (and propagate an error) */

    return MAD_FLOW_CONTINUE;
}


/* 回调函数，在每次读取帧头后执行．这里作为前处理，获取时间
 * To calculate time duration only.
 */
static enum mad_flow header_preprocess(void *data,  struct mad_header const *header )
{
    mp3_file *buffer = data;
    FrameList *node = (FrameList *) buffer->user_data;
    /* Count duration */
    duration_frac += header->duration.fraction;
    // 修改最新获取的帧所在时间
    if(buffer->user_data == NULL)
    {
        printf("err5\n");
    }
    else
    {
        node->dura= duration_frac;
        // printf("header->duration.fraction %ld\n", header->duration.fraction);
    }
    
    return MAD_FLOW_IGNORE;
}


/* 回调函数，在每次读取帧头后执行．在这里读取mp3格式，播放时间等信息．
 * Parse keyinput and frame header
 *
 */
static enum mad_flow header(void *data,  struct mad_header const *header )
{
    int mad_ret=MAD_FLOW_CONTINUE;
    current_frac += header->duration.fraction; // 获取当前时间
    
    return mad_ret;
}


/* MP3前处理解码函数:	初始化解码器并执行解码不输出音频．　这里作为播放前处理，获取每帧帧头位置、帧长和总时长
   Preprocess decoding. Read frame headers 
 */
static int decode_preprocess(mp3_file *mp3fp)
{
    struct mad_decoder decoder;
    int result;
    duration_frac = 0;
    duration_sec = 0;
    frame_count = 0;

    /* configure input, output, and error functions */
    mad_decoder_init(&decoder, mp3fp,
                     input_preprocess, header_preprocess /* header */, 0 /* filter */, 0 /* output */,
                     0 /* error */, 0 /* message */);

    /* start decoding */
    result = mad_decoder_run(&decoder, MAD_DECODER_MODE_SYNC);

    /* release the decoder */
    mad_decoder_finish(&decoder);
    // 计算时长
    duration_sec  += 1.0*duration_frac / MAD_TIMER_RESOLUTION;
    printf("duration_frac = %lld\n", duration_frac);
    printf("duration_sec = %f\n", duration_sec);
    return result;
}



/*
 * This is the function called by main() above to perform all the decoding.
 * It instantiates a decoder object and configures it with the input,
 * output, and error callback functions above. A single call to
 * mad_decoder_run() continues until a callback function returns
 * MAD_FLOW_STOP (to stop decoding) or MAD_FLOW_BREAK (to stop decoding and
 * signal an error).
 */
static int decode(mp3_file *mp3fp)
{
    struct mad_decoder decoder;
    int result;
    current_sec = 0;
    current_frac = 0;
    
    current_frame = frame_head;
    // print_frame_list(frame_head);
    /* configure input, output, and error functions */
    // 初始化输入、帧头处理、输出函数
    mad_decoder_init(&decoder, mp3fp,
                     input, header , 0 /* filter */, output,
                     error, 0 /* message */);

    /* start decoding */
    result = mad_decoder_run(&decoder, MAD_DECODER_MODE_SYNC);

    /* release the decoder */
    mad_decoder_finish(&decoder);

    return result;
}
int mp3_codec(const char *mp3_filepath)
{
    int ret;
    Mp3PlayStopDec();
    Mp3Playing = TRUE;

    ret = pthread_create(&tid_playmp3, NULL, decMp3Init, (void *)mp3_filepath);
    if (ret != 0)
    {
        printf("pthread_create decmp3init failed!\n");
    }

    return ret;
}

int set_volume(int AoVolume)
{
    MI_S32 s32AoGetVolume;

    ExecFunc(MI_AO_SetVolume(AoDevId, AoVolume), MI_SUCCESS);
    ExecFunc(MI_AO_GetVolume(AoDevId, &s32AoGetVolume), MI_SUCCESS);

    //printf("Set DSP volume value: %d (Current volume: %d)\n", AoVolume, s32AoGetVolume);
    Volume = AoVolume;
    return 0;
}

int get_current_sec()
{
    current_sec = 1.0*current_frac / MAD_TIMER_RESOLUTION;
    // printf("current_frac %lld \n", current_frac);
    // printf("current_sec %f\n", current_sec);
    return current_sec;
}



void mp3_pause()
{
    MI_AO_PauseChn(AoDevId, AoChn);
}
void mp3_resume()
{
    MI_AO_ResumeChn(AoDevId, AoChn);
}
