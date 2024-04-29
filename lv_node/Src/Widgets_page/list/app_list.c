/*
 * @Author: Flying
 * @Date: 2022-05-01 20:30:20
 * @LastEditors: Flying
 * @LastEditTime: 2022-05-01 20:42:18
 * @Description: 新建文件
 */

#include "ui_app.h"

int app_list_1(lv_obj_t *parent);
int app_list_2(lv_obj_t *parent);

lv_obj_t *app_list(lv_obj_t *parent)
{
    lv_obj_t *Widobj = Widget_Commom_BackgroundPage(parent);

    lv_obj_t *list1 = app_common_obj_create("list example", Widobj);
    app_common_obj_add_item(list1, "带标题列表", app_list_1);
    app_common_obj_add_item(list1, "可点击按键列表", app_list_2);
    return Widobj;
}

/**点击时间图片事件**/
static void add_ListObj_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED)
    {
        app_list(lv_scr_act());
    }
}

// 创建开关组件的卡片
void ListWitgets(lv_obj_t *parent)
{
    // 创建图片OBJ
    lv_obj_t *ListObj = lv_obj_create(parent);
    lv_obj_set_size(ListObj, 170, 170);
    lv_obj_align(ListObj, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_opa(ListObj, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ListObj, 0, LV_PART_MAIN); // 边框宽度
    lv_obj_clear_flag(ListObj, LV_OBJ_FLAG_SCROLLABLE);      // 禁用滚动条

    image_create(ListObj, &List_W, 0, -30);        // 图片
    card_create_20_text(ListObj, "List组件", 0, 50); // 标题

    lv_obj_add_event_cb(ListObj, add_ListObj_event_cb, LV_EVENT_CLICKED, NULL); /**点击添加图片事件**/
}
