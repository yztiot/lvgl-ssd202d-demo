
/***
 * @Description:通用组件
 * @Author: mai
 * @LastEditors: mai
 * @Date: 2023-04-25 16:23:46
 * @LastEditTime: 2023-04-25 16:31:10
 * @FilePath: /SmartBox_Power/lv_node/src/common/common.h
 */
#ifndef _LV_COMMON_H
#define _LV_COMMON_H

lv_obj_t *card_create(lv_obj_t *parent);
lv_obj_t *CreateMaskBox(lv_obj_t *parent, bool back);
lv_obj_t *create_recv_time(lv_obj_t *parent, const lv_font_t *value, lv_coord_t x, lv_coord_t y);
lv_timer_t *lv_create_time(lv_obj_t *parent, const lv_font_t *value, lv_coord_t x, lv_coord_t y);
lv_obj_t *card_create_20_text(lv_obj_t *parent, const char *title, lv_coord_t x, lv_coord_t y);
lv_obj_t *card_create_24_text(lv_obj_t *parent, const char *title, lv_coord_t x, lv_coord_t y);
lv_obj_t *card_create_24_text_symbol(lv_obj_t *parent, const char *title, lv_coord_t x, lv_coord_t y);
lv_obj_t *create_label(lv_obj_t *parent, const char *text, lv_coord_t x, lv_coord_t y);
lv_obj_t *image_create(lv_obj_t *parent, const void *src, lv_coord_t x, lv_coord_t y);
lv_obj_t *btn_create_text(lv_obj_t *parent, bool on_back, const char *confirm_text, lv_coord_t x_ofs, lv_coord_t y_ofs);
lv_obj_t *lv_Port_Type(lv_obj_t *parent, const char *options, lv_coord_t x_ofs, lv_coord_t y_ofs);
lv_obj_t *create_page_back(lv_obj_t *obj, lv_timer_t *time);
lv_obj_t *Page_Background_Popup(lv_obj_t *parent, const char *title);
void lv_back_event(lv_event_t *e);
lv_obj_t *lv_common_create_mask_box(lv_obj_t *parent, bool back);
lv_obj_t *create_popup(lv_obj_t *parent, bool card_back, const char *title, const char *confirm_text, const char *back_text, const char *text, lv_event_cb_t event_cb, void *user_data);

#endif