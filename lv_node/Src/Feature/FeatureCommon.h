/*
 * @Description: 
 * @Author: chen
 * @Date: 2023-11-30 17:31:52
 * @LastEditors: chen
 * @LastEditTime: 2023-12-19 10:46:50
 */
/***
 * @Description:
 * @Author: mai
 * @LastEditors: mai
 * @Date: 2023-07-27 13:54:43
 * @LastEditTime: 2023-07-27 14:25:30
 * @FilePath: /Demo/lv_node/Src/Feature/FeatureCommon.h
 */
#ifndef LV_F_COMMON_H
#define LV_F_COMMON_H

void update_area(lv_obj_t *obj, const char *msg_buffer);
void execute_command(const char *command, char **buffer);

lv_obj_t *CreatrWifiList(lv_obj_t *parent, int h, int y);
lv_obj_t *CreadWifiPasswordTextarea(lv_obj_t *SrcParent);
lv_obj_t *create_switch(lv_obj_t *parent, lv_event_cb_t *event_cb);
lv_obj_t *lv_radiobutton_create(lv_obj_t *parent, const char *txt);
lv_obj_t *CreateTextBox(lv_obj_t *parent, lv_coord_t x_ofs, lv_coord_t y_ofs);
lv_obj_t *CreateBigButton(lv_obj_t *parent, const char *confirm_text, lv_coord_t x_ofs, lv_coord_t y_ofs, lv_event_cb_t event_cb, void *user_data);
lv_obj_t *CreateGpioAndI2cButton(lv_obj_t *parent, const char *confirm_text, lv_coord_t x_ofs, lv_coord_t y_ofs, lv_event_cb_t event_cb, void *user_data);
lv_obj_t *CreateWifiButton(lv_obj_t *parent, const char *confirm_text, lv_coord_t x_ofs, lv_coord_t y_ofs, lv_event_cb_t event_cb, void *user_data);
lv_obj_t *CreateBackButtonAndText(lv_obj_t *parent, const char *confirm_text, lv_coord_t x_ofs, lv_coord_t y_ofs, lv_event_cb_t event_cb, void *user_data);

lv_obj_t *CreatrWifiListStaticButton(lv_obj_t *parent);
lv_obj_t *create_wifi_page(lv_obj_t *parent);
void create_loding_timer();
void del_loding_timer();
#endif