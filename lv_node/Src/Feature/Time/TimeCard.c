/*
 * @Description: 
 * @Author: chen
 * @Date: 2023-11-24 17:41:54
 * @LastEditors: chen
 * @LastEditTime: 2023-12-19 09:53:20
 */


#include "ui_app.h"

static lv_obj_t *all_roller = NULL;

static char year_buf[32]  = {0};
static char month_buf[32] = {0};
static char day_buf[32]   = {0};
static char hour_buf[32]  = {0};
static char min_buf[32]   = {0};

static void time_roller(lv_obj_t *parent);

static void SaveButton_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED)
    {

        char command[100];

        char year[10];
        char month[10];
        char day[10];
        char hour[10];
        char min[10];
        printf("%s,%s,%s,%s,%s\r\n",year_buf,month_buf,day_buf,hour_buf,min_buf);

        sscanf(year_buf, "%[^ ]", year);
        sscanf(month_buf, "%[^ ]", month);
        sscanf(day_buf, "%[^ ]", day);
        sscanf(hour_buf, "%[^ ]", hour);
        sscanf(min_buf, "%[^ ]", min);

        // 合并指令
        sprintf(command, "date -s \"%s-%s-%s %s:%s:00\"", year, month, day, hour, min);
        system(command);
        system("hwclock -w"); 
        
      
    }
}

static void NTPButton_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED)
    {
        int ret = NTP_get_time_and_set();
        if(ret == 0)
        {
            lv_obj_t *parent = all_roller->parent;
            lv_obj_del(all_roller);
            time_roller(parent);
        }
        else{
            create_popup(lv_scr_act(), true, "", "确定", NULL, "获取失败，请检查网络连接", NULL, NULL);
        }
        
    }

}
// 创建保存按钮
static void create_save_btn(lv_obj_t *parent)
{
    lv_obj_t *save_btn = btn_create_text(parent, false, "修改", 0, 0);   
    lv_obj_align(save_btn, LV_ALIGN_CENTER, 0, 50);
    lv_obj_add_event_cb(save_btn, SaveButton_event_cb, LV_EVENT_ALL, NULL);

    lv_obj_t *NTP_btn = btn_create_text(parent, false, "获取", 0, 0);   
    lv_obj_align(NTP_btn, LV_ALIGN_CENTER, 0, 185);
    lv_obj_add_event_cb(NTP_btn, NTPButton_event_cb, LV_EVENT_ALL, NULL);
}

// 滚轮重绘
static void mask_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    static int16_t mask_top_id = -1;
    static int16_t mask_bottom_id = -1;

    if (code == LV_EVENT_COVER_CHECK)
    {
        lv_event_set_cover_res(e, LV_COVER_RES_MASKED);
    }
    else if (code == LV_EVENT_DRAW_MAIN_BEGIN)
    {
        /* add mask */
        const lv_font_t *font = lv_obj_get_style_text_font(obj, LV_PART_MAIN);
        lv_coord_t line_space = lv_obj_get_style_text_line_space(obj, LV_PART_MAIN);
        lv_coord_t font_h = lv_font_get_line_height(font);

        lv_area_t roller_coords;
        lv_obj_get_coords(obj, &roller_coords);

        lv_area_t rect_area;
        rect_area.x1 = roller_coords.x1;
        rect_area.x2 = roller_coords.x2;
        rect_area.y1 = roller_coords.y1;
        rect_area.y2 = roller_coords.y1 + (lv_obj_get_height(obj) - font_h - line_space) / 2;

        lv_draw_mask_fade_param_t *fade_mask_top = lv_mem_buf_get(sizeof(lv_draw_mask_fade_param_t));
        lv_draw_mask_fade_init(fade_mask_top, &rect_area, LV_OPA_TRANSP, rect_area.y1, LV_OPA_COVER, rect_area.y2);
        mask_top_id = lv_draw_mask_add(fade_mask_top, NULL);

        rect_area.y1 = rect_area.y2 + font_h + line_space - 1;
        rect_area.y2 = roller_coords.y2;

        lv_draw_mask_fade_param_t *fade_mask_bottom = lv_mem_buf_get(sizeof(lv_draw_mask_fade_param_t));
        lv_draw_mask_fade_init(fade_mask_bottom, &rect_area, LV_OPA_COVER, rect_area.y1, LV_OPA_TRANSP, rect_area.y2);
        mask_bottom_id = lv_draw_mask_add(fade_mask_bottom, NULL);
    }
    else if (code == LV_EVENT_DRAW_POST_END)
    {
        lv_draw_mask_fade_param_t *fade_mask_top = lv_draw_mask_remove_id(mask_top_id);
        lv_draw_mask_fade_param_t *fade_mask_bottom = lv_draw_mask_remove_id(mask_bottom_id);
        lv_draw_mask_free_param(fade_mask_top);
        lv_draw_mask_free_param(fade_mask_bottom);
        lv_mem_buf_release(fade_mask_top);
        lv_mem_buf_release(fade_mask_bottom);
        mask_top_id = -1;
        mask_bottom_id = -1;
    }
}

// 滚动事件
static void roller_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    int user_data = (int)lv_event_get_user_data(e);
    if (code == LV_EVENT_VALUE_CHANGED)
    {
        switch (user_data)
        {
        case 1:
            lv_roller_get_selected_str(obj, year_buf, sizeof(year_buf));
            break;
        case 2:
            lv_roller_get_selected_str(obj, month_buf, sizeof(year_buf));
            break;
        case 3:
            lv_roller_get_selected_str(obj, day_buf, sizeof(year_buf));
            break;
        case 4:
            lv_roller_get_selected_str(obj, hour_buf, sizeof(year_buf));
            break;
        case 5:
            lv_roller_get_selected_str(obj, min_buf, sizeof(year_buf));
            break;
        
        default:
            break;
        }

        
    }
}

// 创建滚轮
static lv_obj_t *createTimeCard(lv_obj_t *parent, lv_coord_t x, const char *options, uint16_t sel_opt)
{
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_bg_color(&style, lv_color_make(0x17, 0x68, 0xff));
    lv_style_set_text_color(&style, lv_color_white());
    lv_style_set_border_width(&style, 0);
    lv_style_set_pad_all(&style, 0);

    lv_obj_t *roller1 = lv_roller_create(parent);
    lv_obj_add_style(roller1, &style, 0);
    lv_obj_set_style_bg_opa(roller1, 0, LV_PART_SELECTED);
    lv_obj_set_style_bg_opa(roller1, 0, LV_PART_MAIN);
    lv_obj_set_style_border_opa(roller1, 0, LV_PART_MAIN);
    lv_obj_set_width(roller1, 100);
    lv_obj_set_style_text_line_space(roller1, 40, LV_PART_MAIN);
    lv_obj_set_style_radius(roller1, 10, LV_PART_MAIN);
    lv_obj_align(roller1, LV_ALIGN_CENTER, x, 0);

    lv_obj_set_style_text_font(roller1, &PuHuiTi_Regular_30, LV_PART_SELECTED);
    lv_obj_set_style_text_font(roller1, &PuHuiTi_Regular_20, LV_PART_MAIN);
    lv_obj_set_style_text_color(roller1, lv_color_hex(0x3392FF), LV_PART_SELECTED);
    lv_roller_set_options(roller1, options, LV_ROLLER_MODE_NORMAL);
    lv_roller_set_visible_row_count(roller1, 3);
    lv_obj_add_event_cb(roller1, mask_event_cb, LV_EVENT_ALL, NULL);
    lv_roller_set_selected(roller1, sel_opt, LV_ANIM_OFF);

    return roller1;
}

// 创建时间滚轮
static void time_roller(lv_obj_t *parent)
{

    all_roller = lv_obj_create(parent);
    lv_obj_align(all_roller, LV_ALIGN_CENTER, 0, -100);
    lv_obj_set_size(all_roller, 520, 250);
    lv_obj_set_style_bg_opa(all_roller, 0, LV_PART_MAIN);
    lv_obj_set_style_radius(all_roller, 0, LV_PART_MAIN);



    time_t t = time(NULL);
    struct tm *timer = localtime(&t);

    uint16_t t_year = timer->tm_year + 1900; // 年份需要加上 1900
    uint16_t month = timer->tm_mon;          // 月份从 0 开始，所以需要加上 1
    uint16_t day = timer->tm_mday;
    uint16_t hour = timer->tm_hour;
    uint16_t min = timer->tm_min;

    card_create_24_text(all_roller, "-", -140, -5);
    card_create_24_text(all_roller, "-", -70, -5);
    card_create_24_text(all_roller, ":", 140, -5);
    lv_obj_t *roller1 = createTimeCard(all_roller, -190, TIME_ROLLER_YEAH, t_year - 1970);
    lv_roller_get_selected_str(roller1, year_buf, sizeof(year_buf));
    lv_obj_add_event_cb(roller1, roller_event_handler, LV_EVENT_VALUE_CHANGED, (void *)1);

    lv_obj_t *roller2 = createTimeCard(all_roller, -110, TIME_ROLLER_MONTH, month);
    lv_roller_get_selected_str(roller2, month_buf, sizeof(month_buf));
    lv_obj_add_event_cb(roller2, roller_event_handler, LV_EVENT_VALUE_CHANGED, (void *)2);

    lv_obj_t *roller3 = createTimeCard(all_roller, -30, TIME_ROLLER_DAY, day - 1);
    lv_roller_get_selected_str(roller3, day_buf, sizeof(day_buf));
    lv_obj_add_event_cb(roller3, roller_event_handler, LV_EVENT_VALUE_CHANGED, (void *)3);

    lv_obj_t *roller4 = createTimeCard(all_roller, 100, TIME_ROLLER_HOUR, hour);
    lv_roller_get_selected_str(roller4, hour_buf, sizeof(hour_buf));
    lv_obj_add_event_cb(roller4, roller_event_handler, LV_EVENT_VALUE_CHANGED, (void *)4);

    lv_obj_t *roller5 = createTimeCard(all_roller, 180, TIME_ROLLER_MIN, min);
    lv_roller_get_selected_str(roller5, min_buf, sizeof(min_buf));
    lv_obj_add_event_cb(roller5, roller_event_handler, LV_EVENT_VALUE_CHANGED, (void *)5);
}


void TimeCard(void)
{
    lv_obj_t *parent = Page_Background_Popup(lv_scr_act(), "时间设置");
    
    time_roller(parent);
    create_save_btn(parent);

    card_create_20_text(parent, "获取NTP授时服务器时间", 0, 125);
    
}