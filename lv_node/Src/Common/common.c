/*
 * @Description:
 * @Author: mai
 * @LastEditors: chen
 * @Date: 2023-04-25 16:23:42
 * @LastEditTime: 2023-12-18 15:13:21
 * @FilePath: /Demo/lv_node/Src/Common/common.c
 */
#include "ui_app.h"

static lv_style_t style_bg_mask;

static void BackEvent(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    if (code == LV_EVENT_CLICKED)
    {
        lv_obj_del(obj);
    }
}

static void BackParentEvent(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    if (code == LV_EVENT_CLICKED)
    {

        lv_obj_del(obj->parent->parent);
    }
}

static void SetStyleMask(lv_obj_t *obj)
{
    lv_style_init(&style_bg_mask); // 初始化遮蔽罩风格
    lv_style_set_bg_color(&style_bg_mask, lv_color_hex(0x11141A));
    lv_style_set_bg_opa(&style_bg_mask, 150);
    lv_obj_add_style(obj, &style_bg_mask, LV_PART_MAIN);
}

static void get_time(char *buffer, size_t buffer_size)
{
    time_t t = time(NULL);
    struct tm *tmp = localtime(&t);
    snprintf(buffer, buffer_size, "%04d-%02d-%02d   %02d:%02d", tmp->tm_year + 1900, tmp->tm_mon + 1, tmp->tm_mday, tmp->tm_hour, tmp->tm_min);
}

static void lv_get_time(lv_timer_t *timer)
{
    lv_obj_t *obj = timer->user_data;
    static char time_buff[100];
    get_time(time_buff, sizeof(time_buff));
    lv_label_set_text(obj, time_buff);
}

static void BackTimeEvent(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    lv_timer_t *time = lv_event_get_user_data(e);

    if (code == LV_EVENT_CLICKED)
    {
        lv_timer_del(time);
        lv_obj_del(obj->parent->parent);
    }
}

lv_obj_t *card_create(lv_obj_t *parent)
{
    lv_obj_t *card = lv_obj_create(parent);
    lv_obj_set_size(card, 157, 220);
    lv_obj_set_style_bg_opa(card, LV_OPA_40, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(card, lv_color_make(0, 153, 255), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(card, lv_color_make(0, 19, 46), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(card, LV_GRAD_DIR_VER, LV_STATE_DEFAULT);
    lv_obj_set_style_radius(card, 10, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(card, 0, LV_STATE_DEFAULT);
    return card;
}

lv_obj_t *CreateMaskBox(lv_obj_t *parent, bool back)
{
    lv_obj_t *gray_layer = lv_obj_create(parent); // 主页面灰色遮罩层
    lv_obj_remove_style_all(gray_layer);
    lv_obj_set_pos(gray_layer, 0, 0);
    lv_obj_set_size(gray_layer, lv_obj_get_width(parent), lv_obj_get_height(parent));
    SetStyleMask(gray_layer);
    if (back)
    {
        lv_obj_add_event_cb(gray_layer, BackEvent, LV_EVENT_CLICKED, NULL); // 添加返回事件
    }

    return gray_layer;
}

lv_obj_t *create_recv_time(lv_obj_t *parent, const lv_font_t *value, lv_coord_t x, lv_coord_t y)
{
    lv_obj_t *recv_time = lv_label_create(parent);
    static char time_buff[100];
    get_time(time_buff, sizeof(time_buff));
    lv_label_set_text(recv_time, time_buff);
    lv_obj_align(recv_time, LV_ALIGN_CENTER, x, y);
    lv_obj_set_style_text_color(recv_time, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(recv_time, value, LV_STATE_DEFAULT);
    lv_timer_create(lv_get_time, 1000, recv_time);
    return recv_time;
}

lv_timer_t *lv_create_time(lv_obj_t *parent, const lv_font_t *value, lv_coord_t x, lv_coord_t y)
{
    lv_obj_t *recv_time = lv_label_create(parent);
    static char time_buff[100];
    get_time(time_buff, sizeof(time_buff));
    lv_label_set_text(recv_time, time_buff);
    lv_obj_align(recv_time, LV_ALIGN_CENTER, x, y);
    lv_obj_set_style_text_color(recv_time, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(recv_time, value, LV_STATE_DEFAULT);
    lv_timer_t *timer = lv_timer_create(lv_get_time, 1000, recv_time);
    return timer;
}

lv_obj_t *card_create_20_text(lv_obj_t *parent, const char *title, lv_coord_t x, lv_coord_t y)
{
    lv_obj_t *label = lv_label_create(parent);
    lv_label_set_text(label, title);
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(label, &PuHuiTi_Regular_20, LV_STATE_DEFAULT);
    lv_obj_align(label, LV_ALIGN_CENTER, x, y);
    return label;
}

lv_obj_t *card_create_24_text(lv_obj_t *parent, const char *title, lv_coord_t x, lv_coord_t y)
{
    lv_obj_t *label = lv_label_create(parent);
    lv_label_set_text(label, title);
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(label, &fout_24_text, LV_STATE_DEFAULT);
    lv_obj_align(label, LV_ALIGN_CENTER, x, y);
    return label;
}
lv_obj_t *card_create_24_text_symbol(lv_obj_t *parent, const char *title, lv_coord_t x, lv_coord_t y)
{
    lv_obj_t *label = lv_label_create(parent);
    lv_label_set_text(label, title);
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(label, &font_symbol_24, LV_STATE_DEFAULT);
    lv_obj_align(label, LV_ALIGN_CENTER, x, y);
    return label;
}

lv_obj_t *create_label(lv_obj_t *parent, const char *text, lv_coord_t x, lv_coord_t y)
{
    lv_obj_t *label = lv_label_create(parent);
    lv_obj_align(label, LV_ALIGN_LEFT_MID, x, y);
    lv_obj_set_style_text_font(label, &PuHuiTi_Regular_20, LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
    lv_label_set_text(label, text);
    lv_obj_clean(label);
    return label;
}

lv_obj_t *image_create(lv_obj_t *parent, const void *src, lv_coord_t x, lv_coord_t y)
{
    lv_obj_t *img = lv_img_create(parent);
    lv_img_set_src(img, src);
    lv_obj_align(img, LV_ALIGN_CENTER, x, y);
    return img;
}

lv_obj_t *btn_create_text(lv_obj_t *parent, bool on_back, const char *confirm_text, lv_coord_t x_ofs, lv_coord_t y_ofs)
{
    lv_obj_t *confirm_btn = lv_btn_create(parent);
    lv_obj_set_size(confirm_btn, 90, 31);
    lv_obj_align(confirm_btn, LV_ALIGN_TOP_LEFT, x_ofs, y_ofs);
    lv_obj_set_style_radius(confirm_btn, 5, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(confirm_btn, lv_color_hex(0x14e7f0), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(confirm_btn, lv_color_hex(0x2d6af2), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(confirm_btn, LV_GRAD_DIR_VER, LV_STATE_DEFAULT);
    lv_obj_t *confirm_label = lv_label_create(confirm_btn);
    lv_label_set_text(confirm_label, confirm_text);
    lv_obj_set_style_text_color(confirm_label, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(confirm_label, &PuHuiTi_Regular_20, LV_STATE_DEFAULT);
    lv_obj_align(confirm_label, LV_ALIGN_CENTER, 0, 0);
    if (on_back) 
    {
        lv_obj_add_event_cb(confirm_btn, BackParentEvent, LV_EVENT_CLICKED, NULL);
    }
    return confirm_btn;
}

lv_obj_t* lv_Port_Type(lv_obj_t *parent, const char *options, lv_coord_t x_ofs, lv_coord_t y_ofs)
{
    lv_obj_t *dd = lv_dropdown_create(parent);
    lv_dropdown_set_options(dd, options);
    lv_obj_set_size(dd, 120, 40);
    lv_obj_align(dd, LV_ALIGN_TOP_MID, x_ofs, y_ofs);
    lv_obj_set_style_radius(dd, 5, LV_PART_MAIN);
    lv_obj_set_style_border_color(dd, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_set_style_border_width(dd, 0, LV_PART_MAIN);
    lv_obj_set_style_text_font(dd, &fout_16_text, LV_PART_MAIN);
    lv_obj_set_style_text_color(dd, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_set_style_bg_color(dd, lv_color_make(20, 26, 35), LV_PART_MAIN);
    lv_dropdown_set_symbol(dd, LV_SYMBOL_DOWN);
    lv_obj_set_style_text_color(dd, lv_color_hex(0xffffff), LV_PART_SELECTED);

    lv_obj_t *list = lv_dropdown_get_list(dd);
    lv_obj_set_style_radius(list, 5, LV_PART_MAIN);
    lv_obj_set_style_text_font(list, &fout_16_text, LV_PART_MAIN);
    lv_obj_set_style_bg_color(list, lv_color_hex(0x141A23), LV_PART_MAIN);
    lv_obj_set_style_border_width(list, 0, LV_PART_MAIN);
    lv_obj_set_style_text_color(list, lv_color_hex(0xffffff), LV_PART_MAIN);
    return dd;
}

lv_obj_t *create_page_back(lv_obj_t *obj, lv_timer_t *time)
{
    lv_obj_t *lv_obj = lv_obj_create(obj);
    lv_obj_set_size(lv_obj, 90, 90);
    lv_obj_add_flag(lv_obj, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_align(lv_obj, LV_ALIGN_TOP_LEFT, 10, 5);
    lv_obj_set_style_bg_opa(lv_obj, 0, LV_STATE_DEFAULT); 
    lv_obj_set_style_border_width(lv_obj, 0, LV_PART_MAIN);
    lv_obj_add_event_cb(lv_obj, BackTimeEvent, LV_EVENT_CLICKED, time); 
    lv_obj_t *back_btn = lv_label_create(lv_obj);
    lv_label_set_text(back_btn, LV_SYMBOL_LEFT);
    lv_obj_align(back_btn, LV_ALIGN_CENTER, 0, -20);
    lv_obj_set_style_text_color(back_btn, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
}

lv_obj_t *Page_Background_Popup(lv_obj_t *parent, const char *title)
{
    lv_obj_t *obj = lv_obj_create(parent);
    lv_obj_remove_style_all(obj);
    lv_obj_set_size(obj, 1024, 600);

    lv_obj_t *Page_Popup = image_create(obj, &Page_Background, 0, 0);
    lv_obj_t *PageTitleImg = image_create(Page_Popup, &Page_Title, 0, -285);
    card_create_24_text(PageTitleImg, title, 0, 0);

    lv_timer_t *time = lv_create_time(Page_Popup, &PuHuiTi_Regular_20, 400, -270); 
    create_page_back(Page_Popup, time);
    return Page_Popup;
}

// 删除上一级事件
void lv_back_event(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    if (code == LV_EVENT_CLICKED)
    {
        lv_obj_del(obj);
    }
}

static void lv_common_style_set_style_mask(lv_obj_t *obj)
{
    lv_style_init(&style_bg_mask); // 初始化遮蔽罩风格
    lv_style_set_bg_color(&style_bg_mask, lv_color_hex(0x11141A));
    lv_style_set_bg_opa(&style_bg_mask, 200);
    lv_obj_add_style(obj, &style_bg_mask, LV_PART_MAIN);
}

// 创建遮罩层
lv_obj_t *lv_common_create_mask_box(lv_obj_t *parent, bool back)
{
    lv_obj_t *gray_layer = lv_obj_create(parent); // 主页面灰色遮罩层
    lv_obj_remove_style_all(gray_layer);
    lv_obj_set_pos(gray_layer, 0, 0);
    lv_obj_set_size(gray_layer, lv_obj_get_width(parent), lv_obj_get_height(parent));
    lv_common_style_set_style_mask(gray_layer);
    if (back)
    {
        lv_obj_add_event_cb(gray_layer, lv_back_event, LV_EVENT_CLICKED, NULL); // 添加返回事件
    }

    return gray_layer;
}

//创建一个弹出框
lv_obj_t *create_popup(lv_obj_t *parent, bool card_back, const char *title, const char *confirm_text, const char *back_text, const char *text, lv_event_cb_t event_cb, void *user_data)
{
    lv_obj_t *mask = lv_common_create_mask_box(parent, card_back); // 创建遮罩
    lv_obj_t *popup = lv_obj_create(mask);
    lv_obj_set_size(popup,581,358);
    lv_obj_align(popup,LV_ALIGN_CENTER,0,0);
    lv_obj_set_style_bg_color(popup, lv_color_hex(0x252c39), 0);
    lv_obj_set_style_border_width(popup, 0, LV_PART_MAIN);

    lv_obj_t *label = lv_label_create(popup);                 // 创建标题
    lv_label_set_text(label, title);
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(label, &PuHuiTi_Regular_30, LV_STATE_DEFAULT);
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 0);

    card_create_24_text(popup, text, 0, 0); // 创建文字内容
 
    if(back_text == NULL)
    {
        lv_obj_t *confirm_btn = btn_create_text(popup, true, confirm_text, 230, 265); // 创建按钮
    }
    else
    {
        lv_obj_t *confirm_btn = btn_create_text(popup, false, confirm_text, 155, 265); // 创建按钮
        lv_obj_add_event_cb(confirm_btn, event_cb, LV_EVENT_CLICKED, user_data);       // 添加返回事件
        btn_create_text(popup, true, back_text, 305, 265);                             // 创建按钮
    }
    
    return popup;
}
