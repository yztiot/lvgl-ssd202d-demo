#include "ui_app.h"

static WifiCredentials wifiCredentials;

static void BackParentEvent(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    if (code == LV_EVENT_CLICKED)
    {
        lv_obj_del(obj->parent->parent);
    }
}

static void on_confirm_button_click(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    lv_obj_t *parent = lv_event_get_user_data(e);

    if (code == LV_EVENT_CLICKED)
    {
        WifiNode *node = getWifiNodeFromListButton(parent);
        strncpy(wifiCredentials.password, lv_textarea_get_text(node->password_textarea), SSID_MAX_LENGTH - 1);
        lv_obj_del(obj->parent->parent);

        strcpy(node->password, wifiCredentials.password);// Save password
        
        connect_wifi(node); // Connect to the WIFI
    }
}

void CreateWifiKeyboard(lv_obj_t *parent)
{

    lv_obj_t *SrcParent = CreateMaskBox(lv_scr_act(), true);
    char WifiNameText[256];
    strncpy(wifiCredentials.ssid, GET_LABEL_TEXT(parent), SSID_MAX_LENGTH - 1);
    snprintf(WifiNameText, sizeof(WifiNameText), "输入“%s” 的密码", wifiCredentials.ssid);

    WifiNode *node = getWifiNodeFromListButton(parent);
    node->password_textarea = CreadWifiPasswordTextarea(SrcParent);

    lv_obj_t *Wifibackground = lv_obj_get_parent(node->password_textarea);
    CreateBackButtonAndText(Wifibackground, "确认", -230, 105, on_confirm_button_click, parent);
    CreateBackButtonAndText(Wifibackground, "取消", -110, 105, BackParentEvent, NULL);
    card_create_24_text(Wifibackground, "请输入WIFI密码: ", -390, -160);
    card_create_24_text(Wifibackground, WifiNameText, -20, -250);
}
