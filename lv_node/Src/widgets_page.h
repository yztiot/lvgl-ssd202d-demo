/*
 * @Description:第二页组件区页面
 * @Author: lzs
 * @LastEditors: lzs
 * @Date: 2023-06-28 16:41:06
 * @LastEditTime: 2023-06-28 16:41:06
 * @FilePath: lv_node/src/widgets_page/widget.h
 */

#ifndef _LV_WIDGETSPAGE_H
#define _LV_WIDGETSPAGE_H

typedef struct {
    int x_pos;
    int y_pos;
    void (*widgets_func)(lv_obj_t *WidgetsObj);
} widget_info_t;

void *lv_WidgetsPage_page(lv_obj_t *parent);


#endif



