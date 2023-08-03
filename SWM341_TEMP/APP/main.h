/**
 * @file main.h
 * @author lik
 * @brief 
 * @version 0.1
 * @date 2022-01-14
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __MAIN_H__
#define __MAIN_H__

#include "SWM341.h"

#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

/***********************BSP头文件****************************/
#include "swm_delay.h"
#include "swm_rgb_lcd.h"
#include "swm_usart.h"
#include "swm_dma2d.h"
/***********************LCD芯片选择****************************/
#if defined(KD028_LCD)
	#include "KD028_ST7701S.h"
#elif defined(KD034_LCD)
	#include "KD034_ST7701S.h"
#else
	//空
#endif
/************************************************************/

#endif //__MAIN_H__
