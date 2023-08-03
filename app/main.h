#ifndef __MAIN_H__
#define __MAIN_H__

#include "SWM341.h"
#include "SWM341_timr.h"

#include "swm_delay.h"
#include "dev_systick.h"
#include "screen.h"
#include "board_init.h"

#include <string.h>
#include <stdint.h>

#include "ugui.h"
//#include "test.h"

#include "jfif_parser.h"
#include "ff.h"
#include "avi_parser.h"
#include "video_dma.h"

#include "lvgl.h"
//#include "lv_port_indev_template.h"
#include "lv_port_disp_template.h"
#include "lv_port_fs_template.h"
#include "lv_function1_test.h"
#include "lv_demo_stress.h"


/****************************º¯ÊýÉùÃ÷*************************************/

 void SerialInit(void);
 void SDRAMInit(void);
 void RGBLCDInit(void); 
 void LCD_Handler(void);

 void DMA2D_Handler(void);
 void test_PixelFill(void);

void SysTick_Handler_cb(void);
void SysTick_Handler(void);


/********************************************************************/






#endif //__MAIN_H__
