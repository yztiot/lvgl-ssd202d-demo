// /*
//  * @Author: Flying
//  * @Date: 2022-05-02 18:29:26
//  * @LastEditors: Flying
//  * @LastEditTime: 2022-07-24 22:09:11
//  * @Description: 新建文件
//  */
#include "ui_app.h"
#include "f_game_2048.h"
#include "f_game_snake.h"
#include "f_game_tetris.h"

int game_100ask(lv_obj_t *parent);

lv_obj_t *app_games(lv_obj_t *parent)
{
    lv_obj_t *Widobj = Widget_Commom_BackgroundPage(parent);
    lv_obj_t *list1 = app_common_obj_create("games example", Widobj);
    app_common_obj_add_item(list1, "2048", lv_f_game_2048);
    app_common_obj_add_item(list1, "贪吃蛇", lv_f_game_snake);
    app_common_obj_add_item(list1, "俄罗斯方块", f_game_tetris);
    return Widobj;
}


/**点击时间图片事件**/
static void add_GameObj_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED)
    {
        app_games(lv_scr_act());
    }
}

// 创建开关组件的卡片
void GameWitgets(lv_obj_t *parent)
{
    // 创建图片OBJ
    lv_obj_t *GameObj = lv_obj_create(parent);
    lv_obj_set_size(GameObj, 170, 170);
    lv_obj_align(GameObj, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_opa(GameObj, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(GameObj, 0, LV_PART_MAIN); // 边框宽度
    lv_obj_clear_flag(GameObj, LV_OBJ_FLAG_SCROLLABLE);      // 禁用滚动条

    image_create(GameObj, &Game_W, 0, -30);          // 图片
    card_create_20_text(GameObj, "Game", 0, 50); // 标题

    lv_obj_add_event_cb(GameObj, add_GameObj_event_cb, LV_EVENT_CLICKED, NULL); /**点击添加图片事件**/
}