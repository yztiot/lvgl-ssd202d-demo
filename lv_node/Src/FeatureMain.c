/*
 * @Description:功能区
 * @Author: mai
 * @LastEditors: chen
 * @Date: 2023-03-09 11:28:06
 * @LastEditTime: 2023-12-05 14:59:56
 * @FilePath: /Demo/lv_node/Src/FeatureMain.c
 */

#include "ui_app.h"

static const card_info_t CARD_INFO[] = {
    {40, 35,  SerialCard,       &Img_Port},      
    {250, 35, NetworkCard,      &Img_IP},      
    {460, 35, CreateGpioAndI2C, &Img_IO}, 
    {670, 35, WifiCard,         &Img_Wifi}, 
    {40, 310, StorageCard,      &Img_USB}, 
    {250, 310, TimeCard,        &Img_Time}, 
    {460, 310, AudioCard,        &Img_Audio}, 
    {670, 310, VideoCard,        &Img_Video}, 
    {40,  585, DrawingBoardCard, &Img_DrawingBoard}, 
    {250, 585, CanCard,          &Img_Can} 
};

static lv_obj_t *card_images[sizeof(CARD_INFO) / sizeof(CARD_INFO[0])];


static void add_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED)
    {
        lv_obj_t *img_obj = lv_event_get_target(e);
        for (int i = 0; i < sizeof(CARD_INFO) / sizeof(CARD_INFO[0]); i++)
        {
            if (img_obj ==   card_images[i])
            {
                CARD_INFO[i].card_func(); 
                break;
            }
        }
    }
}

void lv_feature_page(lv_obj_t *parent)
{
    for (int i = 0; i < sizeof(CARD_INFO) / sizeof(CARD_INFO[0]); i++)
    {
        lv_obj_t *card = card_create(parent);
        lv_obj_clear_flag(card, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_set_pos(card, CARD_INFO[i].x_pos, CARD_INFO[i].y_pos);
        card_images[i] = image_create(card, CARD_INFO[i].image,0 ,  0); 
        lv_obj_add_flag(card_images[i], LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_event_cb(card_images[i], add_event_cb, LV_EVENT_CLICKED, NULL);
       
    }
    create_recv_time(parent, &PuHuiTi_Regular_20, 350, -280);
}