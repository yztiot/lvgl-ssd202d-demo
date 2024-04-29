/*
 * @Description: 
 * @Author: chen
 * @Date: 2023-11-30 17:31:52
 * @LastEditors: chen
 * @LastEditTime: 2024-02-06 15:20:34
 */
#include "ui_app.h"
#define __USE_MISC
#include <dirent.h> 
#include <sys/stat.h>

static lv_obj_t *progress_slider = NULL; // 播放进度条
static lv_obj_t *curr_label      = NULL; // 当前时长显示
static lv_timer_t *update_timer  = NULL; // 更新进度定时器
static lv_obj_t *dura_label      = NULL; // 总时长显示
static lv_obj_t *path_label      = NULL; // 路径显示
static Node *curr_node           = NULL; // 当前播放的节点
static int duration              = 0;    // 时长
static lv_obj_t *PAUSE_btn       = NULL; // 暂停、继续按钮



static void duration_time(char buf[], int dura);


// 音量调节事件
static void lv_Voiceslider_event_cb(lv_event_t *e)
{
    lv_obj_t *slider = lv_event_get_target(e);
    set_volume((int)lv_slider_get_value(slider) - 60);
}
// 进度调节事件
static void progress_slider_event_cb(lv_event_t *e)
{
    lv_obj_t *slider = lv_event_get_target(e);
    int num = lv_slider_get_value(slider);
    
    to_any_frame(num);
}

// 切换歌曲
static void switch_source(const char *new_path)
{
    
    Mp3PlayStopDec();
    // 更新总时长
    duration = get_duration(new_path);
    char buf[32] = {0};
    duration_time(buf, duration);
    lv_label_set_text(dura_label, buf);
    lv_label_set_text(path_label, new_path);

    mp3_codec(new_path);
    
}
// 播放器控制事件
static void audio_control_event_cb(lv_event_t *e)
{
    int user_data = (int)lv_event_get_user_data(e);
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED)
    {
        static bool pause = false;
        switch (user_data)
        {
        case 1:
            curr_node = curr_node->prev;
            switch_source(curr_node->path);   
            lv_label_set_text(PAUSE_btn, LV_SYMBOL_PAUSE);       
            break;
        case 2:
            
            if(!pause){
                mp3_pause();
                pause = true;
                lv_label_set_text(PAUSE_btn, LV_SYMBOL_PLAY);
                // printf("pause\n");
            }
            else {
                mp3_resume();
                pause = false;
                lv_label_set_text(PAUSE_btn, LV_SYMBOL_PAUSE);
                // printf("resume\n");
            }    
            break;
        case 3:
            curr_node = curr_node->next;
            switch_source(curr_node->path); 
            lv_label_set_text(PAUSE_btn, LV_SYMBOL_PAUSE);
            break;
        
        default:
            break;
        }
    }
}

// 音频播放首页退出事件
static void exit_event_cb(lv_event_t *e)
{
    destroyList(&au_head);      // 销毁音频路径链表
    
    // printf("PlayStop  destroyList\n");
}
// 把时长转换成分：秒形式
static void duration_time(char buf[], int dura)
{
    uint8_t sec = dura % 60;
    uint8_t min = dura / 60;
    sprintf(buf, "%02d:%02d", min, sec);
}

// 更新进度定时器
static void update_task(lv_timer_t *timer)
{
    if(Mp3Playing)
    {
        // 获取时间
        int time = get_current_sec();
             
        // 更新当前时间标签
        char buf[32] = {0};
        duration_time(buf, time);
        lv_label_set_text(curr_label, buf);
        // 更新进度条
        lv_slider_set_value(progress_slider, time*1.0 / duration*1.0 * 100, LV_ANIM_ON);
    }
    
}

// 播放器界面退出事件
static void page_exit_event_cb(lv_event_t *e)
{
    if(update_timer != NULL){
        lv_timer_del(update_timer); // 删除定时器
    }
        
    Mp3PlayStopDec();           // 关闭播放器
    destroy_frame_list(frame_head); // 销毁帧链表
}
  
// 音频播放界面
static void audio_page(const char *filepath)
{
    duration = 0;
    lv_obj_t *parent = Page_Background_Popup(lv_scr_act(), "音频");
    lv_obj_add_event_cb(parent, page_exit_event_cb, LV_EVENT_DELETE, NULL);
    path_label = card_create_24_text(parent, filepath, 0, 0);

    lv_obj_t *PREV = card_create_24_text_symbol(parent, LV_SYMBOL_PREV, -70, 200); // 上一个
    lv_obj_t *PAUSE = card_create_24_text_symbol(parent, LV_SYMBOL_PAUSE, 0, 200); // 暂停、继续
    lv_obj_t *NEXT = card_create_24_text_symbol(parent, LV_SYMBOL_NEXT, 70, 200);  // 下一个
    lv_obj_add_flag(PREV,  LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_flag(PAUSE, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_flag(NEXT,  LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(PREV,  audio_control_event_cb, LV_EVENT_CLICKED, (void *)1);
    lv_obj_add_event_cb(PAUSE, audio_control_event_cb, LV_EVENT_CLICKED, (void *)2);
    lv_obj_add_event_cb(NEXT,  audio_control_event_cb, LV_EVENT_CLICKED, (void *)3);
    PAUSE_btn = PAUSE;
    // 进度条风格
    static lv_style_t style_main;
    lv_style_init(&style_main);
    lv_style_set_bg_opa(&style_main, LV_OPA_COVER);
    lv_style_set_bg_color(&style_main, lv_color_hex3(0xcccccc));
    lv_style_set_radius(&style_main, LV_RADIUS_CIRCLE);
    lv_style_set_pad_ver(&style_main, -3); /*Makes the indicator larger*/

    static lv_style_t style_indicator;
    lv_style_init(&style_indicator);
    lv_style_set_bg_opa(&style_indicator, LV_OPA_COVER);
    lv_style_set_opa(&style_indicator, LV_OPA_COVER);
    lv_style_set_bg_color(&style_indicator, lv_color_hex(0x5EFFFC));
    lv_style_set_radius(&style_indicator, 7);
    lv_style_set_pad_all(&style_indicator, -8); 

    static lv_style_t style_knob;
    lv_style_init(&style_knob);
    lv_style_set_bg_opa(&style_knob, LV_OPA_COVER);
    lv_style_set_opa(&style_knob, LV_OPA_COVER);
    lv_style_set_bg_color(&style_knob, lv_color_hex(0xFFFFFF));
    lv_style_set_border_width(&style_knob, 0);
    lv_style_set_radius(&style_knob, LV_RADIUS_CIRCLE);
    lv_style_set_pad_all(&style_knob, 12);
    // 进度条
    progress_slider = lv_slider_create(parent);
    lv_obj_remove_style_all(progress_slider);
    lv_obj_set_size(progress_slider, 320, 10);
    lv_slider_set_range(progress_slider, 1, 99);
    lv_obj_align(progress_slider, LV_ALIGN_BOTTOM_MID, 0, -45);
    lv_obj_add_style(progress_slider, &style_main, LV_PART_MAIN);
    lv_obj_add_style(progress_slider, &style_indicator, LV_PART_INDICATOR);
    lv_obj_add_style(progress_slider, &style_knob, LV_PART_KNOB);
    lv_obj_add_event_cb(progress_slider, progress_slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

    // 音量条
    card_create_24_text(parent, "音量", 450, 170);
    lv_obj_t *voice_slider = lv_slider_create(parent);
    lv_obj_set_size(voice_slider, 10, 300);
    lv_obj_align(voice_slider, LV_ALIGN_CENTER, 450, 0);
    lv_slider_set_range(voice_slider, 0, 90);
    lv_slider_set_value(voice_slider, 20, LV_ANIM_OFF);
    lv_obj_add_style(voice_slider, &style_main, LV_PART_MAIN);
    lv_obj_add_style(voice_slider, &style_indicator, LV_PART_INDICATOR);
    lv_obj_add_style(voice_slider, &style_knob, LV_PART_KNOB);
    lv_obj_add_event_cb(voice_slider, lv_Voiceslider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_flag(voice_slider, LV_OBJ_FLAG_ADV_HITTEST);
    

    // 总时长
    duration = get_duration(filepath);
    char buf[32] = {0};
    duration_time(buf, duration);
    dura_label = card_create_20_text(parent, buf, 260, 250);
    card_create_20_text(parent, "/", 230, 250);
    // 当前时长
    curr_label = card_create_20_text(parent, "00:00", 200, 250);

    // 音频解码
    mp3_codec(filepath);
    // 更新进度定时器
    update_timer = lv_timer_create(update_task, 1000, NULL);
}

// 点击文件事件
static void file_event_cb(lv_event_t *e)
{
    //const char *btn_text = lv_list_get_btn_text(lv_obj_get_parent(btn), btn);
    Node *node = (Node *)lv_event_get_user_data(e);
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED)
    {
        printf("filepath %s\n", node->path);
        audio_page(node->path);
        curr_node = node;
    }
}

// 读取path路径下的MP3文件
static void read_audio_file(lv_obj_t *list, const char *path)
{
    DIR *dir = opendir(path);
    if (!dir)
    {
        printf("Error opening folder: %s\n", path);
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        const char *name = entry->d_name;
        if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
        {
            continue;
        }
        if (entry->d_type == DT_REG)
        {
            if((strstr(name, ".mp3") != NULL) || (strstr(name, ".MP3") != NULL))
            {
                // 获取路径添加进链表
                char filepath[128] = {0};
                snprintf(filepath, 128, "%s%s", path, name);
                insertNode(&au_head, filepath);
                Node *node = findNode(au_head, filepath);
                // 添加列表按钮
                lv_obj_t *btn = lv_list_add_btn(list, LV_SYMBOL_FILE, name);
                lv_obj_set_size(btn, lv_pct(100), lv_pct(10));
                lv_obj_set_style_text_font(btn, &fout_16_text, 0);
                lv_obj_set_style_text_color(btn, lv_color_white(), LV_PART_MAIN);
                lv_obj_set_style_radius(btn, 10, LV_PART_MAIN);
                lv_obj_set_style_bg_opa(btn, 60, LV_PART_MAIN);
                lv_obj_set_style_border_width(btn, 0, LV_PART_MAIN);
            
                lv_obj_add_event_cb(btn, file_event_cb, LV_EVENT_CLICKED, (void *)node);
                
            }
        }
    }
    closedir(dir);
}

// 创建列表
static lv_obj_t *create_list(lv_obj_t *parent, lv_coord_t xofs, lv_coord_t yofs)
{
    lv_obj_t *list = lv_list_create(parent);
    lv_obj_set_size(list, 300, 450);
    lv_obj_align(list, LV_ALIGN_CENTER, xofs, yofs);
    lv_obj_set_style_bg_opa(list, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(list, 0, LV_PART_MAIN);
    // lv_obj_set_style_pad_all(list, 5, LV_PART_MAIN);
    return list;
}

// 显示SD卡和U盘
static void audio_create_file(lv_obj_t *parent)
{
    lv_obj_t *list_sdcard = create_list(parent, -300, 30);
    lv_obj_t *list_mnt  = create_list(parent, 0, 30);
    lv_obj_t *list_root = create_list(parent, 300, 30);

    read_audio_file(list_sdcard, "/sdcard/");
    read_audio_file(list_mnt, "/mnt/");
    read_audio_file(list_root, "/root/");
    
}
void AudioCard(void)
{

    lv_obj_t *parent = Page_Background_Popup(lv_scr_act(), "音频");
    lv_obj_add_event_cb(parent, exit_event_cb, LV_EVENT_DELETE, NULL);
    audio_create_file(parent);
    card_create_20_text(parent, "SD卡", -400, -220);
    card_create_20_text(parent, "U盘", -100, -220);
    card_create_20_text(parent, "root", 200, -220);


} 