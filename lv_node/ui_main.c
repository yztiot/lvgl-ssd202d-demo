#include "ui_app.h"

static lv_obj_t *home_button[2]; // 导航栏按钮
static lv_obj_t *home_pages[2];  // 滑动页面
static lv_obj_t *content1;
static lv_obj_t *lv_menu;

ui_main ui_main_page;

struct
{
  const void *img_off;
  const void *img_on;
  int x;
  int x_ofs;

  const char *label;
} obj_buttons[] = {
    {&data_off, &data_on, 0, -10, "组件区"},
    {&setting_off, &setting_on, 0, -10, "功能区"}

};

/*滑动页面时切换页面*/
static void home_tileview(lv_event_t *e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *obj = lv_event_get_target(e);
  lv_obj_t *active_tile = lv_tileview_get_tile_act(obj);

  if (code == LV_EVENT_VALUE_CHANGED)
  {
    for (int i = 0; i < 2; i++)
    {
      lv_imgbtn_set_state(home_button[i], LV_IMGBTN_STATE_RELEASED);
      if (active_tile == home_pages[i])
      {
        lv_imgbtn_set_state(home_button[i], LV_IMGBTN_STATE_PRESSED);
      }
    }
  }
}

/*创建滑动页面*/
static void home_page_box(lv_obj_t *parent)
{
  content1 = lv_tileview_create(parent); // content1 card
  lv_obj_add_event_cb(content1, home_tileview, LV_EVENT_VALUE_CHANGED, NULL);
  lv_obj_set_style_bg_opa(content1, 0, LV_STATE_DEFAULT);
  lv_obj_set_size(content1, 924, 591);
  lv_obj_set_pos(content1, 100, 9);
  lv_obj_set_style_width(content1, 0, LV_PART_SCROLLBAR);
  for (int i = 0; i < 2; i++)
  {
    home_pages[i] = lv_tileview_add_tile(content1, 0, i, LV_DIR_BOTTOM | LV_DIR_TOP);
  }
  // 初始化页面
  // ui_main_page.display = home_pages[0];
  ui_main_page.widgets = home_pages[0];
  ui_main_page.feature = home_pages[1];
}

/*导航点击事件*/
static void home_btn_event(lv_event_t *e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *obj = lv_event_get_user_data(e);

  if (code == LV_EVENT_CLICKED || code == LV_EVENT_VALUE_CHANGED)
  {

    for (int i = 0; i < 2; i++)
    {
      lv_imgbtn_set_state(home_button[i], obj == home_button[i] ? LV_IMGBTN_STATE_PRESSED : LV_IMGBTN_STATE_RELEASED);
      if (obj == home_button[i])
      {
        lv_obj_set_tile_id(content1, 0, i, LV_ANIM_ON);
      }
    }
  }
}

/*导航栏按键范围*/
static lv_obj_t *lv_fout_create(lv_obj_t *obj, lv_coord_t y_ofs)
{
  lv_obj_t *home_fout = lv_obj_create(obj);
  lv_obj_set_size(home_fout, 100, 120);
  lv_obj_align(home_fout, LV_ALIGN_TOP_LEFT, 0, y_ofs);
  lv_obj_set_style_bg_opa(home_fout, 0, LV_STATE_DEFAULT);
  lv_obj_set_style_border_opa(home_fout, 0, LV_STATE_DEFAULT);
  return home_fout;
}

/**创建主导航栏按钮**/
static lv_obj_t *lv_imgbtn_create_with_label(lv_obj_t *obj, const void *img_src, const void *img_crs, lv_coord_t x, lv_coord_t x_ofs, lv_coord_t y_ofs, const char *label_text)
{

  lv_obj_t *home_fout = lv_fout_create(obj, y_ofs);
  lv_obj_t *img_btn = lv_imgbtn_create(home_fout);
  lv_imgbtn_set_src(img_btn, LV_IMGBTN_STATE_RELEASED, NULL, img_src, NULL);
  lv_imgbtn_set_src(img_btn, LV_IMGBTN_STATE_PRESSED, NULL, img_crs, NULL);
  lv_obj_set_size(img_btn, 40, 50);
  lv_obj_set_x(img_btn, x);
  lv_obj_t *label = lv_label_create(obj);
  lv_label_set_text(label, label_text);
  lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
  lv_obj_align_to(label, img_btn, LV_ALIGN_CENTER, x_ofs, 30);
  lv_obj_set_style_text_font(label, &PuHuiTi_Regular_16, 0);
  lv_obj_add_event_cb(home_fout, home_btn_event, LV_EVENT_ALL, img_btn);
  lv_obj_add_event_cb(img_btn, home_btn_event, LV_EVENT_ALL, img_btn);
  return img_btn;
}

/*左边导航页面*/
static void ui_navigation_main_menu(lv_obj_t *obj)
{
  for (int i = 0; i < 2; i++)
  {
    home_button[i] = lv_imgbtn_create_with_label(obj, obj_buttons[i].img_off, obj_buttons[i].img_on, obj_buttons[i].x, obj_buttons[i].x_ofs, 130 + i * 200, obj_buttons[i].label);
  }
  lv_imgbtn_set_state(home_button[0], LV_IMGBTN_STATE_PRESSED);
}

/*主页背景*/
static void home_bg(lv_obj_t *obj)
{
  lv_obj_t *lv_background = lv_img_create(obj); // 背景页
  lv_img_set_src(lv_background, &background);
  lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE); // 禁用滚动条
  lv_menu = lv_img_create(obj);                   // 左边导航页
  lv_img_set_src(lv_menu, &menu);
  ui_navigation_main_menu(lv_menu);
}

void create_lv_layout(lv_obj_t *scr)
{
  home_bg(scr);                       // 背景图片
  home_page_box(scr);                 // 框架
  lv_display_page(lv_scr_act());      // 首页
  lv_WidgetsPage_page(home_pages[0]); // 组件区页面
  lv_feature_page(home_pages[1]);     // 功能区页面

 
}
