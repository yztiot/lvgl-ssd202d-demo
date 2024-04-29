/*
 * @Description: 
 * @Author: chen
 * @Date: 2023-12-04 15:13:01
 * @LastEditors: chen
 * @LastEditTime: 2023-12-20 17:12:43
 */

#include "ui_app.h"
#include <signal.h>

#include "inc/player.h"
#include "inc/interface.h"



extern pthread_mutex_t mutex;  
bool player_working = false; // player正在工作
int player_control = 0; // 控制播放器

// 播放器线程
void *player_thread(void *arg)
{
    double duration = 0, position = 0;  // 视频总时长，当前播放位置

    mm_player_set_opts("audio_device", "", 0); // 设置音频设备

    printf("try playing %s ...\n", (char *)arg); 

    mm_player_set_opts("video_rotate", "", AV_ROTATE_NONE); // 设置视频旋转角度
    mm_player_set_opts("video_only", "", 0);                // 设置视频只播放
    mm_player_set_opts("video_ratio", "", AV_SCREEN_MODE);  // 设置视频比例
    mm_player_set_opts("enable_scaler", "", 0);             // 设置是否启用缩放
    mm_player_set_opts("resolution", "8294400", 0);         // 设置分辨率
    mm_player_set_opts("play_mode", "", AV_ONCE);           // 设置播放模式

    
    int ret = mm_player_open((const char *)arg, 0, 0, 1024, 600); // 打开视频文件
    if(ret < 0){
        perror("mm_player_open err");
        lv_mem_free(arg);
        pthread_mutex_lock(&mutex);
        lv_obj_del(control_page);
        lv_obj_set_style_opa(lv_scr_act(), LV_OPA_100, 0);
        pthread_mutex_unlock(&mutex);
        return NULL;
    }
    mm_player_getduration(&duration); // 获取视频总时长
    player_working = true;
  
    while (player_working)
    {
        int status = mm_player_get_status(); //获取状态
        // printf("status = %d\n", status);
        if ((status & AV_PLAY_COMPLETE) == AV_PLAY_COMPLETE)
        {
            printf("mmplayer has been closed!\n"); // mmplayer已关闭
            player_working = false;                
        }
        // 控制指令
        if(player_control != 0)
        {
            static bool pause = false;
            switch (player_control)
            {
            case 1:
                if(!pause){
                    mm_player_pause(); //暂停播放
                    pause = true;
                }
                else {
                    mm_player_resume(); //恢复播放
                    pause = false;
                }
                player_control = 0;
                break;
            case 2:
                mm_player_getposition(&position);          //获取当前播放位置
                position -= 2.0;                           //快退2秒
                position = (position <= 0) ? 0 : position; //判断是否小于0
                mm_player_seek2time(position);             //跳转到指定位置
                player_control = 0;
                break;
            case 3:
                mm_player_getposition(&position);                        //获取当前播放位置
                position += 2.0;                                         //快进2秒
                position = (position >= duration) ? duration : position; //判断是否超过总时长
                mm_player_seek2time(position);                           //跳转到指定位置
                player_control = 0;
                break;

            default:
                break;
            }   
        }
        usleep(50 * 1000); // 延迟50ms
        
    }
    mm_player_close();
 
    
    // 退出界面

    // 获取点击对象删除,显示ui界面
    
    pthread_mutex_lock(&mutex);
    exit_video_page();
    
    pthread_mutex_unlock(&mutex);
   
    return NULL;
}