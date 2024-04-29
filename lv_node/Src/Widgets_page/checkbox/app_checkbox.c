/*
 * @Author: Flying
 * @Date: 2022-04-28 21:52:22
 * @LastEditors: Flying
 * @LastEditTime: 2022-04-28 21:56:21
 * @Description: 新建文件
 */
#include "ui_app.h"

int app_checkbox_1(lv_obj_t *parent);
int app_checkbox_2(lv_obj_t *parent);



lv_obj_t *app_checkbox(lv_obj_t *parent)
{
    lv_obj_t *Widobj = Widget_Commom_BackgroundPage(parent);

    lv_obj_t *list1 = app_common_obj_create("checkbox example", Widobj);
    app_common_obj_add_item(list1, "多选框", app_checkbox_1);
    app_common_obj_add_item(list1, "单选框", app_checkbox_2);
    return Widobj;
}

/**点击时间图片事件**/
static void add_CheckboxObj_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED)
    {
        app_checkbox(lv_scr_act());
    }
}

// 创建开关组件的卡片
void CheckboxWitgets(lv_obj_t *parent)
{
    // 创建图片OBJ
    lv_obj_t *CheckboxObj = lv_obj_create(parent);
    lv_obj_set_size(CheckboxObj, 170, 170);
    lv_obj_align(CheckboxObj, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_opa(CheckboxObj, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(CheckboxObj, 0, LV_PART_MAIN); // 边框宽度
    lv_obj_clear_flag(CheckboxObj, LV_OBJ_FLAG_SCROLLABLE);      // 禁用滚动条

    image_create(CheckboxObj, &Checkbox_W, 0, -30);                                     // 图片
    card_create_20_text(CheckboxObj, "Checkbox组件", 0, 50);                            // 标题
    lv_obj_add_event_cb(CheckboxObj, add_CheckboxObj_event_cb, LV_EVENT_CLICKED, NULL); /**点击添加图片事件**/
}