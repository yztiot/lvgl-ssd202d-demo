/*
 * @Author: Flying
 * @Date: 2022-05-14 19:37:28
 * @LastEditors: chen
 * @LastEditTime: 2023-12-01 14:27:16
 * @Description: 修改文件
 */
/**
 * @file lv_100ask_sketchpad.h
 *
 */

#ifndef LV_100ASK_SKETCHPAD_H
#define LV_100ASK_SKETCHPAD_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************
 *      INCLUDES
 *********************/
// #include "../../lv_lib_100ask.h"

// #if LV_100ASK_SKETCHPAD != 0
#include "ui_app.h"
    /*********************
     *      DEFINES
     *********************/

    /**********************
     *      TYPEDEFS
     **********************/
    typedef enum
    {
        LV_100ASK_SKETCHPAD_TOOLBAR_OPT_ALL = 0,
        LV_100ASK_SKETCHPAD_TOOLBAR_OPT_CW,
        LV_100ASK_SKETCHPAD_TOOLBAR_OPT_WIDTH,
        LV_100ASK_SKETCHPAD_TOOLBAR_OPT_LAST
    } lv_100ask_sketchpad_toolbar_t;

    /*Data of canvas*/
    typedef struct
    {
        lv_img_t img;
        lv_img_dsc_t dsc;
        lv_draw_line_dsc_t line_rect_dsc;
    } lv_100ask_sketchpad_t;

    /***********************
     * GLOBAL VARIABLES
     ***********************/

    /**********************
     * GLOBAL PROTOTYPES
     **********************/
    lv_obj_t *lv_100ask_sketchpad_create(lv_obj_t *parent);

    /*=====================
     * Setter functions
     *====================*/

    /*=====================
     * Getter functions
     *====================*/

    /*=====================
     * Other functions
     *====================*/

    /**********************
     *      MACROS
     **********************/

#endif /*LV_SKETCHPAD*/
#ifdef __cplusplus
} /*extern "C"*/
#endif

// #endif /*LV_SKETCHPAD_H*/