/*
 * @Description: 
 * @Author: chen
 * @Date: 2023-11-07 16:14:29
 * @LastEditors: chen
 * @LastEditTime: 2023-11-30 14:28:08
 */

#ifndef _LV_CARD_WIFI_H
#define _LV_CARD_WIFI_H

#include "lv_node/Src/Feature/WIFI/WifiKeyboard.h"

void WifiCard(void);

typedef struct wifi_node
{
    lv_obj_t *WifiName;
    lv_obj_t *JoinTxet;
    lv_obj_t *JoinIcon;
    lv_obj_t *password_textarea;
    char ssid[SSID_MAX_LENGTH];
    char password[PASSWORD_MAX_LENGTH];
    struct wifi_node *next;
} WifiNode;

WifiNode *getWifiNodeFromListButton(lv_obj_t *listButton);
void CreatrWifiConnectButton(lv_obj_t *parent, char *text);
void disconnect_ui();
extern WifiNode *wifiListHead;
extern lv_obj_t *WifiList_connect;
#endif