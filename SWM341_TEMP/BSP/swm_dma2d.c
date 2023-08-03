#include "main.h"

DMA2D_LayerSetting fgLayer, bgLayer, outLayer;

uint32_t *Img_Buffer = (uint32_t *)(SDRAMM_BASE + 0x100000);


/****************************************************************************************************************************************** 
* 函数名称: swm_dma2d_init(）
* 功能说明: swm单片机DMA2D初始化
* 输    入: 无
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void swm_dma2d_init(void)
{
	DMA2D_InitStructure DMA2D_initStruct;
	
	DMA2D_initStruct.Interval = CyclesPerUs;
	DMA2D_initStruct.IntEOTEn = 1;
	DMA2D_Init(&DMA2D_initStruct);
	
		/* 全屏填充蓝色 */
	outLayer.Address = (uint32_t)LCD_Buffer;
	outLayer.LineCount = LCD_VDOT;
	outLayer.LinePixel = LCD_HDOT;
	outLayer.LineOffset = 0;
	outLayer.ColorMode = DMA2D_FMT_RGB565;
	
	DMA2D_PixelFill(&outLayer, 0xF800);
	while(DMA2D_IsBusy()) __NOP();
	delay_ms(500);
	DMA2D_PixelFill(&outLayer, 0x07E0);
	while(DMA2D_IsBusy()) __NOP();
	delay_ms(500);
	DMA2D_PixelFill(&outLayer, 0x001F);
	while(DMA2D_IsBusy()) __NOP();
}

/****************************************************************************************************************************************** 
* 函数名称: dma2d_test_show(）
* 功能说明: swm单片机DMA2D测试
* 输    入: 无
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void dma2d_test_show(char *img_buf)
{	
	fgLayer.Address =(uint32_t)img_buf;
	fgLayer.LineOffset = 0;
	fgLayer.ColorMode = DMA2D_FMT_RGB565;
	while(DMA2D_IsBusy()) __NOP();
	
	/* 左上角绘制128*128图片 */
	outLayer.Address = (uint32_t)LCD_Buffer;
	outLayer.LineCount = 480;
	outLayer.LinePixel = 480;
	outLayer.LineOffset = LCD_HDOT-480;
	DMA2D_PixelMove(&fgLayer, &outLayer);
	
	while(DMA2D_IsBusy()) __NOP();
}
/****************************************************************************************************************************************** 
* 函数名称:	DMA2D_Handler()
* 功能说明: swm单片机DMA2D句柄
* 输    入: 无
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void DMA2D_Handler(void)
{	
	DMA2D_INTClr();
}
