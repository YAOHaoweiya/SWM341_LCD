#include "main.h"


/*********************************全局*****************************/
 UG_GUI gui;

#define LCD_HDOT	480		// 水平点数
#define LCD_VDOT	480		// 垂直点数
#define LCD_DIRH	1		// 水平显示？
 uint32_t *LCD_Buffer = (uint32_t *)SDRAMM_BASE;

 DMA2D_LayerSetting fgLayer, bgLayer, outLayer;

jpeg_outset_t jpeg_outset;//解码输出格式
 JPEG_InitStructure JPEG_initStruct;
 uint8_t  *jpeg_image_sdr = (uint8_t *)(SDRAMM_BASE + 0x100000);
 jfif_info_t jfif_info;

 FATFS fatfs;//要挂载的文件系统对象
 FIL filw,filr;//文件对象
 FILINFO fileInfo;//文件属性信息


/********************************************************************/


/**************************About avi 声明和定义*********************************/

 uint32_t rd;

 uint8_t jpeg_buf[512 * 1024] __attribute__((section(".bss.ARM.__at_0x80400000")));
/* 此处直接定义成2048，比较好的方法应该是根据avi文件头解析出的大小在堆中申请空间 */
#define PCM_BUFSIZE (2048)
 int16_t pcm_buf[PCM_BUFSIZE * 2]; // __attribute__((section(".bss.ARM.__at_0x80500000")));

/* ping pong buffer control structure */
typedef struct
{
    int16_t *buf;
    volatile uint8_t ping;
    volatile uint8_t pong;
} ppbuf_t;
 ppbuf_t ppbuf = {&pcm_buf[0], 0, 1}; //init ping-pong buffer

 extern volatile uint8_t frameup ;          //avi一帧结束标志
 extern volatile uint8_t dac_dma ;          //dac dma传输完成标志
/****************************Other_end*************************************/

void _HW_DrawPoint(UG_S16 x, UG_S16 y, UG_COLOR c)
{	
#if LCD_DIRH
	LCD_Buffer[y * LCD_HDOT + x] = c;
#else
	LCD_Buffer[(LCD_VDOT - x) * LCD_HDOT + y] = c;
#endif
}



/*****************************main***********************************/
/*****************************main***********************************/
int main(void)
{
//	uint32_t i, j;
//	FRESULT  res;//枚举类型，文件系操作返回码
//	uint32_t real_len;//存储实际读取的字节数
//	
//	DMA2D_InitStructure DMA2D_initStruct;
// 	
// 	SystemInit();		
//	SerialInit();
//	delay_init();  
//	
//	SDRAMInit();
//	screen_init();//spi 屏幕初始化
//	
//	RGBLCDInit();//屏幕引脚初始化，并开启开启背光
//	
//	LCD_Start(LCD);
//	
///*************DMA2D初始化和测试***************/
//	DMA2D_initStruct.Interval = CyclesPerUs;
//	DMA2D_initStruct.IntEOTEn = 1;
//	DMA2D_Init(&DMA2D_initStruct);
//	
//	test_PixelFill();
//	LCD_Start(LCD);
///*************DMA2D初始化和测试end*************/
//while(1){
///***********************测试图片解码*********************/
//	JPEG_initStruct.DoneIEn = 0;//解码完成中断使能选项
//	JPEG_initStruct.ErrorIEn = 1;//解码出错中断使能选项
//	JPEG_Init(JPEG, &JPEG_initStruct);
//	
//	memcpy(jpeg_image_sdr, jpeg_image1, sizeof(jpeg_image1));
//	jfif_parse(jpeg_image_sdr, sizeof(jpeg_image1), &jfif_info);
//	
//	jpeg_outset.format = JPEG_OUT_RGB565;
//	jpeg_outset.dither = 0;
//	jpeg_outset.RGBAddr = SDRAMM_BASE;
//	JPEG_Decode(JPEG, &jfif_info, &jpeg_outset);
//	LCD_Start(LCD);
///***********************测试图片解码end*********************/

///**********************从SD卡读取图片测试**************************/
//	for(int i = 0; i < SystemCoreClock/10; i++) __NOP();//等待SD卡完成初始化
//	res = f_mount(&fatfs, "sd:", 1);//（文件系统对象，路径【字符串】，挂载选项【立即挂载】）
//	if(res!=FR_OK) return 0;//挂载失败do what
//	
//	res=f_open(&filr,"sd:1.jpg",FA_READ);//以只读方式打开文件 参数：(文件对象，路径，方式)
//	if(res!=FR_OK) return 0;//如果失败,do what
//	
//	if(f_stat("sd:1.jpg",&fileInfo)==FR_OK)//获取文件信息，存储于fileInfo结构体
//		res=f_read(&filr,jpeg_image_sdr,fileInfo.fsize,&real_len);//（文件对象，SDRAM+1地址,文件大小,读取的实际大小）
//	if(res!=FR_OK) return 0; //如果失败，do what
//	f_close(&filr);
//	jfif_parse(jpeg_image_sdr,fileInfo.fsize, &jfif_info);
//	jpeg_outset.format = JPEG_OUT_RGB565;
//	jpeg_outset.dither = 0;
//	jpeg_outset.RGBAddr = SDRAMM_BASE;
//	JPEG_Decode(JPEG, &jfif_info, &jpeg_outset);
//	while(JPEG_DecodeBusy(JPEG)) __NOP();
//	LCD_Start(LCD);
//	
///**********************从SD卡读取图片测试end*************************/
//	
//	
//	

///****************************从SD卡读取视频测试******************************************************/
//	for(int i = 0; i < SystemCoreClock/10; i++) __NOP();//等待SD卡完成初始化
//	/***这里还需进行JPEG结构体初始化和输出格式设置，但是上文已做重复操作******/
////	JPEG_initStruct.DoneIEn = 0;
////   JPEG_initStruct.ErrorIEn = 1;
////   JPEG_Init(JPEG, &JPEG_initStruct);
////   jpeg_outset.format = JPEG_OUT_RGB565;
////   jpeg_outset.dither = 0;
////   jpeg_outset.RGBAddr = SDRAMM_BASE; //jpeg解码输出地址直接指向rgb显存地址
//	
//	res = f_mount(&fatfs, "sd:", 1);//（文件系统对象，路径【字符串】，挂载选项【立即挂载】）
//	if(res!=FR_OK) return 0;//挂载失败do what
//	/* 打开要播放的视频文件 */
//	
//    res = f_open(&filr, "sd:one.avi", FA_OPEN_EXISTING | FA_READ);
//    if (res != FR_OK)
//    {
//        printf("open file fail!\r\n");
//        while (1)
//            ;
//    }
//    /* 读取avi文件头 */
//    res = f_read(&filr, jpeg_buf, sizeof(jpeg_buf), &rd);
//    if (res != FR_OK)
//    {
//        printf("read file fial!\r\n");
//        while (1)
//            ;
//    }
//    /* 对文件头进行解析 */
//    if (avi_init((uint8_t *)jpeg_buf, rd))
//    {
//        printf("avi err:%d\r\n", res);
//    }

//    printf("fps:%d.\r\n", 1000 / (avix.SecPerFrame / 1000));
//    uint32_t offset = 0;
//    offset = avi_srarch_id((uint8_t *)jpeg_buf, rd, "movi"); //寻找movi ID
//    avi_get_streaminfo((uint8_t *)(jpeg_buf + offset + 4));  //获取流信息
//    f_lseek(&filr, offset + 12);                             //跳过标志ID,读地址偏移到流数据开始处

//    /* BTIMR0用于控制帧率，每avix.SecPerFrame 微秒中断一次 */
//    TIMR_Init(BTIMR0, TIMR_MODE_TIMER, CyclesPerUs, avix.SecPerFrame, 1);
//    TIMR_Start(BTIMR0);
//    /* TIMR0用于控制按音频采样率播放音频，每秒中断avix.SampleRate次 */
//    TIMR_Init(TIMR0, TIMR_MODE_TIMER, CyclesPerUs, 1000000 / avix.SampleRate, 0);

//    TIMR_Start(TIMR0);
//    dac_dma_init((uint32_t)&ppbuf.buf[ppbuf.ping * PCM_BUFSIZE], avix.AudioBufSize / 2);
//		
//    while (1 == 1)
//    {
//        if (avix.StreamID == AVI_VIDS_FLAG) //视频流
//        {
//            if (frameup == 1) //等待时间到达(在BTIMR0的中断里面设置为1)
//            {
//                frameup = 0;

//                f_read(&filr, jpeg_buf, avix.StreamSize + 8, &rd); //读入整帧+下一数据流ID信息
//                if (avix.StreamSize != 0)
//                {
//                    // uint32_t t_start = swm_gettick();

//                    /* 如果准备解码下一帧图片时，上一帧图片还没有解码完成，会在此等待，
//                       最好不要出现这种情况，如果出现这种情况，应降低播放视频的帧率 */
//                    jfif_parse(jpeg_buf, avix.StreamSize, &jfif_info);
//                    JPEG_Decode(JPEG, &jfif_info, &jpeg_outset);
//                    while(JPEG_DecodeBusy(JPEG)) __NOP();
//                    // printf("%d\r\n",swm_gettick() - t_start);
//                }

//                if (avi_get_streaminfo(jpeg_buf + avix.StreamSize)) //读取下一帧 流标志
//                {
//                    f_close(&filr);
//                    TIMR_Stop(BTIMR0);
//                    dac_deinit();
//                    printf("frame error \r\n");
//                    break;
//                }
//            }
//        }
//        else //音频流
//        {
//            if (dac_dma == 1) //等待上一帧音频播放完成
//            {
//                dac_dma = 0;

//                f_read(&filr, &ppbuf.buf[ppbuf.pong * PCM_BUFSIZE], avix.StreamSize, &rd);
//                /* avi文件中解析出的音频数据时int16_t，需要加上0x8000转换成uint16_t发送给dac */
//                if (avix.StreamSize != 0)
//                {
//                    for (uint32_t i = 0; i < avix.StreamSize / 2; i++)
//                    {
//                        ppbuf.buf[ppbuf.pong * PCM_BUFSIZE + i] += 32768;
//                    }
//                }
//                /* 由于上述转换过程，直接播放会有卡顿，切换ping和pong会有改善 */
//                ppbuf.ping = ppbuf.ping ^ ppbuf.pong;
//                ppbuf.pong = ppbuf.pong ^ ppbuf.ping;
//                ppbuf.ping = ppbuf.ping ^ ppbuf.pong;
//                DMA->CH[DMA_CH0].SRC = (uint32_t)&ppbuf.buf[ppbuf.ping * PCM_BUFSIZE];

//                uint8_t stream_buf[8];
//                f_read(&filr, stream_buf, 8, &rd);
//                if (avi_get_streaminfo(stream_buf)) //读取下一帧 流标志
//                {
//                    f_close(&filr);
//                    TIMR_Stop(BTIMR0);
//                    dac_deinit();
//                    printf("frame error \r\n");
//                    break;
//                }
//            }
//        }			
//    }
//	
//	
///****************************从SD卡读取视频测试end******************************************************/
//	}
///****************************************LVGL测试*****************************************************/		

	DMA2D_InitStructure DMA2D_initStruct;
 	
 	SystemInit();		
	SerialInit();
	systick_init();
	delay_init();  
	NVIC_SetPriorityGrouping(0);
	NVIC_SetPriority(LCD_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1, 0));

	SDRAMInit();
	screen_init();//spi 屏幕初始化
	
	lv_init();
	lv_port_disp_init();
	//lv_demo_stress();
	lv_ex_label();
		while(1)
		{		
			
			lv_timer_handler();			
		}
/****************************************LVGL测试end*************************************************/	
		
		
		
}

/**************************************************************************/
/*********************main_end*******************************************/

void SysTick_Handler_cb(void)
{
    lv_tick_inc(1);
}

void SysTick_Handler(void)
{
    swm_inctick();
    SysTick_Handler_cb();
}


void SDRAMInit(void)
{
	SDRAM_InitStructure SDRAM_InitStruct;
	
	PORT_Init(PORTM, PIN13, PORTM_PIN13_SDR_CLK, 0);
	PORT_Init(PORTM, PIN14, PORTM_PIN14_SDR_CKE, 0);
	PORT_Init(PORTB, PIN7,  PORTB_PIN7_SDR_WE,   0);
	PORT_Init(PORTB, PIN8,  PORTB_PIN8_SDR_CAS,  0);
	PORT_Init(PORTB, PIN9,  PORTB_PIN9_SDR_RAS,  0);
	PORT_Init(PORTB, PIN10, PORTB_PIN10_SDR_CS,  0);
	PORT_Init(PORTE, PIN15, PORTE_PIN15_SDR_BA0, 0);
	PORT_Init(PORTE, PIN14, PORTE_PIN14_SDR_BA1, 0);
	PORT_Init(PORTN, PIN14, PORTN_PIN14_SDR_A0,  0);
	PORT_Init(PORTN, PIN13, PORTN_PIN13_SDR_A1,  0);
	PORT_Init(PORTN, PIN12, PORTN_PIN12_SDR_A2,  0);
	PORT_Init(PORTN, PIN11, PORTN_PIN11_SDR_A3,  0);
	PORT_Init(PORTN, PIN10, PORTN_PIN10_SDR_A4,  0);
	PORT_Init(PORTN, PIN9,  PORTN_PIN9_SDR_A5,   0);
	PORT_Init(PORTN, PIN8,  PORTN_PIN8_SDR_A6,   0);
	PORT_Init(PORTN, PIN7,  PORTN_PIN7_SDR_A7,   0);
	PORT_Init(PORTN, PIN6,  PORTN_PIN6_SDR_A8,   0);
	PORT_Init(PORTN, PIN3,  PORTN_PIN3_SDR_A9,   0);
	PORT_Init(PORTN, PIN15, PORTN_PIN15_SDR_A10, 0);
	PORT_Init(PORTN, PIN2,  PORTN_PIN2_SDR_A11,  0);
	PORT_Init(PORTM, PIN15, PORTM_PIN15_SDR_A12, 0);
	PORT_Init(PORTE, PIN7,  PORTE_PIN7_SDR_D0,   1);
	PORT_Init(PORTE, PIN6,  PORTE_PIN6_SDR_D1,   1);
	PORT_Init(PORTE, PIN5,  PORTE_PIN5_SDR_D2,   1);
	PORT_Init(PORTE, PIN4,  PORTE_PIN4_SDR_D3,   1);
	PORT_Init(PORTE, PIN3,  PORTE_PIN3_SDR_D4,   1);
	PORT_Init(PORTE, PIN2,  PORTE_PIN2_SDR_D5,   1);
	PORT_Init(PORTE, PIN1,  PORTE_PIN1_SDR_D6,   1);
	PORT_Init(PORTE, PIN0,  PORTE_PIN0_SDR_D7,   1);
	PORT_Init(PORTE, PIN8,  PORTE_PIN8_SDR_D8,   1);
	PORT_Init(PORTE, PIN9,  PORTE_PIN9_SDR_D9,   1);
	PORT_Init(PORTE, PIN10, PORTE_PIN10_SDR_D10, 1);
	PORT_Init(PORTE, PIN11, PORTE_PIN11_SDR_D11, 1);
	PORT_Init(PORTE, PIN12, PORTE_PIN12_SDR_D12, 1);
	PORT_Init(PORTE, PIN13, PORTE_PIN13_SDR_D13, 1);
	PORT_Init(PORTC, PIN14, PORTC_PIN14_SDR_D14, 1);
	PORT_Init(PORTC, PIN15, PORTC_PIN15_SDR_D15, 1);
	PORT_Init(PORTB, PIN6,  PORTB_PIN6_SDR_LDQM, 0);
	PORT_Init(PORTM, PIN12, PORTM_PIN12_SDR_UDQM,0);
	
	SDRAM_InitStruct.Size = SDRAM_SIZE_8MB;
	SDRAM_InitStruct.ClkDiv = SDRAM_CLKDIV_1;
	SDRAM_InitStruct.CASLatency = SDRAM_CASLATENCY_3; // 2
	SDRAM_InitStruct.TimeTRP  = SDRAM_TRP_2;						//3
	SDRAM_InitStruct.TimeTRCD = SDRAM_TRCD_2;           //3
	SDRAM_InitStruct.TimeTRFC  = SDRAM_TRFC_9;         //15
	SDRAM_Init(&SDRAM_InitStruct);
}



void RGBLCDInit(void)
{

	LCD_InitStructure LCD_initStruct;
		
	PORT_Init(PORTB, PIN1,  PORTB_PIN1_LCD_B0,  0);
	PORT_Init(PORTB, PIN11, PORTB_PIN11_LCD_B1, 0);
	PORT_Init(PORTB, PIN13, PORTB_PIN13_LCD_B2, 0);
	PORT_Init(PORTB, PIN15, PORTB_PIN15_LCD_B3, 0);
	PORT_Init(PORTA, PIN2,  PORTA_PIN2_LCD_B4,  0);
	PORT_Init(PORTA, PIN9,  PORTA_PIN9_LCD_B5,  0);
	PORT_Init(PORTA, PIN10, PORTA_PIN10_LCD_B6, 0);
	PORT_Init(PORTA, PIN11, PORTA_PIN11_LCD_B7, 0);
	
	PORT_Init(PORTA, PIN12, PORTA_PIN12_LCD_G0, 0);
	PORT_Init(PORTA, PIN13, PORTA_PIN13_LCD_G1, 0);
	PORT_Init(PORTA, PIN14, PORTA_PIN14_LCD_G2, 0);
	PORT_Init(PORTA, PIN15, PORTA_PIN15_LCD_G3, 0);
	PORT_Init(PORTC, PIN0,  PORTC_PIN0_LCD_G4,  0);
	PORT_Init(PORTC, PIN1,  PORTC_PIN1_LCD_G5,  0);
	PORT_Init(PORTC, PIN2,  PORTC_PIN2_LCD_G6,  0);
	PORT_Init(PORTC, PIN3,  PORTC_PIN3_LCD_G7,  0);
	
	PORT_Init(PORTC, PIN4,  PORTC_PIN4_LCD_R0,  0);
	PORT_Init(PORTC, PIN5,  PORTC_PIN5_LCD_R1,  0);
	PORT_Init(PORTC, PIN8,  PORTC_PIN8_LCD_R2,  0);
	PORT_Init(PORTC, PIN9,  PORTC_PIN9_LCD_R3,  0);
	PORT_Init(PORTC, PIN10, PORTC_PIN10_LCD_R4, 0);
	PORT_Init(PORTC, PIN11, PORTC_PIN11_LCD_R5, 0);
	PORT_Init(PORTC, PIN12, PORTC_PIN12_LCD_R6, 0);
	PORT_Init(PORTC, PIN13, PORTC_PIN13_LCD_R7, 0);
	

	
	PORT_Init(PORTB, PIN2,  PORTB_PIN2_LCD_VSYNC, 0);
	PORT_Init(PORTB, PIN3,  PORTB_PIN3_LCD_HSYNC, 0);
	PORT_Init(PORTB, PIN4,  PORTB_PIN4_LCD_DEN,   0);
	PORT_Init(PORTB, PIN5,  PORTB_PIN5_LCD_DCLK,  0);
	
	GPIO_Init(GPIOD, PIN9, 1, 1, 0, 0);		//屏幕背光
	GPIO_SetBit(GPIOD, PIN9);
	
	LCD_initStruct.ClkDiv = 6;
	LCD_initStruct.Format = LCD_FMT_RGB565;
	LCD_initStruct.HnPixel = LCD_HDOT;
	LCD_initStruct.VnPixel = LCD_VDOT;
	LCD_initStruct.Hfp = 20;
	LCD_initStruct.Hbp = 56;
	LCD_initStruct.Vfp = 40;
	LCD_initStruct.Vbp = 60;
	LCD_initStruct.HsyncWidth = 8;
	LCD_initStruct.VsyncWidth = 10;
	LCD_initStruct.DataSource = (uint32_t)LCD_Buffer;
	LCD_initStruct.Background = 0xFFFF;
	LCD_initStruct.SampleEdge = LCD_SAMPLE_FALL;// LCD_SAMPLE_FALL LCD_SAMPLE_RISE
	LCD_initStruct.IntEOTEn = 1;
	LCD_Init(LCD, &LCD_initStruct);
}

void LCD_Handler(void)
{
	LCD_INTClr(LCD);
	
	LCD_Start(LCD);
}

void test_PixelFill(void)
{
	uint32_t i;
	
	/* 全屏填充蓝色 */
	outLayer.Address = (uint32_t)LCD_Buffer;
	outLayer.LineCount = LCD_VDOT;
	outLayer.LinePixel = LCD_HDOT;
	outLayer.LineOffset = 0;
	outLayer.ColorMode = DMA2D_FMT_RGB888;
	DMA2D_PixelFill(&outLayer, 0x001F);
	
	while(DMA2D_IsBusy()) __NOP();
	
	
}

void DMA2D_Handler(void)
{	
	DMA2D_INTClr();
}







void SerialInit(void)
{
	UART_InitStructure UART_initStruct;
	
	PORT_Init(PORTM, PIN0, PORTM_PIN0_UART0_RX, 1);		//GPIOM.0配置为UART0输入引脚
 	PORT_Init(PORTM, PIN1, PORTM_PIN1_UART0_TX, 0);		//GPIOM.1配置为UART0输出引脚
 	
 	UART_initStruct.Baudrate = 57600;
	UART_initStruct.DataBits = UART_DATA_8BIT;
	UART_initStruct.Parity = UART_PARITY_NONE;
	UART_initStruct.StopBits = UART_STOP_1BIT;
	UART_initStruct.RXThresholdIEn = 0;
	UART_initStruct.TXThresholdIEn = 0;
	UART_initStruct.TimeoutIEn = 0;
 	UART_Init(UART0, &UART_initStruct);
	UART_Open(UART0);
}
/****************************************************************************************************************************************** 
* 函数名称: fputc()
* 功能说明: printf()使用此函数完成实际的串口打印动作
* 输    入: int ch		要打印的字符
*			FILE *f		文件句柄
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
int fputc(int ch, FILE *f)
{
	UART_WriteByte(UART0, ch);
	
	while(UART_IsTXBusy(UART0));
	
 	
	return ch;
}