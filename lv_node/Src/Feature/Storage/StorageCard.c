/*
 * @Description:
 * @Author: mai
 * @LastEditors: chen
 * @Date: 2023-07-27 16:59:13
 * @LastEditTime: 2023-12-06 10:33:29
 * @FilePath: /Demo/lv_node/Src/Feature/StorageCard.c
 */
#include "ui_app.h"

#include <dirent.h> 
#include <sys/stat.h>

#define MAX_FILE_NAME_LENGTH 256
char current_folder_path[MAX_FILE_NAME_LENGTH] = {0};
static void show_folder_contents(lv_obj_t *list, const char *folder_path);
static void list_btn_event_cb(lv_event_t *e);

// 裁剪当前路径字符串
static void remove_last_two_slashes(char *str) 
{
    int len = strlen(str);
    int i, j;
    // 找到倒数第二个斜杠
    for (i = len - 2; i >= 0; i--) 
    {
        if (str[i] == '/') {
            break;
        }
    }
    // 删除斜杠之后的所有内容
    for (j = i + 1; j < len; j++) {
        str[j] = '\0';
    }
}

static bool is_directory(const char *path)
{
    struct stat path_stat;
    if (stat(path, &path_stat) == 0)
    {
        return S_ISDIR(path_stat.st_mode);
    }
    return false;
}

static void show_folder_contents(lv_obj_t *list, const char *folder_path)
{   
    lv_obj_t *parent = list->parent;
    lv_obj_del(list);
    lv_obj_t *list1 = lv_list_create(parent);
    lv_obj_set_size(list1, 800, 500);
    lv_obj_align(list1, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t *btn_back = lv_list_add_btn(list1, LV_SYMBOL_LEFT, "..");
    lv_obj_add_event_cb(btn_back, list_btn_event_cb, LV_EVENT_CLICKED, NULL);

 
    DIR *dir = opendir(folder_path);
    if (!dir)
    {
        printf("Error opening folder: %s\n", folder_path);
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
        if (entry->d_type == DT_DIR)
        {
            lv_obj_t *btn = lv_list_add_btn(list1, LV_SYMBOL_DIRECTORY, name);
            lv_obj_set_style_text_font(btn, &fout_16_text, 0);
            lv_obj_add_event_cb(btn, list_btn_event_cb, LV_EVENT_CLICKED, NULL);
        }
        else if (entry->d_type == DT_REG)
        {
            lv_obj_t *btn = lv_list_add_btn(list1, LV_SYMBOL_FILE, name);
            lv_obj_set_style_text_font(btn, &fout_16_text, 0);
            
        }
    }
    closedir(dir);
}

static void list_btn_event_cb(lv_event_t *e)
{
    lv_obj_t *btn = lv_event_get_target(e);
    const char *btn_text = lv_list_get_btn_text(lv_obj_get_parent(btn), btn);
    // 返回上一个路径
    if(strncmp(btn_text, "..", 2) == 0)
    {
        // 不等于根目录
        if(strcmp(current_folder_path, "/") != 0)
        {
            remove_last_two_slashes(current_folder_path);
        }
    }
    else
    {
        // 拼接新路径
        strcat(current_folder_path, btn_text);
        strcat(current_folder_path, "/");
    }
    
    if (is_directory(current_folder_path))
    {
        show_folder_contents(lv_obj_get_parent(btn), current_folder_path);
    }

    
}

void StorageCard(void)
{
    lv_obj_t *parent = Page_Background_Popup(lv_scr_act(), "存储设备");

    lv_obj_t *list = lv_list_create(parent);
    lv_obj_set_size(list, 800, 500);
    lv_obj_align(list, LV_ALIGN_CENTER, 0, 0);
    
    bzero(current_folder_path, sizeof(current_folder_path));
    strcpy(current_folder_path, "/");
    show_folder_contents(list, current_folder_path);
}
