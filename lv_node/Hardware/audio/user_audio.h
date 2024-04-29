/*
 * @Description: 
 * @Author: chen
 * @Date: 2023-12-06 16:38:08
 * @LastEditors: chen
 * @LastEditTime: 2023-12-15 14:47:52
 */
#ifndef _AUDIO_H_
#define _AUDIO_H_

#include <stdio.h>
#include <stdbool.h>
#include "mi_common.h"
#include "mi_sys.h"
#include "mi_ao.h"


#define ExecFunc(result, value)\
    if (result != value)\
    {\
        printf("[%s %d]exec function failed\n", __FUNCTION__, __LINE__);\
        return -1;\
    }\

extern void Mp3PlayStopDec(void);
extern int mp3_codec(const char *mp3_filepath);
extern int set_volume(int AoVolume);
extern bool Mp3Playing;
int get_duration(const char *mp3file);
int get_current_sec();
void to_any_frame(int count_per);

void mp3_pause();
void mp3_resume();
#endif
