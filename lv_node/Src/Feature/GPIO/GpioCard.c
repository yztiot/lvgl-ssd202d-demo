/*
 * @Description:GPIO
 * @Author: lzs
 * @LastEditors: chen
 * @Date: 2023-03-09 11:28:06
 * @LastEditTime: 2023-12-19 10:33:53
 * @FilePath: /Demo/lv_node/Src/Feature/GpioCard.c
 */

#include "ui_app.h"
static uint32_t active_index = 0;
static gpio_info_t gpio_info; // GPIO info struct

static lv_obj_t *GpioOut; // GPIO output text box
static lv_obj_t *I2cOut;  // I2C output text box

static char *I2cOut_buffer = NULL; // 信息缓冲区

static void radio_event_handler(lv_event_t *e)
{
    uint32_t *active_id = lv_event_get_user_data(e);
    lv_obj_t *cont = lv_event_get_current_target(e);
    lv_obj_t *act_cb = lv_event_get_target(e);
    lv_obj_t *old_cb = lv_obj_get_child(cont, *active_id);

    /*Do nothing if the container was clicked*/
    if (act_cb == cont)
        return;

    lv_obj_clear_state(old_cb, LV_STATE_CHECKED); /*Uncheck the previous radio button*/
    lv_obj_add_state(act_cb, LV_STATE_CHECKED);   /*Uncheck the current radio button*/

    *active_id = lv_obj_get_index(act_cb);

    if (*active_id == 0)
    {
        strncpy(gpio_info.gpio_name, "GPIO7", MAX_GPIO_NAME_LENGTH - 1);
        gpio_info.gpio_number = 7;
    }
    else if (*active_id == 1)
    {
        strncpy(gpio_info.gpio_name, "GPIO11", MAX_GPIO_NAME_LENGTH - 1);
        gpio_info.gpio_number = 11;
    }
    printf("GPIO Name: %s\n", gpio_info.gpio_name);
}

static lv_obj_t *lv_Port_RadioBox(lv_obj_t *parent)
{
    lv_obj_t *ContBox = lv_obj_create(parent); // 创建一个标签
    lv_obj_set_size(ContBox, 400, 100);
    lv_obj_align(ContBox, LV_ALIGN_TOP_LEFT, 50, 100);
    lv_obj_set_style_bg_opa(ContBox, LV_OPA_0, 0);
    lv_obj_set_style_border_width(ContBox, 0, LV_PART_MAIN);
    lv_obj_add_event_cb(ContBox, radio_event_handler, LV_EVENT_CLICKED, &active_index);

    // 创建单选框
    lv_obj_t *GpioCard7 = lv_radiobutton_create(ContBox, "Gpio7");
    lv_obj_set_style_text_font(GpioCard7, &lv_font_montserrat_24, LV_PART_MAIN);

    lv_obj_t *GpioCard11 = lv_radiobutton_create(ContBox, "Gpio11");
    lv_obj_align(GpioCard11, LV_ALIGN_TOP_MID, 100, 0);
    lv_obj_set_style_text_font(GpioCard11, &lv_font_montserrat_24, LV_PART_MAIN);

    lv_obj_add_state(lv_obj_get_child(ContBox, 0), LV_STATE_CHECKED);
    return ContBox;
}

static void GPIO_event_handler(lv_event_t *e)
{
    static char gpio_msg_buffer[MSG_BUFFER_SIZE] = "";
    lv_obj_t *btn = lv_event_get_target(e);

    printf("GPIO Name: %s\n", gpio_info.gpio_name);
    gpio_info.gpio_number = 7;

    const char *btn_text = lv_label_get_text(lv_obj_get_child(btn, 0));

    const char *level_low = "电平值 : 低";
    const char *level_high = "电平值 : 高";
    const char *dir_output = "方向 : 输出";
    const char *dir_input = "方向 : 输入";

    if (strcmp(btn_text, level_low) == 0)
    {
        SET_TEXT(btn, level_high);
        gpio_set_value(gpio_info.gpio_number, GPIO_HIGH);
        strncpy(gpio_info.level_value, "高", MAX_GPIO_NAME_LENGTH - 1);
    }
    else if (strcmp(btn_text, level_high) == 0)
    {
        SET_TEXT(btn, level_low);
        gpio_set_value(gpio_info.gpio_number, GPIO_LOW);
        strncpy(gpio_info.level_value, "低", MAX_GPIO_NAME_LENGTH - 1);
    }
    else if (strcmp(btn_text, dir_output) == 0)
    {
        SET_TEXT(btn, dir_input);
        gpio_set_direction(gpio_info.gpio_number, GPIO_IN);
        strncpy(gpio_info.output_direction, "输入", MAX_GPIO_NAME_LENGTH - 1);
    }
    else if (strcmp(btn_text, dir_input) == 0)
    {
        SET_TEXT(btn, dir_output);
        gpio_set_direction(gpio_info.gpio_number, GPIO_OUT);
        strncpy(gpio_info.output_direction, "输出", MAX_GPIO_NAME_LENGTH - 1);
    }

    // Set default values for GPIO info if they are empty
    const char *default_gpio_name = "GPIO7";
    const char *default_level_value = "低";
    const char *default_output_direction = "输出";

    SET_DEFAULT_STRING(gpio_info.gpio_name, default_gpio_name);
    SET_DEFAULT_STRING(gpio_info.level_value, default_level_value);
    SET_DEFAULT_STRING(gpio_info.output_direction, default_output_direction);

    UPDATE_MSG_BUFFER(gpio_msg_buffer, gpio_info.output_direction, gpio_info.level_value, gpio_info.gpio_name);
    update_area(GpioOut, gpio_msg_buffer);
}

static void I2c_event_handler(lv_event_t *e)
{
    uint8_t cont = lv_event_get_user_data(e);

    char str[32];
    sprintf(str, "i2cdetect -y %d -r", cont);
    printf("str = %s\n", str);
    execute_command(str, &I2cOut_buffer);
    // 更新区域显示
    update_area(I2cOut, I2cOut_buffer);

    // 释放动态分配的内存
    if (I2cOut_buffer != NULL)
    {
        free(I2cOut_buffer);
    }
}

void CreateGpioAndI2C(void)
{

    gpio_export(7);
    gpio_export(11);

    lv_obj_t *parent = Page_Background_Popup(lv_scr_act(), "IO/I2C");
    lv_Port_RadioBox(parent);

    GpioOut = CreateTextBox(parent, 220, 90);
    lv_obj_set_size(GpioOut, 500, 200);

    I2cOut = CreateTextBox(parent, 220, 360);
    lv_obj_set_size(I2cOut, 500, 200);

    lv_obj_t *btnLevelHigh = CreateGpioAndI2cButton(parent, "电平值 : 低", 40, 180, NULL, NULL);
    lv_obj_add_flag(btnLevelHigh, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_t *btnDirectionOutput = CreateGpioAndI2cButton(parent, "方向 : 输出", 270, 180, NULL, NULL);
    lv_obj_add_flag(btnDirectionOutput, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_add_event_cb(btnLevelHigh, GPIO_event_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(btnDirectionOutput, GPIO_event_handler, LV_EVENT_CLICKED, NULL);

    lv_obj_t *I2cButton1 = CreateGpioAndI2cButton(parent, "扫描I2C-1", 40, 420, NULL, NULL);
    lv_obj_t *I2cButton0 = CreateGpioAndI2cButton(parent, "扫描I2C-0", 270, 420, NULL, NULL);

    lv_obj_add_event_cb(I2cButton1, I2c_event_handler, LV_EVENT_CLICKED, 1);
    lv_obj_add_event_cb(I2cButton0, I2c_event_handler, LV_EVENT_CLICKED, 0);
}