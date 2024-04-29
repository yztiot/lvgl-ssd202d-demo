/*
 * @Description: 
 * @Author: chen
 * @Date: 2023-11-27 11:27:46
 * @LastEditors: chen
 * @LastEditTime: 2023-12-26 10:26:41
 */
#include "ui_app.h"
#include <dirent.h> 
#include <sys/stat.h>

lv_obj_t *control_page = NULL;
static char curr_path[128] = {0};

void exit_video_page()
{
    lv_obj_set_style_opa(lv_scr_act(), LV_OPA_100, 0);
    if(control_page != NULL){
        lv_obj_del(control_page);
        control_page = NULL;
    }
    
    
}

// 长按或点击 
static void parent_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_LONG_PRESSED)
    {
        printf("longpress------------\n");
        player_working = false;
        
    }
    else if(code == LV_EVENT_CLICKED)
    {
        player_control = 1;
    }
}
// 快进或快退
static void control_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED)
    {
        player_control = (int)lv_event_get_user_data(e);
    }
}

static lv_obj_t *control_obj(lv_obj_t *parent, lv_align_t align, const char *text)
{
    lv_obj_t *obj = lv_obj_create(parent);
    lv_obj_set_size(obj, 400, 400);
    lv_obj_align(obj, align, 0, 0);
    lv_obj_set_style_bg_color(obj, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(obj, LV_OPA_30, LV_PART_MAIN);
    card_create_24_text(obj, text, 0, 0);
    return obj;
}

// 视频播放
static void video_event_cb(lv_event_t *e)
{
    const char *text = (const char *)lv_event_get_user_data(e);
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED)
    {
        // 控制组件
        control_page = lv_obj_create(lv_scr_act());
        lv_obj_remove_style_all(control_page);
        lv_obj_set_size(control_page, 1024, 600);
        lv_obj_add_event_cb(control_page, parent_event_cb, LV_EVENT_ALL, NULL);

        lv_obj_t *left_btn = control_obj(control_page, LV_ALIGN_LEFT_MID, "快退2秒");
        lv_obj_t *right_btn = control_obj(control_page, LV_ALIGN_RIGHT_MID, "快进2秒");
        lv_obj_add_event_cb(left_btn, control_event_cb, LV_EVENT_ALL, (void *)2);
        lv_obj_add_event_cb(right_btn, control_event_cb, LV_EVENT_ALL, (void *)3);

        // 隐藏ui界面
        lv_obj_t *scr_obj = lv_scr_act();
        lv_obj_set_style_opa(scr_obj, 0, 0);
        
        // 开启播放器线程
        pthread_t mm_thread;
        pthread_create(&mm_thread, NULL, player_thread, (void *)text);
        pthread_detach(mm_thread);
    }
}

// 创建视频播放界面
void video_page(const char *text)
{
    lv_obj_t *video_play_page = Page_Background_Popup(lv_scr_act(), "视频");
    card_create_24_text(video_play_page, "单机暂停\n长按退出", 0, 0);
    card_create_24_text(video_play_page, text, 0, -200);
    lv_obj_t *video_btn = btn_create_text(video_play_page, false, "播放", 0, 0);
    lv_obj_align(video_btn, LV_ALIGN_BOTTOM_MID, 0, -50);
    lv_obj_add_event_cb(video_btn, video_event_cb, LV_EVENT_CLICKED, (void *)text);
    
    control_obj(video_play_page, LV_ALIGN_LEFT_MID, "快退2秒");
    control_obj(video_play_page, LV_ALIGN_RIGHT_MID, "快进2秒");

    
}

static void file_event_cb(lv_event_t *e)
{
    lv_obj_t *btn = lv_event_get_target(e);
    const char *btn_text = lv_list_get_btn_text(lv_obj_get_parent(btn), btn);
    const char *path = lv_event_get_user_data(e);
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED)
    {
        if((strstr(btn_text, ".mp4") != NULL) || (strstr(btn_text, ".MP4") != NULL) || (strstr(btn_text, ".avi") != NULL))
        {
         
            bzero(curr_path, 128);
            snprintf(curr_path, 128, "%s%s", path, btn_text);
            printf("filepath %s\n", curr_path);      
            video_page(curr_path);
            // lv_obj_t * player = lv_ffmpeg_player_create(lv_obj_get_parent(lv_obj_get_parent(btn)));
            // lv_obj_set_size(player, 512, 300);
            // lv_ffmpeg_player_set_src(player, curr_path);
            // lv_ffmpeg_player_set_cmd(player, LV_FFMPEG_PLAYER_CMD_START);
            // lv_obj_center(player);
        }
        
    }
}
// 读取路径的视频文件
static void read_video_file(lv_obj_t *list, const char *path)
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
            if((strstr(name, ".mp4") != NULL) || (strstr(name, ".MP4") != NULL) || (strstr(name, ".avi") != NULL))
            {
                // 添加列表按钮
                lv_obj_t *btn = lv_list_add_btn(list, LV_SYMBOL_FILE, name);
                lv_obj_set_size(btn, lv_pct(100), lv_pct(10));
                lv_obj_set_style_text_font(btn, &fout_16_text, 0);
                lv_obj_set_style_text_color(btn, lv_color_white(), LV_PART_MAIN);
                lv_obj_set_style_radius(btn, 10, LV_PART_MAIN);
                lv_obj_set_style_bg_opa(btn, 60, LV_PART_MAIN);
                lv_obj_set_style_border_width(btn, 0, LV_PART_MAIN);
            
                lv_obj_add_event_cb(btn, file_event_cb, LV_EVENT_CLICKED, (void *)path);
            }
            
        }
    }
    closedir(dir);
}

static lv_obj_t *create_list(lv_obj_t *parent, lv_coord_t xofs, lv_coord_t yofs)
{
    lv_obj_t *list = lv_list_create(parent);
    lv_obj_set_size(list, 300, 450);
    lv_obj_align(list, LV_ALIGN_CENTER, xofs, yofs);
    lv_obj_set_style_bg_opa(list, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(list, 0, LV_PART_MAIN);
    return list;
}

// 显示SD卡和U盘
static void video_create_file(lv_obj_t *parent)
{
    lv_obj_t *list_sdcard = create_list(parent, -300, 30);
    lv_obj_t *list_mnt  = create_list(parent, 0, 30);
    lv_obj_t *list_root = create_list(parent, 300, 30);

    read_video_file(list_sdcard, "/sdcard/");
    read_video_file(list_mnt, "/mnt/");
    read_video_file(list_root, "/root/");
}

void VideoCard(void)
{
    lv_obj_t *parent = Page_Background_Popup(lv_scr_act(), "视频");
    video_create_file(parent);
    card_create_20_text(parent, "SD卡", -400, -220);
    card_create_20_text(parent, "U盘", -100, -220);
    card_create_20_text(parent, "root", 200, -220);

 
    
}