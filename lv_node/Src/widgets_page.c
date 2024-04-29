/*
 * @Description:第二页组件区页面
 * @Author: lzs
 * @LastEditors: mai
 * @Date: 2023-06-28 16:41:06
 * @LastEditTime: 2023-07-28 16:27:42
 * @FilePath: /Demo/lv_node/Src/widgets_page.c
 */

#include "ui_app.h"

static const widget_info_t WIDGETS_INFO[] = {
    {40, 70, SwitchWitgets},   // 位置，大小，开关组件
    {250, 70, GameWitgets},  // 位置，大小，游戏
    {460, 70, ColorwheelWitgets},  // 位置，大小，色轮组件
    {670, 70, CheckboxWitgets},  // 位置，大小，复选框组件
    {40, 300, ListWitgets},  // 位置，大小，列表组件
    {250, 300, RollerWitgets}, // 位置，大小，滚轮条组件
    {460, 300, SliderWitgets}, // 位置，大小，滑动条组件
    {670, 300, CalendarWitgets}  // 位置，大小，日历组件
};

void *lv_WidgetsPage_page(lv_obj_t *parent)
{

    for (int i = 0; i < sizeof(WIDGETS_INFO) / sizeof(WIDGETS_INFO[0]); i++)
    {
        lv_obj_t *WidgetsObj = lv_obj_create(parent); 
        lv_obj_set_size(WidgetsObj, 200, 200);
        lv_obj_set_pos(WidgetsObj, WIDGETS_INFO[i].x_pos, WIDGETS_INFO[i].y_pos);
        WIDGETS_INFO[i].widgets_func(WidgetsObj);
        lv_obj_set_style_bg_opa(WidgetsObj, 0, LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(WidgetsObj, 0, LV_PART_MAIN); 
        lv_obj_clear_flag(WidgetsObj, LV_OBJ_FLAG_SCROLLABLE); 
        
    }
    create_recv_time(parent, &PuHuiTi_Regular_20, 350, -280);
}
