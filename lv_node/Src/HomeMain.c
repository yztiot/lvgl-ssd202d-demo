/*
 * @Description:
 * @Author: mai
 * @LastEditors: chen
 * @Date: 2023-07-27 11:09:27
 * @LastEditTime: 2023-12-06 10:22:23
 * @FilePath: /Demo/lv_node/Src/HomeMain.c
 */
// 创建添加添加的设备
#include "ui_app.h"

static void enter_event_cb(lv_event_t *e)
{
    lv_obj_t *obj = lv_event_get_target(e);
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED)
    {
        lv_obj_del(obj);
    }
}

void lv_display_page(lv_obj_t *parent)
{
    lv_obj_t *lv_background = lv_img_create(parent); // 背景页
    lv_img_set_src(lv_background, &background);
    lv_obj_add_flag(lv_background, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(lv_background, enter_event_cb, LV_EVENT_CLICKED, NULL);
    
    lv_obj_t *home_page = lv_img_create(lv_background);
    lv_img_set_src(home_page, &homePage);
    lv_obj_align(home_page, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t *home_card = lv_img_create(lv_background);
    lv_img_set_src(home_card, &homecard);
    lv_obj_align(home_card, LV_ALIGN_CENTER, 130, 55);
    lv_img_set_zoom(home_card, 200);

    card_create_24_text(lv_background, "点击进入系统", 0, 260);
}
