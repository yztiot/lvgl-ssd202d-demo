#ifndef _LV_NODE_HOME_H
#define _LV_NODE_HOME_H

typedef struct
{
    lv_obj_t *display;
    lv_obj_t *feature;
    lv_obj_t *widgets;

} ui_main;
extern ui_main ui_main_page;
void create_lv_layout(lv_obj_t *scr);

#endif