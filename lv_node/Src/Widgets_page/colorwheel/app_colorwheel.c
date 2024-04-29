/*
 * @Author: Flying
 * @Date: 2022-04-28 21:58:33
 * @LastEditors: Flying
 * @LastEditTime: 2022-04-28 22:01:32
 * @Description: 新建文件
 */
#include "ui_app.h"

int app_colorwheel_1(lv_obj_t *parent);

lv_obj_t *app_colorwheel(lv_obj_t *parent)
{
     lv_obj_t *Widobj = Widget_Commom_BackgroundPage(parent);



    lv_obj_t *list1 = app_common_obj_create("colorwheel example", Widobj);
    app_common_obj_add_item(list1, "颜色选择控件简单演示", app_colorwheel_1);
    return Widobj;
}

/**点击时间图片事件**/
static void add_ColorwheelObj_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED)
    {
        app_colorwheel(lv_scr_act());
    }
}

// 创建开关组件的卡片
void ColorwheelWitgets(lv_obj_t *parent)
{
    // 创建图片OBJ
    lv_obj_t *ColorwheelObj = lv_obj_create(parent);
    lv_obj_set_size(ColorwheelObj, 170, 170);
    lv_obj_align(ColorwheelObj, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_opa(ColorwheelObj, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ColorwheelObj, 0, LV_PART_MAIN); // 边框宽度
    lv_obj_clear_flag(ColorwheelObj, LV_OBJ_FLAG_SCROLLABLE);      // 禁用滚动条

    image_create(ColorwheelObj, &Colorwheel_W, 0, -30);        // 图片
    card_create_20_text(ColorwheelObj, "Colorwheel组件", 0, 50); // 标题

    lv_obj_add_event_cb(ColorwheelObj, add_ColorwheelObj_event_cb, LV_EVENT_CLICKED, NULL); /**点击添加图片事件**/
}
