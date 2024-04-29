/*
 * @Description: 
 * @Author: chen
 * @Date: 2023-11-27 14:16:15
 * @LastEditors: chen
 * @LastEditTime: 2023-12-01 14:34:37
 */
/**
 * @file lv_demo_conf.h
 * Configuration file for v8.1.1-dev
 *
 */
/*
 * COPY THIS FILE AS lv_demo_conf.h
 */

/* clang-format off */
#if 1 /*Set it to "1" to enable the content*/

#ifndef LV_DEMO_CONF_H
#define LV_DEMO_CONF_H


/*******************
 * GENERAL SETTING
 *******************/
#define LV_EX_PRINTF       1       /*Enable printf-ing data in demoes and examples*/
#define LV_EX_KEYBOARD     1       /*Add PC keyboard support to some examples (`lv_drivers` repository is required)*/
#define LV_EX_MOUSEWHEEL   1       /*Add 'encoder' (mouse wheel) support to some examples (`lv_drivers` repository is required)*/

/*********************
 * DEMO USAGE
 *********************/

/*Show some widget*/
#define LV_USE_DEMO_WIDGETS       0
#if LV_USE_DEMO_WIDGETS
#define LV_DEMO_WIDGETS_SLIDESHOW  0
#endif

/*Printer demo, optimized for 800x480*/
#define LV_USE_DEMO_PRINTER     0

/*Demonstrate the usage of encoder and keyboard*/
#define LV_USE_DEMO_KEYPAD_AND_ENCODER     0

/*Benchmark your system*/
#define LV_USE_DEMO_BENCHMARK   0

/*Stress test for LVGL*/
#define LV_USE_DEMO_STRESS      1

/*Music player demo*/
#define LV_USE_DEMO_MUSIC      1
#if LV_USE_DEMO_MUSIC
# define LV_DEMO_MUSIC_SQUARE       1
# define LV_DEMO_MUSIC_LANDSCAPE    1
# define LV_DEMO_MUSIC_ROUND        1
# define LV_DEMO_MUSIC_LARGE        1
# define LV_DEMO_MUSIC_AUTO_PLAY    1
#endif

#endif /*LV_DEMO_CONF_H*/

#endif /*End of "Content enable"*/
