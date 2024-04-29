/*
 * @Description: 
 * @Author: chen
 * @Date: 2023-11-27 11:28:25
 * @LastEditors: chen
 * @LastEditTime: 2023-12-01 14:45:07
 */
#include "ui_app.h"
static lv_color_t *cbuf = NULL;
static void parent_del_cb(lv_event_t *e)
{
    if (cbuf)
    {
        lv_mem_free(cbuf);
        cbuf = NULL;
    }
    lv_anim_del_all();
}

void DrawingBoardCard(void)
{
    lv_obj_t *parent = Page_Background_Popup(lv_scr_act(), "画板");
    lv_obj_add_event_cb(parent, parent_del_cb, LV_EVENT_DELETE, NULL);
    
    int size = LV_CANVAS_BUF_SIZE_TRUE_COLOR(APP_UI_W_MAX, APP_UI_H_MAX) * sizeof(lv_color_t);
    cbuf = (lv_color_t *)lv_mem_alloc(size);
    lv_obj_t *sketchpad = lv_100ask_sketchpad_create(parent);
    lv_obj_set_size(sketchpad, 1024, 550);
    lv_obj_clear_flag(parent,LV_OBJ_FLAG_SCROLL_CHAIN);
    lv_obj_clear_flag(sketchpad,LV_OBJ_FLAG_SCROLL_CHAIN);
    lv_obj_clear_flag(sketchpad,LV_OBJ_FLAG_SCROLLABLE);

    lv_canvas_set_buffer(sketchpad, cbuf, APP_UI_W_MAX, APP_UI_H_MAX, LV_IMG_CF_TRUE_COLOR);
    lv_obj_align(sketchpad, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_canvas_fill_bg(sketchpad, lv_palette_lighten(LV_PALETTE_GREY, 3), LV_OPA_COVER);
}