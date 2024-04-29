/***
 * @Description:
 * @Author: mai
 * @LastEditors: mai
 * @Date: 2023-03-09 11:28:00
 * @LastEditTime: 2023-04-25 20:48:39
 * @FilePath: /SmartBox_Power/lv_node/src/control/control.h
 */
#ifndef _LV_FEATURE_H
#define _LV_FEATURE_H

typedef struct {
    int x_pos;
    int y_pos;
    void (*card_func)(void);  // Update the parameter name from "card" to "parent"
    const lv_img_dsc_t *image;  // Add a pointer to lv_img_dsc_t for image association
} card_info_t;


void lv_feature_page(lv_obj_t *parent);

#endif