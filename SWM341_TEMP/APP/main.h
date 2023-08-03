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

/***********************BSPͷ�ļ�****************************/
#include "swm_delay.h"
#include "swm_rgb_lcd.h"
#include "swm_usart.h"
#include "swm_dma2d.h"
/***********************LCDоƬѡ��****************************/
#if defined(KD028_LCD)
	#include "KD028_ST7701S.h"
#elif defined(KD034_LCD)
	#include "KD034_ST7701S.h"
#else
	//��
#endif
/************************************************************/

#endif //__MAIN_H__
