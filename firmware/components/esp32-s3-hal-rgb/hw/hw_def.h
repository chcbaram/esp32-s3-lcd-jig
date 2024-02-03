/*
 * hw_def.h
 *
 *  Created on: 2021. 1. 9.
 *      Author: baram
 */

#ifndef MAIN_HW_HW_DEF_H_
#define MAIN_HW_HW_DEF_H_


#include "def.h"
#include "bsp.h"



#define _DEF_FIRMWATRE_VERSION    "V240130R1"
#define _DEF_BOARD_NAME           "LCD-JIG-RGB"



#define _HW_DEF_RTOS_THREAD_PRI_CLI           5
#define _HW_DEF_RTOS_THREAD_PRI_LCD           5
#define _HW_DEF_RTOS_THREAD_PRI_GT911         5
#define _HW_DEF_RTOS_THREAD_PRI_FT6236        5

#define _HW_DEF_RTOS_THREAD_MEM_CLI           (8*1024)
#define _HW_DEF_RTOS_THREAD_MEM_LCD           (2*1024)
#define _HW_DEF_RTOS_THREAD_MEM_GT911         (4*1024)
#define _HW_DEF_RTOS_THREAD_MEM_FT6236        (2*1024)


#define _USE_HW_RTOS
#define _USE_HW_NVS
#define _USE_HW_CDC
#define _USE_HW_LCDC
#define _USE_HW_LVGL
#define _USE_HW_FT6236


#define _USE_HW_UART
#define      HW_UART_MAX_CH         2

#define _USE_HW_CLI
#define      HW_CLI_CMD_LIST_MAX    32
#define      HW_CLI_CMD_NAME_MAX    16
#define      HW_CLI_LINE_HIS_MAX    8
#define      HW_CLI_LINE_BUF_MAX    64

#define _USE_HW_CLI_GUI
#define      HW_CLI_GUI_WIDTH       80
#define      HW_CLI_GUI_HEIGHT      24

#define _USE_HW_LOG
#define      HW_LOG_CH              _DEF_UART2
#define      HW_LOG_BOOT_BUF_MAX    1024
#define      HW_LOG_LIST_BUF_MAX    1024

#define _USE_HW_I2C
#define      HW_I2C_MAX_CH          1

#define _USE_HW_TOUCH
#define      HW_TOUCH_MAX_CH        2

#define _USE_HW_GPIO
#define      HW_GPIO_MAX_CH         4

#define _USE_HW_PWM
#define      HW_PWM_MAX_CH          1

#define _USE_HW_LCDC
#define      HW_LCDC_CLK_MHZ        12
#define      HW_LCDC_FB_CNT         3

#define _USE_HW_ST7701
#define      HW_ST7701_WIDTH       320
#define      HW_ST7701_HEIGHT      480

#define _USE_HW_LCD
#define      HW_LCD_LVGL            1
#define      HW_LCD_LOGO            1
#define      HW_LCD_ROTATE          1
#define      HW_LCD_FB_CNT          HW_LCDC_FB_CNT

#if HW_LCD_ROTATE == 1
#define      HW_LCD_WIDTH           HW_ST7701_HEIGHT
#define      HW_LCD_HEIGHT          HW_ST7701_WIDTH
#else
#define      HW_LCD_WIDTH           HW_ST7701_WIDTH
#define      HW_LCD_HEIGHT          HW_ST7701_HEIGHT
#endif

#define _PIN_GPIO_LCD_TS_RST        1
#define _PIN_GPIO_LCD_BLK           2
#define _PIN_GPIO_LCD_TS_INT        0


#endif 
