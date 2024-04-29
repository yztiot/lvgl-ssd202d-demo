// /*
//  * @Description:wifi页面
//  * @Author: lzs
//  * @LastEditors: lzs
//  * @Date: 2023-03-09 11:28:06
//  * @LastEditTime: 2023-06-20 14:51:43
//  * @FilePath: lv_node/src/feature/Card_Wifi/Card_Wifi.c
//  */

#include "ui_app.h"

WifiNode *wifiListHead = NULL;           // WIFI元素链表头
WifiInfo wifi_results[MAX_SCAN_RESULTS]; // 扫描结果

static lv_obj_t *wifi_page = NULL;       // wifi界面
static lv_obj_t *WifiList = NULL;        // 其他WIFI列表
lv_obj_t *WifiList_connect = NULL;       // 已连接WIFI列表

WifiNode *wifi_has_been_connnected = NULL; // 已经连上的wifi
static bool wifi_switch = true;


static void CreatrWifiListButton(lv_obj_t *parent, char *text);

/*释放内存*/
static void freeWifiList()
{
    WifiNode *current = wifiListHead;
    while (current != NULL)
    {
        WifiNode *next = current->next;
   
        free(current);
        current = next;
    }
    wifiListHead = NULL;
}

/*输入密码*/
static void TextWIFIClick(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    if (code == LV_EVENT_CLICKED)
    {
        CreateWifiKeyboard(obj);
    }
}


static lv_obj_t *CreatrWifiJoinIcon(lv_obj_t *parent)
{
    lv_obj_t *JoinIcon = lv_label_create(parent);
    lv_label_set_text(JoinIcon, "");
    lv_obj_set_style_text_font(JoinIcon, &lv_font_montserrat_16, 0);
    lv_obj_align(JoinIcon, LV_ALIGN_RIGHT_MID, -10, 5);
    lv_obj_set_style_text_color(JoinIcon, lv_color_hex(0xffffff), 0);
    return JoinIcon;
}

static lv_obj_t *CreatrWifiJoinTxet(lv_obj_t *parent)
{
    lv_obj_t *JoinTxet = lv_label_create(parent);
    lv_label_set_text(JoinTxet, "");
    lv_obj_set_style_text_font(JoinTxet, &PuHuiTi_Regular_16, 0);
    lv_obj_align(JoinTxet, LV_ALIGN_RIGHT_MID, -30, 5);
    lv_obj_set_style_text_color(JoinTxet, lv_color_hex(0xffffff), 0);
    return JoinTxet;
}

static lv_obj_t *CreatrWifiWifiName(lv_obj_t *parent, char *text)
{
    lv_obj_t *WifiName = lv_label_create(parent);
    lv_label_set_text(WifiName, text);
    lv_obj_set_style_bg_opa(WifiName, LV_OPA_TRANSP, LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(WifiName, LV_OPA_TRANSP, LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(WifiName, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(WifiName, &PuHuiTi_Regular_16, LV_STATE_DEFAULT);
    lv_obj_align(WifiName, LV_ALIGN_LEFT_MID, 40, 5);
    lv_obj_set_style_text_color(WifiName, lv_color_hex(0xffffff), 0);
    return WifiName;
}


/*添加列表*/
static void addWifiInfoToList(lv_obj_t *parent, char *text)
{

    WifiNode *newNode = (WifiNode *)malloc(sizeof(WifiNode));
    if (newNode == NULL)
    {
        printf("Memory allocation failed\n");
        return;
    }

    
    strcpy(newNode->ssid, text);
    newNode->WifiName = CreatrWifiWifiName(parent, text);
    newNode->JoinTxet = CreatrWifiJoinTxet(parent);
    newNode->JoinIcon = CreatrWifiJoinIcon(parent);
    newNode->password_textarea = NULL;
    newNode->next = NULL;

    if (wifiListHead == NULL)
    {
        wifiListHead = newNode;
    }
    else
    {
        WifiNode *temp = wifiListHead;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = newNode;
    }
    lv_obj_set_user_data(parent, newNode);
}




/*扫描wifi列表*/
static void ScanningWifi()
{
    wifi_results->count = wifi_scan(wifi_results, MAX_SCAN_RESULTS); // 扫描WIFI

    lv_obj_del(WifiList);                 // 删除WIFI列表
    freeWifiList();                       // 释放WIFI列表
    WifiList = CreatrWifiList(wifi_page, 340, 140); // 创建WIFI列表

    if (wifi_results->count > 0)
    {
        for (int i = 0; i < wifi_results->count; i++)
        {
            if (strlen(wifi_results[i].SSID) == 0)
            {
                continue; // 跳过空的SSID
            }
            char *converted_str = convert_unicode_str(wifi_results[i].SSID); // 转换SSID
            CreatrWifiListButton(WifiList, converted_str);                   // 创建WIFI列表按钮
            //printf("ssid:%s\n",converted_str);
            free(converted_str);
        }
    }
    else
    {
        printf("No Wi-Fi networks found.\n");
    }
    
}

/*创建列表按钮*/
static void CreatrWifiListButton(lv_obj_t *parent, char *text)
{
    lv_obj_t *ListButton = CreatrWifiListStaticButton(parent);
    lv_obj_add_event_cb(ListButton, TextWIFIClick, LV_EVENT_CLICKED, NULL);
    addWifiInfoToList(ListButton, text);
}


/*获取当前列表结构体*/
WifiNode *getWifiNodeFromListButton(lv_obj_t *listButton)
{
    return (WifiNode *)lv_obj_get_user_data(listButton);
}

// 搜索wifi
void search_wifi(lv_timer_t *timer)
{
    if(WifiList != NULL)
    {
        ScanningWifi();   
          
    }
    lv_timer_del(timer);
    
}

void disconnect_ui()
{
    lv_obj_del(WifiList_connect);
    WifiList_connect = CreatrWifiList(wifi_page, 55, 35);
    free(wifi_has_been_connnected);
    wifi_has_been_connnected = NULL;
}

// 断开当前wifi
static void disconnect_confirm(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    if (code == LV_EVENT_CLICKED)
    {
        disconnect_ui();

        Disconnect_wifi();
        
        // 重新搜索wifi
        lv_timer_create(search_wifi, 500, NULL);
        
        lv_obj_del(obj->parent->parent);
    }
}

static void wifi_disconnect_event(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED)
    {
        create_popup(lv_scr_act(), false, "温馨提示", "确定", "取消", "是否断开连接", disconnect_confirm, NULL);
    }
}


/*创建已连接的wifi按钮*/
void CreatrWifiConnectButton(lv_obj_t *parent, char *text)
{
    lv_obj_t *ListButton = CreatrWifiListStaticButton(parent);
    lv_obj_add_event_cb(ListButton, wifi_disconnect_event, LV_EVENT_CLICKED, NULL);
    addWifiInfoToList(ListButton, text);

    WifiNode *node = getWifiNodeFromListButton(ListButton);
    lv_label_set_text(node->JoinIcon, LV_SYMBOL_OK);
    lv_label_set_text(node->JoinTxet, "已连接"); 

    wifi_has_been_connnected = (WifiNode *)malloc(sizeof(WifiNode));
    memcpy(wifi_has_been_connnected, node, sizeof(WifiNode));
}




void WifiCard(void)
{
    // 标题
    lv_obj_t *parent = Page_Background_Popup(lv_scr_act(), "WIFI");
    if(wifi_init_fail)
    {
         card_create_24_text(parent, "WIFI初始化失败, 请检查WIFI模块", 0, 0);
            return;
        // if(check_wifi() != 0)
        // {
        //     card_create_24_text(parent, "WIFI初始化失败, 请检查WIFI模块", 0, 0);
        //     return;
        // }
        // else{
        //     wifi_init_fail = false;
        // }
    }
    wifi_page = create_wifi_page(parent); // wifi次页面
    
    // 已连接wifi
    WifiList_connect = CreatrWifiList(wifi_page, 55, 35);
    if(wifi_has_been_connnected != NULL)
    {
        CreatrWifiConnectButton(WifiList_connect, wifi_has_been_connnected->ssid);         
    }
    
    // 其他wifi
    WifiList = CreatrWifiList(wifi_page, 340, 140);

    // 搜索wifi

    lv_timer_create(search_wifi, 10, NULL);

}
