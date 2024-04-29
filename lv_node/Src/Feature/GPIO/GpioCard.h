

/***
 * @Description:
 * @Author: mai
 * @LastEditors: mai
 * @Date: 2023-07-27 13:54:43
 * @LastEditTime: 2023-07-27 14:25:30
 * @FilePath: /Demo/lv_node/Src/Feature/GpioCard.h
 */
#ifndef _LV_CARD_IO_H
#define _LV_CARD_IO_H

#define MAX_GPIO_NAME_LENGTH 256
#define GPIO_NAME_1 "GPIO7"
#define GPIO_NAME_2 "GPIO11"

#define SET_TEXT(btn, text) lv_label_set_text(lv_obj_get_child(btn, 0), text)

#define SET_DEFAULT_STRING(str, default_str) \
    if (strcmp(str, "") == 0) { \
        strncpy(str, default_str, MAX_GPIO_NAME_LENGTH - 1); \
    }

#define UPDATE_MSG_BUFFER(msg_buffer, direction, level, gpio_name) \
    snprintf(msg_buffer, MSG_BUFFER_SIZE, "方向 : %s\n电平值 : %s\nGPIO值 : %s", \
             direction, level, gpio_name);


typedef struct
{
     char gpio_name[MAX_GPIO_NAME_LENGTH];        // GPIO name (assuming it is a string)
     char level_value[MAX_GPIO_NAME_LENGTH];      // Level value (can be HIGH or LOW, represented as 1 or 0)
     char output_direction[MAX_GPIO_NAME_LENGTH]; // Output direction (can be OUTPUT or INPUT, represented as 1 or 0)
     uint8_t gpio_number;                         // GPIO number (assuming it is an integer)
} gpio_info_t;

void CreateGpioAndI2C(void);



#endif