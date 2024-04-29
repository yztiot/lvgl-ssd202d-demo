/*
 * @Description: 
 * @Author: chen
 * @Date: 2023-11-07 16:14:29
 * @LastEditors: chen
 * @LastEditTime: 2023-12-18 15:34:14
 */

#ifndef _LV_UI_APP_H
#define _LV_UI_APP_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "time.h"
#include "lvgl/lvgl.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <pthread.h>
#include <regex.h>

/*硬件区*/
#include "lv_node/Hardware/Serial/serial_api.h"
#include "lv_node/Hardware/Serial/serial_define.h"
#include "lv_node/Hardware/Serial/serial_main.h"
#include "lv_node/Hardware/Serial/serial_mutex.h"
#include "lv_node/Hardware/WIFI/wifi_recv.h"
#include "lv_node/Hardware/WIFI/Wifi.h"
#include "lv_node/Hardware/Gpio/gpio_cfg.h"
#include "lv_node/Hardware/audio/user_audio.h"
#include "lv_node/Hardware/audio/frame_list.h"
#include "inc/mi_common_datatype.h"
#include "inc/mi_wlan.h"
#include "inc/mi_wlan_datatype.h"


/*资源区*/
#include "lv_node/Material/image/iamge.h"
#include "lv_node/Material/lv_100ask_pinyin_ime/lv_100ask_pinyin_ime.h"
#include "lv_node/Material/lv_100ask_sketchpad/lv_100ask_sketchpad.h"
#include "lv_node/Material/font/font.h"

/*功能区*/
#include "lv_node/Src/Common/common.h"
// #include "lv_node/Src/Common/keyboard.h"
#include "lv_node/Src/Common/parson.h"
#include "lv_node/Src/Feature/GPIO/GpioCard.h"
#include "lv_node/Src/Feature/Network/NetworkCard.h"
#include "lv_node/Src/Feature/Serial/SerialCard.h"
#include "lv_node/Src/Feature/WIFI/WifiCard.h"
#include "lv_node/Src/Feature/WIFI/WifiKeyboard.h"
#include "lv_node/Src/Feature/FeatureCommon.h"
#include "lv_node/Src/Feature/Storage/StorageCard.h"
#include "lv_node/Src/Feature/Audio/AudioCard.h"
#include "lv_node/Src/Feature/Audio/audio_list.h"
#include "lv_node/Src/Feature/Can/CanCard.h"
#include "lv_node/Src/Feature/DrawingBoard/DrawingBoardCard.h"
#include "lv_node/Src/Feature/Video/VideoCard.h"
#include "lv_node/Src/Feature/Video/mm_player.h"
#include "lv_node/Src/Feature/Time/TimeCard.h"
#include "lv_node/Src/Feature/Time/NTPServer.h"


/*页面区*/
#include "lv_node/Src/Widgets_page/calendar/app_calendar_1.h"
#include "lv_node/Src/Widgets_page/calendar/app_calendar.h"
#include "lv_node/Src/Widgets_page/checkbox/app_checkbox_1.h"
#include "lv_node/Src/Widgets_page/checkbox/app_checkbox_2.h"
#include "lv_node/Src/Widgets_page/checkbox/app_checkbox.h"
#include "lv_node/Src/Widgets_page/colorwheel/app_colorwheel_1.h"
#include "lv_node/Src/Widgets_page/colorwheel/app_colorwheel.h"
#include "lv_node/Src/Widgets_page/common/app_common.h"
#include "lv_node/Src/Widgets_page/game/tetris_logic/tetris_logic.h"
#include "lv_node/Src/Widgets_page/game/app_games.h"
#include "lv_node/Src/Widgets_page/list/app_list.h"
#include "lv_node/Src/Widgets_page/roller/app_roller_1.h"
#include "lv_node/Src/Widgets_page/roller/app_roller_2.h"
#include "lv_node/Src/Widgets_page/roller/app_roller_3.h"
#include "lv_node/Src/Widgets_page/roller/app_roller.h"
#include "lv_node/Src/Widgets_page/slider/app_slider_1.h"
#include "lv_node/Src/Widgets_page/slider/app_slider_2.h"
#include "lv_node/Src/Widgets_page/slider/app_slider_3.h"
#include "lv_node/Src/Widgets_page/slider/app_slider.h"
#include "lv_node/Src/Widgets_page/switch/app_switch_1.h"
#include "lv_node/Src/Widgets_page/switch/app_switch.h"

/*主页面*/
#include "lv_node/Src/FeatureMain.h"
#include "lv_node/Src/widgets_page.h"
#include "lv_node/ui_main.h"
#include "lv_node/Src/HomeMain.h"

// #include "CC0702I50R_1024x600.h"

// /*官方demo*/
// #include "lv_demos/src/lv_demo_music/lv_demo_music.h"


#endif