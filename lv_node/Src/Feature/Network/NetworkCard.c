/*
 * @Description:网络
 * @Author: lzs
 * @LastEditors: mai
 * @Date: 2023-03-09 11:28:06
 * @LastEditTime: 2023-07-27 19:32:28
 * @FilePath: /Demo/lv_node/Src/Feature/NetworkCard.c
 */

#include "ui_app.h"

static char *Network_buffer = NULL;      // 网络信息缓冲区
static char *Connectivity_buffer = NULL; // 连通测试缓冲区
static lv_timer_t *Connectivitytimer;    // 连通测试定时器
static pthread_t ping_thread;            // 连通测试线程



// 获取网络信息
static void SendBtn_Event_Handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_user_data(e);
    if (code == LV_EVENT_CLICKED)
    {
        // 执行ifconfig命令并将输出刷到Network_buffer中
        execute_command("ifconfig", &Network_buffer);

        // 更新区域显示
        update_area(obj, Network_buffer);

        // 释放动态分配的内存
        if (Network_buffer != NULL) 
        {
            free(Network_buffer);
        }
    }
}

static void refresh_text_area(lv_timer_t *timer)
{
    lv_obj_t *obj = (lv_obj_t *)timer->user_data;
    if (Connectivity_buffer != NULL)
    {
        // 更新文本框内容
        update_area(obj, Connectivity_buffer);
    }
}
// 获取连通性信息
static void *ping_thread_function(void *arg)
{
    // 执行ping命令并将输出刷到Connectivity_buffer中
    execute_command("ping -c 5 8.8.8.8", &Connectivity_buffer);

    return NULL;
}

// 获取连通性信息
static void Connectivity_Event_Handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *parent = lv_event_get_target(e);
    lv_obj_t *obj = lv_event_get_user_data(e);
    if (code == LV_EVENT_CLICKED)
    {
        lv_obj_t *Childbutton = lv_obj_get_child(parent, 0);
        if (strcmp(lv_label_get_text(Childbutton), "连通测试") == 0)
        {
            lv_label_set_text(Childbutton, "停止测试");
            lv_obj_set_style_bg_color(parent, lv_color_hex(0xE33C64), LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_color(parent, lv_color_hex(0xE33C64), LV_STATE_DEFAULT);
            if (Connectivity_buffer != NULL)
            {
                free(Connectivity_buffer);
                Connectivity_buffer = NULL;
            }

            if (pthread_create(&ping_thread, NULL, ping_thread_function, NULL) != 0)
            {
                printf("线程创建失败\n");
                return;
            }
            Connectivitytimer = lv_timer_create(refresh_text_area, 1000, obj);
        }
        else if (strcmp(lv_label_get_text(Childbutton), "停止测试") == 0)
        {
            lv_label_set_text(Childbutton, "连通测试");
            lv_obj_set_style_bg_color(parent, lv_color_hex(0x14e7f0), LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_color(parent, lv_color_hex(0x2d6af2), LV_STATE_DEFAULT);
            if (Connectivity_buffer != NULL)
            {
                free(Connectivity_buffer);
                Connectivity_buffer = NULL;
            }
            lv_timer_del(Connectivitytimer);
            pthread_cancel(ping_thread);
        }
    }
}



// 创建网口页面
void NetworkCard(void)
{

    lv_obj_t *parent = Page_Background_Popup(lv_scr_act(), "网络");
    lv_obj_t *Network_information = CreateTextBox(parent, 100, 60); //
    lv_obj_t *Connectivity_test = CreateTextBox(parent, 100, 330);
    CreateBigButton(parent, "连通测试", 40, 400, Connectivity_Event_Handler, Connectivity_test);
    CreateBigButton(parent, "获取网络信息", 40, 150, SendBtn_Event_Handler, Network_information);

}
