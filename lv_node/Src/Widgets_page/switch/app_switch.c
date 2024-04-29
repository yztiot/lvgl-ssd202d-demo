/*
 * @Author: Flying
 * @Date: 2022-05-01 22:51:10
 * @LastEditors: Flying
 * @LastEditTime: 2022-05-01 22:51:48
 * @Description: 新建文件
 */
#include "ui_app.h"

int app_switch_1(lv_obj_t *parent);

lv_obj_t *app_switch(lv_obj_t *parent)
{
    lv_obj_t *Widobj = Widget_Commom_BackgroundPage(parent);

    lv_obj_t *list1 = app_common_obj_create("switch example", Widobj);
    app_common_obj_add_item(list1, "switch 简单演示", app_switch_1);

    return Widobj;
}

/**点击时间图片事件**/
static void add_SwitchObj_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED)
    {
        app_switch(lv_scr_act());
    }
}

// 创建开关组件的卡片
void SwitchWitgets(lv_obj_t *parent)
{
    // 创建图片OBJ
    lv_obj_t *SwitchObj = lv_obj_create(parent);
    lv_obj_set_size(SwitchObj, 170, 170);
    lv_obj_align(SwitchObj, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_opa(SwitchObj, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(SwitchObj, 0, LV_PART_MAIN); // 边框宽度
    lv_obj_clear_flag(SwitchObj, LV_OBJ_FLAG_SCROLLABLE);      // 禁用滚动条

    image_create(SwitchObj, &Switch_W, 0, -30);          // 图片
    card_create_20_text(SwitchObj, "Switch组件", 0, 50); // 标题

    lv_obj_add_event_cb(SwitchObj, add_SwitchObj_event_cb, LV_EVENT_CLICKED, NULL); /**点击添加图片事件**/
}
