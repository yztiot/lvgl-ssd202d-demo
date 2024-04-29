/*
 * @Author: Flying
 * @Date: 2022-05-01 22:38:00
 * @LastEditors: Flying
 * @LastEditTime: 2022-05-01 22:41:04
 * @Description: 新建文件
 */
#include "ui_app.h"

int app_roller_1(lv_obj_t *parent);
int app_roller_2(lv_obj_t *parent);
int app_roller_3(lv_obj_t *parent);

lv_obj_t *app_roller(lv_obj_t *parent)
{
    lv_obj_t *Widobj = Widget_Commom_BackgroundPage(parent);
    lv_obj_t *list1 = app_common_obj_create("roller example", Widobj);
    app_common_obj_add_item(list1, "无线滚动roller演示", app_roller_1);
    app_common_obj_add_item(list1, "左中右 roller演示", app_roller_2);
    app_common_obj_add_item(list1, "渐变色roller演示", app_roller_3);
    return Widobj;
}

/**点击时间图片事件**/
static void add_RollerObj_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED)
    {
        app_roller(lv_scr_act());
    }
}

// 创建开关组件的卡片
void RollerWitgets(lv_obj_t *parent)
{
    // 创建图片OBJ
    lv_obj_t *RollerObj = lv_obj_create(parent);
    lv_obj_set_size(RollerObj, 170, 170);
    lv_obj_align(RollerObj, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_opa(RollerObj, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(RollerObj, 0, LV_PART_MAIN); // 边框宽度
    lv_obj_clear_flag(RollerObj, LV_OBJ_FLAG_SCROLLABLE);      // 禁用滚动条

    image_create(RollerObj, &Roller_W, 0, -30);          // 图片
    card_create_20_text(RollerObj, "Roller组件", 0, 50); // 标题

    lv_obj_add_event_cb(RollerObj, add_RollerObj_event_cb, LV_EVENT_CLICKED, NULL); /**点击添加图片事件**/
}