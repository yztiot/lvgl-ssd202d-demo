/*
 * @Description:
 * @Author: mai
 * @LastEditors: chen
 * @Date: 2023-07-27 13:54:16
 * @LastEditTime: 2023-12-19 11:44:57
 * @FilePath: /Demo/lv_node/Src/Feature/FeatureCommon.c
 */
#include "ui_app.h"

// 创建一个单选框
lv_obj_t *lv_radiobutton_create(lv_obj_t *parent, const char *txt)
{

    static lv_style_t style_radio;
    static lv_style_t style_radio_chk;

    lv_style_init(&style_radio);                         // 初始化单选按钮的样式
    lv_style_set_radius(&style_radio, LV_RADIUS_CIRCLE); // 设置单选框圆形形状
    lv_style_set_bg_img_src(&style_radio_chk, NULL);     // 不使用任何背景图
    lv_style_init(&style_radio_chk);                     // 初始化选中状态

    lv_obj_t *obj = lv_checkbox_create(parent);
    lv_checkbox_set_text(obj, txt);
    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffff), LV_STATE_DEFAULT);

    lv_obj_add_flag(obj, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_add_style(obj, &style_radio, LV_PART_INDICATOR);
    lv_obj_add_style(obj, &style_radio_chk, LV_PART_INDICATOR | LV_STATE_CHECKED);

    return obj;
}

/*功能区的大按钮*/
lv_obj_t *CreateBigButton(lv_obj_t *parent, const char *confirm_text, lv_coord_t x_ofs, lv_coord_t y_ofs, lv_event_cb_t event_cb, void *user_data)
{
    lv_obj_t *confirm_btn = lv_btn_create(parent);
    lv_obj_set_size(confirm_btn, 166, 100);
    lv_obj_align(confirm_btn, LV_ALIGN_TOP_LEFT, x_ofs, y_ofs);
    lv_obj_set_style_radius(confirm_btn, 5, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(confirm_btn, lv_color_hex(0x14e7f0), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(confirm_btn, lv_color_hex(0x2d6af2), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(confirm_btn, LV_GRAD_DIR_VER, LV_STATE_DEFAULT);
    lv_obj_add_event_cb(confirm_btn, event_cb, LV_EVENT_CLICKED, user_data);
    lv_obj_t *confirm_label = lv_label_create(confirm_btn);
    lv_label_set_text(confirm_label, confirm_text);
    lv_obj_set_style_text_color(confirm_label, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(confirm_label, &PuHuiTi_Regular_20, LV_STATE_DEFAULT);
    lv_obj_align(confirm_label, LV_ALIGN_CENTER, 0, 0);
    return confirm_btn;
}

/*创建输出文本框*/
lv_obj_t *CreateTextBox(lv_obj_t *parent, lv_coord_t x_ofs, lv_coord_t y_ofs)
{

    lv_obj_t *text = lv_textarea_create(parent);
    lv_obj_align(text, LV_ALIGN_TOP_MID, x_ofs, y_ofs); // 设置位置
    lv_obj_set_style_text_font(text, &PuHuiTi_Regular_20, LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(text, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
    lv_obj_set_size(text, 780, 257);
    lv_obj_set_style_width(text, 0, LV_PART_SCROLLBAR);
    lv_obj_set_style_bg_color(text, lv_color_hex(0x0E0E24), LV_STATE_DEFAULT);
    lv_textarea_set_text(text, "");
    lv_obj_set_style_border_color(text, lv_color_make(126, 137, 152), LV_STATE_DEFAULT);
    return text;
}

// 创建GPIO和I2C的按钮
lv_obj_t *CreateGpioAndI2cButton(lv_obj_t *parent, const char *confirm_text, lv_coord_t x_ofs, lv_coord_t y_ofs, lv_event_cb_t event_cb, void *user_data)
{
    lv_obj_t *confirm_btn = lv_btn_create(parent);
    lv_obj_set_size(confirm_btn, 175, 94);
    lv_obj_align(confirm_btn, LV_ALIGN_TOP_LEFT, x_ofs, y_ofs);
    lv_obj_set_style_radius(confirm_btn, 5, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(confirm_btn, lv_color_make(42, 131, 228), LV_STATE_DEFAULT);

    lv_obj_add_event_cb(confirm_btn, event_cb, LV_EVENT_CLICKED, user_data);
    lv_obj_t *confirm_label = lv_label_create(confirm_btn);
    lv_label_set_text(confirm_label, confirm_text);
    lv_obj_set_style_text_color(confirm_label, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(confirm_label, &PuHuiTi_Regular_20, LV_STATE_DEFAULT);
    lv_obj_align(confirm_label, LV_ALIGN_CENTER, 0, 0);
    return confirm_btn;
}

lv_obj_t *CreateWifiButton(lv_obj_t *parent, const char *confirm_text, lv_coord_t x_ofs, lv_coord_t y_ofs, lv_event_cb_t event_cb, void *user_data)
{
    lv_obj_t *confirm_btn = lv_btn_create(parent);
    lv_obj_set_size(confirm_btn, 145, 82);
    lv_obj_align(confirm_btn, LV_ALIGN_TOP_LEFT, x_ofs, y_ofs);
    lv_obj_set_style_radius(confirm_btn, 5, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(confirm_btn, lv_color_make(42, 131, 228), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(confirm_btn, lv_color_hex(0x2d6af2), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(confirm_btn, LV_GRAD_DIR_VER, LV_STATE_DEFAULT);
    lv_obj_add_event_cb(confirm_btn, event_cb, LV_EVENT_CLICKED, user_data);
    lv_obj_t *confirm_label = lv_label_create(confirm_btn);
    lv_label_set_text(confirm_label, confirm_text);
    lv_obj_set_style_text_color(confirm_label, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(confirm_label, &PuHuiTi_Regular_20, LV_STATE_DEFAULT);
    lv_obj_align(confirm_label, LV_ALIGN_CENTER, 0, 0);
    return confirm_btn;
}

lv_obj_t *create_switch(lv_obj_t *parent, lv_event_cb_t *event_cb)
{
    lv_obj_t *sw = lv_switch_create(parent);
    lv_obj_set_size(sw, 70, 33);
    lv_obj_align(sw, LV_ALIGN_TOP_RIGHT, -40, 45);
    lv_obj_set_style_bg_color(sw, lv_color_hex(0x14E7F0), LV_PART_INDICATOR | LV_STATE_CHECKED);
    lv_obj_set_style_radius(sw, 18, LV_STATE_DEFAULT);
    lv_obj_add_state(sw, LV_STATE_CHECKED);

    lv_obj_t *sw_label = lv_label_create(sw);
    lv_obj_set_style_text_color(sw_label, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(sw_label, &PuHuiTi_Regular_20, LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(sw_label, &PuHuiTi_Regular_20, LV_PART_INDICATOR | LV_STATE_CHECKED);
    lv_obj_add_event_cb(sw, event_cb, LV_EVENT_VALUE_CHANGED, sw_label);
    lv_obj_align(sw_label, LV_ALIGN_CENTER, -10, 0);
    lv_label_set_text(sw_label, "开");

    return sw;
}

lv_obj_t *CreatrWifiList(lv_obj_t *parent, int h, int y)
{

    lv_obj_t *list = lv_list_create(parent);
    lv_obj_set_size(list, 980, h);
    lv_obj_align(list, LV_ALIGN_TOP_MID, 0, y);
    lv_obj_set_style_bg_opa(list, 0, 0);
    lv_obj_set_style_border_opa(list, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_radius(list, 0, LV_STATE_DEFAULT);
    return list;
}

lv_obj_t *CreateBackButtonAndText(lv_obj_t *parent, const char *confirm_text, lv_coord_t x_ofs, lv_coord_t y_ofs, lv_event_cb_t event_cb, void *user_data)
{

    static lv_style_t style_pr;
    lv_style_init(&style_pr);
    lv_style_set_transform_width(&style_pr, 5);
    lv_style_set_transform_height(&style_pr, 5);

    lv_obj_t *confirm_btn = lv_obj_create(parent);
    lv_obj_set_size(confirm_btn, 90, 31);
    lv_obj_align(confirm_btn, LV_ALIGN_TOP_RIGHT, x_ofs, y_ofs);
    lv_obj_add_flag(confirm_btn, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_radius(confirm_btn, 5, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(confirm_btn, lv_color_hex(0x14e7f0), LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(confirm_btn, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(confirm_btn, lv_color_hex(0x2d6af2), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(confirm_btn, LV_GRAD_DIR_VER, LV_STATE_DEFAULT);
    lv_obj_add_event_cb(confirm_btn, event_cb, LV_EVENT_CLICKED, user_data);

    lv_obj_add_style(confirm_btn, &style_pr, LV_STATE_PRESSED);

    lv_obj_t *confirm_label = lv_label_create(confirm_btn);
    lv_label_set_text(confirm_label, confirm_text);
    lv_obj_set_style_text_color(confirm_label, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(confirm_label, &PuHuiTi_Regular_20, LV_STATE_DEFAULT);
    lv_obj_align(confirm_label, LV_ALIGN_CENTER, 0, 0);

    return confirm_btn;
}
lv_obj_t *CreadWifiPasswordTextarea(lv_obj_t *SrcParent)
{
    lv_obj_t *Wifibackground = lv_obj_create(SrcParent);
    lv_obj_set_size(Wifibackground, 1024, 600);
    lv_obj_align(Wifibackground, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_style_bg_color(Wifibackground, lv_color_hex(0x0b1a2e), LV_PART_MAIN);
    lv_obj_set_style_border_width(Wifibackground, 0, LV_PART_MAIN);

    // // Create the password textarea
    lv_obj_t *password_textarea = lv_textarea_create(Wifibackground);
    lv_textarea_set_password_mode(password_textarea, true);
    lv_textarea_set_one_line(password_textarea, true);
    lv_obj_set_width(password_textarea, lv_pct(40));
    lv_obj_set_pos(password_textarea, 225, 100);

    // Create the keyboard
    lv_obj_t *keyboard = lv_keyboard_create(Wifibackground);
    lv_obj_set_style_bg_color(keyboard, lv_color_hex(0x1b2835), LV_PART_MAIN);
    lv_obj_set_size(keyboard, LV_HOR_RES, LV_VER_RES / 2);
    lv_keyboard_set_textarea(keyboard, password_textarea);
    lv_obj_set_size(keyboard, 980, 300);
    return password_textarea;
}

// WiFi列表界面背景
lv_obj_t *create_wifi_page(lv_obj_t *parent)
{
    lv_obj_t *page = lv_obj_create(parent); 
    lv_obj_set_size(page, 1024, 530);
    lv_obj_align(page, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_style_bg_opa(page, 0, 0);
    lv_obj_set_style_border_opa(page, 0, 0);

    lv_obj_t *text1 = card_create_20_text(page, "当前已连接网络", 0, 0);
    lv_obj_align(text1, LV_ALIGN_TOP_LEFT, 10, 0);
    lv_obj_t *text2 = card_create_20_text(page, "其他网络", 0, 0);
    lv_obj_align(text2, LV_ALIGN_TOP_LEFT, 10, 100);

    return page;
}

 lv_obj_t *CreatrWifiListStaticButton(lv_obj_t *parent)
{
    static lv_style_t style_wifi_list;
    lv_style_init(&style_wifi_list);
    lv_style_set_bg_opa(&style_wifi_list, LV_OPA_TRANSP);
    lv_style_set_bg_color(&style_wifi_list, lv_color_hex(0x4c4965));
    lv_style_set_border_width(&style_wifi_list, 0);
    lv_style_set_radius(&style_wifi_list, 0);
    lv_style_set_text_color(&style_wifi_list, lv_color_hex(0xffffff));
    lv_style_set_text_font(&style_wifi_list, &lv_font_montserrat_16);
    lv_style_set_text_align(&style_wifi_list, LV_TEXT_ALIGN_LEFT);
    lv_style_set_pad_right(&style_wifi_list, 10);
    lv_style_set_pad_all(&style_wifi_list, 20);
    lv_style_set_img_recolor(&style_wifi_list, lv_color_hex(0xffffff));

    lv_obj_t *ListButton = lv_list_add_btn(parent, NULL, NULL);
    lv_obj_remove_style_all(ListButton);
    lv_obj_set_size(ListButton, 920, 50);
    lv_obj_align(ListButton, LV_ALIGN_RIGHT_MID, 0, 0);
    lv_obj_add_style(ListButton, &style_wifi_list, LV_PART_MAIN);
    lv_obj_set_style_pad_all(ListButton, 15, LV_STATE_DEFAULT);
  

    static lv_style_t style_line;
    lv_style_init(&style_line);
    lv_style_set_line_width(&style_line, 1);
    lv_style_set_line_color(&style_line, lv_color_hex(0xCCCCCC));
    lv_style_set_bg_color(&style_line, lv_color_hex(0xCCCCCC));
    lv_style_set_line_opa(&style_line, LV_OPA_50);
    lv_obj_t *line = lv_line_create(parent);
    static const lv_point_t line_points1[] = {{5, 0}, {920, 0}};
    lv_line_set_points(line, line_points1, 2);
    lv_obj_align(line, LV_ALIGN_BOTTOM_MID, 40, 0);
    lv_obj_add_style(line, &style_line, LV_PART_MAIN);

    lv_obj_t *WifiIcon = lv_label_create(ListButton);
    lv_label_set_text(WifiIcon, LV_SYMBOL_WIFI);
    lv_obj_set_style_text_font(WifiIcon, &lv_font_montserrat_16, 0);
    lv_obj_align(WifiIcon, LV_ALIGN_LEFT_MID, 0, 5);
    lv_obj_set_style_text_color(WifiIcon, lv_color_hex(0xffffff), 0);
    return ListButton;
}
// 辅助函数：更新文本框内容
void update_area(lv_obj_t *obj, const char *msg_buffer)
{
    static char formatted_msg[MSG_BUFFER_SIZE];
    strncat(msg_buffer, formatted_msg, MSG_BUFFER_SIZE - strlen(msg_buffer) - 1);
    lv_textarea_set_text(obj, msg_buffer);
}
// 辅助函数：执行系统命令并将输出刷到缓冲区中
void execute_command(const char *command, char **buffer)
{
    FILE *fp;
    fp = popen(command, "r");
    if (fp != NULL)
    {
        // 动态分配内存来存储输出内容
        *buffer = (char *)malloc(MSG_BUFFER_SIZE);
        if (*buffer == NULL)
        {
            printf("内存分配失败\n");
            return;
        }

        // 初始化缓冲区内容
        **buffer = '\0';

        // 逐行读取命令输出并将内容拷贝到缓冲区中
        char line[256];
        while (fgets(line, sizeof(line), fp) != NULL)
        {
            strcat(*buffer, line);

            // 如果缓冲区即将溢出，则重新分配更多内存
            if (strlen(*buffer) >= MSG_BUFFER_SIZE - 1)
            {
                *buffer = (char *)realloc(*buffer, strlen(*buffer) + sizeof(line) + 1);
                if (*buffer == NULL)
                {
                    printf("内存重新分配失败\n");
                    return;
                }
            }
        }

        // 关闭文件指针
        pclose(fp);
    }
}

