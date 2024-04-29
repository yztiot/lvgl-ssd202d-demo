/***
 * @Description:
 * @Author: mai
 * @LastEditors: mai
 * @Date: 2023-07-28 15:41:12
 * @LastEditTime: 2023-07-28 15:46:52
 * @FilePath: /Demo/lv_node/Src/Feature/WifiKeyboard.h
 */
#ifndef LV_WIFI_KEYBOARD_H
#define LV_WIFI_KEYBOARD_H
#include "lvgl/lvgl.h"
// Define the maximum length for SSID and password
#define SSID_MAX_LENGTH     64
#define PASSWORD_MAX_LENGTH 64
#define GET_LABEL_TEXT(parent) lv_label_get_text(lv_obj_get_child(parent, 1))



typedef struct {
    char ssid[SSID_MAX_LENGTH];  // Wi-Fi name (SSID) string
    char password[PASSWORD_MAX_LENGTH];  // Wi-Fi password string
} WifiCredentials;


void CreateWifiKeyboard(lv_obj_t *parent);




#endif // LV_WIFI_KEYBOARD_H