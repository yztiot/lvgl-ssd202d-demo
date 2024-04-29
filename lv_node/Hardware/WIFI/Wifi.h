
#ifndef _WIFI_H__H
#define _WIFI_H__H
#include "lv_node/Src/Feature/WIFI/WifiCard.h"
#include "lv_node/Src/Feature/WIFI/WifiKeyboard.h"

extern bool wifi_init_fail;

 void init_wifi(void); // 初始化wifi
 void Disconnect_wifi(void); // 断开wifi
 void connect_wifi(WifiNode *node); // 连接线程
 int check_wifi();

#endif
